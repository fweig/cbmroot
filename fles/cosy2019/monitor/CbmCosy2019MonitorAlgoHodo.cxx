// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmCosy2019MonitorAlgoHodo                       -----
// -----               Created 03.07.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmCosy2019MonitorAlgoHodo.h"

#include "CbmFormatMsHeaderPrintout.h"

//#include "CbmCosy2019HodoPar.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TList.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TString.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include <stdint.h>

// -------------------------------------------------------------------------
CbmCosy2019MonitorAlgoHodo::CbmCosy2019MonitorAlgoHodo()
  : CbmStar2019Algo()
  ,
  /// From the class itself
  fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fvbMaskedComponents()
  , fuDpbId(0x5b75)
  , fvuElinkIdxHodo(kuNbHodos, 0)
  , fvbHodoSwapXY(kuNbHodos, kFALSE)
  , fvbHodoInvertX(kuNbHodos, kFALSE)
  , fvbHodoInvertY(kuNbHodos, kFALSE)
  , fvuAdcGainHodo(kuNbHodos, 0)
  , fvuAdcOffsHodo(kuNbHodos, 0)
  ,
  /*
   fUnpackPar( nullptr ),
   fuNbModules( 0 ),
   fviModuleType(),
   fviModAddress(),
   fuNrOfDpbs( 0 ),
   fDpbIdIndexMap(),
   fvbCrobActiveFlag(),
   fuNbFebs( 0 ),
   fuNbHodoXyters( 0 ),
   fviFebModuleIdx(),
   fviFebModuleSide(),
   fviFebType(),
   fvdFebAdcGain(),
   fvdFebAdcOffs(),
   fviFebAddress(),
*/
  fdTimeOffsetNs(0.0)
  , fulCurrentTsIdx(0)
  , fulCurrentMsIdx(0)
  , fdTsStartTime(-1.0)
  , fdTsStopTimeCore(-1.0)
  , fvdPrevMsTime(kiMaxNbFlibLinks, -1.0)
  , fdMsTime(-1.0)
  , fuMsIndex(0)
  , fmMsgCounter()
  , fuCurrentEquipmentId(0)
  , fuCurrDpbId(0)
  , fiRunStartDateTimeSec(-1)
  , fiBinSizeDatePlots(-1)
  , fvulCurrentTsMsb()
  , fdStartTime(-1.0)
  , fdStartTimeMsSz(-1.0)
  , ftStartTimeUnix(std::chrono::steady_clock::now())
  , fvmHitsInMs()
  , fhHodoMessType(nullptr)
  , fhHodoStatusMessType(nullptr)
  , fhHodoMsStatusFieldType(nullptr)
  , fhHodoMessTypePerElink(nullptr)
  , fhHodoChanCntRaw(kuNbHodos, nullptr)
  , fhHodoChanAdcRaw(kuNbHodos, nullptr)
  , fhHodoChanAdcRawProf(kuNbHodos, nullptr)
  , fhHodoChanAdcCal(kuNbHodos, nullptr)
  , fhHodoChanAdcCalProf(kuNbHodos, nullptr)
  , fhHodoChanRawTs(kuNbHodos, nullptr)
  , fhHodoChanMissEvt(kuNbHodos, nullptr)
  , fhHodoChanMissEvtEvo(kuNbHodos, nullptr)
  , fhHodoChanHitRateEvo(kuNbHodos, nullptr)
  , fhHodoChanHitRateProf(kuNbHodos, nullptr)
  , fhHodoChanDistT(kuNbHodos, nullptr)
  , fhHodoFiberCnt(kuNbHodos, std::vector<TH1*>(kuNbAxis, nullptr))
  , fhHodoFiberAdc(kuNbHodos, std::vector<TH2*>(kuNbAxis, nullptr))
  , fhHodoFiberAdcProf(kuNbHodos, std::vector<TProfile*>(kuNbAxis, nullptr))
  , fhHodoFiberHitRateEvo(kuNbHodos, std::vector<TH2*>(kuNbAxis, nullptr))
  , fhHodoFiberCoincMapXY(kuNbHodos, nullptr)
  , fhHodoFiberCoincTimeXY(kuNbHodos, nullptr)
  , fhHodoFiberCoincWalkXY_X(kuNbHodos, nullptr)
  , fhHodoFiberCoincWalkXY_Y(kuNbHodos, nullptr)
  , fhHodoFiberCoincMapSameAB(kuNbAxis, nullptr)
  , fhHodoFiberCoincTimeSameAB(kuNbAxis, nullptr)
  , fhHodoFiberCoincMapDiffAB(kuNbAxis, nullptr)
  , fhHodoFiberCoincTimeDiffAB(kuNbAxis, nullptr)
  , fhHodoFullCoincPosA(nullptr)
  , fhHodoFullCoincPosB(nullptr)
  , fhHodoFullCoincCompX(nullptr)
  , fhHodoFullCoincCompY(nullptr)
  , fhHodoFullCoincResidualXY(nullptr)
  , fhHodoFullCoincTimeDiff(nullptr)
  , fhHodoFullCoincTimeWalk(kuNbHodos, std::vector<TH2*>(kuNbAxis, nullptr))
  , fhHodoFullCoincRateEvo(nullptr)
  , fhHodoFullCoincPosEvo(kuNbHodos, std::vector<TH2*>(kuNbAxis, nullptr))
  , fhPrevHitDtAllAsics(nullptr)
  , fhPrevHitDtAsicA(nullptr)
  , fhPrevHitDtAsicB(nullptr)
  , fhPrevHitDtAsicsAB(nullptr)
  , fiTimeIntervalRateUpdate(-1)
  , fviTimeSecLastRateUpdate(kuNbHodos, 0)
  , fvdChanCountsSinceLastRateUpdate(kuNbHodos, std::vector<Double_t>(kuNbChanPerAsic, 0.0))
  , fdHodoChanLastTimeForDist(kuNbHodos, std::vector<Double_t>(kuNbChanPerAsic, 0.0))
  , fuPreviousHitAsic(0)
  , fvdPreviousHitTimePerAsic(2, 0.0)
  , fcSummary(nullptr)
  , fcHodoSummaryRaw(kuNbHodos, nullptr)
  , fcHodoSummaryFiber(kuNbHodos, nullptr)
  , fcHodoFiberCoinc(nullptr)
  , fcHodoFiberCoincAB(nullptr)
  , fcHodoFullCoinc(nullptr)
  , fcHodoFullCoincPos(nullptr)
  , fcHodoPrevHitDt(nullptr)
{
}
CbmCosy2019MonitorAlgoHodo::~CbmCosy2019MonitorAlgoHodo()
{
  /// Clear buffers
  fvmHitsInMs.clear();
}

// -------------------------------------------------------------------------
Bool_t CbmCosy2019MonitorAlgoHodo::Init()
{
  LOG(info) << "Initializing mCBM HODO 2019 monitor algo";

  return kTRUE;
}
void CbmCosy2019MonitorAlgoHodo::Reset() {}
void CbmCosy2019MonitorAlgoHodo::Finish()
{
  /// Printout Goodbye message and stats

  /// Write Output histos
}

