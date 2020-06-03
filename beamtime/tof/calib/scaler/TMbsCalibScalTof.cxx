// ------------------------------------------------------------------
// -----                    TMbsCalibScalTof                    -----
// -----              Created 08/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TMbsCalibScalTof.h"

// General Unpack headers
#include "TMbsUnpackTofPar.h"

// ToF specific headers
#include "TMbsCalibTofPar.h"
#include "TofDef.h"
#include "TofScalerDef.h"
#include "TofScomDef.h"
#include "TofScal2014Def.h"
#include "TofTriglogDef.h"
#include "TofOrGenDef.h"
#include "TTofScalerBoard.h"
#include "TTofTriglogBoard.h"
#include "TTofCalibScaler.h"

// FAIR headers
#include "FairLogger.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"

// ROOT headers
#include "TClonesArray.h"
#include "TH2.h"
#include "TH1.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TTimeStamp.h"

TMbsCalibScalTof::TMbsCalibScalTof() : 
   fMbsUnpackPar(NULL),
   fMbsCalibPar(NULL),
   fhRefClkRate(),
   fhRefClkRateEvo(),
   fhRefMbsTimeComp(),
   fhScalersRate(),
   fhScalersRateEvo(),
   fdEvoRangeUser(-1),
   fdEvoBinSzUser(-1),
   fScalerBoardCollection(NULL),
   fTriglogBoardCollection(NULL),
   fbSaveCalibScalers(kFALSE),
   fCalibScalCollection(NULL),
   bFirstEvent(kTRUE),
   bFirstEvent2014(kTRUE),
   fvuFirstRefClk(),
   fvuRefClkCycle(),
   fvuLastRefClk(),
   fvuLastScalers(),
   fdFirstMbsTime(0.0),
   fdPrevMbsTime(0.0),
   fvdPrevMbsTimeBd(),
   fvuLastRefClkCal(),
   fvdMeanRefClkFreq()
{
   // Get Base Container
   FairRunAna* ana = FairRunAna::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   // Unpack parameter
   fMbsUnpackPar = (TMbsUnpackTofPar*) (rtdb->getContainer("TMbsUnpackTofPar"));
   if( 0 == fMbsUnpackPar )
      LOG(error)<<"TMbsCalibScalTof::TMbsCalibScalTof => Could not obtain the TMbsUnpackTofPar ";
      
   // Calibration parameter
   fMbsCalibPar = (TMbsCalibTofPar*) (rtdb->getContainer("TMbsCalibTofPar"));
   if( 0 == fMbsCalibPar )
      LOG(error)<<"TMbsCalibScalTof::TMbsCalibScalTof => Could not obtain the TMbsCalibTofPar ";
      else fMbsCalibPar->printParams();
}

TMbsCalibScalTof::TMbsCalibScalTof(TMbsUnpackTofPar * parIn, TMbsCalibTofPar *parCalIn) :
   fMbsUnpackPar(parIn),
   fMbsCalibPar(parCalIn),
   fhRefClkRate(),
   fhRefClkRateEvo(),
   fhRefMbsTimeComp(),
   fhScalersRate(),
   fhScalersRateEvo(),
   fdEvoRangeUser(-1),
   fdEvoBinSzUser(-1),
   fScalerBoardCollection(NULL),
   fTriglogBoardCollection(NULL),
   fbSaveCalibScalers(kFALSE),
   fCalibScalCollection(NULL),
   bFirstEvent(kTRUE),
   bFirstEvent2014(kTRUE),
   fvuFirstRefClk(),
   fvuRefClkCycle(),
   fvuLastRefClk(),
   fvuLastScalers(),
   fdFirstMbsTime(0.0),
   fdPrevMbsTime(0.0),
   fvdPrevMbsTimeBd(),
   fvuLastRefClkCal(),
   fvdMeanRefClkFreq()
{
}
      
TMbsCalibScalTof::~TMbsCalibScalTof()
{
   DeleteHistograms();
   LOG(info)<<"**** TMbsCalibScalTof: Delete instance ";
} 

void TMbsCalibScalTof::Clear(Option_t * /*option*/)
{
   fMbsUnpackPar = NULL;
   fMbsCalibPar  = NULL;
   fScalerBoardCollection  = NULL;
   fTriglogBoardCollection = NULL;
   fbSaveCalibScalers      = kFALSE;
   fCalibScalCollection    = NULL;
}

