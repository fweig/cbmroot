// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018MonitorTaskTof                      -----
// -----               Created 27.11.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorTaskTof.h"

#include "CbmMcbm2018MonitorAlgoTof.h"
#include "CbmMcbm2018TofPar.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRunOnline.h"
#include "FairParGenericSet.h"

#include "THttpServer.h"
#include "TROOT.h"
#include "TString.h"
#include "TList.h"
#include "TCanvas.h"
#include <TFile.h>

#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <fstream>
#include <chrono>

Bool_t bMcbm2018MonitorTaskTofResetHistos = kFALSE;
Bool_t bMcbm2018MonitorTaskTofSaveHistos = kFALSE;

CbmMcbm2018MonitorTaskTof::CbmMcbm2018MonitorTaskTof() :
   CbmMcbmUnpack(),
   fbDebugMonitorMode( kFALSE ),
   fbIgnoreCriticalErrors( kFALSE ),
   fuHistoryHistoSize( 3600 ),
   fsHistoFileName( "data/HistosMonitor.root" ),
   fuMinTotPulser( 90 ),
   fuMaxTotPulser( 100 ),
   fiGdpbIndex( -1 ),
   fulTsCounter( 0 ),
   fMonitorAlgo( nullptr )
{
   fMonitorAlgo = new CbmMcbm2018MonitorAlgoTof();
}

CbmMcbm2018MonitorTaskTof::~CbmMcbm2018MonitorTaskTof()
{
   delete fMonitorAlgo;
}

Bool_t CbmMcbm2018MonitorTaskTof::Init()
{
   LOG(info) << "CbmMcbm2018MonitorTaskTof::Init";
   LOG(info) << "Initializing eTOF 2019 Monitor task";

   return kTRUE;
}

void CbmMcbm2018MonitorTaskTof::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();

   TList* parCList = fMonitorAlgo->GetParList();

   for( Int_t iparC = 0; iparC < parCList->GetEntries(); ++iparC )
   {
      FairParGenericSet* tempObj = (FairParGenericSet*)(parCList->At(iparC));
      parCList->Remove(tempObj);

      std::string sParamName{ tempObj->GetName() };
      FairParGenericSet* newObj = dynamic_cast<FairParGenericSet*>( FairRun::Instance()->GetRuntimeDb()->getContainer( sParamName.data() ) );

      if( nullptr == newObj )
      {
         LOG(error) << "Failed to obtain parameter container " << sParamName
                    << ", for parameter index " << iparC;
         return;
      } // if( nullptr == newObj )

      parCList->AddAt(newObj, iparC);
//      delete tempObj;
   } // for( Int_t iparC = 0; iparC < parCList->GetEntries(); ++iparC )
}

Bool_t CbmMcbm2018MonitorTaskTof::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();

   /// Control flags
   CbmMcbm2018TofPar * pUnpackPar = dynamic_cast<CbmMcbm2018TofPar*>( FairRun::Instance()->GetRuntimeDb()->getContainer( "CbmMcbm2018TofPar" ) );
   if( nullptr == pUnpackPar )
   {
      LOG(error) << "Failed to obtain parameter container CbmMcbm2018TofPar";
      return kFALSE;
   } // if( nullptr == pUnpackPar )

   /// Transfer parameter values set from calling macro
   fMonitorAlgo->SetDebugMonitorMode( fbDebugMonitorMode );
   fMonitorAlgo->SetIgnoreCriticalErrors( fbIgnoreCriticalErrors );
   fMonitorAlgo->SetHistoryHistoSize( fuHistoryHistoSize );
   fMonitorAlgo->SetPulserTotLimits( fuMinTotPulser, fuMaxTotPulser );
   fMonitorAlgo->SetGdpbIndex( fiGdpbIndex );

   Bool_t initOK = fMonitorAlgo->InitContainers();

   /// Histos creation, obtain pointer on them and add them to the HTTP server
      /// Trigger histo creation on all associated algos
   initOK &= fMonitorAlgo->CreateHistograms();

      /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
   std::vector< std::pair< TNamed *, std::string > > vHistos = fMonitorAlgo->GetHistoVector();
      /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
   std::vector< std::pair< TCanvas *, std::string > > vCanvases = fMonitorAlgo->GetCanvasVector();

      /// Register the histos in the HTTP server
   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
   if( nullptr != server )
   {
      for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )
      {
//         LOG(info) << "Registering  " << vHistos[ uHisto ].first->GetName()
//                   << " in " << vHistos[ uHisto ].second.data();
         server->Register( Form( "/%s", vHistos[ uHisto ].second.data() ), vHistos[ uHisto ].first );
      } // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

      for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )
      {
//         LOG(info) << "Registering  " << vCanvases[ uCanv ].first->GetName()
//                   << " in " << vCanvases[ uCanv ].second.data();
         server->Register( Form( "/%s", vCanvases[ uCanv ].second.data() ),
                                        gROOT->FindObject( (vCanvases[ uCanv ].first)->GetName() ) );
      } //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )

      server->RegisterCommand("/Reset_Moni_Hist", "bMcbm2018MonitorTaskTofResetHistos=kTRUE");
      server->RegisterCommand("/Save_Moni_Hist", "bMcbm2018MonitorTaskTofSaveHistos=kTRUE");

      server->Restrict("/Reset_Moni_Hist", "allow=admin");
   } // if( nullptr != server )

   return initOK;
}

