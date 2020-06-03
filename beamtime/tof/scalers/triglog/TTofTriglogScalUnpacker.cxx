// ------------------------------------------------------------------
// -----                     TTofTriglogScalUnpacker                -----
// -----              Created 03/07/2013 by P.-A. Loizeau       -----
// -----              Adapted from TTriglogProc.cxx by S. Linev -----
// -----                 and TMbsCrateProc.cxx by P.-A. Loizeau ----
// ------------------------------------------------------------------

#include "TTofTriglogScalUnpacker.h"

// TOF headers
#include "TofDef.h"
#include "TMbsUnpackTofPar.h"
#include "TofTriglogDef.h"
#include "TofScalerDef.h"
#include "TTofTriglogBoard.h"
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
#include "TString.h"

TTofTriglogScalUnpacker::TTofTriglogScalUnpacker():
   fParUnpack(0),
   fScalerBoardCollection(NULL),
   fuNbTriglogScal( 0 ),
   fuTrigOff( 0 ),
   fuTotalTriggerCount(),
   fuFirstMbsTime(),
   fuLastMbsTime(),
   fuFirstScaler(),
   fuLastScaler(),
   fhScalers(),
   fvuFirstScalers(),
   fdFirstMbsTime(),
   fdPrevMbsTime(),
   fdCurrMbsTime(),
   fuLastRefClk(),
   fhRefClkRate(),
   fhRefClkRateEvo(),
   fTriglogBoardCollection(NULL),
   fdEvoRangeUser(-1),
   fdEvoBinSzUser(-1),
   fhScalersCountsEvo()
{
}
TTofTriglogScalUnpacker::TTofTriglogScalUnpacker( TMbsUnpackTofPar * parIn ):
   fParUnpack( parIn ),
   fScalerBoardCollection(NULL),
   fuNbTriglogScal( parIn->GetNbActiveBoards( tofMbs::triglogscal ) ),
   fuTrigOff( 0 ),
   fuTotalTriggerCount(),
   fuFirstMbsTime(),
   fuLastMbsTime(),
   fuFirstScaler(),
   fuLastScaler(),
   fhScalers(),
   fvuFirstScalers(),
   fdFirstMbsTime(),
   fdPrevMbsTime(),
   fdCurrMbsTime(),
   fuLastRefClk(),
   fhRefClkRate(),
   fhRefClkRateEvo(),
   fTriglogBoardCollection(NULL),
   fdEvoRangeUser(-1),
   fdEvoBinSzUser(-1),
   fhScalersCountsEvo()
{
   // Some triglog board can be used as scaler board instead of trigger source
   if( 0 < fuNbTriglogScal )
   {
      // Recover first the TRIGLOG and SCALER board objects created in general unpacker class
      FairRootManager* rootMgr = FairRootManager::Instance();

      fScalerBoardCollection = (TClonesArray*) rootMgr->GetObject("TofRawScalers");
      if(NULL == fScalerBoardCollection)
      {
         LOG(warn)<<"TTofTriglogScalUnpacker::TTofTriglogScalUnpacker : no Raw Scalers array! ";
      } // if(NULL == fScalerBoardCollection)
         else
         {
            // Initialize the ScalerBoard object with TRIGLOG characteristics
            TTofScalerBoard * fScalerBoard = NULL;

            // TRIGLOG is always considered as first scaler board!
            fuTrigOff = 0;
            if( kTRUE == fParUnpack->WithActiveTriglog() )
            {
               fuTrigOff = 1;
               fTriglogBoardCollection = (TClonesArray*) rootMgr->GetObject("TofTriglog");
            } // if( kTRUE == fParUnpack->WithActiveTriglog() )
            // There may be some simple scaler boards
            fuTrigOff += parIn->GetNbActiveBoards( tofMbs::scaler2014 );
            fuTrigOff += parIn->GetNbActiveBoards( tofMbs::orgen );

            LOG(debug)<<"TTofTriglogScalUnpacker::TTofTriglogScalUnpacker : Initialize objects for "
                  << fuNbTriglogScal << " Triglog boards used as scaler ";

            for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
            {
             // TRIGLOG always first scaler board!
             // SCALORMU are always next, followed by Triglog used as scalers!
               fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->ConstructedAt( uScalIndex + fuTrigOff);
               if( tofscaler::undef == fScalerBoard->GetScalerType() )
                  fScalerBoard->SetType( tofscaler::triglogscal );
            } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
         } // else of if(NULL == fScalerBoardCollection)

      fuTotalTriggerCount.resize(fuNbTriglogScal);
      fuFirstMbsTime.resize(fuNbTriglogScal);
      fuLastMbsTime.resize(fuNbTriglogScal);
      fuFirstScaler.resize(fuNbTriglogScal);
      fuLastScaler.resize(fuNbTriglogScal);
      fdFirstMbsTime.resize(fuNbTriglogScal);
      fdPrevMbsTime.resize(fuNbTriglogScal);
      fdCurrMbsTime.resize(fuNbTriglogScal);
      fuLastRefClk.resize(fuNbTriglogScal);
      for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
      {
         // Initialize variable used in filling
         fdFirstMbsTime[uScalIndex] = 0;
         fdPrevMbsTime[uScalIndex]  = -1;
         fdCurrMbsTime[uScalIndex]  = -1;
         fuLastRefClk[uScalIndex]   = 0;
         fuTotalTriggerCount[uScalIndex] = 0;
         fuFirstMbsTime[uScalIndex] = 0;
         fuLastMbsTime[uScalIndex]  = 0;

         fuFirstScaler[uScalIndex].resize(triglog::kuNbScalers);
         fuLastScaler[uScalIndex].resize(triglog::kuNbScalers);
         for( UInt_t uScaler = 0; uScaler < triglog::kuNbScalers; uScaler++)
         {
            fuFirstScaler[uScalIndex][uScaler].resize(triglog::kuNbChan);
            fuLastScaler[uScalIndex][uScaler].resize(triglog::kuNbChan);
         } // for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
      } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
   } // if(  0 < fuNbTriglogScal )
      else LOG(error)<<"TTofTriglogScalUnpacker::TTofTriglogScalUnpacker => No TRIGLOG board active as scaler!!!!!"
                   <<" recheck your VME address matrix ";
}
TTofTriglogScalUnpacker::~TTofTriglogScalUnpacker()
{
   // Some triglog board can be used as scaler board instead of trigger source
   if(  0 < fuNbTriglogScal )
   {
      for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
      {
         for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
         {
            fuFirstScaler[uScalIndex][uScaler].clear();
            fuLastScaler[uScalIndex][uScaler].clear();
         } // for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
         fuFirstScaler[uScalIndex].clear();
         fuLastScaler[uScalIndex].clear();
      } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
      fuTotalTriggerCount.clear();
      fuFirstMbsTime.clear();
      fuLastMbsTime.clear();
      fuFirstScaler.clear();
      fuLastScaler.clear();
   } // if(  0 < fuNbTriglogScal )
      else LOG(error)<<"TTofTriglogScalUnpacker::~TTofTriglogScalUnpacker => No TRIGLOG board active as scaler!!!!!"
                      <<" recheck your VME address matrix ";

//   DeleteHistos();
}

