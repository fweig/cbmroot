#include "CbmMcbm2018UnpackerTaskTrdR.h"
#include "CbmMcbm2018UnpackerAlgoTrdR.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include <TFile.h>
#include <THttpServer.h>

/* Default Constructor */
CbmMcbm2018UnpackerTaskTrdR::CbmMcbm2018UnpackerTaskTrdR()
  : CbmMcbmUnpack()
  , fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fbWriteOutput(kTRUE)
  , fbDebugWriteOutput(kFALSE)
  , fbBaselineAvg(kFALSE)
  , fSystemIdentifier((std::uint8_t) fles::SubsystemIdentifier::TRD)
  , fdMsSizeInNs(1.28e6)  // default value corresponds to mCbm 2020 value
  , fMonitorHistoFileName("")
  , fIsActiveHistoVec(CbmMcbm2018UnpackerAlgoTrdR::kEndDefinedHistos, false)
  , fTrdDigiVector(nullptr)
  , fTrdRawMessageVector(nullptr)
  , fSpadicInfoMsgVector(nullptr)
  , fUnpackerAlgo(nullptr) {
  fUnpackerAlgo = new CbmMcbm2018UnpackerAlgoTrdR();
}

/* Default Destructor */
CbmMcbm2018UnpackerTaskTrdR::~CbmMcbm2018UnpackerTaskTrdR() {
  delete fUnpackerAlgo;
}

Bool_t CbmMcbm2018UnpackerTaskTrdR::Init() {
  LOG(info) << "Initializing CbmMcbm2018UnpackerTaskTrdR...";
  Bool_t initOK = 1;

  FairRootManager* ioman = nullptr;
  ioman                  = FairRootManager::Instance();
  if (ioman == nullptr) { LOG(fatal) << "No FairRootManager instance"; }

  /// Register Digi output vector.
  fTrdDigiVector = new std::vector<CbmTrdDigi>();
  if (fTrdDigiVector) {
    ioman->RegisterAny("TrdDigi", fTrdDigiVector, fbWriteOutput);
    initOK &= fUnpackerAlgo->SetDigiOutputPointer(fTrdDigiVector);
  } else {
    LOG(fatal) << "fTrdDigiVector could not be registered at FairRootManager.";
  }

  /// Register RawMessage output vector, if DebugWrite is enabled.
  if (fbDebugWriteOutput) {
    fTrdRawMessageVector = new std::vector<CbmTrdRawMessageSpadic>();
    fSpadicInfoMsgVector =
      new std::vector<std::pair<std::uint64_t, std::uint64_t>>();
    if (fTrdRawMessageVector && fSpadicInfoMsgVector) {
      ioman->RegisterAny(
        "CbmTrdSpadicRawMessages", fTrdRawMessageVector, kTRUE);
      ioman->RegisterAny(
        "CbmTrdSpadicInfoMessages", fSpadicInfoMsgVector, kTRUE);
      initOK &= fUnpackerAlgo->SetRawOutputPointer(fTrdRawMessageVector,
                                                   fSpadicInfoMsgVector);
    } else {
      LOG(fatal) << "[CbmMcbm2018UnpackerTaskTrdR::Init] Raw output could not "
                    "be registered at FairRootManager.";
    }
  }

  fUnpackerAlgo->SetMsSizeInNs(
    fdMsSizeInNs);  // TODO handle this with asic parameter files
  fUnpackerAlgo->SetFirstChannelsElinkEven(fIsFirstChannelsElinkEven);
  initOK &= fUnpackerAlgo->Init();

  if (initOK) {
    LOG(info) << "Initialization of CbmMcbm2018UnpackerTaskTrdR and "
                 "CbmMcbm2018UnpackerAlgoTrdR successfull!";
  } else {
    LOG(fatal) << "Init of CbmMcbm2018UnpackerAlgoTrdR failed!";
  }

  return initOK;
}

