/* Copyright (C) 2007-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "CbmTaskBuildRawEvents.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmMatch.h"
#include "CbmSeedFinderSlidingWindow.h"

#include <FairLogger.h>
#include <FairRootManager.h>
#include <FairRunOnline.h>

#include <TClonesArray.h>
#include <TFile.h>
#include <TFolder.h>
#include <TH1.h>
#include <TH2.h>
#include <THttpServer.h>
#include <TStopwatch.h>

#include <iomanip>

CbmTaskBuildRawEvents::~CbmTaskBuildRawEvents()
{
  if (fpAlgo) delete fpAlgo;
  if (fSeedFinderSlidingWindow) delete fSeedFinderSlidingWindow;
  if (fSeedTimes) delete fSeedTimes;
  if (fTempDigiTimes) delete fTempDigiTimes;
  if (fTimer) delete fTimer;
  if (fCopyTimer) delete fCopyTimer;
}

CbmTaskBuildRawEvents::CbmTaskBuildRawEvents() : FairTask("BuildRawEvents")
{
  /// Create Algo. To be made generic/switchable when more event building algo are available!
  fpAlgo = new CbmAlgoBuildRawEvents();
}

void CbmTaskBuildRawEvents::AddSeedTimeFillerToList(RawEventBuilderDetector seedDet)
{
  fSeedTimeDetList.push_back(seedDet);

  if (fSeedTimes == nullptr) { fSeedTimes = new std::vector<Double_t>; }
  fpAlgo->SetSeedTimes(fSeedTimes);
}

void CbmTaskBuildRawEvents::SetIdealSeedFinder(const int32_t fileId)
{
  SetSlidingWindowSeedFinder(1, 0.0, 0.0, 0.0);
  fSeedFinderSlidingWindow->SetIdealMode(fileId);
}

void CbmTaskBuildRawEvents::SetSlidingWindowSeedFinder(int32_t minDigis, double dWindDur, double dDeadT, double dOffset)
{
  if (fSeedFinderSlidingWindow) {
    delete fSeedFinderSlidingWindow;
    fSeedFinderSlidingWindow = nullptr;
  }
  if (fSeedTimes == nullptr) { fSeedTimes = new std::vector<Double_t>; }
  fpAlgo->SetSeedTimes(fSeedTimes);

  fSeedFinderSlidingWindow = new CbmSeedFinderSlidingWindow(fSeedTimes, minDigis, dWindDur, dDeadT);
  fSeedFinderSlidingWindow->SetOffset(dOffset);
}

void CbmTaskBuildRawEvents::SetSeedFinderQa(Bool_t bFlagIn)
{
  if (bFlagIn == kTRUE) {
    if (fSeedFinderSlidingWindow == nullptr) {
      std::cout << "SetSeedFinderQa(): Cannot activate Qa when seed finder not active. Exiting." << std::endl;
      exit(1);
    }
    if (fvDigiMatchQa == nullptr) { fvDigiMatchQa = new std::vector<CbmMatch>; }
  }
  else  //kFALSE
  {
    if (fvDigiMatchQa != nullptr) { delete fvDigiMatchQa; }
    fvDigiMatchQa = nullptr;
  }
  if (fSeedFinderSlidingWindow != nullptr) fSeedFinderSlidingWindow->SetQa(bFlagIn);
}

InitStatus CbmTaskBuildRawEvents::Init()
{
  if (fbGetTimings) {
    fTimer = new TStopwatch;
    fTimer->Start();
    fCopyTimer = new TStopwatch;
    fCopyTimer->Reset();
  }

  /// Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  //T0 not included in digi manager.
  fT0Digis = ioman->InitObjectAs<std::vector<CbmTofDigi> const*>("T0Digi");
  if (!fT0Digis) { LOG(info) << "No T0 digi input."; }
  else {
    LOG(info) << "T0 digi input.";
    fpAlgo->SetT0Digis(fT0Digis);
  }

  // Get a pointer to the previous already existing data level
  fDigiMan = CbmDigiManager::Instance();
  if (fbUseMuchBeamtimeDigi) { fDigiMan->UseMuchBeamTimeDigi(); }
  fDigiMan->Init();

  //Init STS digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kSts)) { LOG(info) << "No STS digi input."; }
  else {
    LOG(info) << "STS digi input.";
    fStsDigis = new std::vector<CbmStsDigi>;
    fpAlgo->SetStsDigis(fStsDigis);
  }

  //Init MUCH digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kMuch)) { LOG(info) << "No MUCH digi input."; }
  else {
    LOG(info) << "MUCH digi input.";
    if (fbUseMuchBeamtimeDigi) {
      fMuchBeamTimeDigis = new std::vector<CbmMuchBeamTimeDigi>;
      fpAlgo->SetMuchBeamTimeDigis(fMuchBeamTimeDigis);
    }
    else {
      fMuchDigis = new std::vector<CbmMuchDigi>;
      fpAlgo->SetMuchDigis(fMuchDigis);
    }
  }

  //Init TRD digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kTrd)) { LOG(info) << "No TRD digi input."; }
  else {
    LOG(info) << "TRD digi input.";
    fTrdDigis = new std::vector<CbmTrdDigi>;
    fpAlgo->SetTrdDigis(fTrdDigis);
  }

  //Init TOF digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) { LOG(info) << "No TOF digi input."; }
  else {
    LOG(info) << "TOF digi input.";
    fTofDigis = new std::vector<CbmTofDigi>;
    fpAlgo->SetTofDigis(fTofDigis);
  }

  //Init RICH digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) { LOG(info) << "No RICH digi input."; }
  else {
    LOG(info) << "RICH digi input.";
    fRichDigis = new std::vector<CbmRichDigi>;
    fpAlgo->SetRichDigis(fRichDigis);
  }

  //Init PSD digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kPsd)) { LOG(info) << "No PSD digi input."; }
  else {
    LOG(info) << "PSD digi input.";
    fPsdDigis = new std::vector<CbmPsdDigi>;
    fpAlgo->SetPsdDigis(fPsdDigis);
  }

  /// Register output array (CbmEvent)
  fEvents = new TClonesArray("CbmEvent", 100);
  ioman->Register("CbmEvent", "Cbm_Event", fEvents, IsOutputBranchPersistent("CbmEvent"));
  if (!fEvents) LOG(fatal) << "Output branch was not created";

  // Set timeslice meta data
  fpAlgo->SetTimeSliceMetaDataArray(dynamic_cast<TClonesArray*>(ioman->GetObject("TimesliceMetaData")));

  if (fTimer != nullptr) {
    fTimer->Stop();
    Double_t rtime = fTimer->RealTime();
    Double_t ctime = fTimer->CpuTime();
    LOG(info) << "CbmTaskBuildRawEvents::Init(): Real time " << rtime << " s, CPU time " << ctime << " s";
  }

  // Init seed finder
  if (fSeedFinderSlidingWindow) { fSeedFinderSlidingWindow->Init(); }

  /// Call Algo Init method
  if (kTRUE == fpAlgo->InitAlgo()) return kSUCCESS;
  else
    return kFATAL;
}

InitStatus CbmTaskBuildRawEvents::ReInit() { return kSUCCESS; }

void CbmTaskBuildRawEvents::Exec(Option_t* /*option*/)
{
  if (fTimer != nullptr) { fTimer->Start(kFALSE); }
  TStopwatch timer;
  timer.Start();

  LOG(debug2) << "CbmTaskBuildRawEvents::Exec => Starting sequence";
  //Warning: Int_t must be used for the loop counters instead of UInt_t,
  //as the digi manager can return -1, which would be casted to +1
  //during comparison, leading to an error.

  //Reset explicit seed times if set
  if (fSeedTimes != nullptr) { fSeedTimes->clear(); }

  if (fCopyTimer != nullptr) { fCopyTimer->Start(kFALSE); }

  //Read STS digis
  if (fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    fStsDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kSts); i++) {
      const CbmStsDigi* Digi = fDigiMan->Get<CbmStsDigi>(i);
      fStsDigis->push_back(*Digi);
    }
    LOG(debug) << "Read: " << fStsDigis->size() << " STS digis.";
    LOG(debug) << "In DigiManager: " << fDigiMan->GetNofDigis(ECbmModuleId::kSts) << " STS digis.";
  }

  //Read MUCH digis
  if (fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    if (fbUseMuchBeamtimeDigi) {
      fMuchBeamTimeDigis->clear();
      for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kMuch); i++) {
        const CbmMuchBeamTimeDigi* Digi = fDigiMan->Get<CbmMuchBeamTimeDigi>(i);
        fMuchBeamTimeDigis->push_back(*Digi);
      }
      LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kMuch) << " MUCH digis.";
      LOG(debug) << "In DigiManager: " << fMuchBeamTimeDigis->size() << " MUCH digis.";
    }
    else {
      fMuchDigis->clear();
      for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kMuch); i++) {
        const CbmMuchDigi* Digi = fDigiMan->Get<CbmMuchDigi>(i);
        fMuchDigis->push_back(*Digi);
      }
      LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kMuch) << " MUCH digis.";
      LOG(debug) << "In DigiManager: " << fMuchDigis->size() << " MUCH digis.";
    }
  }

  //Read TRD digis
  if (fDigiMan->IsPresent(ECbmModuleId::kTrd)) {
    fTrdDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kTrd); i++) {
      const CbmTrdDigi* Digi = fDigiMan->Get<CbmTrdDigi>(i);
      fTrdDigis->push_back(*Digi);
    }
    LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kTrd) << " TRD digis.";
    LOG(debug) << "In DigiManager: " << fTrdDigis->size() << " TRD digis.";
  }

  //Read TOF digis
  if (fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    fTofDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kTof); i++) {
      const CbmTofDigi* Digi = fDigiMan->Get<CbmTofDigi>(i);
      fTofDigis->push_back(*Digi);
    }
    LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kTof) << " TOF digis.";
    LOG(debug) << "In DigiManager: " << fTofDigis->size() << " TOF digis.";
  }

  //Read RICH digis
  if (fDigiMan->IsPresent(ECbmModuleId::kRich)) {
    fRichDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kRich); i++) {
      const CbmRichDigi* Digi = fDigiMan->Get<CbmRichDigi>(i);
      fRichDigis->push_back(*Digi);
    }
    LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kRich) << " RICH digis.";
    LOG(debug) << "In DigiManager: " << fRichDigis->size() << " RICH digis.";
  }

  //Read PSD digis
  if (fDigiMan->IsPresent(ECbmModuleId::kPsd)) {
    fPsdDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kPsd); i++) {
      const CbmPsdDigi* Digi = fDigiMan->Get<CbmPsdDigi>(i);
      fPsdDigis->push_back(*Digi);
    }
    LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kPsd) << " PSD digis.";
    LOG(debug) << "In DigiManager: " << fPsdDigis->size() << " PSD digis.";
  }

  if (fCopyTimer != nullptr) { fCopyTimer->Stop(); }

  if (fSeedFinderSlidingWindow != nullptr) { FillSeedTimesFromSlidingWindow(); }
  else if (fSeedTimeDetList.size() > 0) {
    FillSeedTimesFromDetList(fSeedTimes);
  }

  //DumpSeedTimesFromDetList();

  /// Call Algo ProcessTs method
  fpAlgo->ProcessTs();

  /// Save the resulting vector of events in TClonesArray
  FillOutput();
  LOG(debug2) << "CbmTaskBuildRawEvents::Exec => Done";

  if (fTimer != nullptr) { fTimer->Stop(); }

  // --- Timeslice log and statistics
  timer.Stop();
  std::stringstream logOut;
  logOut << std::setw(20) << std::left << GetName() << " [";
  logOut << std::fixed << std::setw(8) << std::setprecision(1) << std::right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNofTs;
  if (fEvents) logOut << ", events " << fEvents->GetEntriesFast();
  LOG(info) << logOut.str();
  if (fSeedFinderSlidingWindow) {
    const size_t seedCount = fSeedFinderSlidingWindow->GetNofSeeds();
    LOG(info) << seedCount << " trigger(s) for this TS.";
    fTotalSeedCount += seedCount;
  }
  fNofTs++;
  fNofEvents += fEvents->GetEntriesFast();
  fTime += timer.RealTime();
}

