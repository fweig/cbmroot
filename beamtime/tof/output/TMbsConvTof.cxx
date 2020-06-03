// ------------------------------------------------------------------
// -----                    TMbsConvTof                       -----
// -----              Created 11/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TMbsConvTof.h"

// General Unpack headers
#include "TMbsUnpackTofPar.h"

// GO4 unpacker headers
#include "TVftxBoardData.h"
#include "Plastics_Event.h"
#include "Rpc_Event.h"
#include "Scalers_Event.h"

// ToF specific headers
#include "TMbsConvTofPar.h"
#include "TMbsMappingTofPar.h"
#include "TMbsCalibTofPar.h"
   //General
#include "TofDef.h"
   // Scalers
#include "TofScalerDef.h"
#include "TTofScalerBoard.h"
#include "TTofTriglogBoard.h"
#include "TTofCalibScaler.h"
   // TDCs
#include "TofTdcDef.h"
#include "TofCaenDef.h"
#include "TofVftxDef.h"
#include "TofTrbTdcDef.h"
#include "TofGet4Def.h"
#include "TTofTdcBoard.h"
#include "TTofTdcData.h"
#include "TTofCalibData.h"
   // Digis
#include "CbmTofAddress.h"

// FAIR headers
#include "FairLogger.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"

// ROOT headers
#include "TClonesArray.h"
#include "TH2.h"
#include "TH1.h"
#include "TROOT.h"
#include "TTimeStamp.h"
#include "TFile.h"

// C++ headers
#include <algorithm>
      
TMbsConvTof::TMbsConvTof() : 
   FairTask("MbsConvTof"),
   fMbsUnpackPar(NULL),
   fMbsCalibPar(NULL),
   fMbsMappingPar(NULL),
   fMbsConvPar(NULL),
   fhRpcHitPosition(),
   fhRpcTotComp(),
   fhPlasticHitPosition(),
   fhPlasticTotComp(),
   fTriglogBoardCollection(NULL),
   fScalerBoardCollection(NULL),
   fCaenBoardCollection(NULL),
   fVftxBoardCollection(NULL),
   fGet4BoardCollection(NULL),
   fCalibDataCollection(NULL),
   fCalibScalCollection(NULL),
   fCbmTofDigiCollection(NULL),
   fRateDataCollection(NULL),
   sOutputFilename(""),
   fOutputFile(NULL),
   fOutputTree(NULL),
   uTriglogSyncNb(0),
   uTriglogPattern(0),
   uTriglogInputPattern(0),
   uTriglogTimeSec(0),
   TriglogTimeMilsec(0),
   fvVftxCalibBoards(),
   fvScalerEvents(),
   fvbMappedDet(),
   fviNbChannels(),
   fvClassedDigis(),
   fvPlasticHits(),
   fvRpcHits(),
   fvVftxBoards(),
   fiNbEvents(0)
{
}

TMbsConvTof::TMbsConvTof(const char* name, Int_t /*mode*/, Int_t verbose) :
   FairTask(name, verbose),
   fMbsUnpackPar(0),
   fMbsCalibPar(NULL),
   fMbsMappingPar(NULL),
   fMbsConvPar(NULL),
   fhRpcHitPosition(),
   fhRpcTotComp(),
   fhPlasticHitPosition(),
   fhPlasticTotComp(),
   fTriglogBoardCollection(NULL),
   fScalerBoardCollection(NULL),
   fCaenBoardCollection(NULL),
   fVftxBoardCollection(NULL),
   fGet4BoardCollection(NULL),
   fCalibDataCollection(NULL),
   fCalibScalCollection(NULL),
   fCbmTofDigiCollection(NULL),
   fRateDataCollection(NULL),
   sOutputFilename(""),
   fOutputFile(NULL),
   fOutputTree(NULL),
   uTriglogSyncNb(0),
   uTriglogPattern(0),
   uTriglogInputPattern(0),
   uTriglogTimeSec(0),
   TriglogTimeMilsec(0),
   fvVftxCalibBoards(),
   fvScalerEvents(),
   fvbMappedDet(),
   fviNbChannels(),
   fvClassedDigis(),
   fvPlasticHits(),
   fvRpcHits(),
   fvVftxBoards(),
   fiNbEvents(0)
{
}
      
TMbsConvTof::~TMbsConvTof()
{
   DeleteHistograms();
   LOG(info)<<"**** TMbsConvTof: Delete instance ";
}

// --------------------------------------------------
// Fairtask specific functions
void TMbsConvTof::SetParContainers()
{
   InitParameters();
}

InitStatus TMbsConvTof::ReInit()
{
   LOG(info)<<"**** TMbsConvTof: Reinitialize the conversion parameters for tof ";
   if( kFALSE == InitParameters() )
      return kFATAL;
   fMbsConvPar->printParams();
   
   return kSUCCESS;
}

InitStatus TMbsConvTof::Init()
{
   if( kFALSE == InitParameters() )
      return kFATAL;
   fMbsConvPar->printParams();
   if( kFALSE == CreateHistogramms() )
      return kFATAL;   
      
   if( kFALSE == RegisterInput() )
      return kFATAL;   
   
   if( kFALSE == InitTempDigiStorage() )
      return kFATAL;
      
   // Create the proper output objects for each detector
   if( kFALSE == RegisterOutput() )
      return kFATAL;
      
   fiNbEvents = 0;
   
   return kSUCCESS;
}
void TMbsConvTof::Exec(Option_t* /*option*/)
{
   ClearOutput();
   ClearTriglogData();
   ClearVftxBoards();
   ClearScalerEvent();
   ClearTempDigiStorage();
   
   // Event rejection
   // E.g.: pulser event used to Empty the VFTX buffers
   if( 1 == fMbsUnpackPar->OnlyOneTriglog() &&
       -1 < fMbsUnpackPar->GetTriggerToReject() )
   {
//      if( NULL == fTriglogBoardCollection )
//         return kFALSE;

      TTofTriglogBoard * xTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->At(0); // Always only 1 TRIGLOG board!

//      if( NULL == xTriglogBoard )
//         return kFALSE;
//      if( 1 == ( 0x1 & ( (xTriglogBoard->GetTriggPatt()) >> (fMbsUnpackPar->GetTriggerToReject()) ) ) )
      // Reject selected trigger only when alone (accept when both not rejected trigger and rejected trigger are present)
      if( (xTriglogBoard->GetTriggPatt()) == ( 1ul << (fMbsUnpackPar->GetTriggerToReject()) ) )
         // Jump this event !
         return;
   } // if trigger rejection enabled and trigger board enabled

   FillTriglogData();
   FillVftxBoards();
   
   FillCalibScaler();
   FillMappedScaler();
      
   FillTempDigiStorage();
   BuildFiredChannels();
   
   FillOutput();
   
   FillHistograms();
   fiNbEvents++;
}
void TMbsConvTof::Finish()
{
   WriteHistogramms();
   
   DeleteTempDigiStorage();
   
   DeleteOutput();
}

