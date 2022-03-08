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

  // ---  Component loop
  for (uint64_t comp = 0; comp < numComp; comp++) {

    uint8_t systemId = timeslice->descriptor(comp, 0).sys_id;
    if (systemId == static_cast<uint8_t>(fles::SubsystemIdentifier::STS)) {
      uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      auto algoIt          = fAlgoSts.find(equipmentId);
      assert(algoIt != fAlgoSts.end());

      // --- Component log
      size_t numDigisInComp = 0;
      compTimer.Start();

      // --- Microslice loop
      uint64_t numMsInComp = timeslice->num_microslices(comp);
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        auto msDescriptor = timeslice->descriptor(comp, mslice);
        auto msContent    = timeslice->content(comp, mslice);
        auto result       = (algoIt->second)(msContent, msDescriptor, tsTime);
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
      LOG(debug) << GetName() << ": Component " << comp << ", microslices " << numMsInComp << ", digis "
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
    fAlgoSts[equip].SetParams(std::move(par));
    LOG(info) << GetName() << ": configured equipment " << equip << " with " << numElinks << " elinks";
  }  //# equipments

  LOG(info) << GetName() << ": configured " << fAlgoSts.size() << " unpacker algorithms for STS.";
  LOG(debug) << "Readout map:" << fStsConfig.PrintReadoutMap();
  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ----------------------------------------------------------------------------


ClassImp(CbmTaskUnpack)
