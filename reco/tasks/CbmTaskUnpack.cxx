/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#include "CbmTaskUnpack.h"

#include "CbmDefs.h"
#include "CbmDigiBranchBase.h"
#include "CbmDigiEvent.h"
#include "CbmDigiManager.h"
#include "CbmDigiTimeslice.h"
#include "CbmSourceTs.h"

#include "MicrosliceDescriptor.hpp"

#include <FairLogger.h>
#include <FairRunOnline.h>

#include <TStopwatch.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "UnpackSts.h"


using namespace std;
using cbm::algo::UnpackStsElinkPar;
using cbm::algo::UnpackStsPar;


// -----   Constructor   -----------------------------------------------------
CbmTaskUnpack::CbmTaskUnpack() : FairTask("Unpack") {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTaskUnpack::~CbmTaskUnpack()
{
  if (fTimeslice) delete fTimeslice;
}
// ---------------------------------------------------------------------------


// -----   Execution   -------------------------------------------------------
void CbmTaskUnpack::Exec(Option_t*)
{

  // --- Reset output branch (CbmDigiTimeslice)
  fTimeslice->Clear();

  // --- Get FLES timeslice
  assert(fSource);
  fles::Timeslice* timeslice = fSource->GetTimeslice();
  assert(timeslice);

  // --- Timer and counters
  TStopwatch timer;
  TStopwatch compTimer;
  timer.Start();
  size_t numMs    = 0;
  size_t numDigis = 0;

  // --- Timeslice properties
  uint64_t tsIndex     = timeslice->index();
  uint64_t tsTime      = timeslice->start_time();
  uint64_t numComp     = timeslice->num_components();
  uint64_t numCompUsed = 0;

  LOG(info) << GetName() << ": TS " << tsIndex << " at t = " << tsTime << ", components " << numComp;

  // ---  Component loop
  for (uint64_t comp = 0; comp < numComp; comp++) {

    uint8_t systemId = timeslice->descriptor(comp, 0).sys_id;
    if (systemId == static_cast<uint8_t>(fles::SubsystemIdentifier::STS)) {

      // --- Component log
      size_t numDigisInComp = 0;
      compTimer.Start();

      // --- Microslice loop
      uint64_t numMsInComp = timeslice->num_microslices(comp);
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        auto msDescriptor = timeslice->descriptor(comp, mslice);
        auto msContent    = timeslice->content(comp, mslice);
        auto result       = fAlgoSts[comp](msContent, msDescriptor, tsTime);
        LOG(debug1) << GetName() << ": Component " << comp << ", microslice " << mslice << ", digis "
                    << result.first.size() << ", errors " << result.second.fNumNonHitOrTsbMessage << " | "
                    << result.second.fNumErrElinkOutOfRange << " | " << result.second.fNumErrInvalidFirstMessage
                    << " | " << result.second.fNumErrInvalidMsSize << " | " << result.second.fNumErrTimestampOverflow
                    << " | ";
        //std::move(result.first.begin(), result.first.end(), fTimeslice->fData.fSts.fDigis.end());
        // TODO: The above usage of std::move does not work (seg. fault). Would need advice.
        auto it = fTimeslice->fData.fSts.fDigis.end();
        fTimeslice->fData.fSts.fDigis.insert(it, result.first.begin(), result.first.end());
        numDigisInComp += result.first.size();
      }  //# microslice

      compTimer.Stop();
      LOG(info) << GetName() << ": Component " << comp << ", microslices " << numMsInComp << ", digis "
                << numDigisInComp << ", CPU time " << compTimer.CpuTime() * 1000. << " ms";
      numCompUsed++;
      numDigis += numDigisInComp;
      numMs += numMsInComp;

    }  //? system (only STS)

  }  //# component


  // --- Timeslice log
  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << " (index " << tsIndex << ")";
  logOut << ", components " << numCompUsed << " / " << numComp << ", microslices " << numMs;
  logOut << ", digis " << numDigis;
  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  fNumMs += numMs;
  fNumDigis += numDigis;
  fTime += timer.RealTime();
}
// ----------------------------------------------------------------------------


