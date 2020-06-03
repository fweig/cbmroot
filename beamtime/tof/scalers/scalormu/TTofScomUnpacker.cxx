// ------------------------------------------------------------------
// -----                     TTofScomUnpacker                   -----
// -----              Created 03/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#include "TTofScomUnpacker.h"

// TOF headers
#include "TofDef.h"
#include "TMbsUnpackTofPar.h"
#include "TofScomDef.h"
#include "TofScalerDef.h"
#include "TTofScalerBoard.h"

// FAIR headers
#include "FairRootManager.h"
#include "FairLogger.h"
#include "TString.h"

// ROOT headers
#include "TClonesArray.h"
#include "TH1.h"
#include "TROOT.h"
#include "TDirectory.h"

TTofScomUnpacker::TTofScomUnpacker():
   fParUnpack(0),
   fuNbScom(0),
   fScalerBoardCollection(NULL),
   fhScalers(),
   bFirstEvent(kTRUE),
   fvuFirstScalers()
{
}
TTofScomUnpacker::TTofScomUnpacker( TMbsUnpackTofPar * parIn ):
   fParUnpack( parIn ),
   fuNbScom( parIn->GetNbActiveBoards( tofMbs::scalormu ) ),
   fScalerBoardCollection(NULL),
   fhScalers(),
   bFirstEvent(kTRUE),
   fvuFirstScalers()
{
   // Recover first the ScalerBoard objects created in general unpacker class
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   fScalerBoardCollection = (TClonesArray*) rootMgr->GetObject("TofRawScalers");
   if(NULL == fScalerBoardCollection) 
   {
      LOG(warn)<<"TTofScomUnpacker::TTofScomUnpacker : no Raw Scalers array! ";
      fuNbScom = 0;
   } // if(NULL == fScalerBoardCollection) 
      else
      {
         // Intialize the ScalerBoard object with each SCOM characteristics
         // TODO: use VME matrix instead of fixed Scalormu types!
         TTofScalerBoard * fScalerBoard = NULL;
         
         // TRIGLOG is always considered as first scaler board!
         UInt_t uTrigOff = 0;
         if( kTRUE == fParUnpack->WithActiveTriglog() )
            uTrigOff = 1;
         
         if( 0 < fuNbScom )
         {
            fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->ConstructedAt(0 + uTrigOff);  // TRIGLOG always first scaler board!
            if( tofscaler::undef == fScalerBoard->GetScalerType() )
               fScalerBoard->SetType( tofscaler::scalormu );
         } // if( 0 < fuNbScom )
         if( 1 < fuNbScom )
         {
            fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->ConstructedAt(1 + uTrigOff);  // TRIGLOG always first scaler board!
            if( tofscaler::undef == fScalerBoard->GetScalerType() )
               fScalerBoard->SetType( tofscaler::scalormubig );
         } // if( 1 < fuNbScom )
      } // else of if(NULL == fScalerBoardCollection) 
   bFirstEvent = kTRUE;
}
TTofScomUnpacker::~TTofScomUnpacker()
{
   DeleteHistos();
}

void TTofScomUnpacker::Clear(Option_t */*option*/)
{
   fParUnpack = NULL;
   fuNbScom    = 0;
   fScalerBoardCollection = NULL;
}

void TTofScomUnpacker::ProcessScom( Int_t iScomIndex, UInt_t* pMbsData, UInt_t uLength )
{
   if( (iScomIndex<0) || (fuNbScom <= static_cast<UInt_t>(iScomIndex) ) ) 
   {
      LOG(error)<<"Error ScalOrMu number "<<iScomIndex<<" out of bound (max "<<fuNbScom<<") ";
      return;
   }
   
   // TRIGLOG is always considered as first scaler board!
   TTofScalerBoard * fScalerBoard = NULL;
   if( kTRUE == fParUnpack->WithActiveTriglog() )
   {
      // First check if ScalerBoard object for Triglog was created already
      if( 2+iScomIndex <= fScalerBoardCollection->GetEntriesFast() )
         fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 1 + iScomIndex, "C"); 
         else for( Int_t iScalBd = fScalerBoardCollection->GetEntriesFast(); iScalBd <= 1 + iScomIndex; iScalBd++ )
            fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( iScalBd); 
   } // if( kTRUE == fParUnpack->WithActiveTriglog() )
      else fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( iScomIndex, "C"); 

   UInt_t uIndx = 0;
   
   UInt_t uNbScalCh = 0;
   if( 0 == iScomIndex )
   {
      if( tofscaler::undef == fScalerBoard->GetScalerType() )
         fScalerBoard->SetType( tofscaler::scalormu );
      uNbScalCh = scalormu::kuNbChan;
   }
   else if( 1 == iScomIndex )
   {
      if( tofscaler::undef == fScalerBoard->GetScalerType() )
         fScalerBoard->SetType( tofscaler::scalormubig );
      uNbScalCh = scalormuBig::kuNbChan;
   }
      else uNbScalCh = 0;
   
   for( UInt_t uScalerInd=0; uScalerInd < uNbScalCh; uScalerInd++)
   {
      // Check is needed to analyse old data with the early version used for tests
      // October 2012:  Only 1 version of the board, 16 channels, testing concept, need clock from TRIGLOG for ref.
      // November 2012: 24 channel version for BUC + small HD (#0), 32 channel version for big HD (#2), both include reference clock
      UInt_t uValue = (UInt_t)( uIndx < uLength ? pMbsData[uIndx] : 0 );
      uIndx++;
      fScalerBoard->SetScalerValue(uScalerInd, uValue);
   } // for( UInt_t uScalerInd=0; uScalerInd < uNbScalCh; uScalerInd++)
   if( uIndx < uLength)
   {
      // Reference clock is inside Tof MBS event only since November 2012
      fScalerBoard->SetRefClk( pMbsData[uIndx] );
      uIndx++;
   } // if( uIndx < uLength)

   fScalerBoard->SetPresentFlag( kTRUE );

   LOG(debug2)<<" ScalOrMu #"<<fParUnpack->GetActiveToAllTypeInd(iScomIndex, tofMbs::scalormu)<<" finished";
   return;
} 