Bool_t CbmMcbm2018UnpackerTaskTrdR::DoUnpack(const fles::Timeslice& ts,
                                             size_t /*component*/) {
  /// Call the Algorithm to unpack the given TS.
  if (kFALSE == fUnpackerAlgo->ProcessTs(ts)) {
    LOG(error) << "CbmMcbm2018UnpackerTaskTrdR: Failed processing TS "
               << ts.index() << " in unpacker algorithm class.";
    return kFALSE;
  }


  // sts does a time sorting of fTrdDigiVector here. but this could also be done in the algo?!


  return kTRUE;
}

void CbmMcbm2018UnpackerTaskTrdR::Reset() {
  if (fTrdDigiVector) fTrdDigiVector->clear();
  if (fTrdRawMessageVector) fTrdRawMessageVector->clear();
  if (fSpadicInfoMsgVector) fSpadicInfoMsgVector->clear();
  if (fUnpackerAlgo) fUnpackerAlgo->Reset();
}

void CbmMcbm2018UnpackerTaskTrdR::Finish() {
  LOG(info) << "Finish of CbmMcbm2018UnpackerTaskTrdR";
  fUnpackerAlgo->Finish();

  if ((fbMonitorMode == kTRUE || fbDebugMonitorMode == kTRUE)
      && fMonitorHistoFileName != "") {
    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos =
      fUnpackerAlgo->GetHistoVector();

    /// (Re-)Create ROOT file to store the histos
    TFile* histoFile   = nullptr;

    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    // open separate histo file in recreate mode
    TString histoFileName = fMonitorHistoFileName;
    histoFile             = new TFile(histoFileName.Data(), "RECREATE");
    histoFile->cd();
    /// Write all the histograms to the file.
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      // Make sure we end up in chosen folder
      TString sFolder = vHistos[uHisto].second.data();
      if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
      gDirectory->cd(sFolder);
      // Write histogram
      vHistos[uHisto].first->Write();
      histoFile->cd();
    }
    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;

    histoFile->Close();
  }
}

void CbmMcbm2018UnpackerTaskTrdR::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();

  TList* fParContList = fUnpackerAlgo->GetParList();

  Int_t iParCont(0);
  for (auto parSetIt : *fParContList) {
    CbmTrdParSet* tempObj = (CbmTrdParSet*) (parSetIt);
    fParContList->Remove(tempObj);
    std::string sParamName {tempObj->GetName()};
    delete tempObj;
    CbmTrdParSet* updatedParSet = nullptr;
    updatedParSet               = dynamic_cast<CbmTrdParSet*>(
      FairRun::Instance()->GetRuntimeDb()->getContainer(sParamName.data()));

    if (!updatedParSet) {
      LOG(error) << "Failed to obtain parameter container " << sParamName
                 << ", for parameter index " << iParCont;
      return;
    }
    fParContList->AddAt(updatedParSet, iParCont);
  }

  // Get timeshift parameters
  fTimeshiftPar = dynamic_cast<CbmMcbm2020TrdTshiftPar*>(
    FairRun::Instance()->GetRuntimeDb()->getContainer(
      "CbmMcbm2020TrdTshiftPar"));
}

