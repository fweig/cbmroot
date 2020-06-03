// ------------------------------------------------------------------
// -----                     TTofOrGenUnpacker                   -----
// -----              Created 22/11/2015 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#include "TTofOrGenUnpacker.h"

// TOF headers
#include "TofDef.h"
#include "TMbsUnpackTofPar.h"
#include "TofOrGenDef.h"
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

TTofOrGenUnpacker::TTofOrGenUnpacker():
   fParUnpack(0),
   fuNbOrGen(0),
   fScalerBoardCollection(NULL),
   fhScalers(),
   fhScalersEclo(),
   fhScalersLemo(),
   bFirstEvent(kTRUE),
   fvuFirstScalers(),
   fvuFirstScalersEclo(),
   fvuFirstScalersLemo()
{
}
TTofOrGenUnpacker::TTofOrGenUnpacker( TMbsUnpackTofPar * parIn ):
   fParUnpack( parIn ),
   fuNbOrGen( parIn->GetNbActiveBoards( tofMbs::orgen ) ),
   fScalerBoardCollection(NULL),
   fhScalers(),
   fhScalersEclo(),
   fhScalersLemo(),
   bFirstEvent(kTRUE),
   fvuFirstScalers(),
   fvuFirstScalersEclo(),
   fvuFirstScalersLemo()
{
   // Recover first the ScalerBoard objects created in general unpacker class
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   fScalerBoardCollection = (TClonesArray*) rootMgr->GetObject("TofRawScalers");
   if(NULL == fScalerBoardCollection) 
   {
      LOG(warn)<<"TTofOrGenUnpacker::TTofOrGenUnpacker : no Raw Scalers array! ";
      fuNbOrGen = 0;
   } // if(NULL == fScalerBoardCollection) 
      else
      {
         // Initialize the ScalerBoard object with each OrGen characteristics
         TTofScalerBoard * fScalerBoard = NULL;
         
         // TRIGLOG is always considered as first scaler board!
         UInt_t uTrigOff = 0;
         if( kTRUE == fParUnpack->WithActiveTriglog() )
            uTrigOff = 1;
         
         LOG(debug)<<"TTofOrGenUnpacker::TTofOrGenUnpacker : Initialize objects for  "
                   <<fuNbOrGen<<" boards ";

         for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
         {
        	 // TRIGLOG always first scaler board!
            fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->ConstructedAt( uScalIndex + uTrigOff);
            if( tofscaler::undef == fScalerBoard->GetScalerType() )
               fScalerBoard->SetType( tofscaler::orgen );
         } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
      } // else of if(NULL == fScalerBoardCollection) 
   bFirstEvent = kTRUE;
}
TTofOrGenUnpacker::~TTofOrGenUnpacker()
{
//   DeleteHistos();
}

void TTofOrGenUnpacker::Clear(Option_t */*option*/)
{
   fParUnpack = NULL;
   fuNbOrGen    = 0;
   fScalerBoardCollection = NULL;
}

