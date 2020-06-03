// ------------------------------------------------------------------
// -----                    TMbsMappingTof                       -----
// -----              Created 09/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TMbsMappingTof.h"

// General Unpack headers
#include "TMbsUnpackTofPar.h"

// ToF specific headers
#include "TMbsMappingTofPar.h"
#include "TMbsCalibTofPar.h"
#include "TofTdcDef.h"
#include "TofCaenDef.h"
#include "TofVftxDef.h"
#include "TofTrbTdcDef.h"
#include "TofGet4Def.h"
#include "TTofTdcBoard.h"
#include "TTofTdcData.h"
#include "TTofCalibData.h"
#include "CbmTofAddress.h"
#include "CbmTofDigi.h"
#include "TTofTriglogBoard.h"

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
#include <TFile.h>

// C++ headers
#include <algorithm>
      
TMbsMappingTof::TMbsMappingTof() : 
   FairTask("MbsCalibTof"),
   fMbsUnpackPar(NULL),
   fMbsCalibPar(NULL),
   fMbsMappingPar(NULL),
   fviNbHitInThisEvent(),
   fhDetChEvtMul(),
   fhDetChTotMul(),
   fhDetChHitsPerEvt(),
   fhDetChCoincSides(),
   fhDetChCoincLeft(),
   fhDetChCoincRight(),
   fCalibDataCollection(NULL),
   fTriglogBoardCollection(NULL),
   fbSaveMappedDigis(kFALSE),
   fbFillHistos(kFALSE),
   fbDuplicatePadDigis(kTRUE),
   fCbmTofDigiCollection(NULL)
{
}

TMbsMappingTof::TMbsMappingTof(const char* name, Int_t /*mode*/, Int_t verbose) :
   FairTask(name, verbose),
   fMbsUnpackPar(0),
   fMbsCalibPar(NULL),
   fMbsMappingPar(NULL),
   fviNbHitInThisEvent(),
   fhDetChEvtMul(),
   fhDetChTotMul(),
   fhDetChHitsPerEvt(),
   fhDetChCoincSides(),
   fhDetChCoincLeft(),
   fhDetChCoincRight(),
   fCalibDataCollection(NULL),
   fTriglogBoardCollection(NULL),
   fbSaveMappedDigis(kFALSE),
   fbFillHistos(kFALSE),
   fbDuplicatePadDigis(kTRUE),
   fCbmTofDigiCollection(NULL)
{
}
      
TMbsMappingTof::~TMbsMappingTof()
{
   DeleteHistograms();
   LOG(info)<<"**** TMbsMappingTof: Delete instance ";
}

// --------------------------------------------------
// Fairtask specific functions
void TMbsMappingTof::SetParContainers()
{
   InitParameters();
}
InitStatus TMbsMappingTof::ReInit()
{
   LOG(info)<<"**** TMbsMappingTof: Reinitialize the mapping parameters for tof ";
   if( kFALSE == InitParameters() )
      return kFATAL;
   fMbsMappingPar->printParams();
   
   return kSUCCESS;
}

InitStatus TMbsMappingTof::Init()
{
   if( kFALSE == InitParameters() )
      return kFATAL;
   fMbsMappingPar->printParams();
   if( kFALSE == CreateHistogramms() )
      return kFATAL;   
      
   if( kFALSE == RegisterInput() )
      return kFATAL;   
      
   // Create the proper output objects for each detector
   if( kFALSE == RegisterOutput() )
      return kFATAL;
   
   return kSUCCESS;
}
void TMbsMappingTof::Exec(Option_t* /*option*/)
{
   ClearOutput();
   
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

   MapTdcDataToDet();

   if( fbFillHistos )
   {
     FillHistograms();
   }
}
void TMbsMappingTof::Finish()
{
   WriteHistogramms();
}

