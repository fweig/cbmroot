// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmGet4EventBuilder                          -----
// -----                    Created 23.02.2015 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmGet4EventBuilder.h"

// TOF Classes and includes
#include "CbmGet4EpochBuffer.h"
#include "CbmGet4EventBuffer.h"
#include "CbmGet4v1xDef.h"

// CBMroot classes and includes

// FAIR classes and includes
#include "FairLogger.h"

// ROOT Classes and includes
#include "TH1.h"
#include "TROOT.h"
#include <TFile.h>
#ifdef MUL_TRIGG_DEBUG
   #include "TH2.h"
#endif // MUL_TRIGG_DEBUG

// C++ std Classes and includes
#ifdef MUL_TRIGG_DEBUG
   #include <iostream>
   #include <limits>
#endif // MUL_TRIGG_DEBUG

/***************** Hit multiplicity trigger ******************/
const Int_t kiBinSizeClkCycle = 10; // Bin size for Multi histo, in clocks
/*************************************************************/

/*************** Channel coincidence trigger *****************/
// Constants for channels coincidence trigger
//const Int_t kiMaxNbTriggers   = 16;
//const Int_t kiMaxNbChPerTrig  =  4;
/*************************************************************/

//___________________________________________________________________
//

// ------------------------------------------------------------------
CbmGet4EventBuilder::CbmGet4EventBuilder()
  : FairTask("CbmGet4EventBuilder"),
    fuInputEvents(0),
    fuOutputEvents(0),
    fuInputEpochs(0),
    fInputBuffer(NULL),
    fOutputBuffer(NULL),
    fpEpoch{NULL, NULL, NULL},
    fiCurrentEpoch(-1),
    fdMaxTimePerLoopS(0.0),
    fStart(),
    fStop(),
    fsHistoFilename("./get4EventBuilder.hst.root"),
    fbTriggerMode(kFALSE),
    fiNbTriggers(0),
    fvdTrigWinNeg(),
    fvdTrigWinPos(),
    fvdTrigDeadtime(),
    fviTrigType(),
    fiEpochStartHitScan(-1),
    fiIndexStartHitScan(-1),
    fhHitMultiplicity(NULL),
    fiMultiHistBinSz(kiBinSizeClkCycle),
    fiNbBinsMultiHist(0),
    fiMultiThreshold(-1),
    fhMaxMulPerEpochDist(NULL),
    fhEventSizeDist(NULL),
#ifdef MUL_TRIGG_DEBUG
    canvasDeb(NULL),
    fhMaxMulEpoch(NULL),
    fhBinThrEpoch(NULL),
    fhMaxEvtSizeEpoch(NULL),
    fhEvtSizeEvo(NULL),
#endif // MUL_TRIGG_DEBUG
    fvvuTriggerComp(),
    fvdCoincWin(),
    fuLastTriggerId(0),
    fvvbTrigCompFound(),
    fvvulTrigCompEp(),
    fvvuTrigCompTs()
{
  LOG(info)<<" CbmGet4EventBuilder: Task started";
}
CbmGet4EventBuilder::CbmGet4EventBuilder(const char* name, Int_t verbose)
  : FairTask(name, verbose),
    fuInputEvents(0),
    fuOutputEvents(0),
    fuInputEpochs(0),
    fInputBuffer(NULL),
    fOutputBuffer(NULL),
    fpEpoch{NULL, NULL, NULL},
    fiCurrentEpoch(-1),
    fdMaxTimePerLoopS(0.0),
    fStart(),
    fStop(),
    fsHistoFilename("./get4EventBuilder.hst.root"),
    fbTriggerMode(kFALSE),
    fiNbTriggers(0),
    fvdTrigWinNeg(),
    fvdTrigWinPos(),
    fvdTrigDeadtime(),
    fviTrigType(),
    fiEpochStartHitScan(-1),
    fiIndexStartHitScan(-1),
    fhHitMultiplicity(NULL),
    fiMultiHistBinSz(kiBinSizeClkCycle),
    fiNbBinsMultiHist(0),
    fiMultiThreshold(-1),
    fhMaxMulPerEpochDist(NULL),
    fhEventSizeDist(NULL),
#ifdef MUL_TRIGG_DEBUG
    canvasDeb(NULL),
    fhMaxMulEpoch(NULL),
    fhBinThrEpoch(NULL),
    fhMaxEvtSizeEpoch(NULL),
    fhEvtSizeEvo(NULL),