void CbmTaskBuildRawEvents::FillSeedTimesFromDetList(std::vector<Double_t>* vdSeedTimes,
                                                     std::vector<CbmMatch>* vDigiMatch)
{
  std::map<ECbmModuleId, UInt_t> DigiNumbers;
  std::map<ECbmModuleId, UInt_t> DigiCounters;
  vdSeedTimes->clear();

  if (vDigiMatch != nullptr) vDigiMatch->clear();

  for (RawEventBuilderDetector& system : fSeedTimeDetList) {
    DigiNumbers[system.detId]  = GetNofDigis(system.detId);
    DigiCounters[system.detId] = 0;
  }

  do {
    ECbmModuleId nextAddedSystem;
    Double_t earliestTime = -1;

    for (RawEventBuilderDetector& system : fSeedTimeDetList) {
      if (DigiCounters[system.detId] < DigiNumbers[system.detId]) {

        // filter T0 digis from Tof (remove this statement if T0 properly implemented)
        if (system.detId == ECbmModuleId::kTof
            && (fTofDigis->at(DigiCounters[system.detId])).GetAddress() == CbmAlgoBuildRawEvents::fuT0Address) {
          DigiCounters[system.detId]++;
          continue;
        }  // end of T0 filter

        Double_t thisTime = GetDigiTime(system.detId, DigiCounters[system.detId]);
        if (thisTime < earliestTime || earliestTime == -1) {
          nextAddedSystem = system.detId;
          earliestTime    = thisTime;
        }
      }
    }
    if (earliestTime != -1) {

      if (vDigiMatch != nullptr) {
        const CbmMatch* digiMatch = fDigiMan->GetMatch(nextAddedSystem, DigiCounters[nextAddedSystem]);
        vDigiMatch->push_back(*digiMatch);
      }
      vdSeedTimes->push_back(earliestTime);
      DigiCounters[nextAddedSystem]++;
    }
    else {
      break;
    }
  } while (true);
}

