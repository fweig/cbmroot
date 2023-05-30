/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Felix Weiglhofer */
#include "ReadoutSetup.h"

#include "CbmStsAddress.h"

#include <cassert>
#include <iomanip>

using std::pair;
using std::setw;

using namespace cbm::algo;

// ---  Constructor  ------------------------------------------------------------------
sts::ReadoutMapping::ReadoutMapping(const ReadoutSetup& config) { Init(config); }
// ------------------------------------------------------------------------------------

// ---   Equipment IDs   --------------------------------------------------------------
std::vector<u16> sts::ReadoutMapping::GetEquipmentIds()
{
  std::vector<uint16_t> result;
  for (auto& entry : fReadoutMapping)
    result.push_back(entry.first);
  return result;
}
// ------------------------------------------------------------------------------------

// ---   Number of elinks for a component / equipment   -------------------------------
size_t sts::ReadoutMapping::GetNumElinks(u16 equipmentId)
{
  size_t result = 0;
  auto it       = fReadoutMapping.find(equipmentId);
  if (it != fReadoutMapping.end()) result = fReadoutMapping[equipmentId].size();
  return result;
}
// ------------------------------------------------------------------------------------

size_t sts::ReadoutMapping::GetNumElinks()
{
  size_t result = 0;
  for (auto& entry : fReadoutMapping) {
    result += entry.second.size();
  }
  return result;
}

// ---  Initialise the mapping structure   --------------------------------------------
void sts::ReadoutMapping::Init(const ReadoutSetup& config)
{
  // The readout hierarchy is: component - CROB - FEB - ASIC (elink). Each elink
  // connects one ASIC. One FEB comprises 8 ASICs and reads out one side of a module (sensor).
  // In this setup, there is only one CROB per component. The code below is formulate such
  // as to support also multiple CROBs per component. In that case, the elinks are numbered
  // consecutively within one component.

  // Constants
  const uint16_t numModules     = config.modules.size();                      // Number of modules in the setup
  const uint16_t numComp        = config.components.size();                   // Number of components
  const uint16_t numCrobPerComp = config.components.at(0).feb2module.size();  // Number of CROBs per component
  // const uint16_t numFebsPerCrob   = config.components.at(0).feb2module.at(0).size();  // Number of FEBs per CROB
  const uint16_t numAsicsPerFeb   = config.numAsicsPerFeb;  // Number of ASICs per FEB
  const uint16_t numElinksPerCrob = config.elinks.size();   // Number of elinks per CROB

  // Constructing the map (equipmentId, eLink) -> (module, ASIC within module)
  uint16_t numElinksPerComp = numCrobPerComp * numElinksPerCrob;
  for (uint16_t comp = 0; comp < numComp; comp++) {
    uint16_t equipment = config.components.at(comp).equipmentId;
    fReadoutMapping[equipment].resize(numElinksPerComp);
    for (uint16_t crob = 0; crob < numCrobPerComp; crob++) {
      for (uint16_t elink = 0; elink < numElinksPerCrob; elink++) {

        int32_t moduleAddress = -1;
        uint16_t asicInModule = 0;
        bool isPulser         = false;

        uint16_t elinkId = numElinksPerCrob * crob + elink;  // elink within component
        int16_t feb      = config.elinks.at(elinkId).toFeb;  // FEB within CROB
        if (feb != -1) {
          int16_t module = config.components[comp].feb2module[crob][feb];         // Module index
          isPulser       = config.components[comp].febIsPulser.at(crob).at(feb);  // Pulser flag

          if (module != -1) {
            assert(module < numModules);
            moduleAddress      = config.modules.at(module).address;                     // Module address
            bool moduleType    = config.modules.at(module).type;                        // 0 or 1
            int16_t moduleSide = config.components.at(comp).feb2moduleSide[crob][feb];  // 0 or 1, -1 is inactive
            int16_t febType    = (moduleType == 0 ? moduleSide : !moduleSide);          // 0 = FEB A, 1 = FEB B
            uint32_t asicIndex = (febType == 0 ? config.elinks[elinkId].toAsicFebA : config.elinks[elinkId].toAsicFebB);
            uint32_t asicInFeb = asicIndex % numAsicsPerFeb;  // ASIC number within FEB

            asicInModule = (moduleSide == 1 ? asicInFeb : asicInFeb + numAsicsPerFeb);
          }
        }
        fReadoutMapping[equipment][elink] = {moduleAddress, asicInModule, isPulser};

      }  //# elink
    }    //# CROB
  }      //# component
}
// ------------------------------------------------------------------------------------

// ---  Mapping (equimentId, elink) -> (address, ASIC)  --------------------------------
sts::ReadoutMapping::Entry sts::ReadoutMapping::Map(uint16_t equipmentId, uint16_t elinkId)
{
  Entry result {-1, 0, false};
  auto equipIter = fReadoutMapping.find(equipmentId);
  if (equipIter != fReadoutMapping.end()) {
    if (elinkId < equipIter->second.size()) { result = equipIter->second.at(elinkId); }
  }
  return result;
}
// ------------------------------------------------------------------------------------

// -----   Print readout map   ------------------------------------------------
std::string sts::ReadoutMapping::PrintReadoutMap()
{

  std::stringstream ss;
  for (auto& equipment : fReadoutMapping) {
    auto eqId = equipment.first;
    for (size_t elink = 0; elink < equipment.second.size(); elink++) {
      auto address = equipment.second.at(elink).moduleAddress;
      auto asicNr  = equipment.second.at(elink).asicNumber;
      ss << "\n Equipment " << eqId << "  elink " << setw(2) << elink;
      ss << "  ASIC " << setw(2) << asicNr << "  module " << address;
      if (address != -1) {
        ss << "  Unit " << setw(2) << CbmStsAddress::GetElementId(address, kStsUnit);
        ss << "  Ladd " << setw(2) << CbmStsAddress::GetElementId(address, kStsLadder);
        ss << "  Hlad " << setw(2) << CbmStsAddress::GetElementId(address, kStsHalfLadder);
        ss << "  Modu " << setw(2) << CbmStsAddress::GetElementId(address, kStsModule);
      }
      else
        ss << "  Inactive";
    }  //# elink
  }    //# component
  return ss.str();
}
// ----------------------------------------------------------------------------