// -------------------------------------------------------------------------
Bool_t CbmCosy2019MonitorAlgoHodo::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmCosy2019MonitorAlgoHodo";
  Bool_t initOK = ReInitContainers();

  return initOK;
}
Bool_t CbmCosy2019MonitorAlgoHodo::ReInitContainers()
{
  LOG(info) << "**********************************************";
  LOG(info) << "ReInit parameter containers for CbmCosy2019MonitorAlgoHodo";
  /*
   fUnpackPar = (CbmCosy2019HodoPar*)fParCList->FindObject("CbmCosy2019HodoPar");
   if( nullptr == fUnpackPar )
      return kFALSE;
*/
  Bool_t initOK = InitParameters();

  return initOK;
}
TList* CbmCosy2019MonitorAlgoHodo::GetParList()
{
  if (nullptr == fParCList) fParCList = new TList();
  /*
   fUnpackPar = new CbmCosy2019HodoPar("CbmCosy2019HodoPar");
   fParCList->Add(fUnpackPar);
*/
  return fParCList;
}
Bool_t CbmCosy2019MonitorAlgoHodo::InitParameters()
{
  /*
   fuNbModules   = fUnpackPar->GetNbOfModules();
   LOG(info) << "Nr. of STS Modules:    " << fuNbModules;

   fviModuleType.resize( fuNbModules );
   fviModAddress.resize( fuNbModules );
   for( UInt_t uModIdx = 0; uModIdx < fuNbModules; ++uModIdx)
   {
      fviModuleType[ uModIdx ] = fUnpackPar->GetModuleType( uModIdx );
      fviModAddress[ uModIdx ] = fUnpackPar->GetModuleAddress( uModIdx );
      LOG(info) << "Module #" << std::setw(2) << uModIdx
                << " Type " << std::setw(4)  << fviModuleType[ uModIdx ]
                << " Address 0x" << std::setw(8) << std::hex <<fviModAddress[ uModIdx ]
                << std::dec;
   } // for( UInt_t uModIdx = 0; uModIdx < fuNbModules; ++uModIdx)

   fuNrOfDpbs = fUnpackPar->GetNrOfDpbs();
   LOG(info) << "Nr. of STS DPBs:       " << fuNrOfDpbs;

   fDpbIdIndexMap.clear();
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fDpbIdIndexMap[ fUnpackPar->GetDpbId( uDpb )  ] = uDpb;
      LOG(info) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = 0x"
                << std::setw(4) << std::hex << fUnpackPar->GetDpbId( uDpb )
                << std::dec
                << " => " << fDpbIdIndexMap[ fUnpackPar->GetDpbId( uDpb )  ];
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   fuNbFebs      = fUnpackPar->GetNrOfFebs();
   LOG(info) << "Nr. of FEBs:           " << fuNbFebs;

   fuNbHodoXyters = fUnpackPar->GetNrOfAsics();
   LOG(info) << "Nr. of HodoXyter ASICs: " << fuNbHodoXyters;

   fvbCrobActiveFlag.resize( fuNrOfDpbs );
   fviFebModuleIdx.resize(   fuNrOfDpbs );
   fviFebModuleSide.resize(  fuNrOfDpbs );
   fviFebType.resize(        fuNrOfDpbs );
   fvdFebAdcGain.resize(     fuNrOfDpbs );
   fvdFebAdcOffs.resize(     fuNrOfDpbs );
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fvbCrobActiveFlag[ uDpb ].resize( fUnpackPar->GetNbCrobsPerDpb() );
      fviFebModuleIdx[ uDpb ].resize(   fUnpackPar->GetNbCrobsPerDpb() );
      fviFebModuleSide[ uDpb ].resize(  fUnpackPar->GetNbCrobsPerDpb() );
      fviFebType[ uDpb ].resize(        fUnpackPar->GetNbCrobsPerDpb() );
      fvdFebAdcGain[ uDpb ].resize(        fUnpackPar->GetNbCrobsPerDpb() );
      fvdFebAdcOffs[ uDpb ].resize(        fUnpackPar->GetNbCrobsPerDpb() );
      for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx )
      {
         fvbCrobActiveFlag[ uDpb ][ uCrobIdx ] = fUnpackPar->IsCrobActive( uDpb, uCrobIdx );

         fviFebModuleIdx[ uDpb ][ uCrobIdx ].resize(   fUnpackPar->GetNbFebsPerCrob() );
         fviFebModuleSide[ uDpb ][ uCrobIdx ].resize(  fUnpackPar->GetNbFebsPerCrob() );
         fviFebType[ uDpb ][ uCrobIdx ].resize(        fUnpackPar->GetNbFebsPerCrob(), -1 );
         fvdFebAdcGain[ uDpb ][ uCrobIdx ].resize(     fUnpackPar->GetNbFebsPerCrob(), 0.0 );
         fvdFebAdcOffs[ uDpb ][ uCrobIdx ].resize(     fUnpackPar->GetNbFebsPerCrob(), 0.0 );
         for( UInt_t uFebIdx = 0; uFebIdx < fUnpackPar->GetNbFebsPerCrob(); ++ uFebIdx )
         {
            fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ]  = fUnpackPar->GetFebModuleIdx( uDpb, uCrobIdx, uFebIdx );
            fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ] = fUnpackPar->GetFebModuleSide( uDpb, uCrobIdx, uFebIdx );
            fvdFebAdcGain[ uDpb ][ uCrobIdx ][ uFebIdx ]    = fUnpackPar->GetFebAdcGain( uDpb, uCrobIdx, uFebIdx );
            fvdFebAdcOffs[ uDpb ][ uCrobIdx ][ uFebIdx ]    = fUnpackPar->GetFebAdcOffset( uDpb, uCrobIdx, uFebIdx );

            if( 0 <= fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ] &&
                fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ] < fuNbModules &&
                0 <= fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ] &&
                fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ] < 2 )
            {
               switch( fviModuleType[ fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ] ] )
               {
                  case 0: // FEB-8-1 with ZIF connector on the right
                  {
                     // P side (0) has type A (0)
                     // N side (1) has type B (1)
                     fviFebType[ uDpb ][ uCrobIdx ][ uFebIdx ] = fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ];

                     ///! FIXME: 1) Geometry is using front/back while we are using P/N !!!!
                     ///!            => Assuming that front facing mdules have connectors on right side
                     ///!        2) No accessor/setter to change only the side field of an STS address
                     ///!            => hardcode the shift
                     fviFebAddress.push_back( fviModAddress[ fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ] ]
                                              + ( fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ] << 25 ) );

                     break;
                  } // case 0: // FEB-8-1 with ZIF connector on the right
                  case 1: // FEB-8-1 with ZIF connector on the left
                  {
                     // P side (0) has type B (1)
                     // N side (1) has type A (0)
                     fviFebType[ uDpb ][ uCrobIdx ][ uFebIdx ] = !(fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ]);

                     ///! FIXME: 1) Geometry is using front/back while we are using P/N !!!!
                     ///!            => Assuming that front facing mdules have connectors on right side
                     ///!        2) No accessor/setter to change only the side field of an STS address
                     ///!            => hardcode the shift
                     fviFebAddress.push_back( fviModAddress[ fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ] ]
                                              + ( (!fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ]) << 25 ) );
                     break;
                  } // case 1: // FEB-8-1 with ZIF connector on the left
                  default:
                     LOG(fatal) << Form( "Bad module type for DPB #%02u CROB #%u FEB %02u: %d",
                                         uDpb, uCrobIdx, uFebIdx,
                                         fviModuleType[ fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ] ] );
                     break;
               } // switch( fviModuleType[ fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ] ] )
            } // FEB active and module index OK
               else if( -1 == fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ] ||
                        -1 == fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ] )
               {
                  fviFebAddress.push_back( 0 );
               } // Module index or type is set to inactive
               else
               {
                  LOG(fatal) << Form( "Bad module Index and/or Side for DPB #%02u CROB #%u FEB %02u: %d %d",
                                      uDpb, uCrobIdx, uFebIdx,
                                      fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ],
                                      fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ] );
               } // Bad module index or type for this FEB
         } // for( UInt_t uFebIdx = 0; uFebIdx < fUnpackPar->GetNbFebsPerCrob(); ++ uFebIdx )
      } // for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx )
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      TString sPrintoutLine = Form( "DPB #%02u CROB Active ?:       ", uDpb);
      for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx )
      {
         sPrintoutLine += Form( "%1u", ( fvbCrobActiveFlag[ uDpb ][ uCrobIdx ] == kTRUE ) );
      } // for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx )
      LOG(info) << sPrintoutLine;
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   UInt_t uGlobalFebIdx = 0;
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx )
      {
         LOG(info) << Form( "DPB #%02u CROB #%u:       ", uDpb, uCrobIdx);
         for( UInt_t uFebIdx = 0; uFebIdx < fUnpackPar->GetNbFebsPerCrob(); ++ uFebIdx )
         {
            if( 0 <= fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ] )
               LOG(info) << Form( "      FEB #%02u (%02u): Mod. Idx = %03d Side %c (%2d) Type %c (%2d) (Addr. 0x%08x) ADC gain %4.0f e- ADC Offs %5.0f e-",
                                    uFebIdx, uGlobalFebIdx,
                                    fviFebModuleIdx[ uDpb ][ uCrobIdx ][ uFebIdx ],
                                    1 == fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ] ? 'N': 'P',
                                    fviFebModuleSide[ uDpb ][ uCrobIdx ][ uFebIdx ],
                                    1 == fviFebType[ uDpb ][ uCrobIdx ][ uFebIdx ] ? 'B' : 'A',
                                    fviFebType[ uDpb ][ uCrobIdx ][ uFebIdx ],
                                    fviFebAddress[ uGlobalFebIdx ],
                                    fvdFebAdcGain[ uDpb ][ uCrobIdx ][ uFebIdx ],
                                    fvdFebAdcOffs[ uDpb ][ uCrobIdx ][ uFebIdx ]
                                 );
            uGlobalFebIdx ++;
         } // for( UInt_t uFebIdx = 0; uFebIdx < fUnpackPar->GetNbFebsPerCrob(); ++ uFebIdx )
      } // for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackPar->GetNbCrobsPerDpb(); ++uCrobIdx )
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   // Internal status initialization
   fvulCurrentTsMsb.resize( fuNrOfDpbs );
   fvuCurrentTsMsbCycle.resize( fuNrOfDpbs );
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fvulCurrentTsMsb[uDpb]     = 0;
      fvuCurrentTsMsbCycle[uDpb] = 0;
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
*/

  /// Initialize vectors
  /*
   for( UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx )
   {
      fhHodoFiberCnt[ uHodoIdx ].resize( kuNbAxis, nullptr );
      fhHodoFiberAdc[ uHodoIdx ].resize( kuNbAxis, nullptr );
      fhHodoFiberAdcProf[ uHodoIdx ].resize( kuNbAxis, nullptr );
      fhHodoFiberHitRateEvo[ uHodoIdx ].resize( kuNbAxis, nullptr );
      fvdChanCountsSinceLastRateUpdate[ uHodoIdx ].resize( kuNbChanPerAsic, 0.0 );
      fdHodoChanLastTimeForDist[ uHodoIdx ].resize( kuNbChanPerAsic, 0.0 );
   } // for( uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx )
*/
  return kTRUE;
}
// -------------------------------------------------------------------------

void CbmCosy2019MonitorAlgoHodo::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  LOG(info) << "CbmCosy2019MonitorAlgoHodo::AddMsComponentToList => Component " << component << " with detector ID 0x"
            << std::hex << usDetectorId << std::dec << " added to list";
}
// -------------------------------------------------------------------------

Bool_t CbmCosy2019MonitorAlgoHodo::ProcessTs(const fles::Timeslice& ts)
{
  fulCurrentTsIdx = ts.index();
  fdTsStartTime   = static_cast<Double_t>(ts.descriptor(0, 0).idx);

  /// Ignore First TS as first MS is typically corrupt
  if (0 == fulCurrentTsIdx) { return kTRUE; }  // if( 0 == fulCurrentTsIdx )

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsFullSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs + fuNbOverMsPerTs);
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a core duration of " << fdTsCoreSizeInNs << " ns and a full duration of "
              << fdTsFullSizeInNs << " ns";

    /// Ignore overlap ms if flag set by user
    fuNbMsLoop = fuNbCoreMsPerTs;
    if (kFALSE == fbIgnoreOverlapMs) fuNbMsLoop += fuNbOverMsPerTs;
    LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
  }  // if( -1.0 == fdTsCoreSizeInNs )

  /// Compute time of TS core end
  fdTsStopTimeCore = fdTsStartTime + fdTsCoreSizeInNs;
  //      LOG(info) << Form( "TS %5d Start %12f Stop %12f", fulCurrentTsIdx, fdTsStartTime, fdTsStopTimeCore );

  /// Loop over core microslices (and overlap ones if chosen)
  for (fuMsIndex = 0; fuMsIndex < fuNbMsLoop; fuMsIndex++) {
    /// Loop over registered components
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx) {
      UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];

      if (kFALSE == ProcessMs(ts, uMsComp, fuMsIndex)) {
        LOG(error) << "Failed to process ts " << fulCurrentTsIdx << " MS " << fuMsIndex << " for component " << uMsComp;
        return kFALSE;
      }  // if( kFALSE == ProcessMs( ts, uMsCompIdx, fuMsIndex ) )
    }    // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )

    /// Sort the buffers of hits
    std::sort(fvmHitsInMs.begin(), fvmHitsInMs.end());
    /*
      /// Add the hits to the output buffer as Digis
      for( auto itHitIn = fvmHitsInMs.begin(); itHitIn < fvmHitsInMs.end(); ++itHitIn )
      {
         UInt_t uFebIdx =  itHitIn->GetAsic() / fUnpackPar->GetNbAsicsPerFeb();
         UInt_t uChanInFeb = itHitIn->GetChan()
                            + fUnpackPar->GetNbChanPerAsic() * (itHitIn->GetAsic() % fUnpackPar->GetNbAsicsPerFeb());

         ULong64_t ulTimeInNs = static_cast< ULong64_t >( itHitIn->GetTs() * stsxyter::kdClockCycleNs - fdTimeOffsetNs );

         fDigiVect.push_back( CbmHodoDigi( fviFebAddress[ uFebIdx ], uChanInFeb, ulTimeInNs, itHitIn->GetAdc() ) );
      } // for( auto itHitIn = fvmHitsInMs.begin(); itHitIn < fvmHitsInMs.end(); ++itHitIn )
*/

    if (kFALSE == FillHistograms()) {
      LOG(error) << "Failed to fill histos in ts " << fulCurrentTsIdx << " MS " << fuMsIndex;
      return kFALSE;
    }  // if( kFALSE == FillHistograms() )

    /// Clear the buffer of hits
    fvmHitsInMs.clear();
  }  // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )

  /// Clear buffers to prepare for the next TS
  fvmHitsInMs.clear();


  return kTRUE;
}