void CbmTaskBuildRawEvents::FillSeedTimesFromSlidingWindow()
{
  if (fSeedTimeDetList.size() == 0) {
    if (fSeedFinderSlidingWindow->IsIdealMode()) {
      fSeedFinderSlidingWindow->FillSeedTimes();
      return;
    }
    else {
      std::cout << "FillSeedTimesFromSlidingWindow(): Error, seed detector list empty." << std::endl;
      exit(1);
    }
  }
  if (fSeedTimeDetList.size() == 1) {
    const RawEventBuilderDetector seedDet = fSeedTimeDetList.at(0);
    FillSeedTimesFromSlidingWindow(&seedDet);
  }
  else {  // more than one seed detector
    if (!fTempDigiTimes) { fTempDigiTimes = new std::vector<Double_t>; }
    FillSeedTimesFromDetList(fTempDigiTimes, fvDigiMatchQa);
    fSeedFinderSlidingWindow->FillSeedTimes(fTempDigiTimes, fvDigiMatchQa);
  }
}

void CbmTaskBuildRawEvents::FillSeedTimesFromSlidingWindow(const RawEventBuilderDetector* seedDet)
{
  if (fvDigiMatchQa != nullptr) {
    if (!fDigiMan->IsMatchPresent(seedDet->detId)) {
      std::cout << "FillSeedTimesFromSlidingWindow(): Error, QA set but no CbmMatch found for seed detector."
                << std::endl;
      exit(1);
    }
    fvDigiMatchQa->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(seedDet->detId); i++) {

      // filter T0 digis from Tof (remove this statement if T0 properly implemented)
      if (seedDet->detId == ECbmModuleId::kTof
          && (fTofDigis->at(i)).GetAddress() == CbmAlgoBuildRawEvents::fuT0Address) {
        continue;
      }  // end of T0 filter

      const CbmMatch* digiMatch = fDigiMan->GetMatch(seedDet->detId, i);
      fvDigiMatchQa->push_back(*digiMatch);
    }
  }

  switch (seedDet->detId) {
    case ECbmModuleId::kMuch:
      if (fbUseMuchBeamtimeDigi) {
        fSeedFinderSlidingWindow->FillSeedTimes(fMuchBeamTimeDigis, fvDigiMatchQa);
        break;
      }
      else {
        fSeedFinderSlidingWindow->FillSeedTimes(fMuchDigis, fvDigiMatchQa);
        break;
      }
    case ECbmModuleId::kSts: fSeedFinderSlidingWindow->FillSeedTimes(fStsDigis, fvDigiMatchQa); break;
    case ECbmModuleId::kTrd: fSeedFinderSlidingWindow->FillSeedTimes(fTrdDigis, fvDigiMatchQa); break;
    case ECbmModuleId::kTof: {

      // filter T0 digis from Tof (remove this statement if T0 properly implemented)
      std::vector<CbmTofDigi> vFilteredTofDigis;
      for (const auto& tofDigi : *fTofDigis) {
        if (tofDigi.GetAddress() == CbmAlgoBuildRawEvents::fuT0Address) { continue; }
        vFilteredTofDigis.push_back(tofDigi);
      }
      fSeedFinderSlidingWindow->FillSeedTimes(&vFilteredTofDigis, fvDigiMatchQa);
      break;
      // end of T0 filter

      //original version (no T0 filter)
      //fSeedFinderSlidingWindow->FillSeedTimes(fTofDigis, fvDigiMatchQa); break;
    }
    case ECbmModuleId::kRich: fSeedFinderSlidingWindow->FillSeedTimes(fRichDigis, fvDigiMatchQa); break;
    case ECbmModuleId::kPsd: fSeedFinderSlidingWindow->FillSeedTimes(fPsdDigis, fvDigiMatchQa); break;
    case ECbmModuleId::kT0: fSeedFinderSlidingWindow->FillSeedTimes(fT0Digis, fvDigiMatchQa); break;
    default: break;
  }
}

