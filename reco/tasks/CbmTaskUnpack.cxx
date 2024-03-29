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
#include "CbmTrdParFasp.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdParSpadic.h"

#include "MicrosliceDescriptor.hpp"

#include <FairParAsciiFileIo.h>
#include <FairParamList.h>
#include <FairRunOnline.h>
#include <Logger.h>

#include <TStopwatch.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#ifdef WITH_EXECUTION
#include <execution>
#endif
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>

using namespace std;
using cbm::algo::UnpackBmonElinkPar;
using cbm::algo::UnpackBmonPar;
using cbm::algo::UnpackMuchElinkPar;
using cbm::algo::UnpackMuchPar;
using cbm::algo::UnpackStsElinkPar;
using cbm::algo::UnpackStsPar;
using cbm::algo::UnpackTofElinkPar;
using cbm::algo::UnpackTofPar;
using cbm::algo::UnpackTrd2dAsicPar;
using cbm::algo::UnpackTrd2dChannelPar;
using cbm::algo::UnpackTrd2dPar;
using cbm::algo::UnpackTrdCrobPar;
using cbm::algo::UnpackTrdElinkPar;
using cbm::algo::UnpackTrdPar;


// -----   Constructor   -----------------------------------------------------
CbmTaskUnpack::CbmTaskUnpack() : FairTask("Unpack") {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTaskUnpack::~CbmTaskUnpack()
{
  if (fTimeslice) delete fTimeslice;
}
// ---------------------------------------------------------------------------


// ----------------- Microslice loop ------------------------------------------
template<class Digi, class UnpackAlgo>
uint64_t CbmTaskUnpack::MsLoop(const fles::Timeslice* timeslice, UnpackAlgo& algo, const uint64_t comp,
                               std::vector<Digi>* digis, size_t* numBytesInComp, size_t* numDigisInComp)
{
  const uint64_t numMsInComp = timeslice->num_microslices(comp);

  for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
    const auto msDescriptor = timeslice->descriptor(comp, mslice);
    const auto msContent    = timeslice->content(comp, mslice);
    *numBytesInComp += msDescriptor.size;
    auto result = algo(msContent, msDescriptor, timeslice->start_time());
    LOG(debug1) << GetName() << ": Component " << comp << ", microslice " << mslice << ", digis " << result.first.size()
                << ", " << result.second.print();
    *numDigisInComp += result.first.size();
#pragma omp critical(insert_digis)
    digis->insert(digis->end(), result.first.begin(), result.first.end());
  }
  return numMsInComp;
}
// ----------------------------------------------------------------------------


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
  timer.Start();
  size_t numMs         = 0;
  size_t numBytes      = 0;
  size_t numDigis      = 0;
  uint64_t numCompUsed = 0;

  // ---  Component loop