// Calibration Functions
Bool_t TMbsCalibScalTof::InitScalersCalib()
{   
/*
   if( kFALSE == RegisterInput() )
      return kFALSE;
      
   if( kFALSE == RegisterOutput() )
      return kFALSE;
      
   if( kFALSE == CreateHistogramms() )
      return kFALSE;
*/
   if( kFALSE == InitCalibration() )
      return kFALSE;
      
   return kTRUE;
}
Bool_t TMbsCalibScalTof::ClearCalib()
{
   ClearOutput();
   return kTRUE;
}
Bool_t TMbsCalibScalTof::CalibScalers()
{
   UInt_t uNbScalBd = fMbsUnpackPar->GetNbActiveScalersB();
   // Loop over all scaler boards
   for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
      Calibration( uScalBdIndx );
      
   // Update the MBS time info used by all boards from the single TRIGLOG unpack event
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
   {
      if( NULL == fTriglogBoardCollection )
         return kFALSE;
         
      TTofTriglogBoard * xTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->At(0); // Always only 1 TRIGLOG board!
      
      if( NULL == xTriglogBoard )
         return kFALSE;
      
      Double_t dMbsTime = xTriglogBoard->GetMbsTimeSec() + xTriglogBoard->GetMbsTimeMilliSec()*1e-3;
      if( ( 0 == fdPrevMbsTime ) || ( fdPrevMbsTime + 0.05 < dMbsTime ) ) 
         fdPrevMbsTime = dMbsTime;

      if( kTRUE == bFirstEvent )
         fdFirstMbsTime = dMbsTime;
   } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      
   if( kTRUE == bFirstEvent )
      bFirstEvent = kFALSE;
      
   return kTRUE;
}
Bool_t TMbsCalibScalTof::CloseScalersCalib()
{
   if( kFALSE == CloseCalibration() )
      return kFALSE;
      
//   if( kFALSE == DeleteHistograms() )
//      return kFALSE;
      
   return kTRUE;
}
Bool_t TMbsCalibScalTof::InitParameters()
{
   return kTRUE;
}
// ------------------------------------------------------------------
Bool_t TMbsCalibScalTof::RegisterInput()
{
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
   {
      fTriglogBoardCollection = (TClonesArray*) rootMgr->GetObject("TofTriglog");
      if( NULL == fTriglogBoardCollection) 
      {
         LOG(error)<<"TMbsCalibScalTof::RegisterInput => Could not get the TofTriglog TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fTriglogBoardCollection) 
   } // if( 1 == fParUnpack->GetNbActiveBoards( tofMbs::triglog ) )
   
   // Each scaler board has its scaler unpack object, same type for TRIGLOG, ScalOrMu, etc...
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
   {
      fScalerBoardCollection = (TClonesArray*) rootMgr->GetObject("TofRawScalers");
      if( NULL == fScalerBoardCollection) 
      {
         LOG(error)<<"TMbsCalibScalTof::RegisterInput => Could not get the TofRawScalers TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fScalerBoardCollection) 
   } // if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
   
   return kTRUE;
}
Bool_t TMbsCalibScalTof::RegisterOutput()
{
   FairRootManager* rootMgr = FairRootManager::Instance();
   fCalibScalCollection = new TClonesArray("TTofCalibScaler");

   rootMgr->Register( "TofCalibScaler","Tof",fCalibScalCollection,
                      fMbsUnpackPar->WriteDataInCbmOut() || fbSaveCalibScalers);
   
   return kTRUE;
}
Bool_t TMbsCalibScalTof::ClearOutput()
{
   fCalibScalCollection->Clear("C");
   return kTRUE;
}
void TMbsCalibScalTof::SetSaveScalers( Bool_t bSaveScal )
{
   fbSaveCalibScalers = bSaveScal;
   if( kTRUE == bSaveScal ) {
     LOG(info)<<"TMbsCalibScalTof => Enable the saving of calibrated scaler data in analysis output file";
   } else {
     LOG(info)<<"TMbsCalibScalTof => Disable the saving of calibrated scaler data in analysis output file";
   }
}
// ------------------------------------------------------------------
Bool_t TMbsCalibScalTof::CreateHistogramms()
{
   UInt_t uNbScalBd = fMbsUnpackPar->GetNbActiveScalersB();
   // Resize vectors with a "board" dimension
   if( 0 < uNbScalBd )
   {      
      fhScalersRate.resize( uNbScalBd );
      fhScalersRateEvo.resize( uNbScalBd );
//      fhRefClkRateEstA.resize( uNbScalBd );
//      fhRefClkRateEstB.resize( uNbScalBd );
      if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      {
         fhRefClkRateEvo.resize( uNbScalBd );
         fhRefMbsTimeComp.resize( uNbScalBd );
         fhRefClkRate.resize( uNbScalBd );
      } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
   } // if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
   
   // Loop over all scalers
   for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
   {
      TTofScalerBoard  * xScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->At(uScalBdIndx);
      UInt_t   uType = xScalerBoard->GetScalerType();
      Double_t dRefClkFreq;
      UInt_t   uNbScalers;
      UInt_t   uNbChan;
      Double_t dEvoRange;
      Double_t dEvoBinNb;
      switch( uType )
      {
         case tofscaler::triglog :
         case tofscaler::triglogscal :
            dRefClkFreq = triglog::kdRefClkFreq;
            uNbScalers  = triglog::kuNbScalers;
            uNbChan     = triglog::kuNbChan;
            dEvoRange   = triglog::kdEvoRange;
            dEvoBinNb   = triglog::kdEvoRange / triglog::kdEvoBin;
            break;
         case tofscaler::scalormu :
            dRefClkFreq = scalormu::kdRefClkFreq;
            uNbScalers  = scalormu::kuNbScalers;
            uNbChan     = scalormu::kuNbChan;
            dEvoRange   = scalormu::kdEvoRange;
            dEvoBinNb   = scalormu::kdEvoRange / scalormu::kdEvoBin;
            break;
         case tofscaler::scalormubig :
            dRefClkFreq = scalormuBig::kdRefClkFreq;
            uNbScalers  = scalormuBig::kuNbScalers;
            uNbChan     = scalormuBig::kuNbChan;
            dEvoRange   = scalormuBig::kdEvoRange;
            dEvoBinNb   = scalormuBig::kdEvoRange / scalormuBig::kdEvoBin;
            break;
         case tofscaler::scaler2014 :
            dRefClkFreq = scaler2014::kdRefClkFreq;
            uNbScalers  = scaler2014::kuNbScalers;
            uNbChan     = scaler2014::kuNbChan;
            dEvoRange   = scaler2014::kdEvoRange;
            dEvoBinNb   = scaler2014::kdEvoRange / scaler2014::kdEvoBin;
            break;
         case tofscaler::orgen :
            dRefClkFreq = orgen::kdRefClkFreq;
            uNbScalers  = orgen::kuNbScalers;
            uNbChan     = orgen::kuNbChan;
            dEvoRange   = orgen::kdEvoRange;
            dEvoBinNb   = orgen::kdEvoRange / orgen::kdEvoBin;
            break;
         case tofscaler::undef :
         default:
            dRefClkFreq = -1.0;
            uNbScalers  =  0;
            uNbChan     =  0;
            dEvoRange   =  0;
            dEvoBinNb   =  0;
            // Here we go to next board for all undefined/invalid types
            continue; 
            break;
      } // switch( xScalerBoard->GetScalerType() )
      
      if( 0 < fdEvoRangeUser && 0 < fdEvoBinSzUser && fdEvoBinSzUser < fdEvoRangeUser )
      {
         dEvoRange = fdEvoRangeUser;
         dEvoBinNb = static_cast<Int_t>(fdEvoRangeUser) / static_cast<Int_t>(fdEvoBinSzUser);
      } // if( 0 < fdEvoRangeUser && 0 < fdEvoBinSzUser && fdEvoBinSzUser < fdEvoRangeUser )

      // Resize vectors with a "Scalers" dimension
      fhScalersRate[uScalBdIndx].resize( uNbScalers );
      fhScalersRateEvo[uScalBdIndx].resize( uNbScalers );
      for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
      {
         // resize vectors with a "channel" dimension
         fhScalersRate[uScalBdIndx][uScaler].resize(uNbChan);
         fhScalersRateEvo[uScalBdIndx][uScaler].resize(uNbChan);
         for( UInt_t uCh = 0; uCh < uNbChan; uCh++) 
         {
            fhScalersRate[uScalBdIndx][uScaler][uCh]    = 
                  new TH1I( Form("tof_cal_%s_%02d_scal_%02d_%02d",
                                  tofscaler::ksTdcHistName[ uType ].Data(), uScalBdIndx, uScaler, uCh ),
                            Form("Rate of the channel %02d in scaler %02d; Freq. [Hz]", uCh, uScaler), 
                            (Int_t)(dRefClkFreq/2000), 0.0 , dRefClkFreq/2 );
//            fhScalersRateEvo[uScalBdIndx][uScaler][uCh] = 
//                  new TH1I( Form("tof_cal_%s_scal_evo_%02d_%02d", tofscaler::ksTdcHistName[ uType ].Data(), uScaler, uCh ),
//                             Form("Channel %02d in scaler %02d counts per second; Time [s]; Counts []", uCh, uScaler), 
//                             3600, 0.0, dEvoRange );
            fhScalersRateEvo[uScalBdIndx][uScaler][uCh] = 
                  new TProfile( Form("tof_cal_%s_%02d_scal_evo_%02d_%02d",
                                tofscaler::ksTdcHistName[ uType ].Data(), uScalBdIndx, uScaler, uCh ),
                             Form("Channel %02d in scaler %02d Mean rate per second; Time [s]; Rate [1/s]", uCh, uScaler), 
                             static_cast<Int_t>(dEvoBinNb), 0.0, dEvoRange );
         } // for( UInt_t uCh = 0; uCh < triglog::kuNbChan; uCh++) 
      } // for( UInt_t uScaler = 0; uScaler <triglog::kuNbScalers; uScaler++)
      
      // There should always be only 1 Trigger board active, if not => problem
      if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      {
         fhRefClkRate[uScalBdIndx] = new TH1I( Form("tof_cal_%s_%02d_refclk",
                                               tofscaler::ksTdcHistName[ uType ].Data(), uScalBdIndx ),
                              "Rate of the reference clock; Freq. [Hz]", 
                              (Int_t)(2*dRefClkFreq/1000), 0.0 , 2*dRefClkFreq );
//         fhRefClkRateEvo[uScalBdIndx] = new TH1I( Form("tof_cal_%s_%02d_refclk_evo",
//                                                  tofscaler::ksTdcHistName[ uType ].Data(), uScalBdIndx ),
//                              "Reference clock counts per second; Time [s]; Counts []", dEvoBinNb, 0.0, dEvoRange );
         fhRefClkRateEvo[uScalBdIndx] = new TProfile( Form("tof_cal_%s_%02d_refclk_evo",
                                                  tofscaler::ksTdcHistName[ uType ].Data(), uScalBdIndx ),
                              "Reference clock counts per second; Time [s]; Counts []",
                              (Int_t)dEvoRange, 0.0, (Int_t)dEvoRange );

//         fhRefClkRateEstA[uScalBdIndx] = new TH2I( Form("tof_cal_%s_%02d_refclk_estA",
//                     tofscaler::ksTdcHistName[ uType ].Data(), uScalBdIndx ),
//                     "Reference clock counts per second; Time [s]; Counts []",
//                     (Int_t)dEvoRange, 0.0, (Int_t)dEvoRange,
//                     500, 750*1e3, 850*1e3 );
//         fhRefClkRateEstB[uScalBdIndx] = new TH2I( Form("tof_cal_%s_%02d_refclk_estB",
//                     tofscaler::ksTdcHistName[ uType ].Data(), uScalBdIndx ),
//                     "Reference clock counts per second; Time [s]; Counts []",
//                     (Int_t)dEvoRange, 0.0, (Int_t)dEvoRange,
//                     500, 750*1e3, 850*1e3 );

         fhRefMbsTimeComp[uScalBdIndx] = new TH2I( Form("tof_cal_%s_%02d_refmbs_comp",
                                                   tofscaler::ksTdcHistName[ uType ].Data(), uScalBdIndx ),
                              "Comparison of clk VS Mbs time since first event; Mbs Time [s]; Mbs Time - Ref Clk Time [s]", 
                              dEvoBinNb/10, 0.0, dEvoRange, 200, -0.1, 0.1 );
      } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
   } //  for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
   
   return kTRUE;
}
Bool_t TMbsCalibScalTof::FillHistograms()
{
   UInt_t uNbScalBd = fMbsUnpackPar->GetNbActiveScalersB();
      
   TTofTriglogBoard * xTriglogBoard;
   Double_t dMbsTime = 0;
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
   {
      if( NULL == fTriglogBoardCollection )
         return kFALSE;
         
      xTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->At(0); // Always only 1 TRIGLOG board!
      
      if( NULL == xTriglogBoard )
         return kFALSE;

      // In GSI April 2014 beamtime some event are used to empty TRB buffers
      // and therefore do not contain the MBS event
      if( kFALSE == xTriglogBoard->IsUpdated() )
      {
         LOG(error)<<"TMbsCalibScalTof::FillHistograms => No TRIGLOG sub-event in this event,"
                   <<" OK for GSI April 2014 beamtime";
         return kFALSE;
      } // if( kFALSE == fTriglogBoard->IsUpdated() && kFALSE == fScalerBoard->IsUpdated() )
         
      dMbsTime = xTriglogBoard->GetMbsTimeSec() + xTriglogBoard->GetMbsTimeMilliSec()*1e-3;
   } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      
   // Loop over all scalers
   for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
   {
      TTofCalibScaler  * xCalibScaler  = (TTofCalibScaler*)  fCalibScalCollection->ConstructedAt(uScalBdIndx);
      UInt_t   uType = xCalibScaler->GetScalerType();
//      Double_t dRefClkFreq; // -> Comment to remove warning because set but never used
      UInt_t   uNbScalers;
      UInt_t   uNbChan;
      switch( uType )
      {
         case tofscaler::triglog :
         case tofscaler::triglogscal :
//            dRefClkFreq = triglog::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = triglog::kuNbScalers;
            uNbChan     = triglog::kuNbChan;
            break;
         case tofscaler::scalormu :
//            dRefClkFreq = scalormu::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = scalormu::kuNbScalers;
            uNbChan     = scalormu::kuNbChan;
            break;
         case tofscaler::scalormubig :
//            dRefClkFreq = scalormuBig::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = scalormuBig::kuNbScalers;
            uNbChan     = scalormuBig::kuNbChan;
            break;
         case tofscaler::scaler2014 :
//            dRefClkFreq = scaler2014::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = scaler2014::kuNbScalers;
            uNbChan     = scaler2014::kuNbChan;
            break;
         case tofscaler::orgen :
//            dRefClkFreq = orgen::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = orgen::kuNbScalers;
            uNbChan     = orgen::kuNbChan;
            break;
         case tofscaler::undef :
         default:
//            dRefClkFreq = -1.0; // -> Comment to remove warning because set but never used
            uNbScalers  =  0;
            uNbChan     =  0;
            // Here we go to next board for all undefined/invalid types
            continue; 
            break;
      } // switch( xScalerBoard->GetScalerType() )

      if( ( kFALSE == xCalibScaler->IsUpdated() || kTRUE == bFirstEvent2014 )
            && uType == tofscaler::scaler2014 )
         // This event did not contain 2014 scalers data
         continue;
            
      // Comparison between the time since first event stored by MBS in the TRIGLOG event and the one
      // obtained using the reference clock counts and frequency
      if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) && kFALSE == bFirstEvent
            && kTRUE == xCalibScaler->IsUpdated() )
      {
         Double_t dMbsTimeSinceFirst = dMbsTime - fdFirstMbsTime;
         fhRefMbsTimeComp[uScalBdIndx]->Fill( dMbsTimeSinceFirst, dMbsTimeSinceFirst - xCalibScaler->GetTimeToFirst() );

         // Instantaneous rate of individual channels
         for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
         {
            for( UInt_t uCh = 0; uCh < uNbChan; uCh++)
            {
               if( 0 < xCalibScaler->GetTimeToLast() )
               {
                  fhScalersRate[uScalBdIndx][uScaler][uCh]->Fill( xCalibScaler->GetScalerValue( uCh, uScaler) );
                  fhScalersRateEvo[uScalBdIndx][uScaler][uCh]->Fill( dMbsTimeSinceFirst,
                                                                     xCalibScaler->GetScalerValue( uCh, uScaler) );
               } // if( 0 < xCalibScaler->GetTimeToLast() )
            } // for( UInt_t uCh = 0; uCh < uNbChan; uCh++)
         } // for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
      } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) && kFALSE == bFirstEvent )
         else
         {
            // Instantaneous rate of individual channels
            for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
            {
               for( UInt_t uCh = 0; uCh < uNbChan; uCh++)
               {
                  if( 0 < xCalibScaler->GetTimeToLast() )
                  {
                     fhScalersRate[uScalBdIndx][uScaler][uCh]->Fill( xCalibScaler->GetScalerValue( uCh, uScaler) );
                     fhScalersRateEvo[uScalBdIndx][uScaler][uCh]->Fill( xCalibScaler->GetTimeToFirst(),
                                                                        xCalibScaler->GetScalerValue( uCh, uScaler) );
                  } // if( 0 < xCalibScaler->GetTimeToLast() )
               } // for( UInt_t uCh = 0; uCh < uNbChan; uCh++)
            } // for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
         } // else of if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) && kFALSE == bFirstEvent )
   } //  for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
      
