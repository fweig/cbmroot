// ------------------------------------------------------------------
// -----                    TMbsCalibTdcTof                     -----
// -----              Created 20/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TMbsCalibTdcTof.h"

// General Unpack headers
#include "TMbsUnpackTofPar.h"

// ToF specific headers
#include "TMbsCalibTofPar.h"
#include "TofTdcDef.h"
#include "TofCaenDef.h"
#include "TofVftxDef.h"
#include "TofTrbTdcDef.h"
#include "TofGet4Def.h"
#include "TTofTdcBoard.h"
#include "TTofTdcData.h"
#include "TTofCalibData.h"
// TRB-TDC specific headers
#include "TTofTrbTdcBoard.h"
#include "TTofTrbTdcData.h"

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
#include <TFile.h>

TMbsCalibTdcTof::TMbsCalibTdcTof() : 
   TObject(),
   fMbsUnpackPar(NULL),
   fMbsCalibPar(NULL),
   fbGsiSep14Fix(kFALSE),
   fiOffsetGsiSep14Fix(-1),
   fbTdcRefMoniMode(kFALSE),
   fCaenBoardCollection(NULL),
   fVftxBoardCollection(NULL),
   fTrb3BoardCollection(NULL),
   fGet4BoardCollection(NULL),
   fbSaveCalibTdcs(kFALSE),
   fCalibDataCollection(NULL),
   fileCalibrationIn(NULL),
   oldDir(NULL),
   fsCalibOutFoldername("./"),
   fsCalibFilename("")
{
   // Get Base Container
   FairRunAna* ana = FairRunAna::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   // Unpack parameter
   fMbsUnpackPar = (TMbsUnpackTofPar*) (rtdb->getContainer("TMbsUnpackTofPar"));
   if( 0 == fMbsUnpackPar )
      
   // Calibration parameter
   fMbsCalibPar = (TMbsCalibTofPar*) (rtdb->getContainer("TMbsCalibTofPar"));
   if( 0 == fMbsCalibPar )
      LOG(error)<<"TMbsCalibTdcTof::TMbsCalibTdcTof => Could not obtain the TMbsCalibTofPar ";
      else fMbsCalibPar->printParams();
}

TMbsCalibTdcTof::TMbsCalibTdcTof(TMbsUnpackTofPar * parIn, TMbsCalibTofPar *parCalIn) :
   TObject(),
   fMbsUnpackPar(parIn),
   fMbsCalibPar(parCalIn),
   fbGsiSep14Fix(kFALSE),
   fiOffsetGsiSep14Fix(-1),
   fbTdcRefMoniMode(kFALSE),
   fCaenBoardCollection(NULL),
   fVftxBoardCollection(NULL),
   fTrb3BoardCollection(NULL),
   fGet4BoardCollection(NULL),
   fbSaveCalibTdcs(kFALSE),
   fCalibDataCollection(NULL),
   fileCalibrationIn(NULL),
   oldDir(NULL),
   fsCalibOutFoldername("./"),
   fsCalibFilename("")
{
}
      
TMbsCalibTdcTof::~TMbsCalibTdcTof()
{
   DeleteHistograms();
   DeleteTotVariables();
   LOG(info)<<"**** TMbsCalibTdcTof: Delete instance ";
} 

void TMbsCalibTdcTof::Clear(Option_t */*option*/)
{
   fMbsUnpackPar = NULL;
   fMbsCalibPar = NULL;
   fCaenBoardCollection = NULL;
   fVftxBoardCollection = NULL;
   fTrb3BoardCollection = NULL;
   fGet4BoardCollection = NULL;
   fCalibDataCollection = NULL;
   fbSaveCalibTdcs = kFALSE;
   fsCalibOutFoldername = "./",
   fsCalibFilename = "";
}