#pragma omp parallel for schedule(dynamic) shared(timeslice) reduction(+ : numMs, numBytes, numDigis, numCompUsed)
  for (uint64_t comp = 0; comp < timeslice->num_components(); comp++) {

    // --- Component log
    size_t numBytesInComp = 0;
    size_t numDigisInComp = 0;
    uint64_t numMsInComp  = 0;

    TStopwatch compTimer;
    compTimer.Start();

    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice->descriptor(comp, 0).sys_id);

    // STS
    if (systemId == fles::SubsystemIdentifier::STS) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoSts.find(equipmentId);
      assert(algoIt != fAlgoSts.end());

      // The current algorithm works for the STS data format version 0x20 used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      assert(timeslice->descriptor(comp, 0).sys_ver == 0x20);

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fSts.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system STS

    // MUCH
    if (systemId == fles::SubsystemIdentifier::MUCH) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoMuch.find(equipmentId);
      assert(algoIt != fAlgoMuch.end());

      // The current algorithm works for the MUCH data format version 0x20 used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      assert(timeslice->descriptor(comp, 0).sys_ver == 0x20);

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fMuch.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system MUCH

    // TOF
    if (systemId == fles::SubsystemIdentifier::RPC) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoTof.find(equipmentId);
      assert(algoIt != fAlgoTof.end());

      // The current algorithm works for the TOF data format version XXXX used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      //assert(timeslice->descriptor(comp, 0).sys_ver == XXXX);

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fTof.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system TOF

    // Bmon
    if (systemId == fles::SubsystemIdentifier::T0) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoBmon.find(equipmentId);
      assert(algoIt != fAlgoBmon.end());

      // The current algorithm works for the T0 data format version XXXX used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      //assert(timeslice->descriptor(comp, 0).sys_ver == XXXX);

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fT0.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system T0

    // system TRD
    if (systemId == fles::SubsystemIdentifier::TRD) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoTrd.find(equipmentId);
      assert(algoIt != fAlgoTrd.end());

      // The current algorithm works for the TRD data format version XXX used in 2022.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      //assert(timeslice->descriptor(comp, 0).sys_ver == XXX);  To do: add something sensible here

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fTrd.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system TRD

    // system TRD2D
    if (systemId == fles::SubsystemIdentifier::TRD2D) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoTrd2d.find(equipmentId);
      assert(algoIt != fAlgoTrd2d.end());

      // The current algorithm works for the TRD2D data format version XXX used in 2022.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      //assert(timeslice->descriptor(comp, 0).sys_ver == XXX);  To do: add something sensible here

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fTrd2d.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system TRD2D


    compTimer.Stop();
    LOG(debug) << GetName() << ": Component " << comp << ", microslices " << numMsInComp << " input size "
               << numBytesInComp << " bytes, "
               << ", digis " << numDigisInComp << ", CPU time " << compTimer.CpuTime() * 1000. << " ms";

    numBytes += numBytesInComp;
    numDigis += numDigisInComp;
    numMs += numMsInComp;

  }  //# component

  // --- Sorting of output digis. Is required by both digi trigger and event builder.