// Histogram management
// TODO: maybe add some options to control which histograms are 
// created and filed (memory management)
void TTofScomUnpacker::CreateHistos()
{
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager!
   
   if( 0 < fuNbScom )
   {
      fhScalers.resize(fuNbScom);
      fvuFirstScalers.resize(fuNbScom);
      
      fhScalers[0] = new TH1I( Form("tof_%s_scalers", tofscaler::ksTdcHistName[ tofscaler::scalormu ].Data() ),
                                "Counts per scaler channel; Channel []; Total counts []", scalormu::kuNbChan, 0, scalormu::kuNbChan);
      fvuFirstScalers[0].resize( scalormu::kuNbChan );
      for( UInt_t uScalerInd=0; uScalerInd < scalormu::kuNbChan; uScalerInd++)
         fvuFirstScalers[0][uScalerInd] = 0;
         
      if( 1 < fuNbScom )
      {
         fhScalers[1] = new TH1I( Form("tof_%s_scalers", tofscaler::ksTdcHistName[ tofscaler::scalormubig ].Data() ),
                                "Counts per scaler channel; Channel []; Total counts []", scalormuBig::kuNbChan, 0, scalormuBig::kuNbChan);
         fvuFirstScalers[1].resize( scalormu::kuNbChan );
         for( UInt_t uScalerInd=0; uScalerInd < scalormuBig::kuNbChan; uScalerInd++)
            fvuFirstScalers[1][uScalerInd] = 0;
      }
   } // if( 0 < fuNbScom )
   
   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
}
void TTofScomUnpacker::FillHistos()
{
   if( 0 < fuNbScom )
   {
      // TRIGLOG is always considered as first scaler board!
      TTofScalerBoard * fScalerBoard;
      if( kTRUE == fParUnpack->WithActiveTriglog() )
         fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 1 ); 
         else fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 0 ); 
         
      for( UInt_t uScalerInd=0; uScalerInd < scalormu::kuNbChan; uScalerInd++)
      {
         if( kTRUE == bFirstEvent )
            fvuFirstScalers[0][uScalerInd] = fScalerBoard->GetScalerValue( uScalerInd );
            else fhScalers[0]->SetBinContent( 1 + uScalerInd, fScalerBoard->GetScalerValue( uScalerInd )
                                                             - fvuFirstScalers[0][uScalerInd] );
      } // for( UInt_t uScalerInd=0; uScalerInd < scalormu::kuNbChan; uScalerInd++)
         
      if( 1 < fuNbScom )
      {
         if( kTRUE == fParUnpack->WithActiveTriglog() )
            fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 2 ); 
            else fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 1 ); 
         
         for( UInt_t uScalerInd=0; uScalerInd < scalormu::kuNbChan; uScalerInd++)
         {
            if( kTRUE == bFirstEvent )
               fvuFirstScalers[1][uScalerInd] = fScalerBoard->GetScalerValue( uScalerInd );
               else fhScalers[1]->SetBinContent( 1 + uScalerInd, fScalerBoard->GetScalerValue( uScalerInd ) 
                                                               - fvuFirstScalers[1][uScalerInd] );
         } // for( UInt_t uScalerInd=0; uScalerInd < scalormu::kuNbChan; uScalerInd++)
      } // if( 1 < fuNbScom )
      
      if( kTRUE == bFirstEvent )
         bFirstEvent = kFALSE;
   } // if( 0 < fuNbScom )
}
void TTofScomUnpacker::WriteHistos( TDirectory* inDir)
{
   TDirectory * oldir = gDirectory;
   
   if( 0 < fuNbScom )
   {
      TDirectory *cdScom = inDir->mkdir( Form( "Unp_%s", tofscaler::ksTdcHistName[ tofscaler::scalormu ].Data() ) );
      cdScom->cd();    // make the "Unp_scom" directory the current directory
      fhScalers[0]->Write();
   
      if( 1 < fuNbScom )
      {
         TDirectory *cdScomBig = inDir->mkdir( Form( "Unp_%s", tofscaler::ksTdcHistName[ tofscaler::scalormubig ].Data() ) );
         cdScomBig->cd();    // make the "Unp_scombig" directory the current directory
         fhScalers[1]->Write();
      } // if( 1 < fuNbScom )
   } // if( 0 < fuNbScom )
   gDirectory->cd( oldir->GetPath() );
}
void TTofScomUnpacker::DeleteHistos()
{
   // not sure if it will not make problems for seeing them
   // => for now just delete histograming variables
   if( 0 < fuNbScom )
   {
      fvuFirstScalers[0].clear();
      if( 1 < fuNbScom )
         fvuFirstScalers[1].clear();
   } // if( 0 < fuNbScom )
   fvuFirstScalers.clear();
} 
