/* Copyright (C) 2007-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "CbmTaskBuildRawEvents.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmMatch.h"
#include "CbmModuleList.h"
#include "CbmSeedFinderSlidingWindow.h"

#include <FairRootManager.h>
#include <FairRunOnline.h>
#include <Logger.h>

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
  if (fDigiEvents) {
    fDigiEvents->clear();
    delete fDigiEvents;
  }
  else if (fEvents) {
    fEvents->Delete();
    delete fEvents;
  }
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

template<class TDigi>
void CbmTaskBuildRawEvents::InitDigis(ECbmModuleId detId, std::vector<TDigi>** vDigi)
{
  TString detName = CbmModuleList::GetModuleNameCaps(detId);
  if (!fDigiMan->IsPresent(detId)) { LOG(info) << "No " << detName << " digi input."; }
  else {
    LOG(info) << detName << " digi input.";
    *vDigi = new std::vector<TDigi>;
    fpAlgo->SetDigis(*vDigi);
  }
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

  // Get a pointer to the previous already existing data level
  fDigiMan = CbmDigiManager::Instance();
  if (fbUseMuchBeamtimeDigi) { fDigiMan->UseMuchBeamTimeDigi(); }
  fDigiMan->Init();

  //Init digis
  if (fbUseMuchBeamtimeDigi) { InitDigis(ECbmModuleId::kMuch, &fMuchBeamTimeDigis); }
  else {
    InitDigis(ECbmModuleId::kMuch, &fMuchDigis);
  }
  InitDigis(ECbmModuleId::kSts, &fStsDigis);
  InitDigis(ECbmModuleId::kTrd, &fTrdDigis);
  InitDigis(ECbmModuleId::kTof, &fTofDigis);
  InitDigis(ECbmModuleId::kRich, &fRichDigis);
  InitDigis(ECbmModuleId::kPsd, &fPsdDigis);
  InitDigis(ECbmModuleId::kT0, &fT0Digis);

  /// Register output (array of CbmEvent or vector of CbmDigiEvents)
  if (fbDigiEvtOut) {
    if (fbUseMuchBeamtimeDigi) LOG(fatal) << "DigiEvent output branch not compatible with MuchBeamtimeDigi";

    fDigiEvents = new std::vector<CbmDigiEvent>();
    ioman->RegisterAny("DigiEvent", fDigiEvents, kTRUE);
    if (!fDigiEvents) LOG(fatal) << "Output branch was not created";
  }
  else {
    fEvents = new TClonesArray("CbmEvent", 100);
    ioman->Register("CbmEvent", "Cbm_Event", fEvents, IsOutputBranchPersistent("CbmEvent"));
    if (!fEvents) LOG(fatal) << "Output branch was not created";
  }

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

template<class TDigi>
void CbmTaskBuildRawEvents::ReadDigis(ECbmModuleId detId, std::vector<TDigi>* vDigis)
{
  //Warning: Int_t must be used for the loop counters instead of UInt_t,
  //as the digi manager can return -1, which would be casted to +1
  //during comparison, leading to an error.
  if (fCopyTimer != nullptr) { fCopyTimer->Start(kFALSE); }

  const TString detName = CbmModuleList::GetModuleNameCaps(detId);

  if (fDigiMan->IsPresent(detId)) {
    vDigis->clear();
    const Int_t nDigis = fDigiMan->GetNofDigis(detId);

    for (Int_t i = 0; i < nDigis; i++) {
      const TDigi* Digi = fDigiMan->Get<TDigi>(i);
      vDigis->push_back(*Digi);
    }
    LOG(debug) << "Read: " << vDigis->size() << " " << detName << " digis.";
    LOG(debug) << "In DigiManager: " << nDigis << " " << detName << " digis.";
  }

  if (fCopyTimer != nullptr) { fCopyTimer->Stop(); }
}

void CbmTaskBuildRawEvents::Exec(Option_t* /*option*/)
{
  if (fTimer != nullptr) { fTimer->Start(kFALSE); }
  TStopwatch timer;
  timer.Start();
  LOG(debug2) << "CbmTaskBuildRawEvents::Exec => Starting sequence";

  // Process Timeslice
  BuildEvents();
  if (fTimer != nullptr) { fTimer->Stop(); }

  // --- Timeslice log and statistics
  timer.Stop();
  std::stringstream logOut;
  logOut << std::setw(20) << std::left << GetName() << " [";
  logOut << std::fixed << std::setw(8) << std::setprecision(1) << std::right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNofTs;
  if (fbDigiEvtOut) {
    logOut << ", events " << fDigiEvents->size();
    fNofEvents += fDigiEvents->size();
  }
  else {
    logOut << ", events " << fEvents->GetEntriesFast();
    fNofEvents += fEvents->GetEntriesFast();
  }
  LOG(info) << logOut.str();
  if (fSeedFinderSlidingWindow) {
    const size_t seedCount = fSeedFinderSlidingWindow->GetNofSeeds();
    LOG(info) << seedCount << " trigger(s) for this TS.";
    fTotalSeedCount += seedCount;
  }
  fNofTs++;
  fTime += timer.RealTime();

  LOG(debug2) << "CbmTaskBuildRawEvents::Exec => Done";
}

