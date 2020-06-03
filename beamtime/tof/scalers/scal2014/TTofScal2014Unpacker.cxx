// ------------------------------------------------------------------
// -----                     TTofScal2014Unpacker                   -----
// -----              Created 19/04/2014 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#include "TTofScal2014Unpacker.h"

// TOF headers
#include "TofDef.h"
#include "TMbsUnpackTofPar.h"
#include "TofScal2014Def.h"
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

TTofScal2014Unpacker::TTofScal2014Unpacker():
   fParUnpack(0),
   fuNbScal2014(0),
   fScalerBoardCollection(NULL),
   fhScalers(),
   fhScalersAnd(),
   bFirstEvent(kTRUE),
   fvuFirstScalers(),
   fvuFirstScalersAnd()
{
}
TTofScal2014Unpacker::TTofScal2014Unpacker( TMbsUnpackTofPar * parIn ):
   fParUnpack( parIn ),
   fuNbScal2014( parIn->GetNbActiveBoards( tofMbs::scaler2014 ) ),
   fScalerBoardCollection(NULL),
   fhScalers(),
   fhScalersAnd(),
   bFirstEvent(kTRUE),
   fvuFirstScalers(),
   fvuFirstScalersAnd()
{
   // Recover first the ScalerBoard objects created in general unpacker class
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   fScalerBoardCollection = (TClonesArray*) rootMgr->GetObject("TofRawScalers");
   if(NULL == fScalerBoardCollection) 
   {
      LOG(warn)<<"TTofScal2014Unpacker::TTofScal2014Unpacker : no Raw Scalers array! ";
      fuNbScal2014 = 0;
   } // if(NULL == fScalerBoardCollection) 
      else
      {
         // Initialize the ScalerBoard object with each Scal2014 characteristics
         TTofScalerBoard * fScalerBoard = NULL;
         
         // TRIGLOG is always considered as first scaler board!
         UInt_t uTrigOff = 0;
         if( kTRUE == fParUnpack->WithActiveTriglog() )
            uTrigOff = 1;
         
         LOG(debug)<<"TTofScal2014Unpacker::TTofScal2014Unpacker : Initialize objects for  "
                  <<fuNbScal2014<<" boards ";

         for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
         {
        	 // TRIGLOG always first scaler board!
            fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->ConstructedAt( uScalIndex + uTrigOff);
            if( tofscaler::undef == fScalerBoard->GetScalerType() )
               fScalerBoard->SetType( tofscaler::scaler2014 );
         } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
      } // else of if(NULL == fScalerBoardCollection) 
   bFirstEvent = kTRUE;
}
TTofScal2014Unpacker::~TTofScal2014Unpacker()
{
//   DeleteHistos();
}

void TTofScal2014Unpacker::Clear(Option_t */*option*/)
{
   fParUnpack = NULL;
   fuNbScal2014    = 0;
   fScalerBoardCollection = NULL;
}

void TTofScal2014Unpacker::ProcessScal2014( Int_t iScalIndex, UInt_t* pMbsData, UInt_t uLength )
{
   if( (iScalIndex<0) || (fuNbScal2014 <= static_cast<UInt_t>(iScalIndex) ) )
   {
      LOG(error)<<"Error Scaler2014 number "<<iScalIndex<<" out of bound (max "<<fuNbScal2014<<") ";
      return;
   }
   
   // TRIGLOG is always considered as first scaler board!
   TTofScalerBoard * fScalerBoard = NULL;
   if( kTRUE == fParUnpack->WithActiveTriglog() )
   {
      // First check if ScalerBoard object for Triglog was created already
      if( 2 + iScalIndex <= fScalerBoardCollection->GetEntriesFast() )
         fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 1 + iScalIndex, "C");
         else for( Int_t iScalBd = fScalerBoardCollection->GetEntriesFast(); iScalBd <= 1 + iScalIndex; iScalBd++ )
            fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( iScalBd); 
   } // if( kTRUE == fParUnpack->WithActiveTriglog() )
      else fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( iScalIndex, "C");

   UInt_t uIndx = 0;
   
/*   UInt_t uNbScalCh = scaler2014::kuNbChan;*/

   if( tofscaler::undef == fScalerBoard->GetScalerType() )
	   fScalerBoard->SetType( tofscaler::scaler2014 );
   
   // First readout scalers for the input channels
   for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanIn; uScalerInd++)
   {
      UInt_t uValue = (UInt_t)( uIndx < uLength ? pMbsData[uIndx] : 0 );
      uIndx++;
      fScalerBoard->SetScalerValue(uScalerInd, uValue);
   } // for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanIn; uScalerInd++)
   
   // Then readout the reference clock scaler
   fScalerBoard->SetRefClk( pMbsData[uIndx] );
   uIndx++;

   // Finally readout the scalers for the AND of the input signal pairs
   for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanAnd; uScalerInd++)
   {
      UInt_t uValue = (UInt_t)( uIndx < uLength ? pMbsData[uIndx] : 0 );
      uIndx++;
	   fScalerBoard->SetScalerValue( scaler2014::kuNbChanIn + uScalerInd, uValue);
   } // for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanAnd; uScalerInd++)

   if( uLength != uIndx )
      LOG(error)<<" TTofScal2014Unpacker::ProcessScal2014 => Scaler 2014 #"
                <<fParUnpack->GetActiveToAllTypeInd(iScalIndex, tofMbs::scaler2014)
                <<" unpacking length missmatch: length "
                <<uLength<<" VS index "<<uIndx;

   fScalerBoard->SetPresentFlag( kTRUE );

   LOG(debug2)<<" Scaler 2014 #"<<fParUnpack->GetActiveToAllTypeInd(iScalIndex, tofMbs::scaler2014)
		      <<" finished";
   return;
} 