void TTofOrGenUnpacker::ProcessOrGen( Int_t iScalIndex, UInt_t* pMbsData, UInt_t uLength )
{
   if( (iScalIndex<0) || (fuNbOrGen <= static_cast<UInt_t>(iScalIndex) ) )
   {
      LOG(error)<<"Error Or Gen number "<<iScalIndex<<" out of bound (max "<<fuNbOrGen<<") ";
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
   
/*   UInt_t uNbScalCh = orgen::kuNbChan;*/

   if( tofscaler::undef == fScalerBoard->GetScalerType() )
	   fScalerBoard->SetType( tofscaler::orgen );
   
   // First readout scalers for the input channels
   for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanIn; uScalerInd++)
   {
      UInt_t uValue = (UInt_t)( uIndx < uLength ? pMbsData[uIndx] : 0 );
      uIndx++;
      fScalerBoard->SetScalerValue(uScalerInd, uValue);
   } // for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanIn; uScalerInd++)
   
   // Then readout the reference clock scaler
   fScalerBoard->SetRefClk( pMbsData[uIndx] );
   uIndx++;

   // Then readout the scalers for the ECL output signal pairs
   for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanEclo; uScalerInd++)
   {
      UInt_t uValue = (UInt_t)( uIndx < uLength ? pMbsData[uIndx] : 0 );
      uIndx++;
	   fScalerBoard->SetScalerValue( orgen::kuNbChanIn + uScalerInd, uValue);
   } // for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanEclo; uScalerInd++)

   // Finally readout the scalers for the LEM output signal (NIM)
   for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanLemo; uScalerInd++)
   {
      UInt_t uValue = (UInt_t)( uIndx < uLength ? pMbsData[uIndx] : 0 );
      uIndx++;
	   fScalerBoard->SetScalerValue( orgen::kuNbChanIn + orgen::kuNbChanEclo + uScalerInd, uValue);
   } // for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanEclo; uScalerInd++)

   if( uLength != uIndx )
      LOG(error)<<" TTofOrGenUnpacker::ProcessOrGen => Or Gen #"
                <<fParUnpack->GetActiveToAllTypeInd(iScalIndex, tofMbs::orgen)
                <<" unpacking length missmatch: length "
                <<uLength<<" VS index "<<uIndx;

   fScalerBoard->SetPresentFlag( kTRUE );

   LOG(debug2)<<" Or Gen #"<<fParUnpack->GetActiveToAllTypeInd(iScalIndex, tofMbs::orgen)
		      <<" finished";
   return;
} 