// ------------------------------------------------------------------
Bool_t TMbsConvTof::InitParameters()
{
   // Get Base Container
   FairRun* ana = FairRun::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   // Unpack parameter
   fMbsUnpackPar = (TMbsUnpackTofPar*) (rtdb->getContainer("TMbsUnpackTofPar"));
   if( 0 == fMbsUnpackPar )
   {
      LOG(error)<<"TMbsConvTof::InitParameters => Could not obtain the TMbsUnpackTofPar ";
      return kFALSE;
   }
      
   // Calibration parameter
   fMbsCalibPar = (TMbsCalibTofPar*) (rtdb->getContainer("TMbsCalibTofPar"));
   if( 0 == fMbsCalibPar )
   {
      LOG(error)<<"TMbsConvTof::InitParameters => Could not obtain the TMbsCalibTofPar ";
      return kFALSE; 
   }
      
   // Calibration parameter
   fMbsMappingPar = (TMbsMappingTofPar*) (rtdb->getContainer("TMbsMappingTofPar"));
   if( 0 == fMbsMappingPar )
   {
      LOG(error)<<"TMbsConvTof::InitParameters => Could not obtain the TMbsMappingTofPar ";
      return kFALSE; 
   }
      
   // Conversion parameter
   fMbsConvPar = (TMbsConvTofPar*) (rtdb->getContainer("TMbsConvTofPar"));
   if( 0 == fMbsConvPar )
   {
      LOG(error)<<"TMbsConvTof::InitParameters => Could not obtain the TMbsConvTofPar ";
      return kFALSE; 
   }

   // PAL: Added to recover loading of parameters before initialization
//   rtdb->initContainers(  ana->GetRunId() );
   
   return kTRUE;
}
// ------------------------------------------------------------------
Bool_t TMbsConvTof::CreateHistogramms()
{
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !
   
   if( 0 < fMbsConvPar->GetNbRpc() )
   {
      fhRpcHitPosition.resize(  fMbsConvPar->GetNbRpc() );
      fhRpcTotComp.resize(      fMbsConvPar->GetNbRpc() );
      for( Int_t iRpcIndx = 0; iRpcIndx < fMbsConvPar->GetNbRpc(); iRpcIndx++ )
      {
         switch( fMbsConvPar->GetRpcChType( iRpcIndx ) )
         {
            case 1: // PADs
            {
               Int_t iUniqueId = fMbsConvPar->GetRpcUid( iRpcIndx );
               Int_t iSmType   = CbmTofAddress::GetSmType( iUniqueId );
               Int_t iSmId     = CbmTofAddress::GetSmId( iUniqueId );
               Int_t iRpcId    = CbmTofAddress::GetRpcId( iUniqueId );
               fhRpcHitPosition[iRpcIndx] =  new TH2I( 
                           Form("tof_ty%01d_sm%03d_rpc%03d_hits_map", iSmType, iSmId, iRpcId ),
                           Form("Hit distribution on Rpc #%03d in Sm %03d of type %d; Column []; Row []", 
                                 iRpcId, iSmId, iSmType ),
                           fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType),
                           2, 0, 2 ); 
               break;
            }
            case 2: // STRIPs
            {
               Int_t iUniqueId = fMbsConvPar->GetRpcUid( iRpcIndx );
               Int_t iSmType   = CbmTofAddress::GetSmType( iUniqueId );
               Int_t iSmId     = CbmTofAddress::GetSmId( iUniqueId );
               Int_t iRpcId    = CbmTofAddress::GetRpcId( iUniqueId );
               fhRpcHitPosition[iRpcIndx] =  new TH2I( 
                           Form("tof_ty%01d_sm%03d_rpc%03d_hits_map", iSmType, iSmId, iRpcId ),
                           Form("Hit distribution on Rpc #%03d in Sm %03d of type %d; Channel []; Time difference [ps]", 
                                 iRpcId, iSmId, iSmType ),
                           fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType),
                           2000, -10000, 10000 );
               fhRpcTotComp[iRpcIndx] =  new TH2I( 
                           Form("tof_ty%01d_sm%03d_rpc%03d_tot_comp", iSmType, iSmId, iRpcId ),
                           Form("Comparison of TOT on each channel end on Rpc #%03d in Sm %03d of type %d; Tot left side []; Tot right side [ps]", 
                                 iRpcId, iSmId, iSmType ),
                           550, -5000.0, 50000.0,
                           550, -5000.0, 50000.0 );
               break;
            }
            default:
               break;
         } // switch( fMbsConvPar->GetRpcChType(iRpcIndx) )
      } // for( Int_t iRpcIndx = 0; iRpcIndx < fMbsConvPar->GetNbRpc(); iRpcIndx++ )
   } // if( 0 < fMbsConvPar->GetNbRpc() )
   if( 0 < fMbsConvPar->GetNbPlastic() )
   {
      fhPlasticHitPosition.resize(  fMbsConvPar->GetNbPlastic() );
      fhPlasticTotComp.resize(      fMbsConvPar->GetNbPlastic() );
      for( Int_t iPlastIndx = 0; iPlastIndx < fMbsConvPar->GetNbPlastic(); iPlastIndx++ )
      {
         switch( fMbsConvPar->GetPlasticEndNb( iPlastIndx ) )
         {
            case 2: // Both ends
            {
               Int_t iUniqueId = fMbsConvPar->GetPlasticUid( iPlastIndx );
               Int_t iSmType   = CbmTofAddress::GetSmType( iUniqueId );
               Int_t iSmId     = CbmTofAddress::GetSmId( iUniqueId );
               Int_t iPlaId    = CbmTofAddress::GetRpcId( iUniqueId );
               fhPlasticHitPosition[iPlastIndx] =  new TH1I( 
                           Form("tof_ty%01d_sm%03d_pla%03d_hits_map", iSmType, iSmId, iPlaId ),
                           Form("Hit distribution on Plastic #%03d in Sm %03d of type %d; Time difference [ps]", 
                                 iPlaId, iSmId, iSmType ),
                           2000, -10000, 10000 );
               fhPlasticTotComp[iPlastIndx] =  new TH2I( 
                           Form("tof_ty%01d_sm%03d_pla%03d_tot_comp", iSmType, iSmId, iPlaId ),
                           Form("Comparison of TOT on each channel end on Plastic #%03d in Sm %03d of type %d; Tot left side []; Tot right side [ps]", 
                                 iPlaId, iSmId, iSmType ),
                           550, -5000.0, 50000.0,
                           550, -5000.0, 50000.0 );
               break;
            }
            case 1: // Single ended
            default:
               break;
         } // switch( fMbsConvPar->GetPlasticEndNb(iPlastIndx) )
      } // for( Int_t iPlastIndx = 0; iPlastIndx < fMbsConvPar->GetNbPlastic(); iPlastIndx++ )
   } // if( 0 < fMbsConvPar->GetNbPlastic() )
   
   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
   
   return kTRUE;
}
Bool_t TMbsConvTof::FillHistograms()
{   
   if( 0 != FairRunAna::Instance() )
   {
      // Get Base Container <- Needed because currently RunOnLmd runs 1 more time after setting end of file flag!!!!!
      // Should not be a problem when using the FAIRROOT way of reading MBS event instead of the custom "Go4 like" way
      FairRunAna* ana = FairRunAna::Instance(); // <- Needed because currently RunOnLmd runs 1 more time after setting end of file flag!!!!!!
      // Needed because currently RunOnLmd runs 1 more time after setting end of file flag!!!!!!
      if( kTRUE == ana->GetLMDProcessingStatus() ) 
         return kFALSE;
   } // if( 0 != FairRunAna::Instance() )
   
   for( Int_t iRpcIndx = 0; iRpcIndx < fMbsConvPar->GetNbRpc(); iRpcIndx++ )
   {
      switch( fMbsConvPar->GetRpcChType( iRpcIndx ) )
      {
         case 1: // PADs
         {
            Int_t iUniqueId = fMbsConvPar->GetRpcUid( iRpcIndx );
            Int_t iSmType   = CbmTofAddress::GetSmType( iUniqueId );
            for( UInt_t uHitInd = 0; uHitInd < fvRpcHits[iRpcIndx].size(); uHitInd ++)
               fhRpcHitPosition[iRpcIndx]->Fill( fvRpcHits[iRpcIndx][uHitInd].iStrip % fMbsMappingPar->GetSmTypeNbCh(iSmType),  // Column
                                                 fvRpcHits[iRpcIndx][uHitInd].iStrip / fMbsMappingPar->GetSmTypeNbCh(iSmType) );// Row
            break;
         }
         case 2: // STRIPs
         {
            for( UInt_t uHitInd = 0; uHitInd < fvRpcHits[iRpcIndx].size(); uHitInd ++)
            {
               fhRpcHitPosition[iRpcIndx]->Fill( fvRpcHits[iRpcIndx][uHitInd].iStrip,
                                                   fvRpcHits[iRpcIndx][uHitInd].dTimeRight
                                                 - fvRpcHits[iRpcIndx][uHitInd].dTimeLeft  );
               fhRpcTotComp[iRpcIndx]->Fill( fvRpcHits[iRpcIndx][uHitInd].dTotLeft,
                                             fvRpcHits[iRpcIndx][uHitInd].dTotRight );
            } // for( UInt_t uHitInd = 0; uHitInd < fvRpcHits[iRpcIndx].size(); uHitInd ++)
            break;
         }
         default:
            break;
      } // switch( fMbsConvPar->GetRpcChType( iRpcIndx ) )
   } // for( Int_t iRpcIndx = 0; iRpcIndx < fMbsConvPar->GetNbRpc(); iRpcIndx++ )
   for( Int_t iPlastIndx = 0; iPlastIndx < fMbsConvPar->GetNbPlastic(); iPlastIndx++ )
   {
      switch( fMbsConvPar->GetPlasticEndNb( iPlastIndx ) )
      {
         case 2: // Both ends
         {
            for( UInt_t uHitInd = 0; uHitInd < fvPlasticHits[iPlastIndx].size(); uHitInd ++)
            {
               fhPlasticHitPosition[iPlastIndx]->Fill( fvPlasticHits[iPlastIndx][uHitInd].dTimeRight
                                                       - fvPlasticHits[iPlastIndx][uHitInd].dTimeLeft  );
               fhPlasticTotComp[iPlastIndx]->Fill( fvPlasticHits[iPlastIndx][uHitInd].dTotLeft,
                                                   fvPlasticHits[iPlastIndx][uHitInd].dTotRight );
            } // for( UInt_t uHitInd = 0; uHitInd < fvPlasticHits[iPlastIndx].size(); uHitInd ++)
            break;
         }
         case 1: // Single ended
         default:
            break;
      } // switch( fMbsConvPar->GetRpcChType( iRpcIndx ) )
   } // for( Int_t iPlastIndx = 0; iPlastIndx < fMbsConvPar->GetNbPlastic(); iPlastIndx++ )
   return kTRUE;
}
void TMbsConvTof::WriteHistogramms()
{
   TDirectory * oldir = gDirectory;
   TFile *fHist = new TFile("./tofMbsConv.hst.root","RECREATE");
   
   if( 0 < fMbsConvPar->GetNbRpc() )
   {
      // create a subdirectory for each histogram type in this file
      TDirectory *cdConvRpc = fHist->mkdir( "Rpc" );
      cdConvRpc->cd();
      for( Int_t iRpcIndx = 0; iRpcIndx < fMbsConvPar->GetNbRpc(); iRpcIndx++ )
      {
         fhRpcHitPosition[iRpcIndx]->Write();
         if( 2 == fMbsConvPar->GetRpcChType( iRpcIndx ) )
               fhRpcTotComp[iRpcIndx]->Write();
      } // for( Int_t iRpcIndx = 0; iRpcIndx < fMbsConvPar->GetNbRpc(); iRpcIndx++ )
   } // if( 0 < fMbsConvPar->GetNbRpc() )
   
   if( 0 < fMbsConvPar->GetNbPlastic() )
   {
      // create a subdirectory for each histogram type in this file
      TDirectory *cdConvPla = fHist->mkdir( "Plastics" );
      cdConvPla->cd();
      for( Int_t iPlastIndx = 0; iPlastIndx < fMbsConvPar->GetNbPlastic(); iPlastIndx++ )
      {
         if( 2 == fMbsConvPar->GetPlasticEndNb( iPlastIndx ) )
         {
            fhPlasticHitPosition[iPlastIndx]->Write();
            fhPlasticTotComp[iPlastIndx]->Write();
         }
      } // for( Int_t iPlastIndx = 0; iPlastIndx < fMbsConvPar->GetNbPlastic(); iPlastIndx++ )
   } // if( 0 < fMbsConvPar->GetNbPlastic() )
   
   gDirectory->cd( oldir->GetPath() );
   
   fHist->Close();
}
void TMbsConvTof::DeleteHistograms()
{
}
// ------------------------------------------------------------------
Bool_t TMbsConvTof::RegisterInput()
{
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   // Obtain Triglog board object
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) &&
         ( kTRUE == fMbsConvPar->TriglogEnabled() || -1 < fMbsUnpackPar->GetTriggerToReject() ) )
   {
      fTriglogBoardCollection = (TClonesArray*) rootMgr->GetObject("TofTriglog");
      if( NULL == fTriglogBoardCollection) 
      {
         LOG(error)<<"TMbsConvTof::RegisterInput => Could not get the TofTriglog TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fTriglogBoardCollection) 
   } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( toftdc::triglog ) && kTRUE == fMbsConvPar->TriglogEnabled() )
   
   // Obtain VFTX boards and calibrated TDC objects
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::vftx ) && 0 < fMbsConvPar->GetNbOutVftx() )
   {
      fVftxBoardCollection = (TClonesArray*) rootMgr->GetObject("TofVftxTdc");
      if( NULL == fVftxBoardCollection) 
      {
         LOG(error)<<"TMbsConvTof::RegisterInput => Could not get the TofVftxTdc TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fVftxBoardCollection) 
      fCalibDataCollection = (TClonesArray*) rootMgr->GetObject("TofCalibData");
      if( NULL == fCalibDataCollection) 
      {
         LOG(error)<<"TMbsConvTof::RegisterInput => Could not get the TofCalibData TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fCalibDataCollection) 
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards() )
   
   // Obtain Calibrated scalers objects
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() && 0 < fMbsConvPar->GetNbOutScal() )
   {
      fCalibScalCollection = (TClonesArray*) rootMgr->GetObject("TofCalibScaler");
      if( NULL == fCalibScalCollection) 
      {
         LOG(error)<<"TMbsConvTof::RegisterInput => Could not get the TofCalibScaler TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fCalibScalCollection) 
   } // if( 0 < fMbsUnpackPar->GetNbActiveScalersB() && 0 < fMbsConvPar->GetNbOutScal() )
   
   // Obtain Digi collection object
   if( 0 < fMbsConvPar->GetNbRpc() || 0 < fMbsConvPar->GetNbPlastic() )
   {
      fCbmTofDigiCollection = (TClonesArray*) rootMgr->GetObject("CbmTofDigi");
      if( NULL == fCbmTofDigiCollection) 
      {
         LOG(error)<<"TMbsConvTof::RegisterInput => Could not get the CbmTofDigi TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fCbmTofDigiCollection) 
   }
   
   return kTRUE;
}
Bool_t TMbsConvTof::RegisterOutput()
{
   TDirectory* oldDir;
   
   oldDir = gDirectory;

   sOutputFilename = fMbsConvPar->GetOutFilename();
   fOutputFile = new TFile(sOutputFilename, "RECREATE", "TTree file in format similar to GO4 unpacker", 9);
   sOutputFilename += ":/";
   if( fOutputFile->IsZombie() )
   {
      LOG(error)<<"TMbsConvTof::RegisterOutput => Error opening file for TTree output!!!";
      return kFALSE;
   } // if (fOutputFile->IsZombie())

   gDirectory->Cd(sOutputFilename);
   
   fOutputTree = new TTree("GsiNov12", "GSI November 2012 data format");
   LOG(info)<<"TMbsConvTof::RegisterOutput => Output Tree created in "<<gDirectory->GetPath();
   
   // TODO: Triglog, scalers
   // TRIGLOG
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) && kTRUE == fMbsConvPar->TriglogEnabled() )
   {
      fOutputTree->Branch("TriglogSyncNb",       &uTriglogSyncNb,       "TriglogSyncNb/i"  );
      fOutputTree->Branch("TriglogPattern",      &uTriglogPattern,      "TriglogPattern/i"  );
      fOutputTree->Branch("TriglogInputPattern", &uTriglogInputPattern, "TriglogInputPattern/i"  );
      fOutputTree->Branch("TriglogTimeSec",      &uTriglogTimeSec,      "TriglogTimeSec/i"  );
      fOutputTree->Branch("TriglogTimeMilsec",   &TriglogTimeMilsec,    "TriglogTimeMilsec/i"  );
   } // if( kTRUE == fMbsConvPar->TriglogEnabled() )
   
   // VFTX
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::vftx ) && 0 < fMbsConvPar->GetNbOutVftx()  )
   {
      Int_t iNbVftx = fMbsUnpackPar->GetNbActiveBoards( toftdc::vftx );
      fvVftxCalibBoards.resize( iNbVftx );
      
      for( Int_t iVftxIndex = 0; iVftxIndex < fMbsConvPar->GetNbOutVftx(); iVftxIndex ++)
         if( fMbsConvPar->GetVftxInd( iVftxIndex ) < iNbVftx )
         {
            fOutputTree->Branch( Form("Vftx%02dData.", iVftxIndex), "TVftxBoardData",
                                 &(fvVftxCalibBoards[ fMbsConvPar->GetVftxInd( iVftxIndex ) ]) );
         }
   }
   
   // SCALERS
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() && 0 < fMbsConvPar->GetNbOutScal() )
   {
      fvScalerEvents.resize( fMbsConvPar->GetNbOutScal() );
      
      for( Int_t iScalEvtIdx = 0; iScalEvtIdx < fMbsConvPar->GetNbOutScal(); iScalEvtIdx ++)
      {
/*         Scalers_Event * test = &(fvScalerEvents[iScalEvtIdx]);*/
         fOutputTree->Branch( Form("CalScal%02d.", iScalEvtIdx), "Scalers_Event", 
                              &( fvScalerEvents[iScalEvtIdx] ) );
      }
   } // if( 0 < fMbsUnpackPar->GetNbActiveScalersB() && 0 < fMbsConvPar->GetNbOutScal() )
   
   // RPC, PLASTICS
   if( 0 < fMbsConvPar->GetNbDetectors() )
   {
      if( 0 < fMbsConvPar->GetNbPlastic() )
         fvPlasticHits.resize( fMbsConvPar->GetNbPlastic() );
      if( 0 < fMbsConvPar->GetNbRpc() )
         fvRpcHits.resize( fMbsConvPar->GetNbRpc() );
         
      for( Int_t iRpcIndex = 0; iRpcIndex < fMbsConvPar->GetNbRpc(); iRpcIndex ++)
         fOutputTree->Branch( Form("Rpc%02dHits", iRpcIndex),  &( fvRpcHits[iRpcIndex] ) );
                              
      for( Int_t iPlasticIndex = 0; iPlasticIndex < fMbsConvPar->GetNbPlastic(); iPlasticIndex ++)
         fOutputTree->Branch( Form("Plastics%02dHits", iPlasticIndex), &( fvPlasticHits[iPlasticIndex] ) );
   } // if( 0 < fMbsConvPar->GetNbDetectors() )
            
   fOutputTree->SetDirectory(gDirectory);
   
   gDirectory->cd( oldDir->GetPath() );
   
   return kTRUE;
}
Bool_t TMbsConvTof::FillOutput()
{
   fOutputTree->Fill();
   return kTRUE;
}
Bool_t TMbsConvTof::ClearOutput()
{
   // Clear the output vectors
   if( 0 < fMbsConvPar->GetNbDetectors() )
   {
      for( Int_t iRpcIndx = 0; iRpcIndx < fMbsConvPar->GetNbRpc(); iRpcIndx++ )
         fvRpcHits[iRpcIndx].clear();
      for( Int_t iPlastIndx = 0; iPlastIndx < fMbsConvPar->GetNbPlastic(); iPlastIndx++ )
         fvPlasticHits[iPlastIndx].clear();
   } // if( 0 < fMbsConvPar->GetNbDetectors() )
   
   return kTRUE;
}
Bool_t TMbsConvTof::DeleteOutput()
{
   TDirectory* oldDir;
   oldDir = gDirectory;
   gDirectory->Cd(sOutputFilename);
   fOutputTree->SetDirectory(gDirectory);
   
   fOutputFile->Write("",TObject::kOverwrite);
   
   gDirectory->cd( oldDir->GetPath() );
   fOutputTree->SetDirectory(gDirectory);
   fOutputFile->Close();
   delete fOutputTree;
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( toftdc::vftx ) && 0 < fMbsConvPar->GetNbOutVftx()  )
      fvVftxCalibBoards.clear();
      
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() && 0 < fMbsConvPar->GetNbOutScal() )
      fvScalerEvents.clear();
      
   // Clear the output vectors
   if( 0 < fMbsConvPar->GetNbDetectors() )
   {
      for( Int_t iRpcIndx = 0; iRpcIndx < fMbsConvPar->GetNbRpc(); iRpcIndx++ )
            fvRpcHits[iRpcIndx].clear();
      fvRpcHits.clear();
      
      for( Int_t iPlastIndx = 0; iPlastIndx < fMbsConvPar->GetNbPlastic(); iPlastIndx++ )
         fvPlasticHits[iPlastIndx].clear();
      fvPlasticHits.clear();
   } // if( 0 < fMbsConvPar->GetNbDetectors() )
   
   return kTRUE;
}
// ------------------------------------------------------------------ 
// Just filling temporary holders for TRIGlOG data
Bool_t TMbsConvTof::FillTriglogData()
{
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) && kTRUE == fMbsConvPar->TriglogEnabled() )
   {
      if( NULL == fTriglogBoardCollection )
         return kFALSE;
            
      TTofTriglogBoard * xTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->At(0); // Always only 1 TRIGLOG board!
         
      if( NULL == xTriglogBoard )
         return kFALSE;
            
      uTriglogSyncNb       = xTriglogBoard->GetSyncNb();      
      uTriglogPattern      = xTriglogBoard->GetTriggPatt();      
      uTriglogInputPattern = xTriglogBoard->GetInpPatt();      
      uTriglogTimeSec      = xTriglogBoard->GetMbsTimeSec();      
      TriglogTimeMilsec    = xTriglogBoard->GetMbsTimeMilliSec();
   } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( toftdc::triglog ) && kTRUE == fMbsConvPar->TriglogEnabled() )
   
   return kTRUE;
}
Bool_t TMbsConvTof::ClearTriglogData()
{
   uTriglogSyncNb       = 0;      
   uTriglogPattern      = 0;      
   uTriglogInputPattern = 0;      
   uTriglogTimeSec      = 0;      
   TriglogTimeMilsec    = 0;
   
   return kTRUE;
}
// ------------------------------------------------------------------ 
// VFTX calibrated TDC boards conversion
Bool_t TMbsConvTof::FillVftxBoards()
{
   if( 0 != FairRunAna::Instance() )
   {
      // Get Base Container <- Needed because currently RunOnLmd runs 1 more time after setting end of file flag!!!!!
      // Should not be a problem when using the FAIRROOT way of reading MBS event instead of the custom "Go4 like" way
      FairRunAna* ana = FairRunAna::Instance(); // <- Needed because currently RunOnLmd runs 1 more time after setting end of file flag!!!!!!
      // Needed because currently RunOnLmd runs 1 more time after setting end of file flag!!!!!!
      if( kTRUE == ana->GetLMDProcessingStatus() ) 
         return kFALSE;
   } // if( 0 != FairRunAna::Instance() )
      
   // Board data
   for( Int_t iVftxIndex = 0; iVftxIndex < fMbsConvPar->GetNbOutVftx(); iVftxIndex ++)
      if( static_cast<UInt_t>(fMbsConvPar->GetVftxInd( iVftxIndex )) < fMbsUnpackPar->GetNbActiveBoards( toftdc::vftx ) )
      {
         Int_t iInputVftxInd = fMbsConvPar->GetVftxInd( iVftxIndex );
         TTofTdcBoard * vftxPtr = ( TTofTdcBoard * )fVftxBoardCollection->At( iInputVftxInd );
         fvVftxCalibBoards[ iInputVftxInd ].iTriggerTime =  vftxPtr->GetTriggerTime();
      } // if( fMbsConvPar->GetVftxInd( iVftxIndex ) < fMbsUnpackPar->GetNbActiveBoards( toftdc::vftx ) )
      
   // Channels data
   for( Int_t iCalibDatInd = 0; iCalibDatInd < fCalibDataCollection->GetEntriesFast(); iCalibDatInd++ )
   {
      TTofCalibData * calibPtr = ( TTofCalibData * )fCalibDataCollection->At( iCalibDatInd );
      UInt_t   uBoard = calibPtr->GetBoard();
      if( toftdc::vftx == calibPtr->GetType() && -1 < fMbsConvPar->GetVftxOutInd( uBoard ) )
      {
         UInt_t   uCh    = calibPtr->GetChannel();
         switch( calibPtr->GetEdge() )
         {
            case 0: // 0 = Rising
               if( fvVftxCalibBoards[uBoard].iMultiplicity[uCh] < TVftxBoardData::MaxMult )
               {
                  fvVftxCalibBoards[uBoard].iMultiplicity[uCh] ++;
                  fvVftxCalibBoards[uBoard].dTimeCorr[uCh]
                                            [ fvVftxCalibBoards[uBoard].iMultiplicity[uCh] ] = 
                                            calibPtr->GetTime();
               } // if( fvVftxCalibBoards[uBoard].iMultiplicity[uCh] < TVftxBoardData::MaxMult )
               break;
            case 1: // 1 = Falling
               if( fvVftxCalibBoards[uBoard].iMultiplicity[uCh] < TVftxBoardData::MaxMult )
               {
                  fvVftxCalibBoards[uBoard].iMultiplicity[uCh] ++;
                  fvVftxCalibBoards[uBoard].dTimeCorr[uCh]
                                            [ fvVftxCalibBoards[uBoard].iMultiplicity[uCh] ] = 
                                            calibPtr->GetTime();
               } // if( fvVftxCalibBoards[uBoard].iMultiplicity[uCh] < TVftxBoardData::MaxMult )
               break;
            case 2: // 2 = Full
               if( fvVftxCalibBoards[uBoard].iMultiplicity[2*uCh] < TVftxBoardData::MaxMult && 
                   fvVftxCalibBoards[uBoard].iMultiplicity[2*uCh + 1] < TVftxBoardData::MaxMult )
               {
                  fvVftxCalibBoards[uBoard].iMultiplicity[2*uCh]     ++;
                  fvVftxCalibBoards[uBoard].iMultiplicity[2*uCh + 1] ++;
                  fvVftxCalibBoards[uBoard].dTimeCorr[2*uCh]
                                            [ fvVftxCalibBoards[uBoard].iMultiplicity[2*uCh] ] = 
                                            calibPtr->GetTime() + calibPtr->GetTot();
                  fvVftxCalibBoards[uBoard].dTimeCorr[2*uCh + 1]
                                            [ fvVftxCalibBoards[uBoard].iMultiplicity[2*uCh + 1] ] = 
                                            calibPtr->GetTime();
               } // if( both edges multiplicity < TVftxBoardData::MaxMult )
               break;
            default:
               break;
         } // switch( calibPtr->GetEdge() )
      } // if( toftdc::vftx == calibPtr->GetType() )
   } // for( Int_t iCalibDatInd = 0; iCalibDatInd < fCalibDataCollection->GetEntriesFast(); iCalibDatInd++ )
   return kTRUE;
}
Bool_t TMbsConvTof::ClearVftxBoards()
{
   for( Int_t iVftxIndex = 0; iVftxIndex < fMbsConvPar->GetNbOutVftx(); iVftxIndex ++)
      if( static_cast<UInt_t>(fMbsConvPar->GetVftxInd( iVftxIndex )) < fMbsUnpackPar->GetNbActiveBoards( toftdc::vftx ) )
      {
         fvVftxCalibBoards[ fMbsConvPar->GetVftxInd( iVftxIndex ) ].Clear();
      } // if( fMbsConvPar->GetVftxInd( iVftxIndex ) < fMbsUnpackPar->GetNbActiveBoards( toftdc::vftx ) )
   
   return kFALSE;
}
// ------------------------------------------------------------------ 
// Fired Channel building & Conversion
Bool_t TMbsConvTof::FillCalibScaler()
{
   UInt_t uNbScalBd = fMbsUnpackPar->GetNbActiveScalersB();
   
   for( Int_t iScalBdIndx = 0; iScalBdIndx < fMbsConvPar->GetNbOutScal(); iScalBdIndx++ )
      if( -1 < fMbsConvPar->GetScalerInd( iScalBdIndx ) &&
          static_cast<UInt_t>(fMbsConvPar->GetScalerInd( iScalBdIndx ))< uNbScalBd )
      {
         if( NULL == fCalibScalCollection )
            return kFALSE;
            
         TTofCalibScaler * xCalibScaler  = (TTofCalibScaler*)  fCalibScalCollection->At(fMbsConvPar->GetScalerInd( iScalBdIndx ) );
            
         if( NULL == xCalibScaler )
            return kFALSE;
            
         switch( xCalibScaler->GetScalerType() )
         {
            case tofscaler::triglog:
            {
               // Filling event times from internal reference clock
               fvScalerEvents[iScalBdIndx].fDTimeSinceFirstEventSecondsTriglog = xCalibScaler->GetTimeToFirst();
               fvScalerEvents[iScalBdIndx].fDTimeSinceLastEventSecondsTriglog  = xCalibScaler->GetTimeToLast(); 
               
               // Filling scaler rates since last event
               for( UInt_t uScalIndx = 0; uScalIndx < xCalibScaler->GetScalerNumber(); uScalIndx++)
                  for( UInt_t uCh = 0; uCh < xCalibScaler->GetChannelNumber(); uCh++)
                     fvScalerEvents[iScalBdIndx].fDTriglogRate[uScalIndx][uCh] = xCalibScaler->GetScalerValue( uCh, uScalIndx);
               break;
            }
            case tofscaler::scalormu:
            {  
               // Filling event times from internal reference clock
               fvScalerEvents[iScalBdIndx].fDTimeSinceFirstEventSecondsScalOrMu = xCalibScaler->GetTimeToFirst(); 
               fvScalerEvents[iScalBdIndx].fDTimeSinceLastEventSecondsScalOrMu  = xCalibScaler->GetTimeToLast(); 
               
               // Filling scaler rates since last event
               for( UInt_t uCh = 0; uCh < xCalibScaler->GetChannelNumber(); uCh++)
                  fvScalerEvents[iScalBdIndx].fDScalOrMuRate[uCh] = xCalibScaler->GetScalerValue( uCh );
               break;
            }
            case tofscaler::scalormubig:
            {
               // Filling event times from internal reference clock
               fvScalerEvents[iScalBdIndx].fDTimeSinceFirstEventSecondsScalOrMu = xCalibScaler->GetTimeToFirst(); 
               fvScalerEvents[iScalBdIndx].fDTimeSinceLastEventSecondsScalOrMu  = xCalibScaler->GetTimeToLast(); 
               
               // Filling scaler rates since last event
               for( UInt_t uCh = 0; uCh < xCalibScaler->GetChannelNumber(); uCh++)
                  fvScalerEvents[iScalBdIndx].fDScalOrMuRate[uCh] = xCalibScaler->GetScalerValue( uCh );
               break;
            }
            case tofscaler::undef:
            default:
               break;
         } // switch( xCalibScaler->GetScalerType() )
      } // if( fMbsConvPar->GetScalerInd( iScalBdIndx ) < uNbScalBd )
   
   return kTRUE;
}
Bool_t TMbsConvTof::FillMappedScaler()
{
   // TODO: if mapped scaler rate to detector rate available
   // fvScalerEvents[uScalBdIndx].fDDetectorRate
   return kTRUE;
}
Bool_t TMbsConvTof::ClearScalerEvent()
{
   // Clear all Scalers_Event objects
   for( UInt_t uScalerEvt = 0; uScalerEvt < fvScalerEvents.size(); uScalerEvt++)
      fvScalerEvents[uScalerEvt].Clear();
      
   return kTRUE;
}
// ------------------------------------------------------------------ 
// Fired Channel building & Conversion
Bool_t TMbsConvTof::InitTempDigiStorage()
{
   if( 0 < fMbsConvPar->GetNbDetectors() )
   {
      fvbMappedDet.resize( fMbsConvPar->GetNbDetectors() );
      fviNbChannels.resize( fMbsConvPar->GetNbDetectors() );
      if( kTRUE == fMbsMappingPar->UseDigiExp() )
      {
         fvClassedDigis = new std::vector< CbmTofDigi > *[ fMbsConvPar->GetNbDetectors() ];
         for( Int_t iDetInd = 0; iDetInd < fMbsConvPar->GetNbDetectors(); iDetInd ++ )
         {
            Int_t iUniqueId = fMbsConvPar->GetDetectorUid( iDetInd );
            if( -1 < fMbsMappingPar->GetMappedDetInd( iUniqueId ) )
            {
               fvbMappedDet[iDetInd] = kTRUE;
               Int_t iSmType   = CbmTofAddress::GetSmType( iUniqueId );
               fviNbChannels[iDetInd] = fMbsMappingPar->GetSmTypeNbCh(iSmType);
               fvClassedDigis[iDetInd] = new std::vector< CbmTofDigi >[
                        fMbsConvPar->GetDetChTyp(iDetInd) * fviNbChannels[iDetInd] ];
            } // if( -1 < fMbsMappingPar->GetMappedDetInd( iUniqueId ) )
               else fvbMappedDet[iDetInd] = kFALSE;
         } // for( Int_t iDetInd = 0; iDetInd < fMbsMappingPar->GetNbMappedDet(); iDetInd ++ )
      } // if( kTRUE == fMbsMappingPar->UseDigiExp() )
         
      return kTRUE;
   } // if( 0 < fMbsMappingPar->GetNbMappedDet() )
      else
      { 
         LOG(error)<<"TMbsConvTof::InitTempDigiStorage => 0 detectors defined in fMbsMappingPar !!!";
         return kFALSE;
      } // else of if( 0 < fMbsMappingPar->GetNbMappedDet() )
}
Bool_t TMbsConvTof::FillTempDigiStorage()
{
   // Saves digis in separate vectors for each Detector/channel pair => allow time oredring for left-right building
   if( kTRUE == fMbsMappingPar->UseDigiExp() )
   {
      for( Int_t iDigiInd = 0; iDigiInd < fCbmTofDigiCollection->GetEntriesFast(); iDigiInd++ )
      {
         CbmTofDigi * digiPtr = ( CbmTofDigi * )fCbmTofDigiCollection->At( iDigiInd );
         Int_t iAddress = digiPtr->GetAddress();
         Int_t iDetInd  = fMbsConvPar->GetDetectorInd( iAddress );
         if( -1 == iDetInd )
            continue;
         if( kTRUE == fvbMappedDet[iDetInd] )
         {
            Int_t iChan    = digiPtr->GetChannel();
            Int_t iSide    = digiPtr->GetSide();
            switch( fMbsConvPar->GetDetChTyp(iDetInd) )
            {
               case 1:
                  // For pads, number left channels as even and right channels as odd
                  fvClassedDigis[iDetInd][ 2*iChan + iSide ].push_back( *digiPtr );
                  break;
               case 2:
                  // First left end of all channels, then right end
                  fvClassedDigis[iDetInd][ iChan + iSide * fviNbChannels[iDetInd] ].push_back( *digiPtr );
                  break;
               default:
                  break;
            } // switch( fMbsConvPar->GetDetChTyp(iDetInd) )
         } // if( kTRUE == fvbMappedDet[iDetInd] )
      } // for( Int_t iDigiInd = 0; iDigiInd < fCbmTofDigiCollection->GetEntriesFast(); iDigiInd++ )
   } // if( kTRUE == fMbsMappingPar->UseDigiExp() )
   return kTRUE;
}
Bool_t TMbsConvTof::BuildFiredChannels()
{
   if( kTRUE == fMbsMappingPar->UseDigiExp() )
   {
      for( Int_t iDetInd = 0; iDetInd < fMbsConvPar->GetNbDetectors(); iDetInd ++ )
         if( kTRUE == fvbMappedDet[iDetInd] )
            switch( fMbsConvPar->GetDetChTyp(iDetInd) )
            {
               case 1:
               {
                  for( Int_t iChanInd = 0; iChanInd < fviNbChannels[iDetInd]; iChanInd ++)
                     if( 0 < fvClassedDigis[iDetInd][iChanInd].size() )
                  {
                     if( iDetInd < fMbsConvPar->GetNbRpc() )
                     {
                        Rpc_Hit hit;
                        hit.iStrip    = iChanInd;
                        hit.dTimeLeft = fvClassedDigis[iDetInd][iChanInd][0].GetTime();  // in ps
                        hit.dTotLeft  = fvClassedDigis[iDetInd][iChanInd][0].GetTot();   // in ps
                        hit.fbMultiEdge = 1 < fvClassedDigis[iDetInd][iChanInd].size() ? kTRUE : kFALSE;
                        fvRpcHits[iDetInd].push_back( hit );
                     } // if( iDetInd < fMbsConvPar->GetNbRpc() )
                     else for( UInt_t uHit = 0; 
                                uHit < fvClassedDigis[iDetInd][iChanInd].size();
                                uHit++)
                     {
                        Plastics_Hit hit;
                        hit.dTimeLeft = fvClassedDigis[iDetInd][iChanInd][uHit].GetTime();  // in ps
                        hit.dTotLeft  = fvClassedDigis[iDetInd][iChanInd][uHit].GetTot();   // in ps
                        hit.fbMultiEdge = 1 < fvClassedDigis[iDetInd][iChanInd].size() ? kTRUE : kFALSE;
                        fvPlasticHits[iDetInd - fMbsConvPar->GetNbRpc() ].push_back( hit );
                     } // else of if( iDetInd < fMbsConvPar->GetNbRpc() )
                  } // for( Int_t iChanInd = 0; iChanInd < fviNbChannels[iDetInd]; iChanInd ++)
                  break;
               }
               case 2:
               {
                  for( Int_t iChanInd = 0; iChanInd < fviNbChannels[iDetInd]; iChanInd ++)
                     if( 0 < fvClassedDigis[iDetInd][iChanInd].size() &&
                         0 < fvClassedDigis[iDetInd][iChanInd + fviNbChannels[iDetInd] ].size() )
                  {
                     if( iDetInd < fMbsConvPar->GetNbRpc() )
                     {
                        Rpc_Hit hit;
                        hit.iStrip      = iChanInd;
                        hit.dTimeLeft   = fvClassedDigis[iDetInd][iChanInd][0].GetTime();  // in ps
                        hit.dTotLeft    = fvClassedDigis[iDetInd][iChanInd][0].GetTot();   // in ps
                        hit.dTimeRight  = fvClassedDigis[iDetInd]
                                                           [iChanInd+  fviNbChannels[iDetInd] ]
                                                           [0].GetTime();  // in ps
                        hit.dTotRight   = fvClassedDigis[iDetInd]
                                                           [iChanInd + fviNbChannels[iDetInd] ]
                                                           [0].GetTot();   // in ps
                        hit.fbMultiEdge = ( 1 < fvClassedDigis[iDetInd][iChanInd].size() ||
                                            1 < fvClassedDigis[iDetInd]
                                                                 [iChanInd + fviNbChannels[iDetInd] ].size() ) ? 
                                          kTRUE : kFALSE;
                        fvRpcHits[iDetInd].push_back( hit );
                     } // if( iDetInd < fMbsConvPar->GetNbRpc() )
                     else for( UInt_t uHit = 0; 
                                uHit < fvClassedDigis[iDetInd][iChanInd].size() &&
                                uHit < fvClassedDigis[iDetInd][iChanInd + fviNbChannels[iDetInd] ].size();
                                uHit++)
                     {
                        Plastics_Hit hit;
                        hit.dTimeLeft   = fvClassedDigis[iDetInd][iChanInd][uHit].GetTime();  // in ps
                        hit.dTotLeft    = fvClassedDigis[iDetInd][iChanInd][uHit].GetTot();   // in ps
                        hit.dTimeRight  = fvClassedDigis[iDetInd]
                                                           [iChanInd+  fviNbChannels[iDetInd] ]
                                                           [uHit].GetTime();  // in ps
                        hit.dTotRight   = fvClassedDigis[iDetInd]
                                                           [iChanInd + fviNbChannels[iDetInd] ]
                                                           [uHit].GetTot();   // in ps
                        hit.fbMultiEdge = ( 1 < fvClassedDigis[iDetInd][iChanInd].size() ||
                                            1 < fvClassedDigis[iDetInd]
                                                                 [iChanInd + fviNbChannels[iDetInd] ].size() ) ? 
                                          kTRUE : kFALSE;
                        fvPlasticHits[iDetInd - fMbsConvPar->GetNbRpc() ].push_back( hit );
                     } // else of if( iDetInd < fMbsConvPar->GetNbRpc() )
                  } // for( Int_t iChanInd = 0; iChanInd < fviNbChannels[iDetInd]; iChanInd ++)
               } 
               break;
               default:
                  break;
            } // switch( fMbsConvPar->GetDetChTyp(iDetInd) )
   } // if( kTRUE == fMbsMappingPar->UseDigiExp() )
   return kTRUE;
}
Bool_t TMbsConvTof::ClearTempDigiStorage()
{
   if( 0 < fMbsMappingPar->GetNbMappedDet() )
   {
      if( kTRUE == fMbsMappingPar->UseDigiExp() )
      {
         for( Int_t iDetInd = 0; iDetInd < fMbsConvPar->GetNbDetectors(); iDetInd ++ )
            if( kTRUE == fvbMappedDet[iDetInd] )
               for( Int_t iChanInd = 0; 
                     iChanInd < fMbsConvPar->GetDetChTyp(iDetInd) * fviNbChannels[iDetInd]; 
                     iChanInd ++)
                  fvClassedDigis[iDetInd][iChanInd].clear();
      } // if( kTRUE == fMbsMappingPar->UseDigiExp() )
   } // if( 0 < fMbsMappingPar->GetNbMappedDet() )
         
   return kTRUE;
}
Bool_t TMbsConvTof::DeleteTempDigiStorage()
{
   if( 0 < fMbsMappingPar->GetNbMappedDet() )
   {
      if( kTRUE == fMbsMappingPar->UseDigiExp() )
      {
         for( Int_t iDetInd = 0; iDetInd < fMbsConvPar->GetNbDetectors(); iDetInd ++ )
            if( kTRUE == fvbMappedDet[iDetInd] )
            {
               for( Int_t iChanInd = 0; 
                     iChanInd < fMbsConvPar->GetDetChTyp(iDetInd) * fviNbChannels[iDetInd]; 
                     iChanInd ++)
                  fvClassedDigis[iDetInd][iChanInd].clear();
               delete [] fvClassedDigis[iDetInd];
            } // for( Int_t iDetInd = 0; iDetInd < fMbsMappingPar->GetNbMappedDet(); iDetInd ++ )
         delete fvClassedDigis;
      } // if( kTRUE == fMbsMappingPar->UseDigiExp() )
      fviNbChannels.clear();
   } // if( 0 < fMbsMappingPar->GetNbMappedDet() )
      
   return kTRUE;
}