Bool_t CbmMcbm2018UnpackerTaskTrdR::InitContainers() {
  if (fUnpackerAlgo == nullptr) {
    LOG(error) << "CbmMcbm2018UnpackerTaskTrdR::InitContainers failed! No "
                  "Unpacker Algo.";
    return kFALSE;
  }
  LOG(info) << "Init containers for CbmMcbm2018UnpackerTaskTrdR";

  /// Hand over control flags to the algorithm.
  fUnpackerAlgo->SetMonitorMode(fbMonitorMode);
  fUnpackerAlgo->SetDebugMonitorMode(fbDebugMonitorMode);
  fUnpackerAlgo->SetWriteOutput(fbWriteOutput);
  fUnpackerAlgo->SetDebugWriteOutput(fbDebugWriteOutput);
  fUnpackerAlgo->SetDebugSortOutput(fbDebugSortOutput);
  fUnpackerAlgo->SetBaselineAvg(fbBaselineAvg);
  // Activate histograms in unpacker
  fUnpackerAlgo->SetActiveHistograms(fIsActiveHistoVec);

  Bool_t initOK = fUnpackerAlgo->InitContainers();

  if (fTimeshiftPar) {
    auto maptimeshifts = fTimeshiftPar->GetTimeshiftsMap();
    fUnpackerAlgo->SetTimeshiftsMap(maptimeshifts);
    LOG(info) << "CbmMcbm2018UnpackerTaskTrdR::SetParContainers() - Parsing "
                 "timeshift correction map to unpacker algo";
  }

  /// If monitor mode enabled, trigger histos creation,
  /// obtain pointer on them and add them to the HTTP server.
  if (fbMonitorMode == kTRUE || fbDebugMonitorMode == kTRUE) {
    /// Trigger Histo creation in the algo.
    initOK &= fUnpackerAlgo->CreateHistograms();

    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder).
    std::vector<std::pair<TNamed*, std::string>> vHistos =
      fUnpackerAlgo->GetHistoVector();

    /// Register the histos in the HTTP server
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
    if (nullptr != server) {
      for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
        server->Register(Form("/%s", vHistos[uHisto].second.data()),
                         vHistos[uHisto].first);
      }
      // FIXME: register the correct command
      //server->RegisterCommand("/Reset_UnpSts_Hist", "bMcbm2018UnpackerTaskStsResetHistos=kTRUE");
      //server->Restrict("/Reset_UnpSts_Hist", "allow=admin");
    } else {
      //			initOK &= 0;
      /// Avoid crash in other unpackers due to the FAIRROOT "feature" that a 0 return value goes on with the run without initializing
      /// tasks which are later in the alphabetical order
      LOG(warning) << "The histograms from CbmMcbm2018UnpackerTaskTrdR will "
                      "not be available online as no server present";
    }  // end if( nullptr != server )
  }    // end if (fbMonitorMode == kTRUE || fbDebugMonitorMode == kTRUE)

  return initOK;
}

Bool_t CbmMcbm2018UnpackerTaskTrdR::ReInitContainers() {
  if (fUnpackerAlgo == nullptr) return kFALSE;

  LOG(info) << "ReInit parameter containers for CbmMcbm2018UnpackerTaskTrdR";

  Bool_t initOK = fUnpackerAlgo->ReInitContainers();
  return initOK;
}

void CbmMcbm2018UnpackerTaskTrdR::AddMsComponentToList(size_t component,
                                                       UShort_t usDetectorId) {
  if (usDetectorId != (UShort_t) fSystemIdentifier) {
    LOG(error) << "CbmMcbm2018UnpackerTaskTrdR::AddMsComponentToList : Wrong "
                  "Detector ID!";
    return;
  }
  if (fUnpackerAlgo != nullptr)
    fUnpackerAlgo->AddMsComponentToList(component, usDetectorId);
}

void CbmMcbm2018UnpackerTaskTrdR::SetNbMsInTs(size_t uCoreMsNb,
                                              size_t uOverlapMsNb) {
  if (fUnpackerAlgo != nullptr)
    fUnpackerAlgo->SetNbMsInTs(uCoreMsNb, uOverlapMsNb);
}

void CbmMcbm2018UnpackerTaskTrdR::SetHistoFileName(TString filename) {
  fMonitorHistoFileName = filename;
  SetMonitorMode(kTRUE);
}

void CbmMcbm2018UnpackerTaskTrdR::SetTimeOffsetNs(Double_t dOffsetIn) {
  if (fUnpackerAlgo != nullptr) fUnpackerAlgo->SetTimeOffsetNs(dOffsetIn);
}

void CbmMcbm2018UnpackerTaskTrdR::SetIgnoreOverlapMs(Bool_t bFlagIn) {
  fUnpackerAlgo->SetIgnoreOverlapMs(bFlagIn);
}
ClassImp(CbmMcbm2018UnpackerTaskTrdR)
