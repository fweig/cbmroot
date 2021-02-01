// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCosy2019MonitorTaskHodo                    -----
// -----               Created 03.07.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmCosy2019MonitorTaskHodo.h"

#include "CbmCosy2019MonitorAlgoHodo.h"
//#include "CbmCosy2019HodoPar.h"
//#include "CbmHodoDigi.h"

#include "FairLogger.h"
#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include "TCanvas.h"
#include "THttpServer.h"
#include "TList.h"
#include "TROOT.h"
#include "TString.h"
#include <TFile.h>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdint.h>

Bool_t bCosy2019MonitorTaskHodoResetHistos = kFALSE;

CbmCosy2019MonitorTaskHodo::CbmCosy2019MonitorTaskHodo(UInt_t /*uNbGdpb*/)
  : CbmMcbmUnpack()
  , fbMonitorMode(kFALSE)
  , fbWriteOutput(kTRUE)
  , fulTsCounter(0)
  ,
  //    fHodoDigiCloneArray(),
  fMonitorAlgo(nullptr) {
  fMonitorAlgo = new CbmCosy2019MonitorAlgoHodo();
}

CbmCosy2019MonitorTaskHodo::~CbmCosy2019MonitorTaskHodo() {
  delete fMonitorAlgo;
}

Bool_t CbmCosy2019MonitorTaskHodo::Init() {
  LOG(info) << "CbmCosy2019MonitorTaskHodo::Init";
  LOG(info) << "Initializing mCBM Hodoscopes Monitor";
  /*
   FairRootManager* ioman = FairRootManager::Instance();
   if( NULL == ioman )
   {
      LOG(fatal) << "No FairRootManager instance";
   }

   fHodoDigiCloneArray= new TClonesArray("CbmHodoDigi", 10);
   if( NULL == fHodoDigiCloneArray )
   {
      LOG(fatal) << "Failed creating the STS Digi TClonesarray ";
   }
   ioman->Register("CbmHodoDigi", "STS raw Digi", fHodoDigiCloneArray, fbWriteOutput);
*/
  return kTRUE;
}

void CbmCosy2019MonitorTaskHodo::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();

  TList* parCList = fMonitorAlgo->GetParList();

  for (Int_t iparC = 0; iparC < parCList->GetEntries(); ++iparC) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (parCList->At(iparC));
    parCList->Remove(tempObj);

    std::string sParamName {tempObj->GetName()};
    FairParGenericSet* newObj = dynamic_cast<FairParGenericSet*>(
      FairRun::Instance()->GetRuntimeDb()->getContainer(sParamName.data()));

    if (nullptr == newObj) {
      LOG(error) << "Failed to obtain parameter container " << sParamName
                 << ", for parameter index " << iparC;
      return;
    }  // if( nullptr == newObj )

    parCList->AddAt(newObj, iparC);
    //      delete tempObj;
  }  // for( Int_t iparC = 0; iparC < parCList->GetEntries(); ++iparC )
}

Bool_t CbmCosy2019MonitorTaskHodo::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  /*
   CbmCosy2019HodoPar * pUnpackPar = dynamic_cast<CbmCosy2019HodoPar*>( FairRun::Instance()->GetRuntimeDb()->getContainer( "CbmCosy2019HodoPar" ) );
   if( nullptr == pUnpackPar )
   {
      LOG(error) << "Failed to obtain parameter container CbmCosy2019HodoPar";
      return kFALSE;
   } // if( nullptr == pUnpackPar )
*/
  /*
   fbMonitorMode = pUnpackPar->GetMonitorMode();
   LOG(info) << "Monitor mode:       "
             << ( fbMonitorMode ? "ON" : "OFF" );

   fbDebugMonitorMode = pUnpackPar->GetDebugMonitorMode();
   LOG(info) << "Debug Monitor mode: "
             << ( fbDebugMonitorMode ? "ON" : "OFF" );
*/
  Bool_t initOK = fMonitorAlgo->InitContainers();


  /// Trigger histo creation on all associated algos
  initOK &= fMonitorAlgo->CreateHistograms();

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos =
    fMonitorAlgo->GetHistoVector();
  std::vector<std::pair<TCanvas*, std::string>> vCanvas =
    fMonitorAlgo->GetCanvasVector();

  /// Register the histos in the HTTP server
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (nullptr != server) {
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      if (nullptr != vHistos[uHisto].first)
        server->Register(Form("/%s", vHistos[uHisto].second.data()),
                         vHistos[uHisto].first);
    }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

    for (UInt_t uCanvas = 0; uCanvas < vCanvas.size(); ++uCanvas) {
      if (nullptr != vCanvas[uCanvas].first)
        server->Register(Form("/%s", vCanvas[uCanvas].second.data()),
                         vCanvas[uCanvas].first);
    }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

    server->RegisterCommand("/Reset_MoniHodo_Hist",
                            "bCosy2019MonitorTaskHodoResetHistos=kTRUE");
    server->Restrict("/Reset_MoniHodo_Hist", "allow=admin");
  }  // if( nullptr != server )

  fMonitorAlgo->SetMonitorMode(fbMonitorMode);

  return initOK;
}