Bool_t CbmCosy2019MonitorAlgoHodo::ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
{
  auto msDescriptor        = ts.descriptor(uMsCompIdx, uMsIdx);
  fuCurrentEquipmentId     = msDescriptor.eq_id;
  const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsCompIdx, uMsIdx));

  uint32_t uSize  = msDescriptor.size;
  fulCurrentMsIdx = msDescriptor.idx;

  Double_t dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
  LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
             << " has size: " << uSize;

  if (0 == fvbMaskedComponents.size()) fvbMaskedComponents.resize(ts.num_components(), kFALSE);

  fuCurrDpbId = static_cast<uint32_t>(fuCurrentEquipmentId & 0xFFFF);

  /// Check if this sDPB ID matches the one declared by the user
  if (fuDpbId != fuCurrDpbId) {
    if (kFALSE == fvbMaskedComponents[uMsCompIdx]) {
      LOG(info) << "---------------------------------------------------------------";
      /*
          LOG(info) << "hi hv eqid flag si sv idx/start        crc      size     offset";
          LOG(info) << Form( "%02x %02x %04x %04x %02x %02x %016llx %08x %08x %016llx",
                            static_cast<unsigned int>(msDescriptor.hdr_id),
                            static_cast<unsigned int>(msDescriptor.hdr_ver), msDescriptor.eq_id, msDescriptor.flags,
                            static_cast<unsigned int>(msDescriptor.sys_id),
                            static_cast<unsigned int>(msDescriptor.sys_ver), msDescriptor.idx, msDescriptor.crc,
                            msDescriptor.size, msDescriptor.offset );
*/
      LOG(info) << FormatMsHeaderPrintout(msDescriptor);
      LOG(warning) << "Could not find the sDPB index for AFCK id 0x" << std::hex << fuCurrDpbId << std::dec
                   << " in timeslice " << fulCurrentTsIdx << " in microslice " << uMsIdx << " component " << uMsCompIdx
                   << "\n"
                   << "If valid this index has to be added in the STS "
                      "parameter file in the DbpIdArray field";
      fvbMaskedComponents[uMsCompIdx] = kTRUE;

      /// If first TS being analyzed, we are probably detecting STS/MUCH boards with same sysid
      /// => Do not report the MS as bad, just ignore it
      if (1 == fulCurrentTsIdx) return kTRUE;
    }  // if( kFALSE == fvbMaskedComponents[ uMsComp ] )
    else
      return kTRUE;

    return kFALSE;
  }  // if( fuDpbId != fuCurrDpbId )

  /// Plots in [X/s] update
  if (static_cast<Int_t>(fvdPrevMsTime[uMsCompIdx]) < static_cast<Int_t>(dMsTime)) {
    /// "new second"
    for (UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx) {
      /// Ignore first interval is not clue how late the data taking was started
      if (0 == fviTimeSecLastRateUpdate[uHodoIdx]) {
        fviTimeSecLastRateUpdate[uHodoIdx] = static_cast<Int_t>(dMsTime);
        for (UInt_t uChan = 0; uChan < kuNbChanPerAsic; ++uChan) {
          fvdChanCountsSinceLastRateUpdate[uHodoIdx][uChan] = 0.0;
        }  // for( UInt_t uChan = 0; uChan < kuNbChanPerAsic; ++uChan )
        continue;
      }  // if( 0 == fviTimeSecLastRateUpdate[ uHodoIdx ] )

      Int_t iTimeInt = static_cast<Int_t>(dMsTime) - fviTimeSecLastRateUpdate[uHodoIdx];
      if (fiTimeIntervalRateUpdate <= iTimeInt) {
        for (UInt_t uChan = 0; uChan < kuNbChanPerAsic; ++uChan) {
          fhHodoChanHitRateProf[uHodoIdx]->Fill(uChan, fvdChanCountsSinceLastRateUpdate[uHodoIdx][uChan] / iTimeInt);
          fvdChanCountsSinceLastRateUpdate[uHodoIdx][uChan] = 0.0;
        }  // for( UInt_t uChan = 0; uChan < kuNbChanPerAsic; ++uChan )

        fviTimeSecLastRateUpdate[uHodoIdx] = static_cast<Int_t>(dMsTime);
      }  // if( fiTimeIntervalRateUpdate <= iTimeInt )
    }    // for( UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx )
  }      // if( static_cast<Int_t>( fvdPrevMsTime[ uMsCompIdx ] ) < static_cast<Int_t>( dMsTime )  )
  fvdPrevMsTime[uMsCompIdx] = dMsTime;

  /// Check Flags field of MS header
  uint16_t uMsHeaderFlags = msDescriptor.flags;
  for (UInt_t uBit = 0; uBit < 16; ++uBit)
    fhHodoMsStatusFieldType->Fill(uBit, (uMsHeaderFlags >> uBit) & 0x1);

  /** Check the current TS_MSb cycle and correct it if wrong **/
  UInt_t uTsMsbCycleHeader = std::floor(fulCurrentMsIdx / (stsxyter::kulTsCycleNbBins * stsxyter::kdClockCycleNs));

  if (0 == uMsIdx) {
    fvuCurrentTsMsbCycle = uTsMsbCycleHeader;
    fvulCurrentTsMsb     = 0;
  }  // if( 0 == uMsIdx )
  else if (uTsMsbCycleHeader != fvuCurrentTsMsbCycle && 4194303 != fvulCurrentTsMsb) {
    LOG(warning) << "TS MSB cycle from MS header does not match current cycle from data "
                 << "for TS " << std::setw(12) << fulCurrentTsIdx << " MS " << std::setw(12) << fulCurrentMsIdx
                 << " MsInTs " << std::setw(3) << uMsIdx << " ====> " << fvuCurrentTsMsbCycle << " VS "
                 << uTsMsbCycleHeader;
    fvuCurrentTsMsbCycle = uTsMsbCycleHeader;
  }

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % kuBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

  // Prepare variables for the loop on contents
  const uint32_t* pInBuff = reinterpret_cast<const uint32_t*>(msContent);
  for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
    // Fill message
    uint32_t ulData = static_cast<uint32_t>(pInBuff[uIdx]);

    stsxyter::Message mess(static_cast<uint32_t>(ulData & 0xFFFFFFFF));

    /// Get message type
    stsxyter::MessType typeMess = mess.GetMessType();
    fmMsgCounter[typeMess]++;
    fhHodoMessType->Fill(static_cast<uint16_t>(typeMess));

    switch (typeMess) {
      case stsxyter::MessType::Hit: {
        // Extract the eLink and Asic indices => Should GO IN the fill method now that obly hits are link/asic specific!
        UShort_t usElinkIdx = mess.GetLinkIndex();
        fhHodoMessTypePerElink->Fill(usElinkIdx, static_cast<uint16_t>(typeMess));

        /// Remap from eLink index to Hodoscope index
        UInt_t uHodoIdx  = 0;
        Bool_t bBadElink = kTRUE;
        for (uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx)
          if (fvuElinkIdxHodo[uHodoIdx] == usElinkIdx) {
            bBadElink = kFALSE;
            break;
          }  // if( fuElinkIdxHodo[ uHodo ] == usElinkIdx )

        if (bBadElink) {
          LOG(warning) << "CbmCosy2019MonitorAlgoHodo::DoUnpack => "
                       << "Wrong elink Idx! Elink raw " << Form("%2d", usElinkIdx);
          continue;
        }  // if( bBadElink )

        ProcessHitInfo(mess, uHodoIdx, uMsIdx);
        break;
      }  // case stsxyter::MessType::Hit :
      case stsxyter::MessType::TsMsb: {
        fhHodoMessTypePerElink->Fill(0., static_cast<uint16_t>(typeMess));

        ProcessTsMsbInfo(mess, uIdx, uMsIdx);
        break;
      }  // case stsxyter::MessType::TsMsb :
      case stsxyter::MessType::Epoch: {
        fhHodoMessTypePerElink->Fill(0., static_cast<uint16_t>(typeMess));

        // The first message in the TS is a special ones: EPOCH
        ProcessEpochInfo(mess);

        if (0 < uIdx)
          LOG(info) << "CbmCosy2019MonitorAlgoHodo::DoUnpack => "
                    << "EPOCH message at unexpected position in MS: message " << uIdx << " VS message 0 expected!";
        break;
      }  // case stsxyter::MessType::TsMsb :
      case stsxyter::MessType::Status: {
        UShort_t usElinkIdx = mess.GetStatusLink();
        fhHodoMessTypePerElink->Fill(usElinkIdx, static_cast<uint16_t>(typeMess));
        ProcessStatusInfo(mess);
        break;
      }  // case stsxyter::MessType::Status
      case stsxyter::MessType::Empty: {
        fhHodoMessTypePerElink->Fill(0., static_cast<uint16_t>(typeMess));
        //                   FillTsMsbInfo( mess );
        break;
      }  // case stsxyter::MessType::Empty :
      case stsxyter::MessType::Dummy: {
        fhHodoMessTypePerElink->Fill(0., static_cast<uint16_t>(typeMess));
        break;
      }  // case stsxyter::MessType::Dummy / ReadDataAck / Ack :
      default: {
        LOG(fatal) << "CbmCosy2019MonitorAlgoHodo::DoUnpack => "
                   << "Unknown message type, should never happen, stopping "
                      "here! Type found was: "
                   << static_cast<int>(typeMess);
      }
    }  // switch( typeMess )
  }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

  return kTRUE;
}