Double_t CbmTaskBuildRawEvents::GetDigiTime(ECbmModuleId _system, UInt_t _entry)
{
  switch (_system) {
    case ECbmModuleId::kMuch:
      if (fbUseMuchBeamtimeDigi) { return (fMuchBeamTimeDigis->at(_entry)).GetTime(); }
      else {
        return (fMuchDigis->at(_entry)).GetTime();
      }
    case ECbmModuleId::kSts: return (fStsDigis->at(_entry)).GetTime();
    case ECbmModuleId::kTrd: return (fTrdDigis->at(_entry)).GetTime();
    case ECbmModuleId::kTof: return (fTofDigis->at(_entry)).GetTime();
    case ECbmModuleId::kRich: return (fRichDigis->at(_entry)).GetTime();
    case ECbmModuleId::kPsd: return (fPsdDigis->at(_entry)).GetTime();
    case ECbmModuleId::kT0: return (fT0Digis->at(_entry)).GetTime();
    default: break;
  }
  return -1;
}

UInt_t CbmTaskBuildRawEvents::GetNofDigis(ECbmModuleId _system)
{
  switch (_system) {
    case ECbmModuleId::kMuch:
      if (fbUseMuchBeamtimeDigi) { return fMuchBeamTimeDigis->size(); }
      else {
        return fMuchDigis->size();
      }
    case ECbmModuleId::kSts: return fStsDigis->size();
    case ECbmModuleId::kTrd: return fTrdDigis->size();
    case ECbmModuleId::kTof: return fTofDigis->size();
    case ECbmModuleId::kRich: return fRichDigis->size();
    case ECbmModuleId::kPsd: return fPsdDigis->size();
    case ECbmModuleId::kT0: return fT0Digis->size();
    default: break;
  }
  return 0;
}

