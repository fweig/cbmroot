// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018UnpackerTaskPsd                    -----
// -----                 Created 09.10.2019 by N.Karpushkin                -----
// -----        based on CbmMcbm2018UnpackerTaskTof by P.-A. Loizeau       -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018UnpackerTaskPsd.h"

#include "CbmMcbm2018PsdPar.h"
#include "CbmMcbm2018UnpackerAlgoPsd.h"

#include "FairLogger.h"
#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "THttpServer.h"
#include "TROOT.h"
#include "TString.h"
#include <TFile.h>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdint.h>

Bool_t bMcbm2018UnpackerTaskPsdResetHistos = kFALSE;

CbmMcbm2018UnpackerTaskPsd::CbmMcbm2018UnpackerTaskPsd(UInt_t /*uNbGdpb*/)
  : CbmMcbmUnpack()
  , fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fbWriteOutput(kTRUE)
  , fuDigiMaskId(0x0001FFFF)
  , fulTsCounter(0)
  , fUnpackerAlgo(nullptr) {
  fUnpackerAlgo = new CbmMcbm2018UnpackerAlgoPsd();
}

CbmMcbm2018UnpackerTaskPsd::~CbmMcbm2018UnpackerTaskPsd() {
  delete fUnpackerAlgo;
}

Bool_t CbmMcbm2018UnpackerTaskPsd::Init() {
  LOG(info) << "CbmMcbm2018UnpackerTaskPsd::Init";
  LOG(info) << "Initializing mCBM PSD 2018 Unpacker";

  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { LOG(fatal) << "No FairRootManager instance"; }

  /// Get address of vector from algo
  fpvDigiPsd = &(fUnpackerAlgo->GetVector());
  ioman->RegisterAny("PsdDigi", fpvDigiPsd, fbWriteOutput);

  return kTRUE;
}

void CbmMcbm2018UnpackerTaskPsd::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();

  TList* fParCList = fUnpackerAlgo->GetParList();

  for (Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);

    std::string sParamName {tempObj->GetName()};
    FairParGenericSet* newObj = dynamic_cast<FairParGenericSet*>(
      FairRun::Instance()->GetRuntimeDb()->getContainer(sParamName.data()));

    if (nullptr == newObj) {
      LOG(error) << "Failed to obtain parameter container " << sParamName
                 << ", for parameter index " << iparC;
      return;
    }  // if( nullptr == newObj )

    fParCList->AddAt(newObj, iparC);
    //      delete tempObj;
  }  // for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
}

Bool_t CbmMcbm2018UnpackerTaskPsd::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmMcbm2018PsdPar* pUnpackPar = dynamic_cast<CbmMcbm2018PsdPar*>(
    FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018PsdPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmMcbm2018PsdPar";
    return kFALSE;
  }  // if( nullptr == pUnpackPar )
     /*
   fbMonitorMode = pUnpackPar->GetMonitorMode();
   LOG(info) << "Monitor mode:       "
             << ( fbMonitorMode ? "ON" : "OFF" );

   fbDebugMonitorMode = pUnpackPar->GetDebugMonitorMode();
   LOG(info) << "Debug Monitor mode: "
             << ( fbDebugMonitorMode ? "ON" : "OFF" );
*/
  Bool_t initOK = fUnpackerAlgo->InitContainers();

  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    /// Trigger histo creation on all associated algos
    initOK &= fUnpackerAlgo->CreateHistograms();

    fhArraySize =
      new TH1I("fhArraySize",
               "Size of the Array VS TS index; TS index; Size [bytes]",
               10000,
               0.,
               10000.);
    fhArrayCapacity =
      new TH1I("fhArrayCapacity",
               "Size of the Array VS TS index; TS index; Size [bytes]",
               10000,
               0.,
               10000.);

    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos =
      fUnpackerAlgo->GetHistoVector();

    /// Register the histos in the HTTP server
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
    if (nullptr != server) {
      for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
        server->Register(Form("/%s", vHistos[uHisto].second.data()),
                         vHistos[uHisto].first);
      }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

      server->RegisterCommand("/Reset_UnpPsd_Hist",
                              "bMcbm2018UnpackerTaskPsdResetHistos=kTRUE");
      server->Restrict("/Reset_UnpPsd_Hist", "allow=admin");

      server->Register("/Array", fhArraySize);
      server->Register("/Array", fhArrayCapacity);
    }  // if( nullptr != server )
  }    // if( kTRUE == fbMonitorMode )

  fUnpackerAlgo->SetMonitorMode(fbMonitorMode);

  return initOK;
}