// -------------------------------------------------------------------------
void CbmCosy2019MonitorAlgoHodo::ProcessHitInfo(stsxyter::Message mess, const UInt_t& uHodoIdx,
                                                const UInt_t& /*uMsIdx*/)
{
  UShort_t usChan   = mess.GetHitChannel();
  UShort_t usRawAdc = mess.GetHitAdc();
  //   UShort_t usTsOver = mess.GetHitTimeOver();
  UShort_t usRawTs = mess.GetHitTime();

  fhHodoChanCntRaw[uHodoIdx]->Fill(usChan);
  fhHodoChanAdcRaw[uHodoIdx]->Fill(usChan, usRawAdc);
  fhHodoChanAdcRawProf[uHodoIdx]->Fill(usChan, usRawAdc);
  fhHodoChanRawTs[uHodoIdx]->Fill(usChan, usRawTs);
  fhHodoChanMissEvt[uHodoIdx]->Fill(usChan, mess.IsHitMissedEvts());

  /*
   Double_t dCalAdc = fvdFebAdcOffs[ fuCurrDpbIdx ][ uCrobIdx ][ uFebIdx ]
                     + (usRawAdc - 1)* fvdFebAdcGain[ fuCurrDpbIdx ][ uCrobIdx ][ uFebIdx ];
   fhHodoChanAdcCal[  uHodoIdx ]->Fill( usChan, dCalAdc );
   fhHodoChanAdcCalProf[  uHodoIdx ]->Fill( usChan, dCalAdc );
*/

  /// Mapping to Fiber
  UInt_t uAxis  = kuChannelToPlaneMap[usChan];
  UInt_t uFiber = kuChannelToFiberMap[usChan];

  /// Remapped = Hodos fibers
  fhHodoFiberCnt[uHodoIdx][uAxis]->Fill(uFiber);
  fhHodoFiberAdc[uHodoIdx][uAxis]->Fill(uFiber, usRawAdc);
  fhHodoFiberAdcProf[uHodoIdx][uAxis]->Fill(uFiber, usRawAdc);

  /// Compute the Full time stamp
  /// Use TS w/o overlap bits as they will anyway come from the TS_MSB
  Long64_t ulHitTime = usRawTs;
  ulHitTime += static_cast<ULong64_t>(stsxyter::kuHitNbTsBins) * static_cast<ULong64_t>(fvulCurrentTsMsb)
               + static_cast<ULong64_t>(stsxyter::kulTsCycleNbBins) * static_cast<ULong64_t>(fvuCurrentTsMsbCycle);

  /// Convert the Hit time in bins to Hit time in ns
  Double_t dHitTimeNs = ulHitTime * stsxyter::kdClockCycleNs;

  /// Data should already be time sorted in FW
  fhHodoChanDistT[uHodoIdx]->Fill(dHitTimeNs - fdHodoChanLastTimeForDist[uHodoIdx][usChan], usChan);
  fdHodoChanLastTimeForDist[uHodoIdx][usChan] = dHitTimeNs;

  fvmHitsInMs.push_back(stsxyter::FinalHit(ulHitTime, usRawAdc, uHodoIdx, uFiber, uAxis, 0));

  /// Check Starting point of histos with time as X axis
  if (-1 == fdStartTime) fdStartTime = dHitTimeNs;

  // Fill histos with time as X axis
  Double_t dTimeSinceStartSec = (dHitTimeNs - fdStartTime) * 1e-9;
  //   Double_t dTimeSinceStartMin = dTimeSinceStartSec / 60.0;

  fvdChanCountsSinceLastRateUpdate[uHodoIdx][usChan] += 1;

  fhHodoChanHitRateEvo[uHodoIdx]->Fill(dTimeSinceStartSec, usChan);
  fhHodoFiberHitRateEvo[uHodoIdx][uAxis]->Fill(dTimeSinceStartSec, uFiber);
  if (mess.IsHitMissedEvts()) {
    fhHodoChanMissEvtEvo[uHodoIdx]->Fill(dTimeSinceStartSec, usChan);
  }  // if( mess.IsHitMissedEvts() )
}

void CbmCosy2019MonitorAlgoHodo::ProcessTsMsbInfo(stsxyter::Message mess, UInt_t uMessIdx, UInt_t uMsIdx)
{
  UInt_t uVal = mess.GetTsMsbVal();

  // Update Status counters
  if (uVal < fvulCurrentTsMsb) {

    LOG(info) << " TS " << std::setw(12) << fulCurrentTsIdx << " MS " << std::setw(12) << fulCurrentMsIdx << " MS Idx "
              << std::setw(4) << uMsIdx << " Msg Idx " << std::setw(5) << uMessIdx << " Old TsMsb " << std::setw(5)
              << fvulCurrentTsMsb << " Old MsbCy " << std::setw(5) << fvuCurrentTsMsbCycle << " new TsMsb "
              << std::setw(5) << uVal;

    fvuCurrentTsMsbCycle++;
  }  // if( uVal < fvulCurrentTsMsb )
  if (uVal != fvulCurrentTsMsb + 1 && 0 != uVal && 4194303 != fvulCurrentTsMsb
      && 1 != uMessIdx)  // 1st TS MSB in MS always repat of last in prev MS
  {
    LOG(info) << "TS MSb Jump in "
              << " TS " << std::setw(12) << fulCurrentTsIdx << " MS " << std::setw(12) << fulCurrentMsIdx << " MS Idx "
              << std::setw(4) << uMsIdx << " Msg Idx " << std::setw(5) << uMessIdx << " => Old TsMsb " << std::setw(5)
              << fvulCurrentTsMsb << " new TsMsb " << std::setw(5) << uVal;
  }  // if( uVal + 1 != fvulCurrentTsMsb && 4194303 != uVal && 0 != fvulCurrentTsMsb && 1 != uMessIdx )
  fvulCurrentTsMsb = uVal;
  /*
   ULong64_t ulNewTsMsbTime =  static_cast< ULong64_t >( stsxyter::kuHitNbTsBins )
                             * static_cast< ULong64_t >( fvulCurrentTsMsb )
                             + static_cast< ULong64_t >( stsxyter::kulTsCycleNbBins )
                             * static_cast< ULong64_t >( fvuCurrentTsMsbCycle );
*/
}

void CbmCosy2019MonitorAlgoHodo::ProcessEpochInfo(stsxyter::Message /*mess*/)
{
  //   UInt_t uVal    = mess.GetEpochVal();
  //   UInt_t uCurrentCycle = uVal % stsxyter::kulTsCycleNbBins;

  /*
   // Update Status counters
   if( usVal < fvulCurrentTsMsb )
      fvuCurrentTsMsbCycle ++;
   fvulCurrentTsMsb = usVal;
*/
}

