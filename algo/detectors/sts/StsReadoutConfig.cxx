/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "StsReadoutConfig.h"

#include "CbmStsAddress.h"

#include <cassert>
#include <iomanip>

using std::pair;
using std::setw;

namespace cbm::algo
{


  // ---  Constructor  ------------------------------------------------------------------
  StsReadoutConfig::StsReadoutConfig() { Init(); }
  // ------------------------------------------------------------------------------------


  // ---   Destructor   -----------------------------------------------------------------
  StsReadoutConfig::~StsReadoutConfig() {}
  // ------------------------------------------------------------------------------------


  // ---   Equipment IDs   --------------------------------------------------------------
  std::vector<uint16_t> StsReadoutConfig::GetEquipmentIds()
  {
    std::vector<uint16_t> result;
    for (auto& entry : fReadoutMap)
      result.push_back(entry.first);
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---   Number of elinks for a component / equipment   -------------------------------
  size_t StsReadoutConfig::GetNumElinks(uint16_t equipmentId)
  {
    size_t result = 0;
    auto it       = fReadoutMap.find(equipmentId);
    if (it != fReadoutMap.end()) result = fReadoutMap[equipmentId].size();
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---   Total number of elinks for STS   ---------------------------------------------
  size_t StsReadoutConfig::GetNumElinks()
  {
    size_t result = 0;
    for (auto& entry : fReadoutMap) {
      result += entry.second.size();
    }
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---  Initialise the mapping structure   --------------------------------------------
  void StsReadoutConfig::Init()
  {

    // This here refers to the mCBM 2021 setup.
    // Taken from CbmMcbm2018StsPar in combination with macro/beamtime/mcbm2021/mSts.par

    // The readout hierarchy is: component - CROB - FEB - ASIC (elink). Each elink
    // connects one ASIC. One FEB comprises 8 ASICs and reads out one side of a module (sensor).
    // In this setup, there is only one CROB per component. The code below is formulate such
    // as to support also multiple CROBs per component. In that case, the elinks are numbered
    // consecutively within one component.

    // Constants
    const uint16_t numModules       = 13;  // Number of modules in the setup
    const uint16_t numComp          = 5;   // Number of components
    const uint16_t numCrobPerComp   = 1;   // Number of CROBs per component
    const uint16_t numFebsPerCrob   = 5;   // Number of FEBs per CROB
    const uint16_t numAsicsPerFeb   = 8;   // Number of ASICs per FEB
    const uint16_t numElinksPerCrob = 42;  // Number of elinks per CROB

    // Equipment IDs for each component
    // This number is written to the data stream (MicrosliceDescriptor).
    uint16_t eqId[numComp] = {0x1003, 0x1002, 0x1006, 0x1005, 0x1004};

    // Mapping of eLink to FEB number within CROB. If -1, elink not used.
    // This mapping is the same for each component.
    const int16_t elink2Feb[numElinksPerCrob] = {4, 4, 4, 4, 4, 3, 3, 3, 4, 4, -1, -1, 4, 3, 2, 2, 3, 3, 3, 1, 2,
                                                 2, 1, 2, 2, 2, 3, 2, 1, 1, 1, 1,  1,  0, 0, 0, 1, 0, 0, 0, 0, 0};

    // Mapping of FEB within CROB to module index (-1 = inactive)
    int16_t feb2module[numComp][numCrobPerComp][numFebsPerCrob] = {
      {{-1, 1, 1, 0, 0}},     // component 0
      {{4, 3, 3, 2, 2}},      // component 1
      {{7, 6, 6, 5, 5}},      // component 2
      {{10, 9, 9, 8, 8}},     // component 3
      {{12, 12, 11, 11, 10}}  // component 4
    };

    // Mapping of FEB to module side (0 = p side, 1 = n side, -1 = inactive)
    int16_t feb2moduleSide[numComp][numCrobPerComp][numFebsPerCrob] = {
      {{-1, 1, 0, 1, 0}},  // component 0
      {{1, 1, 0, 1, 0}},   // component 1
      {{1, 1, 0, 1, 0}},   // component 2
      {{0, 1, 0, 1, 0}},   // component 3
      {{1, 0, 1, 0, 1}}    // component 4
    };

    // Mapping of eLink to ASIC number for FEB Type A
    // The ASIC number is a running number over all FEBs.
    const uint32_t elink2AsicFebA[numElinksPerCrob] = {
      0x0021, 0x0023, 0x0025, 0x0020, 0x0022, 0x0018, 0x001A, 0x001C, 0x0024, 0x0027, 0xFFFF, 0xFFFF, 0x0026, 0x001E,
      0x0010, 0x0012, 0x0019, 0x001B, 0x001F, 0x000E, 0x0011, 0x0013, 0x000C, 0x0015, 0x0017, 0x0016, 0x001D, 0x0014,
      0x0009, 0x000D, 0x000F, 0x0008, 0x000A, 0x0002, 0x0004, 0x0006, 0x000B, 0x0005, 0x0000, 0x0003, 0x0007, 0x0001,
    };

    // Mapping of eLink to ASIC for FEB Type B
    // The ASIC number is a running number over all FEBs.
    const uint32_t elink2AsicFebB[numElinksPerCrob] = {
      0x0027, 0x0025, 0x0023, 0x0026, 0x0024, 0x001E, 0x001C, 0x001A, 0x0022, 0x0021, 0xFFFF, 0xFFFF, 0x0020, 0x0018,
      0x0016, 0x0014, 0x001F, 0x001D, 0x0019, 0x0008, 0x0017, 0x0015, 0x000A, 0x0013, 0x0011, 0x0010, 0x001B, 0x0012,
      0x000F, 0x000B, 0x0009, 0x000E, 0x000C, 0x0004, 0x0002, 0x0000, 0x000D, 0x0003, 0x0006, 0x0005, 0x0001, 0x0007};

    // Module types
    // Type 0 had the connector at the right side, type 1 at the left side.
    // For type 0, the mapping of FEB to module side as above applies,
    // for type 1, it has to be inverted.
    bool modType[numModules] = {0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1};

    // Module addresses (CbmStsAddress)
    int32_t modAddress[numModules];
    modAddress[0]  = 0x10008002;
    modAddress[1]  = 0x10018002;
    modAddress[2]  = 0x10008402;
    modAddress[3]  = 0x10018402;
    modAddress[4]  = 0x10107C02;
    modAddress[5]  = 0x10008412;
    modAddress[6]  = 0x10018412;
    modAddress[7]  = 0x101FFC02;
    modAddress[8]  = 0x10008812;
    modAddress[9]  = 0x10018812;
    modAddress[10] = 0x10028812;
    modAddress[11] = 0x10008012;
    modAddress[12] = 0x10018012;


    // Constructing the map (equipmentId, eLink) -> (module, ASIC within module)
    uint16_t numElinksPerComp = numCrobPerComp * numElinksPerCrob;
    for (uint16_t comp = 0; comp < numComp; comp++) {
      uint16_t equipment = eqId[comp];
      fReadoutMap[equipment].resize(numElinksPerComp);
      for (uint16_t crob = 0; crob < numCrobPerComp; crob++) {
        for (uint16_t elink = 0; elink < numElinksPerCrob; elink++) {

          int32_t moduleAddress = -1;
          uint16_t asicInModule = 0;

          uint16_t elinkId = numElinksPerCrob * crob + elink;  // elink within component
          int16_t feb      = elink2Feb[elinkId];               // FEB within CROB
          if (feb != -1) {
            int16_t module = feb2module[comp][crob][feb];  // Module index
            moduleAddress  = modAddress[module];

            if (module != -1) {
              assert(module < numModules);
              bool moduleType    = modType[module];                               // 0 or 1
              int16_t moduleSide = feb2moduleSide[comp][crob][feb];               // 0 or 1, -1 is inactive
              int16_t febType    = (moduleType == 0 ? moduleSide : !moduleSide);  // 0 = FEB A, 1 = FEB B
              uint32_t asicIndex = (febType == 0 ? elink2AsicFebA[elinkId] : elink2AsicFebB[elinkId]);
              uint32_t asicInFeb = asicIndex % numAsicsPerFeb;  // ASIC number within FEB

              asicInModule = (moduleSide == 1 ? asicInFeb : asicInFeb + numAsicsPerFeb);
            }
          }
          fReadoutMap[equipment][elink] = std::make_pair(moduleAddress, asicInModule);

        }  //# elink
      }    //# CROB
    }      //# component
  }
  // ------------------------------------------------------------------------------------


  // ---  Mapping (equimentId, elink) -> (address, ASIC)  --------------------------------
  pair<int32_t, uint16_t> StsReadoutConfig::Map(uint16_t equipmentId, uint16_t elinkId)
  {
    std::pair<int32_t, uint16_t> result(-1, 0);
    auto equipIter = fReadoutMap.find(equipmentId);
    if (equipIter != fReadoutMap.end()) {
      if (elinkId < equipIter->second.size()) { result = equipIter->second.at(elinkId); }
    }
    return result;
  }
  // ------------------------------------------------------------------------------------


  // -----   Print readout map   ------------------------------------------------
  std::string StsReadoutConfig::PrintReadoutMap()
  {

    std::stringstream ss;
    for (auto& equipment : fReadoutMap) {
      auto eqId = equipment.first;
      for (size_t elink = 0; elink < equipment.second.size(); elink++) {
        auto address = equipment.second.at(elink).first;
        auto asicNr  = equipment.second.at(elink).second;
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


} /* namespace cbm::algo */