#endif // MUL_TRIGG_DEBUG
    fvvuTriggerComp(),
    fvdCoincWin(),
    fuLastTriggerId(0),
    fvvbTrigCompFound(),
    fvvulTrigCompEp(),
    fvvuTrigCompTs()
{
   LOG(info)<<" CbmGet4EventBuilder: Task started";
}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
CbmGet4EventBuilder::~CbmGet4EventBuilder()
{
    // Destructor
#ifdef MUL_TRIGG_DEBUG
   delete canvasDeb;
#endif // MUL_TRIGG_DEBUG
}
// ------------------------------------------------------------------
/************************************************************************************/
void CbmGet4EventBuilder::SetTriggerMode( Bool_t bTrigModeIn )
{
   fbTriggerMode = bTrigModeIn;

   if( kFALSE == fbTriggerMode )
   {
      fiNbTriggers = 1;
      ResizeTriggerVectors();
   } // if( kFALSE == fbTriggerMode )
   return;
}
void CbmGet4EventBuilder::SetTriggerWinNeg(  Double_t dWinNeg, Int_t iTrigger )
{
   if( 0 <= iTrigger && iTrigger < fiNbTriggers )
      fvdTrigWinNeg[iTrigger] = dWinNeg;
   else LOG(error)<<" CbmGet4EventBuilder::SetTriggerWinNeg => Trying to set the Neg trigger Win "
                  <<"for a non existing trigger, therefore ignored\n"
                  <<"                                          Nb Triggers: "<<fiNbTriggers
                  <<" Attempted trigger: "<<iTrigger;
}
void CbmGet4EventBuilder::SetTriggerWinPos(  Double_t dWinPos, Int_t iTrigger )
{
   if( 0 <= iTrigger && iTrigger < fiNbTriggers )
      fvdTrigWinPos[iTrigger] = dWinPos;
   else LOG(error)<<" CbmGet4EventBuilder::SetTriggerWinPos => Trying to set the Pos trigger Win "
                  <<"for a non existing trigger, therefore ignored\n"
                  <<"                                          Nb Triggers: "<<fiNbTriggers
                  <<" Attempted trigger: "<<iTrigger;
}
void CbmGet4EventBuilder::SetTriggerDeadT(   Double_t dDeadT,  Int_t iTrigger )
{
   if( 0 <= iTrigger && iTrigger < fiNbTriggers )
      fvdTrigDeadtime[iTrigger] = dDeadT;
   else LOG(error)<<" CbmGet4EventBuilder::SetTriggerDeadT => Trying to set the trigger deadtime "
                  <<"for a non existing trigger, therefore ignored\n"
                  <<"                                          Nb Triggers: "<<fiNbTriggers
                  <<" Attempted trigger: "<<iTrigger;
}
void CbmGet4EventBuilder::SetTriggerType(    Int_t    iType,   Int_t iTrigger )
{
   if( 0 <= iTrigger && iTrigger < fiNbTriggers )
      fviTrigType[iTrigger] = iType;
   else LOG(error)<<" CbmGet4EventBuilder::SetTriggerType => Trying to set the trigger type "
                  <<"for a non existing trigger, therefore ignored\n"
                  <<"                                          Nb Triggers: "<<fiNbTriggers
                  <<" Attempted trigger: "<<iTrigger;
}
void CbmGet4EventBuilder::SetNbTriggers( Int_t iNbTriggers )
{

   if( kFALSE == fbTriggerMode )
   {
      LOG(error)<<" CbmGet4EventBuilder::SetNbTriggers => Trying to set triggers number "
                <<"in multiplicity trigger mode, therefore ignored";
      return;
   }// if( kFALSE == fbTriggerMode )

   fiNbTriggers = iNbTriggers;
   ResizeTriggerVectors();
}
void CbmGet4EventBuilder::SetHistoFilename( TString sNameIn )
{
   fsHistoFilename = sNameIn;

   LOG(info)<<" CbmGet4EventBuilder::SetHistoFilename => Histograms output file is now\n"
             <<fsHistoFilename;
}
void CbmGet4EventBuilder::ResizeTriggerVectors()
{
   fvdTrigWinNeg.resize(fiNbTriggers);
   fvdTrigWinPos.resize(fiNbTriggers);
   fvdTrigDeadtime.resize(fiNbTriggers);
   fviTrigType.resize(fiNbTriggers);

   if( kTRUE == fbTriggerMode )
   {
      fvvuTriggerComp.resize(fiNbTriggers);
      fvdCoincWin.resize(fiNbTriggers);
      fvvbTrigCompFound.resize(fiNbTriggers);
      fvvulTrigCompEp.resize(fiNbTriggers);
      fvvuTrigCompTs.resize(fiNbTriggers);
   } // if( kTRUE == fbTriggerMode )

   return;
}
Int_t CbmGet4EventBuilder::PrevEpId()
{
   if( 0 < fiCurrentEpoch)
      return (fiCurrentEpoch - 1);
      else return (kiMaxEpochInUse - 1);
}
Int_t CbmGet4EventBuilder::CurrEpId()
{
   return fiCurrentEpoch;
}
Int_t CbmGet4EventBuilder::NextEpId()
{
   /*
   if( fiCurrentEpoch + 1 < kiMaxEpochInUse)
      return fiCurrentEpoch + 1;
      else return fiCurrentEpoch + 1 - kiMaxEpochInUse;
      */
   return (fiCurrentEpoch + 1)%kiMaxEpochInUse;
}
/************************************************************************************/
// FairTasks inherited functions
InitStatus CbmGet4EventBuilder::Init()
{
   if( kFALSE == RegisterInputs() )
      return kFATAL;

   if( kFALSE == RegisterOutputs() )
      return kFATAL;

   if( kFALSE == CreateHistos() )
      return kFATAL;

   // Common variables initial values, mostly in case some are missed in ctor
   fuInputEvents   =  0;
   fiCurrentEpoch  = 0;
   for( Int_t iEp  = 0; iEp < kiMaxEpochInUse; iEp++)
      fpEpoch[iEp] = NULL;
   fiEpochStartHitScan = -1;
   fiIndexStartHitScan =  0;

   if( kFALSE == fbTriggerMode )
   {
      fiNbBinsMultiHist = get4v1x::kuCoarseCounterSize/fiMultiHistBinSz;
      fhHitMultiplicity = new TH1I( "hHitMultiplicity",
                                    "Hit multiplicity evolution within current epoch; Clock cycles [6.4 ns]; counts",
                                    fiNbBinsMultiHist, 0, get4v1x::kuCoarseCounterSize);
      fhMaxMulPerEpochDist = new TH1I( "hMaxMulPerEpochDist",
            "Max multiplicity per epoch; Max Mul [hits]",
            250, 0, 250);
      fhEventSizeDist = new TH1I( "hEventSizeDist",
            "nb Hits per event; event size [hits]",
            250, 0, 250);
#ifdef MUL_TRIGG_DEBUG
      canvasDeb = new TCanvas("canvasDeb", "Multiplicity trigger debug", 1200, 600);
      canvasDeb->Divide(2);
      fhMaxMulEpoch = new TH2I( "hMaxMulEpoch",
            "Max multiplicity per epoch; Epoch Index mod. 1000; Epoch Index/1000; Max Mul [hits]",
            1000, 0, 1000,
            2000, 0, 2000);
      fhBinThrEpoch = new TH2I( "hBinThrEpoch",
            "Max multiplicity per epoch; Epoch Index mod. 1000; Epoch Index/1000; Bin gt thr",
            1000, 0, 1000,
            2000, 0, 2000);
      fhMaxEvtSizeEpoch = new TH2I( "hMaxEvtSizeEpoch",
            "Max event size per epoch; Epoch Index mod. 1000; Epoch Index/1000; Max event size [bin]",
            1000, 0, 1000,
            2000, 0, 2000);
      fhEvtSizeEvo = new TH2I( "hEvtSizeEvo",
            "Event size distribution VS epoch index; Event size [hits]; Epoch Index/1000; Counts []",
             500, 0,  500,
            2000, 0, 2000);
#endif // MUL_TRIGG_DEBUG
   } // if( kFALSE == fbTriggerMode )
      else
      {

      } // else of if( kFALSE == fbTriggerMode )

   return kSUCCESS;
}