//   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      
   if( kTRUE == bFirstEvent2014 )
      // Loop over all scalers to find 2014 ones
      for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
      {
         TTofCalibScaler  * xCalibScaler  = (TTofCalibScaler*)  fCalibScalCollection->ConstructedAt(uScalBdIndx);
         UInt_t   uType = xCalibScaler->GetScalerType();
         if( uType == tofscaler::scaler2014 &&
               kTRUE == xCalibScaler->IsUpdated()  )
         {
            // This event was the first to contain 2014 scalers data
            bFirstEvent2014 = kFALSE;
            break;
         }
      } // for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )

   return kTRUE;
}
Bool_t TMbsCalibScalTof::WriteHistogramms( TDirectory* inDir)
{
   TDirectory * oldir = gDirectory;
   
   LOG(info)<<"TMbsCalibScalTof::WriteHistogramms => Start writing histos!!!";

   UInt_t uNbScalBd = fMbsUnpackPar->GetNbActiveScalersB();
    // Loop over all scalers
   for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
   {
      TTofCalibScaler  * xCalibScaler  = (TTofCalibScaler*)  fCalibScalCollection->ConstructedAt(uScalBdIndx);
      UInt_t   uType = xCalibScaler->GetScalerType();
      UInt_t   uNbScalers;
      UInt_t   uNbChan;
      switch( uType )
      {
         case tofscaler::triglog :
         case tofscaler::triglogscal :
            uNbScalers  = triglog::kuNbScalers;
            uNbChan     = triglog::kuNbChan;
            break;
         case tofscaler::scalormu :
            uNbScalers  = scalormu::kuNbScalers;
            uNbChan     = scalormu::kuNbChan;
            break;
         case tofscaler::scalormubig :
            uNbScalers  = scalormuBig::kuNbScalers;
            uNbChan     = scalormuBig::kuNbChan;
            break;
         case tofscaler::scaler2014 :
            uNbScalers  = scaler2014::kuNbScalers;
            uNbChan     = scaler2014::kuNbChan;
            break;
         case tofscaler::orgen :
            uNbScalers  = orgen::kuNbScalers;
            uNbChan     = orgen::kuNbChan;
            break;
         case tofscaler::undef :
         default:
            // Here we go to next board for all undefined/invalid types
            continue; 
            break;
      } // switch( xScalerBoard->GetScalerType() )
      
      // create a subdirectory "Cal_type" in this file
      TDirectory *cdCal = inDir->mkdir( Form( "Cal_%s_%02d", tofscaler::ksTdcHistName[ uType ].Data(), uScalBdIndx ) );
      cdCal->cd();    // make the "Cal_type" directory the current directory
      TDirectory *cdCalScal[ uNbScalers ];
      
      if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      {
         fhRefClkRate[uScalBdIndx]->Write();
         fhRefClkRateEvo[uScalBdIndx]->Write();
//         fhRefClkRateEstA[uScalBdIndx]->Write();
//         fhRefClkRateEstB[uScalBdIndx]->Write();
         fhRefMbsTimeComp[uScalBdIndx]->Write();
      }
      
      for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
      {
         // Create a sub folder for each scaler
         cdCalScal[uScaler] = cdCal->mkdir( Form( "cScal%03d", uScaler) );
         cdCalScal[uScaler]->cd();
         
         for( UInt_t uCh = 0; uCh < uNbChan; uCh++) 
         {
            fhScalersRate[uScalBdIndx][uScaler][uCh]->Write();
            fhScalersRateEvo[uScalBdIndx][uScaler][uCh]->Write();
         }
      } // for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
   } //  for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
   
   gDirectory->cd( oldir->GetPath() );
   return kTRUE;
}
Bool_t TMbsCalibScalTof::DeleteHistograms()
{
   UInt_t uNbScalBd = fMbsUnpackPar->GetNbActiveScalersB();
   // Loop over all scalers
   for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
   {
      TTofScalerBoard  * xScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->At(uScalBdIndx);
      UInt_t   uType = xScalerBoard->GetScalerType();
         
      UInt_t   uNbScalers;
      switch( uType )
      {
         case tofscaler::triglog :
         case tofscaler::triglogscal :
            uNbScalers  = triglog::kuNbScalers;
            break;
         case tofscaler::scalormu :
            uNbScalers  = scalormu::kuNbScalers;
            break;
         case tofscaler::scalormubig :
            uNbScalers  = scalormuBig::kuNbScalers;
            break;
         case tofscaler::scaler2014 :
            uNbScalers  = scaler2014::kuNbScalers;
            break;
         case tofscaler::orgen :
            uNbScalers  = orgen::kuNbScalers;
            break;
         case tofscaler::undef :
         default:
            // Here we go to next board for all undefined/invalid types
            continue; 
            break;
      } // switch( xScalerBoard->GetScalerType() )
      
      for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
      {
         fhScalersRate[uScalBdIndx][uScaler].clear();
         fhScalersRateEvo[uScalBdIndx][uScaler].clear();
      } // for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
      fhScalersRate[uScalBdIndx].clear();
      fhScalersRateEvo[uScalBdIndx].clear();
   } // for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
   
   if( 0 < uNbScalBd )
   {      
      fhScalersRate.clear();
      fhScalersRateEvo.clear();
      if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      {
         fhRefClkRateEvo.clear();
         fhRefMbsTimeComp.clear();
         fhRefClkRate.clear();
      } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
   } // if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
   
   return kTRUE;
}
// ------------------------------------------------------------------
Bool_t TMbsCalibScalTof::InitCalibration()
{
   UInt_t uNbScalBd = fMbsUnpackPar->GetNbActiveScalersB();
   if( 0 < uNbScalBd )
   {
      fvuLastScalers.resize( uNbScalBd );
      fvuFirstRefClk.resize( uNbScalBd );
      fvuRefClkCycle.resize( uNbScalBd );
      fvuLastRefClk.resize( uNbScalBd );
      fvdPrevMbsTimeBd.resize( uNbScalBd );
      fvuLastRefClkCal.resize( uNbScalBd );
      fvdMeanRefClkFreq.resize( uNbScalBd );
   }
      
   // Loop over all scalers
   for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
   {
      TTofScalerBoard  * xScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->At(uScalBdIndx);
      TTofCalibScaler  * xCalibScaler  = (TTofCalibScaler*)  fCalibScalCollection->ConstructedAt(uScalBdIndx);
      UInt_t   uType = xScalerBoard->GetScalerType();
      
      if( tofscaler::undef == xCalibScaler->GetScalerType() )
         xCalibScaler->SetType( uType );
         
//      Double_t dRefClkFreq; // -> Comment to remove warning because set but never used
      UInt_t   uNbScalers;
      UInt_t   uNbChan;
      switch( uType )
      {
         case tofscaler::triglog :
         case tofscaler::triglogscal :
//            dRefClkFreq = triglog::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = triglog::kuNbScalers;
            uNbChan     = triglog::kuNbChan;
            break;
         case tofscaler::scalormu :
//            dRefClkFreq = scalormu::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = scalormu::kuNbScalers;
            uNbChan     = scalormu::kuNbChan;
            break;
         case tofscaler::scalormubig :
//            dRefClkFreq = scalormuBig::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = scalormuBig::kuNbScalers;
            uNbChan     = scalormuBig::kuNbChan;
            break;
         case tofscaler::scaler2014 :
//            dRefClkFreq = scaler2014::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = scaler2014::kuNbScalers;
            uNbChan     = scaler2014::kuNbChan;
            break;
         case tofscaler::orgen :
//            dRefClkFreq = orgen::kdRefClkFreq; // -> Comment to remove warning because set but never used
            uNbScalers  = orgen::kuNbScalers;
            uNbChan     = orgen::kuNbChan;
            break;
         case tofscaler::undef :
         default:
//            dRefClkFreq = -1.0; // -> Comment to remove warning because set but never used
            uNbScalers  =  0;
            uNbChan     =  0;
            // Here we go to next board for all undefined/invalid types
            continue; 
            break;
      } // switch( xScalerBoard->GetScalerType() )
      
      fvuLastScalers[uScalBdIndx].resize(uNbScalers);
      for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
      {
         fvuLastScalers[uScalBdIndx][uScaler].resize(uNbChan);
         for( UInt_t uCh = 0; uCh < uNbChan; uCh++) 
         {
            fvuLastScalers[uScalBdIndx][uScaler][uCh] = 0;
         } // for( UInt_t uCh = 0; uCh < uNbChan; uCh++) 
      } // for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
      
      fvuFirstRefClk[uScalBdIndx]    = 0;
      fvuRefClkCycle[uScalBdIndx]    = 0;
      fvuLastRefClk[uScalBdIndx]     = 0;
      fvdPrevMbsTimeBd[uScalBdIndx]  = 0;
      fvuLastRefClkCal[uScalBdIndx]  = 0;
      fvdMeanRefClkFreq[uScalBdIndx] = -1.0;
   } //  for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
   
   fdFirstMbsTime = 0;
   fdPrevMbsTime  = 0;
   
   bFirstEvent = kTRUE;
   bFirstEvent2014 = kTRUE;
      
   return kTRUE;
}
Bool_t TMbsCalibScalTof::Calibration( UInt_t uBoard)
{
   if( fMbsUnpackPar->GetNbActiveScalersB() <= uBoard)
      return kFALSE;
       
   if( NULL == fScalerBoardCollection || 
       NULL == fCalibScalCollection )
      return kFALSE;
      
   TTofScalerBoard  * xScalerBoard  = (TTofScalerBoard*)  fScalerBoardCollection->At(uBoard);
   TTofCalibScaler  * xCalibScaler  = (TTofCalibScaler*)  fCalibScalCollection->ConstructedAt(uBoard);
      
   if( NULL == xScalerBoard || NULL == xCalibScaler )
      return kFALSE;
   
   UInt_t   uType = xScalerBoard->GetScalerType();
      
   if( tofscaler::undef == xCalibScaler->GetScalerType() )
      xCalibScaler->SetType( uType );

   if( kFALSE == xScalerBoard->IsUpdated() )
   {
      // This event did not contain scalers data
      xCalibScaler->SetPresentFlag( kFALSE );
      return kFALSE;
   } // if( kFALSE == xScalerBoard->IsUpdated() )
      else xCalibScaler->SetPresentFlag( kTRUE );

   Double_t dRefClkFreq;
   UInt_t   uNbScalers;
   UInt_t   uNbChan;
   switch( uType )
   {
      case tofscaler::triglog :
      case tofscaler::triglogscal :
         dRefClkFreq = triglog::kdRefClkFreq;
         uNbScalers  = triglog::kuNbScalers;
         uNbChan     = triglog::kuNbChan;
         break;
      case tofscaler::scalormu :
         dRefClkFreq = scalormu::kdRefClkFreq;
         uNbScalers  = scalormu::kuNbScalers;
         uNbChan     = scalormu::kuNbChan;
         break;
      case tofscaler::scalormubig :
         dRefClkFreq = scalormuBig::kdRefClkFreq;
         uNbScalers  = scalormuBig::kuNbScalers;
         uNbChan     = scalormuBig::kuNbChan;
         break;
      case tofscaler::scaler2014 :
         dRefClkFreq = scaler2014::kdRefClkFreq;
         uNbScalers  = scaler2014::kuNbScalers;
         uNbChan     = scaler2014::kuNbChan;
         break;
      case tofscaler::orgen :
         dRefClkFreq = orgen::kdRefClkFreq;
         uNbScalers  = orgen::kuNbScalers;
         uNbChan     = orgen::kuNbChan;
         break;
      case tofscaler::undef :
      default:
         dRefClkFreq = -1.0;
         uNbScalers  =  0;
         uNbChan     =  0;
         // Here we go to next board for all undefined/invalid types
         return kFALSE; 
         break;
   } // switch( xScalerBoard->GetScalerType() )

   // Detection of reference clock counter cycles
   if( xScalerBoard->GetRefClk() < fvuLastRefClk[uBoard] )
      fvuRefClkCycle[uBoard] ++;
   
   // There should always be only 1 Trigger board active, if not => problem
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
   {
      TTofTriglogBoard * xTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->At(0); // Always only 1 TRIGLOG board!

      // In GSI April 2014 beamtime some event are used to empty TRB buffers
      // and therefore do not contain the MBS event
      if( kFALSE == xTriglogBoard->IsUpdated() )
      {
         LOG(error)<<"TMbsCalibScalTof::Calibration => No TRIGLOG sub-event in this event,"
                   <<" OK for GSI April 2014 beamtime";
         return kFALSE;
      } // if( kFALSE == fTriglogBoard->IsUpdated() && kFALSE == fScalerBoard->IsUpdated() )
      
      if( NULL == xTriglogBoard )
      {
         LOG(error)<<"TMbsCalibScalTof::Calibration => TRIGLOG board object not existing!!!!";
         LOG(error)<<"TMbsCalibScalTof::Calibration => TTofTriglogBoard = "<<xTriglogBoard;
         LOG(error)<<"TMbsCalibScalTof::Calibration => Probably the TRIGLOG sub-event is not there in this event!";
         return kFALSE;
      } // if( NULL == xTriglogBoard )

      Double_t dMbsTime = xTriglogBoard->GetMbsTimeSec() + xTriglogBoard->GetMbsTimeMilliSec()*1e-3;
      if( 0 < fdPrevMbsTime )
         // MBS time is time in s since LINUX epoch (circa 1970) => never 0 except on first event!
      {
         // Update only when at least 50ms passed since last update due to ms precision of the MBS time
         if( (fdPrevMbsTime + 0.05 < dMbsTime && kFALSE == bFirstEvent && uType != tofscaler::scaler2014) ||
             (fvdPrevMbsTimeBd[uBoard] + 0.05 < dMbsTime && kFALSE == bFirstEvent && uType == tofscaler::triglogscal) ||
             (fvdPrevMbsTimeBd[uBoard] + 0.05 < dMbsTime && kFALSE == bFirstEvent2014
                                                         && uType == tofscaler::scaler2014)  )
         {
            Double_t dTimeDiff = dMbsTime - fdPrevMbsTime;
            if( uType == tofscaler::scaler2014 || uType == tofscaler::triglogscal)
               dTimeDiff = dMbsTime - fvdPrevMbsTimeBd[uBoard];
            // Update only if less than 500ms passed since last update to avoid problems with spill breaks
            if( dTimeDiff < 0.5 &&  uType != tofscaler::scaler2014 )
            {
               // Monitoring the rate of the reference clock
               fhRefClkRate[uBoard]->Fill( (Double_t)((Double_t)(xScalerBoard->GetRefClk()) - (Double_t)(fvuLastRefClkCal[uBoard]) ) /
                                           dTimeDiff );
               fhRefClkRateEvo[uBoard]->Fill(dMbsTime - fdFirstMbsTime,
                     (Double_t)((Double_t)(xScalerBoard->GetRefClk()) - (Double_t)(fvuLastRefClkCal[uBoard]))/dTimeDiff );

               // Try to update the clock frequency only if there are enough points in the monitoring histo,
               // the distribution spread is reasonnable and the option is ON
               if(  2000 < fhRefClkRate[uBoard]->GetEntries() &&
                   10000 < fhRefClkRate[uBoard]->GetRMS() && kTRUE == fMbsCalibPar->CalibRefClocks()
                  )
                  fvdMeanRefClkFreq[uBoard] = fhRefClkRate[uBoard]->GetMean();

//               fhRefClkRateEstA[uBoard]->Fill( dMbsTime - fdFirstMbsTime, fhRefClkRate[uBoard]->GetMean() );
//               fhRefClkRateEstB[uBoard]->Fill( dMbsTime - fdFirstMbsTime, fhRefClkRateEvo[uBoard]->GetMean(2) );
            } // if( 0 < fdPrevMbsTime )
            // in case of 2014 GSI April beamtime this happens quite often for scalers => use time dist!
            // Same at cern Feb15
            else if( dTimeDiff < 5 && (uType == tofscaler::scaler2014 || uType == tofscaler::triglogscal) ) //&&
       //              fvuLastRefClkCal[uBoard] < xScalerBoard->GetRefClk())
            {
               // Monitoring the rate of the reference clock
               fhRefClkRate[uBoard]->Fill( (Double_t)((Double_t)(xScalerBoard->GetRefClk()) - (Double_t)(fvuLastRefClkCal[uBoard]) ) /
                                           dTimeDiff );
               fhRefClkRateEvo[uBoard]->Fill(dMbsTime - fdFirstMbsTime,
                     (Double_t)((Double_t)(xScalerBoard->GetRefClk()) - (Double_t)(fvuLastRefClkCal[uBoard]) )/
                     dTimeDiff );

//               LOG(info)<<"TMbsCalibScalTof::Calibration =>  => Scaler 2014 #"
//               <<uBoard
//               <<" "<<xScalerBoard->GetRefClk()<<" "<<fvuLastRefClkCal[uBoard]
//               <<" "<<(Double_t)(xScalerBoard->GetRefClk() - fvuLastRefClkCal[uBoard] )
//               <<" "<<dTimeDiff
//               <<" "<<(Double_t)(xScalerBoard->GetRefClk() - fvuLastRefClkCal[uBoard] )/dTimeDiff
//               ;
               // Try to update the clock frequency only if there are enough points in the monitoring histo, 
               // the distribution spread is reasonable and the option is ON
               if(  2000 < fhRefClkRate[uBoard]->GetEntries() && 
                   10000 < fhRefClkRate[uBoard]->GetRMS() && kTRUE == fMbsCalibPar->CalibRefClocks()
                  )
                  fvdMeanRefClkFreq[uBoard] = fhRefClkRate[uBoard]->GetMean();

//               fhRefClkRateEstA[uBoard]->Fill( dMbsTime - fdFirstMbsTime, fhRefClkRate[uBoard]->GetMean() );
//               fhRefClkRateEstB[uBoard]->Fill( dMbsTime - fdFirstMbsTime, fhRefClkRateEvo[uBoard]->GetMean(2) );
            } // if( 0 < fdPrevMbsTime ) 
            if( ( 0 == fvdPrevMbsTimeBd[uBoard] ) || ( fvdPrevMbsTimeBd[uBoard] + 0.05 < dMbsTime ) )
               fvdPrevMbsTimeBd[uBoard] = dMbsTime;

            // Save value of the reference clock in last rate update
//            fvuLastRefClkCal[uBoard] = xScalerBoard->GetRefClk();
         } // if( ( 0 == fdPrevMbsTime ) || ( fdPrevMbsTime + 0.05 < dMbsTime ) ) 
            
         if( 0 < fvdMeanRefClkFreq[uBoard] && kTRUE == fMbsCalibPar->CalibRefClocks() )
            dRefClkFreq = fvdMeanRefClkFreq[uBoard];
      } // if( 0 < fdPrevMbsTime )
   } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )

   if( ( kFALSE == bFirstEvent && uType != tofscaler::scaler2014) ||
       ( kFALSE == bFirstEvent2014 && uType == tofscaler::scaler2014) )
   {
      // If not first event, calculate the time since first and last event using the reference clock of the board
      Double_t dRefTime       = (Double_t)( (Double_t)(xScalerBoard->GetRefClk()) - (Double_t)(fvuFirstRefClk[uBoard])
                                           + (Double_t)(fvuRefClkCycle[uBoard])*4294967296. )/dRefClkFreq;

      Double_t dRefTimeToLast = (Double_t)( (Double_t)(xScalerBoard->GetRefClk()) - (Double_t)(fvuLastRefClk[uBoard]) )/dRefClkFreq;
      if( xScalerBoard->GetRefClk() < fvuLastRefClk[uBoard] )
         dRefTimeToLast = (Double_t)( (Double_t)(xScalerBoard->GetRefClk())  + 1 + (Double_t)(0xFFFFFFFF - fvuLastRefClk[uBoard]) )/dRefClkFreq;
                                           
      xCalibScaler->SetRefClk( (Double_t)( xScalerBoard->GetRefClk() )/dRefClkFreq );
      xCalibScaler->SetTimeToFirst( dRefTime );
      xCalibScaler->SetTimeToLast( dRefTimeToLast );
      
      if( 1e-6 < dRefTimeToLast )
      {
         for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
         {
            for( UInt_t uCh = 0; uCh < uNbChan; uCh++)
            {
               // for each board/scaler/channel, calculate the instantaneous rate between last event and this one
               // using the scaler count difference and the time since last event obtained with the reference clock
               UInt_t uCurrVal = xScalerBoard->GetScalerValue( uCh, uScaler);
               Double_t dRate = (Double_t)( (Double_t)(uCurrVal) - (Double_t)(fvuLastScalers[uBoard][uScaler][uCh]) ) / dRefTimeToLast;
               if( uCurrVal < fvuLastScalers[uBoard][uScaler][uCh] )
                  dRate = (Double_t)( (Double_t)(uCurrVal) + 1 + (Double_t)(0xFFFFFFFF - fvuLastScalers[uBoard][uScaler][uCh]) )
                         / dRefTimeToLast;
               xCalibScaler->SetScalerValue( uCh, dRate, uScaler);
//            fhScalersRateEvo[uBoard][uScaler][uCh]->Fill( dRefTime, uCurrVal - fvuLastScalers[uBoard][uScaler][uCh] );
               fvuLastScalers[uBoard][uScaler][uCh] = uCurrVal;
            } // for( UInt_t uCh = 0; uCh < uNbChan; uCh++)
         } // for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
      } // if( 0 < dRefTimeToLast )
      else
      {
         for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
         {
            for( UInt_t uCh = 0; uCh < uNbChan; uCh++)
            {
               // for each board/scaler/channel, calculate the instantaneous rate between last event and this one
               // using the scaler count difference and the time since last event obtained with the reference clock
               UInt_t uCurrVal = xScalerBoard->GetScalerValue( uCh, uScaler);
               Double_t dRate = 0.0;
               xCalibScaler->SetScalerValue( uCh, dRate, uScaler);
               fvuLastScalers[uBoard][uScaler][uCh] = uCurrVal;
            } // for( UInt_t uCh = 0; uCh < uNbChan; uCh++)
         } // for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
      } // else of if( 0 < dRefTimeToLast )
   } // if( kFALSE == bFirstEvent )

   fvuLastRefClk[uBoard] = xScalerBoard->GetRefClk();

   if( ( kTRUE == bFirstEvent && uType != tofscaler::scaler2014) ||
       ( kTRUE == bFirstEvent2014 && uType == tofscaler::scaler2014) )
   {
      fvuFirstRefClk[uBoard] = fvuLastRefClk[uBoard];
      fvuLastRefClkCal[uBoard] = fvuLastRefClk[uBoard];
      
      for( UInt_t uScaler = 0; uScaler < uNbScalers; uScaler++)
         for( UInt_t uCh = 0; uCh < uNbChan; uCh++) 
            fvuLastScalers[uBoard][uScaler][uCh] = xScalerBoard->GetScalerValue( uCh, uScaler);
   } // if( kTRUE == bFirstEvent )
   
   return kTRUE;
}

Bool_t TMbsCalibScalTof::CloseCalibration()
{
   // Loop over all scalers
   for( UInt_t uScalBdIndx = 0; uScalBdIndx < fvuLastScalers.size(); uScalBdIndx++ )
   {
      for( UInt_t uScaler = 0; uScaler < fvuLastScalers[uScalBdIndx].size(); uScaler++)
         fvuLastScalers[uScalBdIndx][uScaler].clear();
      fvuLastScalers[uScalBdIndx].clear();
   } // for( UInt_t uScalBdIndx = 0; uScalBdIndx < uNbScalBd; uScalBdIndx++ )
         
   if( 0 < fvuLastScalers.size() )
   {
      fvuLastScalers.clear();
      fvuFirstRefClk.clear();
      fvuLastRefClk.clear();
      fvdMeanRefClkFreq.clear();
   }
   return kTRUE;
}
// ------------------------------------------------------------------