// -----   End-of-run action   ------------------------------------------------
void CbmTaskUnpack::Finish()
{

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Timeslices         : " << fNumTs;
  LOG(info) << "Microslices        : " << fNumMs;
  LOG(info) << "Digis              : " << fNumDigis;
  LOG(info) << "Time  / TS         : " << fixed << setprecision(2) << 1000. * fTime / double(fNumTs) << " ms";
  LOG(info) << "=====================================";
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
InitStatus CbmTaskUnpack::Init()
{

  std::cout << std::endl;
  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Init";

  // --- Get source instance
  fSource = dynamic_cast<CbmSourceTs*>(FairRunOnline::Instance()->GetSource());
  if (fSource == nullptr) {
    LOG(error) << GetName() << ": No valid source class registered!";
    return kFATAL;
  }
  LOG(info) << GetName() << ": Found CbmSourceTs instance";

  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Register output array (CbmDigiTimeslice)
  if (ioman->GetObject("DigiTimeslice")) {
    LOG(fatal) << GetName() << ": Branch DigiTimeslice already exists!";
    return kFATAL;
  }
  fTimeslice = new CbmDigiTimeslice();
  ioman->RegisterAny("DigiTimeslice.", fTimeslice, kTRUE);
  LOG(info) << GetName() << ": Registered branch DigiTimeslice.";

  // --- Initialise STS readout configuration
  //InitStsConfig();

  // --- Common parameters for all components
  uint32_t numChansPerAsic   = 128;  // R/O channels per ASIC
  uint32_t numAsicsPerModule = 16;   // Number of ASICs per module

  // Create one algorithm per component and configure it with parameters
  auto equipIds = fStsConfig.GetEquipmentIds();
  for (auto& equip : equipIds) {
    std::unique_ptr<UnpackStsPar> par(new UnpackStsPar());
    par->fNumChansPerAsic   = numChansPerAsic;
    par->fNumAsicsPerModule = numAsicsPerModule;
    size_t numElinks        = fStsConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackStsElinkPar elinkPar;
      auto mapEntry        = fStsConfig.Map(equip, elink);
      elinkPar.fAddress    = mapEntry.first;   // Module address for this elink
      elinkPar.fAsicNr     = mapEntry.second;  // ASIC number within module
      elinkPar.fTimeOffset = 0.;
      elinkPar.fAdcOffset  = 0.;
      elinkPar.fAdcGain    = 0.;
      // TODO: Add parameters for time and ADC calibration
      par->fElinkParams.push_back(elinkPar);
    }
    cbm::algo::UnpackSts algo;
    algo.SetParams(std::move(par));
    fAlgoSts.push_back(algo);
    LOG(info) << GetName() << ": configured equipment " << equip << " with " << numElinks << " elinks";
  }  //# equipments

  LOG(info) << GetName() << ": configured " << fAlgoSts.size() << " unpacker algorithms for STS.";
  LOG(info) << "==================================================";
  std::cout << std::endl;
  LOG(info) << "Readout map:" << fStsConfig.PrintReadoutMap();

  return kSUCCESS;
}
// ----------------------------------------------------------------------------