#ifdef WITH_EXECUTION
  std::sort(std::execution::par_unseq, fTimeslice->fData.fSts.fDigis.begin(), fTimeslice->fData.fSts.fDigis.end(),
            [](CbmStsDigi digi1, CbmStsDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(std::execution::par_unseq, fTimeslice->fData.fMuch.fDigis.begin(), fTimeslice->fData.fMuch.fDigis.end(),
            [](CbmMuchDigi digi1, CbmMuchDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(std::execution::par_unseq, fTimeslice->fData.fTof.fDigis.begin(), fTimeslice->fData.fTof.fDigis.end(),
            [](CbmTofDigi digi1, CbmTofDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(std::execution::par_unseq, fTimeslice->fData.fT0.fDigis.begin(), fTimeslice->fData.fT0.fDigis.end(),
            [](CbmTofDigi digi1, CbmTofDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(std::execution::par_unseq, fTimeslice->fData.fTrd.fDigis.begin(), fTimeslice->fData.fTrd.fDigis.end(),
            [](CbmTrdDigi digi1, CbmTrdDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(std::execution::par_unseq, fTimeslice->fData.fTrd2d.fDigis.begin(), fTimeslice->fData.fTrd2d.fDigis.end(),
            [](CbmTrdDigi digi1, CbmTrdDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
#else
  std::sort(fTimeslice->fData.fSts.fDigis.begin(), fTimeslice->fData.fSts.fDigis.end(),
            [](CbmStsDigi digi1, CbmStsDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(fTimeslice->fData.fMuch.fDigis.begin(), fTimeslice->fData.fMuch.fDigis.end(),
            [](CbmMuchDigi digi1, CbmMuchDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(fTimeslice->fData.fTof.fDigis.begin(), fTimeslice->fData.fTof.fDigis.end(),
            [](CbmTofDigi digi1, CbmTofDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(fTimeslice->fData.fT0.fDigis.begin(), fTimeslice->fData.fT0.fDigis.end(),
            [](CbmTofDigi digi1, CbmTofDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(fTimeslice->fData.fTrd.fDigis.begin(), fTimeslice->fData.fTrd.fDigis.end(),
            [](CbmTrdDigi digi1, CbmTrdDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(fTimeslice->fData.fTrd2d.fDigis.begin(), fTimeslice->fData.fTrd2d.fDigis.end(),
            [](CbmTrdDigi digi1, CbmTrdDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
#endif

  // --- Timeslice log
  timer.Stop();
  stringstream logOut;
  logOut << setw(15) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << " (index " << timeslice->index() << ")";
  logOut << ", components " << numCompUsed << " / " << timeslice->num_components() << ", microslices " << numMs;
  logOut << ", input rate " << double(numBytes) / timer.RealTime() / 1.e6 << " MB/s";
  logOut << ", digis " << numDigis;
  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  fNumMs += numMs;
  fNumBytes += numBytes;
  fNumDigis += numDigis;
  fTime += timer.RealTime();
}
// ----------------------------------------------------------------------------


// -----   End-of-run action   ------------------------------------------------
void CbmTaskUnpack::Finish()
{
  double timePerTs = 1000. * fTime / double(fNumTs);  // in ms
  double rate      = fNumBytes / 1.e6 / fTime;        // in MB/s
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Timeslices     : " << fNumTs;
  LOG(info) << "Microslices    : " << fNumMs;
  LOG(info) << "Digis          : " << fNumDigis;
  LOG(info) << "Av. input rate : " << fixed << setprecision(2) << rate << " MB/s";
  LOG(info) << "Time / TS      : " << fixed << setprecision(2) << timePerTs << " ms";
  LOG(info) << "=====================================";
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
InitStatus CbmTaskUnpack::Init()
{

  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Initialising...";

  // --- Get source instance
  fSource = dynamic_cast<CbmSourceTs*>(FairRunOnline::Instance()->GetSource());
  if (fSource == nullptr) {
    LOG(error) << GetName() << ": No valid source class registered!";
    return kFATAL;
  }
  LOG(info) << "--- Found CbmSourceTs instance";

  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Register output array (CbmDigiTimeslice)
  if (ioman->GetObject("DigiTimeslice")) {
    LOG(fatal) << GetName() << ": Branch DigiTimeslice already exists!";
    return kFATAL;
  }
  fTimeslice = new CbmDigiTimeslice();
  ioman->RegisterAny("DigiTimeslice.", fTimeslice, IsOutputBranchPersistent("DigiTimeslice."));
  LOG(info) << "--- Registered branch DigiTimeslice.";

  // --- Common parameters for all components for STS
  uint32_t numChansPerAsicSts   = 128;  // R/O channels per ASIC for STS
  uint32_t numAsicsPerModuleSts = 16;   // Number of ASICs per module for STS

  // Create one algorithm per component for STS and configure it with parameters
  auto equipIdsSts = fStsConfig.GetEquipmentIds();
  for (auto& equip : equipIdsSts) {
    std::unique_ptr<UnpackStsPar> par(new UnpackStsPar());
    par->fNumChansPerAsic   = numChansPerAsicSts;
    par->fNumAsicsPerModule = numAsicsPerModuleSts;
    const size_t numElinks  = fStsConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackStsElinkPar elinkPar;
      auto mapEntry        = fStsConfig.Map(equip, elink);
      elinkPar.fAddress    = mapEntry.first;   // Module address for this elink
      elinkPar.fAsicNr     = mapEntry.second;  // ASIC number within module
      elinkPar.fTimeOffset = 0.;
      elinkPar.fAdcOffset  = 1.;
      elinkPar.fAdcGain    = 1.;
      // TODO: Add parameters for time and ADC calibration
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoSts[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }  //# equipments

  // Create one algorithm per component and configure it with parameters
  auto equipIdsMuch = fMuchConfig.GetEquipmentIds();
  for (auto& equip : equipIdsMuch) {
    std::unique_ptr<UnpackMuchPar> par(new UnpackMuchPar());
    const size_t numElinks = fMuchConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackMuchElinkPar elinkPar;
      elinkPar.fAddress    = fMuchConfig.Map(equip, elink);  // Vector of MUCH addresses for this elink
      elinkPar.fTimeOffset = 0.;
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoMuch[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }

  // Create one algorithm per component and configure it with parameters
  auto equipIdsTof = fTofConfig.GetEquipmentIds();
  for (auto& equip : equipIdsTof) {
    std::unique_ptr<UnpackTofPar> par(new UnpackTofPar());
    const size_t numElinks = fTofConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackTofElinkPar elinkPar;
      elinkPar.fChannelUId = fTofConfig.Map(equip, elink);  // Vector of TOF addresses for this elink
      elinkPar.fTimeOffset = 0.;
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoTof[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }

  // Create one algorithm per component and configure it with parameters
  auto equipIdsBmon = fBmonConfig.GetEquipmentIds();
  for (auto& equip : equipIdsBmon) {
    std::unique_ptr<UnpackBmonPar> par(new UnpackBmonPar());
    const size_t numElinks = fBmonConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackBmonElinkPar elinkPar;
      elinkPar.fChannelUId = fBmonConfig.Map(equip, elink);  // Vector of T0 addresses for this elink
      elinkPar.fTimeOffset = 0.;
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoBmon[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }

  InitTrdReadoutConfig();
  auto equipIdsTrd = fTrdConfig.GetEquipmentIds();
  for (auto& equip : equipIdsTrd) {

    std::unique_ptr<UnpackTrdPar> par(new UnpackTrdPar());
    const size_t numCrobs = fTrdConfig.GetNumCrobs(equip);

    for (size_t crob = 0; crob < numCrobs; crob++) {
      UnpackTrdCrobPar crobPar;
      const size_t numElinks = fTrdConfig.GetNumElinks(equip, crob);

      for (size_t elink = 0; elink < numElinks; elink++) {
        UnpackTrdElinkPar elinkPar;
        auto addresses        = fTrdConfig.Map(equip, crob, elink);
        elinkPar.fAddress     = addresses.first;   // Asic address for this elink
        elinkPar.fChanAddress = addresses.second;  // Channel addresses for this elink
        crobPar.fElinkParams.push_back(elinkPar);
      }
      par->fCrobParams.push_back(crobPar);
    }
    fAlgoTrd[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numCrobs << " crobs";
  }

  InitTrd2dReadoutConfig();
  auto equipIdsTrd2d = fTrd2dConfig.GetEquipmentIds();
  for (auto& equip : equipIdsTrd2d) {

    std::unique_ptr<UnpackTrd2dPar> par(new UnpackTrd2dPar());
    const size_t numAsics = fTrd2dConfig.GetNumAsics(equip);

    for (size_t asic = 0; asic < numAsics; asic++) {
      UnpackTrd2dAsicPar asicPar;
      const size_t numChans = fTrd2dConfig.GetNumChans(equip, asic);

      for (size_t chan = 0; chan < numChans; chan++) {
        UnpackTrd2dChannelPar chanPar;
        auto pars            = fTrd2dConfig.ChanMap(equip, asic, chan);
        chanPar.fPadAddress  = std::get<0>(pars);  // Pad address for channel
        chanPar.fHasPairingR = std::get<1>(pars);  // Flag for R or T compoment
        chanPar.fDaqOffset   = std::get<2>(pars);  // Time calibration parameter
        asicPar.fChanParams.push_back(chanPar);
      }
      auto comppars = fTrd2dConfig.CompMap(equip);
      par->fModId   = comppars.first;
      par->fCrobId  = comppars.second;
      par->fAsicParams.push_back(asicPar);
    }
    fAlgoTrd2d[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numAsics << " asics";
  }

  LOG(info) << "--- Configured " << fAlgoSts.size() << " unpacker algorithms for STS.";
  LOG(debug) << "Readout map:" << fStsConfig.PrintReadoutMap();
  LOG(info) << "--- Configured " << fAlgoMuch.size() << " unpacker algorithms for MUCH.";
  LOG(info) << "--- Configured " << fAlgoTof.size() << " unpacker algorithms for TOF.";
  LOG(info) << "--- Configured " << fAlgoBmon.size() << " unpacker algorithms for T0.";

  LOG(info) << "==================================================";

  return kSUCCESS;
}
// ----------------------------------------------------------------------------

// -----   Initialisation   ---------------------------------------------------
void CbmTaskUnpack::InitTrd2dReadoutConfig()
{
  // Initialize input files
  FairParAsciiFileIo asciiInput;
  std::string digiparfile = Form("%s/parameters/trd/trd_v22h_mcbm.digi.par", std::getenv("VMCWORKDIR"));
  std::string asicparfile = Form("%s/parameters/trd/trd_v22h_mcbm.asic.par", std::getenv("VMCWORKDIR"));

  // Read the .digi file and store result
  CbmTrdParSetDigi digiparset;
  if (asciiInput.open(digiparfile.data())) { digiparset.init(&asciiInput); }
  asciiInput.close();

  // Read the .asic file and store result
  CbmTrdParSetAsic asicparset;
  if (asciiInput.open(asicparfile.data())) { asicparset.init(&asciiInput); }
  asciiInput.close();

  // Initialize map (moduleId, crobId) -> (equipId) explicitly
  std::map<uint32_t, uint16_t[NCROBMOD]> crob_map;
  uint16_t cmap[] = {0xffc2, 0xffc5, 0xffc1, 0, 0};        // "crob map 22" for run Id >= 2335
  memcpy(crob_map[5], cmap, NCROBMOD * sizeof(uint16_t));  // only module Id 5 is used!

  // Then pass to Trd2dReadoutConfig, will invert to obain map (equipId) -> (module iq, crob id)
  fTrd2dConfig.InitComponentMap(crob_map);

  // FASP mapping update wrt the default setting (optional)
  std::map<uint32_t, uint8_t[NFASPMOD]> fasp_map;

  {  // Initialize the "fasp remapping"
    // Hard coded for run Id >= 2335 (only module Id 5).
    uint8_t map[NFASPMOD];
    for (uint32_t i(0); i < NFASPMOD; i++) {
      map[i] = i;
    }
    const size_t nfasp0 = 72;
    const size_t nfasps = 36;
    uint8_t map22[]     = {
      84,  85,  86,  87,  88,  89,   // FEB14/0xffc1
      90,  91,  92,  93,  94,  95,   // FEB17/0xffc1
      96,  97,  98,  99,  100, 101,  // FEB18/0xffc1
      102, 103, 104, 105, 106, 107,  // FEB16/0xffc1
      72,  73,  74,  75,  76,  77,   // FEB9/0xffc1
      78,  79,  80,  81,  82,  83    // FEB8/0xffc1
    };
    for (uint32_t i(0); i < nfasps; i++) {
      map[i + nfasp0] = map22[i];
    }
    memcpy(fasp_map[5], map, NFASPMOD * sizeof(uint8_t));
  }

  // Map (equipId, asicId, chanId) -> (pad address, R pairing flag, daq offset)
  std::map<size_t, std::map<size_t, std::map<size_t, std::tuple<int32_t, bool, uint64_t>>>> channelMap;

  // Loop through a list of module IDs from the .digi file (can in principle contradict crob_map).
  for (auto entry : digiparset.GetModuleMap()) {

    const auto moduleId = entry.first;
    if (crob_map.find(moduleId) == crob_map.end()) { continue; }  //skip if no entry in crob_map

    // Get ASIC parameters for this module
    const CbmTrdParSetAsic* setDet = static_cast<const CbmTrdParSetAsic*>(asicparset.GetModuleSet(moduleId));
    if (!setDet) continue;
    if (setDet->GetAsicType() != int32_t(CbmTrdDigi::eCbmTrdAsicType::kFASP)) continue;

    // Loop through ASICs for this module
    std::vector<int32_t> addresses;
    setDet->GetAsicAddresses(&addresses);
    for (auto add : addresses) {

      //Get local IDs for this component / equipment.
      const int32_t fasp_in_mod = add - 1000 * moduleId;
      const int32_t fasp_in_eq  = fasp_in_mod % (NFASPCROB);
      const int32_t crob_in_mod = fasp_in_mod / (NFASPCROB);
      const uint16_t eq_id      = crob_map[moduleId][crob_in_mod];

      // Apply FASP remapping (optional)
      const int32_t add_remap = fasp_map[moduleId][fasp_in_mod];
      add                     = add_remap + 1000 * moduleId;

      // ASIC parameter set
      CbmTrdParFasp* fasppar = (CbmTrdParFasp*) setDet->GetModulePar(add);

      // Loop through channels for this ASIC and fill map
      for (size_t chan = 0; chan < fasppar->GetNchannels(); chan++) {
        const int32_t pad              = fasppar->GetPadAddress(chan);
        const bool hasPairingR         = fasppar->GetChannel(chan)->HasPairingR();
        const CbmTrdParModDigi* modpar = (CbmTrdParModDigi*) digiparset.GetModulePar(moduleId);
        uint64_t daq_offset            = 0;
        if (modpar->GetPadRow(pad) % 2 == 0) daq_offset = 3;
        channelMap[eq_id][fasp_in_eq][chan] = std::make_tuple(pad, hasPairingR, daq_offset);
      }
    }
  }

  fTrd2dConfig.InitChannelMap(channelMap);
}

// -----   Initialisation   ---------------------------------------------------
void CbmTaskUnpack::InitTrdReadoutConfig()
{
  std::string trdparfile = Form("%s/parameters/trd/trd_v21d_mcbm.asic.par", std::getenv("VMCWORKDIR"));

  CbmTrdParSetAsic trdpar;

  FairParAsciiFileIo asciiInput;
  if (asciiInput.open(trdparfile.data())) { trdpar.init(&asciiInput); }

  FairParamList parlist;
  trdpar.putParams(&parlist);

  std::vector<int> moduleIds(trdpar.GetNrOfModules());
  parlist.fill("ModuleId", moduleIds.data(), moduleIds.size());

  std::map<size_t, std::map<size_t, std::map<size_t, size_t>>> addressMap;  //[criId][crobId][elinkId] -> asicAddress
  std::map<size_t, std::map<size_t, std::map<size_t, std::map<size_t, size_t>>>>
    channelMap;  //[criId][crobId][elinkId][chanId] -> chanAddress

  for (auto module : moduleIds) {
    CbmTrdParSetAsic* moduleSet = (CbmTrdParSetAsic*) trdpar.GetModuleSet(module);

    // Skip entries for "Fasp" modules in .asic.par file
    if (moduleSet->GetAsicType() != static_cast<int32_t>(CbmTrdDigi::eCbmTrdAsicType::kSPADIC)) continue;

    std::vector<int> asicAddresses;
    moduleSet->GetAsicAddresses(&asicAddresses);

    for (auto address : asicAddresses) {
      CbmTrdParSpadic* asicPar = (CbmTrdParSpadic*) moduleSet->GetAsicPar(address);
      const uint16_t criId     = asicPar->GetCriId();
      const uint8_t crobId     = asicPar->GetCrobId();
      const uint8_t elinkId    = asicPar->GetElinkId(0);
      if (elinkId >= 98) { continue; }  // Don't add not connected asics to the map
      addressMap[criId][crobId][elinkId]     = address;
      addressMap[criId][crobId][elinkId + 1] = address;

      const uint8_t numChans = 16;
      for (uint8_t chan = 0; chan < numChans; chan++) {
        auto asicChannelId                       = (elinkId % 2) == 0 ? chan : chan + numChans;
        auto chanAddr                            = asicPar->GetChannelAddresses().at(asicChannelId);
        channelMap[criId][crobId][elinkId][chan] = chanAddr;
      }
      for (uint8_t chan = 0; chan < numChans; chan++) {
        auto asicChannelId                           = (elinkId + 1 % 2) == 0 ? chan : chan + numChans;
        auto chanAddr                                = asicPar->GetChannelAddresses().at(asicChannelId);
        channelMap[criId][crobId][elinkId + 1][chan] = chanAddr;
      }
      LOG(debug) << "componentID " << asicPar->GetComponentId() << " "
                 << "address " << address << " key " << criId << " " << unsigned(crobId) << " " << unsigned(elinkId);
    }
  }
  fTrdConfig.Init(addressMap, channelMap);
  LOG(debug) << fTrdConfig.PrintReadoutMap();
}


ClassImp(CbmTaskUnpack)