// ------------------------------------------------------------------
Bool_t TMbsMappingTof::InitParameters()
{
   // Get Base Container
   FairRun* ana = FairRun::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   // Unpack parameter
   fMbsUnpackPar = (TMbsUnpackTofPar*) (rtdb->getContainer("TMbsUnpackTofPar"));
   if( 0 == fMbsUnpackPar )
   {
      LOG(error)<<"TMbsMappingTof::InitParameters => Could not obtain the TMbsUnpackTofPar ";
      return kFALSE;
   }
      
   // Calibration parameter
   fMbsCalibPar = (TMbsCalibTofPar*) (rtdb->getContainer("TMbsCalibTofPar"));
   if( 0 == fMbsCalibPar )
   {
      LOG(error)<<"TMbsMappingTof::InitParameters => Could not obtain the TMbsCalibTofPar ";
      return kFALSE; 
   }
      
   // Calibration parameter
   fMbsMappingPar = (TMbsMappingTofPar*) (rtdb->getContainer("TMbsMappingTofPar"));
   if( 0 == fMbsMappingPar )
   {
      LOG(error)<<"TMbsMappingTof::InitParameters => Could not obtain the TMbsMappingTofPar ";
      return kFALSE; 
   }

   // PAL: Added to recover loading of parameters before initialization
//   rtdb->initContainers(  ana->GetRunId() );
   
   return kTRUE;
}
// ------------------------------------------------------------------
Bool_t TMbsMappingTof::CreateHistogramms()
{
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !
   
   if( 0 < fMbsMappingPar->GetNbMappedDet() )
   {
      fhDetChEvtMul.resize( fMbsMappingPar->GetNbMappedDet() );
      fhDetChTotMul.resize( fMbsMappingPar->GetNbMappedDet() );
      fhDetChHitsPerEvt.resize( 2*fMbsMappingPar->GetNbMappedDet() );
      fhDetChCoincSides.resize( fMbsMappingPar->GetNbMappedDet() );
      fhDetChCoincLeft.resize( fMbsMappingPar->GetNbMappedDet() );
      fhDetChCoincRight.resize( fMbsMappingPar->GetNbMappedDet() );
      
      fviNbHitInThisEvent.resize( fMbsMappingPar->GetNbMappedDet() );
      for( Int_t iDetIndx = 0; iDetIndx < fMbsMappingPar->GetNbMappedDet(); iDetIndx++ )
      {
         Int_t iUniqueId = fMbsMappingPar->GetMappedDetUId( iDetIndx );
         Int_t iSmType   = CbmTofAddress::GetSmType( iUniqueId );
         Int_t iSmId     = CbmTofAddress::GetSmId( iUniqueId );
         Int_t iRpcId    = CbmTofAddress::GetRpcId( iUniqueId );
         fhDetChEvtMul.at(iDetIndx) =  new TH2I( 
                                    Form("tof_ty%01d_sm%03d_rpc%03d_ch_evt_mul", iSmType, iSmId, iRpcId ),
                                    Form("Number of events with hits for each channel of Rpc #%03d in Sm %03d of type %d; Channel []; Side []; Events with hits[]", 
                                          iRpcId, iSmId, iSmType ),
                                    fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType),
                                    2, 0, 2 ); 
         fhDetChTotMul.at(iDetIndx) =  new TH2I( 
                                    Form("tof_ty%01d_sm%03d_rpc%03d_ch_tot_mul", iSmType, iSmId, iRpcId ),
                                    Form("Number of hits for each channel of Rpc #%03d in Sm %03d of type %d; Channel []; Side []; Hits []", 
                                          iRpcId, iSmId, iSmType ),
                                    fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType),
                                    2, 0, 2 ); 
         fhDetChHitsPerEvt.at(2*iDetIndx) =  new TH2I( 
                                          Form("tof_ty%01d_sm%03d_rpc%03d_ch_left_hitsperevt", iSmType, iSmId, iRpcId ),
                                          Form("Number of hits per event for the left side each channel of Rpc #%03d in Sm %03d of type %d; Channel []; Nb hits per event []; Events []", 
                                                iRpcId, iSmId, iSmType ),
                                          fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType),
                                          10, 0, 10 ); 
         fhDetChHitsPerEvt.at(2*iDetIndx + 1) =  new TH2I( 
                                          Form("tof_ty%01d_sm%03d_rpc%03d_ch_right_hitsperevt", iSmType, iSmId, iRpcId ),
                                          Form("Number of hits per event for the right side each channel of Rpc #%03d in Sm %03d of type %d; Channel []; Nb hits per event []; Events []", 
                                                iRpcId, iSmId, iSmType ),
                                          fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType),
                                          10, 0, 10 ); 

         fhDetChCoincSides.at(iDetIndx) =  new TH2I(
                                    Form("tof_ty%01d_sm%03d_rpc%03d_coinc_sides", iSmType, iSmId, iRpcId ),
                                    Form("Number of events with hits for both channels of Rpc #%03d in Sm %03d of type %d; Channel left []; Channel right []; Events with hits in both[]",
                                          iRpcId, iSmId, iSmType ),
                                    fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType),
                                    fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType));
         fhDetChCoincLeft.at(iDetIndx) =  new TH2I(
                                    Form("tof_ty%01d_sm%03d_rpc%03d_coinc_left", iSmType, iSmId, iRpcId ),
                                    Form("Number of events with hits for both channels of Rpc #%03d in Sm %03d of type %d; Channel left 1 []; Channel left 2 []; Events with hits in both[]",
                                          iRpcId, iSmId, iSmType ),
                                    fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType),
                                    fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType));
         fhDetChCoincRight.at(iDetIndx) =  new TH2I(
                                    Form("tof_ty%01d_sm%03d_rpc%03d_coinc_right", iSmType, iSmId, iRpcId ),
                                    Form("Number of events with hits for both channels of Rpc #%03d in Sm %03d of type %d; Channel right 1 []; Channel right 2 []; Events with hits in both[]",
                                          iRpcId, iSmId, iSmType ),
                                    fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType),
                                    fMbsMappingPar->GetSmTypeNbCh(iSmType), 0, fMbsMappingPar->GetSmTypeNbCh(iSmType));
                                    
         (fviNbHitInThisEvent.at(iDetIndx)).resize( 2*fMbsMappingPar->GetSmTypeNbCh(iSmType), 0 );
      } // for( Int_t iDetIndx = 0; iDetIndx < fMbsMappingPar->GetNbMappedDet(); iDetIndx++ )
   } // if( 0 < fMbsMappingPar->GetNbMappedDet() )
   
   if( kTRUE == fMbsMappingPar->IsDebug() )
   {
      for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      {
         if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
         {
            UInt_t   uNbChan     = 0;

            switch( uType )
            {
               case toftdc::caenV1290:
                  uNbChan     = caentdc::kuNbChan;
                  break;
               case toftdc::vftx:
                  uNbChan     = vftxtdc::kuNbChan / 2;
                  break;
               case toftdc::trb:
                  uNbChan     = trbtdc::kuNbChan / 2;
//                  uNbChan    /= 2; // FIXME: used to reduce channel number (half empty) for GSI Sep 14!!!
                  break;
               case toftdc::get4:
                  uNbChan     = get4tdc::kuNbChan;
                  break;
               default:
                  break;
            } // switch( uType )

            // Calibration variable initialization
            Int_t iHistoIndex = 0;
            for( UInt_t uTdc1 = 0; uTdc1 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc1 ++)
               for( UInt_t uTdc2 = uTdc1; uTdc2 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc2 ++)
                  iHistoIndex ++;
            fhDebTdcChEvtCoinc[uType].resize( iHistoIndex, NULL );
            iHistoIndex = 0;
//            fhDebTdcChEvtCoinc[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) -1 );
            fbDebTdcChEvtThere[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) );
            for( UInt_t uTdc1 = 0; uTdc1 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc1 ++)
            {
//               fhDebTdcChEvtCoinc[uType][uTdc1].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) - uTdc1 -1, NULL );
               ((fbDebTdcChEvtThere[uType]).at(uTdc1)).resize( uNbChan, kFALSE );
               for( UInt_t uTdc2 = uTdc1; uTdc2 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc2 ++)
               {
//                  LOG(info)<<"TMbsMappingTof::CreateHistogramms => "<< uType << " " << uTdc1 << " " << uTdc2;
                  (fhDebTdcChEvtCoinc[uType]).at(iHistoIndex) =  new TH2I(
                        Form("tof_map_deb_ty%01u_tdc%03u_tdc%03u", uType, uTdc1, uTdc2 ),
                        Form("Number of events with hits for in both channels for TDC #%03u and %03u of type %d; Channel TDC #%03u []; Channel TDC #%03u []; Events with hits in both[]",
                              uTdc1, uTdc2, uType, uTdc1, uTdc2 ),
                        uNbChan, 0, uNbChan,
                        uNbChan, 0, uNbChan );
//                  LOG(info)<<" => OK ";
                  iHistoIndex++;
               } // for( UInt_t uTdc2 = uTdc1 + 1; uTdc2 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc2 ++)
            } // for( UInt_t uTdc1 = 0; uTdc1 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc1 ++)
         } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   } // if( kTRUE == fMbsMappingPar->IsDebug() )

   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
   
   return kTRUE;
}
Bool_t TMbsMappingTof::FillHistograms()
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
   
   if( 0 < fMbsMappingPar->GetNbMappedDet() )
   {
      for( Int_t iDetIndx = 0; iDetIndx < fMbsMappingPar->GetNbMappedDet(); iDetIndx++ )
         std::fill( (fviNbHitInThisEvent.at(iDetIndx)).begin(), (fviNbHitInThisEvent.at(iDetIndx)).end(), 0 );
      
      Int_t iAddress   = 0xFFFFFFFF;
      Int_t iMappedDet = -1;
      Int_t iSmType    = -1;
      Int_t iChan      = -1;
      Int_t iSide      = -1;
      // loop over Mapped Data
      for( Int_t iDigiIndex = 0; iDigiIndex < fCbmTofDigiCollection->GetEntriesFast() ; iDigiIndex++ )
      {
         if( kTRUE == fMbsMappingPar->UseDigiExp() )
         {
            CbmTofDigi * xDigiPtr = (CbmTofDigi *)fCbmTofDigiCollection->At( iDigiIndex );
            
            iAddress = xDigiPtr->GetAddress();
            iMappedDet = fMbsMappingPar->GetMappedDetInd( iAddress );
            if( -1 < iMappedDet )
            {
               iSmType = xDigiPtr->GetType();
               iChan = xDigiPtr->GetChannel();
               iSide = xDigiPtr->GetSide();
               (fviNbHitInThisEvent.at(iMappedDet)).at( iSide*fMbsMappingPar->GetSmTypeNbCh(iSmType) + iChan) ++;
               
               (fhDetChTotMul.at(iMappedDet))->Fill( iChan, iSide);
            } // if( -1 < iMappedDet )
         } // if( kTRUE == fMbsMappingPar->UseDigiExp() )
      } // for( Int_t iDigiIndex = 0; iDigiIndex < fCbmTofDigiCollection->GetEntriesFast() ; iDigiIndex++ )
      for( Int_t iDetIndx = 0; iDetIndx < fMbsMappingPar->GetNbMappedDet(); iDetIndx++ )
      {
         Int_t iUniqueId = fMbsMappingPar->GetMappedDetUId( iDetIndx );
         iSmType   = CbmTofAddress::GetSmType( iUniqueId );
         for( Int_t iChIndx = 0; iChIndx < fMbsMappingPar->GetSmTypeNbCh(iSmType); iChIndx++)
         {
            // Hits per events
            (fhDetChHitsPerEvt.at(2*iDetIndx))->Fill( iChIndx, 
                                          (fviNbHitInThisEvent.at(iDetIndx)).at( iChIndx ));
            (fhDetChHitsPerEvt.at(2*iDetIndx+1))->Fill( iChIndx, 
                                          (fviNbHitInThisEvent.at(iDetIndx)).at( fMbsMappingPar->GetSmTypeNbCh(iSmType) +iChIndx ));
                                          
            // Events with at least one hit
            if( 0 < (fviNbHitInThisEvent.at(iDetIndx)).at( iChIndx ) )
               (fhDetChEvtMul.at(iDetIndx))->Fill( iChIndx, 0);
            if( 0 < (fviNbHitInThisEvent.at(iDetIndx)).at( fMbsMappingPar->GetSmTypeNbCh(iSmType) + iChIndx ) )
               (fhDetChEvtMul.at(iDetIndx))->Fill( iChIndx, 1);

            // Events with at least one hit in both channels = coincidences
            for( Int_t iChIndx2 = 0 ; iChIndx2 < fMbsMappingPar->GetSmTypeNbCh(iSmType); iChIndx2++)
            {
            	if( 0 < (fviNbHitInThisEvent.at(iDetIndx)).at( iChIndx ) &&
            	    0 < (fviNbHitInThisEvent.at(iDetIndx)).at( fMbsMappingPar->GetSmTypeNbCh(iSmType) + iChIndx2 ))
            		(fhDetChCoincSides.at(iDetIndx))->Fill( iChIndx, iChIndx2 );
            	if( iChIndx < iChIndx2 &&
            		0 < (fviNbHitInThisEvent.at(iDetIndx)).at( iChIndx ) &&
            	    0 < (fviNbHitInThisEvent.at(iDetIndx)).at( iChIndx2 ))
            		(fhDetChCoincLeft.at(iDetIndx))->Fill( iChIndx,  iChIndx2 );

            	if( iChIndx < iChIndx2 &&
            		0 < (fviNbHitInThisEvent.at(iDetIndx)).at( fMbsMappingPar->GetSmTypeNbCh(iSmType) + iChIndx ) &&
            	    0 < (fviNbHitInThisEvent.at(iDetIndx)).at( fMbsMappingPar->GetSmTypeNbCh(iSmType) + iChIndx2 ))
            		(fhDetChCoincRight.at(iDetIndx))->Fill( iChIndx,  iChIndx2 );
            } // for( Int_t iChIndx2 = iChIndx ; iChIndx2 < fMbsMappingPar->GetSmTypeNbCh(iSmType); iChIndx2++)
         } // for all Det/Ch pairs 
      } // for( Int_t iDetIndx = 0; iDetIndx < fMbsMappingPar->GetNbMappedDet(); iDetIndx++ )
   } // if( 0 < fMbsMappingPar->GetNbMappedDet() )

   if( kTRUE == fMbsMappingPar->IsDebug() )
   {
      TTofCalibData * fCalibData;
      // loop over Calibrated Data
      for( Int_t iDataIndex = 0; iDataIndex < fCalibDataCollection->GetEntriesFast() ; iDataIndex++ )
      {
         fCalibData = (TTofCalibData *)fCalibDataCollection->At( iDataIndex );

         // Monitoring histograms
         UInt_t   uType = fCalibData->GetType();
         UInt_t   uTdc  = fCalibData->GetBoard();
         UInt_t   uChan = fCalibData->GetChannel();
         Double_t dTime = fCalibData->GetTime();
         Double_t dTot  = fCalibData->GetTot();

         if( 0 < dTime && 0 < dTot )
            ((fbDebTdcChEvtThere[uType]).at(uTdc)).at(uChan) = kTRUE;
      } // for( Int_t iDataIndex = 0; iDataIndex < fCalibDataCollection->GetEntriesFast() ; iDataIndex++ )

      for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      {
         if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
         {
            UInt_t   uNbChan     = 0;

            switch( uType )
            {
               case toftdc::caenV1290:
                  uNbChan     = caentdc::kuNbChan;
                  break;
               case toftdc::vftx:
                  uNbChan     = vftxtdc::kuNbChan / 2;
                  break;
               case toftdc::trb:
                  uNbChan     = trbtdc::kuNbChan / 2;
//                  uNbChan    /= 2; // FIXME: used to reduce channel number (half empty) for GSI Sep 14!!!
                  break;
               case toftdc::get4:
                  uNbChan     = get4tdc::kuNbChan;
                  break;
               default:
                  break;
            } // switch( uType )

            // Debug Fill and clear
            Int_t iHistoIndex = 0;
            for( UInt_t uTdc1 = 0; uTdc1 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc1 ++)
            {
               for( UInt_t uTdc2 = uTdc1; uTdc2 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc2 ++)
               {
                  for( UInt_t uChIndx1 = 0 ; uChIndx1 < uNbChan; uChIndx1++)
                     for( UInt_t uChIndx2 = 0 ; uChIndx2 < uNbChan; uChIndx2++)
                        if( kTRUE == ((fbDebTdcChEvtThere[uType]).at(uTdc1)).at(uChIndx1) &&
                            kTRUE == ((fbDebTdcChEvtThere[uType]).at(uTdc2)).at(uChIndx2)  )
                           ((fhDebTdcChEvtCoinc[uType]).at(iHistoIndex))->Fill(uChIndx1, uChIndx2);
                  iHistoIndex++;
               } // for( UInt_t uTdc2 = uTdc1 + 1; uTdc2 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc2 ++)

               for( UInt_t uChIndx1 = 0 ; uChIndx1 < uNbChan; uChIndx1++)
                  ((fbDebTdcChEvtThere[uType]).at(uTdc1)).at(uChIndx1) = kFALSE;
            } // for( UInt_t uTdc1 = 0; uTdc1 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc1 ++)
         } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   } // if( kTRUE == fMbsMappingPar->IsDebug() )
   return kTRUE;
}
void TMbsMappingTof::WriteHistogramms()
{
   TDirectory * oldir = gDirectory;
   TFile *fHist = new TFile("./tofMbsMap.hst.root","RECREATE");
   
//   TDirectory* inDir = (TDirectory *)fHist;
   if( 0 < fMbsMappingPar->GetNbMappedDet() )
   {
   
      // create a subdirectory for each histogram type in this file
      TDirectory *cdMapEvt = fHist->mkdir( "Map_Evt" );
      TDirectory *cdMapTot = fHist->mkdir( "Map_Total" );
      TDirectory *cdMapHitsPerEvt = fHist->mkdir( "Map_HitsPerEvt" );
      TDirectory *cdMapCoincSides = fHist->mkdir( "Map_coincSide" );
      TDirectory *cdMapCoincLeft  = fHist->mkdir( "Map_coincLeft" );
      TDirectory *cdMapCoincRight = fHist->mkdir( "Map_coincRight" );
      
      // loop over all mapped Detectors
      for( Int_t iDetIndx = 0; iDetIndx < fMbsMappingPar->GetNbMappedDet(); iDetIndx++ )
      {
         // Nb Events with hits
         cdMapEvt->cd();
         (fhDetChEvtMul.at(iDetIndx))->Write();
         
         // Total Nb hits per channel/side
         cdMapTot->cd();
         (fhDetChTotMul.at(iDetIndx))->Write();
         
         // Hits per events
         cdMapHitsPerEvt->cd();
         (fhDetChHitsPerEvt.at(2*iDetIndx))->Write();
         (fhDetChHitsPerEvt.at(2*iDetIndx+1))->Write();

         cdMapCoincSides->cd();
         (fhDetChCoincSides.at(iDetIndx))->Write();
         cdMapCoincLeft->cd();
         (fhDetChCoincLeft.at(iDetIndx))->Write();
         cdMapCoincRight->cd();
         (fhDetChCoincRight.at(iDetIndx))->Write();
      } // for( Int_t iDetIndx = 0; iDetIndx < fMbsMappingPar->GetNbMappedDet(); iDetIndx++ )
   } // if( 0 < fMbsMappingPar->GetNbMappedDet() )

   if( kTRUE == fMbsMappingPar->IsDebug() )
   {
      TDirectory *cdDebugMapTdc = fHist->mkdir( "Map_DebugTdc" );
      cdDebugMapTdc->cd();
      for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      {
         if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
         {
            TDirectory *cdDebugMapTdcFirst[fMbsUnpackPar->GetNbActiveBoards( uType )]; // TODO: safe?
            // Debug Write
            Int_t iHistoIndex = 0;
            for( UInt_t uTdc1 = 0; uTdc1 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc1 ++)
            {
               cdDebugMapTdcFirst[uTdc1] = cdDebugMapTdc->mkdir( Form("%s%03u_Map",
                      toftdc::ksTdcHistName[ uType ].Data(), uTdc1 ) );
               cdDebugMapTdcFirst[uTdc1]->cd();
               for( UInt_t uTdc2 = uTdc1; uTdc2 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc2 ++)
               {
                  ((fhDebTdcChEvtCoinc[uType]).at(iHistoIndex))->Write();
                  iHistoIndex++;
               } // for( UInt_t uTdc2 = uTdc1 + 1; uTdc2 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc2 ++)
            } // for( UInt_t uTdc1 = 0; uTdc1 < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc1 ++)
         } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   } // if( kTRUE == fMbsMappingPar->IsDebug() )

   gDirectory->cd( oldir->GetPath() );
   
   fHist->Close();
}
void TMbsMappingTof::DeleteHistograms()
{
}
// ------------------------------------------------------------------
Bool_t TMbsMappingTof::RegisterInput()
{
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards() )
   {
      fCalibDataCollection = (TClonesArray*) rootMgr->GetObject("TofCalibData");
      if( NULL == fCalibDataCollection) 
      {
         LOG(error)<<"TMbsMappingTof::RegisterInput => Could not get the TofCalibData TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fCalibDataCollection) 
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards() )

   // Event rejection
   // E.g.: pulser event used to Empty the VFTX buffers
   if( 1 == fMbsUnpackPar->OnlyOneTriglog() &&
       -1 < fMbsUnpackPar->GetTriggerToReject() )
   {
      LOG(info)<<"TMbsMappingTof::RegisterInput => Load the TofTriglog TClonesArray for trigger rejection!!!";
      fTriglogBoardCollection = (TClonesArray*) rootMgr->GetObject("TofTriglog");
      if( NULL == fTriglogBoardCollection)
      {
         LOG(error)<<"TMbsMappingTof::RegisterInput => Could not get the TofTriglog TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fTriglogBoardCollection)
   } // if trigger rejection enabled and trigger board enabled
   
   return kTRUE;
}
Bool_t TMbsMappingTof::RegisterOutput()
{
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   if( kTRUE == fMbsMappingPar->UseDigiExp() )
      fCbmTofDigiCollection = new TClonesArray("CbmTofDigiExp");
      else fCbmTofDigiCollection = new TClonesArray("CbmTofDigi");
//   rootMgr->Register("CbmTofDigi","Tof",fCbmTofDigiCollection, IsOutputBranchPersistent("CbmTofDigi"));
   rootMgr->Register( "CbmTofDigi","Tof",fCbmTofDigiCollection,
                      fMbsUnpackPar->WriteDataInCbmOut() || fbSaveMappedDigis );
   
   return kTRUE;
}
Bool_t TMbsMappingTof::ClearOutput()
{
   // Actually, neither CbmDigi nor CbmTofDigi nor CbmTofDigiExp reimplement
   // TObject::Clear(). This does, however, not matter here because each event's
   // digi objects are reconstructed in the dedicated TClonesArray by a
   // placement new operation instead of calling TClonesArray::ConstructedAt().
   fCbmTofDigiCollection->Clear("C");
   return kTRUE;
}

void TMbsMappingTof::SetSaveDigis( Bool_t bSaveDigis )
{
   fbSaveMappedDigis = bSaveDigis;
   LOG(info)<<"TMbsMappingTof => Enable the saving of mapped digis in analysis output file";
}
// ------------------------------------------------------------------ 
// Mapping
   // -----   Bit masks -----------------------------------------------------------
const Int_t TMbsMappingTof::fgklTypeMask    = ( 1 << TMbsMappingTof::fgkiTypeSize)    - 1;
const Int_t TMbsMappingTof::fgklBoardMask   = ( 1 << TMbsMappingTof::fgkiBoardSize)   - 1;
const Int_t TMbsMappingTof::fgklChannelMask = ( 1 << TMbsMappingTof::fgkiChannelSize) - 1;
const Int_t TMbsMappingTof::fgklEdgeMask    = ( 1 << TMbsMappingTof::fgkEdgeSize)     - 1;
   // -----------------------------------------------------------------------------
   // -----   Bit shifts   --------------------------------------------------------
const Int_t TMbsMappingTof::fgkiTypeOffs    =   0;
const Int_t TMbsMappingTof::fgkiBoardOffs   =   TMbsMappingTof::fgkiTypeOffs    + TMbsMappingTof::fgkiTypeSize;
const Int_t TMbsMappingTof::fgkiChannelOffs =   TMbsMappingTof::fgkiBoardOffs   + TMbsMappingTof::fgkiBoardSize;
const Int_t TMbsMappingTof::fgkiEdgeOffs    =   TMbsMappingTof::fgkiChannelOffs + TMbsMappingTof::fgkiChannelSize;
   // -----------------------------------------------------------------------------
Int_t TMbsMappingTof::GetTdcUniqueId( UInt_t uType, UInt_t uBoard, UInt_t uChannel, UInt_t uEdge )
{
   Int_t iUniqueId = ( ( uType    & fgklTypeMask )    << fgkiTypeOffs )    +
                     ( ( uBoard   & fgklBoardMask )   << fgkiBoardOffs )   +
                     ( ( uChannel & fgklChannelMask ) << fgkiChannelOffs ) +
                     ( ( uEdge    & fgklEdgeMask )    << fgkiEdgeOffs );
   return iUniqueId;
}
Bool_t  TMbsMappingTof::MapTdcDataToDet()
{
   if( NULL == fCalibDataCollection || NULL == fCbmTofDigiCollection )
      return kFALSE;
      
   TTofCalibData * fCalibData;
   LOG(debug)<<"TMbsCalibTof::MapTdcDataToDet => "<<fCalibDataCollection->GetEntriesFast()
             <<" data calibrated to be mapped in this event!";
          
   // loop over Calibrated Data
   for( Int_t iDataIndex = 0; iDataIndex < fCalibDataCollection->GetEntriesFast() ; iDataIndex++ )
   {
      fCalibData = (TTofCalibData *)fCalibDataCollection->At( iDataIndex );
      
      UInt_t   uType = fCalibData->GetType();
      UInt_t   uTdc  = fCalibData->GetBoard();
      UInt_t   uChan = fCalibData->GetChannel();
      Double_t dTime = fCalibData->GetTime();
      Double_t dTot  = fCalibData->GetTot();
      UInt_t   uEdge = fCalibData->GetEdge();
      
      Int_t iTdcUId       = GetTdcUniqueId( uType, uTdc );
      Int_t iMappedTdcInd = fMbsMappingPar->GetMappedTdcInd( iTdcUId );
      
      // First Check if this TDC is in the mapping and if the calibrated hit is complete
      if( -1 < iMappedTdcInd && 2 == uEdge )
      {
         Int_t iChanUId = fMbsMappingPar->GetMapping( iMappedTdcInd, uChan );
         if( 0xFFFFFFFF != static_cast<UInt_t>(iChanUId) )
         {
            // Create Digi and store it 
            if( kTRUE == fMbsMappingPar->UseDigiExp() )
               new((*fCbmTofDigiCollection)[ fCbmTofDigiCollection->GetEntriesFast() ]) 
                  CbmTofDigi( iChanUId, dTime, dTot );


            if(fbDuplicatePadDigis)
            {
               if (  ((iChanUId & 0x0000F00F) == 0x00005006)     // duplicate diamond, pad  entry
                   ||((iChanUId & 0x0000F00F) == 0x00002006)     // duplicate ceramics entry/interference wih Pla!?
                   ||((iChanUId & 0x0000F00F) == 0x00008006)) {  // duplicate Pad entry  // FIXME
                  iChanUId |= 0x00800000;
                  Int_t Nent = fCbmTofDigiCollection->GetEntriesFast();
                  new((*fCbmTofDigiCollection)[ fCbmTofDigiCollection->GetEntriesFast() ]) 
                  CbmTofDigi( iChanUId, dTime, dTot );
                  LOG(debug)<<Form("TMbsMappingTof: Pad entry duplicated 0x%08x at Nent",iChanUId);
                  CbmTofDigi *pDigi2=(CbmTofDigi*) fCbmTofDigiCollection->At(Nent);
                  CbmTofDigi *pDigi1=(CbmTofDigi*) fCbmTofDigiCollection->At(Nent-1);
                  if(pDigi1->GetTime() !=pDigi2->GetTime()){
                     LOG(error) << "TMbsMappingTof: Digi duplication error ";
                     LOG(error) << "   " <<pDigi1->ToString();
                     LOG(error) << "   " <<pDigi2->ToString();
                  } 
               }
            }

         } //  if( 0xFFFFFFFF != fMbsMappingPar->GetMapping( iMappedTdcInd, uChan ) )
	 else {
	   LOG(debug)<<Form("TMbsMappingTof:: <W> Digi in unmapped TDC %d channel %d", iMappedTdcInd, uChan);
	 }
      } // if( -1 < iMappedTdcInd && 2 == uEdge )
         else if( uEdge < 2)
            LOG(warn)<<"TMbsCalibTof::MapTdcDataToDet => Incomplete calibrated data found"
                     <<" => Maybe you are using the wrong Tot building mode? fuEdge = "<<uEdge;
   } // for( Int_t iDataIndex = 0; iDataIndex < fCalibDataCollection->GetEntriesFast() ; iDataIndex++ )
   
   // Time sort digis
   fCbmTofDigiCollection->Sort();
   
   return kTRUE;
}
