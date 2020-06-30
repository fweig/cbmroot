// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018UnpackerTaskMuch                   -----
// -----               Created 02.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018UnpackerTaskMuch.h"

#include "CbmMcbm2018UnpackerAlgoMuch.h"
#include "CbmMcbm2018MuchPar.h"

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
#include <TFile.h>

#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <fstream>
#include <chrono>

Bool_t bMcbm2018UnpackerTaskMuchResetHistos = kFALSE;

CbmMcbm2018UnpackerTaskMuch::CbmMcbm2018UnpackerTaskMuch( UInt_t /*uNbGdpb*/ )
  : CbmMcbmUnpack(),
    fbMonitorMode( kFALSE ),
    fbWriteOutput( kTRUE ),
    fvChanMasks(),
    fulTsCounter( 0 ),
    fUnpackerAlgo( nullptr )
{
   fUnpackerAlgo = new CbmMcbm2018UnpackerAlgoMuch();
}

CbmMcbm2018UnpackerTaskMuch::~CbmMcbm2018UnpackerTaskMuch()
{
   delete fUnpackerAlgo;
}

Bool_t CbmMcbm2018UnpackerTaskMuch::Init()
{
   LOG(info) << "CbmMcbm2018UnpackerTaskMuch::Init";
   LOG(info) << "Initializing mCBM MUCH 2018 Unpacker";

   FairRootManager* ioman = FairRootManager::Instance();
   if( NULL == ioman )
   {
      LOG(fatal) << "No FairRootManager instance";
   }

   /// Get address of vector from algo
   fpvDigiMuch  = &( fUnpackerAlgo->GetVector() );
   ioman->RegisterAny( "MuchBeamTimeDigi", fpvDigiMuch,  fbWriteOutput);

   /// Get address of error vector from algo
   fpvErrorMuch = &( fUnpackerAlgo->GetErrorVector() );
   ioman->RegisterAny( "CbmMuchError", fpvErrorMuch,  fbWriteOutput);

   return kTRUE;
}

void CbmMcbm2018UnpackerTaskMuch::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();

   TList* fParCList = fUnpackerAlgo->GetParList();

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

Bool_t CbmMcbm2018UnpackerTaskMuch::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();

   /// Control flags
   CbmMcbm2018MuchPar * pUnpackPar = dynamic_cast<CbmMcbm2018MuchPar*>( FairRun::Instance()->GetRuntimeDb()->getContainer( "CbmMcbm2018MuchPar" ) );
   if( nullptr == pUnpackPar )
   {
      LOG(error) << "Failed to obtain parameter container CbmMcbm2018MuchPar";
      return kFALSE;
   } // if( nullptr == pUnpackPar )
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
   if( kTRUE == fbMonitorMode )
   {
      /// Trigger histo creation on all associated algos
      initOK &= fUnpackerAlgo->CreateHistograms();

      /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
      std::vector< std::pair< TNamed *, std::string > > vHistos = fUnpackerAlgo->GetHistoVector();

      /// Register the histos in the HTTP server
      THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
      if( nullptr != server )
      {
         for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )
         {
            server->Register( Form( "/%s", vHistos[ uHisto ].second.data() ), vHistos[ uHisto ].first );
         } // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

         server->RegisterCommand("/Reset_UnpMuch_Hist", "bMcbm2018UnpackerTaskMuchResetHistos=kTRUE");
         server->Restrict("/Reset_UnpMuch_Hist", "allow=admin");
      } // if( nullptr != server )

   } // if( kTRUE == fbMonitorMode )

   fUnpackerAlgo->SetMonitorMode( fbMonitorMode );

   for( std::vector< MuchFebChanMask >::iterator it = fvChanMasks.begin();
        it < fvChanMasks.end();
        ++it )
      fUnpackerAlgo->MaskNoisyChannel( (*it).uFeb, (*it).uChan, (*it).bMasked );

   return initOK;
}

Bool_t CbmMcbm2018UnpackerTaskMuch::ReInitContainers()
{
   LOG(info) << "ReInit parameter containers for " << GetName();
   Bool_t initOK = fUnpackerAlgo->ReInitContainers();

   return initOK;
}