void CbmCosy2019MonitorAlgoHodo::ProcessStatusInfo(stsxyter::Message /*mess*/)
{
  /*
   UInt_t   uCrobIdx   = usElinkIdx / XXXX
   Int_t   uFebIdx    = XXXX
   UInt_t   uAsicIdx   = XXX

   UShort_t usStatusField = mess.GetStatusStatus();

   fhPulserStatusMessType->Fill( uAsicIdx, usStatusField );
   /// Always print status messages... or not?
   if( fbPrintMessages )
   {
      std::cout << Form("TS %12u MS %12u mess %5u ", fulCurrentTsIdx, fulCurrentMsIdx, uIdx );
      mess.PrintMess( std::cout, fPrintMessCtrl );
   } // if( fbPrintMessages )
*/
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

Bool_t CbmCosy2019MonitorAlgoHodo::CreateHistograms()
{
  fhHodoMessType = new TH1I("hHodoMessType", "Nb of message for each type; Type", 6, 0., 6.);
  fhHodoMessType->GetXaxis()->SetBinLabel(1, "Dummy");
  fhHodoMessType->GetXaxis()->SetBinLabel(2, "Hit");
  fhHodoMessType->GetXaxis()->SetBinLabel(3, "TsMsb");
  fhHodoMessType->GetXaxis()->SetBinLabel(4, "Epoch");
  fhHodoMessType->GetXaxis()->SetBinLabel(5, "Status");
  fhHodoMessType->GetXaxis()->SetBinLabel(6, "Empty");

  fhHodoStatusMessType =
    new TH2I("hHodoStatusMessType", "Nb of status message of each type for each DPB; ASIC; Status Type", kuNbHodos, 0,
             kuNbHodos, 16, 0., 16.);
  /*
   fhHodoStatusMessType->GetYaxis()->SetBinLabel( 1, "Dummy");
   fhHodoStatusMessType->GetYaxis()->SetBinLabel( 2, "Hit");
   fhHodoStatusMessType->GetYaxis()->SetBinLabel( 3, "TsMsb");
   fhHodoStatusMessType->GetYaxis()->SetBinLabel( 4, "Epoch");
*/

  fhHodoMsStatusFieldType =
    new TH2I("hHodoMsStatusFieldType", "For each flag in the MS header, ON/OFF counts; Flag bit []; ON/OFF; MS []", 16,
             -0.5, 15.5, 2, -0.5, 1.5);
  /*
   fhHodoStatusMessType->GetYaxis()->SetBinLabel( 1, "Dummy");
   fhHodoStatusMessType->GetYaxis()->SetBinLabel( 2, "Hit");
   fhHodoStatusMessType->GetYaxis()->SetBinLabel( 3, "TsMsb");
   fhHodoStatusMessType->GetYaxis()->SetBinLabel( 4, "Epoch");
*/

  fhHodoMessTypePerElink = new TH2I("hHodoMessTypePerElink", "Nb of message of each type for each eLink; eLink; Type",
                                    kuNbElinksDpb, 0, kuNbElinksDpb, 6, 0., 6.);
  fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1, "Dummy");
  fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(2, "Hit");
  fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(3, "TsMsb");
  fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(4, "Epoch");
  fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(5, "Status");
  fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(6, "Empty");

  AddHistoToVector(fhHodoMessType, "MessTypes");
  AddHistoToVector(fhHodoStatusMessType, "MessTypes");
  AddHistoToVector(fhHodoMsStatusFieldType, "MessTypes");
  AddHistoToVector(fhHodoMessTypePerElink, "MessTypes");

  for (UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx) {
    /// Raw = ASIC channels
    /// Channel counts
    fhHodoChanCntRaw[uHodoIdx] = new TH1I(Form("hHodoChanCntRaw_%u", uHodoIdx),
                                          Form("Hits Count per channel, Hodo #%u; Channel; Hits []", uHodoIdx),
                                          kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5);

    /// Raw Adc Distribution
    fhHodoChanAdcRaw[uHodoIdx] = new TH2I(Form("hHodoChanAdcRaw_%u", uHodoIdx),
                                          Form("Raw Adc distribution per channel, Hodo #%u; Channel []; "
                                               "Adc []; Hits []",
                                               uHodoIdx),
                                          kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5, stsxyter::kuHitNbAdcBins, -0.5,
                                          stsxyter::kuHitNbAdcBins - 0.5);

    /// Raw Adc Distribution profile
    fhHodoChanAdcRawProf[uHodoIdx] =
      new TProfile(Form("hHodoChanAdcRawProf_%u", uHodoIdx),
                   Form("Raw Adc prodile per channel, Hodo #%u; Channel []; Adc []", uHodoIdx), kuNbChanPerAsic, -0.5,
                   kuNbChanPerAsic - 0.5);
    /*
         /// Cal Adc Distribution
      fhHodoChanAdcCal[ uHodoIdx ] =  new TH2I( Form( "hHodoChanAdcCal_%u", uHodoIdx ),
                                 Form( "Cal. Adc distribution per channel, Hodo #%u; Channel []; Adc [e-]; Hits []", uHodoIdx ),
                                 kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5,
                                  50, 0., 100000. );

         /// Cal Adc Distribution profile
      fhHodoChanAdcCalProf[ uHodoIdx ] =  new TProfile( Form( "hHodoChanAdcCalProf_%u", uHodoIdx ),
                                 Form( "Cal. Adc prodile per channel, Hodo #%u; Channel []; Adc [e-]", uHodoIdx ),
                                 kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5 );
*/
    /// Raw Ts Distribution
    fhHodoChanRawTs[uHodoIdx] = new TH2I(Form("hHodoChanRawTs_%u", uHodoIdx),
                                         Form("Raw Timestamp distribution per channel, FEB #%03u; "
                                              "Channel []; Ts []; Hits []",
                                              uHodoIdx),
                                         kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5, stsxyter::kuHitNbTsBins, -0.5,
                                         stsxyter::kuHitNbTsBins - 0.5);

    /// Missed event flag
    fhHodoChanMissEvt[uHodoIdx] = new TH2I(Form("hHodoChanMissEvt_%u", uHodoIdx),
                                           Form("Missed Event flags per channel, Hodo #%u; Channel []; "
                                                "Miss Evt []; Hits []",
                                                uHodoIdx),
                                           kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5, 2, -0.5, 1.5);

    /// Missed event flag counts evolution
    fhHodoChanMissEvtEvo[uHodoIdx] = new TH2I(Form("hHodoChanMissEvtEvo_%u", uHodoIdx),
                                              Form("Missed Evt flags per second & channel in Hodo #%u; Time "
                                                   "[s]; Channel []; Missed Evt flags []",
                                                   uHodoIdx),
                                              1800, 0, 1800, kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5);

    /// Hit rates evo per channel
    fhHodoChanHitRateEvo[uHodoIdx] =
      new TH2I(Form("hHodoChanHitRateEvo_%u", uHodoIdx),
               Form("Hits per second & channel in Hodo #%u; Time [s]; Channel []; Hits []", uHodoIdx), 1800, 0, 1800,
               kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5);

    /// Hit rates profile per channel
    fhHodoChanHitRateProf[uHodoIdx] =
      new TProfile(Form("hHodoChanHitRateProf_%u", uHodoIdx),
                   Form("Hits per second for each channel in Hodo #%u; Channel []; Hits/s []", uHodoIdx),
                   kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5);

    /// Distance between hits on same channel
    fhHodoChanDistT[uHodoIdx] = new TH2I(Form("hHodoChanDistT_%u", uHodoIdx),
                                         Form("Time distance between hits on same channel in Hodo #%u; "
                                              "Time difference [ns]; Channel []; ",
                                              uHodoIdx),
                                         1000, -0.5, 6250.0 - 0.5, kuNbChanPerAsic, -0.5, kuNbChanPerAsic - 0.5);

    /// Remapped = Hodos fibers
    for (UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis) {
      Char_t cAxisName = (uAxis ? 'Y' : 'X');
      /// Fibers counts
      fhHodoFiberCnt[uHodoIdx][uAxis] =
        new TH1I(Form("hHodoFiberCnt%c_%u", cAxisName, uHodoIdx),
                 Form("Hits Count per Fiber, Hodo #%u Axis %c; Fiber; Hits []", uHodoIdx, cAxisName),
                 kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5);

      /// Fibers Adc Distribution
      fhHodoFiberAdc[uHodoIdx][uAxis] = new TH2I(Form("fhHodoFiberAdc%c_%u", cAxisName, uHodoIdx),
                                                 Form("Raw Adc distribution per Fiber, Hodo #%u Axis %c; "
                                                      "Channel []; Adc []; Hits []",
                                                      uHodoIdx, cAxisName),
                                                 kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5,
                                                 stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins - 0.5);

      /// Fibers Adc Distribution profile
      fhHodoFiberAdcProf[uHodoIdx][uAxis] =
        new TProfile(Form("hHodoFiberAdcProf%c_%u", cAxisName, uHodoIdx),
                     Form("Raw Adc prodile per Fiber, Hodo #%u Axis %c; Channel []; Adc []", uHodoIdx, cAxisName),
                     kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5);

      /// Hit rates evo per fiber
      fhHodoFiberHitRateEvo[uHodoIdx][uAxis] =
        new TH2I(Form("hHodoFiberHitRateEvo%c_%u", cAxisName, uHodoIdx),
                 Form("Hits per second & Fiber in Hodo #%u Axis %c; Time [s]; "
                      "Channel []; Hits []",
                      uHodoIdx, cAxisName),
                 1800, 0, 1800, kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5);

      AddHistoToVector(fhHodoFiberCnt[uHodoIdx][uAxis], "Fibers");
      AddHistoToVector(fhHodoFiberAdc[uHodoIdx][uAxis], "Fibers");
      AddHistoToVector(fhHodoFiberAdcProf[uHodoIdx][uAxis], "Fibers");
      AddHistoToVector(fhHodoFiberHitRateEvo[uHodoIdx][uAxis], "Fibers");
    }  // for( UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis )

    /// Clusters
    /// =====> Limits for clustering to be defined, maybe use instead STS SsdOrtho reconstruction class

    /// Coincidences in same Hodo between axis
    /// Map
    fhHodoFiberCoincMapXY[uHodoIdx] = new TH2I(Form("hHodoFiberCoincMapXY_%u", uHodoIdx),
                                               Form("Map of coincident (X, Y) pairs in Hodo #%u; X [Fiber]; Y "
                                                    "[Fiber]; Hits []",
                                                    uHodoIdx),
                                               kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5,
                                               kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5);
    /// Time diff
    fhHodoFiberCoincTimeXY[uHodoIdx] = new TH1I(Form("hHodoFiberCoincTimeXY_%u", uHodoIdx),
                                                Form("Time difference of coincident (X, Y) pairs in Hodo #%u; "
                                                     "t_Y - t_X [ns]; Hits []",
                                                     uHodoIdx),
                                                2 * fdTimeCoincLimit + 1, -fdTimeCoincLimitNs, fdTimeCoincLimitNs);
    /// Walk
    fhHodoFiberCoincWalkXY_X[uHodoIdx] = new TH2I(Form("hHodoFiberCoincWalkXY_X_%u", uHodoIdx),
                                                  Form("Walk X of coincident (X, Y) pairs in Hodo #%u; ADC X "
                                                       "[bin]; t_Y - t_X [ns]; Hits []",
                                                       uHodoIdx),
                                                  stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins - 0.5,
                                                  2 * fdTimeCoincLimit + 1, -fdTimeCoincLimitNs, fdTimeCoincLimitNs);
    fhHodoFiberCoincWalkXY_Y[uHodoIdx] = new TH2I(Form("hHodoFiberCoincWalkXY_Y_%u", uHodoIdx),
                                                  Form("Walk X of coincident (X, Y) pairs in Hodo #%u; ADC X "
                                                       "[bin]; t_Y - t_X [ns]; Hits []",
                                                       uHodoIdx),
                                                  stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins - 0.5,
                                                  2 * fdTimeCoincLimit + 1, -fdTimeCoincLimitNs, fdTimeCoincLimitNs);

    AddHistoToVector(fhHodoChanCntRaw[uHodoIdx], "Raw");
    AddHistoToVector(fhHodoChanAdcRaw[uHodoIdx], "Raw");
    AddHistoToVector(fhHodoChanAdcRawProf[uHodoIdx], "Raw");
    //         AddHistoToVector( fhHodoChanAdcCal[      uHodoIdx ], "Raw" );
    //         AddHistoToVector( fhHodoChanAdcCalProf[  uHodoIdx ], "Raw" );
    AddHistoToVector(fhHodoChanRawTs[uHodoIdx], "Raw");
    AddHistoToVector(fhHodoChanMissEvt[uHodoIdx], "Raw");
    AddHistoToVector(fhHodoChanMissEvtEvo[uHodoIdx], "Raw");
    AddHistoToVector(fhHodoChanHitRateEvo[uHodoIdx], "Raw");
    AddHistoToVector(fhHodoChanHitRateProf[uHodoIdx], "Raw");
    AddHistoToVector(fhHodoChanDistT[uHodoIdx], "Raw");
    AddHistoToVector(fhHodoFiberCoincMapXY[uHodoIdx], "Coinc");
    AddHistoToVector(fhHodoFiberCoincTimeXY[uHodoIdx], "Coinc");
    AddHistoToVector(fhHodoFiberCoincWalkXY_X[uHodoIdx], "Coinc");
    AddHistoToVector(fhHodoFiberCoincWalkXY_Y[uHodoIdx], "Coinc");
  }  // for( UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx )

  for (UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis) {
    Char_t cAxisName      = (uAxis ? 'Y' : 'X');
    Char_t cOtherAxisName = (uAxis ? 'X' : 'Y');
    /// Coincidences in different Hodos between axis <= Valid only for kuNbHodos = 2 !!!
    /// Map Same axis
    fhHodoFiberCoincMapSameAB[uAxis] = new TH2I(Form("hHodoFiberCoincMapSameAB_%c%c", cAxisName, cAxisName),
                                                Form("Map of coincident (%c, %c) pairs in Hodo A and B; %c_A "
                                                     "[Fiber]; %c_B [Fiber]; Hits []",
                                                     cAxisName, cAxisName, cAxisName, cAxisName),
                                                kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5,
                                                kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5);
    /// Time diff Same axis
    fhHodoFiberCoincTimeSameAB[uAxis] = new TH1I(Form("hHodoFiberCoincTimeSameAB_%c%c", cAxisName, cAxisName),
                                                 Form("Time difference of coincident (%c, %c) pairs in Hodo A "
                                                      "and B; t_%c_B - t_%c_A [ns]; Hits []",
                                                      cAxisName, cAxisName, cAxisName, cAxisName),
                                                 2 * fdTimeCoincLimit + 1, -fdTimeCoincLimitNs, fdTimeCoincLimitNs);

    /// Map different axis
    fhHodoFiberCoincMapDiffAB[uAxis] = new TH2I(Form("hHodoFiberCoincMapDiffAB_%c%c", cAxisName, cOtherAxisName),
                                                Form("Map of coincident (%c, %c) pairs in Hodo A and B; %c_A "
                                                     "[Fiber]; %c_B [Fiber]; Hits []",
                                                     cAxisName, cOtherAxisName, cOtherAxisName, cAxisName),
                                                kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5,
                                                kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5);
    /// Time diff different axis
    fhHodoFiberCoincTimeDiffAB[uAxis] = new TH1I(Form("hHodoFiberCoincTimeDiffAB_%c%c", cAxisName, cOtherAxisName),
                                                 Form("Time difference of coincident (%c, %c) pairs in Hodo A and B; "
                                                      "t_%c_B - t_%c_A [ns]; Hits []",
                                                      cAxisName, cOtherAxisName, cOtherAxisName, cAxisName),
                                                 2 * fdTimeCoincLimit + 1, -fdTimeCoincLimitNs, fdTimeCoincLimitNs);

    AddHistoToVector(fhHodoFiberCoincMapSameAB[uAxis], "Coinc");
    AddHistoToVector(fhHodoFiberCoincTimeSameAB[uAxis], "Coinc");
    AddHistoToVector(fhHodoFiberCoincMapDiffAB[uAxis], "Coinc");
    AddHistoToVector(fhHodoFiberCoincTimeDiffAB[uAxis], "Coinc");
  }  // for( UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis )

  /// Full Coincidences between Hodos<= Valid only for kuNbHodos = 2 && kuNbAxis = 2
  /// Position on hodo A
  fhHodoFullCoincPosA = new TH2I("fhHodoFullCoincPosA",
                                 "Position on Hodo A for coincident pairs in Hodo A and B;  X_A "
                                 "[Fiber]; Y_A [Fiber]; Hits []",
                                 kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5, kuNbChanPerAsic / 2, -0.5,
                                 kuNbChanPerAsic / 2 - 0.5);
  /// Position on hodo B
  fhHodoFullCoincPosB = new TH2I("fhHodoFullCoincPosB",
                                 "Position on Hodo B for coincident pairs in Hodo A and B;  X_B "
                                 "[Fiber]; Y_B [Fiber]; Hits []",
                                 kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5, kuNbChanPerAsic / 2, -0.5,
                                 kuNbChanPerAsic / 2 - 0.5);
  /// Comp X axis
  fhHodoFullCoincCompX = new TH2I("hHodoFullCoincCompX",
                                  "Comparison of X pos for coincident pairs in Hodo A and B;  X_A "
                                  "[Fiber]; X_B [Fiber]; Hits []",
                                  kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5, kuNbChanPerAsic / 2, -0.5,
                                  kuNbChanPerAsic / 2 - 0.5);
  /// Comp Y axis
  fhHodoFullCoincCompY = new TH2I("hHodoFullCoincCompY",
                                  "Comparison of Y pos for coincident pairs in Hodo A and B;  Y_A "
                                  "[Fiber]; Y_B [Fiber]; Hits []",
                                  kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5, kuNbChanPerAsic / 2, -0.5,
                                  kuNbChanPerAsic / 2 - 0.5);

  /// Residuals X vs Y ("alignment")
  fhHodoFullCoincResidualXY =
    new TH2I("hHodoFullCoincResidualXY",
             "X and Y residuals for coincident pairs in Hodo A and B; X_B - "
             "X_A [Fiber]; Y_B - Y_A [Fiber]; Hits []",
             kuNbChanPerAsic + 1, -1.0 * kuNbChanPerAsic / 2 - 0.5, kuNbChanPerAsic / 2 + 0.5, kuNbChanPerAsic + 1,
             -1.0 * kuNbChanPerAsic / 2 - 0.5, kuNbChanPerAsic / 2 + 0.5);
  /// Time diff different axis
  fhHodoFullCoincTimeDiff = new TH1I("hHodoFullCoincTimeDiff",
                                     "Time difference of coincident pairs in Hodo A and B; (t_X_B + "
                                     "t_Y_B)/2 - (t_X_A + t_Y_A)/2 [ns]; Hits []",
                                     2 * fdTimeCoincLimit + 1, -fdTimeCoincLimitNs, fdTimeCoincLimitNs);

  fhHodoFullCoincRateEvo = new TH1I("fhHodoFullCoincRateEvo",
                                    "Evolution of the full coincidence rate; "
                                    "Time in run [s]; Full coincidences;",
                                    1800, -0.5, 1800 - 0.5);

  AddHistoToVector(fhHodoFullCoincPosA, "FullCoinc");
  AddHistoToVector(fhHodoFullCoincPosB, "FullCoinc");
  AddHistoToVector(fhHodoFullCoincCompX, "FullCoinc");
  AddHistoToVector(fhHodoFullCoincCompY, "FullCoinc");
  AddHistoToVector(fhHodoFullCoincResidualXY, "FullCoinc");
  AddHistoToVector(fhHodoFullCoincTimeDiff, "FullCoinc");
  AddHistoToVector(fhHodoFullCoincRateEvo, "FullCoinc");

  for (UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx) {
    for (UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis) {
      Char_t cAxisName = (uAxis ? 'Y' : 'X');
      fhHodoFullCoincTimeWalk[uHodoIdx][uAxis] =
        new TH2I(Form("hHodoFullCoincTimeWalk_%u%c", uHodoIdx, cAxisName),
                 Form("Time walk of coincident (A, B) pairs in Hodo #%u Axis "
                      "%c; ADC %u_%c [bin]; Time Diff <B> - <A> [ns]; Hits []",
                      uHodoIdx, cAxisName, uHodoIdx, cAxisName),
                 stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins - 0.5, 2 * fdTimeCoincLimit + 1,
                 -fdTimeCoincLimitNs, fdTimeCoincLimitNs);
      AddHistoToVector(fhHodoFullCoincTimeWalk[uHodoIdx][uAxis], "Walk");

      fhHodoFullCoincPosEvo[uHodoIdx][uAxis] =
        new TH2I(Form("hHodoFullCoincPosEvo_%u%c", uHodoIdx, cAxisName),
                 Form("Time evolution of coincident (A, B) pairs position in Hodo #%u "
                      "Axis %c; Time in run [s]; Position %u_%c [Fiber]; Hits []",
                      uHodoIdx, cAxisName, uHodoIdx, cAxisName),
                 2000, -0.5, 1000 - 0.5, kuNbChanPerAsic / 2, -0.5, kuNbChanPerAsic / 2 - 0.5);
      AddHistoToVector(fhHodoFullCoincPosEvo[uHodoIdx][uAxis], "FullCoinc");
    }  // for( UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis )
  }    // for( UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx )

  /// Setup debugging
  fhPrevHitDtAllAsics = new TH1I("hPrevHitDtAllAsics",
                                 "Time difference between current and previous hits in any ASIC; t "
                                 "- t_prev [ns]; Hit pairs []",
                                 10000, 0.0, 10000 * stsxyter::kdClockCycleNs);
  fhPrevHitDtAsicA    = new TH1I("hPrevHitDtAsicA",
                              "Time difference between current and previous "
                              "hits in ASIC A; t - t_prev [ns]; Hit pairs []",
                              10000, 0.0, 10000 * stsxyter::kdClockCycleNs);
  fhPrevHitDtAsicB    = new TH1I("hPrevHitDtAsicB",
                              "Time difference between current and previous "
                              "hits in ASIC B; t - t_prev [ns]; Hit pairs []",
                              10000, 0.0, 10000 * stsxyter::kdClockCycleNs);
  fhPrevHitDtAsicsAB  = new TH1I("hPrevHitDtAsicsAB",
                                "Time difference between current in ASIC A and previous hit in "
                                "ASIC B; t - t_prev [ns]; Hit pairs []",
                                10000, 0.0, 10000 * stsxyter::kdClockCycleNs);

  AddHistoToVector(fhPrevHitDtAllAsics, "SetupDebugging");
  AddHistoToVector(fhPrevHitDtAsicA, "SetupDebugging");
  AddHistoToVector(fhPrevHitDtAsicB, "SetupDebugging");
  AddHistoToVector(fhPrevHitDtAsicsAB, "SetupDebugging");

  /// Canvases
  Double_t w = 860;
  Double_t h = 480;

  fcSummary = new TCanvas("fcSummary", "Summary for the Hodo sDPB", w, h);
  fcSummary->Divide(2, 2);

  fcSummary->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhHodoMessType->Draw("hist");

  fcSummary->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoStatusMessType->Draw("colz");

  fcSummary->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoMsStatusFieldType->Draw("colz");

  fcSummary->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoMessTypePerElink->Draw("colz");

  AddCanvasToVector(fcSummary, "canvases");


  for (UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx) {
    /// Distributions before fiber mapping per Hodo
    fcHodoSummaryRaw[uHodoIdx] =
      new TCanvas(Form("cHodoSummaryRaw%u", uHodoIdx), Form("Raw Summary for Hodo %u", uHodoIdx), w, h);
    fcHodoSummaryRaw[uHodoIdx]->Divide(4, 2);

    fcHodoSummaryRaw[uHodoIdx]->cd(1);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fhHodoChanCntRaw[uHodoIdx]->Draw("hist");

    fcHodoSummaryRaw[uHodoIdx]->cd(2);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fhHodoChanHitRateProf[uHodoIdx]->Draw("hist");

    fcHodoSummaryRaw[uHodoIdx]->cd(3);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhHodoChanAdcRaw[uHodoIdx]->Draw("colz");

    fcHodoSummaryRaw[uHodoIdx]->cd(4);
    gPad->SetGridx();
    gPad->SetGridy();
    fhHodoChanAdcRawProf[uHodoIdx]->Draw("hist");

    fcHodoSummaryRaw[uHodoIdx]->cd(5);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhHodoChanDistT[uHodoIdx]->Draw("colz");

    fcHodoSummaryRaw[uHodoIdx]->cd(6);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhHodoChanHitRateEvo[uHodoIdx]->Draw("colz");

    fcHodoSummaryRaw[uHodoIdx]->cd(7);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhHodoChanMissEvt[uHodoIdx]->Draw("colz");

    fcHodoSummaryRaw[uHodoIdx]->cd(8);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhHodoChanMissEvtEvo[uHodoIdx]->Draw("colz");

    /// Distributions after fiber mapping per Hodo and axis
    fcHodoSummaryFiber[uHodoIdx] =
      new TCanvas(Form("cHodoSummaryFiber%u", uHodoIdx), Form("Fiber Summary for Hodo %u", uHodoIdx), w, h);
    fcHodoSummaryFiber[uHodoIdx]->Divide(4, 2);

    for (UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis) {
      fcHodoSummaryFiber[uHodoIdx]->cd(1 + 4 * uAxis);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fhHodoFiberCnt[uHodoIdx][uAxis]->Draw("hist");

      fcHodoSummaryFiber[uHodoIdx]->cd(2 + 4 * uAxis);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhHodoFiberAdc[uHodoIdx][uAxis]->Draw("colz");

      fcHodoSummaryFiber[uHodoIdx]->cd(3 + 4 * uAxis);
      gPad->SetGridx();
      gPad->SetGridy();
      fhHodoFiberAdcProf[uHodoIdx][uAxis]->Draw("hist");

      fcHodoSummaryFiber[uHodoIdx]->cd(4 + 4 * uAxis);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhHodoFiberHitRateEvo[uHodoIdx][uAxis]->Draw("colz");
    }  // for( UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis )

    AddCanvasToVector(fcHodoSummaryRaw[uHodoIdx], "canvases");
    AddCanvasToVector(fcHodoSummaryFiber[uHodoIdx], "canvases");
  }  // for( UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx )

  fcHodoFiberCoinc = new TCanvas("fcHodoFiberCoinc", "X/Y coincidences in same hodoscope", w, h);
  fcHodoFiberCoinc->Divide(4, 2);

  fcHodoFiberCoinc->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFiberCoincMapXY[0]->Draw("colz");

  fcHodoFiberCoinc->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhHodoFiberCoincTimeXY[0]->Draw("hist");

  fcHodoFiberCoinc->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFiberCoincWalkXY_X[0]->Draw("colz");

  fcHodoFiberCoinc->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFiberCoincWalkXY_Y[0]->Draw("colz");

  fcHodoFiberCoinc->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFiberCoincMapXY[1]->Draw("colz");

  fcHodoFiberCoinc->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhHodoFiberCoincTimeXY[1]->Draw("hist");

  fcHodoFiberCoinc->cd(7);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFiberCoincWalkXY_X[1]->Draw("colz");

  fcHodoFiberCoinc->cd(8);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFiberCoincWalkXY_Y[1]->Draw("colz");

  fcHodoFiberCoincAB = new TCanvas("fcHodoFiberCoincAB", "X/Y coincidences between hodoscopes", w, h);
  fcHodoFiberCoincAB->Divide(4, 2);

  for (UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis) {
    fcHodoFiberCoincAB->cd(1 + 4 * uAxis);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhHodoFiberCoincMapSameAB[uAxis]->Draw("colz");

    fcHodoFiberCoincAB->cd(2 + 4 * uAxis);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fhHodoFiberCoincTimeSameAB[uAxis]->Draw("hist");

    fcHodoFiberCoincAB->cd(3 + 4 * uAxis);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhHodoFiberCoincMapDiffAB[uAxis]->Draw("colz");

    fcHodoFiberCoincAB->cd(4 + 4 * uAxis);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fhHodoFiberCoincTimeDiffAB[uAxis]->Draw("hist");
  }  // for( UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis )

  fcHodoFullCoinc = new TCanvas("fcHodoFullCoinc", "Full coincidences between hodoscopes", w, h);
  fcHodoFullCoinc->Divide(4, 2);

  fcHodoFullCoinc->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincCompX->Draw("colz");

  fcHodoFullCoinc->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincResidualXY->Draw("colz");

  fcHodoFullCoinc->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincTimeWalk[0][0]->Draw("colz");

  fcHodoFullCoinc->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincTimeWalk[0][1]->Draw("colz");

  fcHodoFullCoinc->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincCompY->Draw("colz");


  fcHodoFullCoinc->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhHodoFullCoincTimeDiff->Draw("hist");

  fcHodoFullCoinc->cd(7);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincTimeWalk[1][0]->Draw("colz");

  fcHodoFullCoinc->cd(8);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincTimeWalk[1][1]->Draw("colz");

  fcHodoFullCoincPos =
    new TCanvas("fcHodoFullCoincPos", "Hit Positions for Full coincidences between hodoscopes", w, h);
  fcHodoFullCoincPos->Divide(2);

  fcHodoFullCoincPos->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincPosA->Draw("colz");

  fcHodoFullCoincPos->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincPosB->Draw("colz");

  fcHodoFullCoincPosEvo =
    new TCanvas("fcHodoFullCoincPosEvo", "Hit Positions Evo for Full coincidences between hodoscopes", w, h);
  fcHodoFullCoincPosEvo->Divide(4);

  fcHodoFullCoincPosEvo->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincPosEvo[0][0]->Draw("colz");

  fcHodoFullCoincPosEvo->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincPosEvo[0][1]->Draw("colz");

  fcHodoFullCoincPosEvo->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincPosEvo[1][0]->Draw("colz");

  fcHodoFullCoincPosEvo->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHodoFullCoincPosEvo[1][1]->Draw("colz");

  fcHodoPrevHitDt = new TCanvas("fcHodoPrevHitDt", "Time difference between current and previous hits", w, h);
  fcHodoPrevHitDt->Divide(2, 2);

  fcHodoPrevHitDt->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhPrevHitDtAllAsics->Draw("hist");

  fcHodoPrevHitDt->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhPrevHitDtAsicA->Draw("hist");

  fcHodoPrevHitDt->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhPrevHitDtAsicB->Draw("hist");

  fcHodoPrevHitDt->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhPrevHitDtAsicsAB->Draw("hist");

  AddCanvasToVector(fcHodoFiberCoinc, "canvases");
  AddCanvasToVector(fcHodoFiberCoincAB, "canvases");
  AddCanvasToVector(fcHodoFullCoinc, "canvases");
  AddCanvasToVector(fcHodoFullCoincPos, "canvases");
  AddCanvasToVector(fcHodoFullCoincPosEvo, "canvases");

  return kTRUE;
}
Bool_t CbmCosy2019MonitorAlgoHodo::FillHistograms()
{
  /// Prepare storage variables
  std::vector<std::vector<stsxyter::FinalHit>> lastHitHodoAxis;
  std::vector<std::vector<Bool_t>> bHitFoundHodoAxis;
  lastHitHodoAxis.resize(kuNbHodos);
  bHitFoundHodoAxis.resize(kuNbHodos);
  for (UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx) {
    lastHitHodoAxis[uHodoIdx].resize(kuNbAxis);
    bHitFoundHodoAxis[uHodoIdx].resize(kuNbAxis, kFALSE);
  }  // for( UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx )

  /// Loop on Hits
  UInt_t uTotalNbHits = fvmHitsInMs.size();
  for (UInt_t uHit = 0; uHit < uTotalNbHits; ++uHit) {
    UInt_t uHodo = fvmHitsInMs[uHit].GetAsic();
    UInt_t uAxis = fvmHitsInMs[uHit].GetDpb();
    //      UInt_t uFiber = fvmHitsInMs[ uHit ].GetChan();

    /// Setup debugging
    Double_t dCurrentHitTime = fvmHitsInMs[uHit].GetTs() * stsxyter::kdClockCycleNs;
    fhPrevHitDtAllAsics->Fill(dCurrentHitTime - fvdPreviousHitTimePerAsic[fuPreviousHitAsic]);
    if (0 == uHodo) {
      fhPrevHitDtAsicA->Fill(dCurrentHitTime - fvdPreviousHitTimePerAsic[0]);
      fhPrevHitDtAsicsAB->Fill(dCurrentHitTime - fvdPreviousHitTimePerAsic[1]);
    }  // if( 0 == uHodo )
    else
      fhPrevHitDtAsicB->Fill(dCurrentHitTime - fvdPreviousHitTimePerAsic[1]);

    fuPreviousHitAsic                = uHodo;
    fvdPreviousHitTimePerAsic[uHodo] = dCurrentHitTime;

    lastHitHodoAxis[uHodo][uAxis]   = fvmHitsInMs[uHit];
    bHitFoundHodoAxis[uHodo][uAxis] = kTRUE;

    /// Coincidences in same Hodo <= !!!! WORKS ONLY FOR kuNbAxis = 2 !!!
    if (bHitFoundHodoAxis[uHodo][0] && bHitFoundHodoAxis[uHodo][1]) {
      Double_t dTimeDiffAxis = lastHitHodoAxis[uHodo][1].GetTs() * stsxyter::kdClockCycleNs
                               - lastHitHodoAxis[uHodo][0].GetTs() * stsxyter::kdClockCycleNs;

      if (-fdTimeCoincLimitNs < dTimeDiffAxis && dTimeDiffAxis < fdTimeCoincLimitNs) {
        fhHodoFiberCoincMapXY[uHodo]->Fill(lastHitHodoAxis[uHodo][0].GetChan(), lastHitHodoAxis[uHodo][1].GetChan());
        fhHodoFiberCoincTimeXY[uHodo]->Fill(dTimeDiffAxis);
        fhHodoFiberCoincWalkXY_X[uHodo]->Fill(lastHitHodoAxis[uHodo][0].GetAdc(), dTimeDiffAxis);
        fhHodoFiberCoincWalkXY_Y[uHodo]->Fill(lastHitHodoAxis[uHodo][1].GetAdc(), dTimeDiffAxis);
      }  // if( -fdTimeCoincLimitNs < dTimeDiffAxis && dTimeDiffAxis < fdTimeCoincLimitNs )
    }    // if( bHitFoundHodoAxis[ uHodo ][ 0 ] && bHitFoundHodoAxis[ uHodo ][ 1 ] )

    /// Concidences between Hodos <= !!!! WORKS ONLY FOR kuNbHodos = 2 && kuNbAxis = 2 !!!
    if (bHitFoundHodoAxis[0][uAxis] && bHitFoundHodoAxis[1][uAxis]) {
      Double_t dTimeDiffHodoSame = lastHitHodoAxis[1][uAxis].GetTs() * stsxyter::kdClockCycleNs
                                   - lastHitHodoAxis[0][uAxis].GetTs() * stsxyter::kdClockCycleNs;

      if (-fdTimeCoincLimitNs < dTimeDiffHodoSame && dTimeDiffHodoSame < fdTimeCoincLimitNs) {
        fhHodoFiberCoincMapSameAB[uAxis]->Fill(lastHitHodoAxis[0][uAxis].GetChan(),
                                               lastHitHodoAxis[1][uAxis].GetChan());
        fhHodoFiberCoincTimeSameAB[uAxis]->Fill(dTimeDiffHodoSame);
      }  // if( -fdTimeCoincLimitNs < dTimeDiffAxis && dTimeDiffAxis < fdTimeCoincLimitNs )
    }    // if( bHitFoundHodoAxis[ 0 ][ uAxis ] && bHitFoundHodoAxis[ 1 ][ uAxis ] )

    UInt_t uAxisA = (uHodo ? !uAxis : uAxis);
    UInt_t uAxisB = (uHodo ? uAxis : !uAxis);
    if (bHitFoundHodoAxis[0][uAxisA] && bHitFoundHodoAxis[1][uAxisB]) {
      Double_t dTimeDiffHodoDiff = lastHitHodoAxis[1][uAxisB].GetTs() * stsxyter::kdClockCycleNs
                                   - lastHitHodoAxis[0][uAxisA].GetTs() * stsxyter::kdClockCycleNs;

      if (-fdTimeCoincLimitNs < dTimeDiffHodoDiff && dTimeDiffHodoDiff < fdTimeCoincLimitNs) {
        fhHodoFiberCoincMapDiffAB[uAxisA]->Fill(lastHitHodoAxis[0][uAxisA].GetChan(),
                                                lastHitHodoAxis[1][uAxisB].GetChan());
        fhHodoFiberCoincTimeDiffAB[uAxisA]->Fill(dTimeDiffHodoDiff);
      }  // if( -fdTimeCoincLimitNs < dTimeDiffAxis && dTimeDiffAxis < fdTimeCoincLimitNs )
    }

    /// Full Concidences between Hodos <= !!!! WORKS ONLY FOR kuNbHodos = 2 && kuNbAxis = 2 !!!
    if (bHitFoundHodoAxis[0][0] && bHitFoundHodoAxis[0][1] && bHitFoundHodoAxis[1][0] && bHitFoundHodoAxis[1][1]) {
      Double_t dTimeDiffHodoA = lastHitHodoAxis[0][1].GetTs() * stsxyter::kdClockCycleNs
                                - lastHitHodoAxis[0][0].GetTs() * stsxyter::kdClockCycleNs;
      Double_t dTimeDiffHodoB = lastHitHodoAxis[1][1].GetTs() * stsxyter::kdClockCycleNs
                                - lastHitHodoAxis[1][0].GetTs() * stsxyter::kdClockCycleNs;
      Double_t dTimeDiffHodoAB = (lastHitHodoAxis[1][1].GetTs() * stsxyter::kdClockCycleNs
                                  + lastHitHodoAxis[1][0].GetTs() * stsxyter::kdClockCycleNs)
                                   / 2.0
                                 - (lastHitHodoAxis[0][1].GetTs() * stsxyter::kdClockCycleNs
                                    + lastHitHodoAxis[0][0].GetTs() * stsxyter::kdClockCycleNs)
                                     / 2.0;
      Double_t dTimeHitHodoAB = (lastHitHodoAxis[1][1].GetTs() * stsxyter::kdClockCycleNs
                                 + lastHitHodoAxis[1][0].GetTs() * stsxyter::kdClockCycleNs
                                 + lastHitHodoAxis[0][1].GetTs() * stsxyter::kdClockCycleNs
                                 + lastHitHodoAxis[0][0].GetTs() * stsxyter::kdClockCycleNs)
                                / 4.0;

      if (-fdTimeCoincLimitNs < dTimeDiffHodoA && dTimeDiffHodoA < fdTimeCoincLimitNs
          && -fdTimeCoincLimitNs < dTimeDiffHodoB && dTimeDiffHodoB < fdTimeCoincLimitNs
          && -fdTimeCoincLimitNs < dTimeDiffHodoAB && dTimeDiffHodoAB < fdTimeCoincLimitNs) {
        UInt_t uPosXA = fvbHodoSwapXY[0] ? lastHitHodoAxis[0][1].GetChan() : lastHitHodoAxis[0][0].GetChan();
        UInt_t uPosYA = fvbHodoSwapXY[0] ? lastHitHodoAxis[0][0].GetChan() : lastHitHodoAxis[0][1].GetChan();
        UInt_t uPosXB = fvbHodoSwapXY[1] ? lastHitHodoAxis[1][1].GetChan() : lastHitHodoAxis[1][0].GetChan();
        UInt_t uPosYB = fvbHodoSwapXY[1] ? lastHitHodoAxis[1][0].GetChan() : lastHitHodoAxis[1][1].GetChan();

        if (fvbHodoInvertX[0]) uPosXA = kuNbChanPerAsic / 2 - 1 - uPosXA;
        if (fvbHodoInvertY[0]) uPosYA = kuNbChanPerAsic / 2 - 1 - uPosYA;
        if (fvbHodoInvertX[1]) uPosXB = kuNbChanPerAsic / 2 - 1 - uPosXB;
        if (fvbHodoInvertY[1]) uPosYB = kuNbChanPerAsic / 2 - 1 - uPosYB;

        Double_t dResX = uPosXB;
        Double_t dResY = uPosYB;

        dResX -= uPosXA;
        dResY -= uPosYA;

        fhHodoFullCoincPosA->Fill(uPosXA, uPosYA);
        fhHodoFullCoincPosB->Fill(uPosXB, uPosYB);

        fhHodoFullCoincCompX->Fill(uPosXA, uPosXB);
        fhHodoFullCoincCompY->Fill(uPosYA, uPosYB);
        fhHodoFullCoincResidualXY->Fill(dResX, dResY);
        fhHodoFullCoincTimeDiff->Fill(dTimeDiffHodoAB);

        fhHodoFullCoincTimeWalk[0][0]->Fill(lastHitHodoAxis[0][0].GetAdc(), dTimeDiffHodoAB);
        fhHodoFullCoincTimeWalk[0][1]->Fill(lastHitHodoAxis[0][1].GetAdc(), dTimeDiffHodoAB);
        fhHodoFullCoincTimeWalk[1][0]->Fill(lastHitHodoAxis[1][0].GetAdc(), dTimeDiffHodoAB);
        fhHodoFullCoincTimeWalk[1][1]->Fill(lastHitHodoAxis[1][1].GetAdc(), dTimeDiffHodoAB);

        Double_t dTimeSinceStart = (dTimeHitHodoAB - fdStartTime) * 1e-9;
        fhHodoFullCoincRateEvo->Fill(dTimeSinceStart);

        fhHodoFullCoincPosEvo[0][0]->Fill(dTimeSinceStart, uPosXA);
        fhHodoFullCoincPosEvo[0][1]->Fill(dTimeSinceStart, uPosYA);
        fhHodoFullCoincPosEvo[1][0]->Fill(dTimeSinceStart, uPosXB);
        fhHodoFullCoincPosEvo[1][1]->Fill(dTimeSinceStart, uPosYB);
        /*
            LOG(info) << "Hodoscopes full coincidence found at " << (dTimeHitHodoAB*1e-9)
                      << " Position A = ( " << uPosXA << ", " << uPosYA << ")"
                      << " Position B = ( " << uPosXB << ", " << uPosYB << ")"
                      << " dT = " << dTimeDiffHodoAB;
*/
      }  // if all hodo axis in coinc and hodos in coinc
    }    // if all ( Hodo, Axis ) pairs have at least 1 hit
         /*
      for( UInt_t uHodoOther = 0; uHodoOther < kuNbHodos; ++uHodoOther )
      {
         for( UInt_t uAxisOther = 0; uAxisOther < kuNbAxis; ++uAxisOther )
         {
            if( uHodoOther == uHodo )
            {
               if( uAxisOther == uAxis )
                  continue;

            } // if( uHodoOther == uHodo )

         } // for( UInt_t uAxisOther = 0; uAxisOther < kuNbAxis; ++uAxisOther )
      } // for( UInt_t uHodoOther = 0; uHodoOther < kuNbHodos; ++uHodoOther )
*/
  }      // for( UInt_t uHit = 0; uHit < uTotalNbHits; ++uHit )

  return kTRUE;
}
Bool_t CbmCosy2019MonitorAlgoHodo::ResetHistograms()
{
  fhHodoMessType->Reset();
  fhHodoStatusMessType->Reset();
  fhHodoMsStatusFieldType->Reset();
  fhHodoMessTypePerElink->Reset();

  for (UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx) {
    for (UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis) {
      fhHodoFiberCnt[uHodoIdx][uAxis]->Reset();
      fhHodoFiberAdc[uHodoIdx][uAxis]->Reset();
      fhHodoFiberAdcProf[uHodoIdx][uAxis]->Reset();
      fhHodoFiberHitRateEvo[uHodoIdx][uAxis]->Reset();

      fhHodoFullCoincTimeWalk[uHodoIdx][uAxis]->Reset();
      fhHodoFullCoincPosEvo[uHodoIdx][uAxis]->Reset();
    }  // for( UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis )

    fhHodoChanCntRaw[uHodoIdx]->Reset();
    fhHodoChanAdcRaw[uHodoIdx]->Reset();
    fhHodoChanAdcRawProf[uHodoIdx]->Reset();
    fhHodoChanAdcCal[uHodoIdx]->Reset();
    fhHodoChanAdcCalProf[uHodoIdx]->Reset();
    fhHodoChanRawTs[uHodoIdx]->Reset();
    fhHodoChanMissEvt[uHodoIdx]->Reset();
    fhHodoChanMissEvtEvo[uHodoIdx]->Reset();
    fhHodoChanHitRateEvo[uHodoIdx]->Reset();
    fhHodoChanHitRateProf[uHodoIdx]->Reset();
    fhHodoChanDistT[uHodoIdx]->Reset();

    fhHodoFiberCoincMapXY[uHodoIdx]->Reset();
    fhHodoFiberCoincTimeXY[uHodoIdx]->Reset();
    fhHodoFiberCoincWalkXY_X[uHodoIdx]->Reset();
    fhHodoFiberCoincWalkXY_Y[uHodoIdx]->Reset();

  }  // for( UInt_t uHodoIdx = 0; uHodoIdx < kuNbHodos; ++uHodoIdx )


  for (UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis) {
    fhHodoFiberCoincMapSameAB[uAxis]->Reset();
    fhHodoFiberCoincTimeSameAB[uAxis]->Reset();
    fhHodoFiberCoincMapDiffAB[uAxis]->Reset();
    fhHodoFiberCoincTimeDiffAB[uAxis]->Reset();
  }  // for( UInt_t uAxis = 0; uAxis < kuNbAxis; ++uAxis )

  fhHodoFullCoincPosA->Reset();
  fhHodoFullCoincPosB->Reset();
  fhHodoFullCoincCompX->Reset();
  fhHodoFullCoincCompY->Reset();
  fhHodoFullCoincResidualXY->Reset();
  fhHodoFullCoincTimeDiff->Reset();

  fhPrevHitDtAllAsics->Reset();
  fhPrevHitDtAsicA->Reset();
  fhPrevHitDtAsicB->Reset();
  fhPrevHitDtAsicsAB->Reset();

  return kTRUE;
}
// -------------------------------------------------------------------------