void CbmGet4EventBuilder::Exec(Option_t* /*option*/)
{
   // Task execution

   LOG(debug)<<" CbmGet4EventBuilder => New loop";

   // Initialize the looping time start point
   fStart.Set();

   // Prepare temp triggers variables
   std::vector< Int_t > vTriggerTimes;

   // Build events from as many available closed epochs as possible
   // TODO: Last epoch may not be processed, to be checked
   while( 0 < fInputBuffer->GetSize()  )
   {
      LOG(debug2)<<" CbmGet4EventBuilder => out buffer size: "<<fOutputBuffer->GetSize();
      LOG(debug2)<<" CbmGet4EventBuilder => PrevEpId: "<<PrevEpId()
                <<" Pointer "<< fpEpoch[ PrevEpId() ] ;
      LOG(debug2)<<" CbmGet4EventBuilder => CurrEpId: "<<CurrEpId()
                <<" Pointer "<< fpEpoch[ CurrEpId() ] ;
      LOG(debug2)<<" CbmGet4EventBuilder => NextEpId: "<<NextEpId()
                <<" Pointer "<< fpEpoch[ NextEpId() ] ;

      // Get next Closed Epoch
      std::multiset< get4v1x::FullMessage >* pvEpCont = fInputBuffer->GetNextEpoch();
      if( NULL == pvEpCont )
      {
         LOG(error)<<" CbmGet4EventBuilder => Tried to get one epoch too much!!!";
         break;
      } // if( NULL == pvEpCont )

      if( 0 == fuInputEpochs )
      {
         // first epoch, store it and wait for next epoch
         fpEpoch[ CurrEpId() ] = pvEpCont;
         fuInputEpochs++;
         continue;
      } // if( 0 == fuInputEpochs )
      else if( 1 == fuInputEpochs )
      {
         // second epoch, store it and process the first epoch
         fpEpoch[ NextEpId() ] = pvEpCont;
      } // if( 1 == fuInputEpochs )
      else
      {
         // Update Epoch index
         fiCurrentEpoch = NextEpId();
         // remove data of old epoch buffer to be overwritten
         if( NULL != fpEpoch[ NextEpId() ])
         {
            if( 0 < fpEpoch[ NextEpId() ]->size() )
               fpEpoch[ NextEpId() ]->clear();
            delete fpEpoch[ NextEpId() ];
         } // if( NULL != fpEpoch[ NextEpId() ])
         // Store pointer on epoch vector for next epoch
         fpEpoch[ NextEpId() ] = pvEpCont;
      } // if more than 2 epochs already received
      LOG(debug2)<<" CbmGet4EventBuilder => PrevEpId: "<<PrevEpId()
                <<" Pointer "<< fpEpoch[ PrevEpId() ] ;
      LOG(debug2)<<" CbmGet4EventBuilder => CurrEpId: "<<CurrEpId()
                <<" Pointer "<< fpEpoch[ CurrEpId() ] ;
      LOG(debug2)<<" CbmGet4EventBuilder => NextEpId: "<<NextEpId()
                <<" Pointer "<< fpEpoch[ NextEpId() ] ;
      fuInputEpochs++;

      LOG(debug)<<" CbmGet4EventBuilder => Find where to start event building";
      // Update index of epoch in which one should start scanning, counting deadtime
      if( -1 < fiEpochStartHitScan )
         fiEpochStartHitScan --;
         else
         {
            // Case where many epochs without events => not up to date
            fiEpochStartHitScan = -1;
            fiIndexStartHitScan =  0;
         } // else of if( -1 < fiEpochStartHitScan )

      // Just jump empty epochs: should not happen but still safer
      if( fpEpoch[ CurrEpId() ]->size() < 1 )
         continue;

      // Check if Prev epoch is there
      Bool_t bPrevEpThere = kTRUE;
      if( NULL == fpEpoch[ PrevEpId() ] )
         bPrevEpThere = kFALSE;
      else if( fpEpoch[ PrevEpId() ]->size() < 1 )
         // Just jump empty epochs: should not happen but still safer
         bPrevEpThere = kFALSE;
      else if( (fpEpoch[ CurrEpId() ]->begin() )->getExtendedEpoch() - 1 !=
            (fpEpoch[ PrevEpId() ]->begin() )->getExtendedEpoch() )
         bPrevEpThere = kFALSE;

      // Check if next epoch is there
      LOG(debug2)<<" CbmGet4EventBuilder pointer => "
                <<fpEpoch[ CurrEpId() ] <<"\n"
                <<fpEpoch[ NextEpId() ] ;
      LOG(debug2)<<" CbmGet4EventBuilder size => "
                <<(fpEpoch[ CurrEpId() ]->size() ) <<"\n"
                <<(fpEpoch[ NextEpId() ]->size() ) ;
      LOG(debug2)<<" CbmGet4EventBuilder epoch => "
                <<(fpEpoch[ CurrEpId() ]->begin() )->getExtendedEpoch() <<"\n"
                <<(fpEpoch[ NextEpId() ]->begin() )->getExtendedEpoch() ;
      Bool_t bNextEpThere = kTRUE;
      if( NULL == fpEpoch[ NextEpId() ] )
         bNextEpThere = kFALSE;
      else if( fpEpoch[ NextEpId() ]->size() < 1 )
         // Just jump empty epochs: should not happen but still safer
         bNextEpThere = kFALSE;
      else if( (fpEpoch[ CurrEpId() ]->begin() )->getExtendedEpoch() + 1 !=
            (fpEpoch[ NextEpId() ]->begin() )->getExtendedEpoch() )
         bNextEpThere = kFALSE;

      if( -1 == fiEpochStartHitScan && kFALSE == bPrevEpThere )
      {
         fiEpochStartHitScan = 0;
         fiIndexStartHitScan =  0;
      } // if( -1 == fiEpochStartHitScan && kFALSE == bPrevEpThere )

      // Look for trigger
      if( kFALSE == fbTriggerMode )
      {
         LOG(debug)<<" CbmGet4EventBuilder => Start event finding";
         // First scan Hits to fill the multiplicity histogram
         for( auto itHit = fpEpoch[ CurrEpId() ]->begin();
                   itHit != fpEpoch[ CurrEpId() ]->end(); ++itHit)
         {
//            get4v1x::FullMessage mess = *itData; // Maybe not smart to copy data => ref/pointer?
            const get4v1x::FullMessage& mess = *itHit; // Attempt with reference
            if( mess.isGet4Msg() )
               fhHitMultiplicity->Fill( mess.getGet4CoarseTs() );
            else if( mess.isGet4Hack32Msg() )
               // Assume only hit messages are filled in epoch buffer in 32b mode
               // If crazy values, maybe add here a test on 32b message type
               fhHitMultiplicity->Fill( mess.getGet4V10R32HitTs() );
            else
               LOG(error)<<" CbmGet4EventBuilder => Non GET4 hit message found in epoch buffer!!! Type: "
                         << mess.getMessageType()
                         ;
         } // for( auto itHit = fpEpoch[ CurrEpId() ]->begin(); itHit != fpEpoch[ CurrEpId() ]->end(); ++itHit)

         // Find bins above multiplicity threshold
         for( Int_t iBin = 0; iBin < fiNbBinsMultiHist; iBin++)
            if( fiMultiThreshold <= fhHitMultiplicity->GetBinContent( iBin + 1) )
            {
               // => New trigger candidate
               // First check if already some triggers in current epoch and if "yes",
               // check if we are outside of deadtime
               if( 0 < vTriggerTimes.size() )
                  if( iBin*fiMultiHistBinSz - vTriggerTimes.back() < fvdTrigDeadtime[0] )
                     // If within deadtime, ignore this candidate and go on with scanning
                     continue;

               // If out of deadtime => new trigger! => Save approx. middle of bin as trigger time!
               vTriggerTimes.push_back( iBin*fiMultiHistBinSz + fiMultiHistBinSz/2 );
            } // if( fiMultiThreshold <= fhHitMultiplicity->GetBinContent( iBin + 1) )

         ULong64_t ulEpochIdx = (fpEpoch[ CurrEpId() ]->begin() )->getExtendedEpoch();

         // Now rescan the hits to see which ones correspond to events
         UInt_t uMaxEventSzEp = 0;
            // For each possible trigger, check from last possible hit to hits
            // in next epoch
         LOG(debug)<<" CbmGet4EventBuilder => Start event building for "
                   << vTriggerTimes.size() <<" event triggers";
         for( UInt_t uTrigg = 0; uTrigg < vTriggerTimes.size(); uTrigg ++)
         {
            LOG(debug)<<"~~~~~~~~~~~~~ CbmGet4EventBuilder => Scan hits for trigger nb "
                      <<uTrigg<<" over "<<vTriggerTimes.size();
            // create a new vector for this event
            std::vector< get4v1x::FullMessage >* vEvent = new std::vector< get4v1x::FullMessage >;

            // Event borders
            Int_t iTriggLoBorder  = vTriggerTimes[uTrigg] + fvdTrigWinNeg[0];
            Int_t iTriggHiBorder  = vTriggerTimes[uTrigg] + fvdTrigWinPos[0];
            Int_t iFirstHitBorder = vTriggerTimes[uTrigg] + fvdTrigDeadtime[0] + fvdTrigWinNeg[0];
            Bool_t bEventOver = kFALSE;

            LOG(debug)<<"~~~~~~~~~~~~~ Start epoch "<<fiEpochStartHitScan
                      <<" Start index "<<fiIndexStartHitScan;
            LOG(debug)<<"~~~~~~~~~~~~~ iTriggLoBorder "<<iTriggLoBorder<<" iTriggHiBorder "<<iTriggHiBorder
                      <<" vTriggerTimes "<<vTriggerTimes[uTrigg]
                      ;

            if( -1 == fiEpochStartHitScan && kTRUE == bPrevEpThere )
            {
               LOG(debug)<<" CbmGet4EventBuilder => Scan hits in previous epoch";
               Int_t iHitIndex = 0;
//               for( auto itHit = fpEpoch[ PrevEpId() ]->begin() + fiIndexStartHitScan; // NOT WORKING
               for( auto itHit = fpEpoch[ PrevEpId() ]->begin(); // NOT OPTIMAL
                     itHit != fpEpoch[ PrevEpId() ]->end(); ++itHit)
               {
                  // NOT OPTIMAL (PAL): best would be to use an iterator for the start index
                  // itself, but not sure how to do this safely
                  if( iHitIndex < fiIndexStartHitScan)
                  {
                     iHitIndex++;
                     continue;
                  } // if( iHitIndex < fiIndexStartHitScan)

                  const get4v1x::FullMessage& mess = *itHit;

                  // Offset for previous epoch: -1 full epoch
                  Int_t iHitCoarseTime = -get4v1x::kuCoarseCounterSize;
                  if( mess.isGet4Msg() )
                     iHitCoarseTime += mess.getGet4CoarseTs();
                  else if( mess.isGet4Hack32Msg() )
                     // Assume only hit messages are filled in epoch buffer in 32b mode
                     // If crazy values, maybe add here a test on 32b message type
                     iHitCoarseTime += mess.getGet4V10R32HitTs();
                  else
                  {
                     LOG(error)<<" CbmGet4EventBuilder => Non GET4 hit message found in epoch buffer!!! Type: "
                               << mess.getMessageType()
                               ;
                     // Update Hit index
                     iHitIndex++;
                     continue;
                  } // Bad message type => Jump

                  // Find the first possible hit for next event (or as close as possible)
                  if( iHitCoarseTime <= iFirstHitBorder )
                  {
                     if( iHitIndex + 1 == static_cast<Int_t>(fpEpoch[ PrevEpId() ]->size()) )
                     {
                        // End of epoch => first possible is first hit in current Ep
                        fiEpochStartHitScan = 0;
                        fiIndexStartHitScan = 0;
                     } // if( itHit + 1 == fpEpoch[ PrevEpId() ]->end() )
                     else fiIndexStartHitScan = iHitIndex + 1;
                  } // if( iHitCoarseTime <= iFirstHitBorder )

                  if( iTriggHiBorder < iHitCoarseTime )
                  {
                     // Passed the end of event => Stop scanning and go to next event
                     bEventOver = kTRUE;
                     break;
                  } // if( iTriggHiBorder < iHitCoarseTime )

                  if( iTriggLoBorder < iHitCoarseTime )
                     // Hit within event borders
                     vEvent->push_back(*itHit);

                  // Update Hit index
                  iHitIndex++;
               } // for hits is previous epoch
               LOG(debug)<<" CbmGet4EventBuilder => Done scanning hits in prvious epoch";
            } // if( -1 == fiEpochStartHitScan && kTRUE == bPrevEpThere )
            if( 0 == fiEpochStartHitScan && kFALSE == bEventOver )
            {
               LOG(debug)<<" CbmGet4EventBuilder => Scan hits in current epoch";
               Int_t iHitIndex = 0;
//               for( auto itHit = fpEpoch[ CurrEpId() ]->begin() + fiIndexStartHitScan; // NOT WORKING
               for( auto itHit = fpEpoch[ CurrEpId() ]->begin(); // NOT OPTIMAL
                     itHit != fpEpoch[ CurrEpId() ]->end(); ++itHit)
               {
                  // NOT OPTIMAL (PAL): best would be to use an iterator for the start index
                  // itself, but not sure how to do this safely
                  if( iHitIndex < fiIndexStartHitScan)
                  {
                     iHitIndex++;
                     continue;
                  } // if( iHitIndex < fiIndexStartHitScan)

                  const get4v1x::FullMessage& mess = *itHit;

                  // Offset for current epoch: 0
                  Int_t iHitCoarseTime = 0;
                  if( mess.isGet4Msg() )
                     iHitCoarseTime += mess.getGet4CoarseTs();
                  else if( mess.isGet4Hack32Msg() )
                     // Assume only hit messages are filled in epoch buffer in 32b mode
                     // If crazy values, maybe add here a test on 32b message type
                     iHitCoarseTime += mess.getGet4V10R32HitTs();
                  else
                  {
                     LOG(error)<<" CbmGet4EventBuilder => Non GET4 hit message found in epoch buffer!!! Type: "
                               << mess.getMessageType()
                               ;
                     // Update Hit index
                     iHitIndex++;
                     continue;
                  } // Bad message type => Jump

                  // Find the first possible hit for next event (or as close as possible)
                  if( iHitCoarseTime <= iFirstHitBorder )
                  {
                     if( iHitIndex + 1 == static_cast<Int_t>(fpEpoch[ CurrEpId() ]->size()) )
                     {
                        // End of epoch => first possible is first hit in next Ep
                        fiEpochStartHitScan = 1;
                        fiIndexStartHitScan = 0;
                     } // if( itHit + 1 == fpEpoch[ CurrEpId() ]->end() )
                     else fiIndexStartHitScan = iHitIndex + 1;
                  } // if( iHitCoarseTime <= iFirstHitBorder )

                  if( iTriggHiBorder < iHitCoarseTime )
                  {
                     // Passed the end of event => Stop scanning and go to next event
                     bEventOver = kTRUE;
                     break;
                  } // if( iTriggHiBorder < iHitCoarseTime )

                  if( iTriggLoBorder < iHitCoarseTime )
                     // Hit within event borders
                     vEvent->push_back(*itHit);

                  // Update Hit index
                  iHitIndex++;
               } // for hits is current epoch
               LOG(debug)<<" CbmGet4EventBuilder => Done scanning hits in current epoch";
            } // if( 0 == fiEpochStartHitScan )
            if( 1 == fiEpochStartHitScan && kTRUE == bNextEpThere && kFALSE == bEventOver )
            {
               LOG(debug)<<" CbmGet4EventBuilder => Scan hits in next epoch";
               Int_t iHitIndex = 0;
//               for( auto itHit = fpEpoch[ NextEpId() ]->begin() + fiIndexStartHitScan; // NOT WORKING
               for( auto itHit = fpEpoch[ NextEpId() ]->begin(); // NOT OPTIMAL
                     itHit != fpEpoch[ NextEpId() ]->end(); ++itHit)
               {
                  // NOT OPTIMAL (PAL): best would be to use an iterator for the start index
                  // itself, but not sure how to do this safely
                  if( iHitIndex < fiIndexStartHitScan)
                  {
                     iHitIndex++;
                     continue;
                  } // if( iHitIndex < fiIndexStartHitScan)

                  const get4v1x::FullMessage& mess = *itHit;

                  // Offset for next epoch: +1 full epoch
                  Int_t iHitCoarseTime = get4v1x::kuCoarseCounterSize;
                  if( mess.isGet4Msg() )
                     iHitCoarseTime += mess.getGet4CoarseTs();
                  else if( mess.isGet4Hack32Msg() )
                     // Assume only hit messages are filled in epoch buffer in 32b mode
                     // If crazy values, maybe add here a test on 32b message type
                     iHitCoarseTime += mess.getGet4V10R32HitTs();
                  else
                  {
                     LOG(error)<<" CbmGet4EventBuilder => Non GET4 hit message found in epoch buffer!!! Type: "
                               << mess.getMessageType()
                               ;
                     // Update Hit index
                     iHitIndex++;
                     continue;
                  } // Bad message type => Jump

                  // Find the first possible hit for next event (or as close as possible)
                  if( iHitCoarseTime <= iFirstHitBorder )
                  {
                     if( iHitIndex + 1 == static_cast<Int_t>(fpEpoch[ NextEpId() ]->size()) )
                     {
                        // End of epoch => first possible is first hit in over next Ep
                        // Case should be impossible but as crazy user can try to set
                        // a deadtime bigger than an epoch, check in case
                        fiEpochStartHitScan = 2;
                        fiIndexStartHitScan = 0;
                     } // if( itHit + 1 == fpEpoch[ NextEpId() ]->end() )
                     else fiIndexStartHitScan = iHitIndex + 1;
                  } // if( iHitCoarseTime <= iFirstHitBorder )

                  if( iTriggHiBorder < iHitCoarseTime )
                     // Passed the end of event => Stop scanning and go to next event
                     break;

                  if( iTriggLoBorder < iHitCoarseTime )
                     // Hit within event borders
                     vEvent->push_back(*itHit);

                  // Update Hit index
                  iHitIndex++;
               } // for hits is next epoch
               LOG(debug)<<" CbmGet4EventBuilder => Done scanning hits in next epoch";
            } // if( 1 == fiEpochStartHitScan )

            fhEventSizeDist->Fill( vEvent->size() );
            if( uMaxEventSzEp < vEvent->size() )
               uMaxEventSzEp = vEvent->size();
#ifdef MUL_TRIGG_DEBUG
            fhEvtSizeEvo->Fill( vEvent->size(), ulEpochIdx/1000 );
#endif // MUL_TRIGG_DEBUG

            if( 0 < vEvent->size() )
            {
               // Not empty => push to buffer
               // Trigger index for this simple multiplicity trigger: -1 or -Threshold?
               fOutputBuffer->InsertEvent( fuOutputEvents, -1, ulEpochIdx, vTriggerTimes[uTrigg], vEvent );
               fuOutputEvents++;

               if( 0 == ( fuOutputEvents%100000 ) && 0 < fuOutputEvents )
               {
                  LOG(info)<< "CbmGet4EventBuilder::Exec : "
                            << fuOutputEvents << " events found and processed.";
               } // if( 0 == ( fuOutputEvents%100 ) && 0 < fuOutputEvents )
            } // if( 0 < vEvent->size() )
               else
               {
                  vEvent->clear();
                  delete vEvent;
               } // else of if( 0 < vEvent->size() )
         } // for( UInt_t uTrigg = 0; uTrigg < vTriggerTimes.size(); uTrigg ++)

         // All event triggers scanned, now clear them
         vTriggerTimes.clear();

         fhMaxMulPerEpochDist->Fill( fhHitMultiplicity->GetMaximum() );
#ifdef MUL_TRIGG_DEBUG
         fhMaxMulEpoch->Fill( ulEpochIdx%1000, ulEpochIdx/1000,fhHitMultiplicity->GetMaximum() );
         for( Int_t iBin = 0; iBin < fiNbBinsMultiHist; iBin++)
            if( fiMultiThreshold <= fhHitMultiplicity->GetBinContent( iBin + 1) )
               fhBinThrEpoch->Fill( ulEpochIdx%1000, ulEpochIdx/1000);

         if( 0 < uMaxEventSzEp )
            fhMaxEvtSizeEpoch->Fill(ulEpochIdx%1000, ulEpochIdx/1000, uMaxEventSzEp );
//         delete canvasDeb;
#endif // MUL_TRIGG_DEBUG

         fhHitMultiplicity->Reset();
      } // if( kFALSE == fbTriggerMode )
         else
         {
         } // else of if( kFALSE == fbTriggerMode )

      // Fill histos if needed
      FillHistos();

      // if total looping time larger than limit: stop looping for now
      // We can start on the backlog later
      fStop.Set();
      Double_t dLooptTime = fStop.GetSec() - fStart.GetSec()
                          + (fStop.GetNanoSec() - fStart.GetNanoSec())/1e9;
      if( fdMaxTimePerLoopS < dLooptTime )
         break;
   } // while( 0 < fInputBuffer->GetSize()  )
#ifdef MUL_TRIGG_DEBUG
   fuInputEvents += 1;
   if( 0 == ( fuInputEvents%10000 ) && 0 < fuInputEvents )
   {
      canvasDeb->cd(1);
//      fhMaxMulEpoch->Draw("colz");
//      fhBinThrEpoch->Draw("colz");
      fhMaxEvtSizeEpoch->Draw("colz");
      gPad->Modified();
      gPad->Update();
      canvasDeb->cd(2);
//      fhMaxMulPerEpochDist->Draw("");
      fhEventSizeDist->Draw("");
      gPad->Modified();
      gPad->Update();
   }

   LOG(debug)<< "CbmGet4EventBuilder::Exec : "
         << fuInputEvents << " events processed and "
         << fuOutputEvents << " events built => buffer size "
         << fOutputBuffer->GetSize();
   // Clear output buffer as no event consumer in this mode
   // Commented out as should be DONE in CbmGet4EventDumper
 //  fOutputBuffer->Clear();
#endif // MUL_TRIGG_DEBUG
}