void TTofTriglogScalUnpacker::Clear(Option_t */*option*/)
{
   fParUnpack = NULL;
   fuNbTriglogScal    = 0;
   fScalerBoardCollection = NULL;
}

void TTofTriglogScalUnpacker::ProcessTriglogScal( Int_t iTrigScalIndex, UInt_t* pMbsData, UInt_t uLength )
{
   if( (iTrigScalIndex<0) || (fuNbTriglogScal <= static_cast<UInt_t>(iTrigScalIndex)) )
   {
      LOG(error)<<"Error TriglogScal number "<<iTrigScalIndex<<" out of bound (max "<<fuNbTriglogScal<<") ";
      return;
   }

   // Getting output objects
   // TRIGLOG always first scaler board!
   TTofScalerBoard * fScalerBoard = NULL;
   if( kTRUE == fParUnpack->WithActiveTriglog() )
   {
      // First check if ScalerBoard object for Triglog was created already
      // and check if ScalerBoard object for ScalOrMu14 were created already
      if( 1 + static_cast<Int_t>(fuTrigOff) + iTrigScalIndex <= fScalerBoardCollection->GetEntriesFast() )
         fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( fuTrigOff + iTrigScalIndex, "C");
         else for( Int_t iScalBd = fScalerBoardCollection->GetEntriesFast(); iScalBd <= static_cast<Int_t>(fuTrigOff) + iTrigScalIndex; iScalBd++ )
            fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( iScalBd);
   } // if( kTRUE == fParUnpack->WithActiveTriglog() )
      else fScalerBoard = (TTofScalerBoard*) fScalerBoardCollection->ConstructedAt( fuTrigOff + iTrigScalIndex, "C");

   if( tofscaler::undef == fScalerBoard->GetScalerType() )
      fScalerBoard->SetType( tofscaler::triglogscal );

   UInt_t uIndx = 0;

   // Jump TRIGLOG not scaler data
   Int_t sync_num = pMbsData[uIndx];
//   fTriglogBoard->SetSyncNb( sync_num );
   uIndx++;
   LOG(debug2)<<" ===================== TRIGLOG SCAL EVENT ============ " << sync_num % 0x1000000 ;
//   fTriglogBoard->SetTriggPatt( (UInt_t) pMbsData[uIndx] );
   uIndx++;
//   fTriglogBoard->SetMbsTimeSec( (UInt_t) pMbsData[uIndx] );
   UInt_t uMbsTime = (UInt_t) pMbsData[uIndx];
   uIndx++;
   LOG(debug2)<<" TRIGLOG SCAL UNPACK test 0 "<< iTrigScalIndex ;
//   fTriglogBoard->SetMbsTimeMilliSec( (UInt_t) pMbsData[uIndx] );
   UInt_t uMbsTimeMs = (UInt_t) pMbsData[uIndx];
   LOG(debug2)<<" TRIGLOG SCAL UNPACK test 0a "<< iTrigScalIndex ;
   fdCurrMbsTime[iTrigScalIndex] = uMbsTime + uMbsTimeMs*1e-3; // BROKEN: Will not work as histo filling in different instance
   uIndx++;
   LOG(debug2)<<" TRIGLOG SCAL UNPACK test 1";

   for( UInt_t uScaler = 0; uScaler < triglog::kuNbScalers; uScaler++)
   {
      // SL 20.10.11 MBS readout was changed and two more scalers were included
      // to be able process old files, add this break condition;

      for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
      {
         // SL: put protection against different length of this subevent
         // also trigger-pattern was not correctly counted
         UInt_t uValue = (UInt_t)(uIndx < uLength ? pMbsData[uIndx] : 0);
         uIndx++;
         fScalerBoard->SetScalerValue( uCh, uValue, uScaler );
      } // for( UInt uCh = 0; uCh < triglog::kuNbChan; uCh++) {
   } // for( UInt uScaler = 0; uScaler < triglog::kuNbScalers; uScaler++)

   LOG(debug2)<<" TRIGLOG SCAL UNPACK test 2";
   if( uIndx < uLength)
   {
      // Jump TRIGLOG not scaler data
      // Input pattern is inside Tof MBS event only since August 2012
//      fTriglogBoard->SetInpPatt( pMbsData[uIndx] );
      uIndx++;
   } // if( uIndx < uLength)
   if( uIndx < uLength)
   {
      // Reference clock is inside Tof MBS event only since October 2012
      fScalerBoard->SetRefClk(  pMbsData[uIndx] );
      uIndx++;
   } // if( uIndx < uLength)
   LOG(debug2)<<" TRIGLOG SCAL UNPACK finished";

   fScalerBoard->SetPresentFlag( kTRUE );

   UpdateStats( iTrigScalIndex, uMbsTime );

   return;
}
void TTofTriglogScalUnpacker::UpdateStats( Int_t iTrigScalIndex, UInt_t uMbsTime )
{
   if( (iTrigScalIndex<0) || (fuNbTriglogScal <= static_cast<UInt_t>(iTrigScalIndex)) )
   {
      LOG(error)<<"Error TriglogScal number "<<iTrigScalIndex<<" out of bound (max "<<fuNbTriglogScal<<") ";
      return;
   }
   // Some triglog board can be used as scaler board instead of trigger source
   if(  0 < fuNbTriglogScal )
   {
      // Getting output objects
      // TRIGLOG always first scaler board! then ScalOrMu/Scal2014
      TTofScalerBoard  * fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->At(fuTrigOff + iTrigScalIndex);
      if(  NULL == fScalerBoard )
      {
         LOG(error)<<"TTofTriglogScalUnpacker::UpdateStats => Board objects not existing!!!!";
         LOG(error)<<"TTofTriglogScalUnpacker::UpdateStats => TTofScalerBoard = "<<fScalerBoard;
         LOG(error)<<"TTofTriglogScalUnpacker::UpdateStats => Probably the TRIGLOG SCALER sub-event is not there in this event!";
         return;
      } // if( | NULL == fScalerBoard )

      // Preparing output at the end:
         // MBS time is time in s since LINUX epoch (circa 1970) => never 0 except on first event!
      for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
         for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
         {
            fuLastScaler[iTrigScalIndex][uScaler][uCh] = fScalerBoard->GetScalerValue( uCh, uScaler);
            if( fuFirstMbsTime[iTrigScalIndex] == 0 )
               fuFirstScaler[iTrigScalIndex][uScaler][uCh] = fuLastScaler[iTrigScalIndex][uScaler][uCh];
         } // for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
      fuLastMbsTime[iTrigScalIndex]     = uMbsTime;
      if( 0 == fuFirstMbsTime[iTrigScalIndex] )
         fuFirstMbsTime[iTrigScalIndex] = fuLastMbsTime[iTrigScalIndex];
   } // if(  0 < fuNbTriglogScal )
      else LOG(error)<<"TTofTriglogScalUnpacker::UpdateStats => No TRIGLOG board active as scaler!!!!!"
                <<" recheck your VME address matrix ";
}

