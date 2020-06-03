// ------------------------------------------------------------------
// -----                     TTofTriglogUnpacker                -----
// -----              Created 03/07/2013 by P.-A. Loizeau       -----
// -----              Adapted from TTriglogProc.cxx by S. Linev -----
// -----                 and TMbsCrateProc.cxx by P.-A. Loizeau ----
// ------------------------------------------------------------------ 

#include "TTofTriglogUnpacker.h"

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

TTofTriglogUnpacker::TTofTriglogUnpacker():
   fParUnpack(0),
   fTriglogBoardCollection(NULL),
   fScalerBoardCollection(NULL),
   fuTotalTriggerCount(0),
   fuFirstMbsTime(0),
   fuLastMbsTime(0),
   fuFirstScaler(NULL),
   fuLastScaler(NULL),
   fhTriglogSyncs(NULL),
   fuLastSync(0),
   fhSpillRate(NULL),
   fTriggerPattern(NULL),
   fInputPattern(NULL),
   fhScalers(),
   fvuFirstScalers(),
   fdFirstMbsTime(0.),
   fdPrevMbsTime(0.),
   fuLastRefClk(0),
   fhRefClkRate(NULL),
   fhRefClkRateEvo(NULL)
{
}
TTofTriglogUnpacker::TTofTriglogUnpacker( TMbsUnpackTofPar * parIn ):
   fParUnpack( parIn ),
   fTriglogBoardCollection(NULL),
   fScalerBoardCollection(NULL),
   fuTotalTriggerCount(0),
   fuFirstMbsTime(0),
   fuLastMbsTime(0),
   fuFirstScaler(NULL),
   fuLastScaler(NULL),
   fhTriglogSyncs(NULL),
   fuLastSync(0),
   fhSpillRate(NULL),
   fTriggerPattern(NULL),
   fInputPattern(NULL),
   fhScalers(),
   fvuFirstScalers(),
   fdFirstMbsTime(0.),
   fdPrevMbsTime(0.),
   fuLastRefClk(0),
   fhRefClkRate(NULL),
   fhRefClkRateEvo(NULL)
{
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   {
      // Recover first the TRIGLOG and SCALER board objects created in general unpacker class
      FairRootManager* rootMgr = FairRootManager::Instance();
      
      fTriglogBoardCollection = (TClonesArray*) rootMgr->GetObject("TofTriglog");
      if(NULL == fTriglogBoardCollection) 
      {
         LOG(warn)<<"TTofTriglogUnpacker::TTofTriglogUnpacker : no TRIGLOG object in manager! ";
      } // if(NULL == fTriglogBoardCollection) 
      
      fScalerBoardCollection = (TClonesArray*) rootMgr->GetObject("TofRawScalers");
      if(NULL == fScalerBoardCollection) 
      {
         LOG(warn)<<"TTofTriglogUnpacker::TTofTriglogUnpacker : no Raw Scalers array! ";
      } // if(NULL == fScalerBoardCollection) 
         else
         {
            // Intialize the ScalerBoard object with TRIGLOG characteristics
            TTofScalerBoard  * fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->ConstructedAt(0);  // TRIGLOG always first scaler board!
            if( tofscaler::undef == fScalerBoard->GetScalerType() )
               fScalerBoard->SetType( tofscaler::triglog );
         } // else of if(NULL == fScalerBoardCollection) 
      
      fuTotalTriggerCount = 0;
      fuFirstMbsTime = 0;
      fuLastMbsTime  = 0;
      fuFirstScaler = new UInt_t*[triglog::kuNbScalers];
      fuLastScaler  = new UInt_t*[triglog::kuNbScalers];
      for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
      {
         fuFirstScaler[uScaler] = new UInt_t[triglog::kuNbChan];
         fuLastScaler[uScaler]  = new UInt_t[triglog::kuNbChan];
      } // for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
         LOG(error)<<"TTofTriglogUnpacker::TTofTriglogUnpacker => More than 1 TRIGLOG board active!!!!!"
                   <<" recheck your VME address matrix ";
}
TTofTriglogUnpacker::~TTofTriglogUnpacker()
{   
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   {
      for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
      {
         delete[] fuFirstScaler[uScaler];
         delete[] fuLastScaler[uScaler];
      } // for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
      delete[] fuFirstScaler;
      delete[] fuLastScaler;
   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
         LOG(error)<<"TTofTriglogUnpacker::~TTofTriglogUnpacker => More than 1 TRIGLOG board active!!!!!"
                   <<" recheck your VME address matrix ";
  
//   DeleteHistos();
}

void TTofTriglogUnpacker::Clear(Option_t */*option*/)
{
   fParUnpack = NULL;
   fTriglogBoardCollection = NULL;
   fScalerBoardCollection = NULL;
}

void TTofTriglogUnpacker::ProcessTriglog( UInt_t* pMbsData, UInt_t uLength )
{
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   {
      // Getting output objects
      TTofTriglogBoard * fTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->ConstructedAt(0); // Always only 1 TRIGLOG board!
      TTofScalerBoard  * fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->ConstructedAt(0);  // TRIGLOG always first scaler board!
      
      if( tofscaler::undef == fScalerBoard->GetScalerType() )
         fScalerBoard->SetType( tofscaler::triglog );
         
      UInt_t uIndx = 0;
      Int_t sync_num = pMbsData[uIndx];
      fTriglogBoard->SetSyncNb( sync_num );
      uIndx++;

      LOG(debug2)<<" ===================== TRIGLOG EVENT ================= " << sync_num % 0x1000000 ;

      fTriglogBoard->SetTriggPatt( (UInt_t) pMbsData[uIndx] );
      uIndx++;

      fTriglogBoard->SetMbsTimeSec( (UInt_t) pMbsData[uIndx] );
      uIndx++;
      fTriglogBoard->SetMbsTimeMilliSec( (UInt_t) pMbsData[uIndx] );
      uIndx++;

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
         
      if( uIndx < uLength)
      {
         // Input pattern is inside Tof MBS event only since August 2012
         fTriglogBoard->SetInpPatt( pMbsData[uIndx] );
         uIndx++;
      } // if( uIndx < uLength)
      if( uIndx < uLength)
      {
         // Reference clock is inside Tof MBS event only since October 2012
         fTriglogBoard->SetRefClk( pMbsData[uIndx] );
         fScalerBoard->SetRefClk(  pMbsData[uIndx] );
         uIndx++;
      } // if( uIndx < uLength)
      LOG(debug2)<<" TRIGLOG UNPACK finished";

      fTriglogBoard->SetPresentFlag( kTRUE );
      fScalerBoard->SetPresentFlag( kTRUE );
      
      UpdateStats();
   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
         LOG(error)<<"TTofTriglogUnpacker::ProcessTriglog => More than 1 TRIGLOG board active!!!!!"
                   <<" recheck your VME address matrix ";

   return;
} 
void TTofTriglogUnpacker::UpdateStats()
{
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   {
      // Getting output objects
      TTofTriglogBoard * fTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->At(0); // Always only 1 TRIGLOG board!
      TTofScalerBoard  * fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->At(0);  // TRIGLOG always first scaler board!

      if( NULL == fTriglogBoard || NULL == fScalerBoard )
      {
         LOG(error)<<"TTofTriglogUnpacker::UpdateStats => Board objects not existing!!!!";
         LOG(error)<<"TTofTriglogUnpacker::UpdateStats => TTofTriglogBoard = "<<fTriglogBoard
                   <<" TTofTriglogBoard = "<<fScalerBoard;
         LOG(error)<<"TTofTriglogUnpacker::UpdateStats => Probably the TRIGLOG sub-event is not there in this event!";
         return;
      } // if( NULL == fTriglogBoard || NULL == fScalerBoard )

      // In GSI April 2014 beamtime some event are used to empty TRB buffers
      // and therefore do not contain the MBS event
      if( kFALSE == fTriglogBoard->IsUpdated() && kFALSE == fScalerBoard->IsUpdated() )
      {
         LOG(error)<<"TTofTriglogUnpacker::UpdateStats => No TRIGLOG sub-event in this event,"
                   <<" OK for GSI April 2014 beamtime";
         return;
      } // if( kFALSE == fTriglogBoard->IsUpdated() && kFALSE == fScalerBoard->IsUpdated() )
      
      // Preparing output at the end:
         // MBS time is time in s since LINUX epoch (circa 1970) => never 0 except on first event!
      fuTotalTriggerCount++;
      for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
         for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++) 
         {
            fuLastScaler[uScaler][uCh] = fScalerBoard->GetScalerValue( uCh, uScaler);
            if( fuFirstMbsTime == 0 ) fuFirstScaler[uScaler][uCh] = fuLastScaler[uScaler][uCh];
         } // for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++) 
      fuLastMbsTime     = fTriglogBoard->GetMbsTimeSec();
      if( 0 == fuFirstMbsTime ) 
         fuFirstMbsTime = fuLastMbsTime;
   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
         LOG(error)<<"TTofTriglogUnpacker::ProcessTriglog => More than 1 TRIGLOG board active!!!!!"
                   <<" recheck your VME address matrix ";
}

void TTofTriglogUnpacker::FinishTriglog( )
{
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   {
      Double_t dRuntime = fuLastMbsTime - fuFirstMbsTime;
      LOG(info)<<Form("Total MBS run time %6.0f sec.", dRuntime);

      if( 0 < dRuntime ) 
      {
         LOG(info)<<Form("-------- Total number of triggers %d --------", fuTotalTriggerCount);

         TString sLine = "";
         for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++) 
         {
            sLine = Form("  Ch %2d", uCh );
            for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
               sLine += Form("   %9u", fuLastScaler[uScaler][uCh] - fuFirstScaler[uScaler][uCh]);
            LOG(info)<<sLine;
         } // for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++)

         LOG(info)<<Form("------- Trigger rate %8.3f Hz ---------", fuTotalTriggerCount/dRuntime);

         for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++) 
         {
            sLine = Form("  Ch %2d", uCh );
            for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
               sLine += Form("   %9.2f", (fuLastScaler[uScaler][uCh] - fuFirstScaler[uScaler][uCh])/dRuntime);
            LOG(info)<<sLine;
         } // for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++) 
      } // if( 0 < dRuntime ) 
   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
         LOG(error)<<"TTofTriglogUnpacker::FinishTriglog => More than 1 TRIGLOG board active!!!!!"
                   <<" recheck your VME address matrix ";
}

// Histogram management
// TODO: maybe add some options to control which histograms are 
// created and filed (memory management)
void TTofTriglogUnpacker::CreateHistos()
{
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   {
      TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
      gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !
      
      fhTriglogSyncs  = new TH1I( Form("tof_%s_syncs", tofscaler::ksTdcHistName[ tofscaler::triglog ].Data() ),
                           "Number of sync messages from VULOM", 65536, 0, 65536.);
      fhSpillRate     = new TH1I( Form("tof_%s_trig_rate", tofscaler::ksTdcHistName[ tofscaler::triglog ].Data() ),
                           "Number of triggers per second; Time [s]", 3600, 0.0, 3600 );
      fhRefClkRate    = new TH1I( Form("tof_%s_refclk", tofscaler::ksTdcHistName[ tofscaler::triglog ].Data() ),
                           "Rate of the reference clock; Freq. [Hz]", 
                           (Int_t)(2*triglog::kdRefClkFreq/1000), 0.0 , 2*triglog::kdRefClkFreq );
      fhRefClkRateEvo = new TH1I( Form("tof_%s_refclk_evo", tofscaler::ksTdcHistName[ tofscaler::triglog ].Data() ),
                           "Reference clock counts per second; Time [s]; Counts []", 3600, 0.0, 3600 );
      
      fTriggerPattern = new TH1I( Form("tof_%s_trig_patt", tofscaler::ksTdcHistName[ tofscaler::triglog ].Data() ),
                           "Trigger Pattern ; Trigger []; Counts []", 16, -0.5, 15.5 );
      fInputPattern   = new TH1I( Form("tof_%s_inp_patt", tofscaler::ksTdcHistName[ tofscaler::triglog ].Data() ),
                           "Input Pattern ; Input []; Counts []", 16, -0.5, 15.5 );
          
      fhScalers.resize(triglog::kuNbScalers);
      for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
         fhScalers[uScaler] = new TH1I( Form("tof_%s_scalers%02d", tofscaler::ksTdcHistName[ tofscaler::triglog ].Data(), uScaler ),
                                        Form("Counts per scaler channel in scaler %02d; Channel []; Total counts []", uScaler),
                                        triglog::kuNbChan, 0, triglog::kuNbChan);
                                
      gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
      
      // Initialize variable used in filling
      fuLastSync     = 0;
      fdFirstMbsTime = 0;
      fdPrevMbsTime  = -1;
      fuLastRefClk   = 0;
   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
         LOG(error)<<"TTofTriglogUnpacker::CreateHistos => More than 1 TRIGLOG board active!!!!!"
                   <<" recheck your VME address matrix ";
}
void TTofTriglogUnpacker::FillHistos()
{
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   {
      // Getting output objects
      TTofTriglogBoard * fTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->At(0); // Always only 1 TRIGLOG board!
      TTofScalerBoard  * fScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->At(0);  // TRIGLOG always first scaler board!

      if( NULL == fTriglogBoard || NULL == fScalerBoard )
      {
         LOG(error)<<"TTofTriglogUnpacker::FillHistos => Board objects not existing!!!!";
         LOG(error)<<"TTofTriglogUnpacker::FillHistos => TTofTriglogBoard = "<<fTriglogBoard
                   <<" TTofScalerBoard = "<<fScalerBoard;
         LOG(error)<<"TTofTriglogUnpacker::FillHistos => Probably the TRIGLOG sub-event is not there in this event!";
         return;
      } // if( NULL == fTriglogBoard || NULL == fScalerBoard )

      // In GSI April 2014 beamtime some event are used to empty TRB buffers
      // and therefore do not contain the MBS event
      if( kFALSE == fTriglogBoard->IsUpdated() && kFALSE == fScalerBoard->IsUpdated() )
      {
         LOG(error)<<"TTofTriglogUnpacker::FillHistos => No TRIGLOG sub-event in this event,"
                   <<" OK for GSI April 2014 beamtime";
         return;
      } // if( kFALSE == fTriglogBoard->IsUpdated() && kFALSE == fScalerBoard->IsUpdated() )

      LOG(debug3)<<(fTriglogBoardCollection->GetEntries())<<" "<<(fScalerBoardCollection->GetEntries());

      Double_t dMbsTime = fTriglogBoard->GetMbsTimeSec() + fTriglogBoard->GetMbsTimeMilliSec()*1e-3;
      if( 0 == fdFirstMbsTime )
         // MBS time is time in s since LINUX epoch (circa 1970) => never 0 except on first event!
         fdFirstMbsTime = dMbsTime;

      // just to see that sync number is changing
      UInt_t uSyncValue = fTriglogBoard->GetSyncNb();
      fhTriglogSyncs->Fill(uSyncValue % 65536);
      if( (fuLastSync!=0) && (uSyncValue > fuLastSync) )
         fhSpillRate->Fill( fuLastMbsTime - fuFirstMbsTime, uSyncValue - fuLastSync);
      fuLastSync = uSyncValue;
      
      for( UInt_t uBit=0; uBit < 16; uBit++)
      {
         if( ( (UInt_t) fTriglogBoard->GetTriggPatt() >>uBit )&0x00000001 )
         {
            fTriggerPattern->Fill(uBit);
         } // if( ( (UInt_t) fTriglogBoard->GetTriggPatt() >>uBit )&0x00000001 )
         if( ( (UInt_t) fTriglogBoard->GetInpPatt() >>uBit )&0x00000001 )
         {
            fInputPattern->Fill(uBit);
         } // if( ( (UInt_t) fTriglogBoard->GetTriggPatt() >>uBit )&0x00000001 )
      } // for( UInt_t uBit=0; uBit < 16; uBit++)

      if( ( 0 == fdPrevMbsTime ) || ( fdPrevMbsTime + 0.05 < dMbsTime ) ) 
      {
         Double_t dTimeDiff = dMbsTime - fdPrevMbsTime;
         if( 0 < fdPrevMbsTime && dTimeDiff < 0.3 ) 
         {
            fhRefClkRate->Fill( (Double_t)(fScalerBoard->GetRefClk() - fuLastRefClk ) / 
                                dTimeDiff );
            fhRefClkRateEvo->Fill(dMbsTime - fdFirstMbsTime, fScalerBoard->GetRefClk() - fuLastRefClk);
         } // if( 0 < fdPrevMbsTime ) 

         fdPrevMbsTime = dMbsTime;
         fuLastRefClk = fScalerBoard->GetRefClk();
      } // if( ( 0 == fdPrevMbsTime ) || ( dMbsTime > fdPrevMbsTime + 0.1 ) ) 

      for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
         for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++) 
            fhScalers[uScaler]->SetBinContent( 1 + uCh, fScalerBoard->GetScalerValue( uCh, uScaler) 
                                                      - fuFirstScaler[uScaler][uCh] );

   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
         LOG(error)<<"TTofTriglogUnpacker::FillHistos => More than 1 TRIGLOG board active!!!!!"
                   <<" recheck your VME address matrix ";

}
void TTofTriglogUnpacker::WriteHistos( TDirectory* inDir)
{
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   {
      TDirectory * oldir = gDirectory;
      
      TDirectory *cdTriglog = inDir->mkdir( Form( "Unp_%s", tofscaler::ksTdcHistName[ tofscaler::triglog ].Data() ) );
      cdTriglog->cd();    // make the "Unp_triglog" directory the current directory
      
      fhTriglogSyncs->Write();
      fhSpillRate->Write();
      fhRefClkRate->Write();
      fhRefClkRateEvo->Write();
      fTriggerPattern->Write();
      fInputPattern->Write();
      for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
         fhScalers[uScaler]->Write();
      
      gDirectory->cd( oldir->GetPath() );
   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
         LOG(error)<<"TTofTriglogUnpacker::WriteHistos => More than 1 TRIGLOG board active!!!!!"
                   <<" recheck your VME address matrix ";
}
void TTofTriglogUnpacker::DeleteHistos()
{
/*
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   {      
   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
         LOG(error)<<"TTofTriglogUnpacker::DeleteHistos => More than 1 TRIGLOG board active!!!!!"
                   <<" recheck your VME address matrix ";
*/
} 