void CbmTaskBuildRawEvents::PrintTimings()
{
  if (fTimer == nullptr) { LOG(fatal) << "Trying to print timings but timer not set"; }
  else {
    Double_t rtime = fTimer->RealTime();
    Double_t ctime = fTimer->CpuTime();
    LOG(info) << "CbmTaskBuildRawEvents: Real time " << rtime << " s, CPU time " << ctime << " s";
  }
  if (fCopyTimer == nullptr) { LOG(fatal) << "Trying to print timings but timer not set"; }
  else {
    Double_t rtime = fCopyTimer->RealTime();
    Double_t ctime = fCopyTimer->CpuTime();
    LOG(info) << "CbmTaskBuildRawEvents (digi copy only): Real time " << rtime << " s, CPU time " << ctime << " s";
  }
}

void CbmTaskBuildRawEvents::Finish()
{
  if ((fvDigiMatchQa != nullptr) && (fSeedFinderSlidingWindow != nullptr)) { fSeedFinderSlidingWindow->OutputQa(); }

  /// Call Algo finish method
  fpAlgo->Finish();
  if (fbFillHistos) { SaveHistos(); }
  if (fbGetTimings) { PrintTimings(); }

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices          : " << fNofTs;
  LOG(info) << "Events               : " << fNofEvents;
  if (fSeedFinderSlidingWindow) { LOG(info) << "Triggers             : " << fTotalSeedCount; }
  LOG(info) << "Time  / TS           : " << std::fixed << std::setprecision(2) << 1000. * fTime / Double_t(fNofTs)
            << " ms";
  LOG(info) << "=====================================";
}