void TTofTriglogScalUnpacker::FinishTriglog( )
{
   // Some triglog board can be used as scaler board instead of trigger source
   if(  0 < fuNbTriglogScal )
   {
      for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
      {
         LOG(info)<<Form("-------- Triglog Scaler board %02u ----", uScalIndex);
         Double_t dRuntime = fuLastMbsTime[uScalIndex] - fuFirstMbsTime[uScalIndex];
         LOG(info)<<Form("Total MBS run time %6.0f sec.", dRuntime);

         if( 0 < dRuntime )
         {
            TString sLine = "";
            for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
            {
               sLine = Form("  Ch %2d", uCh );
               for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
                  sLine += Form("   %9u", fuLastScaler[uScalIndex][uScaler][uCh] - fuFirstScaler[uScalIndex][uScaler][uCh]);
               LOG(info)<<sLine;
            } // for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)

            LOG(info)<<Form("---------------------------------------");

            for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
            {
               sLine = Form("  Ch %2d", uCh );
               for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
                  sLine += Form("   %9.2f", (fuLastScaler[uScalIndex][uScaler][uCh] - fuFirstScaler[uScalIndex][uScaler][uCh])/dRuntime);
               LOG(info)<<sLine;
            } // for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
         } // if( 0 < dRuntime )
         LOG(info)<<Form("---------------------------------------");
      } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
   } // if(  0 < fuNbTriglogScal )
      else LOG(error)<<"TTofTriglogScalUnpacker::FinishTriglog => No TRIGLOG board active as scaler!!!!!"
             <<" recheck your VME address matrix ";
}