Bool_t CbmMcbm2018UnpackerTaskPsd::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fUnpackerAlgo->ReInitContainers();

  return initOK;
}

void CbmMcbm2018UnpackerTaskPsd::AddMsComponentToList(size_t component,
                                                      UShort_t usDetectorId) {
  fUnpackerAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmMcbm2018UnpackerTaskPsd::DoUnpack(const fles::Timeslice& ts,
                                            size_t /*component*/) {

  if (fbMonitorMode && bMcbm2018UnpackerTaskPsdResetHistos) {
    LOG(info) << "Reset PSD unpacker histos ";
    fUnpackerAlgo->ResetHistograms();
    bMcbm2018UnpackerTaskPsdResetHistos = kFALSE;
  }  // if( fbMonitorMode && bMcbm2018UnpackerTaskPsdResetHistos )

  if (kFALSE == fUnpackerAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index()
               << " in unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fUnpackerAlgo->ProcessTs( ts ) )

  /*
   /// Sort the buffers of hits due to the time offsets applied
   => Done in the algo!!!
   sort(fpvDigiPsd->begin(), fpvDigiPsd->end(),
        [](const CbmPsdDigi & a, const CbmPsdDigi & b) -> bool
        {
          return a.GetTime() < b.GetTime();
        });
*/

  /*
   fhArraySize->Fill( fulTsCounter, fPsdDigiCloneArray->GetEntries()  );
   fhArrayCapacity->Fill( fulTsCounter, fPsdDigiCloneArray->Capacity()  );
*/
  if (0 == fulTsCounter % 10000)
    LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  return kTRUE;
}

void CbmMcbm2018UnpackerTaskPsd::Reset() { fUnpackerAlgo->ClearVector(); }


void CbmMcbm2018UnpackerTaskPsd::Finish() {
  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos =
      fUnpackerAlgo->GetHistoVector();

    /// (Re-)Create ROOT file to store the histos
    TDirectory* oldDir = NULL;
    TFile* histoFile   = NULL;
    // Store current directory position to allow restore later
    oldDir = gDirectory;
    // open separate histo file in recreate mode
    histoFile = new TFile("data/HistosUnpackerPsd.root", "RECREATE");
    histoFile->cd();

    /// Register the histos in the HTTP server
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      /// Make sure we end up in chosen folder
      TString sFolder = vHistos[uHisto].second.data();
      if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
      gDirectory->cd(sFolder);

      /// Write plot
      vHistos[uHisto].first->Write();

      histoFile->cd();
    }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

    fhArraySize->Write();
    fhArrayCapacity->Write();
    // Restore original directory position
    oldDir->cd();
    histoFile->Close();
  }  // if( kTRUE == fbMonitorMode )
}

void CbmMcbm2018UnpackerTaskPsd::SetIgnoreOverlapMs(Bool_t bFlagIn) {
  fUnpackerAlgo->SetIgnoreOverlapMs(bFlagIn);
}

void CbmMcbm2018UnpackerTaskPsd::SetTimeOffsetNs(Double_t dOffsetIn) {
  fUnpackerAlgo->SetTimeOffsetNs(dOffsetIn);
}
void CbmMcbm2018UnpackerTaskPsd::SetDiamondDpbIdx(UInt_t uIdx) {
  fUnpackerAlgo->SetDiamondDpbIdx(uIdx);
}

ClassImp(CbmMcbm2018UnpackerTaskPsd)