// Histogram management
// TODO: maybe add some options to control which histograms are 
// created and filed (memory management)
void TTofOrGenUnpacker::CreateHistos()
{
   LOG(debug)<<"TTofOrGenUnpacker::CreateHistos : Create histos for  "
            <<fuNbOrGen<<" boards ";

   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being su

   fhScalers.resize(fuNbOrGen, NULL);
   fvuFirstScalers.resize(fuNbOrGen);

   fhScalersEclo.resize(fuNbOrGen, NULL);
   fhScalersLemo.resize(fuNbOrGen, NULL);
   fvuFirstScalersEclo.resize(fuNbOrGen);
   fvuFirstScalersLemo.resize(fuNbOrGen);

   for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
   {
      fhScalers[uScalIndex] = new TH1I(
            Form("tof_%s_%02u_scalersIn", tofscaler::ksTdcHistName[ tofscaler::orgen ].Data(), uScalIndex ),
            "Counts per scaler channel; Channel []; Total counts []", orgen::kuNbChanIn, 0., orgen::kuNbChanIn);

      fvuFirstScalers[uScalIndex].resize( orgen::kuNbChanIn );
      for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanIn; uScalerInd++)
         fvuFirstScalers[uScalIndex][uScalerInd] = 0;

      fhScalersEclo[uScalIndex] = new TH1I(
            Form("tof_%s_%02u_scalersEclo", tofscaler::ksTdcHistName[ tofscaler::orgen ].Data(), uScalIndex ),
            "Counts per scaler channel; Channel []; Total counts []", orgen::kuNbChanEclo, 0., orgen::kuNbChanEclo);
      fhScalersLemo[uScalIndex] = new TH1I(
            Form("tof_%s_%02u_scalerslemo", tofscaler::ksTdcHistName[ tofscaler::orgen ].Data(), uScalIndex ),
            "Counts per scaler channel; Channel []; Total counts []", orgen::kuNbChanLemo, 0., orgen::kuNbChanLemo);

      fvuFirstScalersEclo[uScalIndex].resize( orgen::kuNbChanEclo );
      for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanEclo; uScalerInd++)
         fvuFirstScalersEclo[uScalIndex][uScalerInd] = 0;
      fvuFirstScalersLemo[uScalIndex].resize( orgen::kuNbChanLemo );
      for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanLemo; uScalerInd++)
         fvuFirstScalersLemo[uScalIndex][uScalerInd] = 0;

   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
   
   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!

   LOG(debug)<<"TTofOrGenUnpacker::CreateHistos : Created histos for  "
             <<fuNbOrGen<<" boards ";
}
void TTofOrGenUnpacker::FillHistos()
{
   LOG(debug)<<"TTofOrGenUnpacker::FillHistos : Fill histos for  "
             <<fuNbOrGen<<" boards ";

   for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
   {

      // TRIGLOG is always considered as first scaler board!
      TTofScalerBoard * fScalerBoard;
      if( kTRUE == fParUnpack->WithActiveTriglog() )
         fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 1 + uScalIndex );
         else fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( 0 + uScalIndex );

      if( kFALSE == fScalerBoard->IsUpdated() )
         continue;

      for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanIn; uScalerInd++)
      {
         if( kTRUE == bFirstEvent )
            fvuFirstScalers[uScalIndex][uScalerInd] = fScalerBoard->GetScalerValue( uScalerInd );
            else fhScalers[uScalIndex]->SetBinContent( 1 + uScalerInd, fScalerBoard->GetScalerValue( uScalerInd )
                                                             - fvuFirstScalers[uScalIndex][uScalerInd] );
      } // for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanIn; uScalerInd++)
      for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanEclo; uScalerInd++)
      {
         if( kTRUE == bFirstEvent )
            fvuFirstScalersEclo[uScalIndex][uScalerInd] = fScalerBoard->GetScalerValue(
                  orgen::kuNbChanIn + uScalerInd );
            else fhScalersEclo[uScalIndex]->SetBinContent( 1 + uScalerInd,
                     fScalerBoard->GetScalerValue( orgen::kuNbChanIn +uScalerInd )
                     - fvuFirstScalersEclo[uScalIndex][uScalerInd] );
      } // for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanEclo; uScalerInd++)
      for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanLemo; uScalerInd++)
      {
         if( kTRUE == bFirstEvent )
            fvuFirstScalersLemo[uScalIndex][uScalerInd] = fScalerBoard->GetScalerValue(
                  orgen::kuNbChanIn + orgen::kuNbChanEclo + uScalerInd );
            else fhScalersLemo[uScalIndex]->SetBinContent( 1 + uScalerInd,
                     fScalerBoard->GetScalerValue( orgen::kuNbChanIn + orgen::kuNbChanEclo + uScalerInd )
                     - fvuFirstScalersLemo[uScalIndex][uScalerInd] );
      } // for( UInt_t uScalerInd=0; uScalerInd < orgen::kuNbChanLemo; uScalerInd++)
   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
   if( kTRUE == bFirstEvent )
      bFirstEvent = kFALSE;

   LOG(debug)<<"TTofOrGenUnpacker::FillHistos : Filled histos for  "
             <<fuNbOrGen<<" boards ";
}
void TTofOrGenUnpacker::WriteHistos( TDirectory* inDir)
{
   TDirectory * oldir = gDirectory;

   LOG(debug)<<"TTofOrGenUnpacker::WriteHistos : Write histos for  "
             <<fuNbOrGen<<" boards ";
   for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
   {
      TDirectory *cdOrGen = inDir->mkdir(
            Form( "Unp_%s_%02u", tofscaler::ksTdcHistName[ tofscaler::orgen ].Data(), uScalIndex ) );
      cdOrGen->cd();    // make the "Unp_OrGen" directory the current directory
      fhScalers[uScalIndex]->Write();
      fhScalersEclo[uScalIndex]->Write();
      fhScalersLemo[uScalIndex]->Write();
   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
   
   gDirectory->cd( oldir->GetPath() );
}
void TTofOrGenUnpacker::DeleteHistos()
{
   // not sure if it will not make problems for seeing them
   // => for now just delete histograming variables

   for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
   {
      fvuFirstScalers[uScalIndex].clear();
      fvuFirstScalersEclo[uScalIndex].clear();
      fvuFirstScalersLemo[uScalIndex].clear();
   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbOrGen; uScalIndex++)
   fvuFirstScalers.clear();
   fvuFirstScalersEclo.clear();
   fvuFirstScalersLemo.clear();
} 