// Histogram management
// TODO: maybe add some options to control which histograms are
// created and filed (memory management)
void TTofTriglogScalUnpacker::CreateHistos()
{
   // Some triglog board can be used as scaler board instead of trigger source
   if(  0 < fuNbTriglogScal )
   {
      TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
      gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !

      Double_t dEvoRange = triglog::kdEvoRange;
      Double_t dEvoBinNb = triglog::kdEvoRange / triglog::kdEvoBin;
      if( 0 < fdEvoRangeUser && 0 < fdEvoBinSzUser && fdEvoBinSzUser < fdEvoRangeUser )
      {
         dEvoRange = fdEvoRangeUser;
         dEvoBinNb = static_cast<Int_t>(fdEvoRangeUser) / static_cast<Int_t>(fdEvoBinSzUser);
         LOG(info)<<"TTofTriglogScalUnpacker::CreateHistos => Set the scaler evolution plot with user range: "
                  << dEvoRange << " s/range " << fdEvoBinSzUser << " s/bin " << dEvoBinNb << " bins ";
      } // if( 0 < fdEvoRangeUser && 0 < fdEvoBinSzUser && fdEvoBinSzUser < fdEvoRangeUser )


      fhRefClkRate.resize(fuNbTriglogScal);
      fhRefClkRateEvo.resize(fuNbTriglogScal);
      fhScalers.resize(fuNbTriglogScal);
      fhScalersCountsEvo.resize(fuNbTriglogScal);
      for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
      {
         fhRefClkRate[uScalIndex]    = new TH1I( Form("tof_%s_%02u_refclk", tofscaler::ksTdcHistName[ tofscaler::triglogscal ].Data(),
                                                                            uScalIndex ),
                              "Rate of the reference clock; Freq. [Hz]",
                              (Int_t)(2*triglog::kdRefClkFreq/1000), 0.0 , 2*triglog::kdRefClkFreq );
         fhRefClkRateEvo[uScalIndex] = new TH1I( Form("tof_%s_%02u_refclk_evo", tofscaler::ksTdcHistName[ tofscaler::triglogscal ].Data(),
               uScalIndex ),
                              "Reference clock counts per second; Time [s]; Counts []", 3600, 0.0, 3600 );

         fhScalers[uScalIndex].resize(triglog::kuNbScalers);
         fhScalersCountsEvo[uScalIndex].resize(triglog::kuNbScalers);
         for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
         {
            fhScalers[uScalIndex][uScaler] = new TH1I( Form("tof_%s_%02u_scalers%02u", tofscaler::ksTdcHistName[ tofscaler::triglogscal ].Data(),
                                                                                       uScalIndex, uScaler ),
                                           Form("Counts per scaler channel in scaler %02u; Channel []; Total counts []", uScaler),
                                           triglog::kuNbChan, 0, triglog::kuNbChan);
            fhScalersCountsEvo[uScalIndex][uScaler].resize(triglog::kuNbChan);
            for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
               fhScalersCountsEvo[uScalIndex][uScaler][uCh] = new TH1I( Form("tof_%s_%02u_scalers_evo_%02u_%02u",
                                                                               tofscaler::ksTdcHistName[ tofscaler::triglogscal ].Data(), 
                                                                               uScalIndex, uScaler, uCh ),
                                           Form("Channel %02d in scaler %02d counts evolution; Time [s]; Counts []", uCh, uScaler), 
                                           static_cast<Int_t>(dEvoBinNb), 0.0, dEvoRange );

         } // for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
      } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)

      gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
   } // if(  0 < fuNbTriglogScal )
      else LOG(error)<<"TTofTriglogScalUnpacker::CreateHistos => No TRIGLOG board active as scaler!!!!!"
          <<" recheck your VME address matrix ";
}
void TTofTriglogScalUnpacker::FillHistos()
{
   // Some triglog board can be used as scaler board instead of trigger source
   for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
   {
      // Getting output objects
      // TRIGLOG always first scaler board! then ScalOrMu/Scal2014
      TTofScalerBoard  * fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->At(fuTrigOff + uScalIndex);

      if( NULL == fScalerBoard )
      {
         LOG(error)<<"TTofTriglogScalUnpacker::FillHistos => Board objects not existing!!!!";
         LOG(error)<<"TTofTriglogScalUnpacker::FillHistos => TTofScalerBoard = "<<fScalerBoard;
         LOG(error)<<"TTofTriglogScalUnpacker::FillHistos => Probably the TRIGLOG SCALER sub-event is not there in this event!";
         return;
      } // if( NULL == fScalerBoard )

      LOG(debug3)<<(fScalerBoardCollection->GetEntries());

      // BROKEN: Will not work as data filling in different instance
/*
      if( 0 == fdFirstMbsTime[uScalIndex] )
         // MBS time is time in s since LINUX epoch (circa 1970) => never 0 except on first event!
         fdFirstMbsTime[uScalIndex] = fdCurrMbsTime[uScalIndex];
*/
      // Fix attempt
      if( kTRUE == fParUnpack->WithActiveTriglog() )
      {
         TTofTriglogBoard * fTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->At(0); // Always only 1 TRIGLOG board!

         fdCurrMbsTime[uScalIndex] = fTriglogBoard->GetMbsTimeSec();
         if( 0 == fdFirstMbsTime[uScalIndex] )
            // MBS time is time in s since LINUX epoch (circa 1970) => never 0 except on first event!
            fdFirstMbsTime[uScalIndex] = fdCurrMbsTime[uScalIndex];
      }

      if( ( 0 == fdPrevMbsTime[uScalIndex] ) || ( fdPrevMbsTime[uScalIndex] + 0.05 < fdCurrMbsTime[uScalIndex] ) )
      {
         Double_t dTimeDiff = fdCurrMbsTime[uScalIndex] - fdPrevMbsTime[uScalIndex];
         if( 0 < fdPrevMbsTime[uScalIndex] && dTimeDiff < 0.3 )
         {
            fhRefClkRate[uScalIndex]->Fill( (Double_t)(fScalerBoard->GetRefClk() - fuLastRefClk[uScalIndex] ) /
                                dTimeDiff );
            fhRefClkRateEvo[uScalIndex]->Fill(fdCurrMbsTime[uScalIndex] - fdFirstMbsTime[uScalIndex],
                                              fScalerBoard->GetRefClk() - fuLastRefClk[uScalIndex]);
         } // if( 0 < fdPrevMbsTime )

         fdPrevMbsTime[uScalIndex] = fdCurrMbsTime[uScalIndex];
         fuLastRefClk[uScalIndex] = fScalerBoard->GetRefClk();
      } // if( ( 0 == fdPrevMbsTime ) || ( dMbsTime > fdPrevMbsTime + 0.1 ) )

      for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
         for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
         {
            fhScalers[uScalIndex][uScaler]->SetBinContent( 1 + uCh, fScalerBoard->GetScalerValue( uCh, uScaler)
                                                      - fuFirstScaler[uScalIndex][uScaler][uCh] );
            fhScalersCountsEvo[uScalIndex][uScaler][uCh]->SetBinContent( fdCurrMbsTime[uScalIndex] - fdFirstMbsTime[uScalIndex],
                                                        fScalerBoard->GetScalerValue( uCh, uScaler) );
         } // for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
}
void TTofTriglogScalUnpacker::WriteHistos( TDirectory* inDir)
{
   // Some triglog board can be used as scaler board instead of trigger source
   for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
   {
      TDirectory * oldir = gDirectory;

      TDirectory *cdTriglog = inDir->mkdir( Form( "Unp_%s_%02u", tofscaler::ksTdcHistName[ tofscaler::triglogscal ].Data(), uScalIndex ) );
      cdTriglog->cd();    // make the "Unp_triglog" directory the current directory

      fhRefClkRate[uScalIndex]->Write();
      fhRefClkRateEvo[uScalIndex]->Write();
      for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
      {
         fhScalers[uScalIndex][uScaler]->Write();
         for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)
            fhScalersCountsEvo[uScalIndex][uScaler][uCh]->Write();
      } // for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)

      gDirectory->cd( oldir->GetPath() );
   } // for( UInt_t uScalIndex = 0; uScalIndex < fuNbTriglogScal; uScalIndex++)
}
void TTofTriglogScalUnpacker::DeleteHistos()
{
/*
   // Some triglog board can be used as scaler board instead of trigger source
   if(  0 < fuNbTriglogScal )
   {
   } // if(  0 < fuNbTriglogScal )
      else LOG(error)<<"TTofTriglogScalUnpacker::DeleteHistos => No TRIGLOG board active as scaler!!!!!"
            <<" recheck your VME address matrix ";
*/
}
