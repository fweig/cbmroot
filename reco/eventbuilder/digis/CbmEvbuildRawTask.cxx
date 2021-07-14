/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmEvbuildRawTask.h"

#include "CbmDigiManager.h"
#include "CbmRawEvent.h"

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

using std::cout;
using std::endl;
using std::fixed;
using std::left;
using std::pair;
using std::right;
using std::setprecision;
using std::setw;
using std::stringstream;


CbmEvbuildRawTask::CbmEvbuildRawTask() : FairTask("BuildRawEvents")
{
  /// Create Algo. To be made generic/switchable when more event building algo are available!
  fpAlgo = new CbmEvbuildRawAlgo();
}

CbmEvbuildRawTask::~CbmEvbuildRawTask() {}

void CbmEvbuildRawTask::SetParContainers() {}

void CbmEvbuildRawTask::SetSeedTimeFiller(RawEventBuilderDetector seedDet)
{
  if (fSeedTimeDetList.size() > 0) { LOG(fatal) << "Cannot use seed detector list and single instance together."; }

  fSeedTimeDet = seedDet;
  if (fSeedTimeDet != kRawEventBuilderDetUndef) {
    if (fSeedTimes == nullptr) { fSeedTimes = new std::vector<Double_t>; }
  }
  else {
    if (fSeedTimes != nullptr) {
      fSeedTimes->clear();
      delete fSeedTimes;
      fSeedTimes = nullptr;
    }
  }
  fpAlgo->SetSeedTimes(fSeedTimes);
}

void CbmEvbuildRawTask::AddSeedTimeFillerToList(RawEventBuilderDetector seedDet)
{
  if (fSeedTimeDet != kRawEventBuilderDetUndef) {
    LOG(fatal) << "Cannot use seed detector list and single instance together.";
  }
  if (fSeedTimes == nullptr) { fSeedTimes = new std::vector<Double_t>; }

  fSeedTimeDetList.push_back(seedDet);
  fpAlgo->SetSeedTimes(fSeedTimes);
}

InitStatus CbmEvbuildRawTask::Init()
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
  fEvents = new TClonesArray("CbmRawEvent", 100);
  ioman->Register("RawEvent", "CBM Raw Event", fEvents, IsOutputBranchPersistent("RawEvent"));
  if (!fEvents) LOG(fatal) << "Output branch was not created";

  // Set timeslice meta data
  fpAlgo->SetTimeSliceMetaDataArray(dynamic_cast<TClonesArray*>(ioman->GetObject("TimesliceMetaData")));

  if (fTimer != nullptr) {
    fTimer->Stop();
    Double_t rtime = fTimer->RealTime();
    Double_t ctime = fTimer->CpuTime();
    LOG(info) << "CbmEvbuildRawTask::Init(): Real time " << rtime << " s, CPU time " << ctime << " s";
  }

  /// Call Algo Init method
  if (kTRUE == fpAlgo->InitAlgo()) return kSUCCESS;
  else
    return kFATAL;
}


InitStatus CbmEvbuildRawTask::ReInit() { return kSUCCESS; }

void CbmEvbuildRawTask::Exec(Option_t* /*option*/)
{
  if (fTimer != nullptr) { fTimer->Start(kFALSE); }
  TStopwatch timer;
  timer.Start();

  LOG(debug2) << "CbmEvbuildRawTask::Exec => Starting sequence";
  //Warning: Int_t must be used for the loop counters instead of UInt_t,
  //as the digi manager can return -1, which would be casted to +1
  //during comparison, leading to an error.

  if (fSeedTimeDet != kRawEventBuilderDetUndef && fSeedTimeDetList.size() > 0) {
    LOG(fatal) << "Cannot use seed detector list and single instance together.";
  }

  //Reset explicit seed times if set
  if (fSeedTimeDet != kRawEventBuilderDetUndef || fSeedTimeDetList.size() > 0) { fSeedTimes->clear(); }

  if (fCopyTimer != nullptr) { fCopyTimer->Start(kFALSE); }

  //Read STS digis
  if (fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    fStsDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kSts); i++) {
      const CbmStsDigi* Digi = fDigiMan->Get<CbmStsDigi>(i);
      fStsDigis->push_back(*Digi);
      if (fSeedTimeDet.detId == ECbmModuleId::kSts) { fSeedTimes->push_back(Digi->GetTime()); }
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
        if (fSeedTimeDet.detId == ECbmModuleId::kMuch) { fSeedTimes->push_back(Digi->GetTime()); }
      }
      LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kMuch) << " MUCH digis.";
      LOG(debug) << "In DigiManager: " << fMuchBeamTimeDigis->size() << " MUCH digis.";
    }
    else {
      fMuchDigis->clear();
      for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kMuch); i++) {
        const CbmMuchDigi* Digi = fDigiMan->Get<CbmMuchDigi>(i);
        fMuchDigis->push_back(*Digi);
        if (fSeedTimeDet.detId == ECbmModuleId::kMuch) { fSeedTimes->push_back(Digi->GetTime()); }
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
      if (fSeedTimeDet.detId == ECbmModuleId::kTrd) { fSeedTimes->push_back(Digi->GetTime()); }
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
      if (fSeedTimeDet.detId == ECbmModuleId::kTof) { fSeedTimes->push_back(Digi->GetTime()); }
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
      if (fSeedTimeDet.detId == ECbmModuleId::kRich) { fSeedTimes->push_back(Digi->GetTime()); }
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
      if (fSeedTimeDet.detId == ECbmModuleId::kPsd) { fSeedTimes->push_back(Digi->GetTime()); }
    }
    LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kPsd) << " PSD digis.";
    LOG(debug) << "In DigiManager: " << fPsdDigis->size() << " PSD digis.";
  }

  if (fCopyTimer != nullptr) { fCopyTimer->Stop(); }

  if (fSeedTimeDetList.size() > 0) { FillSeedTimesFromDetList(); }
  //DumpSeedTimesFromDetList();

  /// Call Algo ProcessTs method
  fpAlgo->ProcessTs();

  /// Save the resulting vector of events in TClonesArray
  FillOutput();
  LOG(debug2) << "CbmEvbuildRawTask::Exec => Done";

  if (fTimer != nullptr) { fTimer->Stop(); }

  // --- Timeslice log and statistics
  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNofTs;
  if (fEvents) logOut << ", events " << fEvents->GetEntriesFast();
  LOG(info) << logOut.str();
  fNofTs++;
  fNofEvents += fEvents->GetEntriesFast();
  fTime += timer.RealTime();
}