Bool_t CbmCosy2019MonitorTaskHodo::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fMonitorAlgo->ReInitContainers();

  return initOK;
}

void CbmCosy2019MonitorTaskHodo::AddMsComponentToList(size_t component,
                                                      UShort_t usDetectorId) {
  fMonitorAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmCosy2019MonitorTaskHodo::DoUnpack(const fles::Timeslice& ts,
                                            size_t /*component*/) {
  if (fbMonitorMode && bCosy2019MonitorTaskHodoResetHistos) {
    LOG(info) << "Reset Hodoscopes monitor histos ";
    fMonitorAlgo->ResetHistograms();
    bCosy2019MonitorTaskHodoResetHistos = kFALSE;
  }  // if( fbMonitorMode && bCosy2019MonitorTaskHodoResetHistos )

  if (kFALSE == fMonitorAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index()
               << " in monitor algorithm class";
    return kTRUE;
  }  // if( kFALSE == fMonitorAlgo->ProcessTs( ts ) )
     /*
   /// Copy the digis in the DaqBuffer
   std::vector< CbmHodoDigi > vDigi = fMonitorAlgo->GetVector();

   for( auto digi: vDigi) {
     /// Insert data in output container
     LOG(debug) << "Fill digi TClonesarray with "
                << Form("0x%08x", digi.GetAddress())
                << " at " << static_cast<Int_t>( fHodoDigiCloneArray->GetEntriesFast() );

     new( (*fHodoDigiCloneArray)[ fHodoDigiCloneArray->GetEntriesFast() ] )
       CbmHodoDigi( digi) ;
   }

   vDigi.clear();
   fMonitorAlgo->ClearVector();
*/
  if (0 == fulTsCounter % 10000)
    LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  if (0 == fulTsCounter % 6000 && 0 < fulTsCounter) SaveHistograms();

  return kTRUE;
}

void CbmCosy2019MonitorTaskHodo::Reset() {
  //   fHodoDigiCloneArray->Clear();
}

void CbmCosy2019MonitorTaskHodo::Finish() {
  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    SaveHistograms();
  }  // if( kTRUE == fbMonitorMode )
}

Bool_t CbmCosy2019MonitorTaskHodo::SaveHistograms() {
  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos =
    fMonitorAlgo->GetHistoVector();
  std::vector<std::pair<TCanvas*, std::string>> vCanvas =
    fMonitorAlgo->GetCanvasVector();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile = nullptr;

  // open separate histo file in recreate mode
  histoFile = new TFile("data/HistosMonitorHodo.root", "RECREATE");

  if (nullptr == histoFile) return kFALSE;

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

  for (UInt_t uCanvas = 0; uCanvas < vCanvas.size(); ++uCanvas) {
    /// Make sure we end up in chosen folder
    TString sFolder = vCanvas[uCanvas].second.data();
    if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
    gDirectory->cd(sFolder);

    /// Write plot
    vCanvas[uCanvas].first->Write();

    histoFile->cd();
  }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  histoFile->Close();

  return kTRUE;
}

void CbmCosy2019MonitorTaskHodo::SetIgnoreOverlapMs(Bool_t bFlagIn) {
  fMonitorAlgo->SetIgnoreOverlapMs(bFlagIn);
}

void CbmCosy2019MonitorTaskHodo::SetTimeOffsetNs(Double_t dOffsetIn) {
  fMonitorAlgo->SetTimeOffsetNs(dOffsetIn);
}
void CbmCosy2019MonitorTaskHodo::SetDpbId(UInt_t uDpbId) {
  fMonitorAlgo->SetDpbId(uDpbId);
}
void CbmCosy2019MonitorTaskHodo::SetHodoElinkIdx(UInt_t uElinkHodoA,
                                                 UInt_t uElinkHodoB) {
  fMonitorAlgo->SetHodoElinkIdx(uElinkHodoA, uElinkHodoB);
}
void CbmCosy2019MonitorTaskHodo::SetHodoSwapXY(Bool_t bSwapHodoA,
                                               Bool_t bSwapHodoB) {
  fMonitorAlgo->SetHodoSwapXY(bSwapHodoA, bSwapHodoB);
}
void CbmCosy2019MonitorTaskHodo::SetHodoInvertX(Bool_t bInvHodoA,
                                                Bool_t bInvHodoB) {
  fMonitorAlgo->SetHodoInvertX(bInvHodoA, bInvHodoB);
}
void CbmCosy2019MonitorTaskHodo::SetHodoInvertY(Bool_t bInvHodoA,
                                                Bool_t bInvHodoB) {
  fMonitorAlgo->SetHodoInvertY(bInvHodoA, bInvHodoB);
}


ClassImp(CbmCosy2019MonitorTaskHodo)