// Calibration Functions
Bool_t TMbsCalibTdcTof::InitiTdcCalib()
{   
   if( kFALSE == InitCalibration() )
      return kFALSE;
      
   if( kFALSE == CreateTotVariables() )
      return kFALSE;
   
   if( kFALSE == GetHistosFromUnpack() )
      return kFALSE;

//   if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
//   {
      if( kFALSE == GetRefHistosFromUnpack() )
         return kFALSE;
//   } // if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
      
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::ClearCalib()
{
   ClearOutput();
   ClearTotVariables();
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::CalibTdc()
{
   // Clean event start
   ClearTdcReference();
   // Use only events with no corrupt data
   if( kFALSE == CheckAllTdcValid() )
      return kFALSE;

   // Loop over all defined TDC types
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // Calibrate first the reference channel of the main TDC for TDC-TDc offset calc.
      if( fMbsCalibPar->GetTdcOffsetMainTdc() < fMbsUnpackPar->GetNbActiveBoards( uType ) )
         CalibrateReference( uType, fMbsCalibPar->GetTdcOffsetMainTdc());

      // loop over all active TDCs
      for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
      {
         // Make Main TDC reference is not calibrated twice (would spoil the calibration stats)
         if( uTdc != fMbsCalibPar->GetTdcOffsetMainTdc() )
            CalibrateReference( uType, uTdc);

         if( kTRUE == fMbsCalibPar->GetTdcOffEnaFlag(uType, uTdc) )
         {
            TdcOffsetCalc( uType, uTdc);
         } // if( kTRUE == fMbsCalibPar->GetTdcOffEnaFlag(uType, uTdc) )

         // If TDC Ref channel monitoring mode, calibrate and 
         // fill histos only for reference channel
         if( kTRUE == fbTdcRefMoniMode )
            continue;

         Calibration( uType, uTdc);
      }
      
      // If TDC Ref channel monitoring mode, calibrate and 
      // fill histos only for reference channel
      if( kTRUE == fbTdcRefMoniMode )
         continue;
   
      // TOT mode 1 and 4 are already done.
      // TOT mode 2 time orders and builds TOT for each TDC inside the Calibration function
      if( 3 == fMbsCalibPar->GetTotMode( uType ) )
         // In the case where 1 input channel correspond to a TDC channel in 2 consecutive boards, 
         // we can time order the hits now that we have all boards calibrated
         // and then we will be able to associate the hits in full calibrated TDC data!
         BuildTotSplitBoards( uType );
   }
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::CloseTdcCalib()
{
   if( kTRUE == fMbsCalibPar->EnaCalibOutput() )
      WriteCalibrationFile();
   if( kTRUE == fMbsCalibPar->EnaSingleCalibOutput() )
      WriteSingleCalibrations();
      
   return kTRUE;
}
void  TMbsCalibTdcTof::SetCalibFilename( TString sFilenameIn )
{
   fsCalibFilename = sFilenameIn;
   if( "" == fsCalibFilename ) {
     LOG(info)<<"TMbsCalibTdcTof => Use default name generation from current time for the TDC calibration file output";
   } else {
     LOG(info)<<"TMbsCalibTdcTof => New name for the TDC calibration file output: "
              << Form("%sTofTdcCalibHistos_%s.root", fsCalibOutFoldername.Data(), fsCalibFilename.Data() );
   }
}
void  TMbsCalibTdcTof::SetCalibOutFolder( TString sFoldernameIn )
{
   fsCalibOutFoldername = sFoldernameIn;
   if( "./" == fsCalibOutFoldername ) {
     LOG(info)<<"TMbsCalibTdcTof => Use current folder for the TDC calibration file output";
   } else {
     LOG(info)<<"TMbsCalibTdcTof => New folder for the TDC calibration file output: "
              << Form("%sTofTdcCalibHistos_%s.root", fsCalibOutFoldername.Data(), fsCalibFilename.Data() );
   }
}
Bool_t TMbsCalibTdcTof::InitParameters()
{
   return kTRUE;
}
// ------------------------------------------------------------------
Bool_t TMbsCalibTdcTof::RegisterInput()
{
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( toftdc::caenV1290 ) )
   {
      fCaenBoardCollection = (TClonesArray*) rootMgr->GetObject("TofCaenTdc");
      if( NULL == fCaenBoardCollection)
      {
         LOG(error)<<"TMbsCalibTdcTof::RegisterInput => Could not get the TofCaenTdc TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fCaenBoardCollection)
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )

   if( 0 < fMbsUnpackPar->GetNbActiveBoards( toftdc::vftx ) )
   {
      fVftxBoardCollection = (TClonesArray*) rootMgr->GetObject("TofVftxTdc");
      if( NULL == fVftxBoardCollection) 
      {
         LOG(error)<<"TMbsCalibTdcTof::RegisterInput => Could not get the TofVftxTdc TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fVftxBoardCollection) 
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( toftdc::trb ) )
   {
      fTrb3BoardCollection = (TClonesArray*) rootMgr->GetObject("TofTrbTdc");
      if( NULL == fTrb3BoardCollection)
      {
         LOG(error)<<"TMbsCalibTdcTof::RegisterInput => Could not get the TofTrb3Tdc TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fTrb3BoardCollection)
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )

   if( 0 < fMbsUnpackPar->GetNbActiveBoards( toftdc::get4 ) )
   {
      fGet4BoardCollection = (TClonesArray*) rootMgr->GetObject("TofGet4Tdc");
      if( NULL == fGet4BoardCollection)
      {
         LOG(error)<<"TMbsCalibTdcTof::RegisterInput => Could not get the TofGet4Tdc TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fGet4BoardCollection)
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::RegisterOutput()
{
   FairRootManager* rootMgr = FairRootManager::Instance();
   fCalibDataCollection = new TClonesArray("TTofCalibData");

   rootMgr->Register( "TofCalibData","Tof",fCalibDataCollection,
                      fMbsUnpackPar->WriteDataInCbmOut() || fbSaveCalibTdcs);
   /*
   fCalibTdcTrigCollection = new TClonesArray("Double_t");
   rootMgr->Register("TofCalibTdcTrigg","Tof",fCalibTdcTrigCollection, kTRUE);
   */
   
   // Prepare the saving of the reference channel time (trigger)
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // TODO: for now only TRB has reference channel methods implemented
      if( toftdc::caenV1290 == uType ||
           toftdc::vftx == uType ||
           toftdc::get4 == uType ||
           toftdc::undef == uType )
            continue;

      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {
         fCalibRefCollection[uType] = new TClonesArray( "TTofCalibData", fMbsUnpackPar->GetNbActiveBoards( uType ) );
         rootMgr->Register( Form("TofCalibRef%s",toftdc::ksTdcParName[ uType ].Data() ),
                            "TofCalib", fCalibRefCollection[uType],
                            fMbsUnpackPar->WriteDataInCbmOut() || fbSaveCalibTdcs );
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::CheckAllTdcValid()
{
   // Loop over all TDC types
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
      {
         // TDC type specific values
         TClonesArray * xUnpDataArray = NULL;
         switch( uType )
         {
            case toftdc::caenV1290:
               xUnpDataArray = fCaenBoardCollection;
               break;
            case toftdc::vftx:
               xUnpDataArray = fVftxBoardCollection;
               break;
            case toftdc::trb:
               xUnpDataArray = fTrb3BoardCollection;
               break;
            case toftdc::get4:
               xUnpDataArray = fGet4BoardCollection;
               break;
            default:
               break;
         } // switch( uType )
         
         if( NULL == xUnpDataArray )
         {
            LOG(error)<<"TMbsCalibTdcTof::CheckAllTdcValid => Data pt failed Type "<<uType
                  <<" Active bd "<<fMbsUnpackPar->GetNbActiveBoards( uType )
                  <<" Board "<<uTdc<<" Unp array "<<xUnpDataArray;
            return kFALSE;
         }
         
         // Board raw data recovery
         TTofTdcBoard * xBoardUnpData = (TTofTdcBoard*) xUnpDataArray->At(uTdc);
               
         if( NULL == xBoardUnpData )
         {
            fdBoardTriggerTime[uType][uTdc] = 0.0;
            LOG(error)<<"TMbsCalibTdcTof::CheckAllTdcValid => Bd pt failed Type "<<uType
                  <<" Active bd "<<fMbsUnpackPar->GetNbActiveBoards( uType )
                  <<" Board "<<uTdc;
            return kFALSE;
         } // if( NULL == xBoardUnpData )

         if( kFALSE == xBoardUnpData->IsValid() )
         {
            fdBoardTriggerTime[uType][uTdc] = 0.0;
            LOG(debug)<<"TMbsCalibTdcTof::CheckAllTdcValid => Bd data invalid Type "<<uType
                  <<" Active bds "<<fMbsUnpackPar->GetNbActiveBoards( uType )
                  <<" Board "<<uTdc;
            return kFALSE;
         } // if( kFALSE == xBoardUnpData->IsValid() )
      } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::ClearOutput()
{
   fCalibDataCollection->Clear("C");

   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // TODO: for now only TRB has reference channel methods implemented
      if( toftdc::caenV1290 == uType ||
           toftdc::vftx == uType ||
           toftdc::get4 == uType ||
           toftdc::undef == uType )
            continue;

      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
         fCalibRefCollection[uType]->Clear("C");
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )

   return kTRUE;
}
void   TMbsCalibTdcTof::SetSaveTdcs( Bool_t bSaveTdcs )
{
   fbSaveCalibTdcs = bSaveTdcs;
   if( kTRUE == bSaveTdcs ) {
     LOG(info)<<"TMbsCalibTdcTof => Enable the saving of calibrated tdc data in analysis output file";
   } else {
     LOG(info)<<"TMbsCalibTdcTof => Disable the saving of calibrated tdc data in analysis output file";
   }
}
// ------------------------------------------------------------------
Bool_t TMbsCalibTdcTof::CreateHistogramms()
{
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !

   // Loop over all TDC types
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {    
         UInt_t   uNbChan     = 0;
         Double_t dClockCycle = 0.0;
         UInt_t   uFtBinNb    = 0;
         UInt_t   uMaxMul     = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan     = caentdc::kuNbChan;
               dClockCycle = caentdc::kdClockCycleSize;
               uFtBinNb    = caentdc::kiFineTime + 1;
               uMaxMul     = toftdc::kuDefNbMulti; // TODO: add proper value for this type
               break;
            case toftdc::vftx:
               uNbChan     = vftxtdc::kuNbChan;
               dClockCycle = vftxtdc::kdClockCycleSize;
               uFtBinNb    = vftxtdc::kiFifoFineTime + 1;
               uMaxMul     = vftxtdc::kuNbMulti;
               break;
            case toftdc::trb:
               uNbChan     = trbtdc::kuNbChan;
               dClockCycle = trbtdc::kdClockCycleSize;
               uFtBinNb    = trbtdc::kiFineCounterSize;
               uMaxMul     = toftdc::kuDefNbMulti; // TODO: add proper value for this type
               break;
            case toftdc::get4:
               uNbChan     = get4tdc::kuNbChan;
               dClockCycle = get4tdc::kdClockCycleSize;
               uFtBinNb    = get4tdc::kiFineTime + 1;
               uMaxMul     = toftdc::kuDefNbMulti; // TODO: add proper value for this type
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
         
         // Calibration variable initialization
         fhDnlChan[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbChan, NULL );
         fhDnlSum[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbChan, NULL );
         fhBinSizeChan[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbChan, NULL );
         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
            for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
            {
               Int_t iHistoIndex = uTdc*uNbChan + uChanInd;
               
               // Current Dnl correction per bin
               fhDnlChan[uType][iHistoIndex] =  new TH1D( 
                                 Form("tof_%s_dnlch_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(),
                                                                   uTdc, uChanInd),
                                 Form("Current Dnl factor for channel %3u in %s TDC #%03u", uChanInd, 
                                 toftdc::ksTdcHistName[ uType ].Data(), uTdc),
                                 uFtBinNb, -0.5, uFtBinNb-0.5 );
               
               // Current Dnl Sum per bin
               fhDnlSum[uType][iHistoIndex] =  new TH1D( 
                                 Form("tof_%s_dnlsum_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(),
                                                                    uTdc, uChanInd),
                                 Form("Current Dnl Sum for channel %3u in %s TDC #%03u; Bin[]; ", uChanInd,
                                 toftdc::ksTdcHistName[ uType ].Data(), uTdc),
                                 uFtBinNb, -0.5, uFtBinNb-0.5 );

               // Current Bin sizes from Dnl correction
               fhBinSizeChan[uType][iHistoIndex] =  new TH1D(
                                 Form("tof_%s_binszch_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(),
                                                                   uTdc, uChanInd),
                                 Form("Current Bin size from Dnl factor for channel %3u in %s TDC #%03u; Bin size [ps]; Bins []",
                                       uChanInd, toftdc::ksTdcHistName[ uType ].Data(), uTdc),
                                 (Int_t)(20*dClockCycle/uFtBinNb), -0.5, 20*dClockCycle/uFtBinNb -0.5 );
            } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
            
         // Monitoring histograms
         UInt_t uNbDataChan = uNbChan;
            // If we are using the 1 input chan = 2 tdc chan tot mode, the number of channels in 
            // calibrated data is halved!
         if( 2 == fMbsCalibPar->GetTotMode(uType) )
            uNbDataChan /= 2;
            
         if( kTRUE == fMbsCalibPar->IsTimeHistEna() )
            fhTimeToTrigg[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbDataChan, NULL );
         if( kTRUE == fMbsCalibPar->IsSingleTimeHistEna() )
            fhTimeToTriggSingles[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbDataChan, NULL );
         if( kTRUE == fMbsCalibPar->IsTotHistEna() && 0 < fMbsCalibPar->GetTotMode(uType) )
            fhToT[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbDataChan, NULL );
         if( kTRUE == fMbsCalibPar->IsMultiDistHistEna() )
            fhMultiDist[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbDataChan, NULL );
            
         fhMultiplicity[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ), NULL );
         
         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         {
            for( UInt_t uChanInd = 0; uChanInd< uNbDataChan; uChanInd++) 
            {
               Int_t iHistoIndex = uTdc*uNbDataChan + uChanInd;
               
               // Time to board trigger in ps
               if( kTRUE == fMbsCalibPar->IsTimeHistEna() )
                  fhTimeToTrigg[uType][iHistoIndex] =  new TH1I( 
                                    Form("tof_%s_t2trig_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(),
                                                                      uTdc, uChanInd),
                                    Form("Time to board trigger for channel %3u in %s TDC #%03u; Trigger time - Calibrated Time [ps]", 
                                          uChanInd, toftdc::ksTdcHistName[ uType ].Data(), uTdc),
                                    11000, -2000000.0, 200000.0 );
//                                    8000, -700000.0, 100000.0 );
               // Time to board trigger in ps for events with a single hit in the channel
               if( kTRUE == fMbsCalibPar->IsSingleTimeHistEna() )
                  fhTimeToTriggSingles[uType][iHistoIndex] =  new TH1I( 
                                    Form("tof_%s_t2trig_sing_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(),
                                                                      uTdc, uChanInd),
                                    Form("Time to board trigger for events with a single hit for channel %3u in %s TDC #%03u; Trigger time - Calibrated Time [ps]", 
                                          uChanInd, toftdc::ksTdcHistName[ uType ].Data(), uTdc),
                                    8000, -700000.0, 100000.0 );
               // Tot in ps
               if( kTRUE == fMbsCalibPar->IsTotHistEna() && 0 < fMbsCalibPar->GetTotMode(uType)  )
                  fhToT[uType][iHistoIndex] =  new TH1I( 
                                    Form("tof_%s_tot_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(),
                                                                      uTdc, uChanInd),
                                    Form("Time over threshold for channel %3d in %s TDC #%03u; ToT [ps]", 
                                          uChanInd, toftdc::ksTdcHistName[ uType ].Data(), uTdc),
                                    30000, -150000, 150000 );
                                    
               // Distance between consecutive multiple hits on same channel in ps
               if( kTRUE == fMbsCalibPar->IsMultiDistHistEna() )
                  fhMultiDist[uType][iHistoIndex] =  new TH2I( 
                                    Form("tof_%s_muldist_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(),
                                                                      uTdc, uChanInd),
                                    Form("Time to previous hit for multiple hits for channel %3u in %s TDC #%03u; T(n) - T(n-1) [ps]; n (Multiple hits index) []", 
                                          uChanInd, toftdc::ksTdcHistName[ uType ].Data(), uTdc),
                                    7500, -50000, 100000,
                                    vftxtdc::kuNbMulti, 1, vftxtdc::kuNbMulti + 1 );
                                    // TODO: why VFTX values here?
            } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
            
            //Multiplicity
            fhMultiplicity[uType][uTdc] =  new TH2I( 
                                    Form("tof_%s_mul_b%03u", toftdc::ksTdcHistName[ uType ].Data(), uTdc),
                                    Form("Data multiplicty per channel in %s TDC #%03u; Channel []; Multiplicity []", 
                                          toftdc::ksTdcHistName[ uType ].Data(), uTdc),
                                    uNbDataChan, 0.0, uNbDataChan,
                                    uMaxMul + 1, 0, uMaxMul + 1 );
         } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )

//   if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
//   {
      if( kFALSE == CreateReferenceHistogramms() )
      {
         gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
         return kFALSE;
      }
//   } // if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )

   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::FillHistograms()
{
   TTofCalibData * fCalibData;
   
   // Use only events with no corrupt data
   if( kFALSE == CheckAllTdcValid() )
      return kFALSE;
   
   LOG(debug)<<"TMbsCalibTdcTof::FillHistograms => "<<fCalibDataCollection->GetEntriesFast()
            <<" data unpacked & calibrated successfully in this event!";
            
   if( kTRUE == fbTdcRefMoniMode )
   {
      // If TDC Ref channel monitoring mode, calibrate and 
      // fill histos only for reference channel
      if( kFALSE == FillReferenceHistograms() )
         return kFALSE;
         else return kTRUE;
   } // if( kTRUE == fbTdcRefMoniMode )

   UInt_t uNbChan[ toftdc::NbTdcTypes];
   uNbChan[ toftdc::undef ]     = 0;
   uNbChan[ toftdc::caenV1290 ] = caentdc::kuNbChan;
   uNbChan[ toftdc::vftx ]      = vftxtdc::kuNbChan;
   uNbChan[ toftdc::trb ]      = trbtdc::kuNbChan;
   uNbChan[ toftdc::get4 ]      = get4tdc::kuNbChan;
   if( 4 == fMbsCalibPar->GetTotMode( toftdc::get4 ) )
      uNbChan[ toftdc::get4 ]  = get4v10::kuNbChan;
   
   std::vector< UInt_t >    uMul[ toftdc::NbTdcTypes ];
   std::vector< Double_t >  dLastTime[ toftdc::NbTdcTypes ];
   
   // If we are using the 1 input chan = 2 tdc chan tot mode, the number of channels in 
   // calibrated data is halved!
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      if( 2 == fMbsCalibPar->GetTotMode(uType) )
         uNbChan[ uType ] /= 2;
      uMul[ uType ].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbChan[uType] );
      dLastTime[ uType ].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbChan[uType] );
   }
        
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
      
      Int_t iHistoIndex = uTdc*uNbChan[uType] + uChan;
      
         // Time to board trigger in ps
      if( kTRUE == fMbsCalibPar->IsTimeHistEna() )
         fhTimeToTrigg[uType][iHistoIndex]->Fill( dTime - fdBoardTriggerTime[uType][uTdc] );
         // Tot in ps
      if( kTRUE == fMbsCalibPar->IsTotHistEna() )
         fhToT[uType][iHistoIndex]->Fill( dTot );
         // Distance between consecutive multiple hits on same channel in ps
      if( kTRUE == fMbsCalibPar->IsMultiDistHistEna() && 0 < uMul[ uType ][ iHistoIndex ] )
         fhMultiDist[uType][iHistoIndex]->Fill( dTime - dLastTime[ uType ][ iHistoIndex ], uMul[ uType ][ iHistoIndex ] );
      
      // Multi hits data
      uMul[ uType ][ iHistoIndex ]++;
      dLastTime[ uType ][ iHistoIndex ] = dTime;
      
      TString sTemp = Form( "TMbsCalibTdcTof::FillHistos: Data #%04d Type %s Board #%03d Chan %3d Time %7.0f Tot %7.0f Edge %1d",
               iDataIndex,
               toftdc::ksTdcHistName[ fCalibData->GetType() ].Data(), 
//               "Missing",
               fCalibData->GetBoard(), fCalibData->GetChannel(), fCalibData->GetTime(), 
               fCalibData->GetTot(), fCalibData->GetEdge() );
      LOG(debug)<<sTemp;
   } // for( Int_t iDataIndex = 0; iDataIndex < fVftxBoard[iBoardIndex]->GetDataNb() ; iDataIndex++ )
   
   // Time to board trigger in ps for events with a single hit in the channel
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
      {
         TString sTemp = Form( "TMbsCalibTdcTof::FillHistos: %s Board #%03u Trigger Time %7.0f ",
                  toftdc::ksTdcHistName[ uType ].Data(), uTdc, fdBoardTriggerTime[uType][uTdc] );
         LOG(debug)<<sTemp;
         for( UInt_t uChanInd = 0; uChanInd< uNbChan[uType]; uChanInd++) 
         {
            fhMultiplicity[uType][uTdc]->Fill(uChanInd, uMul[uType][ uTdc*uNbChan[uType] + uChanInd ] );
            if( 1 == uMul[uType][ uTdc*uNbChan[uType] + uChanInd ] &&
                kTRUE == fMbsCalibPar->IsSingleTimeHistEna() )
            {
               Int_t iHistoIndex = uTdc*uNbChan[uType] + uChanInd;
               fhTimeToTriggSingles[uType][iHistoIndex]->Fill( dLastTime[uType][iHistoIndex]
                                                             - fdBoardTriggerTime[uType][uTdc] );
            } // if( 1 == uMul[uType][ uTdc*uNbChan[uType] + uChanInd ] )
         } // for( UInt_t uChanInd = 0; uChanInd< uNbChan[uType]; uChanInd++) 
      } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
   

//   if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
//   {
      if( kFALSE == FillReferenceHistograms() )
         return kFALSE;
//   } // if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::WriteHistogramms( TDirectory* inDir)
{
   TDirectory * oldir = gDirectory;
   
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {
         UInt_t uNbChan  = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan  = caentdc::kuNbChan;
               break;
            case toftdc::vftx:
               uNbChan  = vftxtdc::kuNbChan;
               break;
            case toftdc::trb:
               uNbChan  = trbtdc::kuNbChan;
               break;
            case toftdc::get4:
               uNbChan  = get4tdc::kuNbChan;
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )

         // create a subdirectory "Cal_type" in this file
         TDirectory *cdCal = inDir->mkdir( Form( "Cal_%s", toftdc::ksTdcHistName[ uType ].Data() ) );
         cdCal->cd();    // make the "Cal_type" directory the current directory
         TDirectory *cdCalTdc[ fMbsUnpackPar->GetNbActiveBoards( uType ) ];
         TDirectory *cdCalTdcHist[ fMbsUnpackPar->GetNbActiveBoards( uType ) ][3];
         
         // loop over active TDCs
         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         {
            // Create a sub folder for each TDC
            cdCalTdc[uTdc] = cdCal->mkdir( Form( "cTdc%03u", uTdc) );
            cdCalTdc[uTdc]->cd();
            
            // Create a sub folder for each histogram type
            cdCalTdcHist[uTdc][0] = cdCalTdc[uTdc]->mkdir( Form( "dnl%03u", uTdc) );
            cdCalTdcHist[uTdc][1] = cdCalTdc[uTdc]->mkdir( Form( "dnlSum%03u", uTdc) );
            cdCalTdcHist[uTdc][2] = cdCalTdc[uTdc]->mkdir( Form( "binSz%03u", uTdc) );
            
            // Loop over all TDC channels
            for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
            {
               Int_t iHistoIndex = uTdc*uNbChan + uChanInd;
               
               // Current Dnl correction per bin
               cdCalTdcHist[uTdc][0]->cd();
               fhDnlChan[uType][iHistoIndex]->Write();
               
               // Current Dnl Sum per bin
               cdCalTdcHist[uTdc][1]->cd();
               fhDnlSum[uType][iHistoIndex]->Write();

               // Current Bin sizes from Dnl correction
               cdCalTdcHist[uTdc][2]->cd();
               fhBinSizeChan[uType][iHistoIndex]->Write();
            } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
         } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
            
         // Monitoring histograms
         UInt_t uNbDataChan = uNbChan;
            // If we are using the 1 input chan = 2 tdc chan tot mode, the number of channels in 
            // calibrated data is halved!
         if( 2 == fMbsCalibPar->GetTotMode(uType) )
            uNbDataChan /= 2;
            
         // create a subdirectory "Mon_Type" in this file
         TDirectory *cdMon = inDir->mkdir( Form( "Mon_%s", toftdc::ksTdcHistName[ uType ].Data() ) );
         cdMon->cd();    // make the "Mon_type" directory the current directory
         TDirectory *cdMonTdc[ fMbsUnpackPar->GetNbActiveBoards( uType ) ];
         TDirectory *cdMonTdcHist[ fMbsUnpackPar->GetNbActiveBoards( uType ) ][4];
         
         // loop over active TDCs
         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         {
            // Create a sub folder for each TDC
            cdMonTdc[uTdc] = cdMon->mkdir( Form( "mTdc%03u", uTdc) );
            cdMonTdc[uTdc]->cd();
            
            fhMultiplicity[uType][uTdc]->Write();
            
            // Create a sub folder for each histogram type
            if( kTRUE == fMbsCalibPar->IsTimeHistEna() )
               cdMonTdcHist[uTdc][0] = cdMonTdc[uTdc]->mkdir( Form( "t2trig%03u", uTdc) );
            if( kTRUE == fMbsCalibPar->IsSingleTimeHistEna() )
               cdMonTdcHist[uTdc][1] = cdMonTdc[uTdc]->mkdir( Form( "t2trig_sing%03u", uTdc) );
            if( kTRUE == fMbsCalibPar->IsTotHistEna() )
               cdMonTdcHist[uTdc][2] = cdMonTdc[uTdc]->mkdir( Form( "tot%03u", uTdc) );
            if( kTRUE == fMbsCalibPar->IsMultiDistHistEna() )
               cdMonTdcHist[uTdc][3] = cdMonTdc[uTdc]->mkdir( Form( "mulDist%03u", uTdc) );
            
            // Loop over all input channels
            for( UInt_t uChanInd = 0; uChanInd< uNbDataChan; uChanInd++) 
            {
               Int_t iHistoIndex = uTdc*uNbDataChan + uChanInd;
               
               if( kTRUE == fMbsCalibPar->IsTimeHistEna() )
               {
                  cdMonTdcHist[uTdc][0]->cd();
                  fhTimeToTrigg[uType][iHistoIndex]->Write();
               } // if( kTRUE == fMbsCalibPar->IsTimeHistEna() )
                  
               if( kTRUE == fMbsCalibPar->IsSingleTimeHistEna() )
               {
                  cdMonTdcHist[uTdc][1]->cd();
                  fhTimeToTriggSingles[uType][iHistoIndex]->Write();
               } // if( kTRUE == fMbsCalibPar->IsSingleTimeHistEna() )
                  
               if( kTRUE == fMbsCalibPar->IsTotHistEna() )
               {
                  cdMonTdcHist[uTdc][2]->cd();
                  fhToT[uType][iHistoIndex]->Write();
               } // if( kTRUE == fMbsCalibPar->IsTotHistEna() )
                  
               if( kTRUE == fMbsCalibPar->IsMultiDistHistEna() )
               {
                  cdMonTdcHist[uTdc][3]->cd();
                  fhMultiDist[uType][iHistoIndex]->Write();
               } // if( kTRUE == fMbsCalibPar->IsMultiDistHistEna() )
            } // for( UInt_t uChanInd = 0; uChanInd< uNbDataChan[uType]; uChanInd++) 
         } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      
 //  if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
 //  {
      if( kFALSE == WriteReferenceHistogramms( inDir ) )
         return kFALSE;
//   } // if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )

   gDirectory->cd( oldir->GetPath() );
   
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::DeleteHistograms()
{
//   if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
//   {
      if( kFALSE == DeleteReferenceHistograms( ) )
         return kFALSE;
//   } // if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )

   return kTRUE;
}
// ------------------------------------------------------------------
Bool_t TMbsCalibTdcTof::InitCalibration()
{
   // Loop over all TDC types
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {    
         UInt_t uNbChan  = 0;
         UInt_t uFtBinNb = 0;
         Int_t  iFtLinCalMin = 0;
         Int_t  iFtLinCalMax = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan  = caentdc::kuNbChan;
               uFtBinNb = caentdc::kiFineTime + 1;
               break;
            case toftdc::vftx:
               uNbChan  = vftxtdc::kuNbChan;
               uFtBinNb = vftxtdc::kiFifoFineTime + 1;
               break;
            case toftdc::trb:
               uNbChan  = trbtdc::kuNbChan;
               uFtBinNb = trbtdc::kiFineCounterSize;
               iFtLinCalMin = trbtdc::kuSimpleFineTimeMinValue;
               iFtLinCalMax = trbtdc::kuSimpleFineTimeMinValue;
               break;
            case toftdc::get4:
               uNbChan  = get4tdc::kuNbChan;
               uFtBinNb = get4tdc::kiFineTime + 1;
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
         
         // Calibration variable initialization
         fiNbHitsForCalib[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbChan, 0 );
         fbCalibAvailable[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbChan, kFALSE );
         fdCorr[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbChan );
         fdBoardTriggerTime[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ), 0.0 );

         // TDC offsets variables calibration
         fhInitialCalibHistoRef[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ), 0 );
         fiLinCalRefMinBin[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ), iFtLinCalMin );
         fiLinCalRefMaxBin[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ), iFtLinCalMax );
         fdCorrRef[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) );
         fdTdcReference[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ), 0.0 );
         fdTdcOffsets[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ), 0.0 );
         
         fbFirstEventPassedRef[uType] = kFALSE;
         fdTdcReferenceFirstEvent[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ), 0.0 );

         // loop over all active TDC & all channels
         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         {
            for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
            {
               Int_t iHistoIndex = uTdc*uNbChan + uChanInd;
               fdCorr[uType][iHistoIndex].resize(uFtBinNb, 0.0);
            } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 

            fdCorrRef[uType][uTdc].resize( uFtBinNb, 0.0 );
         } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   
   if( kTRUE == fMbsCalibPar->EnaSingleCalib() )
      LoadSingleCalibrations();
      else LoadCalibrationFile();
      
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::GetHistosFromUnpack()
{
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !
   
   // Loop over all TDC types
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {    
         UInt_t uNbChan  = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan  = caentdc::kuNbChan;
               break;
            case toftdc::vftx:
               uNbChan  = vftxtdc::kuNbChan;
               break;
            case toftdc::trb:
               uNbChan  = trbtdc::kuNbChan;
               break;
            case toftdc::get4:
               uNbChan  = get4tdc::kuNbChan;
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
         
         fhFineTime[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) * uNbChan );
         
         TString sInfoLoading = "Got FineTime histograms from unpack step for following "+
                                toftdc::ksTdcHistName[ uType ] + " TDC channels:";
         LOG(info)<<sInfoLoading;
         
         sInfoLoading = "          ";
         for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
            sInfoLoading += Form("%3u ", uChanInd);
         LOG(info)<<sInfoLoading;
            
         // Loop over all active TDCs and all channels
         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         {
            sInfoLoading = Form("tdc #%3u: ",uTdc );
            for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
            {
               Int_t iHistoIndex = uTdc*uNbChan + uChanInd;
//               gDirectory->GetObject( Form("tof_%s_ft_b%03d_ch%03d", toftdc::ksTdcHistName[ uType ].Data(), 
//                                                                     uTdc, uChanInd), 
//                                       fhFineTime[uType][iHistoIndex]);

               if( 0 == fMbsCalibPar->GetMinHitCalib() )
                  if( NULL != fhInitialCalibHisto[uType][iHistoIndex] )
                  {
                     // Only initial calibration should be used and the calibration histogram was found
                     // => no need to access unpack histogram!
                     fhFineTime[uType][iHistoIndex] = NULL;
                     sInfoLoading += "  - ";
                     continue;
                  } // if( NULL != fhInitialCalibHisto[uType][iHistoIndex] )

               fhFineTime[uType][iHistoIndex] = (TH1*) gDirectory->FindObject( Form("tof_%s_ft_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(), 
                                                                     uTdc, uChanInd));
               if( NULL == fhFineTime[uType][iHistoIndex] )
               {
                  LOG(error)<<" TMbsCalibTdcTof::GetHistosFromUnpack => Could not get FT histo for "
                            <<toftdc::ksTdcHistName[ uType ]<<" #"
                            <<uTdc<<" ch "<<uChanInd<<" from the unpack step";
                  sInfoLoading += "  0 ";
                  LOG(info)<<sInfoLoading;

                  gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
                  return kFALSE;
               } // if( NULL == fhFineTime[uType][iHistoIndex] )
                  else LOG(debug)<<" TMbsCalibTdcTof::GetHistosFromUnpack =>Got FT histo for "
                                  <<toftdc::ksTdcHistName[ uType ]<<" #"
                                  <<uTdc<<" ch "<<uChanInd<<" from the unpack step: 0x"
                                  <<fhFineTime[uType][iHistoIndex]<<" "
                                  <<fhFineTime[uType][iHistoIndex]->GetEntries();
               sInfoLoading += "  1 ";
            } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
            LOG(info)<<sInfoLoading;
         } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
         else LOG(info)<<" TMbsCalibTdcTof::GetHistosFromUnpack => no boards for "<<toftdc::ksTdcHistName[ uType ];
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   
   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
   
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::LoadCalibrationFile()
{
   // First check if the inital calibration file name is properly defined
   if(  kTRUE != fMbsCalibPar->GetInitCalFilename().EqualTo("") &&
        kTRUE != fMbsCalibPar->GetInitCalFilename().EqualTo("-") )
   {
      // Save online ROOT directory as the File opening auto change current Dir
      oldDir = gDirectory;

      TString sInitialCalibHistoName = "";
      fileCalibrationIn = new TFile( fMbsCalibPar->GetInitCalFilename(), "READ");

      if( kTRUE == fileCalibrationIn->IsOpen() )
      {
         // Loop over all TDC types
         for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
         {
            if( 0 == fMbsCalibPar->GetNbCalibBoards( uType ) ||
                0 == fMbsUnpackPar->GetNbActiveBoards( uType ) )
               continue;
               
            UInt_t uNbChan  = 0;
            UInt_t uFtBinNb = 0;
            
            switch( uType )
            {
               case toftdc::caenV1290:
                  uNbChan  = caentdc::kuNbChan;
                  uFtBinNb = caentdc::kiFineTime + 1;
                  break;
               case toftdc::vftx:
                  uNbChan  = vftxtdc::kuNbChan;
                  uFtBinNb = vftxtdc::kiFifoFineTime + 1;
                  break;
               case toftdc::trb:
                  uNbChan  = trbtdc::kuNbChan;
                  uFtBinNb = trbtdc::kiFineCounterSize;
                  break;
               case toftdc::get4:
                  uNbChan  = get4tdc::kuNbChan;
                  uFtBinNb = get4tdc::kiFineTime + 1;
                  if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                     uNbChan  = get4v10::kuNbChan;
                  break;
               default:
                  break;
            } // switch( uType )
            if( 0 == uNbChan || 1 == uFtBinNb )
            {
               LOG(info)<<"TMbsCalibTdcTof::LoadCalibrationFile Undefined tdc parameters for type "
                         <<toftdc::ksTdcHistName[ uType ];
               LOG(info)<<" => No initial calib loading!";
               continue;
            }
            // TODO: Does not handle the TRB case!
            // One could replace trbtdc::kiFineCounterSize everywhere by trbtdc::time_FineTimeBitMask + 1
            // to handle this case like every other TDC
         
            TString sInfoLoading = "Loaded initial calibration histograms for following "+
                                   toftdc::ksTdcHistName[ uType ] + " TDC channels:";
            LOG(info)<<sInfoLoading;
            
            sInfoLoading = "          ";
            for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
               sInfoLoading += Form("%3u ", uChanInd);
            LOG(info)<<sInfoLoading;

            //define correction variables
            fhInitialCalibHisto[ uType ].resize( fMbsCalibPar->GetNbCalibBoards( uType ) *
                                                 uNbChan, NULL );
/*            Bool_t bincontrol[uFtBinNb];*/ // Commented out to remove warning bec. unused
/*            Int_t iSum[fMbsCalibPar->GetNbCalibBoards( uType )][uNbChan];*/ // Commented out to remove warning bec. unused

            // Temp variable to store pointer on calib histo in File
            TH1* fInitialCalibHistoFromFile = 0;

            for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
            {
               if( -1 < fMbsCalibPar->GetInitialCalInd( uType, uTdc ) )
               {
                  sInfoLoading = Form("tdc #%3u: ",uTdc );
                  for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
                  {
                     Int_t iHistoIndex = uTdc*uNbChan + uChanInd;
                     
                     // Initialize pointer to NULL
                     fInitialCalibHistoFromFile = NULL;

                     // Find histogram in file and store its pointer in a temp variable
                     sInitialCalibHistoName = Form("tof_%s_ft_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(), 
                                                   fMbsCalibPar->GetInitialCalInd( uType, uTdc ), uChanInd );
                     
                     fileCalibrationIn->GetObject( sInitialCalibHistoName, fInitialCalibHistoFromFile);

                     if( NULL == fInitialCalibHistoFromFile )
                        sInfoLoading += "  0 ";
                        else
                        {
                           sInfoLoading += "  1 ";
                           // Clone the found histo and move it to online ROOT directory instead of File
                           fhInitialCalibHisto[ uType ][ iHistoIndex ] = (TH1*)fInitialCalibHistoFromFile->Clone( 
                                                   Form("%s_CalibFile", sInitialCalibHistoName.Data() ) );
                           fhInitialCalibHisto[ uType ][ iHistoIndex ]->SetDirectory( oldDir );
                        } // else of if( 0 == fInitialCalibHistoFromFile )
                        
                     // Extract calibration factors from initialization histo
                     CalibFactorsInit( uType, uTdc, uChanInd );
                  } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
                  LOG(info)<<sInfoLoading;
               } // if( -1 < fMbsCalibPar->GetInitialCalInd( uType, uTdc ) )
            } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
         
         // File closing and going back to online ROOT folder
         fileCalibrationIn->Close();
         gDirectory->Cd(oldDir->GetPath());
      } // if( kTRUE == fileCalibrationIn->IsOpen() )
         else
         {
            LOG(info)<<"Could not open "<<fMbsCalibPar->GetInitCalFilename()
                     <<" to load initial TDC calibration, please check setting in Calibration option file";
            for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
               if( 0 == fMbsCalibPar->GetNbCalibBoards( uType ) ||
                   0 == fMbsUnpackPar->GetNbActiveBoards( uType ) )
                  continue;
               else
               {
                  UInt_t uNbChan  = 0;
            
                  switch( uType )
                  {
                     case toftdc::caenV1290:
                        uNbChan  = caentdc::kuNbChan;
                        break;
                     case toftdc::vftx:
                        uNbChan  = vftxtdc::kuNbChan;
                        break;
                     case toftdc::trb:
                        uNbChan  = trbtdc::kuNbChan;
                        break;
                     case toftdc::get4:
                        uNbChan  = get4tdc::kuNbChan;
                        break;
                     default:
                        break;
                  } // switch( uType )
                  if( 0 == uNbChan )
                  {
                     LOG(info)<<"TMbsCalibTdcTof::LoadCalibrationFile Undefined tdc parameters for type "
                              <<toftdc::ksTdcHistName[ uType ];
                     LOG(info)<<" => No initial calib loading!";
                     continue;
                  }
                  fhInitialCalibHisto[ uType ].resize( fMbsCalibPar->GetNbCalibBoards( uType ) *
                                                       uNbChan, NULL );
               }
            return kFALSE;
         } // else of if( kTRUE == fileCalibrationIn->IsOpen() )
   } // if(  sInitialCalibrationFilename OK and 1 == fMbsCalibPar->uEnableCalib)
   
      
//   if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
//   {
      if( kFALSE == LoadCalibrationFileRef( ) )
         return kFALSE;
//   } // if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::LoadSingleCalibrations()
{
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      if( 0 == fMbsCalibPar->GetNbCalibBoards( uType ) ||
          0 == fMbsUnpackPar->GetNbActiveBoards( uType ) )
         continue;
         
      UInt_t uNbChan  = 0;
      UInt_t uFtBinNb = 0;
      
      switch( uType )
      {
         case toftdc::caenV1290:
            uNbChan  = caentdc::kuNbChan;
            uFtBinNb = caentdc::kiFineTime + 1;
            break;
         case toftdc::vftx:
            uNbChan  = vftxtdc::kuNbChan;
            uFtBinNb = vftxtdc::kiFifoFineTime + 1;
            break;
         case toftdc::trb:
            uNbChan  = trbtdc::kuNbChan;
            uFtBinNb = trbtdc::kiFineCounterSize;
            break;
         case toftdc::get4:
            uNbChan  = get4tdc::kuNbChan;
            uFtBinNb = get4tdc::kiFineTime + 1;
            if( 4 == fMbsCalibPar->GetTotMode( uType ) )
               uNbChan  = get4v10::kuNbChan;
            break;
         default:
            break;
      } // switch( uType )
      if( 0 == uNbChan || 1 == uFtBinNb )
      {
         LOG(info)<<"TMbsCalibTdcTof::LoadSingleCalibrations Undefined tdc parameters for type "
                  <<toftdc::ksTdcHistName[ uType ];;
         LOG(info)<<" => No initial calib loading!";
         continue;
      }
      // TODO: Does not handle the TRB case!
      // One could replace trbtdc::kiFineCounterSize everywhere by trbtdc::time_FineTimeBitMask + 1
      // to handle this case like every other TDC

      TString sInfoLoading = "Loaded initial calibration histograms for following "+
                             toftdc::ksTdcHistName[ uType ] + " TDC channels:";
      LOG(info)<<sInfoLoading;
   
      sInfoLoading = "          ";
      for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
         sInfoLoading += Form("%3u ", uChanInd);
      LOG(info)<<sInfoLoading;

      //define correction variables
      fhInitialCalibHisto[ uType ].resize( fMbsCalibPar->GetNbCalibBoards( uType ) *
                                           uNbChan, NULL );
/*      Bool_t bincontrol[uFtBinNb];*/ // Commented out to remove warning bec. unused
/*      Int_t iSum[fMbsCalibPar->GetNbCalibBoards( uType )][uNbChan];*/ // Commented out to remove warning bec. unused

      // Save online ROOT directory as the File opening auto change current Dir
      oldDir = gDirectory;

      for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         if( -1 < fMbsCalibPar->GetInitialCalInd( uType, uTdc ) )
         {
            sInfoLoading = Form("tdc #%3u: ",uTdc );
            for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
            {
               TString sInitialCalibFileName = Form("./calib/%s_Tdc%03dChan%03u.root ", toftdc::ksTdcHistName[ uType ].Data(),
                                                      fMbsCalibPar->GetInitialCalInd( uType, uTdc ), uChanInd);
               TString sInitialCalibHistoName = "";
               fileCalibrationIn = new TFile( sInitialCalibFileName, "READ");
               if( kTRUE == fileCalibrationIn->IsOpen() )
               {
                  // Initialize pointer to NULL
                  TH1* fInitialCalibHistoFromFile = NULL;
                  sInitialCalibHistoName = Form("tof_%s_ft_b%03d_ch%03u", toftdc::ksTdcHistName[ uType ].Data(), 
                                                fMbsCalibPar->GetInitialCalInd( uType, uTdc ), uChanInd );
                        
                  fileCalibrationIn->GetObject( sInitialCalibHistoName, fInitialCalibHistoFromFile);

                  Int_t iHistoIndex = uTdc*uNbChan + uChanInd;
                  
                  if( NULL == fInitialCalibHistoFromFile )
                     sInfoLoading += "  0 ";
                     else
                     {
                        sInfoLoading += "  1 ";
                        fhInitialCalibHisto[ uType ][ iHistoIndex ] = (TH1*)fInitialCalibHistoFromFile->Clone(
                                                      Form("%s_CalibFile", sInitialCalibHistoName.Data() ) );
                        fhInitialCalibHisto[ uType ][ iHistoIndex ]->SetDirectory( oldDir );
                        
                        // Extract calibration factors from initialization histo
                        CalibFactorsInit( uType, uTdc, uChanInd );
                     } // else of if( NULL == fInitialCalibHistoFromFile )
                  fileCalibrationIn->Close();
               } // if( kTRUE == fileCalibrationIn->IsOpen() )
                  else sInfoLoading += "  0 ";
            } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++) 
            LOG(info)<<sInfoLoading;
         } // if( -1 < fMbsCalibPar->GetInitialCalInd( uType, uTdc ) )
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   
//   if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
//   {
      if( kFALSE == LoadSingleCalibrationsRef( ) )
         return kFALSE;
//   } // if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::CalibFactorsInit( UInt_t uType, UInt_t uBoard, UInt_t uChan )
{
   UInt_t   uNbChan     = 0;
   Double_t dClockCycle = 0.0;
   UInt_t   uFtBinNb    = 0;
   
   switch( uType )
   {
      case toftdc::caenV1290:
         uNbChan     = caentdc::kuNbChan;
         dClockCycle = caentdc::kdClockCycleSize;
         uFtBinNb    = caentdc::kiFineTime + 1;
         break;
      case toftdc::vftx:
         uNbChan     = vftxtdc::kuNbChan;
         dClockCycle = vftxtdc::kdClockCycleSize;
         uFtBinNb    = vftxtdc::kiFifoFineTime + 1;
         break;
      case toftdc::trb:
         uNbChan     = trbtdc::kuNbChan;
         dClockCycle = trbtdc::kdClockCycleSize;
         uFtBinNb    = trbtdc::kiFineCounterSize;
         break;
      case toftdc::get4:
         uNbChan     = get4tdc::kuNbChan;
         dClockCycle = get4tdc::kdClockCycleSize;
         uFtBinNb    = get4tdc::kiFineTime + 1;
         if( 4 == fMbsCalibPar->GetTotMode( uType ) )
            uNbChan  = get4v10::kuNbChan;
         break;
      default:
         break;
   } // switch( uType )
   
   Int_t iHistoIndex      = uBoard*uNbChan + uChan;
   
   if( NULL != fhInitialCalibHisto[ uType ][ iHistoIndex ])
   {
      // Reset Histos with DNL ploting
      fhDnlChan[uType][iHistoIndex]->Reset();
      fhDnlSum[uType][iHistoIndex]->Reset();
      fhBinSizeChan[uType][iHistoIndex]->Reset();
      
      //set the sum to zero before summing over all bins in one channel
      Bool_t bincontrol[uFtBinNb];
      Int_t iSum = 0;
      
      Double_t dTotalEntriesInHisto = (Double_t)(fhInitialCalibHisto[ uType ][ iHistoIndex ]->GetEntries() );

      for(Int_t iBin = 0; iBin < (Int_t)uFtBinNb; iBin++)
      {
         Int_t iBinContent = (Int_t)(fhInitialCalibHisto[ uType ][ iHistoIndex ]->GetBinContent(iBin+1));
         //Looking for the used bins
         if( iBinContent <= 0)
            bincontrol[iBin] = kFALSE;
         else if( iBinContent > 0)
            bincontrol[iBin] = kTRUE;

         // build the sum of all bin content
         if(bincontrol[iBin])
         {
            iSum = iSum + iBinContent;
            fdCorr[uType][iHistoIndex][iBin] = (Double_t)iSum / dTotalEntriesInHisto;
            fhDnlChan[uType][iHistoIndex]->Fill(iBin, (Double_t)iBinContent/dTotalEntriesInHisto );
            fhBinSizeChan[uType][iHistoIndex]->Fill( dClockCycle*(Double_t)iBinContent/dTotalEntriesInHisto );
         } // if(bincontrol[iBin])
            else
            {
               if( 0 < iBin)
                  fdCorr[uType][iHistoIndex][iBin] = fdCorr[uType][iHistoIndex][iBin-1];
                  else fdCorr[uType][iHistoIndex][iBin] = 0.0;
            } // else of if(bincontrol[iBin])
         fhDnlSum[uType][iHistoIndex]->Fill(iBin, fdCorr[uType][iHistoIndex][iBin] );
      } // for(Int_t iBin=0; iBin < (Int_t)uFtBinNb; iBin++)
      fbCalibAvailable[uType][iHistoIndex] = kTRUE;
   } // if( NULL != fInitialCalibHisto[uTdc][uChanInd])
      else return kFALSE;
      
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::Calibration( UInt_t uType, UInt_t uBoard)
{
   LOG(debug)<<"TMbsCalibTdcTof::Calibration => Type "<<uType
         <<" Active bd "<<fMbsUnpackPar->GetNbActiveBoards( uType )
         <<" Board "<<uBoard;
   if( uBoard < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   {
      // TDC type specific values
      UInt_t         uNbChan      = 0;
      TClonesArray * xUnpDataArray = NULL;
      Double_t       dClockCycle  = 0.0;
      Bool_t         bInvertFt    = kFALSE;
      Double_t       dTotBinToPs  = 0.0;
      Int_t          iCoarseSize  = 0;
      Int_t          iCoarseOfLim = 0;
      switch( uType )
      {
         case toftdc::caenV1290:
            uNbChan       = caentdc::kuNbChan;
            xUnpDataArray = fCaenBoardCollection;
            dClockCycle   = caentdc::kdClockCycleSize;
            bInvertFt     = caentdc::kbInvertFt;
            iCoarseSize   = caentdc::kiCoarseCounterSize;
            iCoarseOfLim  = caentdc::kuCoarseOverflowTest;
            break;
         case toftdc::vftx:
            uNbChan       = vftxtdc::kuNbChan;
            xUnpDataArray = fVftxBoardCollection;
            dClockCycle   = vftxtdc::kdClockCycleSize;
            bInvertFt     = vftxtdc::kbInvertFt;
            iCoarseSize   = vftxtdc::kiCoarseCounterSize;
            iCoarseOfLim  = vftxtdc::kuCoarseOverflowTest;
            break;
         case toftdc::trb:
            uNbChan       = trbtdc::kuNbChan;
            xUnpDataArray = fTrb3BoardCollection;
            dClockCycle   = trbtdc::kdClockCycleSize;
            bInvertFt     = trbtdc::kbInvertFt;
            iCoarseSize   = trbtdc::kiCoarseCounterSize;
            iCoarseOfLim  = trbtdc::kuCoarseOverflowTest;
            break;
         case toftdc::get4:
            uNbChan       = get4tdc::kuNbChan;
            xUnpDataArray = fGet4BoardCollection;
            dClockCycle   = get4tdc::kdClockCycleSize;
            bInvertFt     = get4tdc::kbInvertFt;
            dTotBinToPs   = get4tdc::kdTotBinSize;
            iCoarseSize   = get4tdc::kiCoarseCounterSize;
            iCoarseOfLim  = get4tdc::kuCoarseOverflowTest;
            if( 4 == fMbsCalibPar->GetTotMode( uType ) )
               uNbChan  = get4v10::kuNbChan;
            break;
         default:
            break;
      } // switch( uType )
      
      if( NULL == xUnpDataArray || NULL == fCalibDataCollection )
      {
         LOG(error)<<"TMbsCalibTdcTof::Calibration => Data pt failed Type "<<uType
               <<" Active bd "<<fMbsUnpackPar->GetNbActiveBoards( uType )
               <<" Board "<<uBoard<<" Unp array "<<xUnpDataArray<<" CalibDataColl "<<fCalibDataCollection;
         return kFALSE;
      }
      
      // Board raw data recovery
      TTofTdcBoard * xBoardUnpData = (TTofTdcBoard*) xUnpDataArray->At(uBoard);
            
      if( NULL == xBoardUnpData )
      {
         fdBoardTriggerTime[uType][uBoard] = 0.0;
         LOG(error)<<"TMbsCalibTdcTof::Calibration => Bd pt failed Type "<<uType
               <<" Active bd "<<fMbsUnpackPar->GetNbActiveBoards( uType )
               <<" Board "<<uBoard;
         return kFALSE;
      } // if( NULL == xBoardUnpData )

      if( kFALSE == xBoardUnpData->IsValid() )
      {
         fdBoardTriggerTime[uType][uBoard] = 0.0;
         LOG(debug)<<"TMbsCalibTdcTof::Calibration => Bd data invalid Type "<<uType
               <<" Active bds "<<fMbsUnpackPar->GetNbActiveBoards( uType )
               <<" Board "<<uBoard;
         return kFALSE;
      } // if( kFALSE == xBoardUnpData->IsValid() )

      // TOT variables preparation
         // TOT mode 1 and 4 don't need intermediate storage for time ordering
         // TOT mode 3 needs all boards to have been processed before time ordering of calibrated data and 
         // Tot building.
         // Only TOT mode 2 needs time ordering of a single board data
      if( 2 == fMbsCalibPar->GetTotMode( uType ) )
         xTempCalibData[uType]->Clear("C");
      
      // Trigger time
      Int_t iTriggerCoarseTime = xBoardUnpData->GetTriggerTime();
//      fdBoardTriggerTime[uType][uBoard] = iTriggerCoarseTime * dClockCycle;
      // TODO: board time corrections => board to board offset, Offsets between different tdc types, etc...
      // Partially done for TRB by using channel 0.
      Int_t iMainTriggerCoarseTime = 0;
      if( NULL != (TTofTdcBoard*) xUnpDataArray->At(fMbsCalibPar->GetTdcOffsetMainTdc()) )
         if( kTRUE == ( (TTofTdcBoard*) xUnpDataArray->At(fMbsCalibPar->GetTdcOffsetMainTdc()) )->IsValid() )
         {
            iMainTriggerCoarseTime = ((TTofTdcBoard*) xUnpDataArray->At(
                                       fMbsCalibPar->GetTdcOffsetMainTdc() ))->GetTriggerTime();

            // Fix for big trigger time deviation in GSI Sep14 data
            // The TDCs in the TRB3 boards had a big deviation in time counter origin relative
            // to the FPGA TDC small boards.
            // Here we try to add to the offset and additional offset for the boards
            // which are too far away from the main TDC time origin.
            // For this we use the trigger time of the first board in this case and in the first event.
            // Hopefully this is enough to avoid introducing different offsets for different boards/events.
            // Problem: the boards are jumping around
            if( kTRUE == fbGsiSep14Fix && toftdc::trb == uType && 0 < iTriggerCoarseTime )
               if( ( iTriggerCoarseTime - iMainTriggerCoarseTime > 1000000 ) ||
                   ( iTriggerCoarseTime - iMainTriggerCoarseTime < -1000000 ) ) // deviation > ~5ms
               {
                  std::stringstream ss;
                  ss <<"TMbsCalibTdcTof::Calibration Type "<<uType<<" Board "<<uBoard
                        <<" Instead of Main Trigger coarse time "<<iMainTriggerCoarseTime;

                  if( fiOffsetGsiSep14Fix < 0 )
                     fiOffsetGsiSep14Fix = iTriggerCoarseTime - iMainTriggerCoarseTime;

                  Int_t iFurtherOffset = fiOffsetGsiSep14Fix + iMainTriggerCoarseTime;

                  ss <<" Use Main Trigger coarse time "<<iFurtherOffset
                     <<" ("<< ((iFurtherOffset& 0x7FFFFE00) - (iMainTriggerCoarseTime& 0x7FFFFE00))<<")";
                  LOG(debug) << ss.str();

                  iMainTriggerCoarseTime += fiOffsetGsiSep14Fix;

                  if( ( (iMainTriggerCoarseTime - iTriggerCoarseTime) > 1000000) ||
                      ( (iMainTriggerCoarseTime - iTriggerCoarseTime) < -1000000)  )
                  {
                     LOG(info)<<"TMbsCalibTdcTof::Calibration Type "<<uType<<" Board "<<uBoard
                           <<" Trigger time used for offset may have jumped "<<iMainTriggerCoarseTime - iTriggerCoarseTime
                           <<" in event ";
                     // Try to update the common offset => may lead to multiple peaks in time difference
                     iMainTriggerCoarseTime -= fiOffsetGsiSep14Fix; // Remove the offset we just added
                     fiOffsetGsiSep14Fix = iTriggerCoarseTime - iMainTriggerCoarseTime; // compute new offset
                     iMainTriggerCoarseTime += fiOffsetGsiSep14Fix; // apply this offset
                  }

               } // if( iTriggerCoarseTime - iMainTriggerCoarseTime > 100000 ) // deviation > ~500us

         } // if( kTRUE == ( (TTofTdcBoard*) xUnpDataArray->At(fMbsCalibPar->GetTdcOffsetMainTdc()) )->IsValid() )
      // In order to avoid big numbers after conversion to double float
      // (precision loss), we subtract part of the main board trigger time counter from calib hits
      // Use for now 128 clock cycle (~600ns for trb)
      // Therefore we need to do the same to "calib" trigger time for plotting
      if( kTRUE == fMbsCalibPar->UseCoarse() ) 
//         fdBoardTriggerTime[uType][uBoard] = ( iTriggerCoarseTime - (iMainTriggerCoarseTime & 0x7FFFFF80) ) 
         fdBoardTriggerTime[uType][uBoard] = ( iTriggerCoarseTime - (iMainTriggerCoarseTime & 0x7FFFFFFF) ) 
                                             * dClockCycle;
         else fdBoardTriggerTime[uType][uBoard] = iTriggerCoarseTime * dClockCycle;

      LOG(debug)<<"TMbsCalibTdcTof::Calibration Type "<<uType<<" Board "<<uBoard
                <<" Main Trigger coarse time "<<iMainTriggerCoarseTime;
      
      // Data calibration
      Int_t iNbUnpData = xBoardUnpData->GetDataNb();
      TTofTdcData   * xUnpDataPtr;
/*      TTofCalibData * xCalData;*/ // Commented out to remove warning bec. unused

      LOG(debug)<<"TMbsCalibTdcTof::Calibration Type "<<uType<<" Board "<<uBoard<<" N Data "<<iNbUnpData;
         // Loop over unpacked data
      for( Int_t iUnpDataInd = 0; iUnpDataInd < iNbUnpData; iUnpDataInd++ )
      {
//         xCalData.Clear();
         xUnpDataPtr = xBoardUnpData->GetDataPtr(iUnpDataInd);
         
         Int_t iHistoIndex      = uBoard*uNbChan + xUnpDataPtr->GetChannel();
         
         // Check if the calibration factors are available (either initial or new data)
         if( kFALSE == fbCalibAvailable[uType][iHistoIndex] )
            continue;
         
         Int_t    iCoarseTime = xUnpDataPtr->GetCoarseTime();
         if( kTRUE == fMbsCalibPar->UseCoarse() ) 
         {
            // Test Coarse counter Overflow
            if( iCoarseOfLim < iCoarseTime - iTriggerCoarseTime  )
               // trigger -- OverflowLimit --> hit 
               //=> if right order probably: hit ... Overflow ... trigger
               iCoarseTime -= iCoarseSize;
            else if( iCoarseOfLim < iTriggerCoarseTime - iCoarseTime )
               // hit -- OverflowLimit --> trigger 
               //=> if right order probably: trigger ... Overflow ... hit 
               iCoarseTime += iCoarseSize;

            // In order to avoid big numbers after conversion to double float
            // (precision loss), subtract part of the main board trigger time counter
            // Use for now 128 clock cycle (~600ns for trb)
//            iCoarseTime -= iMainTriggerCoarseTime & 0x7FFFFF80;
            iCoarseTime -= iMainTriggerCoarseTime & 0x7FFFFFFF;

            if( iCoarseTime < -1000000 || 1000000 < iCoarseTime ) {
               std::stringstream ss;
               ss <<"TMbsCalibTdcTof::Calibration Type "<<uType<<" Board "<<uBoard<<" Chan "<< xUnpDataPtr->GetChannel()
                  <<" Trigger time used for offset may have jumped "<<iTriggerCoarseTime
                  <<" "<<iMainTriggerCoarseTime<<" "<<fiOffsetGsiSep14Fix
                  <<" "<<iMainTriggerCoarseTime - iTriggerCoarseTime
                  << "\n"
                  <<" data coarse  "<< iCoarseTime;
               LOG(debug) << ss.str();
            }
         } // if( kTRUE == fMbsCalibPar->UseCoarse()
         
         Double_t dCalibTime = dClockCycle * iCoarseTime;
         if( kTRUE == bInvertFt )
            dCalibTime += dClockCycle *( 1 - fdCorr[uType][iHistoIndex][xUnpDataPtr->GetFineTime()] );
            else dCalibTime += dClockCycle * fdCorr[uType][iHistoIndex][xUnpDataPtr->GetFineTime()];


         // ToT calculation
         // TODO: multiple consecutive Rising edges
         // TODO: Time difference with coarse counter overflow check/safety for Tot
         Double_t dTot  = 0.0;
         UInt_t   uEdge = 0;
         switch( fMbsCalibPar->GetTotMode( uType ) )
         {
            case 1:
            {
               Int_t  iDataIndex = uBoard*uNbChan + xUnpDataPtr->GetChannel();
               UInt_t uChan = xUnpDataPtr->GetChannel();
               if( fMbsCalibPar->GetTotInvFlag( uType, uBoard, uChan ) == static_cast<Int_t>(xUnpDataPtr->GetEdge()) )
               {
                  if( NULL != xTofTdcDataPrevArray[uType]->At( iDataIndex ) )
                     ((TTofCalibData*)(xTofTdcDataPrevArray[uType]->At( iDataIndex )))->Clear();
                  // rising edge
                  new((*xTofTdcDataPrevArray[uType])[ iDataIndex ]) 
                     TTofCalibData( uType, uBoard, uChan, dCalibTime , dTot, uEdge );
               } // if( 0 == xUnpDataPtr->GetEdge() )
                  else if( NULL != xTofTdcDataPrevArray[uType]->At( iDataIndex ) )
                  {
                     TTofCalibData* dataRisingEdge = (TTofCalibData*)(xTofTdcDataPrevArray[uType]->At( iDataIndex ) );
                     if( kTRUE == dataRisingEdge->IsFilled() )
                     {
                        Double_t dDataTime = dataRisingEdge->GetTime();
                        
                        // falling edge & rising edge present & filled
                        dTot = dataRisingEdge->GetTimeDifference( dCalibTime );
                        
                        // Time offset
                        if( kTRUE == fMbsCalibPar->EnaTimeOffset() )
                           dDataTime -= fMbsCalibPar->GetTimeOffsetVal( uType, uBoard, uChan );
                        
                        // TDC to TDC offset
//                        if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
                        if( kTRUE == fMbsCalibPar->GetTdcOffEnaFlag(uType, uBoard) )
                           dDataTime -= fdTdcOffsets[uType][uBoard];
                           
                        // Tot offset
                        // Tot offset signe inverted if rising and falling inverted
                        if( kTRUE == fMbsCalibPar->EnaTimeOffset() )
                           dTot -= fMbsCalibPar->GetTotOffsetVal( uType, uBoard, uChan ) * 
                                   ( 1 == fMbsCalibPar->GetTotInvFlag( uType, uBoard, uChan ) ? -1 : 1 );
                                   
                        // Use time from rising edge and newly calculated tot to create the hit
                        new((*fCalibDataCollection)[ fCalibDataCollection->GetEntriesFast() ]) 
                           TTofCalibData( uType, uBoard, uChan, 
                                          dDataTime, 
                                          dTot, 2 );
                     } // if( kTRUE == (xTofTdcDataPrevArray[uType]->At( iDataIndex ) )->IsFilled() )
                  } // else if( NULL != xTofTdcDataPrevArray[uType]->At( iDataIndex ) )
               break;
            } // encapsulation needed because of iDataIndex presence/initialization in multiple cases
            case 2:
            {
               UInt_t uInputChan = ( xUnpDataPtr->GetChannel() - xUnpDataPtr->GetChannel()%2 ) / 2;
               
               // Save data for later time ordering
               Int_t iDataIndex = xTempCalibData[uType]->GetEntriesFast();
               uEdge = ( xUnpDataPtr->GetChannel()%2 + 1 + fMbsCalibPar->GetTotInvFlag( uType, uBoard, uInputChan ) )%2;

                // Tot offset signe inverted if rising and falling inverted
               if( 1 == uEdge && kTRUE == fMbsCalibPar->EnaTotOffset() )
                  // TODO: For now use the value of TOT offsets as just a way to make sure the TOT is positive, 
                  // to change once the real offset are measured with pulser
                  dCalibTime -= fMbsCalibPar->GetTotOffsetVal( uType, uBoard, uInputChan );
                  // TODO: Change to enable the use of measured TOT offset
//                  dCalibTime -= fMbsCalibPar->GetTotOffsetVal( uType, uBoard, uInputChan ) * 
//                                ( 1 == fMbsCalibPar->GetTotInvFlag( uType, uBoard, uInputChan ) ? -1 : 1 );

               new((*xTempCalibData[uType])[ iDataIndex ]) 
                  TTofCalibData( uType, uBoard, uInputChan, dCalibTime , 0.0, 
                                 uEdge );
               break;
            } // encapsulation needed because of iDataIndex presence/initialization in multiple cases
            case 3:
            {
               // TODO: make it independent of board order! 
               //       (time ordering is done in unpacker only inside a board => fail with inverted board order)
               // TODO: board to board offset
               UInt_t uChan = xUnpDataPtr->GetChannel();
               
               // Save data for later time ordering
               Int_t iDataIndex = xTempCalibData[uType]->GetEntriesFast();
               uEdge = ( uBoard%2 + fMbsCalibPar->GetTotInvFlag( uType, uBoard, uChan ) )%2;
               new((*xTempCalibData[uType])[ iDataIndex ]) 
                  TTofCalibData( uType, uBoard%2, uChan, dCalibTime , 0.0, 
                                 uEdge );
               break;
            } // encapsulation needed because of iDataIndex presence/initialization in multiple cases
            case 4:
               dTot = xUnpDataPtr->GetTot() * dTotBinToPs;
               // Offset of input channel time
               if( kTRUE == fMbsCalibPar->EnaTimeOffset() )
                  dCalibTime -= fMbsCalibPar->GetTimeOffsetVal( uType, uBoard, xUnpDataPtr->GetChannel() );

               // TDC to TDC offset
               if( kTRUE == fMbsCalibPar->GetTdcOffEnaFlag(uType, uBoard) )
//               if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
                  dCalibTime -= fdTdcOffsets[uType][uBoard];

               // Offset between rising and falling edge => Tot offset
               if( kTRUE == fMbsCalibPar->EnaTotOffset() )
                  dTot -= fMbsCalibPar->GetTotOffsetVal( uType, uBoard, xUnpDataPtr->GetChannel() );
                  
               new((*fCalibDataCollection)[ fCalibDataCollection->GetEntriesFast() ]) 
                  TTofCalibData( uType, uBoard, xUnpDataPtr->GetChannel(), dCalibTime , dTot, 2 );
               break;
            case 0:
            default :
            {
               uEdge = xUnpDataPtr->GetEdge();
               
               if( kTRUE == fMbsCalibPar->EnaTimeOffset() )
               {
                  // Time offset applies to input channel, not TDC channel!
                  UInt_t uInputChan;
                  switch( uType )
                  {
                     case toftdc::vftx:
                     case toftdc::trb:
                        // 2 TDC channel per input channel
                        uInputChan = ( xUnpDataPtr->GetChannel() - xUnpDataPtr->GetChannel()%2 ) / 2;
                        break;
                     case toftdc::caenV1290:
                     case toftdc::get4:
                     default:
                        // 1 TDC channel per input channel
                        uInputChan = xUnpDataPtr->GetChannel();
                        break;
                  } // switch( uType )
                  dCalibTime -= fMbsCalibPar->GetTimeOffsetVal( uType, uBoard, uInputChan );
               } // if( kTRUE == fMbsCalibPar->EnaTimeOffset() )
                        
               // TDC to TDC offset
               if( kTRUE == fMbsCalibPar->GetTdcOffEnaFlag(uType, uBoard) )
//               if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
                  dCalibTime -= fdTdcOffsets[uType][uBoard];

               // Not Tot => no Tot offset!!
            
               // For now always save      
               new((*fCalibDataCollection)[ fCalibDataCollection->GetEntriesFast() ]) 
                  TTofCalibData( uType, uBoard, xUnpDataPtr->GetChannel(), dCalibTime , dTot, uEdge );
               break;
            } // encapsulation needed because of uInputChan presence/initialization in multiple cases
         } // switch( fMbsCalibPar->GetTotMode() )
      } // for( Int_t iUnpDataInd = 0; iUnpDataInd < iNbUnpData; iUnpDataInd++ )
      
      // TOT mode 1 and 4 are already done.
      // TOT mode 3 needs all boards to have been processed before time ordering of calibrated data and 
      // Tot building.
      if( 2 == fMbsCalibPar->GetTotMode( uType ) )
      {
         // In the case where 1 input channel correspond to 2 TDC channels, 
         // we can time order the hits now that we have all of them calibrated
         // and then we will be able to associate them in full calibrated TDC data!
         BuildTotSplitChannels( uType, uBoard );
      } // if( 2 == fMbsCalibPar->GetTotMode( uType ) )
      
      // Recalibration if necessary/possible
         // Check if initial histos array was created
      Bool_t bBoardInitialThere = kFALSE;
      if( fhInitialCalibHisto[ uType ].size() == fMbsUnpackPar->GetNbActiveBoards( uType )*uNbChan )
         bBoardInitialThere = kTRUE;
         
         // loop over channels
      for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
      {
         Int_t iHistoIndex      = uBoard*uNbChan + uChanInd;
         
         if( 0 < fMbsCalibPar->GetMinHitCalib() )
         {
            Int_t iNbHitsThisCh = fhFineTime[uType][iHistoIndex]->GetEntries();
            
            // Check if initial calibration histograms are available
            Bool_t bInitialThere = kFALSE;
            if( kTRUE == bBoardInitialThere )
               if( NULL != fhInitialCalibHisto[ uType ][ iHistoIndex ] )
                  bInitialThere = kTRUE;
            
            // Check whether we should use the initial calibration on top of the new data
            Bool_t bUseInitial = bInitialThere;
            if( 0 < fMbsCalibPar->GetMinHitCalibNewOnly() &&
                fMbsCalibPar->GetMinHitCalibNewOnly() <= iNbHitsThisCh )
                bUseInitial = kFALSE;
                  
            if( 0 < iNbHitsThisCh &&
                0 == iNbHitsThisCh % fMbsCalibPar->GetMinHitCalib() )
               CalibFactorsCalc( uType, uBoard, uChanInd, bUseInitial);
         } // if( 0 < fMbsCalibPar->GetMinHitCalib() )
      } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      else return kFALSE;
      
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::CalibFactorsCalc( UInt_t uType, UInt_t uBoard, UInt_t uChan, Bool_t bWithInitial )
{
   UInt_t   uNbChan     = 0;
   Double_t dClockCycle = 0.0;
   UInt_t   uFtBinNb    = 0;
   
   switch( uType )
   {
      case toftdc::caenV1290:
         uNbChan     = caentdc::kuNbChan;
         dClockCycle = caentdc::kdClockCycleSize;
         uFtBinNb    = caentdc::kiFineTime + 1;
         break;
      case toftdc::vftx:
         uNbChan     = vftxtdc::kuNbChan;
         dClockCycle = vftxtdc::kdClockCycleSize;
         uFtBinNb    = vftxtdc::kiFifoFineTime + 1;
         break;
      case toftdc::trb:
         uNbChan     = trbtdc::kuNbChan;
         dClockCycle = trbtdc::kdClockCycleSize;
         uFtBinNb    = trbtdc::kiFineCounterSize;
         break;
      case toftdc::get4:
         uNbChan     = get4tdc::kuNbChan;
         dClockCycle = get4tdc::kdClockCycleSize;
         uFtBinNb    = get4tdc::kiFineTime + 1;
         if( 4 == fMbsCalibPar->GetTotMode( uType ) )
            uNbChan  = get4v10::kuNbChan;
         break;
      default:
         break;
   } // switch( uType )
   Int_t iHistoIndex      = uBoard*uNbChan + uChan;
   
   // define correction variables
   // & set the sum to zero before summing over all bins in one channel
   Bool_t bincontrol[uFtBinNb];
   Int_t iSum = 0;
   
   if( kTRUE == bWithInitial )
   {
      // Use both initial calibration histograms and new data
      // To extract the correction factors
      if( NULL != fhInitialCalibHisto[ uType ][ iHistoIndex ])
      {
         // Reset Histos with DNL ploting
         fhDnlChan[uType][iHistoIndex]->Reset();
         fhDnlSum[uType][iHistoIndex]->Reset();
         fhBinSizeChan[uType][iHistoIndex]->Reset();
         
         Double_t dTotalEntriesInHisto = (Double_t)(fhFineTime[ uType ][ iHistoIndex ]->GetEntries() ) + 
                                         (Double_t)(fhInitialCalibHisto[ uType ][ iHistoIndex ]->GetEntries() );

         for(Int_t iBin = 0; iBin < (Int_t)uFtBinNb; iBin++)
         {
            Int_t iBinContent = (Int_t)(fhFineTime[ uType ][ iHistoIndex ]->GetBinContent(iBin+1)) +
                                (Int_t)(fhInitialCalibHisto[ uType ][ iHistoIndex ]->GetBinContent(iBin+1));
            //Looking for the used bins
            if( iBinContent <= 0)
               bincontrol[iBin] = kFALSE;
            else if( iBinContent > 0)
               bincontrol[iBin] = kTRUE;

            // build the sum of all bin content
            if(bincontrol[iBin])
            {
               iSum = iSum + iBinContent;
               fdCorr[uType][iHistoIndex][iBin] = (Double_t)iSum / dTotalEntriesInHisto;
               fhDnlChan[uType][iHistoIndex]->Fill(iBin, (Double_t)iBinContent/dTotalEntriesInHisto );
               fhBinSizeChan[uType][iHistoIndex]->Fill( dClockCycle*(Double_t)iBinContent/dTotalEntriesInHisto );
            } // if(bincontrol[iBin])
               else
               {
                  if( 0 < iBin)
                     fdCorr[uType][iHistoIndex][iBin] = fdCorr[uType][iHistoIndex][iBin-1];
                     else fdCorr[uType][iHistoIndex][iBin] = 0.0;
               } // else of if(bincontrol[iBin])
            fhDnlSum[uType][iHistoIndex]->Fill(iBin, fdCorr[uType][iHistoIndex][iBin] );
         } // for(Int_t iBin=0; iBin < (Int_t)uFtBinNb; iBin++)
         fbCalibAvailable[uType][iHistoIndex] = kTRUE;
         return kTRUE;
      } // if( NULL != fhInitialCalibHisto[ uType ][ iHistoIndex ])
   } // if( kTRUE == bWithInitial )
   
   // If either choice to use only new data or initial histo missing
   // => Use only new data to extract the correction factors
   
   // Reset Histos with DNL ploting
   fhDnlChan[uType][iHistoIndex]->Reset();
   fhDnlSum[uType][iHistoIndex]->Reset();
   fhBinSizeChan[uType][iHistoIndex]->Reset();
   
   Double_t dTotalEntriesInHisto = (Double_t)(fhFineTime[ uType ][ iHistoIndex ]->GetEntries() );

   for(Int_t iBin = 0; iBin < (Int_t)uFtBinNb; iBin++)
   {
      Int_t iBinContent = (Int_t)(fhFineTime[ uType ][ iHistoIndex ]->GetBinContent(iBin+1));
      //Looking for the used bins
      if( iBinContent <= 0)
         bincontrol[iBin] = kFALSE;
      else if( iBinContent > 0)
         bincontrol[iBin] = kTRUE;

      // build the sum of all bin content
      if(bincontrol[iBin])
      {
         iSum = iSum + iBinContent;
         fdCorr[uType][iHistoIndex][iBin] = (Double_t)iSum / dTotalEntriesInHisto;
         fhDnlChan[uType][iHistoIndex]->Fill(iBin, (Double_t)iBinContent/dTotalEntriesInHisto );
         fhBinSizeChan[uType][iHistoIndex]->Fill( dClockCycle*(Double_t)iBinContent/dTotalEntriesInHisto );
      } // if(bincontrol[iBin])
         else
         {
            if( 0 < iBin)
               fdCorr[uType][iHistoIndex][iBin] = fdCorr[uType][iHistoIndex][iBin-1];
               else fdCorr[uType][iHistoIndex][iBin] = 0.0;
         } // else of if(bincontrol[iBin])
      fhDnlSum[uType][iHistoIndex]->Fill(iBin, fdCorr[uType][iHistoIndex][iBin] );
      fbCalibAvailable[uType][iHistoIndex] = kTRUE;
   } // for(Int_t iBin=0; iBin < (Int_t)uFtBinNb; iBin++)
   return kTRUE;
}


Bool_t TMbsCalibTdcTof::WriteCalibrationFile()
{
   TTimeStamp timeCurrent;
   oldDir = gDirectory;
   TString sCalibOutFilename;
   if( "" == fsCalibFilename )
      sCalibOutFilename = Form("%sTofTdcCalibHistos_%u_%u.root", fsCalibOutFoldername.Data(), timeCurrent.GetDate( kFALSE), timeCurrent.GetTime( kFALSE) );
      else sCalibOutFilename = Form("%sTofTdcCalibHistos_%s.root", fsCalibOutFoldername.Data(), fsCalibFilename.Data() );
   TFile* fileCalibrationOut = new TFile( sCalibOutFilename, "RECREATE", 
                                         Form("Calibration Data for ToF TDCs, saved from analysis on %s", timeCurrent.AsString("lc") ), 
                                         9);
   if( kTRUE == fileCalibrationOut->IsOpen() )
   {
      sCalibOutFilename += ":/";
      gDirectory->Cd(sCalibOutFilename);
      
      for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
         if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
         {
            UInt_t uNbChan  = 0;
            
            switch( uType )
            {
               case toftdc::caenV1290:
                  uNbChan  = caentdc::kuNbChan;
                  break;
               case toftdc::vftx:
                  uNbChan  = vftxtdc::kuNbChan;
                  break;
               case toftdc::trb:
                  uNbChan  = trbtdc::kuNbChan;
                  break;
               case toftdc::get4:
                  uNbChan  = get4tdc::kuNbChan;
                  if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                     uNbChan  = get4v10::kuNbChan;
                  break;
               default:
                  break;
            } // switch( uType )
            if( 0 == uNbChan  )
            {
               LOG(info)<<"TMbsCalibTdcTof::WriteSingleCalibrations Undefined tdc parameters for type "
                        <<toftdc::ksTdcHistName[ uType ];
               LOG(info)<<" => No calib histo saving!";
               continue;
            }
            
            TString sInfoSaving = "Saved initial calibration histograms for following "
                                  + toftdc::ksTdcHistName[ uType ] +" channels:";
            LOG(info)<<sInfoSaving;
            
            sInfoSaving = "          ";
            for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
               sInfoSaving += Form("%3u ", uChanInd);
            LOG(info)<<sInfoSaving;

            for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
            {
               sInfoSaving = Form("tdc #%3u: ",uTdc );
               for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
               {
                  TString sCalibHistoOutputName = Form("tof_%s_ft_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(), 
                                                         uTdc, uChanInd );
                  Int_t iHistoIndex      = uTdc*uNbChan + uChanInd;

                  // If only initial calib was used and the histogram is not there try again to get it as it was not accessed before
                  if( 0 == fMbsCalibPar->GetMinHitCalib() && NULL == fhFineTime[uType][iHistoIndex] )
                    fhFineTime[uType][iHistoIndex] = (TH1*) gROOT->FindObjectAny( Form("tof_%s_ft_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(), 
                                                                     uTdc, uChanInd));
                  if( NULL == fhFineTime[uType][iHistoIndex] )
                     LOG(info)<<uType<<" "<<iHistoIndex<<" "<<uTdc<<" "<<uChanInd;

                  // New Calibration histo using only new data
                  if( 0 < (fhFineTime[ uType ][ iHistoIndex ]->GetEntries() ) )
                  {
                     fhFineTime[ uType ][ iHistoIndex ]->Write( sCalibHistoOutputName, TObject::kOverwrite);
                     sInfoSaving += "  1 ";
                  } // if( 0 < (fhFineTime[ uType ][ iHistoIndex ]->GetEntries() ) )
                     else sInfoSaving += "  0 ";
               } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
               LOG(info)<<sInfoSaving;
            } // for( UInt_t uTdc = 0; uTdc < fMbsCalibPar->GetNbCalibBoards( uType ); uTdc ++)
         } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      
//      if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
//      {
         if( kFALSE == WriteCalibrationFileRef( sCalibOutFilename ) )
            return kFALSE;
//      } // if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )

      //fileCalibrationOut->Write("",TObject::kOverwrite);
      fileCalibrationOut->Close();
      LOG(info)<<"Calibration data saved in "<<sCalibOutFilename;
      LOG(info)<<"Parameter line: InitialCalibFilename: Text_t   "
               <<(TString)( sCalibOutFilename(0, sCalibOutFilename.Length() -2 ) );
   } // if( kTRUE == fileCalibrationOut->IsOpen() )
      else LOG(warn)<<"TMbsCalibTdcTof::WriteCalibrationFile => Unable to open root file "
                        <<sCalibOutFilename
                        <<" to save calibration data, please check settings";
   gDirectory->Cd(oldDir->GetPath());

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::WriteSingleCalibrations()
{
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {
         UInt_t uNbChan  = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan  = caentdc::kuNbChan;
               break;
            case toftdc::vftx:
               uNbChan  = vftxtdc::kuNbChan;
               break;
            case toftdc::trb:
               uNbChan  = trbtdc::kuNbChan;
               break;
            case toftdc::get4:
               uNbChan  = get4tdc::kuNbChan;
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
         if( 0 == uNbChan  )
         {
            LOG(info)<<"TMbsCalibTdcTof::WriteSingleCalibrations Undefined tdc parameters for type "
                     <<toftdc::ksTdcHistName[ uType ];
            LOG(info)<<" => No calib histo saving!";
            continue;
         }
         
         TString sInfoSaving = "Saved initial calibration histograms for following "
                               + toftdc::ksTdcHistName[ uType ] +" channels:";
         LOG(info)<<sInfoSaving;
         
         sInfoSaving = "          ";
         for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
            sInfoSaving += Form("%3u ", uChanInd);
         LOG(info)<<sInfoSaving;

         // Save current directory as TFile opening change it!
         oldDir = gDirectory;
         
         // Check if initial histos arrays was created
         Bool_t bBoardInitialThere = kFALSE;
         if( fhInitialCalibHisto[ uType ].size() == fMbsUnpackPar->GetNbActiveBoards( uType )*uNbChan )
            bBoardInitialThere = kTRUE;
               
         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         {
            sInfoSaving = Form("tdc #%3u: ",uTdc );
               
            for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
            {
               Bool_t bDataToSave = kFALSE;
               Bool_t bInitialThere = kFALSE;
               Int_t iHistoIndex      = uTdc*uNbChan + uChanInd;
               
               if( kTRUE == bBoardInitialThere )
                  if( NULL != fhInitialCalibHisto[ uType ][ iHistoIndex ] )
                     bInitialThere = kTRUE;
                     
               if( 0 < (fhFineTime[ uType ][ iHistoIndex ]->GetEntries() ) )
                  bDataToSave = kTRUE;
                                             
               if( 1 == fMbsCalibPar->GetSingleCalOutMode() || kFALSE == bInitialThere )
               {
                  if( kTRUE == bDataToSave )
                  {
                     TString sCalibOutFilename = Form("./calib/%s_Tdc%03uChan%03u.root", 
                                                         toftdc::ksTdcHistName[ uType ].Data(),
                                                         uTdc, uChanInd);
                     TFile* fileCalibrationOut = new TFile( sCalibOutFilename, "RECREATE",
                                                            Form("Calibration Data for %s TDC %03u channel %03u", 
                                                            toftdc::ksTdcHistName[ uType ].Data(),
                                                            uTdc, uChanInd ),
                                                            9);
                     if( kTRUE == fileCalibrationOut->IsOpen() )
                     {
                        sCalibOutFilename += ":/";
                        gDirectory->Cd(sCalibOutFilename);
                        TString sCalibHistoOutputName = Form("tof_%s_ft_b%03u_ch%03u", toftdc::ksTdcHistName[ uType ].Data(), 
                                                               uTdc, uChanInd );
                        fhFineTime[ uType ][ iHistoIndex ]->Write( sCalibHistoOutputName, TObject::kOverwrite);
                        sInfoSaving += "  1 ";
                        
                        //fileCalibrationOut->Write("",TObject::kOverwrite);
                        fileCalibrationOut->Close();
                        gDirectory->Cd(oldDir->GetPath());
                     } // if( kTRUE == fileCalibrationOut->IsOpen() )
                     else sInfoSaving += "  0 ";
                  } // if( kTRUE == bDataToSave )
                     else sInfoSaving += "  0 ";
               } // if( 1 == fMbsCalibPar->GetSingleCalOutMode() || kFALSE == bInitialThere )
                  else if( kTRUE == bDataToSave )
                  {
                     // Update old calibration histo with new data
                     TString sCalibOutFilename = Form("./calib/%s_Tdc%03uChan%03u.root ", 
                                                         toftdc::ksTdcHistName[ uType ].Data(),
                                                         uTdc, uChanInd);
                     TFile* fileCalibrationOut = new TFile( sCalibOutFilename, "RECREATE",
                                                            Form("Calibration Data for %s TDC %03u channel %03u", 
                                                            toftdc::ksTdcHistName[ uType ].Data(),
                                                            uTdc, uChanInd ),
                                                            9);
                     if( kTRUE == fileCalibrationOut->IsOpen() )
                     {
                        sCalibOutFilename += ":/";
                        gDirectory->Cd(sCalibOutFilename);
                        TString sCalibHistoOutputName = Form("tof_%s_ft_b%03d_ch%03u", toftdc::ksTdcHistName[ uType ].Data(), 
                                                               fMbsCalibPar->GetInitialCalInd( uType, uTdc ), uChanInd );
                        // Try to update old calibration histo with new data
                        fhInitialCalibHisto[ uType ][ iHistoIndex ]->Add( fhFineTime[ uType ][ iHistoIndex ] );
                        fhInitialCalibHisto[ uType ][ iHistoIndex ]->Write( sCalibHistoOutputName, TObject::kOverwrite);
                        sInfoSaving += "  1 ";
                        
                        //fileCalibrationOut->Write("",TObject::kOverwrite);
                        fileCalibrationOut->Close();
                        gDirectory->Cd(oldDir->GetPath());
                     } // if( kTRUE == fileCalibrationOut->IsOpen() )
                     else sInfoSaving += "  0 ";
                  } // else of if( 1 == fMbsCalibPar->uSingleChannelCalibFilesOutput )
                     // No new Data => no need to update calibration!!!
                     else sInfoSaving += "  0 "; 
            } // for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
            LOG(info)<<sInfoSaving;
         } // for( UInt_t uTdc = 0; uTdc < fMbsCalibPar->GetNbCalibBoards( uType ); uTdc ++)
         
         // Go back to original directory
         gDirectory->Cd(oldDir->GetPath());
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   
//   if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
//   {
      if( kFALSE == WriteSingleCalibrationsRef( ) )
         return kFALSE;
//   } // if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )

   return kTRUE;
}
// ------------------------------------------------------------------
Bool_t TMbsCalibTdcTof::CreateTotVariables()
{
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {
         UInt_t uNbChan  = 0;
         UInt_t uMaxMult = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan  = caentdc::kuNbChan;
               uMaxMult = toftdc::kuDefNbMulti;
               break;
            case toftdc::vftx:
               uNbChan  = vftxtdc::kuNbChan;
               uMaxMult = vftxtdc::kuNbMulti;
               break;
            case toftdc::trb:
               uNbChan  = trbtdc::kuNbChan;
               uMaxMult = toftdc::kuDefNbMulti;
               break;
            case toftdc::get4:
               uNbChan  = get4tdc::kuNbChan;
               uMaxMult = toftdc::kuDefNbMulti;
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
      
         // TODO: test for flag for TOT inside a single data
         switch( fMbsCalibPar->GetTotMode( uType ) )
         {
            case 1:
               xTofTdcDataPrevArray[uType] = new TClonesArray("TTofCalibData", 
                        uNbChan * fMbsUnpackPar->GetNbActiveBoards( uType )); 
               // no need of a temporary vector to hold not associated edges and time order them
               // as the hits are either already ordered
               break;
            case 2:
               xTofTdcDataPrevArray[uType] = new TClonesArray("TTofCalibData", 
                        uNbChan/2 * fMbsUnpackPar->GetNbActiveBoards( uType )); 
                        
               // Use a temporary vector to hold not associated edges and time order them
               // 1 input channel -> 2 TDC channels => all boards used
               xTempCalibData[uType] = new TClonesArray("TTofCalibData", 
                        uMaxMult * uNbChan/2 * fMbsUnpackPar->GetNbActiveBoards( uType )); 
               break;
            case 3:
               if( 1 == fMbsUnpackPar->GetNbActiveBoards( uType )%2 )
                  return kFALSE;
                  
               xTofTdcDataPrevArray[uType] = new TClonesArray("TTofCalibData", 
                        uNbChan   * fMbsUnpackPar->GetNbActiveBoards( uType )); 
                        
               // Use a temporary vector to hold not associated edges and time order them
               // 1 FEE channel -> 2 input channels in each board => 1/2 boards used
               xTempCalibData[uType] = new TClonesArray("TTofCalibData", 
                        uMaxMult * uNbChan * fMbsUnpackPar->GetNbActiveBoards( uType ) /2); 
               break;
            case 0:
            case 4:
               // no need of a temporary vector to hold not associated edges and time order them
               // as the hits are either already associated
            default :
               xTofTdcDataPrevArray[uType] = NULL;
               break;
         } // switch( fMbsCalibPar->GetTotMode() )
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
         else xTofTdcDataPrevArray[uType] = NULL;
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::ClearTotVariables()
{
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {         
      
         // TODO: test for flag for TOT inside a single data
         switch( fMbsCalibPar->GetTotMode( uType ) )
         {
            case 1:
               xTofTdcDataPrevArray[uType]->Clear("C"); 
               // no need of a temporary vector to hold not associated edges and time order them
               // as the hits are either already ordered
               break;
            case 2:
               xTofTdcDataPrevArray[uType]->Clear("C"); 
                        
               // Use a temporary vector to hold not associated edges and time order them
               // 1 input channel -> 2 TDC channels => all boards used
               xTempCalibData[uType]->Clear("C"); 
               break;
            case 3:
               if( 1 == fMbsUnpackPar->GetNbActiveBoards( uType )%2 )
                  return kFALSE;
                  
               xTofTdcDataPrevArray[uType]->Clear("C"); 
                        
               // Use a temporary vector to hold not associated edges and time order them
               // 1 FEE channel -> 2 input channels in each board => 1/2 boards used
               xTempCalibData[uType]->Clear("C"); 
               break;
            case 0:
            case 4:
               // no need of a temporary vector to hold not associated edges and time order them
               // as the hits are either already associated
            default :
               break;
         } // switch( fMbsCalibPar->GetTotMode() )
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::DeleteTotVariables()
{
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {         
      
         // TODO: test for flag for TOT inside a single data
         switch( fMbsCalibPar->GetTotMode( uType ) )
         {
            case 1:
               delete xTofTdcDataPrevArray[uType]; 
               // no need of a temporary vector to hold not associated edges and time order them
               // as the hits are either already ordered
               break;
            case 2:
               delete xTofTdcDataPrevArray[uType]; 
                        
               // Use a temporary vector to hold not associated edges and time order them
               // 1 input channel -> 2 TDC channels => all boards used
               delete xTempCalibData[uType]; 
               break;
            case 3:
               if( 1 == fMbsUnpackPar->GetNbActiveBoards( uType )%2 )
                  return kFALSE;
                  
               delete xTofTdcDataPrevArray[uType]; 
                        
               // Use a temporary vector to hold not associated edges and time order them
               // 1 FEE channel -> 2 input channels in each board => 1/2 boards used
               delete xTempCalibData[uType]; 
               break;
            case 0:
            case 4:
               // no need of a temporary vector to hold not associated edges and time order them
               // as the hits are either already associated
            default :
               break;
         } // switch( fMbsCalibPar->GetTotMode() )
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::BuildTotSplitChannels( UInt_t uType, UInt_t uBoard )
{
   UInt_t uNbChan  = 0;
   switch( uType )
   {
      case toftdc::caenV1290:
         uNbChan  = caentdc::kuNbChan;
         break;
      case toftdc::vftx:
         uNbChan  = vftxtdc::kuNbChan;
         break;
      case toftdc::trb:
         uNbChan  = trbtdc::kuNbChan;
         break;
      case toftdc::get4:
         uNbChan  = get4tdc::kuNbChan;
         if( 4 == fMbsCalibPar->GetTotMode( uType ) )
            uNbChan  = get4v10::kuNbChan;
         break;
      default:
         break;
   } // switch( uType )
   
   // In the case where 1 input channel correspond to 2 TDC channels, 
   // we can time order the hits now that we have all of them calibrated
   // and then we will be able to associate them in full calibrated TDC data!
   xTempCalibData[uType]->Sort();
   
   // now build TOT
   Int_t iNbCalData = xTempCalibData[uType]->GetEntriesFast();
   LOG(debug)<<"TMbsCalibTdcTof::BuildTotSplitChannels Type "<<uType<<" Board "<<uBoard<<" N Data "<<iNbCalData;
   for( Int_t iCalDataInd = 0; iCalDataInd < iNbCalData; iCalDataInd++ )
   {
      TTofCalibData * xCalibData = (TTofCalibData*) xTempCalibData[uType]->At(iCalDataInd);
      
      UInt_t   uInputChan = xCalibData->GetChannel();
      Int_t    iDataIndex = uBoard*uNbChan/2 + uInputChan;
      UInt_t   uEdge      = xCalibData->GetEdge();
      Double_t dCalibTime = xCalibData->GetTime();
/*
   LOG(info)<<"TMbsCalibTdcTof::BuildTotSplitChannels Type "<<uType<<" Board "<<uBoard<<" Data "<<iCalDataInd;
   LOG(info)<<" Chan "<<uInputChan<<" Edge "<<uEdge;
   LOG(info)<<" Time "<<dCalibTime;
*/
      if( 0 == uEdge )
      {
         if( NULL != xTofTdcDataPrevArray[uType]->At( iDataIndex ) )
            ((TTofCalibData*)(xTofTdcDataPrevArray[uType]->At( iDataIndex )))->Clear();
         // rising edge channel
         new((*xTofTdcDataPrevArray[uType])[ iDataIndex ]) 
            TTofCalibData( *xCalibData );
      } // if( 1 == xUnpDataPtr->GetChannel()%2 )
         else if( NULL != xTofTdcDataPrevArray[uType]->At( iDataIndex ) )
         {
            TTofCalibData* dataRisingEdge = (TTofCalibData*)(xTofTdcDataPrevArray[uType]->At( iDataIndex ) );
            if( kTRUE == dataRisingEdge->IsFilled() )
            {
               Double_t dDataTime = dataRisingEdge->GetTime();
               // falling edge & rising edge present & filled
               Double_t dTot = dataRisingEdge->GetTimeDifference( dCalibTime );
               
               // Time offset applies to input channel, not TDC channel!
               if( kTRUE == fMbsCalibPar->EnaTimeOffset() )
                  dDataTime -= fMbsCalibPar->GetTimeOffsetVal( uType, uBoard, uInputChan );

               // TDC to TDC offset
               if( kTRUE == fMbsCalibPar->GetTdcOffEnaFlag(uType, uBoard) )
//               if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
                  dDataTime -= fdTdcOffsets[uType][uBoard];
                  
               // Tot offset applies to input channel, not TDC channel!
               // Tot offset signe inverted if rising and falling inverted
//               if( kTRUE == fMbsCalibPar->EnaTotOffset() )
//                  dTot -= fMbsCalibPar->GetTotOffsetVal( uType, uBoard, uInputChan ) * 
//                          ( 1 == fMbsCalibPar->GetTotInvFlag( uType, uBoard, uInputChan ) ? -1 : 1 );
      
               // Use time from rising edge and newly calculated tot to create the hit
               new((*fCalibDataCollection)[ fCalibDataCollection->GetEntriesFast() ]) 
                  TTofCalibData( uType, uBoard, uInputChan, 
                                 dDataTime, 
                                 dTot, 2 );
            } // if( kTRUE == (xTofTdcDataPrevArray[uType]->At( iDataIndex ) )->IsFilled() )
            else
            {
            	LOG(debug)<<"Cannot build ToT falling before rising. Please cross-check your cabling!";
            }
         } // else if( NULL != xTofTdcDataPrevArray[uType]->At( iDataIndex ) )
         else
         {
         	LOG(debug)<<"Cannot build ToT falling before any rising. Please cross-check your cabling!";
         }
   } // for( Int_t iUnpDataInd = 0; iUnpDataInd < iNbUnpData; iUnpDataInd++ )
//   LOG(info)<<"TMbsCalibTdcTof::BuildTotSplitChannels Type "<<uType<<" Board "<<uBoard<<" N Data End "<<fCalibDataCollection->GetEntriesFast();
         
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::BuildTotSplitBoards( UInt_t uType )
{
   UInt_t uNbChan  = 0;
   switch( uType )
   {
      case toftdc::caenV1290:
         uNbChan  = caentdc::kuNbChan;
         break;
      case toftdc::vftx:
         uNbChan  = vftxtdc::kuNbChan;
         break;
      case toftdc::trb:
         uNbChan  = trbtdc::kuNbChan;
         break;
      case toftdc::get4:
         uNbChan  = get4tdc::kuNbChan;
         if( 4 == fMbsCalibPar->GetTotMode( uType ) )
            uNbChan  = get4v10::kuNbChan;
         break;
      default:
         break;
   } // switch( uType )
   
   // In the case where 1 input channel correspond to a TDC channel in 2 consecutive boards, 
   // we can time order the hits now that we have all boards calibrated
   // and then we will be able to associate the hits in full calibrated TDC data!
   xTempCalibData[uType]->Sort();
   
   // now build TOT
   Int_t iNbCalData = xTempCalibData[uType]->GetEntriesFast();
   for( Int_t iCalDataInd = 0; iCalDataInd < iNbCalData; iCalDataInd++ )
   {
      TTofCalibData * xCalibData = (TTofCalibData*) xTempCalibData[uType]->At(iCalDataInd);
      
      UInt_t   uBoard     = xCalibData->GetBoard();
      UInt_t   uInputChan = xCalibData->GetChannel();
      Int_t    iDataIndex = uBoard*uNbChan + uInputChan;
      UInt_t   uEdge      = xCalibData->GetEdge();
      Double_t dCalibTime = xCalibData->GetTime();
         
      if( 0 == uEdge )
      {
         if( NULL != xTofTdcDataPrevArray[uType]->At( iDataIndex ) )
            ((TTofCalibData*)(xTofTdcDataPrevArray[uType]->At( iDataIndex )))->Clear();
         // rising edge channel
         new((*xTofTdcDataPrevArray[uType])[ iDataIndex ]) 
            TTofCalibData( *xCalibData );
      } // if( 1 == xUnpDataPtr->GetChannel()%2 )
         else if( NULL != xTofTdcDataPrevArray[uType]->At( iDataIndex ) )
         {
            TTofCalibData* dataRisingEdge = (TTofCalibData*)(xTofTdcDataPrevArray[uType]->At( iDataIndex ) );
            if( kTRUE == dataRisingEdge->IsFilled() )
            {
               Double_t dDataTime = dataRisingEdge->GetTime();
               // falling edge & rising edge present & filled
               Double_t dTot = dataRisingEdge->GetTimeDifference( dCalibTime );
               
               // Time offset applies to input channel, not TDC channel!
               if( kTRUE == fMbsCalibPar->EnaTimeOffset() )
                  dDataTime -= fMbsCalibPar->GetTimeOffsetVal( uType, uBoard, uInputChan );

               // TDC to TDC offset
               if( kTRUE == fMbsCalibPar->GetTdcOffEnaFlag(uType, uBoard) )
//               if( kTRUE == fMbsCalibPar->IsTdcOffsetEna() )
                  dDataTime -= fdTdcOffsets[uType][uBoard];
                  
               // Tot offset applies to input channel, not TDC channel!
               // Tot offset signe inverted if rising and falling inverted
               if( kTRUE == fMbsCalibPar->EnaTimeOffset() )
                  dTot -= fMbsCalibPar->GetTotOffsetVal( uType, uBoard, uInputChan ) * 
                          ( 1 == fMbsCalibPar->GetTotInvFlag( uType, uBoard, uInputChan ) ? -1 : 1 );
      
               // Use time from rising edge and newly calculated tot to create the hit
               new((*fCalibDataCollection)[ fCalibDataCollection->GetEntriesFast() ]) 
                  TTofCalibData( uType, uBoard, uInputChan, 
                                 dDataTime, 
                                 dTot, 2 );
            } // if( kTRUE == (xTofTdcDataPrevArray[uType]->At( iDataIndex ) )->IsFilled() )
         } // else if( NULL != xTofTdcDataPrevArray[uType]->At( iDataIndex ) )
   } // for( Int_t iUnpDataInd = 0; iUnpDataInd < iNbUnpData; iUnpDataInd++ )
         
   return kTRUE;
}

// ------------------------------------------------------------------
Bool_t TMbsCalibTdcTof::GetRefHistosFromUnpack()
{
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !
   
   // Loop over all TDC types
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // TODO: for now only TRB has reference channel methods implemented
      if( toftdc::caenV1290 == uType || 
           toftdc::vftx == uType || 
           toftdc::get4 == uType )
            continue;

      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {             
         fhFineTimeRef[uType].resize( fMbsUnpackPar->GetNbActiveBoards( uType ) );
         
         TString sInfoLoading = "Got FineTime histograms from unpack step for following "+
                                toftdc::ksTdcHistName[ uType ] + " TDC reference channels:";
         LOG(info)<<sInfoLoading;
         
         sInfoLoading = "          ";
         LOG(info)<<sInfoLoading;
            
         // Loop over all active TDCs and all channels
         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         {
            sInfoLoading = Form("tdc #%3u: ",uTdc );

            fhFineTimeRef[uType][uTdc] = (TH1*) gDirectory->FindObject( Form("tof_%s_ft_b%03u_ref", toftdc::ksTdcHistName[ uType ].Data(), 
                                                                  uTdc));
            if( NULL == fhFineTimeRef[uType][uTdc] )
            {
               LOG(error)<<" TMbsCalibTdcTof::GetRefHistosFromUnpack => Could not get FT histo for "
                         <<toftdc::ksTdcHistName[ uType ]<<" #"
                         <<uTdc<<" reference channel from the unpack step";
               sInfoLoading += "  0 ";
               gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
               return kFALSE;
            } // if( NULL == fhFineTime[uType][iHistoIndex] )
               else LOG(debug)<<" TMbsCalibTdcTof::GetHistosFromUnpack =>Got FT histo for "
                               <<toftdc::ksTdcHistName[ uType ]<<" #"
                               <<uTdc<<" reference channel from the unpack step: 0x"
                               <<fhFineTimeRef[uType][uTdc]<<" "
                               <<fhFineTimeRef[uType][uTdc]->GetEntries();

            sInfoLoading += "  1 ";
            LOG(info)<<sInfoLoading;
         } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
         else LOG(info)<<" TMbsCalibTdcTof::GetRefHistosFromUnpack => no boards for "<<toftdc::ksTdcHistName[ uType ]<<" "<<uType;
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   
   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
   
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::LoadCalibrationFileRef()
{     
   // First check if the inital calibration file name is properly defined
   if(  kTRUE != fMbsCalibPar->GetInitCalFilename().EqualTo("") &&
        kTRUE != fMbsCalibPar->GetInitCalFilename().EqualTo("-") )
   {
      // Save online ROOT directory as the File opening auto change current Dir
      oldDir = gDirectory;

      TString sInitialCalibHistoName = "";
      fileCalibrationIn = new TFile( fMbsCalibPar->GetInitCalFilename(), "READ");

      if( kTRUE == fileCalibrationIn->IsOpen() )
      {
         // Loop over all TDC types
         for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
         {
            // TODO: for now only TRB has reference channel methods implemented
            if( toftdc::caenV1290 == uType || 
                 toftdc::vftx == uType || 
                 toftdc::get4 == uType )
                  continue;

            if( 0 == fMbsCalibPar->GetNbCalibBoards( uType ) ||
                0 == fMbsUnpackPar->GetNbActiveBoards( uType ) )
               continue;
               
            UInt_t uFtBinNb = 0;
            
            switch( uType )
            {
               case toftdc::caenV1290:
                  uFtBinNb = caentdc::kiFineTime + 1;
                  break;
               case toftdc::vftx:
                  uFtBinNb = vftxtdc::kiFifoFineTime + 1;
                  break;
               case toftdc::trb:
                  uFtBinNb = trbtdc::kiFineCounterSize;
                  break;
               case toftdc::get4:
                  uFtBinNb = get4tdc::kiFineTime + 1;
                  break;
               default:
                  break;
            } // switch( uType )
            if( 1 == uFtBinNb )
            {
               LOG(info)<<"TMbsCalibTdcTof::LoadCalibrationFileRef Undefined tdc parameters for type "
                        <<toftdc::ksTdcHistName[ uType ];
               LOG(info)<<" => No initial calib loading!";
               continue;
            }
            // TODO: Does not handle the TRB case!
            // One could replace trbtdc::kiFineCounterSize everywhere by trbtdc::time_FineTimeBitMask + 1
            // to handle this case like every other TDC
         
            TString sInfoLoading = "Loaded initial calibration histograms for following "+
                                   toftdc::ksTdcHistName[ uType ] + " TDC reference channel:";
            LOG(info)<<sInfoLoading;

            //define correction variables
            fhInitialCalibHistoRef[ uType ].resize( fMbsCalibPar->GetNbCalibBoards( uType ), NULL );
/*            Bool_t bincontrol[uFtBinNb];*/ // Commented out to remove warning bec. unused
//            Int_t iSum[fMbsCalibPar->GetNbCalibBoards( uType )][uNbChan];

            // Temp variable to store pointer on calib histo in File
            TH1* fInitialCalibHistoFromFile = 0;

            for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
            {
               if( -1 < fMbsCalibPar->GetInitialCalInd( uType, uTdc ) )
               {
                  sInfoLoading = Form("tdc #%3u: ",uTdc );
                     
                  // Initialize pointer to NULL
                  fInitialCalibHistoFromFile = NULL;

                  // Find histogram in file and store its pointer in a temp variable
                  sInitialCalibHistoName = Form("tof_%s_ft_b%03d_ref", toftdc::ksTdcHistName[ uType ].Data(), 
                                                fMbsCalibPar->GetInitialCalInd( uType, uTdc ) );
                  
                  fileCalibrationIn->GetObject( sInitialCalibHistoName, fInitialCalibHistoFromFile);

                  if( NULL == fInitialCalibHistoFromFile )
                     sInfoLoading += "  0 ";
                     else
                     {
                        sInfoLoading += "  1 ";
                        // Clone the found histo and move it to online ROOT directory instead of File
                        fhInitialCalibHistoRef[ uType ][ uTdc ] = (TH1*)fInitialCalibHistoFromFile->Clone( 
                                                Form("%s_CalibFile", sInitialCalibHistoName.Data() ) );
                        fhInitialCalibHistoRef[ uType ][ uTdc ]->SetDirectory( oldDir );
                     } // else of if( 0 == fInitialCalibHistoFromFile )
                     
                  // Extract calibration factors from initialization histo
                  CalibFactorsInitReference( uType, uTdc );

                  LOG(info)<<sInfoLoading;
               } // if( -1 < fMbsCalibPar->GetInitialCalInd( uType, uTdc ) )
            } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
         
         // File closing and going back to online ROOT folder
         fileCalibrationIn->Close();
         gDirectory->Cd(oldDir->GetPath());
      } // if( kTRUE == fileCalibrationIn->IsOpen() )
         else
         {
            LOG(info)<<"Could not open "<<fMbsCalibPar->GetInitCalFilename()
                     <<" to load initial TDC calibration for reference channel, please check setting in Calibration option file";
            for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
               if( 0 == fMbsCalibPar->GetNbCalibBoards( uType ) ||
                   0 == fMbsUnpackPar->GetNbActiveBoards( uType ) )
                  continue;
               else
               {
                  // resize array and assign NULL to all pointers to avoid segmentation faults
                  fhInitialCalibHistoRef[ uType ].resize( fMbsCalibPar->GetNbCalibBoards( uType ), NULL );
               }
            return kFALSE;
         } // else of if( kTRUE == fileCalibrationIn->IsOpen() )
   } // if(  sInitialCalibrationFilename OK and 1 == fMbsCalibPar->uEnableCalib)
   
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::LoadSingleCalibrationsRef()
{     
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // TODO: for now only TRB has reference channel methods implemented
      if( toftdc::caenV1290 == uType || 
           toftdc::vftx == uType || 
           toftdc::get4 == uType )
            continue;

      if( 0 == fMbsCalibPar->GetNbCalibBoards( uType ) ||
          0 == fMbsUnpackPar->GetNbActiveBoards( uType ) )
         continue;
         
      UInt_t uFtBinNb = 0;
      
      switch( uType )
      {
         case toftdc::caenV1290:
            uFtBinNb = caentdc::kiFineTime + 1;
            break;
         case toftdc::vftx:
            uFtBinNb = vftxtdc::kiFifoFineTime + 1;
            break;
         case toftdc::trb:
            uFtBinNb = trbtdc::kiFineCounterSize;
            break;
         case toftdc::get4:
            uFtBinNb = get4tdc::kiFineTime + 1;
            break;
         default:
            break;
      } // switch( uType )
      if( 1 == uFtBinNb )
      {
         LOG(info)<<"TMbsCalibTdcTof::LoadSingleCalibrationsRef Undefined tdc parameters for type "
                  <<toftdc::ksTdcHistName[ uType ];;
         LOG(info)<<" => No initial calib loading!";
         continue;
      }
      // TODO: Does not handle the TRB case!
      // One could replace trbtdc::kiFineCounterSize everywhere by trbtdc::time_FineTimeBitMask + 1
      // to handle this case like every other TDC

      TString sInfoLoading = "Loaded initial calibration histograms for following "+
                             toftdc::ksTdcHistName[ uType ] + " TDC reference channel:";
      LOG(info)<<sInfoLoading;

      //define correction variables
      fhInitialCalibHistoRef[ uType ].resize( fMbsCalibPar->GetNbCalibBoards( uType ), NULL );
/*      Bool_t bincontrol[uFtBinNb];*/ // Commented out to remove warning bec. unused

      // Save online ROOT directory as the File opening auto change current Dir
      oldDir = gDirectory;

      for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         if( -1 < fMbsCalibPar->GetInitialCalInd( uType, uTdc ) )
         {
            sInfoLoading = Form("tdc #%3u: ",uTdc );

            TString sInitialCalibFileName = Form("./calib/%s_Tdc%03dRefChan.root ", toftdc::ksTdcHistName[ uType ].Data(),
                                                   fMbsCalibPar->GetInitialCalInd( uType, uTdc ) );
            TString sInitialCalibHistoName = "";
            fileCalibrationIn = new TFile( sInitialCalibFileName, "READ");
            if( kTRUE == fileCalibrationIn->IsOpen() )
            {
               // Initialize pointer to NULL
               TH1* fInitialCalibHistoFromFile = NULL;
               sInitialCalibHistoName = Form("tof_%s_ft_b%03d_ref", toftdc::ksTdcHistName[ uType ].Data(), 
                                             fMbsCalibPar->GetInitialCalInd( uType, uTdc ) );
                     
               fileCalibrationIn->GetObject( sInitialCalibHistoName, fInitialCalibHistoFromFile);
               
               if( NULL == fInitialCalibHistoFromFile )
                  sInfoLoading += "  0 ";
                  else
                  {
                     sInfoLoading += "  1 ";
                     fhInitialCalibHistoRef[ uType ][ uTdc ] = (TH1*)fInitialCalibHistoFromFile->Clone(
                                                   Form("%s_CalibFile", sInitialCalibHistoName.Data() ) );
                     fhInitialCalibHistoRef[ uType ][ uTdc ]->SetDirectory( oldDir );
                     
                     // Extract calibration factors from initialization histo
                     CalibFactorsInitReference( uType, uTdc );
                  } // else of if( NULL == fInitialCalibHistoFromFile )
               fileCalibrationIn->Close();
            } // if( kTRUE == fileCalibrationIn->IsOpen() )
               else sInfoLoading += "  0 ";

            LOG(info)<<sInfoLoading;
         } // if( -1 < fMbsCalibPar->GetInitialCalInd( uType, uTdc ) )
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::CalibFactorsInitReference( UInt_t uType, UInt_t uBoard)
{     
//   Double_t dClockCycle = 0.0; // -> Comment to remove warning because set but never used
   UInt_t   uFtBinNb    = 0;
   
   switch( uType )
   {
      case toftdc::caenV1290:
//         dClockCycle = caentdc::kdClockCycleSize; // -> Comment to remove warning because set but never used
         uFtBinNb    = caentdc::kiFineTime + 1;
         break;
      case toftdc::vftx:
//         dClockCycle = vftxtdc::kdClockCycleSize; // -> Comment to remove warning because set but never used
         uFtBinNb    = vftxtdc::kiFifoFineTime + 1;
         break;
      case toftdc::trb:
//         dClockCycle = trbtdc::kdClockCycleSize; // -> Comment to remove warning because set but never used
         uFtBinNb    = trbtdc::kiFineCounterSize;
         break;
      case toftdc::get4:
//         dClockCycle = get4tdc::kdClockCycleSize; // -> Comment to remove warning because set but never used
         uFtBinNb    = get4tdc::kiFineTime + 1;
         break;
      default:
         break;
   } // switch( uType )
   
   if( NULL != fhInitialCalibHistoRef[ uType ][ uBoard ])
   {
      // Reset Histos with DNL ploting
//      fhDnlChan[uType][uBoard]->Reset();
//      fhDnlSum[uType][uBoard]->Reset();
//      fhBinSizeChan[uType][uBoard]->Reset();
      
      //set the sum to zero before summing over all bins in one channel
      Bool_t bincontrol[uFtBinNb];
      Int_t iSum = 0;
      
      Double_t dTotalEntriesInHisto = (Double_t)(fhInitialCalibHistoRef[ uType ][ uBoard ]->GetEntries() );

      for(Int_t iBin = 0; iBin < (Int_t)uFtBinNb; iBin++)
      {
         Int_t iBinContent = (Int_t)(fhInitialCalibHistoRef[ uType ][ uBoard ]->GetBinContent(iBin+1));
         //Looking for the used bins
         if( iBinContent <= 0)
            bincontrol[iBin] = kFALSE;
         else if( iBinContent > 0)
            bincontrol[iBin] = kTRUE;

         // build the sum of all bin content
         if(bincontrol[iBin])
         {
            iSum = iSum + iBinContent;
            fdCorrRef[uType][uBoard][iBin] = (Double_t)iSum / dTotalEntriesInHisto;
//            fhDnlChan[uType][uBoard]->Fill(iBin, (Double_t)iBinContent/dTotalEntriesInHisto );
//            fhBinSizeChan[uType][uBoard]->Fill( dClockCycle*(Double_t)iBinContent/dTotalEntriesInHisto );
         } // if(bincontrol[iBin])
            else
            {
               if( 0 < iBin)
                  fdCorrRef[uType][uBoard][iBin] = fdCorrRef[uType][uBoard][iBin-1];
                  else fdCorrRef[uType][uBoard][iBin] = 0.0;
            } // else of if(bincontrol[iBin])
//         fhDnlSum[uType][uBoard]->Fill(iBin, fdCorrRef[uType][uBoard][iBin] );
      } // for(Int_t iBin=0; iBin < (Int_t)uFtBinNb; iBin++)
   } // if( NULL != fhInitialCalibHistoRef[uTdc][uBoard])
      else return kFALSE;
      
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::CalibFactorsCalcReference( UInt_t uType, UInt_t uBoard, Bool_t bWithInitial )
{     
//   Double_t dClockCycle = 0.0;
   UInt_t   uFtBinNb    = 0;
   
   switch( uType )
   {
      case toftdc::caenV1290:
//         dClockCycle = caentdc::kdClockCycleSize; // -> Comment to remove warning because set but never used
         uFtBinNb    = caentdc::kiFineTime + 1;
         break;
      case toftdc::vftx:
//         dClockCycle = vftxtdc::kdClockCycleSize; // -> Comment to remove warning because set but never used
         uFtBinNb    = vftxtdc::kiFifoFineTime + 1;
         break;
      case toftdc::trb:
//         dClockCycle = trbtdc::kdClockCycleSize; // -> Comment to remove warning because set but never used
         uFtBinNb    = trbtdc::kiFineCounterSize;
         break;
      case toftdc::get4:
//         dClockCycle = get4tdc::kdClockCycleSize; // -> Comment to remove warning because set but never used
         uFtBinNb    = get4tdc::kiFineTime + 1;
         break;
      default:
         break;
   } // switch( uType )
   
   // define correction variables
   // & set the sum to zero before summing over all bins in one channel
   Bool_t bincontrol[uFtBinNb];
   Int_t iSum = 0;
   
   if( kTRUE == bWithInitial )
   {
      // Use both initial calibration histograms and new data
      // To extract the correction factors
      if( NULL != fhInitialCalibHistoRef[ uType ][ uBoard ])
      {
         // Reset Histos with DNL ploting
//         fhDnlChan[uType][uBoard]->Reset();
//         fhDnlSum[uType][uBoard]->Reset();
//         fhBinSizeChan[uType][uBoard]->Reset();
         
         Double_t dTotalEntriesInHisto = (Double_t)(fhFineTimeRef[ uType ][ uBoard ]->GetEntries() ) + 
                                         (Double_t)(fhInitialCalibHistoRef[ uType ][ uBoard ]->GetEntries() );

         for(Int_t iBin = 0; iBin < (Int_t)uFtBinNb; iBin++)
         {
            Int_t iBinContent = (Int_t)(fhFineTimeRef[ uType ][ uBoard ]->GetBinContent(iBin+1)) +
                                (Int_t)(fhInitialCalibHistoRef[ uType ][ uBoard ]->GetBinContent(iBin+1));
            //Looking for the used bins
            if( iBinContent <= 0)
               bincontrol[iBin] = kFALSE;
            else if( iBinContent > 0)
               bincontrol[iBin] = kTRUE;

            // build the sum of all bin content
            if(bincontrol[iBin])
            {
               iSum = iSum + iBinContent;
               fdCorr[uType][uBoard][iBin] = (Double_t)iSum / dTotalEntriesInHisto;
               //TODO-CHS: must be fdCorrRef!!!

//               fhDnlChan[uType][iHistoIndex]->Fill(iBin, (Double_t)iBinContent/dTotalEntriesInHisto );
//               fhBinSizeChan[uType][iHistoIndex]->Fill( dClockCycle*(Double_t)iBinContent/dTotalEntriesInHisto );
            } // if(bincontrol[iBin])
               else
               {
                  if( 0 < iBin)
                     fdCorrRef[uType][uBoard][iBin] = fdCorrRef[uType][uBoard][iBin-1];
                     else fdCorrRef[uType][uBoard][iBin] = 0.0;
               } // else of if(bincontrol[iBin])
//            fhDnlSum[uType][iHistoIndex]->Fill(iBin, fdCorrRef[uType][iHistoIndex][iBin] );
         } // for(Int_t iBin=0; iBin < (Int_t)uFtBinNb; iBin++)
         return kTRUE;
      } // if( NULL != fhInitialCalibHistoRef[ uType ][ uBoard ])
   } // if( kTRUE == bWithInitial )
   
   // If either choice to use only new data or initial histo missing
   // => Use only new data to extract the correction factors
   
   // Reset Histos with DNL ploting
//   fhDnlChan[uType][uBoard]->Reset();
//   fhDnlSum[uType][uBoard]->Reset();
//   fhBinSizeChan[uType][uBoard]->Reset();
   
   Double_t dTotalEntriesInHisto = (Double_t)(fhFineTimeRef[ uType ][ uBoard ]->GetEntries() );

   for(Int_t iBin = 0; iBin < (Int_t)uFtBinNb; iBin++)
   {
      Int_t iBinContent = (Int_t)(fhFineTimeRef[ uType ][ uBoard ]->GetBinContent(iBin+1));
      //Looking for the used bins
      if( iBinContent <= 0)
         bincontrol[iBin] = kFALSE;
      else if( iBinContent > 0)
         bincontrol[iBin] = kTRUE;

      // build the sum of all bin content
      if(bincontrol[iBin])
      {
         iSum = iSum + iBinContent;
         fdCorr[uType][uBoard][iBin] = (Double_t)iSum / dTotalEntriesInHisto;
         //TODO-CHS: must be fdCorrRef!!!

//         fhDnlChan[uType][uBoard]->Fill(iBin, (Double_t)iBinContent/dTotalEntriesInHisto );
//         fhBinSizeChan[uType][uBoard]->Fill( dClockCycle*(Double_t)iBinContent/dTotalEntriesInHisto );
      } // if(bincontrol[iBin])
         else
         {
            if( 0 < iBin)
               fdCorrRef[uType][uBoard][iBin] = fdCorrRef[uType][uBoard][iBin-1];
               else fdCorrRef[uType][uBoard][iBin] = 0.0;
         } // else of if(bincontrol[iBin])
//      fhDnlSum[uType][uBoard]->Fill(iBin, fdCorrRef[uType][uBoard][iBin] );
   } // for(Int_t iBin=0; iBin < (Int_t)uFtBinNb; iBin++)
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::WriteCalibrationFileRef( TString outDir )
{     
   gDirectory->Cd( outDir );
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // TODO: for now only TRB has reference channel methods implemented
      if( toftdc::caenV1290 == uType || 
           toftdc::vftx == uType || 
           toftdc::get4 == uType )
            continue;

      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {
         UInt_t uNbChan  = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan  = caentdc::kuNbChan;
               break;
            case toftdc::vftx:
               uNbChan  = vftxtdc::kuNbChan;
               break;
            case toftdc::trb:
               uNbChan  = trbtdc::kuNbChan;
               break;
            case toftdc::get4:
               uNbChan  = get4tdc::kuNbChan;
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
         if( 0 == uNbChan  )
         {
            LOG(info)<<"TMbsCalibTdcTof::WriteCalibrationFileRef Undefined tdc parameters for type "
                     <<toftdc::ksTdcHistName[ uType ];
            LOG(info)<<" => No calib histo saving!";
            continue;
         }
         
         TString sInfoSaving = "Saved initial calibration histograms for following "
                               + toftdc::ksTdcHistName[ uType ] +" reference channel:";
         LOG(info)<<sInfoSaving;

         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         {
            sInfoSaving = Form("tdc #%3u: ",uTdc );
            TString sCalibHistoOutputName = Form("tof_%s_ft_b%03u_ref", toftdc::ksTdcHistName[ uType ].Data(), 
                                                   uTdc );
                 
            // New Calibration histo using only new data
            if( 0 < (fhFineTimeRef[ uType ][ uTdc ]->GetEntries() ) )
            {
               fhFineTimeRef[ uType ][ uTdc ]->Write( sCalibHistoOutputName, TObject::kOverwrite);
               sInfoSaving += "  1 ";
            } // if( 0 < (fhFineTime[ uType ][ uTdc ]->GetEntries() ) )
               else sInfoSaving += "  0 ";
            LOG(info)<<sInfoSaving;
         } // for( UInt_t uTdc = 0; uTdc < fMbsCalibPar->GetNbCalibBoards( uType ); uTdc ++)
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::WriteSingleCalibrationsRef()
{     
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // TODO: for now only TRB has reference channel methods implemented
      if( toftdc::caenV1290 == uType || 
           toftdc::vftx == uType || 
           toftdc::get4 == uType )
            continue;

      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {
         UInt_t uNbChan  = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan  = caentdc::kuNbChan;
               break;
            case toftdc::vftx:
               uNbChan  = vftxtdc::kuNbChan;
               break;
            case toftdc::trb:
               uNbChan  = trbtdc::kuNbChan;
               break;
            case toftdc::get4:
               uNbChan  = get4tdc::kuNbChan;
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
         if( 0 == uNbChan  )
         {
            LOG(info)<<"TMbsCalibTdcTof::WriteSingleCalibrationsRef Undefined tdc parameters for type "
                     <<toftdc::ksTdcHistName[ uType ];
            LOG(info)<<" => No calib histo saving!";
            continue;
         }
         
         TString sInfoSaving = "Saved initial calibration histograms for following "
                               + toftdc::ksTdcHistName[ uType ] +" reference channel:";
         LOG(info)<<sInfoSaving;

         // Save current directory as TFile opening change it!
         oldDir = gDirectory;
         
         // Check if initial histos arrays was created
         Bool_t bBoardInitialThere = kFALSE;
         if( fhInitialCalibHisto[ uType ].size() == fMbsUnpackPar->GetNbActiveBoards( uType )*uNbChan )
            bBoardInitialThere = kTRUE;
               
         for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
         {
            sInfoSaving = Form("tdc #%3u: ",uTdc );
               
            Bool_t bDataToSave = kFALSE;
            Bool_t bInitialThere = kFALSE;
            
            if( kTRUE == bBoardInitialThere )
               if( NULL != fhInitialCalibHistoRef[ uType ][ uTdc ] )
                  bInitialThere = kTRUE;
                  
            if( 0 < (fhFineTimeRef[ uType ][ uTdc ]->GetEntries() ) )
               bDataToSave = kTRUE;
                                          
            if( 1 == fMbsCalibPar->GetSingleCalOutMode() || kFALSE == bInitialThere )
            {
               if( kTRUE == bDataToSave )
               {
                  TString sCalibOutFilename = Form("./calib/%s_Tdc%03uRefChan.root", 
                                                      toftdc::ksTdcHistName[ uType ].Data(),
                                                      uTdc);
                  TFile* fileCalibrationOut = new TFile( sCalibOutFilename, "RECREATE",
                                                         Form("Calibration Data for %s TDC %03d Reference channel ", 
                                                         toftdc::ksTdcHistName[ uType ].Data(),
                                                         uTdc ),
                                                         9);
                  if( kTRUE == fileCalibrationOut->IsOpen() )
                  {
                     sCalibOutFilename += ":/";
                     gDirectory->Cd(sCalibOutFilename);
                     TString sCalibHistoOutputName = Form("tof_%s_ft_b%03u_ref", toftdc::ksTdcHistName[ uType ].Data(), 
                                                            uTdc );
                     fhFineTimeRef[ uType ][ uTdc ]->Write( sCalibHistoOutputName, TObject::kOverwrite);
                     sInfoSaving += "  1 ";
                     
                     //fileCalibrationOut->Write("",TObject::kOverwrite);
                     fileCalibrationOut->Close();
                     gDirectory->Cd(oldDir->GetPath());
                  } // if( kTRUE == fileCalibrationOut->IsOpen() )
                  else sInfoSaving += "  0 ";
               } // if( kTRUE == bDataToSave )
                  else sInfoSaving += "  0 ";
            } // if( 1 == fMbsCalibPar->GetSingleCalOutMode() || kFALSE == bInitialThere )
               else if( kTRUE == bDataToSave )
               {
                  // Update old calibration histo with new data
                  TString sCalibOutFilename = Form("./calib/%s_Tdc%03uRefChan.root ", 
                                                      toftdc::ksTdcHistName[ uType ].Data(),
                                                      uTdc );
                  TFile* fileCalibrationOut = new TFile( sCalibOutFilename, "RECREATE",
                                                         Form("Calibration Data for %s TDC %03u Reference", 
                                                         toftdc::ksTdcHistName[ uType ].Data(),
                                                         uTdc ),
                                                         9);
                  if( kTRUE == fileCalibrationOut->IsOpen() )
                  {
                     sCalibOutFilename += ":/";
                     gDirectory->Cd(sCalibOutFilename);
                     TString sCalibHistoOutputName = Form("tof_%s_ft_b%03d_ref", toftdc::ksTdcHistName[ uType ].Data(), 
                                                            fMbsCalibPar->GetInitialCalInd( uType, uTdc ) );
                     // Try to update old calibration histo with new data
                     fhInitialCalibHistoRef[ uType ][ uTdc ]->Add( fhFineTimeRef[ uType ][ uTdc ] );
                     fhInitialCalibHistoRef[ uType ][ uTdc ]->Write( sCalibHistoOutputName, TObject::kOverwrite);
                     sInfoSaving += "  1 ";
                     
                     //fileCalibrationOut->Write("",TObject::kOverwrite);
                     fileCalibrationOut->Close();
                     gDirectory->Cd(oldDir->GetPath());
                  } // if( kTRUE == fileCalibrationOut->IsOpen() )
                  else sInfoSaving += "  0 ";
               } // else of if( 1 == fMbsCalibPar->uSingleChannelCalibFilesOutput )
                  // No new Data => no need to update calibration!!!
                  else sInfoSaving += "  0 "; 

            LOG(info)<<sInfoSaving;
         } // for( UInt_t uTdc = 0; uTdc < fMbsCalibPar->GetNbCalibBoards( uType ); uTdc ++)
         
         // Go back to original directory
         gDirectory->Cd(oldDir->GetPath());
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::CalibrateReference(UInt_t uType, UInt_t uBoard)
{
   // TODO: Make it TRB specific: use TRB board and TRB data and reject all othe TDC types
   LOG(debug)<<"TMbsCalibTdcTof::CalibrateReference => Type "<<uType
         <<" Active bd "<<fMbsUnpackPar->GetNbActiveBoards( uType )
         <<" Board "<<uBoard;
   if( uBoard < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   {
      // TDC type specific values
//      UInt_t         uNbChan        = 0; // -> Comment to remove warning because set but never used
      TClonesArray * xUnpDataArray  = NULL;
      Double_t       dClockCycle    = 0.0;
      Bool_t         bInvertFt      = kFALSE;
/*      Double_t       dTotBinToPs    = 0.0;*/ // Commented out to remove warning bec. unused
//      Int_t          iCoarseSize    = 0; // -> Comment to remove warning because set but never used
//      Int_t          iCoarseOfLim   = 0; // -> Comment to remove warning because set but never used
      switch( uType )
      {
         case toftdc::caenV1290:
/*
            uNbChan        = caentdc::kuNbChan;
            xUnpDataArray  = fCaenBoardCollection;
            dClockCycle    = caentdc::kdClockCycleSize;
            bInvertFt      = caentdc::kbInvertFt;
            iCoarseSize    = caentdc::kiCoarseCounterSize;
            iCoarseOfLim   = caentdc::kuCoarseOverflowTest;
*/
            return kFALSE;
            break;
         case toftdc::vftx:
/*
            uNbChan        = vftxtdc::kuNbChan;
            xUnpDataArray  = fVftxBoardCollection;
            dClockCycle    = vftxtdc::kdClockCycleSize;
            bInvertFt      = vftxtdc::kbInvertFt;
            iCoarseSize    = vftxtdc::kiCoarseCounterSize;
            iCoarseOfLim   = vftxtdc::kuCoarseOverflowTest;
*/
            return kFALSE;
            break;
         case toftdc::trb:
//            uNbChan        = trbtdc::kuNbChan; // -> Comment to remove warning because set but never used
            xUnpDataArray  = fTrb3BoardCollection;
            dClockCycle    = trbtdc::kdClockCycleSize;
            bInvertFt      = trbtdc::kbInvertFt;
//            iCoarseSize    = trbtdc::kiCoarseCounterSize; // -> Comment to remove warning because set but never used
//            iCoarseOfLim   = trbtdc::kuCoarseOverflowTest; // -> Comment to remove warning because set but never used
            break;
         case toftdc::get4:
/*
            uNbChan        = get4tdc::kuNbChan;
            xUnpDataArray  = fGet4BoardCollection;
            dClockCycle    = get4tdc::kdClockCycleSize;
            bInvertFt      = get4tdc::kbInvertFt;
            dTotBinToPs    = get4tdc::kdTotBinSize;
            iCoarseSize    = get4tdc::kiCoarseCounterSize;
            iCoarseOfLim   = get4tdc::kuCoarseOverflowTest;
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
*/
            return kFALSE;
            break;
         default:
            return kFALSE;
            break;
      } // switch( uType )
      
      // Getting access to the  Board object and to the output object
      TTofCalibData * pCalibRef = (TTofCalibData*) fCalibRefCollection[uType]->ConstructedAt(uBoard);
      if( NULL == xUnpDataArray ||
          NULL == fCalibRefCollection[uType] ||
          0 == pCalibRef )
      {
         LOG(error)<<"TMbsCalibTdcTof::Calibration => Data pt failed Type "<<uType
               <<" Active bd "<<fMbsUnpackPar->GetNbActiveBoards( uType )
               <<" Board "<<uBoard<<" Unp array "<<xUnpDataArray<<" CalibRefCollection "<<(fCalibRefCollection[uType])
               <<" CalibRef object "<<pCalibRef;
         return kFALSE;
      }

      // Board raw data recovery
      TTofTrbTdcBoard * xBoardUnpData = (TTofTrbTdcBoard*) xUnpDataArray->At(uBoard);
            
      if( NULL == xBoardUnpData )
      {
         fdTdcReference[uType][uBoard] = -1.0;
         LOG(error)<<"TMbsCalibTdcTof::Calibration => Bd pt failed Type "<<uType
               <<" Active bd "<<fMbsUnpackPar->GetNbActiveBoards( uType )
               <<" Board "<<uBoard;
         return kFALSE;
      } // if( NULL == xBoardUnpData )

      if( kFALSE == xBoardUnpData->IsValid() )
      {
         fdTdcReference[uType][uBoard] = -1.0;
         LOG(debug)<<"TMbsCalibTdcTof::Calibration => Bd data invalid Type "<<uType
               <<" Active bds "<<fMbsUnpackPar->GetNbActiveBoards( uType )
               <<" Board "<<uBoard;
         return kFALSE;
      } // if( kFALSE == xBoardUnpData->IsValid() )

      TTofTrbTdcData& xUnpDataRef = xBoardUnpData->GetRefChannelData(); // 

      // In order to avoid big numbers after conversion to double float
      // (precision loss), subtract part of the main board trigger time counter
      Int_t iMainTriggerCoarseTime = 0;
      if( NULL != (TTofTdcBoard*) xUnpDataArray->At(fMbsCalibPar->GetTdcOffsetMainTdc()) )
         if( kTRUE == ( (TTofTdcBoard*) xUnpDataArray->At(fMbsCalibPar->GetTdcOffsetMainTdc()) )->IsValid() )
            if( toftdc::trb == uType )
            {
               TTofTrbTdcData& xUnpDataRefMain = ((TTofTrbTdcBoard*) xUnpDataArray->At(
                                               fMbsCalibPar->GetTdcOffsetMainTdc() ))->GetRefChannelData();
               iMainTriggerCoarseTime = xUnpDataRefMain.GetCoarseTime();
            }
      
      Int_t    iCoarseTime = xUnpDataRef.GetCoarseTime();
      if( kTRUE == fMbsCalibPar->UseCoarse() )
      {
         // In order to avoid big numbers after conversion to double float
         // (precision loss), subtract part of the main board trigger time counter
         // Use for now 128 clock cycle (~600ns for trb)
//         iCoarseTime -= iMainTriggerCoarseTime & 0x7FFFFF80;
         iCoarseTime -= iMainTriggerCoarseTime & 0x7FFFFFFF;
      } // if( kTRUE == fMbsCalibPar->UseCoarse()
      fdTdcReference[uType][uBoard]  = dClockCycle * iCoarseTime;
      //TODO: The total coarse time range (including the epoch counter) spans
      //      values between [0, 2^39-1] or [0, 5.5e11]. The double floating
      //      point precision is 1,1e-16. The coarse counter granularity is 5 ns.
      //      The maximum time measurable by a TRB-TDC, 5.5e11*5 ns = 2.7e12 ns,
      //      would have a maximum absolute error of 0.3 ps if stored in a
      //      double floating point variable which is sufficient for our calculations.

      Int_t iNbEntriesFtHist = fhFineTimeRef[uType][uBoard]->GetEntries();
      if( NULL != fhInitialCalibHistoRef[uType][uBoard])
         iNbEntriesFtHist += fhInitialCalibHistoRef[uType][uBoard]->GetEntries();
      Int_t iNbHitsThisCh    = fhFineTimeRef[uType][uBoard]->GetEntries();

      // FIXME: for TRB TDC, first try to prevent invalid FT from being processed
      if( toftdc::trb != uType || 0x3ff != xUnpDataRef.GetFineTime() )
      {
         if( 0 < fMbsCalibPar->GetMinHitCalib() &&
             iNbEntriesFtHist < fMbsCalibPar->GetMinHitCalib() )
         {
            // Use initial calibration values or linear calibration with data limits for TRB-TDC
            if( toftdc::trb == uType && kTRUE == fMbsCalibPar->IsTrbLinCalEna() )
            {
               if( kTRUE == bInvertFt )
                  fdTdcReference[uType][uBoard] += dClockCycle
                                                   * ( 1 -  (Double_t)(xUnpDataRef.GetFineTime() - fiLinCalRefMinBin[uType][uBoard])
                                                          / (Double_t)(fiLinCalRefMaxBin[uType][uBoard] - fiLinCalRefMinBin[uType][uBoard]) );
                  else fdTdcReference[uType][uBoard] += dClockCycle
                                                        * (Double_t)(xUnpDataRef.GetFineTime() - fiLinCalRefMinBin[uType][uBoard])
                                                        / (Double_t)(fiLinCalRefMaxBin[uType][uBoard] - fiLinCalRefMinBin[uType][uBoard]);
            } // if( toftdc::trb == uType && kTRUE == fMbsCalibPar->IsTrbLinCalEna())
         } // else if( iNbEntriesFtHist < fMbsCalibPar->GetMinHitCalib() )
         else
         {
            // Use standard calibration method
            if( kTRUE == bInvertFt )
               fdTdcReference[uType][uBoard] += dClockCycle *( 1 - fdCorrRef[uType][uBoard][xUnpDataRef.GetFineTime()] );
               else fdTdcReference[uType][uBoard] += dClockCycle * fdCorrRef[uType][uBoard][xUnpDataRef.GetFineTime()];
         } // else of if( iNbEntriesFtHist < fMbsCalibPar->GetMinHitCalibNewOnly() )

         pCalibRef->SetType(    uType );      // For consistency cross check
         pCalibRef->SetBoard(   uBoard );     // For consistency cross check
         pCalibRef->SetChannel( 0xFFFFFFFF ); // Fixed value for consistency cross check
         pCalibRef->SetEdge(    3 );          // For consistency cross check (0 = Rising, 1 = Falling, 2 = Full Chan )
         pCalibRef->SetTime(    fdTdcReference[uType][uBoard] ); // Reference time
         pCalibRef->SetTot(     -1.0 ); // Fixed value for cross check
         pCalibRef->SetFilled();
      } // if( toftdc::trb != uType || 0x3ff != xUnpDataRef.GetFineTime() )
       
      if( 0 < fMbsCalibPar->GetMinHitCalib() )
      {         
         // Check if initial calibration histograms are available
         Bool_t bInitialThere = kFALSE;
         if( kTRUE == bInitialThere )
            if( NULL != fhFineTimeRef[ uType ][ uBoard ] )
               bInitialThere = kTRUE;
         //TODO: The above piece of code is never executed!!!

         // Check whether we should use the initial calibration on top of the new data
         Bool_t bUseInitial = bInitialThere;
         if( 0 < fMbsCalibPar->GetMinHitCalibNewOnly() &&
             fMbsCalibPar->GetMinHitCalibNewOnly() <= iNbHitsThisCh )
             bUseInitial = kFALSE;
               
         if( 0 < iNbHitsThisCh &&
             0 == iNbHitsThisCh % fMbsCalibPar->GetMinHitCalib() )
            CalibFactorsCalcReference( uType, uBoard, bUseInitial);
      } // if( 0 < fMbsCalibPar->GetMinHitCalib() )

      // Update limits with real counts for linear calibration
      if( 1000 < iNbHitsThisCh &&
          iNbHitsThisCh < fMbsCalibPar->GetMinHitCalib() ) // <= To use for limits updates
      {
         Int_t iBin = 0;
         Int_t iNbCounts = 0;

         while( 0 == fhFineTimeRef[uType][uBoard]->GetBinContent(iBin) )
            iBin++;
         fiLinCalRefMinBin[uType][uBoard] = iBin;

         iNbCounts = fhFineTimeRef[uType][uBoard]->GetBinContent(iBin);
         while( fhFineTimeRef[uType][uBoard]->GetEntries() > iNbCounts )
         {
            iBin++;
            iNbCounts += fhFineTimeRef[uType][uBoard]->GetBinContent(iBin);
         } // while( fhFineTimeRef[uType][uBoard]->GetEntries() > iNbCounts )
         fiLinCalRefMaxBin[uType][uBoard] = iBin;
      } // if( 1000 < iNbEntriesFtHist) // <= To use for limits updates

   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      else return kFALSE;

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::ClearTdcReference()
{
   // Loop over all TDC types
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
      {
         fdTdcOffsets[ uType ][ uTdc ] = 0.0;
         fdTdcReference[uType][uTdc] = -1;
      } // for( UInt_t uTdc = 0; uTdc < fMbsUnpackPar->GetNbActiveBoards( uType ); uTdc ++)
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::TdcOffsetCalc(UInt_t uType, UInt_t uBoard)
{
   LOG(debug)<<"TMbsCalibTdcTof::TdcOffsetCalc => Type "<<uType
         <<" Active bd "<<fMbsUnpackPar->GetNbActiveBoards( uType )
         <<" Board "<<uBoard;
   if( uBoard < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   {
      if( -1 != fdTdcReference[uType][uBoard]  &&
          -1 != fdTdcReference[ uType ][ fMbsCalibPar->GetTdcOffsetMainTdc() ] )
      {
         // TDC type specific values
         Double_t       dClockCycle    = 0.0;
         switch( uType )
         {
            case toftdc::caenV1290:
               return kFALSE;
               break;
            case toftdc::vftx:
               return kFALSE;
               break;
            case toftdc::trb:
               dClockCycle    = trbtdc::kdClockCycleSize;
               break;
            case toftdc::get4:
               return kFALSE;
               break;
            default:
               return kFALSE;
               break;
         } // switch( uType )
/*
         fdTdcOffsets[ uType ][ uBoard ] = fdTdcReference[ uType ][ uBoard ]
                                         - fdTdcReference[ uType ][ fMbsCalibPar->GetTdcOffsetMainTdc() ];
*/
         // New method to detect only offset modulo the clock cycle
         fdTdcOffsets[ uType ][ uBoard ] = dClockCycle * (Int_t)(
                                           (  fdTdcReference[ uType ][ uBoard ]
                                            - fdTdcReference[ uType ][ fMbsCalibPar->GetTdcOffsetMainTdc() ]
                                            + dClockCycle/2
                                           )/(dClockCycle) 
                                           + (fdTdcReference[ uType ][ uBoard ]
                                               < fdTdcReference[ uType ][ fMbsCalibPar->GetTdcOffsetMainTdc()]? -1 : 0) );
          LOG(debug)<<"TMbsCalibTdcTof::TdcOffsetCalc => Type "<<uType
            <<" Ref  "    <<fdTdcReference[ uType ][ uBoard ]
            <<" Main Ref "<<fdTdcReference[ uType ][ fMbsCalibPar->GetTdcOffsetMainTdc() ]
            <<" Offset "  <<fdTdcOffsets[ uType ][ uBoard ];
      } // if both values OK
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      else
      {
         fdTdcOffsets[ uType ][ uBoard ] = 0.0;
         return kFALSE;
      } // else of if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::CreateReferenceHistogramms()
{
   // Loop over all TDC types
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // TODO: for now only TRB has reference channel methods implemented
      if( toftdc::caenV1290 == uType || 
           toftdc::vftx == uType || 
           toftdc::get4 == uType )
{
            LOG(info)<<"TMbsCalibTdcTof::CreateReferenceHistogramms => Type "<<uType
                  <<" not allowed ";  
            continue;
}

      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {    
/*  // -> Comment to remove warning because set but never used
         UInt_t   uNbChan     = 0;
         Double_t dClockCycle = 0.0;
         UInt_t   uFtBinNb    = 0;
         UInt_t   uMaxMul     = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan     = caentdc::kuNbChan;
               dClockCycle = caentdc::kdClockCycleSize;
               uFtBinNb    = caentdc::kiFineTime + 1;
               uMaxMul     = toftdc::kuDefNbMulti; // TODO: add proper value for this type
               break;
            case toftdc::vftx:
               uNbChan     = vftxtdc::kuNbChan;
               dClockCycle = vftxtdc::kdClockCycleSize;
               uFtBinNb    = vftxtdc::kiFifoFineTime + 1;
               uMaxMul     = vftxtdc::kuNbMulti;
               break;
            case toftdc::trb:
               uNbChan     = trbtdc::kuNbChan;
               dClockCycle = trbtdc::kdClockCycleSize;
               uFtBinNb    = trbtdc::kiFineCounterSize;
               uMaxMul     = toftdc::kuDefNbMulti; // TODO: add proper value for this type
               break;
            case toftdc::get4:
               uNbChan     = get4tdc::kuNbChan;
               dClockCycle = get4tdc::kdClockCycleSize;
               uFtBinNb    = get4tdc::kiFineTime + 1;
               uMaxMul     = toftdc::kuDefNbMulti; // TODO: add proper value for this type
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
*/
         LOG(info)<<"TMbsCalibTdcTof::CreateReferenceHistogramms => Create Histo for Type "<<uType
                  <<" Nb active boards: " << fMbsUnpackPar->GetNbActiveBoards( uType )
                  <<" Name pattern: " << toftdc::ksTdcHistName[ uType ].Data(); 
         
         // Reference histograms initialization
         Int_t iNbBoards = fMbsUnpackPar->GetNbActiveBoards( uType );
         fhTdcReferenceComp[uType].resize( iNbBoards - 1, NULL );
         if( -1 < fMbsCalibPar->GetChanResTest() )
         {
            fhTdcResolutionTest[uType].resize( iNbBoards - 1, NULL );
            fhTdcTestToRef[uType].resize( iNbBoards, NULL );
         } // if( -1 < fMbsCalibPar->GetChanResTest() )
         for( Int_t iTdc = 0; iTdc < iNbBoards - 1; iTdc ++)
         {
            if( -1 < fMbsCalibPar->GetChanResTest() )
               fhTdcReferenceComp[uType][iTdc] =  new TH2I(
                              Form("tof_%s_RefChComp_b%03d", toftdc::ksTdcHistName[ uType ].Data(), iTdc),
                              Form("Comparison of the reference channels of %s TDC #%03d to all other TDC of same type; TDC#n []; Ref(TDC#n) -Ref(TDC #%03d)", 
                                 toftdc::ksTdcHistName[ uType ].Data(), iTdc, iTdc),
                              iNbBoards -1 -iTdc, iTdc +1 -0.5, iNbBoards -0.5,
                              10000, -50000, 50000 );
               else fhTdcReferenceComp[uType][iTdc] =  new TH2I(
                              Form("tof_%s_RefChComp_b%03d", toftdc::ksTdcHistName[ uType ].Data(), iTdc),
                              Form("Comparison of the reference channels of %s TDC #%03d to all other TDC of same type; TDC#n []; Ref(TDC#n) -Ref(TDC #%03d)",
                                 toftdc::ksTdcHistName[ uType ].Data(), iTdc, iTdc),
                              iNbBoards -1 -iTdc, iTdc +1 -0.5, iNbBoards -0.5,
                              80000, -800000, 800000 );
            if( -1 < fMbsCalibPar->GetChanResTest() )
               fhTdcResolutionTest[uType][iTdc] =  new TH2I( 
                              Form("tof_%s_ResoTest_b%03d", toftdc::ksTdcHistName[ uType ].Data(), iTdc),
                              Form("Comparison of the channel %03d of %s TDC #%03d to all other TDC of same type; TDC#n []; t(TDC#n) -t(TDC #%03d)", 
                                 fMbsCalibPar->GetChanResTest(), toftdc::ksTdcHistName[ uType ].Data(), iTdc, iTdc),
                              iNbBoards -1 -iTdc, iTdc +1 -0.5, iNbBoards -0.5,
                              10000, -50000, 50000 );
         LOG(debug2)<<"TMbsCalibTdcTof::CreateReferenceHistogramms => Create Histo "
                  <<Form("tof_%s_RefChComp_b%03d", toftdc::ksTdcHistName[ uType ].Data(), iTdc)
                  <<" at " << fhTdcReferenceComp[uType][iTdc]
                  <<" found at "<< (TH2 *)(gROOT->FindObjectAny(Form("tof_trb_RefChComp_b%03d", iTdc))); 
         } // for( Int_t iTdc = 0; iTdc < fMbsUnpackPar->GetNbActiveBoards( uType ) - 1; iTdc ++)

         fhTdcOffsetFirstEvent[uType] =  new TProfile(
               Form("tof_%s_OffsetFirstEvent", toftdc::ksTdcHistName[ uType ].Data()),
               Form("Comparison of the reference channels of %s TDC #001 to all other TDC of same type in first event; TDC#n []; Ref(TDC#n) -Ref(TDC #001)",
                  toftdc::ksTdcHistName[ uType ].Data() ),
               iNbBoards , -0.5, iNbBoards -0.5);

         if( -1 < fMbsCalibPar->GetChanResTest() )
            for( Int_t iTdc = 0; iTdc < iNbBoards; iTdc ++)
            {
               fhTdcTestToRef[uType][iTdc] =  new TH1I( 
                              Form("tof_%s_TestToRef_b%03d", toftdc::ksTdcHistName[ uType ].Data(), iTdc),
                              Form("Comparison of the channel %03d of %s TDC #%03d to reference channel of the same board; TDC#n []; t(ch %03d) -t(ref)", 
                                 fMbsCalibPar->GetChanResTest(), toftdc::ksTdcHistName[ uType ].Data(), iTdc, fMbsCalibPar->GetChanResTest()),
                              2000, -400000, 400000 );
            } // for( Int_t iTdc = 0; iTdc < iNbBoards; iTdc ++)
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      else LOG(info)<<"TMbsCalibTdcTof::CreateReferenceHistogramms => Type "<<uType
                  <<" no active boards: " << fMbsUnpackPar->GetNbActiveBoards( uType );  
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::FillReferenceHistograms()
{
   // Loop over all TDC types
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // TODO: for now only TRB has reference channel methods implemented
      if( toftdc::caenV1290 == uType || 
           toftdc::vftx == uType || 
           toftdc::get4 == uType )
            continue;

      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {    
/*  // -> Comment to remove warning because set but never used
         UInt_t   uNbChan     = 0;
         Double_t dClockCycle = 0.0;
         UInt_t   uFtBinNb    = 0;
         UInt_t   uMaxMul     = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan     = caentdc::kuNbChan;
               dClockCycle = caentdc::kdClockCycleSize;
               uFtBinNb    = caentdc::kiFineTime + 1;
               uMaxMul     = toftdc::kuDefNbMulti; // TODO: add proper value for this type
               break;
            case toftdc::vftx:
               uNbChan     = vftxtdc::kuNbChan;
               dClockCycle = vftxtdc::kdClockCycleSize;
               uFtBinNb    = vftxtdc::kiFifoFineTime + 1;
               uMaxMul     = vftxtdc::kuNbMulti;
               break;
            case toftdc::trb:
               uNbChan     = trbtdc::kuNbChan;
               dClockCycle = trbtdc::kdClockCycleSize;
               uFtBinNb    = trbtdc::kiFineCounterSize;
               uMaxMul     = toftdc::kuDefNbMulti; // TODO: add proper value for this type
               break;
            case toftdc::get4:
               uNbChan     = get4tdc::kuNbChan;
               dClockCycle = get4tdc::kdClockCycleSize;
               uFtBinNb    = get4tdc::kiFineTime + 1;
               uMaxMul     = toftdc::kuDefNbMulti; // TODO: add proper value for this type
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
*/

         // Reference histograms initialization
         Int_t iNbBoards = fMbsUnpackPar->GetNbActiveBoards( uType );

         // Try to compensate fixed offsets between boards
         if( kTRUE == fbFirstEventPassedRef[uType])
         {
            for( Int_t iTdcA = 0; iTdcA < iNbBoards - 1; iTdcA ++)
               if( -1 != fdTdcReference[uType][iTdcA] )
            {
               for( Int_t iTdcB = iTdcA + 1; iTdcB < iNbBoards; iTdcB ++)
                  if( -1 != fdTdcReference[uType][iTdcB] )
               {
                  fhTdcReferenceComp[uType][iTdcA]->Fill( iTdcB,
                        (fdTdcReference[uType][iTdcB] - fdTdcReference[uType][iTdcA])
                        - ( kTRUE == fMbsCalibPar->GetTdcOffEnaFlag(uType, iTdcB) && kTRUE == fMbsCalibPar->GetTdcOffEnaFlag(uType, iTdcA) ? 
                            (fdTdcOffsets[ uType ][ iTdcB ] - fdTdcOffsets[ uType ][ iTdcA ] ) : 
                            0.0 )
                          );
//                        - (fdTdcReferenceFirstEvent[uType][iTdcB] - fdTdcReferenceFirstEvent[uType][iTdcA]) );

                  LOG(debug)<<"TMbsCalibTdcTof::FillReferenceHistograms => Type "<<uType
                        <<" Board A "<<iTdcA<<" Board B "<<iTdcB
                        <<" T A "<<fdTdcReference[uType][iTdcA]<<" T B "<<fdTdcReference[uType][iTdcB]
                        <<" Off A "<<fdTdcOffsets[ uType ][ iTdcA ]<<" Off B "<<fdTdcOffsets[ uType ][ iTdcB ]
                        <<" First A "<<fdTdcReferenceFirstEvent[ uType ][ iTdcA ]<<" First B "<<fdTdcReferenceFirstEvent[ uType ][ iTdcB ];
               }
            } // for( Int_t iTdcA = 0; iTdcA < fMbsUnpackPar->GetNbActiveBoards( uType ) - 1; iTdcA ++)

            /*
            if( 0 < fdTdcReference[uType][14] )
               for( Int_t iTdcA = 0; iTdcA < iNbBoards; iTdcA ++)
                  if( 0 < fdTdcReference[uType][iTdcA] )
                     fhTdcOffsetFirstEvent[uType]->Fill(iTdcA,
                        fdTdcReference[uType][iTdcA] - fdTdcReference[uType][14]);
            */
         } // if( kTRUE == fbFirstEventPassedRef[uType])
         else
         {
            for( Int_t iTdcA = 0; iTdcA < iNbBoards; iTdcA ++)
               if( -1 != fdTdcReference[uType][iTdcA] )
               fdTdcReferenceFirstEvent[uType][iTdcA] = fdTdcReference[uType][iTdcA];

            if( -1 != fdTdcReference[uType][fMbsCalibPar->GetTdcOffsetMainTdc()] )
               for( Int_t iTdcA = 0; iTdcA < iNbBoards; iTdcA ++)
                  if( -1 != fdTdcReference[uType][iTdcA] )
                     fhTdcOffsetFirstEvent[uType]->Fill(iTdcA,
                        fdTdcReference[uType][iTdcA] 
                        - fdTdcReference[uType][fMbsCalibPar->GetTdcOffsetMainTdc()]);

            fbFirstEventPassedRef[uType] = kTRUE;
         } // else of if( kTRUE == fbFirstEventPassedRef[uType])

         if( -1 < fMbsCalibPar->GetChanResTest() )
         {
            Bool_t   bChannelFound[iNbBoards];
            Double_t dTimeDebugCheck[iNbBoards];
            for( Int_t iTdcA = 0; iTdcA < iNbBoards - 1; iTdcA ++)
            {
               bChannelFound[iTdcA] = kFALSE;
               dTimeDebugCheck[iTdcA] = 0.0;
            } // for( Int_t iTdcA = 0; iTdcA < fMbsUnpackPar->GetNbActiveBoards( uType ) - 1; iTdcA ++)
            for( Int_t iDataIndex = 0; iDataIndex < fCalibDataCollection->GetEntriesFast() ; iDataIndex++ )
            {
               TTofCalibData * fCalibData;
               fCalibData = (TTofCalibData *)fCalibDataCollection->At( iDataIndex );
               
               UInt_t   uTypeData = fCalibData->GetType();
               UInt_t   uChan = fCalibData->GetChannel();

               if( fMbsCalibPar->GetChanResTest() != (Int_t)uChan ||
                   uType != uTypeData )
                  continue;

               UInt_t   uTdc  = fCalibData->GetBoard();

               if( kTRUE == bChannelFound[uTdc] )
                  continue;

               Double_t dTime = fCalibData->GetTime();
/*               Double_t dTot  = fCalibData->GetTot(); */ // Commented out to remove warning bec. unused

               bChannelFound[uTdc] = kTRUE;
               dTimeDebugCheck[uTdc] = dTime;

            } // for( Int_t iDataIndex = 0; iDataIndex < fCalibDataCollection->GetEntriesFast() ; iDataIndex++ )
            for( Int_t iTdcA = 0; iTdcA < iNbBoards - 1; iTdcA ++)
            {
               for( Int_t iTdcB = iTdcA + 1; iTdcB < iNbBoards; iTdcB ++)
                  if( kTRUE == bChannelFound[iTdcA] && kTRUE == bChannelFound[iTdcB] )
                  {
                     fhTdcResolutionTest[uType][iTdcA]->Fill( iTdcB, dTimeDebugCheck[iTdcB] - dTimeDebugCheck[iTdcA]);
                  } // if( kTRUE == bChannelFound[iTdcA] && kTRUE == bChannelFound[iTdcB] )
            } // for( Int_t iTdcA = 0; iTdcA < fMbsUnpackPar->GetNbActiveBoards( uType ) - 1; iTdcA ++)
            for( Int_t iTdc = 0; iTdc < iNbBoards; iTdc ++)
               if( -1 != fdTdcReference[uType][iTdc] )
               fhTdcTestToRef[uType][iTdc]->Fill( dTimeDebugCheck[iTdc] - fdTdcReference[uType][iTdc]);
         } // if( -1 < fMbsCalibPar->GetChanResTest() )

      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )

   return kTRUE;
}
Bool_t TMbsCalibTdcTof::WriteReferenceHistogramms( TDirectory* inDir)
{
   TDirectory * oldir = gDirectory;
   
   for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
   {
      // TODO: for now only TRB has reference channel methods implemented
      if( toftdc::caenV1290 == uType || 
           toftdc::vftx == uType || 
           toftdc::get4 == uType )
            continue;

      if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
      {
/*  // -> Comment to remove warning because set but never used
         UInt_t uNbChan  = 0;
         
         switch( uType )
         {
            case toftdc::caenV1290:
               uNbChan  = caentdc::kuNbChan;
               break;
            case toftdc::vftx:
               uNbChan  = vftxtdc::kuNbChan;
               break;
            case toftdc::trb:
               uNbChan  = trbtdc::kuNbChan;
               break;
            case toftdc::get4:
               uNbChan  = get4tdc::kuNbChan;
               if( 4 == fMbsCalibPar->GetTotMode( uType ) )
                  uNbChan  = get4v10::kuNbChan;
               break;
            default:
               break;
         } // switch( uType )
*/

         // create a subdirectory "Cal_type" in this file
         TDirectory *cdRef = inDir->mkdir( Form( "Ref_%s", toftdc::ksTdcHistName[ uType ].Data() ) );
         cdRef->cd();    // make the "Cal_type" directory the current directory
         
         // loop over active TDCs
         Int_t iNbBoards = fMbsUnpackPar->GetNbActiveBoards( uType );
         for( Int_t iTdcA = 0; iTdcA < iNbBoards - 1; iTdcA ++)
         {
            fhTdcReferenceComp[uType][iTdcA]->Write();

            if( -1 < fMbsCalibPar->GetChanResTest() )
               fhTdcResolutionTest[uType][iTdcA]->Write();
         } // for( Int_t iTdcA = 0; iTdcA < iNbBoards - 1; iTdcA ++)
         fhTdcOffsetFirstEvent[uType]->Write();

         if( -1 < fMbsCalibPar->GetChanResTest() )
            for( Int_t iTdc = 0; iTdc < iNbBoards; iTdc ++)
               fhTdcTestToRef[uType][iTdc]->Write();
      } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( uType ) )
   } // for( UInt_t uType = toftdc::caenV1290; uType < toftdc::NbTdcTypes; uType++ )
      
   LOG(info)<<"TMbsCalibTdcTof::WriteReferenceHistogramms Done!";
   gDirectory->cd( oldir->GetPath() );
   return kTRUE;
}
Bool_t TMbsCalibTdcTof::DeleteReferenceHistograms()
{
   return kTRUE;
}