void CbmTaskBuildRawEvents::FillOutput()
{
  /// Clear TClonesArray before usage.
  fEvents->Delete();

  /// Get vector reference from algo
  std::vector<CbmEvent*> vEvents = fpAlgo->GetEventVector();

  /// Move CbmEvent from temporary vector to TClonesArray
  for (CbmEvent* event : vEvents) {
    LOG(debug) << "Vector: " << event->ToString();
    new ((*fEvents)[fEvents->GetEntriesFast()]) CbmEvent(std::move(*event));
    LOG(debug) << "TClonesArray: " << static_cast<CbmEvent*>(fEvents->At(fEvents->GetEntriesFast() - 1))->ToString();
  }
  /// Clear event vector after usage
  fpAlgo->ClearEventVector();
}

void CbmTaskBuildRawEvents::SaveHistos()
{
  if (fbWriteHistosToFairSink) {
    if (!FairRootManager::Instance() || !FairRootManager::Instance()->GetSink()) {
      LOG(error) << "No sink found";
      return;
    }
    FairSink* sink = FairRootManager::Instance()->GetSink();
    sink->WriteObject(dynamic_cast<TObject*>(fpAlgo->GetOutFolder()), nullptr);
  }
  else {

    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos = fpAlgo->GetHistoVector();

    /// (Re-)Create ROOT file to store the histos
    TDirectory* oldDir = NULL;
    TFile* histoFile   = NULL;
    /// Store current directory position to allow restore later
    oldDir = gDirectory;
    /// open separate histo file in recreate mode
    histoFile = new TFile(fsOutFileName, "RECREATE");
    histoFile->cd();

    /// Save all plots and create folders if needed
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      /// Make sure we end up in chosen folder
      const TString sFolder = vHistos[uHisto].second.data();
      if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
      gDirectory->cd(sFolder);

      /// Write plot
      vHistos[uHisto].first->Write();
      histoFile->cd();
    }

    /// Restore original directory position
    oldDir->cd();
    histoFile->Close();
  }
}

void CbmTaskBuildRawEvents::DumpSeedTimesFromDetList()
{
  std::ofstream timesUnsorted("digiTimesUnsorted.dat", std::ofstream::out);
  timesUnsorted << std::setprecision(16);

  for (RawEventBuilderDetector& system : fSeedTimeDetList) {
    for (UInt_t i = 0; i < GetNofDigis(system.detId); i++) {
      timesUnsorted << GetDigiTime(system.detId, i) << std::endl;
    }
  }
  timesUnsorted.close();
  LOG(info) << "Completed write of unsorted digi list.";

  std::ofstream timesSorted("digiTimesSorted.dat", std::ofstream::out);
  timesSorted << std::setprecision(16);

  for (UInt_t i = 0; i < fSeedTimes->size(); i++) {
    timesSorted << fSeedTimes->at(i) << std::endl;
  }
  timesSorted.close();
  LOG(info) << "Completed DumpSeedTimesFromDetList(). Closing.";
  exit(0);  //terminate as this method should only be used for diagnostics
}

ClassImp(CbmTaskBuildRawEvents)
