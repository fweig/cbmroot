// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmStar2019MonitorPulserTask                    -----
// -----               Created 12.10.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmStar2019MonitorPulserTask.h"

#include "CbmStar2019MonitorPulserAlgo.h"
#include "CbmStar2019TofPar.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRunOnline.h"
#include "FairParGenericSet.h"

#include "THttpServer.h"
#include "TROOT.h"
#include "TString.h"
#include "TClonesArray.h"
#include "TCanvas.h"
#include <TFile.h>

#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <fstream>
#include <chrono>

Bool_t bStar2019MonitorPulserTaskResetHistos = kFALSE;
Bool_t bStar2019MonitorPulserTaskSaveHistos = kFALSE;

CbmStar2019MonitorPulserTask::CbmStar2019MonitorPulserTask() :
   CbmMcbmUnpack(),
   fbEtofFeeIndexing( kTRUE ),
   fsHistoFileName( "data/HistosMonitorPulser.root" ),
   fuUpdateFreqTs( 100 ),
   fuPulserMinTot( 90 ),
   fuPulserMaxTot( 110 ),
   fuPulserChannel( 3 ),
   fiSectorIndex( -1 ),
   fuHistoryHistoSize( 1800 ),
   fParCList( nullptr ),
   fulTsCounter( 0 ),
   fMonitorPulserAlgo( nullptr )
{
   fMonitorPulserAlgo = new CbmStar2019MonitorPulserAlgo();
}

CbmStar2019MonitorPulserTask::~CbmStar2019MonitorPulserTask()
{
   delete fMonitorPulserAlgo;
}

Bool_t CbmStar2019MonitorPulserTask::Init()
{
   LOG(info) << "CbmStar2019MonitorPulserTask::Init";
   LOG(info) << "Initializing eTOF 2019 Monitor task";

   return kTRUE;
}

void CbmStar2019MonitorPulserTask::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();

   fParCList = fMonitorPulserAlgo->GetParList();

   for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
   {
      FairParGenericSet* tempObj = (FairParGenericSet*)(fParCList->At(iparC));
      fParCList->Remove(tempObj);

      std::string sParamName{ tempObj->GetName() };
      FairParGenericSet* newObj = dynamic_cast<FairParGenericSet*>( FairRun::Instance()->GetRuntimeDb()->getContainer( sParamName.data() ) );

      if( nullptr == newObj )
      {
         LOG(error) << "Failed to obtain parameter container " << sParamName
                    << ", for parameter index " << iparC;
         return;
      } // if( nullptr == newObj )

      fParCList->AddAt(newObj, iparC);
//      delete tempObj;
   } // for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
}

Bool_t CbmStar2019MonitorPulserTask::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();

   /// Control flags
   CbmStar2019TofPar * pUnpackPar = dynamic_cast<CbmStar2019TofPar*>( FairRun::Instance()->GetRuntimeDb()->getContainer( "CbmStar2019TofPar" ) );
   if( nullptr == pUnpackPar )
   {
      LOG(error) << "Failed to obtain parameter container CbmStar2019TofPar";
      return kFALSE;
   } // if( nullptr == pUnpackPar )

   /// Transfer parameter values set from calling macro
   fMonitorPulserAlgo->SetEtofFeeIndexing( fbEtofFeeIndexing );
   fMonitorPulserAlgo->SetSectorIndex( fiSectorIndex );
   fMonitorPulserAlgo->SetUpdateFreqTs( fuUpdateFreqTs );
   fMonitorPulserAlgo->SetPulserTotLimits( fuPulserMinTot, fuPulserMaxTot );
   fMonitorPulserAlgo->SetPulserChannel( fuPulserChannel );

   Bool_t initOK = fMonitorPulserAlgo->InitContainers();

   /// Histos creation, obtain pointer on them and add them to the HTTP server
      /// Trigger histo creation on all associated algos
   initOK &= fMonitorPulserAlgo->CreateHistograms();

      /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
   std::vector< std::pair< TNamed *, std::string > > vHistos = fMonitorPulserAlgo->GetHistoVector();
      /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
   std::vector< std::pair< TCanvas *, std::string > > vCanvases = fMonitorPulserAlgo->GetCanvasVector();

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

      server->RegisterCommand("/Reset_Pulser_Hist", "bStar2019MonitorPulserTaskResetHistos=kTRUE");
      server->RegisterCommand("/Save_Pulser_Hist", "bStar2019MonitorPulserTaskSaveHistos=kTRUE");

      server->Restrict("/Reset_Moni_Hist", "allow=admin");
      server->Restrict("/Save_Pulser_Hist", "allow=admin");
   } // if( nullptr != server )

   return initOK;
}

Bool_t CbmStar2019MonitorPulserTask::ReInitContainers()
{
   LOG(info) << "ReInit parameter containers for " << GetName();
   Bool_t initOK = fMonitorPulserAlgo->ReInitContainers();

   return initOK;
}

void CbmStar2019MonitorPulserTask::AddMsComponentToList( size_t component, UShort_t usDetectorId )
{
   fMonitorPulserAlgo->AddMsComponentToList( component, usDetectorId );
}

Bool_t CbmStar2019MonitorPulserTask::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
   if( bStar2019MonitorPulserTaskResetHistos )
   {
      LOG(info) << "Reset Monitor histos ";
      fMonitorPulserAlgo->ResetHistograms();
      bStar2019MonitorPulserTaskResetHistos = kFALSE;
   } // if( bStar2019MonitorPulserTaskResetHistos )

   if( bStar2019MonitorPulserTaskSaveHistos )
   {
      LOG(info) << "Save Monitor histos & canvases";
      SaveHistograms();
      bStar2019MonitorPulserTaskSaveHistos = kFALSE;
   } // if( bStar2019MonitorPulserTaskSaveHistos )

   if( kFALSE == fMonitorPulserAlgo->ProcessTs( ts ) )
   {
      LOG(error) << "Failed processing TS " << ts.index()
                 << " in unpacker algorithm class";
      return kTRUE;
   } // if( kFALSE == fMonitorPulserAlgo->ProcessTs( ts ) )

   /// Cleqr the digis vector in case it was filled
   std::vector< CbmTofDigi > vDigi = fMonitorPulserAlgo->GetVector();
   fMonitorPulserAlgo->ClearVector();

   if( 0 == fulTsCounter % 10000 )
      LOG(info) << "Processed " << fulTsCounter << "TS";
   fulTsCounter++;

   return kTRUE;
}

void CbmStar2019MonitorPulserTask::Reset()
{
}

void CbmStar2019MonitorPulserTask::Finish()
{
   fMonitorPulserAlgo->UpdateStats();
   SaveHistograms();
}

void CbmStar2019MonitorPulserTask::SetIgnoreOverlapMs( Bool_t bFlagIn )
{
   fMonitorPulserAlgo->SetIgnoreOverlapMs( bFlagIn );
}

Bool_t CbmStar2019MonitorPulserTask::SaveHistograms()
{
   /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
   std::vector< std::pair< TNamed *, std::string > > vHistos = fMonitorPulserAlgo->GetHistoVector();
   std::vector< std::pair< TCanvas *, std::string > > vCanvas = fMonitorPulserAlgo->GetCanvasVector();

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
ClassImp(CbmStar2019MonitorPulserTask)