void CbmGet4EventBuilder::Finish()
{
   LOG(info)<<"CbmGet4EventBuilder::Finish up with "
            << fuInputEvents << " events processed and "
            << fuOutputEvents << " events built" ;

   WriteHistos();

   DeleteHistos();
}

/************************************************************************************/
Bool_t   CbmGet4EventBuilder::RegisterInputs()
{
   fInputBuffer = CbmGet4EpochBuffer::Instance();

   if(NULL == fInputBuffer)
      return kFALSE;

   return kTRUE;
}
Bool_t   CbmGet4EventBuilder::RegisterOutputs()
{
   fOutputBuffer = CbmGet4EventBuffer::Instance();

   if(NULL == fOutputBuffer)
      return kFALSE;

   fOutputBuffer->PrintStatus();

   return kTRUE;
}
/************************************************************************************/
// ------------------------------------------------------------------
Bool_t CbmGet4EventBuilder::CreateHistos()
{
  // Create histogramms

   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !

   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!

   return kTRUE;
}

// ------------------------------------------------------------------
Bool_t CbmGet4EventBuilder::FillHistos()
{
   return kTRUE;
}
// ------------------------------------------------------------------

Bool_t CbmGet4EventBuilder::WriteHistos()
{
   // TODO: add sub-folders

   // Write histogramms to the file
   TDirectory * oldir = gDirectory;
   TFile *fHist = new TFile(fsHistoFilename,"RECREATE");
   fHist->cd();

   fhMaxMulPerEpochDist->Write();
   fhEventSizeDist->Write();
#ifdef MUL_TRIGG_DEBUG
   fhMaxMulEpoch->Write();
   fhBinThrEpoch->Write();
   fhMaxEvtSizeEpoch->Write();
   fhEvtSizeEvo->Write();
#endif // MUL_TRIGG_DEBUG
   gDirectory->cd( oldir->GetPath() );

   fHist->Close();

   return kTRUE;
}
Bool_t   CbmGet4EventBuilder::DeleteHistos()
{
   return kTRUE;
}

ClassImp(CbmGet4EventBuilder)