void CbmEvbuildRawTask::FillSeedTimesFromDetList()
{
  std::map<ECbmModuleId, UInt_t> DigiNumbers;
  std::map<ECbmModuleId, UInt_t> DigiCounters;
  fSeedTimes->clear();

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
      fSeedTimes->push_back(earliestTime);
      DigiCounters[nextAddedSystem]++;
    }
    else {
      break;
    }
  } while (true);
}

void CbmEvbuildRawTask::DumpSeedTimesFromDetList()
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

Double_t CbmEvbuildRawTask::GetDigiTime(ECbmModuleId _system, UInt_t _entry)
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
    case ECbmModuleId::kHodo: return (fT0Digis->at(_entry)).GetTime();
    default: break;
  }
  return -1;
}

UInt_t CbmEvbuildRawTask::GetNofDigis(ECbmModuleId _system)
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
    case ECbmModuleId::kHodo: return fT0Digis->size();
    default: break;
  }
  return 0;
}

void CbmEvbuildRawTask::PrintTimings()
{
  if (fTimer == nullptr) { LOG(fatal) << "Trying to print timings but timer not set"; }
  else {
    Double_t rtime = fTimer->RealTime();
    Double_t ctime = fTimer->CpuTime();
    LOG(info) << "CbmEvbuildRawTask: Real time " << rtime << " s, CPU time " << ctime << " s";
  }
  if (fCopyTimer == nullptr) { LOG(fatal) << "Trying to print timings but timer not set"; }
  else {
    Double_t rtime = fCopyTimer->RealTime();
    Double_t ctime = fCopyTimer->CpuTime();
    LOG(info) << "CbmEvbuildRawTask (digi copy only): Real time " << rtime << " s, CPU time " << ctime << " s";
  }
}

void CbmEvbuildRawTask::Finish()
{
  /// Call Algo finish method
  fpAlgo->Finish();
  if (fbFillHistos) { SaveHistos(); }
  if (fbGetTimings) { PrintTimings(); }

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices          : " << fNofTs;
  LOG(info) << "Events               : " << fNofEvents;
  LOG(info) << "Time  / TS           : " << fixed << setprecision(2) << 1000. * fTime / Double_t(fNofTs) << " ms";
  LOG(info) << "=====================================";
}

void CbmEvbuildRawTask::FillOutput()
{
  /// Clear TClonesArray before usage.
  fEvents->Delete();

  /// Get vector reference from algo
  std::vector<CbmRawEvent*> vEvents = fpAlgo->GetEventVector();

  /// Move CbmEvent from temporary vector to TClonesArray
  for (CbmRawEvent* event : vEvents) {
    LOG(debug) << "Vector: " << event->ToString();
    new ((*fEvents)[fEvents->GetEntriesFast()]) CbmRawEvent(std::move(*event));
    LOG(debug) << "TClonesArray: " << static_cast<CbmRawEvent*>(fEvents->At(fEvents->GetEntriesFast() - 1))->ToString();
  }
  /// Clear event vector after usage
  fpAlgo->ClearEventVector();
}

void CbmEvbuildRawTask::SaveHistos()
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


ClassImp(CbmEvbuildRawTask)