// -----   Set the STS readout configuration   --------------------------------
void CbmTaskUnpack::InitStsConfig()
{

  // This here refers to the mCBM 2021 setup. (from macro/beamtime/mcbm2021/mStsPar.par)
  // I have to confess that this is among the weirdest things I ever came across.

  // Constants
  const uint16_t numModules       = 13;  // Number of modules in the setup
  const uint16_t numElinksPerCrob = 42;  // Number of elinks per CROB
  const uint16_t numAsicsPerFeb   = 8;   // Number of ASICs per FEB

  // Module addresses and types
  // Type 0 means connector at the right side, type 1 has connector at the left
  int32_t modAddress[numModules];
  modAddress[0]            = 0x10008002;
  modAddress[1]            = 0x10018002;
  modAddress[2]            = 0x10008402;
  modAddress[3]            = 0x10018402;
  modAddress[4]            = 0x10107C02;
  modAddress[5]            = 0x10008412;
  modAddress[6]            = 0x10018412;
  modAddress[7]            = 0x101FFC02;
  modAddress[8]            = 0x10008012;
  modAddress[9]            = 0x10018012;
  modAddress[10]           = 0x10008812;
  modAddress[11]           = 0x10018812;
  modAddress[12]           = 0x10028812;
  bool modType[numModules] = {0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1};

  // Mapping of eLink to ASIC for FEB A
  const uint32_t elink2AsicFebA[numElinksPerCrob] = {
    0x0021, 0x0023, 0x0025, 0x0020, 0x0022, 0x0018, 0x001A, 0x001C, 0x0024, 0x0027, 0xFFFF, 0xFFFF, 0x0026, 0x001E,
    0x0010, 0x0012, 0x0019, 0x001B, 0x001F, 0x000E, 0x0011, 0x0013, 0x000C, 0x0015, 0x0017, 0x0016, 0x001D, 0x0014,
    0x0009, 0x000D, 0x000F, 0x0008, 0x000A, 0x0002, 0x0004, 0x0006, 0x000B, 0x0005, 0x0000, 0x0003, 0x0007, 0x0001,
  };

  // Mapping of eLink to ASIC for FEB B
  const uint32_t elink2AsicFebB[numElinksPerCrob] = {
    0x0027, 0x0025, 0x0023, 0x0026, 0x0024, 0x001E, 0x001C, 0x001A, 0x0022, 0x0021, 0xFFFF, 0xFFFF, 0x0020, 0x0018,
    0x0016, 0x0014, 0x001F, 0x001D, 0x0019, 0x0008, 0x0017, 0x0015, 0x000A, 0x0013, 0x0011, 0x0010, 0x001B, 0x0012,
    0x000F, 0x000B, 0x0009, 0x000E, 0x000C, 0x0004, 0x0002, 0x0000, 0x000D, 0x0003, 0x0006, 0x0005, 0x0001, 0x0007};

  // Mapping of eLink to FEB number within CROB. If -1, elink not used.
  const int16_t elink2Feb[numElinksPerCrob] = {4, 4, 4, 4, 4, 3, 3, 3, 4, 4, -1, -1, 4, 3, 2, 2, 3, 3, 3, 1, 2,
                                               2, 1, 2, 2, 2, 3, 2, 1, 1, 1, 1,  1,  0, 0, 0, 1, 0, 0, 0, 0, 0};


  // The readout hierarchy is: DPB (component) - CROB - FEB
  const uint16_t numComp = 5;  // Number of components
  const uint16_t numCrob = 1;  // Number of CROBs per component
  const uint16_t numFebs = 5;  // FEBs per CROB

  // Mapping of component to equipment ID
  uint16_t eqId[numComp] = {0x1003, 0x1002, 0x1006, 0x1005, 0x1004};

  // Mapping of FEB to module index (-1 = inactive)
  int16_t feb2module[numComp][numCrob][numFebs] = {
    {{-1, 1, 1, 0, 0}},     // component 0
    {{4, 3, 3, 2, 2}},      // component 1
    {{7, 6, 6, 5, 5}},      // component 2
    {{10, 9, 9, 8, 8}},     // component 3
    {{12, 12, 11, 11, 10}}  // component 4
  };

  // Mapping of FEB to module side (0 = p side, 1 = n side, -1 = inactive)
  int16_t feb2moduleSide[numComp][numCrob][numFebs] = {
    {{-1, 1, 0, 1, 0}},  // component 0
    {{1, 1, 0, 1, 0}},   // component 1
    {{1, 1, 0, 1, 0}},   // component 2
    {{0, 1, 0, 1, 0}},   // component 3
    {{1, 0, 1, 0, 1}}    // component 4
  };

  // Map (component, CROB, eLink) -> (module, ASIC within module)
  LOG(info) << "STS readout mapping: ";
  for (uint16_t comp = 0; comp < numComp; comp++) {
    uint16_t equipment = eqId[comp];
    for (uint16_t crob = 0; crob < numCrob; crob++) {
      for (uint16_t elink = 0; elink < numElinksPerCrob; elink++) {

        int32_t moduleAddress = -1;
        uint16_t asicInModule = 0;

        uint16_t elinkId = numElinksPerCrob * crob + elink;  // elink within component
        int16_t feb      = elink2Feb[elinkId];               // FEB within CROB
        if (feb != -1) {
          int16_t module = feb2module[comp][crob][feb];  // Module index
          if (module != -1) {
            assert(module < numModules);
            bool moduleType    = modType[module];                               // 0 or 1
            int16_t moduleSide = feb2moduleSide[comp][crob][feb];               // 0 or 1, -1 is inactive
            int16_t febType    = (moduleType == 0 ? moduleSide : !moduleSide);  // 0 = FEB A, 1 = FEB B
            uint32_t asicIndex = (febType == 0 ? elink2AsicFebA[elinkId] : elink2AsicFebB[elinkId]);
            uint32_t asicInFeb = asicIndex % numAsicsPerFeb;  // ASIC number within FEB

            moduleAddress = modAddress[module];
            asicInModule  = (moduleSide == 0 ? asicInFeb : asicInFeb + numAsicsPerFeb);
          }
        }
        fStsMap[equipment][elink] = std::make_pair(moduleAddress, asicInModule);

      }  //# elink
    }    //# CROB
  }      //# component
}
// ----------------------------------------------------------------------------


// -----   Print readout map   ------------------------------------------------
std::string CbmTaskUnpack::PrintReadoutMap()
{

  std::stringstream ss;
  for (auto& stsComp : fStsMap) {
    for (auto& stsElink : stsComp.second) {
      auto comp    = stsComp.first;
      auto elink   = stsElink.first;
      auto address = stsElink.second.first;
      auto asic    = stsElink.second.second;
      ss << "\n Component " << comp << "  elink " << setw(2) << elink;
      ss << "  ASIC " << setw(2) << asic << "  module " << address;
      ss << "  Unit " << setw(2) << CbmStsAddress::GetElementId(address, kStsUnit);
      ss << "  Ladd " << setw(2) << CbmStsAddress::GetElementId(address, kStsLadder);
      ss << "  Hlad " << setw(2) << CbmStsAddress::GetElementId(address, kStsHalfLadder);
      ss << "  Modu " << setw(2) << CbmStsAddress::GetElementId(address, kStsModule);
    }  //# elink
  }    //# component
  return ss.str();
}
// ----------------------------------------------------------------------------

ClassImp(CbmTaskUnpack)