void CbmTaskBuildRawEvents::BuildEvents()
{
  //Reset explicit seed times if set
  if (fSeedTimes != nullptr) { fSeedTimes->clear(); }

  //Read digis
  if (fbUseMuchBeamtimeDigi) { ReadDigis(ECbmModuleId::kMuch, fMuchBeamTimeDigis); }
  else {
    ReadDigis(ECbmModuleId::kMuch, fMuchDigis);
  }
  ReadDigis(ECbmModuleId::kSts, fStsDigis);
  ReadDigis(ECbmModuleId::kTrd, fTrdDigis);
  ReadDigis(ECbmModuleId::kTof, fTofDigis);
  ReadDigis(ECbmModuleId::kRich, fRichDigis);
  ReadDigis(ECbmModuleId::kPsd, fPsdDigis);
  ReadDigis(ECbmModuleId::kT0, fT0Digis);

  //Fill seeds
  if (fSeedFinderSlidingWindow != nullptr) { FillSeedTimesFromSlidingWindow(); }
  else if (fSeedTimeDetList.size() > 0) {
    FillSeedTimesFromDetList(fSeedTimes);
  }
  //DumpSeedTimesFromDetList();

  /// Call Algo ProcessTs method
  fpAlgo->ProcessTs();

  /// Save the resulting vector of events in TClonesArray
  FillOutput();
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
    case ECbmModuleId::kTof: fSeedFinderSlidingWindow->FillSeedTimes(fTofDigis, fvDigiMatchQa); break;
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
  /// Get vector reference from algo
  std::vector<CbmEvent*> vEvents = fpAlgo->GetEventVector();

  if (fbDigiEvtOut) {
    /// Clear data from previous TS before usage.
    fDigiEvents->clear();

    /// Convert each CbmEvent to a CbmDigiEvent by extracting the corresponding data from the input vectors
    ExtractSelectedData(vEvents);
  }
  else {
    /// Clear TClonesArray before usage.
    fEvents->Delete();

    /// Move CbmEvent from temporary vector to TClonesArray
    for (CbmEvent* event : vEvents) {
      LOG(debug) << "Vector: " << event->ToString();
      new ((*fEvents)[fEvents->GetEntriesFast()]) CbmEvent(std::move(*event));
      LOG(debug) << "TClonesArray: " << static_cast<CbmEvent*>(fEvents->At(fEvents->GetEntriesFast() - 1))->ToString();
    }
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

void CbmTaskBuildRawEvents::ExtractSelectedData(std::vector<CbmEvent*> vEvents)
{
  /// Move CbmEvent from temporary vector to std::vector of full objects
  LOG(debug) << "In Vector size: " << vEvents.size();

  fDigiEvents->reserve(vEvents.size());
  for (CbmEvent* event : vEvents) {
    CbmDigiEvent selEvent;
    selEvent.fTime   = event->GetStartTime();
    selEvent.fNumber = event->GetNumber();

    /// FIXME: for pure digi based event, we select "continuous slices of digis"
    ///        => Copy block of [First Digi index, last digi index] with assign(it_start, it_stop)
    /// FIXME: Keep TRD1D + TRD2D support, may lead to holes in the digi sequence!
    ///        => Would need to keep the loop

    /// Get the proper order for block selection as TRD1D and TRD2D may insert indices in separate loops
    /// => Needed to ensure that the start and stop of the block copy do not trigger a vector size exception
    event->SortIndices();

    /// for each detector, find the data in the Digi vectors and copy them
    /// TODO: Template + loop on list of data types?
    /// ==> T0
    uint32_t uNbDigis = (0 < event->GetNofData(ECbmDataType::kT0Digi) ? event->GetNofData(ECbmDataType::kT0Digi) : 0);
    if (0 < uNbDigis) {
      auto startIt = fT0Digis->begin() + event->GetIndex(ECbmDataType::kT0Digi, 0);
      auto stopIt  = fT0Digis->begin() + event->GetIndex(ECbmDataType::kT0Digi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fT0.fDigis.assign(startIt, stopIt);
    }

    /// ==> STS
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kStsDigi) ? event->GetNofData(ECbmDataType::kStsDigi) : 0);
    if (0 < uNbDigis) {
      auto startIt = fStsDigis->begin() + event->GetIndex(ECbmDataType::kStsDigi, 0);
      auto stopIt  = fStsDigis->begin() + event->GetIndex(ECbmDataType::kStsDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fSts.fDigis.assign(startIt, stopIt);
    }

    /// ==> MUCH
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kMuchDigi) ? event->GetNofData(ECbmDataType::kMuchDigi) : 0);
    if (0 < uNbDigis) {
      auto startIt = fMuchDigis->begin() + event->GetIndex(ECbmDataType::kMuchDigi, 0);
      auto stopIt  = fMuchDigis->begin() + event->GetIndex(ECbmDataType::kMuchDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fMuch.fDigis.assign(startIt, stopIt);
    }

    /// ==> TRD + TRD2D
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kTrdDigi) ? event->GetNofData(ECbmDataType::kTrdDigi) : 0);
    if (0 < uNbDigis) {
      auto startIt = fTrdDigis->begin() + event->GetIndex(ECbmDataType::kTrdDigi, 0);
      auto stopIt  = fTrdDigis->begin() + event->GetIndex(ECbmDataType::kTrdDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fTrd.fDigis.assign(startIt, stopIt);
    }

    /// ==> TOF
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kTofDigi) ? event->GetNofData(ECbmDataType::kTofDigi) : 0);
    if (0 < uNbDigis) {
      auto startIt = fTofDigis->begin() + event->GetIndex(ECbmDataType::kTofDigi, 0);
      auto stopIt  = fTofDigis->begin() + event->GetIndex(ECbmDataType::kTofDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fTof.fDigis.assign(startIt, stopIt);
    }

    /// ==> RICH
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kRichDigi) ? event->GetNofData(ECbmDataType::kRichDigi) : 0);
    if (0 < uNbDigis) {
      auto startIt = fRichDigis->begin() + event->GetIndex(ECbmDataType::kRichDigi, 0);
      auto stopIt  = fRichDigis->begin() + event->GetIndex(ECbmDataType::kRichDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fRich.fDigis.assign(startIt, stopIt);
    }

    /// ==> PSD
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kPsdDigi) ? event->GetNofData(ECbmDataType::kPsdDigi) : 0);
    if (0 < uNbDigis) {
      auto startIt = fPsdDigis->begin() + event->GetIndex(ECbmDataType::kPsdDigi, 0);
      auto stopIt  = fPsdDigis->begin() + event->GetIndex(ECbmDataType::kPsdDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fPsd.fDigis.assign(startIt, stopIt);
    }

    fDigiEvents->push_back(std::move(selEvent));
  }
}

ClassImp(CbmTaskBuildRawEvents)