void CbmMcbm2018UnpackerTaskMuch::AddMsComponentToList( size_t component, UShort_t usDetectorId )
{
   fUnpackerAlgo->AddMsComponentToList( component, usDetectorId );
}

Bool_t CbmMcbm2018UnpackerTaskMuch::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
   if( fbMonitorMode && bMcbm2018UnpackerTaskMuchResetHistos )
   {
      LOG(info) << "Reset MUCH unpacker histos ";
      fUnpackerAlgo->ResetHistograms();
      bMcbm2018UnpackerTaskMuchResetHistos = kFALSE;
   } // if( fbMonitorMode && bMcbm2018UnpackerTaskMuchResetHistos )

   if( kFALSE == fUnpackerAlgo->ProcessTs( ts ) )
   {
      LOG(error) << "Failed processing TS " << ts.index()
                 << " in unpacker algorithm class";
      return kTRUE;
   } // if( kFALSE == fUnpackerAlgo->ProcessTs( ts ) )

/*
   /// Sort the buffers of hits due to the time offsets applied
   => Done in the algo!!!
   sort(fpvDigiMuch->begin(), fpvDigiMuch->end(),
        [](const CbmMuchBeamTimeDigi & a, const CbmMuchBeamTimeDigi & b) -> bool
        {
          return a.GetTime() < b.GetTime();
        });
*/

   if( 0 == fulTsCounter % 10000 )
      LOG(info) << "Processed " << fulTsCounter << "TS";
   fulTsCounter++;

   return kTRUE;
}

void CbmMcbm2018UnpackerTaskMuch::Reset()
{
   fUnpackerAlgo->ClearVector();
   fUnpackerAlgo->ClearErrorVector();
}

void CbmMcbm2018UnpackerTaskMuch::Finish()
{
   /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
   if( kTRUE == fbMonitorMode )
   {
      /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
      std::vector< std::pair< TNamed *, std::string > > vHistos = fUnpackerAlgo->GetHistoVector();

      /// (Re-)Create ROOT file to store the histos
      TDirectory * oldDir = NULL;
      TFile * histoFile = NULL;
      // Store current directory position to allow restore later
      oldDir = gDirectory;
      // open separate histo file in recreate mode
      histoFile = new TFile( "data/HistosUnpackerMuch.root" , "RECREATE");
      histoFile->cd();

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

     // Restore original directory position
     oldDir->cd();
     histoFile->Close();
   } // if( kTRUE == fbMonitorMode )
}

void CbmMcbm2018UnpackerTaskMuch::SetIgnoreOverlapMs( Bool_t bFlagIn )
{
   fUnpackerAlgo->SetIgnoreOverlapMs( bFlagIn );
}

void CbmMcbm2018UnpackerTaskMuch::SetTimeOffsetNs( Double_t dOffsetIn )
{
   fUnpackerAlgo->SetTimeOffsetNs( dOffsetIn );
}

void CbmMcbm2018UnpackerTaskMuch::SetTimeOffsetNsAsic( UInt_t uAsicIdx, Double_t dOffsetIn )
{
   return fUnpackerAlgo->SetTimeOffsetNsAsic( uAsicIdx, dOffsetIn );
}

void CbmMcbm2018UnpackerTaskMuch::EnableAsicType( Int_t fiFlag )
{
   fUnpackerAlgo->EnableAsicType( fiFlag );
}

void CbmMcbm2018UnpackerTaskMuch::SetBinningFwFlag( Bool_t bEnable )
{
   fUnpackerAlgo->SetBinningFwFlag( bEnable );
}

void CbmMcbm2018UnpackerTaskMuch::MaskNoisyChannel( UInt_t uFeb, UInt_t uChan, Bool_t bMasked )
{

   fvChanMasks.push_back( MuchFebChanMask{ uFeb, uChan, bMasked } );

}

void CbmMcbm2018UnpackerTaskMuch::SetAdcCut( UInt_t uAdc )
{
   fUnpackerAlgo->SetAdcCut( uAdc );
}


ClassImp(CbmMcbm2018UnpackerTaskMuch)