// Histogram management
// TODO: maybe add some options to control which histograms are 
// created and filed (memory management)
void TTofScal2014Unpacker::CreateHistos()
{
   LOG(debug)<<"TTofScal2014Unpacker::CreateHistos : Create histos for  "
            <<fuNbScal2014<<" boards ";

   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being su

   fhScalers.resize(fuNbScal2014, NULL);
   fvuFirstScalers.resize(fuNbScal2014);

   fhScalersAnd.resize(fuNbScal2014, NULL);
   fvuFirstScalersAnd.resize(fuNbScal2014);

   for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
   {
      fhScalers[uScalIndex] = new TH1I(
            Form("tof_%s_%02u_scalersIn", tofscaler::ksTdcHistName[ tofscaler::scaler2014 ].Data(), uScalIndex ),
            "Counts per scaler channel; Channel []; Total counts []", scaler2014::kuNbChanIn, 0., scaler2014::kuNbChanIn);

      fvuFirstScalers[uScalIndex].resize( scaler2014::kuNbChanIn );
      for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanIn; uScalerInd++)
         fvuFirstScalers[uScalIndex][uScalerInd] = 0;

      fhScalersAnd[uScalIndex] = new TH1I(
            Form("tof_%s_%02u_scalersAnd", tofscaler::ksTdcHistName[ tofscaler::scaler2014 ].Data(), uScalIndex ),
            "Counts per scaler channel; Channel []; Total counts []", scaler2014::kuNbChanAnd, 0., scaler2014::kuNbChanAnd);

      fvuFirstScalersAnd[uScalIndex].resize( scaler2014::kuNbChanAnd );
      for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanAnd; uScalerInd++)
         fvuFirstScalersAnd[uScalIndex][uScalerInd] = 0;

   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
   
   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!

   LOG(debug)<<"TTofScal2014Unpacker::CreateHistos : Created histos for  "
             <<fuNbScal2014<<" boards ";
}
void TTofScal2014Unpacker::FillHistos()
{
   LOG(debug)<<"TTofScal2014Unpacker::FillHistos : Fill histos for  "
             <<fuNbScal2014<<" boards ";

   for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
   {

      // TRIGLOG is always considered as first scaler board!
      TTofScalerBoard * fScalerBoard;
      if( kTRUE == fParUnpack->WithActiveTriglog() )
         fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 1 + uScalIndex );
         else fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 0 + uScalIndex );

      if( kFALSE == fScalerBoard->IsUpdated() )
         continue;

      for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanIn; uScalerInd++)
      {
         if( kTRUE == bFirstEvent )
            fvuFirstScalers[uScalIndex][uScalerInd] = fScalerBoard->GetScalerValue( uScalerInd );
            else fhScalers[uScalIndex]->SetBinContent( 1 + uScalerInd, fScalerBoard->GetScalerValue( uScalerInd )
                                                             - fvuFirstScalers[uScalIndex][uScalerInd] );
      } // for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanIn; uScalerInd++)
      for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanAnd; uScalerInd++)
      {
         if( kTRUE == bFirstEvent )
            fvuFirstScalersAnd[uScalIndex][uScalerInd] = fScalerBoard->GetScalerValue(
                  scaler2014::kuNbChanIn + uScalerInd );
            else fhScalersAnd[uScalIndex]->SetBinContent( 1 + uScalerInd,
                     fScalerBoard->GetScalerValue( scaler2014::kuNbChanIn +uScalerInd )
                     - fvuFirstScalersAnd[uScalIndex][uScalerInd] );
      } // for( UInt_t uScalerInd=0; uScalerInd < scaler2014::kuNbChanAnd; uScalerInd++)
   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
   if( kTRUE == bFirstEvent )
      bFirstEvent = kFALSE;

   LOG(debug)<<"TTofScal2014Unpacker::FillHistos : Filled histos for  "
             <<fuNbScal2014<<" boards ";
}
void TTofScal2014Unpacker::WriteHistos( TDirectory* inDir)
{
   TDirectory * oldir = gDirectory;

   LOG(debug)<<"TTofScal2014Unpacker::WriteHistos : Write histos for  "
             <<fuNbScal2014<<" boards ";
   for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
   {
      TDirectory *cdScal2014 = inDir->mkdir(
            Form( "Unp_%s_%02u", tofscaler::ksTdcHistName[ tofscaler::scaler2014 ].Data(), uScalIndex ) );
      cdScal2014->cd();    // make the "Unp_Scal2014" directory the current directory
      fhScalers[uScalIndex]->Write();
      fhScalersAnd[uScalIndex]->Write();
   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
   
   gDirectory->cd( oldir->GetPath() );
}
void TTofScal2014Unpacker::DeleteHistos()
{
   // not sure if it will not make problems for seeing them
   // => for now just delete histograming variables

   for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
   {
      fvuFirstScalers[uScalIndex].clear();
      fvuFirstScalersAnd[uScalIndex].clear();
   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbScal2014; uScalIndex++)
   fvuFirstScalers.clear();
   fvuFirstScalersAnd.clear();
} 