Bool_t CbmMcbm2018MonitorTaskTof::ReInitContainers()
{
   LOG(info) << "ReInit parameter containers for " << GetName();
   Bool_t initOK = fMonitorAlgo->ReInitContainers();

   return initOK;
}

void CbmMcbm2018MonitorTaskTof::AddMsComponentToList( size_t component, UShort_t usDetectorId )
{
   fMonitorAlgo->AddMsComponentToList( component, usDetectorId );
}

Bool_t CbmMcbm2018MonitorTaskTof::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
   if( bMcbm2018MonitorTaskTofResetHistos )
   {
      LOG(info) << "Reset Monitor histos ";
      fMonitorAlgo->ResetHistograms();
      bMcbm2018MonitorTaskTofResetHistos = kFALSE;
   } // if( bMcbm2018MonitorTaskTofResetHistos )

   if( bMcbm2018MonitorTaskTofSaveHistos )
   {
      LOG(info) << "Save Monitor histos & canvases";
      SaveHistograms();
      bMcbm2018MonitorTaskTofSaveHistos = kFALSE;
   } // if( bMcbm2018MonitorTaskTofSaveHistos )

   if( kFALSE == fMonitorAlgo->ProcessTs( ts ) )
   {
      LOG(error) << "Failed processing TS " << ts.index()
                 << " in unpacker algorithm class";
      return kTRUE;
   } // if( kFALSE == fMonitorAlgo->ProcessTs( ts ) )

   /// Cleqr the digis vector in case it was filled
   std::vector< CbmTofDigi > vDigi = fMonitorAlgo->GetVector();
   fMonitorAlgo->ClearVector();

   if( 0 == fulTsCounter % 10000 )
      LOG(info) << "Processed " << fulTsCounter << "TS";
   fulTsCounter++;

   return kTRUE;
}

void CbmMcbm2018MonitorTaskTof::Reset()
{
}

void CbmMcbm2018MonitorTaskTof::Finish()
{
   SaveHistograms();
}

void CbmMcbm2018MonitorTaskTof::SetIgnoreOverlapMs( Bool_t bFlagIn )
{
   fMonitorAlgo->SetIgnoreOverlapMs( bFlagIn );
}

Bool_t CbmMcbm2018MonitorTaskTof::SaveHistograms()
{
   /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
   std::vector< std::pair< TNamed *, std::string > > vHistos = fMonitorAlgo->GetHistoVector();
   std::vector< std::pair< TCanvas *, std::string > > vCanvas = fMonitorAlgo->GetCanvasVector();

   /// (Re-)Create ROOT file to store the histos
   TDirectory * oldDir = NULL;
   TFile * histoFile = NULL;
   // Store current directory position to allow restore later
   oldDir = gDirectory;
   // open separate histo file in recreate mode
   histoFile = new TFile( fsHistoFileName , "RECREATE");

   if( nullptr == histoFile )
      return kFALSE;

   /// Register the histos in the HTTP server
   for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )
   {
      /// Make sure we end up in chosen folder
      TString sFolder = vHistos[ uHisto ].second.data();
      if( nullptr == gDirectory->Get( sFolder ) )
         gDirectory->mkdir( sFolder );
      gDirectory->cd( sFolder );

      /// Write plot
      vHistos[ uHisto ].first->Write();

      histoFile->cd();
   } // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

   for( UInt_t uCanvas = 0; uCanvas < vCanvas.size(); ++uCanvas )
   {
      /// Make sure we end up in chosen folder
      TString sFolder = vCanvas[ uCanvas ].second.data();
      if( nullptr == gDirectory->Get( sFolder ) )
         gDirectory->mkdir( sFolder );
      gDirectory->cd( sFolder );

      /// Write plot
      vCanvas[ uCanvas ].first->Write();

      histoFile->cd();
   } // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

  // Restore original directory position
  oldDir->cd();
  histoFile->Close();

  return kTRUE;
}
ClassImp(CbmMcbm2018MonitorTaskTof)
