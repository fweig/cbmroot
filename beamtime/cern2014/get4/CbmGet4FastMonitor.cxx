// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmGet4FastMonitor                           -----
// -----                    Created 15.12.2014 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                  Based on CbmGet4FastMonitor                       -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmGet4FastMonitor.h"

// Specific headers
#include "CbmGet4v1xDef.h"

// FAIRROOT headers
#include "FairLogger.h"
#include "FairRootManager.h"

// ROOT headers
#include "TF1.h"
#include "TProfile.h"
#include "TH2.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TMath.h"
#include "TTimeStamp.h"
#include "TROOT.h"

// C++ std headers
#include <iostream>
#include <stdint.h>

/*********
 * DONE
 * 2) Counts as function of time (s bin, per ch/chip/ROC/Det) => / OK / OK / OK
 * 3) Split class => OK
 *********/

// Default values
const Double_t kdEvoBinSizeS  =    1.5;
const Double_t kdEvoLengthSec = 1800.0;
//const Double_t kdMsSizeSec    = 1e-3; // up to 01/03 15:00
const Double_t kdMsSizeSec    = 16384*8*(1e-9); // From to 01/03 15:00

const Int_t   kiNbDet            = 4;
const TString ksDetName[kiNbDet] = {"HDP2", "TSU", "HDP5", "USTC"};
const UInt_t  kuNbTdcMappedDet   = 88;
const Int_t   kiTdcDetMap[kuNbTdcMappedDet]    =
   {
      0, 0, 0, 0, 0, 0, 0, 0, // ROC 0 FEE 0
      0, 0, 0, 0, 0, 0, 0, 0, // ROC 0 FEE 1
      1, 1, 1, 1, 1, 1, 1, 1, // ROC 0 FEE 2
      4, 4, 4, 4, 4, 4, 4, 4, // ROC 0 FEE 3
      4, 4, 4, 4, 4, 4, 4, 4, // ROC 0 FEE 4
      4, 4, 4, 4, 4, 4, 4, 4, // ROC 0 FEE 5
      4, 4, 4, 4, 4, 4, 4, 4, // ROC 0 FEE 6
      4, 4, 4, 4, 4, 4, 4, 4, // ROC 0 FEE 7
      1, 1, 1, 1, 1, 1, 1, 1, // ROC 1 FEE 0
      2, 2, 2, 2, 2, 2, 2, 2, // ROC 1 FEE 1
      3, 3, 3, 3, 3, 3, 3, 3, // ROC 1 FEE 2
   };

// Defautl value for nb bins in Pulser time difference histos
const UInt_t kuNbBinsDt    = 5000;
Double_t dMinDt     = -1.*(kuNbBinsDt*get4v1x::kdBinSize/2.) -get4v1x::kdBinSize/2.;
Double_t dMaxDt     =  1.*(kuNbBinsDt*get4v1x::kdBinSize/2.) +get4v1x::kdBinSize/2.;

struct DTM_header
{
   uint8_t packet_length;
   uint8_t packet_counter;
   uint16_t ROC_ID;

   void Dump() {
      printf ("HEADER ======================= pack_len=0x%02X, pack_cnt=0x%02X, ROC=0x%04X\n",
               packet_length, packet_counter, ROC_ID);
   }
};

CbmGet4FastMonitor::CbmGet4FastMonitor()
: CbmTSUnpack(),
  fbVerbose(kFALSE),
  fbDebug(kFALSE),
  fiDebugTsStart(-1),
  fiDebugTsStop(-1),
  fiMode(0),
  fsHistoFilename("./get4Monitor.hst.root"),
  fuNbRocs(0),
  fuNbGet4(0),
  fuNbFee(0),
  fuMsOverlapTs(0),
  fvbActiveChips(),
  fulTsNb(0),
  fulMsNb(0),
  fsMaxMsNb(0),
  fuOffset(0),
  fuMaxOffset(0),
  fuLocalOffset(0),
  fuMaxLocalOffset(0),
  fvbRocFeetFirstSyncDlm(kFALSE),
  fvbRocFeetSyncStart(kFALSE),
  fvbGet4WaitFirstSync(kTRUE),
  fvuCurrEpoch(),
  fvuCurrEpochCycle(),
  fvuCurrEpoch2(),
  fvuCurrEpoch2Cycle(),
  fhMessageTypePerRoc(NULL),
  fhRocSyncTypePerRoc(NULL),
  fhRocAuxTypePerRoc(NULL),
  fhSysMessTypePerRoc(NULL),
  fhMessTypePerGet4(NULL),
  fhGet4EpochFlags(NULL),
  fhGet4EpochSyncDist(NULL),
  fhGet4EpochJumps(NULL),
  fhGet4BadEpochRatio(NULL),
  fhGet4BadEpRatioChip(NULL),
  fhGet4ChanDataCount(NULL),
  fhGet4ChanDllStatus(NULL),
  fhGet4ChanTotMap(NULL),
  fhGet4ChanErrors(NULL),
  fhGet4ChanTotOwErrorsProp(NULL),
  fhGet4ChanTotOrErrorsProp(NULL),
  fhGet4ChanTotEdErrorsProp(NULL),
  fhGet4ChanSlowContM(NULL),
  fhGet4ChanEdgesCounts(NULL),
  fhGet4ChanDeadtime(NULL),
  fhGet4SeuCounter(NULL),
  fdEvoBinSizeS(kdEvoBinSizeS),
  fdEvoLengthSec(kdEvoLengthSec),
  fdMsLengthSec(kdMsSizeSec),
  fhMsSizeEvo(NULL),
  fvhRocDataCntEvo(),
  fvhRocEpochCntEvo(),
  fvhRocErrorCntEvo(),
  fvhRocTotErrorCntEvo(),
  fvhRocEpochMissmCntEvo(),
  fvhRocDataLossCntEvo(),
  fvhRocEpochLossCntEvo(),
  fvhGet4DataCntEvo(),
  fvhGet4ErrorCntEvo(),
  fvhGet4EpochMissmCntEvo(),
  fvhGet4DataLossCntEvo(),
  fvhGet4EpochLossCntEvo(),
  fvhGet4ChDataCntEvo(),
  fvhGet4ChErrorCntEvo(),
  fvhDetDataCntEvo(),
  fvhDetErrorCntEvo(),
  fvhDetEpochMissmCntEvo(),
  fvhDetDataLossCntEvo(),
  fvhDetEpochLossCntEvo(),
  fhClosedEpSizeEvo(NULL),
  fbPulserMode(kFALSE),
  fuPulserFee(0),
  fuPulserChan(),
  fvuLastHitEp(),
  fvmLastHit(),
  fhTimeResFee(),
  fhTimeRmsAllFee(NULL),
  fhTimeResAllFee(NULL),
  fbPulserFeeB(kFALSE),
  fuPulserFeeB(kFALSE),
  fhTimeResFeeB(),
  fhTimeRmsAllFeeB(NULL),
  fhTimeResAllFeeB(NULL),
  fhTimeResFeeAB(),
  fhTimeRmsAllFeeAB(NULL),
  fhTimeResAllFeeAB(NULL),
  fhTimeRmsAllPairs(NULL),
  fhTimeResAllPairs(NULL),
  fhTimeRmsAllCombi(NULL),
  fhTimeResAllCombi(NULL),
  fhTimeResPairs(),
  fhTimeResCombi(),
  fhTimeRmsAllCombiEncA(NULL),
  fhTimeResAllCombiEncA(NULL),
  fhTimeRmsAllCombiEncB(NULL),
  fhTimeResAllCombiEncB(NULL),
  fhPulserHitDistNs(NULL),
  fhPulserHitDistUs(NULL),
  fhPulserHitDistMs(NULL),
  fhPulserFeeDnl(NULL),
  fhPulserFeeInl(NULL),
  fhPulserFeeDistCT(),
  fvuPrevHitEp(),
  fvmPrevHit(),
  fhFtSmallDtFeeA(NULL),
  fhFtSmallDtFeeB(NULL),
  fhFtBigDtFeeA(NULL),
  fhFtBigDtFeeB(NULL),
  fhFtPrevSmallDtFeeA(NULL),
  fhFtPrevSmallDtFeeB(NULL),
  fhFtPrevBigDtFeeA(NULL),
  fhFtPrevBigDtFeeB(NULL),
  fhFullCtEpJumpFeeA(),
  fhFullCtEpJumpFeeACh(),
  fhFullCtEpJumpFeeAChSort(),
  fhFullCtEpJumpFeeAChOrder(NULL),
  fvChanOrder(),
  fhFullCtChOrderCh(),
  fhFullCtEpQualityCh(),
  fhFullCtEpQualityChZoom(),
  fhEpQualityFirstChFeeA(),
  fbOldReadoutOk(kFALSE),
  fhGet4ChanTotCount(NULL),
  fvuLastOldTotEp(),
  fvmLastOldTot(),
  fhPulserFeeTotDnl(NULL),
  fhPulserFeeTotInl(NULL),
  fhPulserFeeRisCtWideBins(NULL),
  fhPulserFeeFalCtWideBins(NULL),
  fbEnableMissingEdgeCheck(kFALSE),
  fvuLastTotInFtBins(),
  fhPulserFeeFtRecoMissRis(NULL),
  fhPulserFeeFtRecoMissFal(NULL),
  fvuNbRisEdgeEpoch(),
  fvuNbFalEdgeEpoch(),
  fhPulserFeeExtraRisEp(NULL),
  fhPulserFeeExtraFalEp(NULL),
  fhPulserFeeExtraEdgesEp(NULL),
  fvuFeePrevRisEp(),
  fvmFeePrevRis(),
  fvuFeePrevFalEp(),
  fvmFeePrevFal(),
  fhPulserFeeFtExtraEdgeRisA(NULL),
  fhPulserFeeFtExtraEdgeFalA(NULL),
  fhPulserFeeFtExtraEdgeRisB(NULL),
  fhPulserFeeFtExtraEdgeFalB(NULL),
  fhPulserFeeGoodTot(NULL),
  fhPulserFeeExtraRecoTot(NULL),
  fhPulserFeeTotDistCT(),
  fvuPrevOldTotEp(),
  fvmPrevOldTot(),
  fhFtTotSmallDtFeeA(NULL),
  fhFtTotSmallDtFeeB(NULL),
  fhFtTotBigDtFeeA(NULL),
  fhFtTotBigDtFeeB(NULL),
  fhFtPrevTotSmallDtFeeA(NULL),
  fhFtPrevTotSmallDtFeeB(NULL),
  fhFtPrevTotBigDtFeeA(NULL),
  fhFtPrevTotBigDtFeeB(NULL),
  fvvhChannelsCoinc(),
  fdMaxCoincDist(100.0),
  fvhGet4MultipleHits(),
  fvhGet4DistDoubleHits(),
  fvhGet4DistTripleHits(),
  fvhGet4DistMultipleHits(),
  fvhGet4MultipleHitsVsTot(),
  fdMaxDtMultiHit(20.0),
  fvbChanSecondHit(),
  fvbChanThirdHit(),
  fvdChanFirstHitTot(),
  fbHistoWriteDone(kFALSE),
  fbEnaCalibOutput(kFALSE),
  fsCalibOutFoldername(""),
  fsCalibFilename(""),
  fvhFineTime()
{
}

CbmGet4FastMonitor::~CbmGet4FastMonitor()
{
}

Bool_t CbmGet4FastMonitor::Init()
{
  LOG(info) << "Initializing";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }
//  ioman->Register("Get4RawMessage",       "GET4 raw data",      fGet4Raw, kTRUE);
//  ioman->Register("NxyterRawSyncMessage", "GET4 raw sync data", fGet4RawSync, kTRUE);

  if( 0 >= fuNbRocs || 0 >= fuNbGet4 )
  {
     LOG(error) << "CbmGet4FastMonitor::Init => Nb of ROCs or GET4s not defined!!! ";
     LOG(error) << "Nb of ROCs:"<< fuNbRocs << " Nb of GET4s : " << fuNbGet4;
     LOG(fatal) << "Please use the functions SetRocNb and/or SetGet4Nb before running!!";
  }

  // Initialize TS counter
  fulTsNb = 0;

  // At start all ROCs and GET4 are considered "unsynchronized"
  // Stay so until the DLM 10 is issued: ~/flesnet/build/sync_frontend
  // The GET4 are synchronized after reception of 1st epoch2 with SYNC
  fvbRocFeetFirstSyncDlm.resize( fuNbRocs);
  fvbRocFeetSyncStart.resize( fuNbRocs);
  for( UInt_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
  {
     fvbRocFeetFirstSyncDlm[uRoc] = kFALSE;
     fvbRocFeetSyncStart[uRoc] = kFALSE;
  } // for( UInt_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
  fvbGet4WaitFirstSync.resize(fuNbGet4);
  for( UInt_t uChip = 0; uChip < fuNbGet4; uChip++)
     fvbGet4WaitFirstSync[uChip] = kTRUE;

  // Prepare the active chips flags
  if( 0 == fvbActiveChips.size() && 0 < fuNbGet4  )
  {
     fvbActiveChips.resize(fuNbGet4);
     for( UInt_t uChip = 0; uChip < fuNbGet4; uChip++)
        fvbActiveChips[uChip] = kTRUE;
  } // if( 0 == fvbActiveChips.size()
  // Prepare the epoch storing vectors
  fvuCurrEpoch.resize( fuNbRocs);
  fvuCurrEpochCycle.resize( fuNbRocs);
  for( UInt_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
  {
     fvuCurrEpoch[uRoc] = 0;
     fvuCurrEpochCycle[uRoc] = 0;
  } // for( UInt_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
  fvuCurrEpoch2.resize(fuNbGet4);
  fvuCurrEpoch2Cycle.resize(fuNbGet4);
  for( UInt_t uChip = 0; uChip < fuNbGet4; uChip++)
  {
     fvuCurrEpoch2[uChip] = 0;
     fvuCurrEpoch2Cycle[uChip] = 0;
  } // for( UInt_t uChip = 0; uChip < fuNbGet4; uChip++)

  LOG(info) << "Initializing Histos";
  InitMonitorHistograms();
  LOG(info) << "Initialization done";

  PrintOptions();

  return kTRUE;
}

void CbmGet4FastMonitor::PrintOptions()
{
   LOG(info) << "Nb of ROCs: "<< fuNbRocs << " Nb of GET4s : " << fuNbGet4;
   LOG(info) << "TS overlap: "<< fuMsOverlapTs << " MS ";

   LOG(info) << "Monitor mode: "<< fiMode;

   LOG(info) << "Active chips: ";
   for( UInt_t iChip = 0; iChip < fuNbGet4; iChip++)
      LOG(info) << Form(" %3u", iChip);
   LOG(info);
   for( UInt_t iChip = 0; iChip < fuNbGet4; iChip++)
      LOG(info) << Form(" %3d", static_cast<Int_t> (fvbActiveChips[iChip]) );
   LOG(info) << "";

   LOG(info) << "Pulser mode: "<< fbPulserMode;
   LOG(info) << "Pulser FEE board: "<< fuPulserFee;
   if( kTRUE == fbPulserFeeB )
   {
      LOG(info) << "Second FEE B board: ON";
      LOG(info) << "Pulser FEE B board: "<< fuPulserFeeB;
   } // if( kTRUE == fbPulserFeeB )
   LOG(info) << "Pulser channels: ";
   for( UInt_t iChan = 0; iChan < kuNbChanTest; iChan++)
      LOG(info) << Form(" %3d", fuPulserChan[iChan] );
   LOG(info) << "";

   LOG(info) << "Binning evolution plots: "<< fdEvoBinSizeS  << " s";
   LOG(info) << "Length  evolution plots: "<< fdEvoLengthSec << " s";
   LOG(info) << "MS length: "<< fdMsLengthSec << " s";
   LOG(info) << "24b mode allowed: "<< fbOldReadoutOk;

   LOG(info) << "Max time dist for coincidences:  "<< fdMaxCoincDist  << " ps";
   LOG(info) << "Max time dist for multiple hits: "<< fdMaxDtMultiHit << " ps";

   LOG(info) << "Calibration saving enabled: "<< fbEnaCalibOutput;
   LOG(info) << "Calibration out folder: "<< fsCalibOutFoldername;
   LOG(info) << "Calibration out file: "<< fsCalibFilename;

}

void CbmGet4FastMonitor::SetActiveGet4( UInt_t uChipsIndex, Bool_t bActiveFlag)
{
   if( 0 == fvbActiveChips.size() && 0 < fuNbGet4 )
   {
      fvbActiveChips.resize(fuNbGet4);
      for( UInt_t uChip = 0; uChip < fuNbGet4; uChip++)
         fvbActiveChips[uChip] = kTRUE;
   } // if( 0 == fvbActiveChips.size()

   if( uChipsIndex < fuNbGet4 )
      fvbActiveChips[uChipsIndex] = bActiveFlag;
      else LOG(error)<<" CbmGet4FastMonitor::SetActiveGet4 => Invalid chip index "
                     << uChipsIndex ;
}

void CbmGet4FastMonitor::SetPulserChans(
      UInt_t inPulserChanA, UInt_t inPulserChanB, UInt_t inPulserChanC, UInt_t inPulserChanD,
      UInt_t inPulserChanE, UInt_t inPulserChanF, UInt_t inPulserChanG, UInt_t inPulserChanH,
      UInt_t inPulserChanI, UInt_t inPulserChanJ, UInt_t inPulserChanK, UInt_t inPulserChanL,
      UInt_t inPulserChanM, UInt_t inPulserChanN, UInt_t inPulserChanO, UInt_t inPulserChanP )
{
   fuPulserChan[ 0] = inPulserChanA;
   fuPulserChan[ 1] = inPulserChanB;
   fuPulserChan[ 2] = inPulserChanC;
   fuPulserChan[ 3] = inPulserChanD;
   fuPulserChan[ 4] = inPulserChanE;
   fuPulserChan[ 5] = inPulserChanF;
   fuPulserChan[ 6] = inPulserChanG;
   fuPulserChan[ 7] = inPulserChanH;
   fuPulserChan[ 8] = inPulserChanI;
   fuPulserChan[ 9] = inPulserChanJ;
   fuPulserChan[10] = inPulserChanK;
   fuPulserChan[11] = inPulserChanL;
   fuPulserChan[12] = inPulserChanM;
   fuPulserChan[13] = inPulserChanN;
   fuPulserChan[14] = inPulserChanO;
   fuPulserChan[15] = inPulserChanP;
}

void CbmGet4FastMonitor::SetHistoFilename( TString sNameIn )
{
   fsHistoFilename = sNameIn;

   LOG(info)<<" CbmGet4FastMonitor::SetHistoFilename => Histograms output file is now \n"
             <<fsHistoFilename;
}
Bool_t CbmGet4FastMonitor::DoUnpack(const fles::Timeslice& ts, size_t component)
{
   fulTsNb++;
   if( 0 == fiMode || kTRUE == fbVerbose ||
       (kTRUE == fbDebug && fiDebugTsStart <= fiDebugTsStop &&
       static_cast<UInt_t> (fiDebugTsStart) <= fulTsNb &&
      fulTsNb <= static_cast<UInt_t> (fiDebugTsStop) ) )
      LOG(info)<<" ++++++++++++ Ts # "<<fulTsNb;

   if( 2 == fiMode )
      LOG(debug)<<" ++++++++++++ Ts # "<<fulTsNb<<" Start!";

   // Loop over microslices
   fsMaxMsNb = ts.num_microslices(component) - fuMsOverlapTs;
   for (size_t m = 0; m < fsMaxMsNb; ++m)
   {
      fulMsNb = m;
      auto msDescriptor = ts.descriptor(component, m);
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(component, m));

      uint32_t offset = 16;
      uint32_t local_offset = 0;
      const uint8_t* msContent_shifted;
      int padding;

      if( 0 == fiMode || kTRUE == fbVerbose )
         LOG(info)<<" ************ Ms # "<<m;

      fuMaxOffset = msDescriptor.size;

      // AdHoc conversion factor for TS number to time
      // Work only for 2 links and 1 MS per TS
      fhMsSizeEvo->Fill( (fulTsNb/2)*fdMsLengthSec, fuMaxOffset);

      // Loop over the data of one microslice
      while (offset < msDescriptor.size)
      {
         msContent_shifted = &msContent[offset];
         fuOffset = offset;

         // Extract DTM header info
         DTM_header cur_DTM_header;

         cur_DTM_header.packet_length = msContent_shifted[0];
         cur_DTM_header.packet_counter = msContent_shifted[1];
         const uint16_t* ROC_ID_pointer = reinterpret_cast<const uint16_t*>(&msContent_shifted[2]);
//         cur_DTM_header.ROC_ID = *ROC_ID_pointer;
         // TODO: Check if planned behavior
         //       -> ROC ID increased by 256 per link, 16 bits => only links 1 and 2 easy to use
         cur_DTM_header.ROC_ID = (*ROC_ID_pointer) / 256; // ROC ID increased by 256 per link

         uint32_t packageSize = static_cast<uint32_t>(cur_DTM_header.packet_length*2+4);
         fuMaxLocalOffset = packageSize;

         // Loop over messages
         local_offset = 4;
         if( 0 == fiMode || kTRUE == fbVerbose )
         {
            // Debug printout
            cur_DTM_header.Dump();
            LOG(info)<<" Package Size: "<<packageSize<<" ROC ID: "<<cur_DTM_header.ROC_ID;
            while (local_offset < packageSize)
            {
               fuLocalOffset = local_offset;
               get4v1x::Message mess;
               uint64_t dataContent =
                       ( static_cast<uint64_t>( cur_DTM_header.ROC_ID                & 0xFFFF) << 48)
                     + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 0] ) & 0xFF) << 40)
                     + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 1] ) & 0xFF) << 32)
                     + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 2] ) & 0xFF) << 24)
                     + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 3] ) & 0xFF) << 16)
                     + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 4] ) & 0xFF) <<  8)
                     + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 5] ) & 0xFF));
               mess.setData( dataContent );
               mess.printDataLog();

               local_offset += 6; // next message
            } // while (local_offset < packageSize)
         } // if( 0 == fiMode || kTRUE == fbVerbose )
         switch(fiMode)
         {
            case 0:
            {
               // debug mode, nothing to do here (printout already done)
               break;
            } // case 0
            case 1:
            {
               // Monitor mode, fill histograms
               while (local_offset < packageSize)
               {
                  fuLocalOffset = local_offset;
                  get4v1x::Message mess;
                  uint64_t dataContent =
                          ( static_cast<uint64_t>( cur_DTM_header.ROC_ID                & 0xFFFF) << 48)
                        + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 0] ) & 0xFF) << 40)
                        + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 1] ) & 0xFF) << 32)
                        + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 2] ) & 0xFF) << 24)
                        + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 3] ) & 0xFF) << 16)
                        + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 4] ) & 0xFF) <<  8)
                        + ( static_cast<uint64_t>( (msContent_shifted[local_offset + 5] ) & 0xFF));
                  mess.setData( dataContent );

                  if( kTRUE == fbDebug && fiDebugTsStart <= fiDebugTsStop &&
                      static_cast<UInt_t> (fiDebugTsStart) <= fulTsNb &&
                      fulTsNb <= static_cast<UInt_t> (fiDebugTsStop) )
                     mess.printDataLog();

                  fhMessageTypePerRoc->Fill( cur_DTM_header.ROC_ID, mess.getMessageType() );

                  switch( mess.getMessageType() )
                  {
                     case get4v1x::MSG_HIT:
                     {
                        // This is NXYTER in a GET4 unpacker => ignore
                        break;
                     } // case get4v1x::MSG_HIT:
                     case get4v1x::MSG_EPOCH:
                     {
                        // Ignore all messages before RocFeet system SYNC
                        if (kFALSE == fvbRocFeetSyncStart[cur_DTM_header.ROC_ID] )
                           break;
                        this->MonitorMessage_epoch(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_EPOCH:
                     case get4v1x::MSG_SYNC:
                     {
                        // Ignore all messages before RocFeet system SYNC
                        if (kFALSE == fvbRocFeetSyncStart[cur_DTM_header.ROC_ID] )
                           break;
                        this->MonitorMessage_sync(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_SYNC:
                     case get4v1x::MSG_AUX:
                     {
                        // Ignore all messages before RocFeet system SYNC
                        if (kFALSE == fvbRocFeetSyncStart[cur_DTM_header.ROC_ID] )
                           break;
                        this->MonitorMessage_aux(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_AUX:
                     case get4v1x::MSG_EPOCH2:
                     {
                        // Ignore all messages before RocFeet system SYNC
                        if (kFALSE == fvbRocFeetSyncStart[cur_DTM_header.ROC_ID] )
                           break;
                        this->MonitorMessage_epoch2(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_EPOCH2:
                     case get4v1x::MSG_GET4:
                     {
                        // Ignore all messages before RocFeet system SYNC
                        if (kFALSE == fvbRocFeetSyncStart[cur_DTM_header.ROC_ID] )
                           break;
                        this->MonitorMessage_get4(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_GET4:
                     case get4v1x::MSG_SYS:
                        this->MonitorMessage_sys(mess, msDescriptor.eq_id);
                        break;
                     default:
                        break;
                  } // switch( mess.getMessageType() )
                  local_offset += 6; // next message
               } // while (local_offset < packageSize)
               break;
            } // case 1
            default:
               break;
         } // switch(fiMode)

         offset += packageSize;

         // shift some more bytes to fit the CbmNet package size
         if ((padding = offset % 8) > 0)
         {
            offset += (8-padding);
         } // if ((padding = offset % 8) > 0)

      } // while (offset < msDescriptor.size)

   } // for (size_t m = 0; m < ts.num_microslices(component); ++m)

  FinishUnpack();

  return kTRUE;
}

void CbmGet4FastMonitor::FinishUnpack()
{
}

void CbmGet4FastMonitor::Reset()
{
//  fGet4Raw->Clear();
//  fGet4RawSync->Clear();
}

void CbmGet4FastMonitor::Finish()
{
   if( kFALSE == fbHistoWriteDone )
   {
      WriteMonitorHistograms();
      fbHistoWriteDone = kTRUE;
   } // if( kFALSE == fbHistoWriteDone )
      else LOG(error)<<"CbmGet4FastMonitor::Finish => WriteMonitorHistograms already called once in this run, therefore ignore it";
   if( kTRUE == fbEnaCalibOutput )
      WriteCalibrationFile();
   DeleteMonitorHistograms();
}

//************** Monitor functions *************/


void CbmGet4FastMonitor::InitMonitorHistograms()
{
   // FIXME: maybe changed current folder to TRoot and back
   //        as for now all histos are placed in the analysis output file
   //        which leads to problems at Finish

   fhMessageTypePerRoc = new TH2I("hMessageTypePerRoc",
         "Nb of message for each type per ROC; ROC #; Type",
         fuNbRocs, -0.5, fuNbRocs -0.5,
         16, -0.5, 15.5);
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_NOP,      "NOP");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_HIT,      "HIT");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_EPOCH,    "EPOCH");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_SYNC,     "SYNC");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_AUX,      "AUX");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_EPOCH2,   "EPOCH2");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_GET4,     "GET4");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_SYS,      "SYS");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_GET4_SLC, "MSG_GET4_SLC");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::MSG_GET4_32B, "MSG_GET4_32B");
   fhMessageTypePerRoc->GetYaxis()->SetBinLabel(1 + 15, "GET4 Hack 32B");

   fhRocSyncTypePerRoc = new TH2I("hRocSyncTypePerRoc",
         "Nb of message for each SYNC type per ROC; ROC # ; SYNC Type",
         fuNbRocs, -0.5, fuNbRocs -0.5,
         get4v1x::kuMaxSync, -0.5, get4v1x::kuMaxSync - 0.5);

   fhRocAuxTypePerRoc = new TH2I("hRocAuxTypePerRoc",
         "Nb of message for each AUX type per ROC; ROC # ; AUX Type",
         fuNbRocs, -0.5, fuNbRocs -0.5,
         get4v1x::kuMaxAux, -0.5, get4v1x::kuMaxAux - 0.5);

   fhSysMessTypePerRoc = new TH2I("hSysMessTypePerRoc",
         "Nb of system message for each type per ROC; ROC # ; System Type",
         fuNbRocs, -0.5, fuNbRocs -0.5,
         16, -0.5, 15.5);
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_DAQ_START,       "DAQ START");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_NX_PARITY,       "NX PARITY");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_SYNC_PARITY,     "SYNC PARITY");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_DAQ_RESUME,      "DAQ RESUME");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_FIFO_RESET,      "FIFO RESET");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_USER,            "USER");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_PCTIME,          "PCTIME");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_ADC,             "ADC");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_PACKETLOST,      "PACKET LOST");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_GET4_EVENT,      "GET4 ERROR");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_CLOSYSYNC_ERROR, "CLOSYSYNC ERROR");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + get4v1x::SYSMSG_TS156_SYNC,      "TS156 SYNC");
   fhSysMessTypePerRoc->GetYaxis()->SetBinLabel(1 + 15, "GET4 Hack 32B");

   fhMessTypePerGet4 = new TH2I("hMessTypePerGet4",
         "Nb of message for each type per GET4; GET4 chip # ; Type",
         fuNbGet4, -0.5, fuNbGet4 -0.5,
         4, -0.5, 3.5);
   fhMessTypePerGet4->GetYaxis()->SetBinLabel(1 + get4v1x::GET4_32B_EPOCH, "EPOCH");
   fhMessTypePerGet4->GetYaxis()->SetBinLabel(1 + get4v1x::GET4_32B_SLCM,  "S.C. M");
   fhMessTypePerGet4->GetYaxis()->SetBinLabel(1 + get4v1x::GET4_32B_ERROR, "ERROR");
   fhMessTypePerGet4->GetYaxis()->SetBinLabel(1 + get4v1x::GET4_32B_DATA,  "DATA");

   fhGet4EpochFlags    = new TH2I("hGet4EpochFlags",
         "Number of epochs with corresponding flag set per GET4; GET4 # ;",
         fuNbGet4, -0.5, fuNbGet4 -0.5,
         4, -0.5, 3.5);
   fhGet4EpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
   fhGet4EpochFlags->GetYaxis()->SetBinLabel(2, "Ep. Missmatch");
   fhGet4EpochFlags->GetYaxis()->SetBinLabel(3, "Ep. Loss");
   fhGet4EpochFlags->GetYaxis()->SetBinLabel(4, "Data Loss");

   fhGet4EpochSyncDist = new TH2I("hGet4EpochSyncDist",
         "Distance between epochs with SYNC flag for each GET4; SYNC distance [epochs]; Epochs",
         fuNbGet4, -0.5, fuNbGet4 -0.5,
         2*get4v1x::kuSyncCycleSzGet4, -0.5, 2*get4v1x::kuSyncCycleSzGet4 -0.5);

   fhGet4EpochJumps    = new TH2I("hGet4EpochJumps",
         "Distance between epochs when jump happens for each GET4; GET4 Chip # ; Epoch Jump of [Epoch]",
         fuNbGet4, -0.5, fuNbGet4 -0.5,
         401, -200.5, 200.5);
   fhGet4BadEpochRatio = new TH1I("hGet4BadEpochRatio",
         "Counts of bad (missing, repeated, ...) epoch messages and good ones, mean represents G/T ratio; Epoch Quality; Counts []",
         2, -0.5, 1.5);
   fhGet4BadEpRatioChip= new TH2I("hGet4BadEpRatioChip",
         "Counts of bad (missing, repeated, ...) epoch messages and good ones, per chip; GET4 Chip # ; Epoch Quality; Counts []",
         fuNbGet4, -0.5, fuNbGet4 -0.5,
         2, -0.5, 1.5);

   fhGet4ChanDataCount = new TH1I("hGet4ChanDataCount",
         "Data Messages per GET4 channel; GET4 channel # ; Data Count",
         fuNbGet4*get4v1x::kuChanPerGet4, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5);

   fhGet4ChanDllStatus = new TH2I("hGet4ChanDllStatus",
         "DLL flag status per GET4 channel; GET4 channel # ; DLL Flag value Count",
         fuNbGet4*get4v1x::kuChanPerGet4 , -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5,
         2, -0.5, 1.5);

   fhGet4ChanTotMap    = new TH2I("hGet4ChanTotMap",
         "TOT values per GET4 channel; GET4 channel # ; TOT value [ns]",
         fuNbGet4*get4v1x::kuChanPerGet4, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5,
         50, 0.0, 50.0);

   fhGet4ChanErrors    = new TH2I("hGet4ChanErrors",
         "Error messages per GET4 channel; GET4 channel # ; Error",
         fuNbGet4*get4v1x::kuChanPerGet4*2, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5,
         32, -0.5, 31.5);
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(1,  "0x00: Readout Init    ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(2,  "0x01: Sync            ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(3,  "0x02: Epoch count sync");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(4,  "0x03: Epoch           ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(5,  "0x04: FIFO Write      ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(6,  "0x05: Lost event      ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(7,  "0x06: Channel state   ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(8,  "0x07: Token Ring state");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(9,  "0x08: Token           ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  "); // <- From GET4 v1.2
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc."); // <- From GET4 v1.2
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(17, "0x7f: Unknown         ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(18, "Corrupt error or unsupported yet");

   fhGet4ChanTotOwErrorsProp    = new TProfile("hGet4ChanTotOwErrorsProp",
         "Percentage of Overwrite Error messages per GET4 channel; GET4 channel # ; Overwrite prop. [\%]",
         fuNbGet4*get4v1x::kuChanPerGet4*2, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5);
   fhGet4ChanTotOrErrorsProp    = new TProfile("hGet4ChanTotOrErrorsProp",
         "Percentage of ToT out of range Error messages per GET4 channel; GET4 channel # ; Overwrite prop. [\%]",
         fuNbGet4*get4v1x::kuChanPerGet4*2, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5);
   fhGet4ChanTotEdErrorsProp    = new TProfile("hGet4ChanTotEdErrorsProp",
         "Percentage of Event Discarded Error messages per GET4 channel; GET4 channel # ; Overwrite prop. [\%]",
         fuNbGet4*get4v1x::kuChanPerGet4*2, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5);

   fhGet4ChanSlowContM = new TH2I("hGet4ChanSlowContM",
         "Slow control messages per GET4 channel; GET4 channel # ; Type",
         fuNbGet4*get4v1x::kuChanPerGet4*2, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5,
         4, -0.5, 3.5);
   fhGet4ChanSlowContM->GetYaxis()->SetBinLabel(1,  "0: Scaler event            ");
   fhGet4ChanSlowContM->GetYaxis()->SetBinLabel(2,  "1: Dead time counter event ");
   fhGet4ChanSlowContM->GetYaxis()->SetBinLabel(3,  "2: SPI receiver data        ");
   fhGet4ChanSlowContM->GetYaxis()->SetBinLabel(4,  "3: Start message/Hamming Er.");
   fhGet4ChanEdgesCounts = new TH2I("hGet4ChanEdgesCounts",
         "Detected edges scaler per GET4 channel; GET4 channel # ; Scaler Value [det. edges]",
         fuNbGet4*get4v1x::kuChanPerGet4*2, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5,
         128, -0.5, 127.5);
   fhGet4ChanDeadtime = new TH2I("hGet4ChanDeadtime",
         "Deadtime per GET4 channel; GET4 channel # ; Deadtime [Clock cycles]",
         fuNbGet4*get4v1x::kuChanPerGet4*2, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5,
         100, -0.5, 999.5);
   fhGet4SeuCounter = new TH2I("hGet4SeuCounter",
         "SEU count per GET4 ; GET4 channel # ; SEU [events]",
         fuNbGet4, -0.5, fuNbGet4 -0.5,
         128, -0.5, 127.5);

      // Time evolution histos
   Int_t    iNbBinsEvo    = fdEvoLengthSec / fdEvoBinSizeS;
         // FLIB
   fhMsSizeEvo = new TProfile( "hMsSizeEvo",
         "Evolution of the size of Microslices; Time [s] ; Mean size [Bytes]",
         iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
         // ROC
   fvhRocDataCntEvo.resize(fuNbRocs);
   fvhRocEpochCntEvo.resize(fuNbRocs);
   fvhRocErrorCntEvo.resize(fuNbRocs);
   fvhRocTotErrorCntEvo.resize(fuNbRocs);
   fvhRocTotErrorCntEvo.resize(fuNbRocs);
   fvhRocEpochMissmCntEvo.resize(fuNbRocs);
   fvhRocDataLossCntEvo.resize(fuNbRocs);
   fvhRocEpochLossCntEvo.resize(fuNbRocs);
   for( UInt_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
   {
      fvhRocDataCntEvo[uRoc] = new TH1I(
            Form("hRocDataCntEvo_%03u", uRoc),
            Form("Evolution of data rate for ROC #%03d; Time [s] ; Data rate [1/s]", uRoc),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhRocEpochCntEvo[uRoc] = new TH1I(
            Form("hRocEpochCntEvo_%03u", uRoc),
            Form("Evolution of epoch rate for ROC #%03d; Time [s] ; Epoch rate [1/s]", uRoc),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhRocErrorCntEvo[uRoc] = new TH1I(
            Form("hRocErrorCntEvo_%03u", uRoc),
            Form("Evolution of error rate for ROC #%03d; Time [s] ; Error rate [1/s]", uRoc),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhRocTotErrorCntEvo[uRoc] = new TH1I(
            Form("hRocTotErrorCntEvo_%03u", uRoc),
            Form("Evolution of TOT error rate for ROC #%03d; Time [s] ; TOT Error rate [1/s]", uRoc),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhRocEpochMissmCntEvo[uRoc] = new TH1I(
            Form("hRocEpochMissmCntEvo_%03u", uRoc),
            Form("Evolution of epoch missmatch rate for ROC #%03d; Time [s] ; Epoch missmatch rate [1/s]", uRoc),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhRocDataLossCntEvo[uRoc] = new TH1I(
            Form("hRocDataLossCntEvo_%03u", uRoc),
            Form("Evolution of data loss rate for ROC #%03d; Time [s] ; Data loss rate [1/s]", uRoc),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhRocEpochLossCntEvo[uRoc] = new TH1I(
            Form("hRocEpochLossCntEvo_%03u", uRoc),
            Form("Evolution of epoch loss rate for ROC #%03d; Time [s] ; Epoch loss rate [1/s]", uRoc),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
   } // for( Int_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
         // GET4
   fvhGet4DataCntEvo.resize(fuNbGet4);
   fvhGet4ErrorCntEvo.resize(fuNbGet4);
   fvhGet4EpochMissmCntEvo.resize(fuNbGet4);
   fvhGet4DataLossCntEvo.resize(fuNbGet4);
   fvhGet4EpochLossCntEvo.resize(fuNbGet4);
   fvhGet4ChDataCntEvo.resize(fuNbGet4);
   fvhGet4ChErrorCntEvo.resize(fuNbGet4);
   fvhFineTime.resize(fuNbGet4);
   for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
   {
      if( kTRUE == fvbActiveChips[uGet4] )
      {
         fvhGet4DataCntEvo[uGet4] = new TH1I(
            Form("hGet4DataCntEvo_%03u", uGet4),
            Form("Evolution of data rate for GET4 #%03d; Time [s] ; Data rate [1/s]", uGet4),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
         fvhGet4ErrorCntEvo[uGet4] = new TH1I(
            Form("hGet4ErrorCntEvo_%03u", uGet4),
            Form("Evolution of error rate for GET4 #%03d; Time [s] ; Error rate [1/s]", uGet4),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
         fvhGet4EpochMissmCntEvo[uGet4] = new TH1I(
            Form("hGet4EpochMissmCntEvo_%03u", uGet4),
            Form("Evolution of epoch missmatch rate for GET4 #%03d; Time [s] ; Error missmatch rate [1/s]", uGet4),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
         fvhGet4DataLossCntEvo[uGet4] = new TH1I(
            Form("hGet4DataLossCntEvo_%03u", uGet4),
            Form("Evolution of data loss rate for GET4 #%03d; Time [s] ; Data loss rate [1/s]", uGet4),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
         fvhGet4EpochLossCntEvo[uGet4] = new TH1I(
            Form("hGet4EpochLossCntEvo_%03u", uGet4),
            Form("Evolution of epoch loss rate for GET4 #%03d; Time [s] ; Error loss rate [1/s]", uGet4),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
         fvhGet4ChDataCntEvo[uGet4] = new TH2I(
            Form("hGet4ChDataCntEvo_%03u", uGet4),
            Form("Evolution of data rate for GET4 #%03d; Channel ; Time [s] ; Data rate [1/s]", uGet4),
            get4v1x::kuChanPerGet4, -0.5, get4v1x::kuChanPerGet4 -0.5,
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
         fvhGet4ChErrorCntEvo[uGet4] = new TH2I(
            Form("hGet4ChErrorCntEvo_%03u", uGet4),
            Form("Evolution of error rate for GET4 #%03d; Channel ; Time [s] ; Error rate [1/s]", uGet4),
            get4v1x::kuChanPerGet4, -0.5, get4v1x::kuChanPerGet4 -0.5,
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      } // if( kTRUE == fvbActiveChips[uGet4] )

      // Calibration TDC indexing not compatible with making only for active TDC
      if( kTRUE == fbEnaCalibOutput )
      {
         fvhFineTime[ uGet4 ].resize(get4v1x::kuChanPerGet4);
         for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )
         {
            TString sCalibHistoOutputName = Form("tof_get4_ft_b%03u_ch%03u",
                                                 uGet4, uChan );
            fvhFineTime[ uGet4 ][ uChan ] = new TH1I(
                  sCalibHistoOutputName,
                  Form("Fine Time distribution for channel %03d in  GET4 #%03d; FineTime [bin] ; Counts [1]",
                        uChan, uGet4),
                  get4v1x::kuFineCounterSize, -0.5, get4v1x::kuFineCounterSize - 0.5 );
         } // for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )
      } // if( kTRUE == fbEnaCalibOutput )
   } // for( Int_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
         // DETECTORS
   fvhDetDataCntEvo.resize(kiNbDet);
   fvhDetErrorCntEvo.resize(kiNbDet);
   fvhDetEpochMissmCntEvo.resize(kiNbDet);
   fvhDetDataLossCntEvo.resize(kiNbDet);
   fvhDetEpochLossCntEvo.resize(kiNbDet);
   for( Int_t iDet = 0; iDet < kiNbDet; iDet++)
   {
      fvhDetDataCntEvo[iDet] = new TH1I(
            Form("hDetDataCntEvo_%s", ksDetName[iDet].Data()),
            Form("Evolution of data rate for %s; Time [s] ; Data rate [1/s]",
                  ksDetName[iDet].Data()),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhDetErrorCntEvo[iDet] = new TH1I(
            Form("hDetErrorCntEvo_%s", ksDetName[iDet].Data()),
            Form("Evolution of error rate for %s; Time [s] ; Error rate [1/s]",
                  ksDetName[iDet].Data()),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhDetEpochMissmCntEvo[iDet] = new TH1I(
            Form("hDetEpochMissmCntEvo_%s", ksDetName[iDet].Data()),
            Form("Evolution of epoch missmatch rate for %s; Time [s] ; Error missmatch rate [1/s]",
                  ksDetName[iDet].Data()),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhDetDataLossCntEvo[iDet] = new TH1I(
            Form("hDetDataLossCntEvo_%s", ksDetName[iDet].Data()),
            Form("Evolution of data loss rate for %s; Time [s] ; Data loss rate [1/s]",
                  ksDetName[iDet].Data()),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
      fvhDetEpochLossCntEvo[iDet] = new TH1I(
            Form("hDetEpochLossCntEvo_%s", ksDetName[iDet].Data()),
            Form("Evolution of epoch loss rate for %s; Time [s] ; Error loss rate [1/s]",
                  ksDetName[iDet].Data()),
            iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );
   } // for( Int_t iDet = 0; iDet < kiNbDet; iDet++)

   fhClosedEpSizeEvo = new TProfile( "hClosedEpSizeEvo",
         "Evolution of the size of closed epochs; Time [s] ; Mean size rate [Messages]",
         iNbBinsEvo, -0.5, fdEvoLengthSec - 0.5 );

   if( kTRUE == fbPulserMode )
   {
      // Full Fee test
      UInt_t uHistoFeeIdx =   0;
      UInt_t uNbBinsDt = kuNbBinsDt + 1; // To account for extra bin due to shift by 1/2 bin of both ranges
      for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
      {
         for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
         {
            fhTimeResFee[uHistoFeeIdx] = new TH1I(
                  Form("hTimeResFee_%03u_%03u", uChanFeeA, uChanFeeB),
                  Form("Time difference for channels %03u and %03u in chosen Fee; DeltaT [ps]; Counts",
                        uChanFeeA, uChanFeeB),
                  uNbBinsDt, dMinDt, dMaxDt);
            if( kTRUE == fbPulserFeeB )
            {
               fhTimeResFeeB[uHistoFeeIdx] = new TH1I(
                     Form("hTimeResFeeB_%03u_%03u", uChanFeeA, uChanFeeB),
                     Form("Time difference for channels %03u and %03u in 2nd chosen Fee; DeltaT [ps]; Counts",
                           uChanFeeA, uChanFeeB),
                     uNbBinsDt, dMinDt, dMaxDt);
            } // if( kTRUE == fbPulserFeeB )
            uHistoFeeIdx++;
         } // for any unique pair of channel in chosen Fee

         if( kTRUE == fbPulserFeeB )
         {
            for( UInt_t uChanFeeB = 0; uChanFeeB < kuNbChanFee; uChanFeeB++)
               fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB] = new TH1I(
                     Form("hTimeResFeeAB_%03u_%03u", uChanFeeA, uChanFeeB),
                     Form("Time difference for channels %03u in 1st chosen Fee and %03u in 2nd chosen Fee; DeltaT [ps]; Counts",
                           uChanFeeA, uChanFeeB),
                     uNbBinsDt, dMinDt, dMaxDt);
         } // if( kTRUE == fbPulserFeeB )
      } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
      fhTimeRmsAllFee = new TH2D( "hTimeRmsAllFee",
            "Time difference RMS for any channels pair in chosen Fee; Ch A; Ch B; [ps]",
            kuNbChanFee - 1, -0.5, kuNbChanFee - 1.5,
            kuNbChanFee - 1,  0.5, kuNbChanFee - 0.5);
      fhTimeResAllFee = new TH2D( "hTimeResAllFee",
            "Time resolution for any channels pair in chosen Fee (for single channel); Ch A; Ch B; [ps]",
            kuNbChanFee - 1, -0.5, kuNbChanFee - 1.5,
            kuNbChanFee - 1,  0.5, kuNbChanFee - 0.5);

      if( kTRUE == fbPulserFeeB )
      {
         fhTimeRmsAllFeeB = new TH2D( "hTimeRmsAllFeeB",
               "Time difference RMS for any channels pair in 2nd chosen Fee; Ch A; Ch B; [ps]",
               kuNbChanFee - 1, -0.5, kuNbChanFee - 1.5,
               kuNbChanFee - 1,  0.5, kuNbChanFee - 0.5);
         fhTimeResAllFeeB = new TH2D( "hTimeResAllFeeB",
               "Time resolution for any channels pair in 2nd chosen Fee (for single channel); Ch A; Ch B; [ps]",
               kuNbChanFee - 1, -0.5, kuNbChanFee - 1.5,
               kuNbChanFee - 1,  0.5, kuNbChanFee - 0.5);
         fhTimeRmsAllFeeAB = new TH2D( "hTimeRmsAllFeeAB",
               "Time difference RMS for any channels pair in 1st and 2nd chosen Fee; Ch A FEE A; Ch B FEE B; [ps]",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               kuNbChanFee, -0.5, kuNbChanFee - 0.5);
         fhTimeResAllFeeAB = new TH2D( "hTimeResAllFeeAB",
               "Time resolution for any channels pair in 1st and 2nd chosen Fee (for single channel); Ch A FEE A; Ch B FEE B; [ps]",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               kuNbChanFee, -0.5, kuNbChanFee - 0.5);
      } // if( kTRUE == fbPulserFeeB )

      // Chosen channels test
      fhTimeRmsAllPairs = new TH1D( "hTimeRmsAllPairs",
            "Time difference RMS for chosen channels pairs; Pair # ; [ps]",
            kuNbChanTest - 1, -0.5, kuNbChanTest - 1.5);
      fhTimeResAllPairs = new TH1D( "hTimeResAllPairs",
            "Time resolution for chosen channels pairs (for single channel); Pair # ; [ps]",
            kuNbChanTest - 1, -0.5, kuNbChanTest - 1.5);
      fhTimeRmsAllCombi = new TH2D( "hTimeRmsAllCombi",
            "Time difference RMS for chosen channels pairs; Ch A; Ch B; [ps]",
            kuNbChanComb - 1, -0.5, kuNbChanComb - 1.5,
            kuNbChanComb - 1,  0.5, kuNbChanComb - 0.5);
      fhTimeResAllCombi = new TH2D( "hTimeResAllCombi",
            "Time resolution for chosen channels combinations (for single channel); Ch A; Ch B; [ps]",
            kuNbChanComb - 1, -0.5, kuNbChanComb - 1.5,
            kuNbChanComb - 1,  0.5, kuNbChanComb - 0.5);
      UInt_t uHistoCombiIdx = 0;
      for( UInt_t uChanA = 0; uChanA < kuNbChanTest-1; uChanA++)
      {
         fhTimeResPairs[uChanA]  = new TH1I(
               Form("hTimeResPairs_%03u_%03u", fuPulserChan[uChanA], fuPulserChan[uChanA+1]),
               Form("Time difference for selected channels %03u and %03u; DeltaT [ps]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanA+1]),
               uNbBinsDt, dMinDt, dMaxDt);
         fhTimeResPairsTot[2*uChanA]  = new TH2I(
               Form("hTimeResPairsTot_%03u_%03u_A", fuPulserChan[uChanA], fuPulserChan[uChanA+1]),
               Form("Time difference for selected channels %03u and %03u; DeltaT [ps]; TOT %03u [bin]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanA+1], fuPulserChan[uChanA] ),
               uNbBinsDt, dMinDt, dMaxDt,
               get4v1x::kuTotCounterSize, 0, get4v1x::kuTotCounterSize );
         fhTimeResPairsTot[2*uChanA+1]  = new TH2I(
               Form("hTimeResPairsTot_%03u_%03u_B", fuPulserChan[uChanA], fuPulserChan[uChanA+1]),
               Form("Time difference for selected channels %03u and %03u; DeltaT [ps]; TOT %03u [bin]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanA+1], fuPulserChan[uChanA+1] ),
               uNbBinsDt, dMinDt, dMaxDt,
               get4v1x::kuTotCounterSize, 0, get4v1x::kuTotCounterSize );
         for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
         {
            fhTimeResCombi[uHistoCombiIdx]  = new TH1I(
               Form("hTimeResCombi_%03u_%03u", fuPulserChan[uChanA], fuPulserChan[uChanB]),
               Form("Time difference for selected channels %03u and %03u; DeltaT [ps]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanB]),
               uNbBinsDt, dMinDt, dMaxDt);
            fhTimeResCombiTot[2*uHistoCombiIdx]  = new TH2I(
               Form("hTimeResCombiTot_%03u_%03u_A", fuPulserChan[uChanA], fuPulserChan[uChanB]),
               Form("Time difference for selected channels %03u and %03u; DeltaT [ps]; TOT %03u [bin]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanB], fuPulserChan[uChanA]),
               uNbBinsDt, dMinDt, dMaxDt,
               get4v1x::kuTotCounterSize, 0, get4v1x::kuTotCounterSize );
            fhTimeResCombiTot[2*uHistoCombiIdx+1]  = new TH2I(
               Form("hTimeResCombiTot_%03u_%03u_B", fuPulserChan[uChanA], fuPulserChan[uChanB]),
               Form("Time difference for selected channels %03u and %03u; DeltaT [ps]; TOT %03u [bin]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanB], fuPulserChan[uChanB]),
               uNbBinsDt, dMinDt, dMaxDt,
               get4v1x::kuTotCounterSize, 0, get4v1x::kuTotCounterSize );

            fhTimeResCombiEncA[uHistoCombiIdx]  = new TH1I(
               Form("hTimeResCombiEncA_%03u_%03u", fuPulserChan[uChanA], fuPulserChan[uChanB]),
               Form("Time difference for selected channels %03u and %03u; DeltaT [ps]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanB]),
               uNbBinsDt, dMinDt, dMaxDt);
            fhTimeResCombiEncB[uHistoCombiIdx]  = new TH1I(
               Form("hTimeResCombiEncB_%03u_%03u", fuPulserChan[uChanA], fuPulserChan[uChanB]),
               Form("Time difference for selected channels %03u and %03u; DeltaT [ps]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanB]),
               uNbBinsDt, dMinDt, dMaxDt);

            uHistoCombiIdx++;
         } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
      } // for( UInt_t uChanA = 0; uChanA < kuNbChanTest; uChanA++)

      fhTimeRmsAllCombiEncA = new TH2D( "hTimeRmsAllCombiEncA",
            "Time difference RMS for chosen channels pairs, if FT of both in Encoder A; Ch A; Ch B; [ps]",
            kuNbChanComb - 1, -0.5, kuNbChanComb - 1.5,
            kuNbChanComb - 1,  0.5, kuNbChanComb - 0.5);
      fhTimeResAllCombiEncA = new TH2D( "hTimeResAllCombiEncA",
            "Time resolution for chosen channels combinations (for single channel), if FT of both in Encoder A; Ch A; Ch B; [ps]",
            kuNbChanComb - 1, -0.5, kuNbChanComb - 1.5,
            kuNbChanComb - 1,  0.5, kuNbChanComb - 0.5);
      fhTimeRmsAllCombiEncB = new TH2D( "hTimeRmsAllCombiEncB",
            "Time difference RMS for chosen channels pairs, if FT of both in Encoder B; Ch A; Ch B; [ps]",
            kuNbChanComb - 1, -0.5, kuNbChanComb - 1.5,
            kuNbChanComb - 1,  0.5, kuNbChanComb - 0.5);
      fhTimeResAllCombiEncB= new TH2D( "hTimeResAllCombiEncB",
            "Time resolution for chosen channels combinations (for single channel), if FT of both in Encoder B; Ch A; Ch B; [ps]",
            kuNbChanComb - 1, -0.5, kuNbChanComb - 1.5,
            kuNbChanComb - 1,  0.5, kuNbChanComb - 0.5);

      fhPulserHitDistNs = new TH2D( "hPulserHitDistNs",
            "Time Interval between hits for all channels in chosen Fee; Chan # ; Hits interval [ns]",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            1000 ,  0., 1000.0);
      fhPulserHitDistUs = new TH2D( "hPulserHitDistUs",
            "Time Interval between hits for all channels in chosen Fee; Chan # ; Hits interval [us]",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            1000 ,  0., 1000.0);
      fhPulserHitDistMs = new TH2D( "hPulserHitDistMs",
            "Time Interval between hits for all channels in chosen Fee; Chan # ; Hits interval [ms]",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            1000 ,  0., 1000.0);

      fhPulserFeeDnl = new TH2D( "hPulserFeeDnl",
            "DNL for all channels in chosen FEE board; Chan # ; FT Bin; DNL [bin]",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhPulserFeeInl = new TH2D( "hPulserFeeInl",
            "INL for all channels in chosen FEE board; Chan # ; FT Bin; INL [bin]",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhPulserFeeDistCT.resize( kuNbChipFee );
      for( UInt_t uChip = 0; uChip < kuNbChipFee; uChip ++)
         fhPulserFeeDistCT[uChip] = new TH2D( Form("fhPulserFeeDistCT_chip%03u", uChip),
               Form("Coarse counter distribution for all channels in chip %03u in chosen FEE board; CT Bin; Chan # ; Counts []", uChip),
               get4v1x::kuCoarseCounterSize,  -0.5, get4v1x::kuCoarseCounterSize - 0.5,
               get4v1x::kuChanPerGet4, -0.5, get4v1x::kuChanPerGet4 - 0.5 );

      fhFtSmallDtFeeA = new TH2D( "hFtSmallDtFeeA",
            "FT of both channels when normal time difference in FEE A; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
      fhFtSmallDtFeeB = new TH2D( "hFtSmallDtFeeB",
            "FT of both channels when normal time difference in FEE B; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhFtBigDtFeeA = new TH2D( "hFtBigDtFeeA",
            "FT of both channels when big time difference in FEE A; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
      fhFtBigDtFeeB = new TH2D( "hFtBigDtFeeB",
            "FT of both channels when big time difference in FEE B; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhFtPrevSmallDtFeeA = new TH2D( "hFtPrevSmallDtFeeA",
            "FT of Previous time for both channels when normal time difference in FEE A; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
      fhFtPrevSmallDtFeeB = new TH2D( "hFtPrevSmallDtFeeB",
            "FT of Previous time for both channels when normal time difference in FEE B; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhFtPrevBigDtFeeA = new TH2D( "hFtPrevBigDtFeeA",
            "FT of Previous time for both channels when big time difference in FEE A; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
      fhFtPrevBigDtFeeB = new TH2D( "hFtPrevBigDtFeeB",
            "FT of Previous time for both channels when big time difference in FEE B; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhFullCtEpJumpFeeA.resize(kuNbChipFee);
      fhEpQualityFirstChFeeA.resize(kuNbChipFee);
      for( UInt_t uChipFeeA = 0; uChipFeeA < kuNbChipFee; uChipFeeA++)
      {
         fhFullCtEpJumpFeeA[uChipFeeA] = new TH2D( Form("hFullCtEpJumpFeeA_%03u", uChipFeeA),
               Form("Coarse time for time and tot of last hits when epoch jump, Range is %04u; Coarse Bin Time ; Full Coarse bin TOT; Counts []",
                     get4v1x::kuCoarseCounterSize),
               400 + 1 ,  get4v1x::kuCoarseCounterSize - 200 -0.5, get4v1x::kuCoarseCounterSize + 200 + 0.5,
               400 + 1 ,  get4v1x::kuCoarseCounterSize - 200 -0.5, get4v1x::kuCoarseCounterSize + 200 + 0.5 );
         fhEpQualityFirstChFeeA[uChipFeeA] = new TH2D( Form("hEpQualityFirstChFeeA_%03u", uChipFeeA),
               Form("Epoch quality as function of first channel in last pulse, chip %03u; First channel in last pulse ; Epoch quality; Counts []",
                     uChipFeeA),
               get4v1x::kuChanPerGet4,  -0.5, get4v1x::kuChanPerGet4 - 0.5,
               2 , -0.5, 1.5 );
      } // for( UInt_t uChipFeeA = 0; uChipFeeA < kuNbChipFee; uChipFeeA++)
      fhFullCtEpJumpFeeACh.resize(get4v1x::kuChanPerGet4);
      fhFullCtEpJumpFeeAChSort.resize(get4v1x::kuChanPerGet4);
      fhFullCtChOrderCh.resize(get4v1x::kuChanPerGet4);
      fhFullCtEpQualityCh.resize(get4v1x::kuChanPerGet4);
      fhFullCtEpQualityChZoom.resize(get4v1x::kuChanPerGet4);
      for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++)
      {
         fhFullCtEpJumpFeeACh[uChan] = new TH2D( Form("hFullCtEpJumpFeeACh_%03u", uChan),
               Form("Coarse time for time and tot of last hits when epoch jump, Range is %04u; Coarse Bin Time ; Full Coarse bin TOT; Counts []",
                     get4v1x::kuCoarseCounterSize),
               400 + 1 ,  get4v1x::kuCoarseCounterSize - 200 -0.5, get4v1x::kuCoarseCounterSize + 200 + 0.5,
               400 + 1 ,  get4v1x::kuCoarseCounterSize - 200 -0.5, get4v1x::kuCoarseCounterSize + 200 + 0.5 );
         fhFullCtEpJumpFeeAChSort[uChan] = new TH2D( Form("hFullCtEpJumpFeeAChSort_%03u", uChan),
               Form("Coarse time for time and tot of last hits when epoch jump, Range is %04u; Coarse Bin Time ; Full Coarse bin TOT; Counts []",
                     get4v1x::kuCoarseCounterSize),
               400 + 1 ,  get4v1x::kuCoarseCounterSize - 200 -0.5, get4v1x::kuCoarseCounterSize + 200 + 0.5,
               400 + 1 ,  get4v1x::kuCoarseCounterSize - 200 -0.5, get4v1x::kuCoarseCounterSize + 200 + 0.5 );

         fhFullCtChOrderCh[uChan] = new TH2D( Form("hFullCtChOrderCh_%03u", uChan),
               Form("Channel position VS Coarse time for time of last hits when epoch jump, normal Range is %04u, chan %03u; Coarse Bin Time ; Channel order; Counts []",
                     get4v1x::kuCoarseCounterSize, uChan),
               400 + 1 ,  get4v1x::kuCoarseCounterSize - 200 -0.5, get4v1x::kuCoarseCounterSize + 200 + 0.5,
               get4v1x::kuChanPerGet4 ,  -0.5, get4v1x::kuChanPerGet4 - 0.5 );
         fhFullCtEpQualityCh[uChan] = new TH2D( Form("hFullCtEpQualityCh_%03u", uChan),
               Form("Epoch quality VS Coarse time for time of last hits, normal Range is %04u, chan %03u; Coarse Bin Time ; Epoch Quality; Counts []",
                     get4v1x::kuCoarseCounterSize, uChan),
               (get4v1x::kuCoarseCounterSize + 24)/100 ,  -0.5, get4v1x::kuCoarseCounterSize + 24 - 0.5,
               2, -0.5, 1.5 );
         fhFullCtEpQualityChZoom[uChan] = new TH2D( Form("hFullCtEpQualityChZoom_%03u", uChan),
               Form("Epoch quality VS Coarse time for time of last hits, normal Range is %04u, chan %03u; Coarse Bin Time ; Epoch Quality; Counts []",
                     get4v1x::kuCoarseCounterSize, uChan),
//               200,  get4v1x::kuCoarseCounterSize - 200 -0.5, get4v1x::kuCoarseCounterSize - 0.5,
               get4v1x::kuCoarseCounterSize,  -0.5, get4v1x::kuCoarseCounterSize - 0.5,
               2, -0.5, 1.5 );
      }
      fvChanOrder.resize(kuNbChipFee);
      fhFullCtEpJumpFeeAChOrder = new TH2D( "hFullCtEpJumpFeeAChOrder",
            "Get4 channels time order when epoch jump found; Channel Order ; Get4 channel; Counts []",
            get4v1x::kuChanPerGet4 ,  -0.5, get4v1x::kuChanPerGet4 - 0.5,
            get4v1x::kuChanPerGet4 ,  -0.5, get4v1x::kuChanPerGet4 - 0.5);
   } // if( kTRUE == fbPulserMode )

   if( kTRUE == fbOldReadoutOk )
   {
      fhGet4ChanTotCount = new TH1I("hGet4ChanTotCount",
            "Data Messages per GET4 TOT channel; GET4 channel # ; Data Count",
            fuNbGet4*get4v1x::kuChanPerGet4, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5);

      fhPulserFeeTotDnl = new TH2D( "hPulserFeeTotDnl",
            "DNL for all TOT channels in chosen FEE board; Chan # ; FT Bin; DNL [bin]",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhPulserFeeTotInl = new TH2D( "hPulserFeeTotInl",
            "INL for all TOT channels in chosen FEE board; Chan # ; FT Bin; INL [bin]",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhPulserFeeRisCtWideBins = new TH2D( "hPulserFeeRisCtWideBins",
            "CT for all Rising edge channels in chosen FEE board; Chan # ; CT Bin; counts []",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            get4v1x::kuCoarseCounterSize/10 + 1,  -5, get4v1x::kuCoarseCounterSize + 5);
      fhPulserFeeFalCtWideBins = new TH2D( "hPulserFeeFalCtWideBins",
            "CT for all Falling edge channels in chosen FEE board; Chan # ; CT Bin; counts []",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            get4v1x::kuCoarseCounterSize/10 + 1,  -5, get4v1x::kuCoarseCounterSize + 5);

      if( kTRUE == fbEnableMissingEdgeCheck)
      {
         fvuLastTotInFtBins.resize( kuNbChanFee );
         fvuNbRisEdgeEpoch.resize( kuNbChanFee );
         fvuNbFalEdgeEpoch.resize( kuNbChanFee );
         fvuFeePrevRisEp.resize( kuNbChanFee );
         fvmFeePrevRis.resize( kuNbChanFee );
         fvuFeePrevFalEp.resize( kuNbChanFee );
         fvmFeePrevFal.resize( kuNbChanFee );
         for( UInt_t uChan = 0; uChan < kuNbChanFee; uChan ++)
         {
            fvuLastTotInFtBins[uChan] = 0;
            fvuNbRisEdgeEpoch[uChan] = 0;
            fvuNbFalEdgeEpoch[uChan] = 0;
            fvuFeePrevRisEp[uChan] = 0;
            fvuFeePrevFalEp[uChan] = 0;
         }
         fhPulserFeeFtRecoMissRis = new TH2D( "hPulserFeeFtRecoMissRis",
               "Reconstructed FT of missing rising edges in chosen FEE board; Chan # ; FT Bin; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
         fhPulserFeeFtRecoMissFal = new TH2D( "hPulserFeeFtRecoMissFal",
               "Reconstructed FT of missing rising edges in chosen FEE board; Chan # ; FT Bin; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

         fhPulserFeeExtraRisEp = new TH2D( "hPulserFeeExtraRisEp",
               "Number of extra rising edges per epoch in chosen FEE board; Chan # ; Nb Rising edge; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               10 ,  1.5, 11.5);
         fhPulserFeeExtraFalEp = new TH2D( "hPulserFeeExtraFalEp",
               "Number of extra falling edges per epoch in chosen FEE board; Chan # ; Nb Falling edge; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               10 ,  1.5, 11.5);
         fhPulserFeeExtraEdgesEp = new TH2D( "hPulserFeeExtraEdgesEp",
               "Number of extra edges per epoch in chosen FEE board; Chan (R, F)# ; Nb edges; Counts []",
               2*kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               10 ,  1.5, 11.5);

         fhPulserFeeFtExtraEdgeRisA = new TH2D( "hPulserFeeFtExtraEdgeRisA",
               "When extra edge message, FT of first rising edge message in chosen FEE board; Chan # ; FT Bin; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
         fhPulserFeeFtExtraEdgeFalA = new TH2D( "hPulserFeeFtExtraEdgeFalA",
               "When extra edge message, FT of first falling edge message in chosen FEE board; Chan # ; FT Bin; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
         fhPulserFeeFtExtraEdgeRisB = new TH2D( "hPulserFeeFtExtraEdgeRisB",
               "When extra edge message, FT of second rising edge message in chosen FEE board; Chan # ; FT Bin; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
         fhPulserFeeFtExtraEdgeFalB = new TH2D( "hPulserFeeFtExtraEdgeFalB",
               "When extra edge message, FT of second falling edge message in chosen FEE board; Chan # ; FT Bin; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

         fhPulserFeeGoodTot = new TH2D( "hPulserFeeGoodTot",
               "TOT distrib for good pulses in chosen FEE board; Chan # ; Tot [FT Bin]; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
         fhPulserFeeExtraRecoTot = new TH2D( "hPulserFeeExtraRecoTot",
               "When extra edge message, reconstructed TOT, for channels in chosen FEE board; Chan # ; TOT reco [Bin]; Counts []",
               kuNbChanFee, -0.5, kuNbChanFee - 0.5,
               get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      } // if( kTRUE == fbEnableMissingEdgeCheck)

      fhPulserFeeTotDistCT.resize( kuNbChipFee );
      for( UInt_t uChip = 0; uChip < kuNbChipFee; uChip ++)
         fhPulserFeeTotDistCT[uChip] = new TH2D( Form("fhPulserFeeTotDistCT_chip%03u", uChip),
               Form("Coarse counter distribution for all channels in chip %03u in chosen FEE board; CT Bin; Chan # ; Counts []", uChip),
               get4v1x::kuCoarseCounterSize,  -0.5, get4v1x::kuCoarseCounterSize - 0.5,
               get4v1x::kuChanPerGet4, -0.5, get4v1x::kuChanPerGet4 - 0.5 );

      fhFtTotSmallDtFeeA = new TH2D( "hFtTotSmallDtFeeA",
            "FT of Tot for both channels when normal time difference in FEE A; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
      fhFtTotSmallDtFeeB = new TH2D( "hFtTotSmallDtFeeB",
            "FT of Tot for both channels when normal time difference in FEE B; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhFtTotBigDtFeeA = new TH2D( "hFtTotBigDtFeeA",
            "FT of Tot for both channels when big time difference in FEE A; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
      fhFtTotBigDtFeeB = new TH2D( "hFtTotBigDtFeeB",
            "FT of Tot for both channels when big time difference in FEE B; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhFtPrevTotSmallDtFeeA = new TH2D( "hFtPrevTotSmallDtFeeA",
            "FT of Previous Tot for both channels when normal time difference in FEE A; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
      fhFtPrevTotSmallDtFeeB = new TH2D( "hFtPrevTotSmallDtFeeB",
            "FT of Previous Tot for both channels when normal time difference in FEE B; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhFtPrevTotBigDtFeeA = new TH2D( "hFtPrevTotBigDtFeeA",
            "FT of Previous Tot for both channels when big time difference in FEE A; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
      fhFtPrevTotBigDtFeeB = new TH2D( "hFtPrevTotBigDtFeeB",
            "FT of Previous Tot for both channels when big time difference in FEE B; FT Bin Chan 1 ; FT Bin Chan 2; Counts []",
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
   } // if( kTRUE == fbOldReadoutOk )

   // Prepare the vector storing the hit data for time diff calculation
   fvuLastHitEp.resize( fuNbGet4 * get4v1x::kuChanPerGet4);
   fvmLastHit.resize(   fuNbGet4 * get4v1x::kuChanPerGet4);
   fvuPrevHitEp.resize( fuNbGet4 * get4v1x::kuChanPerGet4);
   fvmPrevHit.resize(   fuNbGet4 * get4v1x::kuChanPerGet4);
   if( kTRUE == fbOldReadoutOk )
   {
      fvuLastOldTotEp.resize(   fuNbGet4 * get4v1x::kuChanPerGet4);
      fvmLastOldTot.resize(   fuNbGet4 * get4v1x::kuChanPerGet4);
      fvuPrevOldTotEp.resize(   fuNbGet4 * get4v1x::kuChanPerGet4);
      fvmPrevOldTot.resize(   fuNbGet4 * get4v1x::kuChanPerGet4);
   } // if( kTRUE == fbOldReadoutOk )

   // Now clear the hits
   for( UInt_t uChan = 0; uChan < fuNbGet4 * get4v1x::kuChanPerGet4; uChan++)
   {
      fvuLastHitEp[uChan] = 0;
      fvmLastHit[uChan].reset();
      fvuPrevHitEp[uChan] = 0;
      fvmPrevHit[uChan].reset();
      if( kTRUE == fbOldReadoutOk )
      {
         fvuLastOldTotEp[uChan] = 0;
         fvmLastOldTot[uChan].reset();
         fvuPrevOldTotEp[uChan] = 0;
         fvmPrevOldTot[uChan].reset();
      } // if( kTRUE == fbOldReadoutOk )
   } // for( UInt_t uChan = 0; uChan < fuNbGet4 * get4v1x::kuChanPerGet4; uChan++)

   fuNbFee = fuNbGet4 / get4v1x::kuGet4PerFee;
   LOG(info) << "Nb FEE boards: " <<fuNbFee;
   fvvhChannelsCoinc.resize(fuNbFee);
   for( UInt_t uFeeA = 0; uFeeA < fuNbFee; uFeeA++)
   {
      LOG(info) << uFeeA << " / " <<fvvhChannelsCoinc.size();
//      fvvhChannelsCoinc[uFeeA].resize( fuNbFee - uFeeA); // Pb: index start at 0  of course!
      fvvhChannelsCoinc[uFeeA].resize( fuNbFee ); // TODO: stupid but faster to code, find better way
      for( UInt_t uFeeB = uFeeA; uFeeB < fuNbFee; uFeeB++)
      {
         LOG(info) << uFeeA << " / " <<fvvhChannelsCoinc.size() << " "
                   << uFeeB << " / " <<fvvhChannelsCoinc[uFeeA].size();
         fvvhChannelsCoinc[uFeeA][uFeeB] =
            new TH2D( Form("hChannelsCoinc_%03u_%03u", uFeeA, uFeeB),
                      Form("Coincidence between FEE %3u and %3u; Ch FEE %3u ; Ch FEE %3u;",
                            uFeeA, uFeeB, uFeeA, uFeeB),
                      get4v1x::kuChanPerFee, -0.5, get4v1x::kuChanPerFee - 0.5,
                      get4v1x::kuChanPerFee, -0.5, get4v1x::kuChanPerFee - 0.5);
      }
   } // for( UInt_t uFee = 0; uFee < fuNbFee; uFee++)

   // Multiple hits (reflections, ...) detection
   if( 1 == fiMode )
   {
      fvhGet4MultipleHits.resize(fuNbGet4);
      fvhGet4DistDoubleHits.resize(fuNbGet4);
      fvhGet4DistTripleHits.resize(fuNbGet4);
      fvhGet4DistMultipleHits.resize(fuNbGet4);
      fvhGet4MultipleHitsVsTot.resize(fuNbGet4);
      fvbChanSecondHit.resize(fuNbGet4);
      fvbChanThirdHit.resize(fuNbGet4);
      fvdChanFirstHitTot.resize(fuNbGet4);
      for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
      {
         if( kTRUE == fvbActiveChips[uGet4] )
         {
            fvhGet4MultipleHits[uGet4] = new TH2I(
                  Form("hGet4MultipleHits_%03u", uGet4),
                  Form("Multiple hits for GET4 #%03d; Channel ; Close hit # [1]; Counts[1]", uGet4),
                  get4v1x::kuChanPerGet4, -0.5, get4v1x::kuChanPerGet4 -0.5,
                  4, 0.5, 4.5 );
            fvhGet4DistDoubleHits[uGet4] = new TH2I(
                  Form("hGet4DistDoubleHits_%03u", uGet4),
                  Form("Time dist between 1st and 2nd hit for GET4 #%03d; Channel ; Time dist [ps]; Counts[1]", uGet4),
                  get4v1x::kuChanPerGet4, -0.5, get4v1x::kuChanPerGet4 -0.5,
                  static_cast<Int_t>(fdMaxDtMultiHit/100.0), 0.0, fdMaxDtMultiHit );
            fvhGet4DistTripleHits[uGet4] = new TH2I(
                  Form("hGet4DistTripleHits_%03u", uGet4),
                  Form("Time dist between 2nd and 3rd hit for GET4 #%03d; Channel ; Time dist [ps]; Counts[1]", uGet4),
                  get4v1x::kuChanPerGet4, -0.5, get4v1x::kuChanPerGet4 -0.5,
                  static_cast<Int_t>(fdMaxDtMultiHit/100.0), 0.0, fdMaxDtMultiHit );
            fvhGet4DistMultipleHits[uGet4] = new TH2I(
                  Form("hGet4DistMultipleHits_%03u", uGet4),
                  Form("Time dist between hits after 3rd one for GET4 #%03d; Channel ; Time dist [ps]; Counts[1]", uGet4),
                  get4v1x::kuChanPerGet4, -0.5, get4v1x::kuChanPerGet4 -0.5,
                  static_cast<Int_t>(fdMaxDtMultiHit/100.0), 0.0, fdMaxDtMultiHit );
            fvbChanSecondHit[uGet4].resize(get4v1x::kuChanPerGet4, kFALSE);
            fvbChanThirdHit[uGet4].resize(get4v1x::kuChanPerGet4, kFALSE);

            fvhGet4MultipleHitsVsTot[uGet4] = new TH2I(
                  Form("hGet4MultipleHitsVsTot_%03u", uGet4),
                  Form("Multiple hits for GET4 #%03d; TOT [ns] ; Close hit # [1]; Counts[1]", uGet4),
                  26, 0.0, 26.0,
                  4, 0.5, 4.5 );
            fvdChanFirstHitTot[uGet4].resize(get4v1x::kuChanPerGet4, 0.0);
         } // if( kTRUE == fvbActiveChips[uGet4] )
      } // for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
   } // if( 1 == fiMode )
}
void CbmGet4FastMonitor::FillMonitorHistograms()
{
}
void CbmGet4FastMonitor::WriteMonitorHistograms()
{
   TDirectory * oldir = gDirectory;
   TFile *fHist;
   fHist = new TFile( fsHistoFilename,"RECREATE");

   LOG(info)<<" Saving monitor histograms to : "<<fsHistoFilename;

   fHist->cd();

   fhMessageTypePerRoc->Write();
   fhRocSyncTypePerRoc->Write();
   fhRocAuxTypePerRoc ->Write();
   fhSysMessTypePerRoc->Write();
   fhMessTypePerGet4  ->Write();
   fhGet4EpochFlags   ->Write();
   fhGet4EpochSyncDist->Write();
   fhGet4EpochJumps   ->Write();
   fhGet4BadEpRatioChip->Write();
   fhGet4BadEpochRatio->Write();
   fhGet4ChanDataCount->Write();
   fhGet4ChanDllStatus->Write();
   fhGet4ChanTotMap   ->Write();
   fhGet4ChanErrors   ->Write();
   fhGet4ChanTotOwErrorsProp->Write();
   fhGet4ChanTotOrErrorsProp->Write();
   fhGet4ChanTotEdErrorsProp->Write();
   fhGet4ChanSlowContM->Write();
   fhGet4ChanEdgesCounts->Write();
   fhGet4ChanDeadtime->Write();
   fhGet4SeuCounter->Write();

   fhMsSizeEvo->Write();
   for( UInt_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
   {
      // FIXME: problem with scale function: on at least 1 computer it acts as if the
      // "width" option was given!!!!!!!!
      fvhRocDataCntEvo[uRoc]     ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocEpochCntEvo[uRoc]    ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocErrorCntEvo[uRoc]    ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocTotErrorCntEvo[uRoc] ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocEpochMissmCntEvo[uRoc]->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocDataLossCntEvo[uRoc] ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocEpochLossCntEvo[uRoc]->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocDataCntEvo[uRoc]     ->Write();
      fvhRocEpochCntEvo[uRoc]    ->Write();
      fvhRocErrorCntEvo[uRoc]    ->Write();
      fvhRocTotErrorCntEvo[uRoc] ->Write();
      fvhRocEpochMissmCntEvo[uRoc]->Write();
      fvhRocDataLossCntEvo[uRoc] ->Write();
      fvhRocEpochLossCntEvo[uRoc]->Write();
   } // for( Int_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
         // GET4
   for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
      if( kTRUE == fvbActiveChips[uGet4] )
      {
         fvhGet4DataCntEvo[uGet4]     ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4ErrorCntEvo[uGet4]    ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4EpochMissmCntEvo[uGet4]->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4DataLossCntEvo[uGet4] ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4EpochLossCntEvo[uGet4]->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4ChDataCntEvo[uGet4]   ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4ChErrorCntEvo[uGet4]  ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4DataCntEvo[uGet4]     ->Write();
         fvhGet4ErrorCntEvo[uGet4]    ->Write();
         fvhGet4EpochMissmCntEvo[uGet4]->Write();
         fvhGet4DataLossCntEvo[uGet4] ->Write();
         fvhGet4EpochLossCntEvo[uGet4]->Write();
         fvhGet4ChDataCntEvo[uGet4]   ->Write();
         fvhGet4ChErrorCntEvo[uGet4]  ->Write();
      } // for( Int_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
         // DETECTORS
   for( Int_t iDet = 0; iDet < kiNbDet; iDet++)
   {
      fvhDetDataCntEvo[iDet]     ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhDetErrorCntEvo[iDet]    ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhDetEpochMissmCntEvo[iDet]->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhDetDataLossCntEvo[iDet] ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhDetEpochLossCntEvo[iDet]->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhDetDataCntEvo[iDet]     ->Write();
      fvhDetErrorCntEvo[iDet]    ->Write();
      fvhDetEpochMissmCntEvo[iDet]->Write();
      fvhDetDataLossCntEvo[iDet] ->Write();
      fvhDetEpochLossCntEvo[iDet]->Write();
   } // for( Int_t iDet = 0; iDet < kiNbDet; iDet++)
   fhClosedEpSizeEvo->Write();

   if( kTRUE == fbPulserMode )
   {
      // Reset summary histograms for safety
      fhTimeRmsAllFee->Reset();
      fhTimeResAllFee->Reset();
      if( kTRUE == fbPulserFeeB )
      {
         fhTimeRmsAllFeeB->Reset();
         fhTimeResAllFeeB->Reset();
         fhTimeRmsAllFeeAB->Reset();
         fhTimeResAllFeeAB->Reset();
      } // if( kTRUE == fbPulserFeeB )

      // First make a gauss fit to obtain the time resolution data
      // for all Fee channels pairs
      UInt_t uHistoFeeIdx = 0;
      UInt_t uHistoFeeIdxB = 0;
      TF1 *fitFunc[kuNbChanFee*(kuNbChanFee-1)/2];
      TF1 *fitFuncB[kuNbChanFee*(kuNbChanFee-1)/2];
      TF1 *fitFuncAB[kuNbChanFee*kuNbChanFee];
      for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
      {
         TString sFitRes = "";
         for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
         {
            Double_t dRes = 0.0;

            // No need to fit if not data in histo
            if( 0 == fhTimeResFee[uHistoFeeIdx]->Integral() )
            {
               LOG(debug)<<" FEE histo empty: "<<uHistoFeeIdx<<" "
                     <<uChanFeeA<<" "<<uChanFeeB;
               uHistoFeeIdx++;
               continue;
            } //  if( 0 == fhTimeResFee[uHistoFeeIdx]->Integral() )

            Double_t dFitLimit = fhTimeResFee[uHistoFeeIdx]->GetRMS();
            if( dFitLimit < get4v1x::kdBinSize )
               dFitLimit = get4v1x::kdBinSize;

            fitFunc[uHistoFeeIdx] = new TF1( Form("f_%03d_%03d",uChanFeeA,uChanFeeB), "gaus",
                  fhTimeResFee[uHistoFeeIdx]->GetMean() - 5*dFitLimit ,
                  fhTimeResFee[uHistoFeeIdx]->GetMean() + 5*dFitLimit);
            // Fix the Mean fit value around the Histogram Mean
            fitFunc[uHistoFeeIdx]->SetParameter( 0, fhTimeResFee[uHistoFeeIdx]->Integral());
            fitFunc[uHistoFeeIdx]->FixParameter( 1, fhTimeResFee[uHistoFeeIdx]->GetMean() );
            fitFunc[uHistoFeeIdx]->SetParameter( 2, 2*dFitLimit );
//            fitFunc[uHistoFeeIdx]->SetParLimits(1,
//                  fhTimeResFee[uHistoFeeIdx]->GetMean() - 3,
//                  fhTimeResFee[uHistoFeeIdx]->GetMean() + 3 );

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
//            fhTimeResFee[uHistoFeeIdx]->Fit( Form("f_%03d_%03d",uChanFeeA,uChanFeeB), "IQRM0");
            fhTimeResFee[uHistoFeeIdx]->Fit( Form("f_%03d_%03d",uChanFeeA,uChanFeeB), "QRM0");

            dRes = fitFunc[uHistoFeeIdx]->GetParameter(2);

            // If needed uncomment for debugging
            //(WARNING: this adds 1024 histos to the file!)
//            fhTimeResFee[uHistoFeeIdx]->Write();

            delete fitFunc[uHistoFeeIdx];

            fhTimeRmsAllFee->Fill(uChanFeeA, uChanFeeB, fhTimeResFee[uHistoFeeIdx]->GetRMS() );
            fhTimeResAllFee->Fill(uChanFeeA, uChanFeeB, dRes/TMath::Sqrt2() );
//            fhTimeResAllFee->Fill(uChanFeeA, uChanFeeB, dRes/TMath::Sqrt2() );
            sFitRes += Form("%4.0f (%4.0f) ", dRes, dRes/TMath::Sqrt2() );

            uHistoFeeIdx++;
         } // for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
         LOG(info)<<" FEE Fit ch "<<uChanFeeA<<" Results: "<<sFitRes;

         if( kTRUE == fbPulserFeeB )
         {
            for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
            {
               Double_t dResB = 0.0;

               // No need to fit if not data in histo
               if( 0 == fhTimeResFeeB[uHistoFeeIdxB]->Integral() )
               {
                  LOG(debug)<<" FEE B histo empty: "<<uHistoFeeIdxB<<" "
                        <<uChanFeeA<<" "<<uChanFeeB;
                  uHistoFeeIdxB++;
                  continue;
               } //  if( 0 == fhTimeResFeeB[uHistoFeeIdx]->Integral() )

               Double_t dFitLimit = fhTimeResFeeB[uHistoFeeIdxB]->GetRMS();
               if( dFitLimit < get4v1x::kdBinSize )
                  dFitLimit = get4v1x::kdBinSize;

               fitFuncB[uHistoFeeIdxB] = new TF1( Form("fB_%03d_%03d",uChanFeeA,uChanFeeB), "gaus",
                     fhTimeResFeeB[uHistoFeeIdxB]->GetMean() - 5*dFitLimit ,
                     fhTimeResFeeB[uHistoFeeIdxB]->GetMean() + 5*dFitLimit);
               // Fix the Mean fit value around the Histogram Mean
               fitFuncB[uHistoFeeIdxB]->SetParameter( 0, fhTimeResFeeB[uHistoFeeIdxB]->Integral() );
               fitFuncB[uHistoFeeIdxB]->FixParameter( 1, fhTimeResFeeB[uHistoFeeIdxB]->GetMean() );
               fitFuncB[uHistoFeeIdxB]->SetParameter( 2, 2*dFitLimit );
//               fitFuncB[uHistoFeeIdxB]->SetParLimits(1,
//                     fhTimeResFeeB[uHistoFeeIdxB]->GetMean() - 3,
//                     fhTimeResFeeB[uHistoFeeIdxB]->GetMean() + 3 );


               // Using integral instead of bin center seems to lead to unrealistic values => no "I"
   //            fhTimeResFee[uHistoFeeIdx]->Fit( Form("f_%03d_%03d",uChanFeeA,uChanFeeB), "IQRM0");
               fhTimeResFeeB[uHistoFeeIdxB]->Fit( Form("fB_%03d_%03d",uChanFeeA,uChanFeeB), "QRM0B");

               dResB = fitFuncB[uHistoFeeIdxB]->GetParameter(2);

               // If needed uncomment for debugging
               //(WARNING: this adds 1024 histos to the file!)
//               fhTimeResFeeB[uHistoFeeIdxB]->Write();

               delete fitFuncB[uHistoFeeIdxB];

               fhTimeRmsAllFeeB->Fill(uChanFeeA, uChanFeeB, fhTimeResFeeB[uHistoFeeIdxB]->GetRMS() );
               fhTimeResAllFeeB->Fill(uChanFeeA, uChanFeeB, dResB/TMath::Sqrt2() );

               uHistoFeeIdxB++;
            } // for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)

            for( UInt_t uChanFeeB = 0; uChanFeeB < kuNbChanFee; uChanFeeB++)
            {
               Double_t dResAB = 0.0;

               // No need to fit if not data in histo
               if( 0 == fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->Integral() )
               {
                  LOG(debug)<<" FEE AB histo empty: "
                        <<uChanFeeA<<" "<<uChanFeeB;
                  continue;
               } //  if( 0 == fhTimeResFeeAB[uHistoFeeIdx]->Integral() )

               Double_t dFitLimit = fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->GetRMS();
               if( dFitLimit < get4v1x::kdBinSize )
                  dFitLimit = get4v1x::kdBinSize;

               fitFuncAB[uChanFeeA*kuNbChanFee + uChanFeeB] = new TF1( Form("fAB_%03d_%03d",uChanFeeA,uChanFeeB), "gaus",
                     fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->GetMean() - 5*dFitLimit ,
                     fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->GetMean() + 5*dFitLimit );
               // Fix the Mean fit value around the Histogram Mean
               fitFuncAB[uChanFeeA*kuNbChanFee + uChanFeeB]->SetParameter( 0, fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->Integral() );
               fitFuncAB[uChanFeeA*kuNbChanFee + uChanFeeB]->FixParameter( 1, fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->GetMean() );
               fitFuncAB[uChanFeeA*kuNbChanFee + uChanFeeB]->SetParameter( 2, 2*dFitLimit );
//               fitFuncAB[uChanFeeA*kuNbChanFee + uChanFeeB]->SetParLimits(1,
//                     fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->GetMean() - 3,
//                     fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->GetMean() + 3 );

               // Using integral instead of bin center seems to lead to unrealistic values => no "I"
   //            fhTimeResFee[uHistoFeeIdx]->Fit( Form("f_%03d_%03d",uChanFeeA,uChanFeeB), "IQRM0");
               fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->Fit( Form("fAB_%03d_%03d",uChanFeeA,uChanFeeB), "QRM0B");

               dResAB = fitFuncAB[uChanFeeA*kuNbChanFee + uChanFeeB]->GetParameter(2);

               // If needed uncomment for debugging
               //(WARNING: this adds 1024 histos to the file!)
//               fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->Write();

               delete fitFuncAB[uChanFeeA*kuNbChanFee + uChanFeeB];

               fhTimeRmsAllFeeAB->Fill(uChanFeeA, uChanFeeB, fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->GetRMS() );
               fhTimeResAllFeeAB->Fill(uChanFeeA, uChanFeeB, dResAB/TMath::Sqrt2() );
            } // for( UInt_t uChanFeeB = 0; uChanFeeB < kuNbChanFee; uChanFeeB++)
         } // if( kTRUE == fbPulserFeeB )
      } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)

      // Then write the Fee summary histo
      fhTimeRmsAllFee->Write();
      fhTimeResAllFee->Write();
      if( kTRUE == fbPulserFeeB )
      {
         fhTimeRmsAllFeeB->Write();
         fhTimeResAllFeeB->Write();
         fhTimeRmsAllFeeAB->Write();
         fhTimeResAllFeeAB->Write();
      } // if( kTRUE == fbPulserFeeB )

      // Reset summary histograms for safety
      fhTimeRmsAllPairs->Reset();
      fhTimeResAllPairs->Reset();
      fhTimeRmsAllCombi->Reset();
      fhTimeResAllCombi->Reset();
      fhTimeRmsAllCombiEncA->Reset();
      fhTimeResAllCombiEncA->Reset();
      fhTimeRmsAllCombiEncB->Reset();
      fhTimeResAllCombiEncB->Reset();

      // Write the histos for the test on chosen channel pairs
      UInt_t uHistoCombiIdx = 0;
      UInt_t uHistoCombiIdxEncA = 0;
      UInt_t uHistoCombiIdxEncB = 0;
      TF1 *fitFuncPairs[kuNbChanTest - 1];
      TF1 *fitFuncCombi[kuNbChanComb*(kuNbChanComb-1)/2];
      TF1 *fitFuncCombiEncA[kuNbChanComb*(kuNbChanComb-1)/2];
      TF1 *fitFuncCombiEncB[kuNbChanComb*(kuNbChanComb-1)/2];
      TString sFitCombRes = "\n";
      for( UInt_t uChanA = 0; uChanA < kuNbChanTest-1; uChanA++)
      {
         Double_t dSigPair = 0.0;
         Double_t dResPair = 0.0;
         Double_t dFitLimit = 0.0;
         // No need to fit if not data in histo
         if( 0 == fhTimeResPairs[uChanA]->Integral() )
         {

            LOG(debug)<<" Pair histo empty: "<<uHistoFeeIdx<<" "
                  <<uChanA<<" "<<uChanA+1;
         } //  if( 0 == fhTimeResFee[uHistoFeeIdx]->Integral() )
         else
         {
            dFitLimit = fhTimeResPairs[uChanA]->GetRMS();
            if( dFitLimit < get4v1x::kdBinSize )
               dFitLimit = get4v1x::kdBinSize;

            fitFuncPairs[uChanA] = new TF1( Form("fPair_%03d_%03d",uChanA, uChanA+1), "gaus",
                  fhTimeResPairs[uChanA]->GetMean() - 5*dFitLimit ,
                  fhTimeResPairs[uChanA]->GetMean() + 5*dFitLimit);
            // Fix the Mean fit value around the Histogram Mean
            fitFuncPairs[uChanA]->SetParameter( 0, fhTimeResPairs[uChanA]->Integral());
            fitFuncPairs[uChanA]->FixParameter( 1, fhTimeResPairs[uChanA]->GetMean() );
            fitFuncPairs[uChanA]->SetParameter( 2, 2*dFitLimit );
   //         fitFuncPairs[uChanA]->SetParLimits(1,
   //               fhTimeResPairs[uChanA]->GetMean() - 3,
   //               fhTimeResPairs[uChanA]->GetMean() + 3 );

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
   //         fhTimeResPairs[uChanA]->Fit( Form("fPair_%03d_%03d",uChanA,uChanA+1), "IQRM0");
            fhTimeResPairs[uChanA]->Fit( Form("fPair_%03d_%03d",uChanA,uChanA+1), "QRM0B");
            dSigPair = fitFuncPairs[uChanA]->GetParameter(2);
            dResPair = dSigPair/TMath::Sqrt2();

            fhTimeResPairs[uChanA]->Write();
            fhTimeResPairsTot[(2*uChanA)]->Write();
            fhTimeResPairsTot[(2*uChanA)+1]->Write();

            delete fitFuncPairs[uChanA];

            fhTimeRmsAllPairs->Fill(uChanA, fhTimeResPairs[uChanA]->GetRMS() );
            fhTimeResAllPairs->Fill(uChanA, dResPair );
            LOG(info)<<" Pair Fit ch "<<uChanA<<" ch "<<uChanA+1<<" Results: "
                     <<Form("%4.0f (%4.0f) ", dSigPair, dResPair );
         } // els of if( 0 == fhTimeResPairs[uChanA]->Integral() )

         for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
         {
            Double_t dSigCombi = 0.0;
            Double_t dResCombi = 0.0;
            // No need to fit if not data in histo
            if( 0 == fhTimeResCombi[uHistoCombiIdx]->Integral() )
            {

               LOG(debug)<<" Combi histo empty: "<<uHistoCombiIdx<<" "
                     <<uChanA<<" "<<uChanA+1;
               uHistoCombiIdx++;
               continue;
            } //  if( 0 == fhTimeResFee[uHistoFeeIdx]->Integral() )

            dFitLimit = fhTimeResCombi[uHistoCombiIdx]->GetRMS();
            if( dFitLimit < get4v1x::kdBinSize )
               dFitLimit = get4v1x::kdBinSize;

            fitFuncCombi[uHistoCombiIdx] = new TF1( Form("fCombi_%03d_%03d",uChanA, uChanB), "gaus",
                  fhTimeResCombi[uHistoCombiIdx]->GetMean() - 5*dFitLimit ,
                  fhTimeResCombi[uHistoCombiIdx]->GetMean() + 5*dFitLimit);
            // Fix the Mean fit value around the Histogram Mean
            fitFuncCombi[uHistoCombiIdx]->SetParameter( 0, fhTimeResCombi[uHistoCombiIdx]->Integral());
            fitFuncCombi[uHistoCombiIdx]->FixParameter( 1, fhTimeResCombi[uHistoCombiIdx]->GetMean() );
            fitFuncCombi[uHistoCombiIdx]->SetParameter( 2, 2*dFitLimit );
//            fitFuncCombi[uHistoCombiIdx]->SetParLimits(1,
//                  fhTimeResCombi[uHistoCombiIdx]->GetMean() - 3,
//                  fhTimeResCombi[uHistoCombiIdx]->GetMean() + 3 );

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
//            fhTimeResCombi[uHistoCombiIdx]->Fit( Form("fCombi_%03d_%03d",uChanA,uChanB), "IQRM0");
            fhTimeResCombi[uHistoCombiIdx]->Fit( Form("fCombi_%03d_%03d",uChanA,uChanB), "QRM0B");
            dSigCombi = fitFuncCombi[uHistoCombiIdx]->GetParameter(2);
            dResCombi = dSigCombi/TMath::Sqrt2();

            fhTimeResCombi[uHistoCombiIdx]->Write();
            fhTimeResCombiTot[2*uHistoCombiIdx]->Write();
            fhTimeResCombiTot[2*uHistoCombiIdx+1]->Write();

            delete fitFuncCombi[uHistoCombiIdx];

            fhTimeRmsAllCombi->Fill(uChanA, uChanB, fhTimeResCombi[uHistoCombiIdx]->GetRMS() );
            fhTimeResAllCombi->Fill(uChanA, uChanB, dResCombi );

            sFitCombRes += Form("Pair Fit ch %d ch %d Results:  %4.0f (%4.0f)\n",
                  uChanA, uChanB, dSigCombi, dResCombi );

            uHistoCombiIdx++;
         } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)

         for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
         {
            Double_t dSigCombi = 0.0;
            Double_t dResCombi = 0.0;
            // Same for signals in FT encoder A
            dSigCombi = 0.0;
            dResCombi = 0.0;
            // No need to fit if not data in histo
            if( 0 < fhTimeResCombiEncA[uHistoCombiIdxEncA]->Integral() )
            {
               dFitLimit = fhTimeResCombiEncA[uHistoCombiIdxEncA]->GetRMS();
               if( dFitLimit < get4v1x::kdBinSize )
                  dFitLimit = get4v1x::kdBinSize;

               fitFuncCombiEncA[uHistoCombiIdxEncA] = new TF1( Form("fCombiEncA_%03d_%03d",uChanA, uChanB), "gaus",
                     fhTimeResCombiEncA[uHistoCombiIdxEncA]->GetMean() - 5*dFitLimit ,
                     fhTimeResCombiEncA[uHistoCombiIdxEncA]->GetMean() + 5*dFitLimit);
               // Fix the Mean fit value around the Histogram Mean
               fitFuncCombiEncA[uHistoCombiIdxEncA]->SetParameter( 0, fhTimeResCombiEncA[uHistoCombiIdxEncA]->Integral());
               fitFuncCombiEncA[uHistoCombiIdxEncA]->FixParameter( 1, fhTimeResCombiEncA[uHistoCombiIdxEncA]->GetMean() );
               fitFuncCombiEncA[uHistoCombiIdxEncA]->SetParameter( 2, 2*dFitLimit );

               fhTimeResCombiEncA[uHistoCombiIdxEncA]->Fit( Form("fCombiEncA_%03d_%03d",uChanA,uChanB), "QRM0B");
               dSigCombi = fitFuncCombiEncA[uHistoCombiIdxEncA]->GetParameter(2);
               dResCombi = dSigCombi/TMath::Sqrt2();

               fhTimeResCombiEncA[uHistoCombiIdxEncA]->Write();

               delete fitFuncCombiEncA[uHistoCombiIdxEncA];

               fhTimeRmsAllCombiEncA->Fill(uChanA, uChanB, fhTimeResCombiEncA[uHistoCombiIdxEncA]->GetRMS() );
               fhTimeResAllCombiEncA->Fill(uChanA, uChanB, dResCombi );
            } //  if( 0 != fhTimeResFee[uHistoFeeIdx]->Integral() )
            uHistoCombiIdxEncA++;

            // Same for signals in FT encoder B
            dSigCombi = 0.0;
            dResCombi = 0.0;
            // No need to fit if not data in histo
            if( 0 < fhTimeResCombiEncB[uHistoCombiIdxEncB]->Integral() )
            {
               dFitLimit = fhTimeResCombiEncB[uHistoCombiIdxEncB]->GetRMS();
               if( dFitLimit < get4v1x::kdBinSize )
                  dFitLimit = get4v1x::kdBinSize;

               fitFuncCombiEncB[uHistoCombiIdxEncB] = new TF1( Form("fCombiEncB_%03d_%03d",uChanA, uChanB), "gaus",
                     fhTimeResCombiEncB[uHistoCombiIdxEncB]->GetMean() - 5*dFitLimit ,
                     fhTimeResCombiEncB[uHistoCombiIdxEncB]->GetMean() + 5*dFitLimit);
               // Fix the Mean fit value around the Histogram Mean
               fitFuncCombiEncB[uHistoCombiIdxEncB]->SetParameter( 0, fhTimeResCombiEncB[uHistoCombiIdxEncB]->Integral());
               fitFuncCombiEncB[uHistoCombiIdxEncB]->FixParameter( 1, fhTimeResCombiEncB[uHistoCombiIdxEncB]->GetMean() );
               fitFuncCombiEncB[uHistoCombiIdxEncB]->SetParameter( 2, 2*dFitLimit );

               fhTimeResCombiEncB[uHistoCombiIdxEncB]->Fit( Form("fCombiEncB_%03d_%03d",uChanA,uChanB), "QRM0B");
               dSigCombi = fitFuncCombiEncB[uHistoCombiIdxEncB]->GetParameter(2);
               dResCombi = dSigCombi/TMath::Sqrt2();

               fhTimeResCombiEncB[uHistoCombiIdxEncB]->Write();

               delete fitFuncCombiEncB[uHistoCombiIdxEncB];

               fhTimeRmsAllCombiEncB->Fill(uChanA, uChanB, fhTimeResCombiEncB[uHistoCombiIdxEncB]->GetRMS() );
               fhTimeResAllCombiEncB->Fill(uChanA, uChanB, dResCombi );
            } //  if( 0 != fhTimeResFee[uHistoFeeIdx]->Integral() )

            uHistoCombiIdxEncB++;
         } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
      } // for( UInt_t uChanA = 0; uChanA < kuNbChanTest; uChanA++)
      LOG(info)<<sFitCombRes;
      fhTimeRmsAllPairs->Write();
      fhTimeResAllPairs->Write();
      fhTimeRmsAllCombi->Write();
      fhTimeResAllCombi->Write();
      fhTimeRmsAllCombiEncA->Write();
      fhTimeResAllCombiEncA->Write();
      fhTimeRmsAllCombiEncB->Write();
      fhTimeResAllCombiEncB->Write();

      // Write the hists interval histos
      fhPulserHitDistNs->Write();
      fhPulserHitDistUs->Write();
      fhPulserHitDistMs->Write();

      // Write the raw count per FT bin histograms to file
      TH2 * hFtDistribFeeRis = (TH2*)(fhPulserFeeDnl->Clone("hFtDistribFeeRis"));
      hFtDistribFeeRis->Write();
      delete hFtDistribFeeRis;

      // Compute the DNL from the bins occupancy
      for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
      {
         Double_t dNbCountsTotal = fhPulserFeeDnl->Integral( uChanFeeA+1, uChanFeeA+1, 1, get4v1x::kuFineTime+1);
         Double_t dNbCountsBinMean = dNbCountsTotal/(get4v1x::kuFineTime+1);
         for( UInt_t uBin = 1; uBin <= get4v1x::kuFineTime+1; uBin ++)
            fhPulserFeeDnl->SetBinContent( uChanFeeA+1, uBin,
                            fhPulserFeeDnl->GetBinContent(uChanFeeA+1, uBin)/dNbCountsBinMean );
      } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
      fhPulserFeeDnl->Write();
      // COmpute the INL from the DNL
      for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
      {
         Double_t dInl = 0.0;
         for( UInt_t uBin = 1; uBin <= get4v1x::kuFineTime+1; uBin ++)
         {
            dInl += 0.5 * fhPulserFeeDnl->GetBinContent(uChanFeeA+1, uBin);
            fhPulserFeeInl->SetBinContent( uChanFeeA+1, uBin, dInl - (uBin-0.5) );
            dInl += 0.5 * fhPulserFeeDnl->GetBinContent(uChanFeeA+1, uBin);
         } // for( UInt_t uBin = 1; uBin <= get4v1x::kuFineTime+1; uBin ++)
      } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
      fhPulserFeeInl->Write();
      for( UInt_t uChip = 0; uChip < kuNbChipFee; uChip ++)
         fhPulserFeeDistCT[uChip]->Write();

      fhFtSmallDtFeeA->Write();
      fhFtSmallDtFeeB->Write();

      fhFtBigDtFeeA->Write();
      fhFtBigDtFeeB->Write();

      fhFtPrevSmallDtFeeA->Write();
      fhFtPrevSmallDtFeeB->Write();

      fhFtPrevBigDtFeeA->Write();
      fhFtPrevBigDtFeeB->Write();

      for( UInt_t uChipFeeA = 0; uChipFeeA < kuNbChipFee; uChipFeeA++)
      {
         fhFullCtEpJumpFeeA[uChipFeeA]->Write();
         fhEpQualityFirstChFeeA[uChipFeeA]->Write();
      }
      for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++)
      {
         fhFullCtEpJumpFeeACh[uChan]->Write();
         fhFullCtEpJumpFeeAChSort[uChan]->Write();
         fhFullCtChOrderCh[uChan]->Write();
         fhFullCtEpQualityCh[uChan]->Write();
         fhFullCtEpQualityChZoom[uChan]->Write();
      }
      fhFullCtEpJumpFeeAChOrder->Write();

      if( kTRUE == fbOldReadoutOk )
      {
         fhGet4ChanTotCount->Write();

         // Write the raw count per FT bin histograms to file
         TH2 * hFtDistribFeeFal = (TH2*)(fhPulserFeeTotDnl->Clone("hFtDistribFeeFal"));
         hFtDistribFeeFal->Write();
         delete hFtDistribFeeFal;

         // Write the raw count per CT bin histograms to file
         fhPulserFeeRisCtWideBins->Write();
         fhPulserFeeFalCtWideBins->Write();

         // Write the missing message counts per FT bin histograms to file
         if( kTRUE == fbEnableMissingEdgeCheck)
         {
            fhPulserFeeFtRecoMissRis->Write();
            fhPulserFeeFtRecoMissFal->Write();
            fhPulserFeeExtraRisEp->Write();
            fhPulserFeeExtraFalEp->Write();
            fhPulserFeeExtraEdgesEp->Write();

            fhPulserFeeFtExtraEdgeRisA->Write();
            fhPulserFeeFtExtraEdgeFalA->Write();
            fhPulserFeeFtExtraEdgeRisB->Write();
            fhPulserFeeFtExtraEdgeFalB->Write();

            fhPulserFeeGoodTot->Write();
            fhPulserFeeExtraRecoTot->Write();
         } // if( kTRUE == fbEnableMissingEdgeCheck)

         // Compute the DNL from the bins occupancy
         for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
         {
            Double_t dNbCountsTotal = fhPulserFeeTotDnl->Integral( uChanFeeA+1, uChanFeeA+1, 1, get4v1x::kuFineTime+1);
            Double_t dNbCountsBinMean = dNbCountsTotal/(get4v1x::kuFineTime+1);
            for( UInt_t uBin = 1; uBin <= get4v1x::kuFineTime+1; uBin ++)
               fhPulserFeeTotDnl->SetBinContent( uChanFeeA+1, uBin,
                               fhPulserFeeTotDnl->GetBinContent(uChanFeeA+1, uBin)/dNbCountsBinMean );
         } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
         fhPulserFeeTotDnl->Write();
         // COmpute the INL from the DNL
         for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
         {
            Double_t dInl = 0.0;
            for( UInt_t uBin = 1; uBin <= get4v1x::kuFineTime+1; uBin ++)
            {
               dInl += 0.5 * fhPulserFeeTotDnl->GetBinContent(uChanFeeA+1, uBin);
               fhPulserFeeTotInl->SetBinContent( uChanFeeA+1, uBin, dInl - (uBin-0.5) );
               dInl += 0.5 * fhPulserFeeTotDnl->GetBinContent(uChanFeeA+1, uBin);
            } // for( UInt_t uBin = 1; uBin <= get4v1x::kuFineTime+1; uBin ++)
         } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
         fhPulserFeeTotInl->Write();
         for( UInt_t uChip = 0; uChip < kuNbChipFee; uChip ++)
            fhPulserFeeTotDistCT[uChip]->Write();

         fhFtTotSmallDtFeeA->Write();
         fhFtTotSmallDtFeeB->Write();

         fhFtTotBigDtFeeA->Write();
         fhFtTotBigDtFeeB->Write();

         fhFtPrevTotSmallDtFeeA->Write();
         fhFtPrevTotSmallDtFeeB->Write();

         fhFtPrevTotBigDtFeeA->Write();
         fhFtPrevTotBigDtFeeB->Write();
      } // if( kTRUE == fbOldReadoutOk )
   } // if( kTRUE == fbPulserMode )

   for( UInt_t uFeeA = 0; uFeeA < fuNbFee; uFeeA++)
      for( UInt_t uFeeB = uFeeA; uFeeB < fuNbFee; uFeeB++)
         fvvhChannelsCoinc[uFeeA][uFeeB]->Write();

   // Multiple hits (reflections, ...) detection
   if( 1 == fiMode )
   {
      for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
      {
         if( kTRUE == fvbActiveChips[uGet4] )
         {
            fvhGet4MultipleHits[uGet4]    ->Write();
            fvhGet4DistDoubleHits[uGet4]  ->Write();
            fvhGet4DistTripleHits[uGet4]  ->Write();
            fvhGet4DistMultipleHits[uGet4]->Write();
            fvhGet4MultipleHitsVsTot[uGet4]->Write();
         } // if( kTRUE == fvbActiveChips[uGet4] )
      } // for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
   } // if( 1 == fiMode )

   gDirectory->cd( oldir->GetPath() );
   fHist->Close();
}
void CbmGet4FastMonitor::DeleteMonitorHistograms()
{

}
void CbmGet4FastMonitor::MonitorMessage_epoch(  get4v1x::Message mess, uint16_t EqID)
{
   // ROC epoch message: 250 MHz clock for now
   // TODO: check compatibility when ROC clock changed to 200 MHz
   uint8_t  cRocId    = mess.getRocNumber();
   LOG(debug3)<<"CbmGet4FastMonitor::MonitorMessage_epoch => ROC Epoch: EqId "
               <<EqID<<" roc "
               <<cRocId;

   // Epoch counter overflow book keeping
   if( (get4v1x::kulMainEpochCycleSz - 2) < fvuCurrEpoch[cRocId] &&
       mess.getEpochNumber() < 2  )
      fvuCurrEpochCycle[cRocId]++;

   fvuCurrEpoch[cRocId] = mess.getEpochNumber();
}
void CbmGet4FastMonitor::MonitorMessage_sync(   get4v1x::Message mess, uint16_t EqID)
{
   // ROC sync message: TS in 250 MHz clock for now
   // TODO: check compatibility when ROC clock changed to 200 MHz
   uint8_t  cRocId    = mess.getRocNumber();
   fhRocSyncTypePerRoc->Fill( cRocId, mess.getSyncChNum() );
   LOG(debug3)<<"CbmGet4FastMonitor::MonitorMessage_sync => ROC Sync: EqId "
               <<EqID<<" roc "
               <<cRocId;
}
void CbmGet4FastMonitor::MonitorMessage_aux(    get4v1x::Message mess, uint16_t EqID)
{
   // ROC aux message: TS in 250 MHz clock for now
   // TODO: check compatibility when ROC clock changed to 200 MHz
   uint8_t  cRocId    = mess.getRocNumber();
   fhRocAuxTypePerRoc->Fill( cRocId, mess.getAuxChNum() );
   LOG(debug3)<<"CbmGet4FastMonitor::MonitorMessage_aux => ROC Aux: EqId "
               <<EqID<<" roc "
               <<cRocId;
}
void CbmGet4FastMonitor::MonitorMessage_epoch2( get4v1x::Message mess, uint16_t EqID)
{
   // GET4 v1.x epoch message (24b only for now)
   // TODO: check compatibility when 32b format without hack ready
   uint8_t  cRocId    = mess.getRocNumber();
   uint8_t  cChipId   = mess.getEpoch2ChipNumber();
   uint32_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;

   LOG(debug3)<<"CbmGet4FastMonitor::MonitorMessage_epoch2 => GET4 Epoch2: EqId "
               <<EqID<<" roc "
               <<cRocId<<" chip "
               <<cChipId;

   // Reject data of chips above defined limit to avoid overflow/seg fault
   if( fuNbGet4 <= uChipFullId )
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmGet4FastMonitor::MonitorMessage_epoch2 => Ignored message with ChipId above limit!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId
                  <<" Limit: " <<fuNbGet4;
      return;
   } // if( fuNbGet4 <= cChipId + kuMaxGet4PerRoc*mess.getRocNumber() )

   // Reject data of Inactive chips
   if( kFALSE == fvbActiveChips[uChipFullId])
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmGet4FastMonitor::MonitorMessage_epoch2 => Ignored message with Inactive Chip!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId ;
      return;
   } // if( kFALSE == fvbActiveChips[uChipFullId])

   // As long as the 1st epoch with sync flag after SYNC DLM is found,
   // consider data as not good
   if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )
   {
      if( 1 == mess.getEpoch2Sync() )
      {
         fvbGet4WaitFirstSync[uChipFullId] = kFALSE;

         LOG(info) << "First SYNC Epoch after DLM in chip "
                   <<Form("%3u: ", uChipFullId)
                   <<Form(" EP #%6u", mess.getEpoch2Number() )
                   <<Form(" TS #%12llu", fulTsNb)
                   <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                   <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                   <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
      } // if( 1 == mess.getEpoch2Sync() )
         else return;
   } // if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )

   // Count epoch messages per chip
   fhMessTypePerGet4->Fill( uChipFullId, get4v1x::GET4_32B_EPOCH);

   if( fvuCurrEpoch2[uChipFullId] +1 != mess.getEpoch2Number() &&
         0 != fvuCurrEpoch2[uChipFullId] )
   {
      Int_t iEpJump = mess.getEpoch2Number();
      iEpJump      -= fvuCurrEpoch2[uChipFullId];

      // If debug mode, print the message
      if( kTRUE == fbDebug )
         LOG(info) << "Epoch nb jump in chip "
                 <<Form("%3u: ", uChipFullId)
                 << Form(" %3d (%6u -> %6u)", iEpJump, fvuCurrEpoch2[uChipFullId],
                                         mess.getEpoch2Number() )
                 <<Form(" TS #%12llu", fulTsNb)
                 <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                 <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                 <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
      else LOG(debug) << "Epoch nb jump in chip "
            <<Form("%3u: ", uChipFullId)
            << Form(" %3d (%6u -> %6u)", iEpJump, fvuCurrEpoch2[uChipFullId],
                                    mess.getEpoch2Number() )
            <<Form(" TS #%12llu", fulTsNb)
            <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
            <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
            <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);

      fhGet4EpochJumps->Fill(uChipFullId, iEpJump);
      fhGet4BadEpochRatio->Fill(0);
      fhGet4BadEpRatioChip->Fill( uChipFullId, 0);

      if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk && fuPulserFee == (uChipFullId/kuNbChipFee) )
      {
         UInt_t uChipFeeA = uChipFullId%kuNbChipFee;
         for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )
            if( 0ull != fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getData() )
         {
            fhFullCtEpJumpFeeA[uChipFeeA]->Fill(
               fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4CoarseTs(),
               fvmLastOldTot[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4CoarseTs() );
            fhFullCtEpJumpFeeACh[uChan]->Fill(
                  fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4CoarseTs(),
                  fvmLastOldTot[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4CoarseTs() );

            fhFullCtEpQualityCh[uChan]->Fill(
               fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4CoarseTs(), 0);
            fhFullCtEpQualityChZoom[uChan]->Fill(
               fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4CoarseTs(), 0);

            if( get4v1x::kuChanPerGet4 == fvChanOrder[uChipFeeA].size() )
            {
               UInt_t uChOrder;
               for( uChOrder = 0; uChOrder < get4v1x::kuChanPerGet4; uChOrder++ )
                  if( uChan == fvChanOrder[uChipFeeA][uChOrder] )
                     break;
               fhFullCtChOrderCh[uChan]->Fill(
                     fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4CoarseTs(), uChOrder);
            } // if( get4v1x::kuChanPerGet4 == fvChanOrder[uChipFeeA].size() )
         } // for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )

         if( get4v1x::kuChanPerGet4 == fvChanOrder[uChipFeeA].size() )
            for( UInt_t uChOrder = 0; uChOrder < fvChanOrder[uChipFeeA].size(); uChOrder++ )
            {
               UInt_t uChanOrdered =  fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ fvChanOrder[uChipFeeA][uChOrder];
               fhFullCtEpJumpFeeAChSort[uChOrder]->Fill(
                     fvmLastHit[ uChanOrdered ].getGet4CoarseTs(),
                     fvmLastOldTot[ uChanOrdered ].getGet4CoarseTs() );
               fhFullCtEpJumpFeeAChOrder->Fill( uChOrder, fvChanOrder[uChipFeeA][uChOrder]);
            } // for( UInt_t uChOrder = 0; uChOrder < fvChanOrder[uChipFeeA].size(); uChOrder++ )

         if( 0 < fvChanOrder[uChipFeeA].size() )
            fhEpQualityFirstChFeeA[uChipFeeA]->Fill( fvChanOrder[uChipFeeA][0], 0 );
      } // if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk && fuPulserFee == (uChipFullId/kuNbChipFee) )
   } // if( fvuCurrEpoch2[uChipFullId] +1 != mess.getGet4V10R32EpochNumber())
      else
      {
         fhGet4BadEpochRatio->Fill( 1 );
         fhGet4BadEpRatioChip->Fill( uChipFullId, 1);
         if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk && fuPulserFee == (uChipFullId/kuNbChipFee) )
         {
            UInt_t uChipFeeA = uChipFullId%kuNbChipFee;

            for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )
               if( 0ull != fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getData() )
            {
               fhFullCtEpQualityCh[uChan]->Fill(
                  fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4CoarseTs(), 1);
               fhFullCtEpQualityChZoom[uChan]->Fill(
                  fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4CoarseTs(), 1);
            } // for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )

            if( 0 < fvChanOrder[uChipFeeA].size() )
               fhEpQualityFirstChFeeA[uChipFeeA]->Fill( fvChanOrder[uChipFeeA][0], 1 );
         } // if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk && fuPulserFee == (uChipFullId/kuNbChipFee) )
      } // else of  if( fvuCurrEpoch2[uChipFullId] +1 != mess.getGet4V10R32EpochNumber())

   // Epoch counter overflow book keeping
   if( (get4v1x::kul24bGet4EpochCycleSz - 2) < fvuCurrEpoch2[uChipFullId] &&
       mess.getEpoch2Number() < 2  )
      fvuCurrEpoch2Cycle[uChipFullId]++;

   fvuCurrEpoch2[uChipFullId] = mess.getEpoch2Number();

   // Monitoring of data Time evolution
   fvhRocEpochCntEvo[cRocId]->Fill(
         ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
          ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
          *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
/*
   fvhGet4EpochCntEvo[uChipFullId]->Fill(
            ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
              ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
            *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
   if( uChipFullId < kuNbTdcMappedDet )
      if( kiTdcDetMap[uChipFullId] < kiNbDet )
         fvhDetEpochCntEvo[ kiTdcDetMap[uChipFullId] ]->Fill(
             ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
              ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
             *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
*/

   if( 1 == mess.getEpoch2Sync() )
   {
      fhGet4EpochFlags->Fill(uChipFullId, 0);
      fhGet4EpochSyncDist->Fill( uChipFullId, fvuCurrEpoch2[uChipFullId] % get4v1x::kuSyncCycleSzGet4 );
   } // if( 1 == mess.getEpoch2Sync() )

   if( 1 == mess.getEpoch2EpochMissmatch() )
   {
      fhGet4EpochFlags->Fill(uChipFullId, 1);
      // If debug mode, print the message
      if( kTRUE == fbDebug )
         LOG(debug) << "Epoch missmatch in chip "
                <<Form("%3u: ", uChipFullId)
                <<Form(" EP #%6u", mess.getEpoch2Number() )
                <<Form(" TS #%12llu", fulTsNb)
                <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
      else LOG(debug) << "Epoch missmatch in chip "
             <<Form("%3u: ", uChipFullId)
             <<Form(" EP #%6u", mess.getEpoch2Number() )
             <<Form(" TS #%12llu", fulTsNb)
             <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
             <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
             <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);

      fvhRocEpochMissmCntEvo[cRocId]->Fill(
            ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
              ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
            *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
      fvhGet4EpochMissmCntEvo[uChipFullId]->Fill(
               ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                 ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
               *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
      if( uChipFullId < kuNbTdcMappedDet )
         if( kiTdcDetMap[uChipFullId] < kiNbDet )
            fvhDetEpochMissmCntEvo[ kiTdcDetMap[uChipFullId] ]->Fill(
                ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                 ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
   } // if( 1 == mess.getEpoch2EpochMissmatch() )
   if( 1 == mess.getEpoch2EpochLost() )
   {
      fhGet4EpochFlags->Fill(uChipFullId, 2);
      // If debug mode, print the message
      if( kTRUE == fbDebug )
         LOG(info) << "Epoch loss in chip      "
                <<Form("%3u: ", uChipFullId)
                <<Form(" EP #%6u", mess.getEpoch2Number() )
                <<Form(" TS #%12llu", fulTsNb)
                <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
      else LOG(debug) << "Epoch loss in chip      "
                <<Form("%3u: ", uChipFullId)
                <<Form(" EP #%6u", mess.getEpoch2Number() )
                <<Form(" TS #%12llu", fulTsNb)
                <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);

      fvhRocEpochLossCntEvo[cRocId]->Fill(
            ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
              ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
            *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
      fvhGet4EpochLossCntEvo[uChipFullId]->Fill(
               ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                 ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
               *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
      if( uChipFullId < kuNbTdcMappedDet )
         if( kiTdcDetMap[uChipFullId] < kiNbDet )
            fvhDetEpochLossCntEvo[ kiTdcDetMap[uChipFullId] ]->Fill(
                ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                 ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
   } // if( 1 == mess.getEpoch2EpochLost() )
   if( 1 == mess.getEpoch2DataLost() )
   {
      fhGet4EpochFlags->Fill(uChipFullId, 3);
      // If debug mode, print the message
      if( kTRUE == fbDebug )
         LOG(info) << "Data Loss in chip       "
                <<Form("%3u: ", uChipFullId)
                <<Form(" EP #%6u", mess.getEpoch2Number() )
                <<Form(" TS #%12llu", fulTsNb)
                <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
      else LOG(debug) << "Data Loss in chip       "
            <<Form("%3u: ", uChipFullId)
            <<Form(" EP #%6u", mess.getEpoch2Number() )
            <<Form(" TS #%12llu", fulTsNb)
            <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
            <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
            <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);

      fvhRocDataLossCntEvo[cRocId]->Fill(
            ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
              ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
            *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
      fvhGet4DataLossCntEvo[uChipFullId]->Fill(
               ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                 ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
               *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
      if( uChipFullId < kuNbTdcMappedDet )
         if( kiTdcDetMap[uChipFullId] < kiNbDet )
            fvhDetDataLossCntEvo[ kiTdcDetMap[uChipFullId] ]->Fill(
                ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                 ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s
   } // if( 1 == mess.getEpoch2DataLost() )

   // Fill Pulser test histos if needed
   // Accepted pairs are when both messages are defined and they are at most
   // 1 epoch apart => possibility of double use is the pulse happens on top of
   // an epoch and more than once every 3 epochs. For example:
   // HHHHEHHHH.......E......HHHHEHHHH leads to
   // (HHHHHHHH)             (HHHHHHHH) and
   //     (HHHH              HHHH)
   if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk
         && 0 == (uChipFullId%kuNbChipFee)
         && fuPulserFee == (uChipFullId/kuNbChipFee))
   {
      // Fill the time difference for all channels pairs in
      // the chosen Fee
      UInt_t uHistoFeeIdx = 0;
      for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
      {
         for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
         {
            if(
                ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA].getMessageType() ) &&
                ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeB].getMessageType() ) &&
                (   fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA ]
                  < fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeB ] + 2 ) &&
                (   fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA ] + 2
                  > fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeB ] ) )
            {
               Double_t dTimeDiff = 0.0;
               dTimeDiff = fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA].CalcGet4V10R24HitTimeDiff(
                     fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA],
                     fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeB],
                     fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeB] );

               fhTimeResFee[uHistoFeeIdx]->Fill( dTimeDiff );

               if( TMath::Abs(dTimeDiff) <= 5000 )
               {
                  fhFtSmallDtFeeA->Fill(
                        (fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtTotSmallDtFeeA->Fill(
                        (fvmLastOldTot[ fuPulserFee * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmLastOldTot[ fuPulserFee * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtPrevSmallDtFeeA->Fill(
                        (fvmPrevHit[ fuPulserFee * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmPrevHit[ fuPulserFee * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtPrevTotSmallDtFeeA->Fill(
                        (fvmPrevOldTot[ fuPulserFee * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmPrevOldTot[ fuPulserFee * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
               } // if( TMath::Abs(dTimeDiff) < 5000  )
               else if(  TMath::Abs(dTimeDiff) < 15000)
               {
                  fhFtBigDtFeeA->Fill(
                        (fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtTotBigDtFeeA->Fill(
                        (fvmLastOldTot[ fuPulserFee * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmLastOldTot[ fuPulserFee * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtPrevBigDtFeeA->Fill(
                        (fvmPrevHit[ fuPulserFee * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmPrevHit[ fuPulserFee * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtPrevTotBigDtFeeA->Fill(
                        (fvmPrevOldTot[ fuPulserFee * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmPrevOldTot[ fuPulserFee * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );

                  if( kTRUE == fbDebug )
                  {
                     LOG(info) << "Big time difference for FEE A channels "
                          <<Form("%3u %3u: ", uChanFeeA, uChanFeeB)
                          <<Form(" TS  #%12llu", fulTsNb);
                     fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA ].printDataLog();
                     fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeB ].printDataLog();
                  } // if( kTRUE == fbDebug )
               } // if( TMath::Abs(dTimeDiff) > 5000  && TMath::Abs(dTimeDiff) < 15000)
            } // if both channels have matching data

            if( kTRUE == fbPulserFeeB )
               if(
                ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeA].getMessageType() ) &&
                ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB].getMessageType() ) &&
                (   fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ]
                  < fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ] + 2 ) &&
                (   fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ] + 2
                  > fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ] ) )
            {
               Double_t dTimeDiffB = 0.0;
               dTimeDiffB = fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeA].CalcGet4V10R24HitTimeDiff(
                     fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeA],
                     fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB],
                     fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB] );

               fhTimeResFeeB[uHistoFeeIdx]->Fill( dTimeDiffB );

               if( TMath::Abs(dTimeDiffB) <= 5000 )
               {
                  fhFtSmallDtFeeB->Fill(
                        (fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtTotSmallDtFeeB->Fill(
                        (fvmLastOldTot[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmLastOldTot[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtPrevSmallDtFeeB->Fill(
                        (fvmPrevHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmPrevHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtPrevTotSmallDtFeeB->Fill(
                        (fvmPrevOldTot[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmPrevOldTot[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
               } // if( TMath::Abs(dTimeDiff) < 5000  )
               else if(  TMath::Abs(dTimeDiffB) < 15000)
               {
                  fhFtBigDtFeeB->Fill(
                        (fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtTotBigDtFeeB->Fill(
                        (fvmLastOldTot[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmLastOldTot[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtPrevBigDtFeeB->Fill(
                        (fvmPrevHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmPrevHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
                  fhFtPrevTotBigDtFeeB->Fill(
                        (fvmPrevOldTot[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ].getGet4Ts()) & get4v1x::kuFineTime,
                        (fvmPrevOldTot[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ].getGet4Ts()) & get4v1x::kuFineTime );
               } // if( TMath::Abs(dTimeDiff) > 5000  && TMath::Abs(dTimeDiff) < 15000)
            } // if both channels  in FEE B have matching data
            uHistoFeeIdx++;
         } // for any unique pair of channel in chosen Fee

         if( kTRUE == fbPulserFeeB )
            for( UInt_t uChanFeeB = 0; uChanFeeB < kuNbChanFee; uChanFeeB++)
         {
            if(
                ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA].getMessageType() ) &&
                ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB].getMessageType() ) &&
                (   fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA ]
                  < fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ] + 2 ) &&
                (   fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA ] + 2
                  > fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ] ) )
            {
               Double_t dTimeDiffAB = 0.0;
               dTimeDiffAB = fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA].CalcGet4V10R24HitTimeDiff(
                     fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA],
                     fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB],
                     fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB] );

               fhTimeResFeeAB[uChanFeeA*kuNbChanFee + uChanFeeB]->Fill( dTimeDiffAB );
            } // if both channels in FEE A and B have matching data
         } // if( kTRUE == fbPulserFeeB ) && for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
      } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)

      // Fill the time difference for the chosen channel pairs
      UInt_t uHistoCombiIdx = 0;
      for( UInt_t uChanA = 0; uChanA < kuNbChanTest-1; uChanA++)
      {
         if( ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserChan[uChanA]   ].getMessageType() ) &&
             ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserChan[uChanA+1] ].getMessageType() ) &&
             ( fvuLastHitEp[ fuPulserChan[uChanA]   ]
                   < fvuLastHitEp[ fuPulserChan[uChanA+1] ] + 2 ) &&
             ( fvuLastHitEp[ fuPulserChan[uChanA]   ] + 2
                   > fvuLastHitEp[ fuPulserChan[uChanA+1] ]     ) )
         {
            Double_t dTimeDiff =
                  fvmLastHit[   fuPulserChan[uChanA]   ].CalcGet4V10R24HitTimeDiff(
                  fvuLastHitEp[ fuPulserChan[uChanA]   ],
                  fvuLastHitEp[ fuPulserChan[uChanA+1] ],
                  fvmLastHit[   fuPulserChan[uChanA+1] ] );
            fhTimeResPairs[uChanA]->Fill( dTimeDiff );

            if( TMath::Abs(dTimeDiff) > 5000  && TMath::Abs(dTimeDiff) < 15000)
            {
               if( kTRUE == fbDebug )
               {
                  LOG(info) << "Big time difference for channel pair "
                       <<Form("%3u: ", uChanA)
                       <<Form(" TS  #%12llu", fulTsNb);
                  fvmLastHit[   fuPulserChan[uChanA] ].printDataLog();
                  fvmLastHit[   fuPulserChan[uChanA+1] ].printDataLog();
               } // if( kTRUE == fbDebug )
            } // if( TMath::Abs(dTimeDiff) > 5000 && TMath::Abs(dTimeDiff) < 15000)
         } // // if both channels have data

         for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
         {
            if( ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserChan[uChanA] ].getMessageType() ) &&
                ( get4v1x::MSG_GET4 == fvmLastHit[ fuPulserChan[uChanB] ].getMessageType() ) &&
                ( fvuLastHitEp[ fuPulserChan[uChanA] ]
                      < fvuLastHitEp[ fuPulserChan[uChanB] ] + 2 ) &&
                ( fvuLastHitEp[ fuPulserChan[uChanA] ] + 2
                      > fvuLastHitEp[ fuPulserChan[uChanB] ]     ) )
            {
               Double_t dTimeDiff =
                     fvmLastHit[   fuPulserChan[uChanA] ].CalcGet4V10R24HitTimeDiff(
                     fvuLastHitEp[ fuPulserChan[uChanA] ],
                     fvuLastHitEp[ fuPulserChan[uChanB] ],
                     fvmLastHit[   fuPulserChan[uChanB] ] );
               fhTimeResCombi[uHistoCombiIdx]->Fill( dTimeDiff );

               if( ( fvmLastHit[ fuPulserChan[uChanA] ].getGet4FineTs() < (get4v1x::kuFineCounterSize/2) ) &&
                   ( fvmLastHit[ fuPulserChan[uChanB] ].getGet4FineTs() < (get4v1x::kuFineCounterSize/2) ) )
                  fhTimeResCombiEncA[uHistoCombiIdx]->Fill( dTimeDiff );
               else if( ( (get4v1x::kuFineCounterSize/2) <= fvmLastHit[ fuPulserChan[uChanA] ].getGet4FineTs() ) &&
                        ( (get4v1x::kuFineCounterSize/2) <= fvmLastHit[ fuPulserChan[uChanB] ].getGet4FineTs() ) )
                  fhTimeResCombiEncB[uHistoCombiIdx]->Fill( dTimeDiff );

               if( TMath::Abs(dTimeDiff) > 5000  && TMath::Abs(dTimeDiff) < 15000)
               {
                  if( kTRUE == fbDebug )
                  {
                     LOG(info) << "Big time difference for channel combination "
                          <<Form("%3u: ", uHistoCombiIdx)
                          <<Form(" TS  #%12llu", fulTsNb);
                     fvmLastHit[   fuPulserChan[uChanA] ].printDataLog();
                     fvmLastHit[   fuPulserChan[uChanB] ].printDataLog();
                  } // if( kTRUE == fbDebug )
               } // if( TMath::Abs(dTimeDiff) > 5000 && TMath::Abs(dTimeDiff) < 15000)

            } // if both channels have data
            uHistoCombiIdx++;
         } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
      } // for( UInt_t uChanA = 0; uChanA < kuNbChanTest; uChanA++)

   } // if( kTRUE == fbPulserMode && 0 == uChipFullId && kTRUE == fbOldReadoutOk )

   // Check for missing edges in 24b mode with pulser
   if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk
         && fuPulserFee == (uChipFullId/kuNbChipFee) &&
         kTRUE == fbEnableMissingEdgeCheck )
   {
      UInt_t uChipInFee = uChipFullId%kuNbChipFee;
      UInt_t uChInFee   = get4v1x::kuChanPerGet4 * uChipInFee;
      UInt_t uFullChId  = get4v1x::kuChanPerGet4*( uChipFullId );

      for( uChInFee   = get4v1x::kuChanPerGet4 * uChipInFee;
           uChInFee   < get4v1x::kuChanPerGet4 * (uChipInFee+1);
           uChInFee ++, uFullChId++ )
      {
         UInt_t uLastTotValFt = fvuLastTotInFtBins[uChInFee];
         // Try to find missing edges
         if( ( ( 20 < fvmLastHit[uFullChId ].getGet4CoarseTs() ) &&
               ( fvmLastHit[uFullChId ].getGet4CoarseTs() < (get4v1x::kuCoarseCounterSize - 20) ) ) &&
             ( ( 20 < fvmLastOldTot[uFullChId ].getGet4CoarseTs() ) &&
               ( fvmLastOldTot[uFullChId ].getGet4CoarseTs() < (get4v1x::kuCoarseCounterSize - 20) ) ) )
         {
            if( fvuLastOldTotEp[uFullChId] == fvuLastHitEp[uFullChId] )
            {
               // Should be a good pulse away from the epoch edges and with matching edge messages
               // (same epoch, middle range of CT, ok as long as no more than 1 pulse/epoch)
               Double_t dTot = fvmLastOldTot[  uFullChId ].CalcGet4V10R24HitTimeDiff(
                                 fvuLastOldTotEp[uFullChId],
                                 fvuLastHitEp[ uFullChId],
                                 fvmLastHit[   uFullChId ] )/1000.0;
               if( 0.0 < dTot && dTot < 100.0 )
               {
                  // Good TOT => save its FT
                  if( fvmLastHit[ uFullChId ].getGet4FineTs() < fvmLastOldTot[ uFullChId ].getGet4FineTs() )
                     fvuLastTotInFtBins[uChInFee] =  fvmLastOldTot[ uFullChId ].getGet4FineTs()
                                                   - fvmLastHit[ uFullChId ].getGet4FineTs();
                     else fvuLastTotInFtBins[uChInFee] = get4v1x::kuFineCounterSize
                                                       + fvmLastOldTot[ uFullChId ].getGet4FineTs()
                                                       - fvmLastHit[ uFullChId ].getGet4FineTs();
               } // if( 0.0 < dTot && dTot < 100.0 )
               else
                  LOG(info)<<"Bad TOT "
                              <<uChInFee<<", FT(R) "
                              <<fvmLastHit[ uFullChId ].getGet4FineTs()<<", FT(F) "
                              <<fvmLastOldTot[ uFullChId ].getGet4FineTs()<<", CT(R) "
                              <<fvmLastHit[uFullChId ].getGet4CoarseTs()<<", CT(F) "
                              <<fvmLastOldTot[uFullChId ].getGet4CoarseTs()<<", Ep(R) "
                              <<fvuLastHitEp[uFullChId]<<", Ep(F) "
                              <<fvuLastOldTotEp[uFullChId]<<", TOT "
                              <<dTot;
            } // Same epoch => both edges there
               else // one edge is missing, assume its the one with old epoch index
               {
                  UInt_t uRecoFt = 0;
                  if( fvuLastOldTotEp[uFullChId] < fvuLastHitEp[uFullChId])
                  {
                     // Falling edge is missing
                     uRecoFt =  fvmLastHit[ uFullChId ].getGet4FineTs()
                               + fvuLastTotInFtBins[uChInFee];
                     if( get4v1x::kuFineTime < uRecoFt )
                        uRecoFt -= get4v1x::kuFineTime + 1;
                     fhPulserFeeFtRecoMissFal->Fill( uChInFee, uRecoFt);

                     LOG(info)<<"Missing edge: F, ch "
                                 <<uChInFee<<", FT(R) "
                                 <<fvmLastHit[ uFullChId ].getGet4FineTs()<<", FT(F) "
                                 <<uRecoFt<<", CT(R) "
                                 <<fvmLastHit[uFullChId ].getGet4CoarseTs()<<", CT(F) "
                                 <<fvmLastOldTot[uFullChId ].getGet4CoarseTs()<<", Ep(R) "
                                 <<fvuLastHitEp[uFullChId]<<", Ep(F) "
                                 <<fvuLastOldTotEp[uFullChId];
                  } // if( fvuLastOldTotEp[uChipInFee] < fvuLastHitEp[uChipInFee])
                     else
                     {
                        // Rising edge is missing
                        if( fvmLastOldTot[ uFullChId ].getGet4FineTs() < fvuLastTotInFtBins[uChInFee])
                           uRecoFt =   get4v1x::kuFineTime + 1
                                     + fvmLastOldTot[ uFullChId ].getGet4FineTs()
                                     - fvuLastTotInFtBins[uChInFee];
                           else uRecoFt =  fvmLastOldTot[ uFullChId ].getGet4FineTs()
                                         - fvuLastTotInFtBins[uChInFee];
                        fhPulserFeeFtRecoMissRis->Fill( uChInFee, uRecoFt);

                        LOG(info)<<"Missing edge: R, ch "
                                    <<uChInFee<<", FT(R) "
                                    <<uRecoFt<<", FT(F) "
                                    <<fvmLastOldTot[ uFullChId ].getGet4FineTs()<<", CT(R) "
                                    <<fvmLastHit[uFullChId ].getGet4CoarseTs()<<", CT(F) "
                                    <<fvmLastOldTot[uFullChId ].getGet4CoarseTs()<<", Ep(R) "
                                    <<fvuLastHitEp[uFullChId]<<", Ep(F) "
                                    <<fvuLastOldTotEp[uFullChId];
                     } // else of if( fvuLastOldTotEp[uChipInFee] < fvuLastHitEp[uChipInFee])
               } // else of if( fvuLastOldTotEp[uChipInFee] == fvuLastHitEp[uChipInFee] )
         } // if both edges in safe CT range to avoid epoch edges crossing

         // Check if extra edges
         if( (1 < fvuNbRisEdgeEpoch[uChInFee] || 1 < fvuNbFalEdgeEpoch[uChInFee]) &&
               ( fvuNbRisEdgeEpoch[uChInFee] != fvuNbFalEdgeEpoch[uChInFee] ))
         {
            if( (2 == fvuNbRisEdgeEpoch[uChInFee] && 2 >= fvuNbFalEdgeEpoch[uChInFee]) ||
                (2 >= fvuNbRisEdgeEpoch[uChInFee] && 2 == fvuNbFalEdgeEpoch[uChInFee]) )
            {
               LOG(info)<< "Extra edges and edges nb missmatch: ch " << Form("%2u", uChInFee)
                        << " Nb Ris " <<fvuNbRisEdgeEpoch[uChInFee]
                        << " Nb Fal " <<fvuNbFalEdgeEpoch[uChInFee]
                        << Form(" TS #%12llu", fulTsNb)
                        << Form(" Last TOT %3u %3u", uLastTotValFt, fvuLastTotInFtBins[uChInFee]);
               if(2 == fvuNbRisEdgeEpoch[uChInFee] && 1 == fvuNbFalEdgeEpoch[uChInFee])
               {
                  UInt_t uTotReco = 0;
                  if( fvmFeePrevRis[  uChInFee ].getGet4FineTs() < fvmLastOldTot[ uFullChId ].getGet4FineTs() )
                     uTotReco =  fvmLastOldTot[ uFullChId ].getGet4FineTs()
                               - fvmFeePrevRis[  uChInFee ].getGet4FineTs();
                     else uTotReco = get4v1x::kuFineCounterSize
                                    + fvmLastOldTot[ uFullChId ].getGet4FineTs()
                                    - fvmFeePrevRis[  uChInFee ].getGet4FineTs();
                  LOG(info)<< Form(" TOT reco %3u", uTotReco);
                  fhPulserFeeExtraRecoTot->Fill( uChInFee, uTotReco);
               } // if(2 == fvuNbRisEdgeEpoch[uChInFee] && 1 == fvuNbFalEdgeEpoch[uChInFee])
               if(1 == fvuNbRisEdgeEpoch[uChInFee] && 2 == fvuNbFalEdgeEpoch[uChInFee])
               {
                  UInt_t uTotReco = 0;
                  if( fvmLastHit[ uFullChId ].getGet4FineTs() < fvmLastOldTot[ uFullChId ].getGet4FineTs() )
                     uTotReco =  fvmLastOldTot[ uFullChId ].getGet4FineTs()
                               - fvmLastHit[ uFullChId ].getGet4FineTs();
                     else uTotReco = get4v1x::kuFineCounterSize
                                    + fvmLastOldTot[ uFullChId ].getGet4FineTs()
                                    - fvmLastHit[ uFullChId ].getGet4FineTs();
                  LOG(info)<< Form(" TOT reco %3u", uTotReco);
                  fhPulserFeeExtraRecoTot->Fill( uChInFee, uTotReco);
               } // if(1 == fvuNbRisEdgeEpoch[uChInFee] && 2 == fvuNbFalEdgeEpoch[uChInFee])
               LOG(info)<< "";
               LOG(info)<<"      Previous messages            "
                        << ( fvuFeePrevRisEp[uChInFee] == fvuLastHitEp[uFullChId] ?
                           Form("  FT(R) %3u, CT(R) %4u, Ep(R) %6u",
                              fvmFeePrevRis[  uChInFee ].getGet4FineTs(),
                              fvmFeePrevRis[  uChInFee ].getGet4CoarseTs(),
                              fvuFeePrevRisEp[uChInFee] ) :
                           "                                     ")
                        << ( fvuFeePrevFalEp[uChInFee] == fvuLastOldTotEp[uFullChId] ?
                           Form("  FT(F) %3u, CT(F) %4u, Ep(F) %6u",
                              fvmFeePrevFal[  uChInFee ].getGet4FineTs(),
                              fvmFeePrevFal[  uChInFee ].getGet4CoarseTs(),
                              fvuFeePrevFalEp[uChInFee] ) :
                           "                                     ");
               LOG(info)<< "";
               LOG(info)<<"      Last messages                "
                        <<Form("  FT(R) %3u, CT(R) %4u, Ep(R) %6u",
                              fvmLastHit[  uFullChId ].getGet4FineTs(),
                              fvmLastHit[  uFullChId ].getGet4CoarseTs(),
                              fvuLastHitEp[uFullChId] )
                        <<Form("  FT(F) %3u, CT(F) %4u, Ep(F) %6u",
                              fvmLastOldTot[  uFullChId ].getGet4FineTs(),
                              fvmLastOldTot[  uFullChId ].getGet4CoarseTs(),
                              fvuLastOldTotEp[uFullChId] );

               if( 2 == fvuNbRisEdgeEpoch[uChInFee] )
                  fhPulserFeeFtExtraEdgeRisA->Fill( uChInFee, fvmFeePrevRis[  uChInFee ].getGet4FineTs());
               if( 2 == fvuNbFalEdgeEpoch[uChInFee] )
                  fhPulserFeeFtExtraEdgeFalA->Fill( uChInFee, fvmFeePrevFal[  uChInFee ].getGet4FineTs());
               fhPulserFeeFtExtraEdgeRisB->Fill( uChInFee, fvmLastHit[  uFullChId ].getGet4FineTs());
               fhPulserFeeFtExtraEdgeFalB->Fill( uChInFee, fvmLastOldTot[  uFullChId ].getGet4FineTs());
            } // if only one extra message or less per edge
            else
            {
               LOG(info)<<"Extra edges and edges nb missmatch: ch " << uChInFee
                        << " Nb Ris " <<fvuNbRisEdgeEpoch[uChInFee]
                        << " Nb Fal " <<fvuNbFalEdgeEpoch[uChInFee]
                        << Form(" TS #%12llu", fulTsNb);
               LOG(info)<< "";
               LOG(info)<<"                                   "
                        <<Form("  FT(R) %3u, CT(R) %4u, Ep(R) %6u",
                              fvmLastHit[  uFullChId ].getGet4FineTs(),
                              fvmLastHit[  uFullChId ].getGet4CoarseTs(),
                              fvuLastHitEp[uFullChId] )
                        <<Form("  FT(F) %3u, CT(F) %4u, Ep(F) %6u",
                              fvmLastOldTot[  uFullChId ].getGet4FineTs(),
                              fvmLastOldTot[  uFullChId ].getGet4CoarseTs(),
                              fvuLastOldTotEp[uFullChId] );
            }
         }
            else fhPulserFeeGoodTot->Fill( uChInFee, fvuLastTotInFtBins[uChInFee]);
         if( 1 < fvuNbRisEdgeEpoch[uChInFee] )
         {
            fhPulserFeeExtraRisEp->Fill( uChInFee, fvuNbRisEdgeEpoch[uChInFee]);
            fhPulserFeeExtraEdgesEp->Fill( uChInFee - 0.25, fvuNbRisEdgeEpoch[uChInFee]);
         }
         fvuNbRisEdgeEpoch[uChInFee] = 0;
         if( 1 < fvuNbFalEdgeEpoch[uChInFee] )
         {
            fhPulserFeeExtraFalEp->Fill( uChInFee, fvuNbFalEdgeEpoch[uChInFee]);
            fhPulserFeeExtraEdgesEp->Fill( uChInFee + 0.25, fvuNbFalEdgeEpoch[uChInFee]);
         }
         fvuNbFalEdgeEpoch[uChInFee] = 0;
      } // Loop over channels in this GET4 chip
   } //    if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk && fuPulserFee == (uChipFullId/kuNbChipFee))

   // for channel readout order in case of epoch jumps
   if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk
         && fuPulserFee == (uChipFullId/kuNbChipFee))
      fvChanOrder[uChipFullId%kuNbChipFee].clear();

}
void CbmGet4FastMonitor::MonitorMessage_get4(   get4v1x::Message mess, uint16_t EqID)
{
   // GET4 v1.x data message (24b only for now)
   // TODO: check compatibility when 32b format without hack ready
   uint8_t  cRocId    = mess.getRocNumber();
   LOG(debug3)<<"CbmGet4FastMonitor::MonitorMessage_get4 => GET4 Data: EqId "
               <<EqID<<" roc "
               <<cRocId;

   uint8_t  cChipId   = mess.getGet4Number();
   UInt_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;
   UInt_t uFullChId =
         get4v1x::kuChanPerGet4*( uChipFullId )
         + mess.getGet4ChNum();
   UInt_t uTimeStamp = mess.getGet4Ts();

   // Reject data of chips above defined limit to avoid overflow/seg fault
   if( fuNbGet4 <= uChipFullId )
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmGet4FastMonitor::MonitorMessage_get4 => Ignored message with ChipId above limit!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId
                  <<" Limit: " <<fuNbGet4;
      return;
   } // if( fuNbGet4 <= cChipId + kuMaxGet4PerRoc*mess.getRocNumber() )

   // Reject data of Inactive chips
   if( kFALSE == fvbActiveChips[uChipFullId])
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmGet4FastMonitor::MonitorMessage_get4 => Ignored message with Inactive Chip!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId ;
      return;
   } // if( kFALSE == fvbActiveChips[uChipFullId])

   // As long as the 1st epoch with sync flag after SYNC DLM is found,
   // consider data as not good
   if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )
      return;

   fvhRocDataCntEvo[cRocId]->Fill(
            ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
              ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
             *get4v1x::kdEpochInPs
             + uTimeStamp*get4v1x::kdBinSize )
            *(1e-12) ); // Time since start in s
   fvhGet4DataCntEvo[uChipFullId]->Fill(
            ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
              ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
             *get4v1x::kdEpochInPs
             + uTimeStamp*get4v1x::kdBinSize )
             *(1e-12) ); // Time since start in s
   fvhGet4ChDataCntEvo[uChipFullId]->Fill( mess.getGet4ChNum(),
            ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
              ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
             *get4v1x::kdEpochInPs
             + uTimeStamp*get4v1x::kdBinSize )
             *(1e-12) ); // Time since start in s
   if( uChipFullId < kuNbTdcMappedDet )
      if( kiTdcDetMap[uChipFullId] < kiNbDet )
         fvhDetDataCntEvo[ kiTdcDetMap[uChipFullId] ]->Fill(
            ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
              ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
             *get4v1x::kdEpochInPs
             + uTimeStamp*get4v1x::kdBinSize )
            *(1e-12) ); // Time since start in s

   // Count data messages per chip
   fhMessTypePerGet4->Fill( uChipFullId, get4v1x::GET4_32B_DATA);

   if( kTRUE == fbEnaCalibOutput )
      fvhFineTime[ uChipFullId ][ mess.getGet4ChNum() ]->Fill( mess.getGet4FineTs() );

   if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk )
   {
      if( 1 == mess.getGet4Edge() )
      {
         Double_t dTot = mess.CalcGet4V10R24HitTimeDiff(
                           fvuCurrEpoch2[uChipFullId],
                           fvuLastHitEp[    uFullChId],
                           fvmLastHit[   uFullChId ] )/1000.0;
         if( 0.0 < dTot && dTot < 100.0 )
            fhGet4ChanTotMap->Fill(   uFullChId, dTot);

         // Save the hit info in order to fill later the pulser histos
         // First fill time interval histos
         if( fuPulserFee == (uFullChId/kuNbChanFee) )
         {
            // Fill the DNL histos
            fhPulserFeeTotDnl->Fill( uFullChId%kuNbChanFee, uTimeStamp & get4v1x::kuFineTime );

            // Fill the CT histo
            fhPulserFeeFalCtWideBins->Fill( uFullChId%kuNbChanFee, mess.getGet4CoarseTs() );
            fhPulserFeeTotDistCT[ uChipFullId%kuNbChipFee ]->Fill( mess.getGet4CoarseTs(), mess.getGet4ChNum());

            if( kTRUE == fbEnableMissingEdgeCheck )
            {
               fvuFeePrevFalEp[uFullChId%kuNbChanFee] = fvuLastOldTotEp[ uFullChId ];
               fvmFeePrevFal[  uFullChId%kuNbChanFee] = fvmLastOldTot[   uFullChId ];
               // count edges per epoch
               fvuNbFalEdgeEpoch[uFullChId%kuNbChanFee] ++;
            } // if( kTRUE == fbEnableMissingEdgeCheck )
         } // if( fuPulserFee == (uFullChId/kuNbChanFee) )
      } // if( 1 == mess.getGet4Edge() )
      else
         {
            Double_t dTot = fvmLastOldTot[   uFullChId ].CalcGet4V10R24HitTimeDiff(
                              fvuLastOldTotEp[ uFullChId ],
                              fvuCurrEpoch2[uChipFullId],
                              mess )/1000.0;
            if( 0.0 < dTot && dTot < 100.0 )
               fhGet4ChanTotMap->Fill(   uFullChId, dTot);

            // Save the hit info in order to fill later the pulser histos
            // First fill time interval histos
            if( fuPulserFee == (uFullChId/kuNbChanFee) )
            {
               Double_t dHitsDt = mess.CalcGet4V10R24HitTimeDiff(
                           fvuCurrEpoch2[uChipFullId],
                           fvuLastHitEp[ uFullChId ],
                           fvmLastHit[   uFullChId ] );
               if( 0 == fvuLastHitEp[uFullChId])
                  {}
               else if( dHitsDt < 1e6 )
                  fhPulserHitDistNs->Fill( uFullChId%kuNbChanFee, dHitsDt / 1e3 );
               else if( dHitsDt < 1e9)
                  fhPulserHitDistUs->Fill( uFullChId%kuNbChanFee, dHitsDt / 1e6 );
               else
                  fhPulserHitDistMs->Fill( uFullChId%kuNbChanFee, dHitsDt / 1e9 );

               if( 0 < fvuLastHitEp[uFullChId] && dHitsDt < 5e3 )
                  LOG(debug) <<uFullChId<<" "<<dHitsDt<<" "
                      <<fvuLastHitEp[ uFullChId ]<<" "<<fvuCurrEpoch2[uChipFullId]<<" "
                      <<fvmLastOldTot[   uFullChId ].CalcGet4V10R24HitTimeDiff(
                            fvuLastOldTotEp[ uFullChId ],
                            fvuLastHitEp[    uFullChId],
                            fvmLastHit[   uFullChId ] )/1000.0<<" "
                      <<fvmLastOldTot[   uFullChId ].CalcGet4V10R24HitTimeDiff(
                            fvuLastOldTotEp[ uFullChId ],
                            fvuCurrEpoch2[uChipFullId],
                            mess )/1000.0;;

               // Fill the DNL histos
               fhPulserFeeDnl->Fill( uFullChId%kuNbChanFee, uTimeStamp & get4v1x::kuFineTime );

               // Fill the CT histo
               fhPulserFeeRisCtWideBins->Fill( uFullChId%kuNbChanFee, mess.getGet4CoarseTs() );
               fhPulserFeeDistCT[ uChipFullId%kuNbChipFee ]->Fill( mess.getGet4CoarseTs(), mess.getGet4ChNum());

               if( kTRUE == fbEnableMissingEdgeCheck )
               {
                  fvuFeePrevRisEp[uFullChId%kuNbChanFee] = fvuLastHitEp[ uFullChId ];
                  fvmFeePrevRis[  uFullChId%kuNbChanFee] = fvmLastHit[   uFullChId ];
                  // count edges per epoch
                  fvuNbRisEdgeEpoch[uFullChId%kuNbChanFee] ++;
               } // if( kTRUE == fbEnableMissingEdgeCheck )
            } // if( fuPulserFee == (uFullChId/kuNbChanFee) )
         } // else of if( 1 == mess.getGet4Edge() )
   } // if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk )

   if( kTRUE == fbOldReadoutOk )
   {
      if( 1 == mess.getGet4Edge() )
      {
         fhGet4ChanTotCount->Fill( uFullChId );

         // Keep Epoch of Previous hit message (one per GET4 chip & channel)
         fvuPrevOldTotEp[ uFullChId ] = fvuLastOldTotEp[ uFullChId ];
         // Keep Previous hit message (one per GET4 chip & channel)
         fvmPrevOldTot[ uFullChId ]   = fvmLastOldTot[ uFullChId ];

         // Epoch of Last hit message (one per GET4 chip & channel)
         fvuLastOldTotEp[ uFullChId ] = fvuCurrEpoch2[uChipFullId];
         // Last hit message (one per GET4 chip & channel)
         fvmLastOldTot[ uFullChId ] = mess;

         fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()] =
               mess.CalcGet4V10R24HitTimeDiff(
                  fvuCurrEpoch2[uChipFullId], fvuLastHitEp[ uFullChId ],  fvmLastHit[   uFullChId ] );
      } // if( 1 == mess.getGet4Edge() )
      else
      {
         fhGet4ChanDataCount->Fill( uFullChId );
         // Fill time coincidence map
         for( UInt_t uFeeA = 0; uFeeA < fuNbFee; uFeeA++)
            for( UInt_t uFeeChan = 0; uFeeChan < get4v1x::kuChanPerFee; uFeeChan++ )
               if( ( get4v1x::MSG_GET4 ==
                     fvmLastHit[ uFeeA*get4v1x::kuChanPerFee + uFeeChan ].getMessageType() ) &&
                   ( fvuLastHitEp[ uFeeA*get4v1x::kuChanPerFee + uFeeChan/get4v1x::kuChanPerGet4 ]
                         < fvuCurrEpoch2[uChipFullId] + 2 ) &&
                   ( fvuLastHitEp[ uFeeA*get4v1x::kuChanPerFee + uFeeChan/get4v1x::kuChanPerGet4 ] + 2
                         > fvuCurrEpoch2[uChipFullId]     ) )
            {
               // compare to last hit in other channel
               Double_t dHitsDt = mess.CalcGet4V10R24HitTimeDiff(
                           fvuCurrEpoch2[uChipFullId],
                           fvuLastHitEp[ uFeeA*get4v1x::kuChanPerFee + uFeeChan/get4v1x::kuChanPerGet4 ],
                           fvmLastHit[   uFeeA*get4v1x::kuChanPerFee + uFeeChan ] );
               if( TMath::Abs(dHitsDt) < fdMaxCoincDist  )
               {
                  if( uFeeA*get4v1x::kuChanPerFee + uFeeChan < uFullChId )
                     fvvhChannelsCoinc[uFeeA][uFullChId/get4v1x::kuChanPerFee]->Fill(
                        uFeeChan, uFullChId%get4v1x::kuChanPerFee);
                     else fvvhChannelsCoinc[uFullChId/get4v1x::kuChanPerFee][uFeeA]->Fill(
                           uFullChId%get4v1x::kuChanPerFee, uFeeChan);
               } // if( TMath::Abs(dHitsDt) < fdMaxCoincDist  )
            } // for each pair, if both hits valid

         // Multiple hits (reflections, ...) detection
         if( 1 == fiMode)
         {
            Double_t dHitsDt = mess.CalcGet4V10R24HitTimeDiff(
                        fvuCurrEpoch2[uChipFullId],
                        fvuLastHitEp[ uFullChId ],
                        fvmLastHit[   uFullChId ] );
            if( dHitsDt < fdMaxDtMultiHit )
            {
               if( kFALSE == fvbChanSecondHit[uChipFullId][mess.getGet4ChNum()] )
               {
                  fvhGet4MultipleHits[uChipFullId]->Fill( mess.getGet4ChNum(), 2);
                  fvhGet4DistDoubleHits[uChipFullId]->Fill( mess.getGet4ChNum(), dHitsDt);
                  fvbChanSecondHit[uChipFullId][mess.getGet4ChNum()] = kTRUE;

                  fvhGet4MultipleHitsVsTot[uChipFullId]->Fill(
                        fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()], 2);
               }
               else if ( kFALSE == fvbChanThirdHit[uChipFullId][mess.getGet4ChNum()] )
               {
                  fvhGet4MultipleHits[uChipFullId]->Fill( mess.getGet4ChNum(), 3);
                  fvhGet4DistTripleHits[uChipFullId]->Fill( mess.getGet4ChNum(), dHitsDt);
                  fvbChanThirdHit[uChipFullId][mess.getGet4ChNum()] = kTRUE;

                  fvhGet4MultipleHitsVsTot[uChipFullId]->Fill(
                        fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()], 3);
               }
               else
               {
                  fvhGet4MultipleHits[uChipFullId]->Fill( mess.getGet4ChNum(), 4);
                  fvhGet4DistMultipleHits[uChipFullId]->Fill( mess.getGet4ChNum(), dHitsDt);

                  fvhGet4MultipleHitsVsTot[uChipFullId]->Fill(
                        fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()], 4);
               }
            } // if( dHitsDt < fdMaxDtMultiHit )
               else
               {
                  fvhGet4MultipleHits[uChipFullId]->Fill( mess.getGet4ChNum(), 1);
                  fvbChanSecondHit[uChipFullId][mess.getGet4ChNum()] = kFALSE;
                  fvbChanThirdHit[uChipFullId][mess.getGet4ChNum()]  = kFALSE;

                  fvhGet4MultipleHitsVsTot[uChipFullId]->Fill(
                        fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()], 1);
               } // else of if( dHitsDt < fdMaxDtMultiHit )
         } // if( 1 == fiMode)

         // Keep Epoch of Previous hit message (one per GET4 chip & channel)
         fvuPrevHitEp[ uFullChId ] = fvuLastHitEp[ uFullChId ];
         // Keep Previous hit message (one per GET4 chip & channel)
         fvmPrevHit[ uFullChId ] = fvmLastHit[ uFullChId ];

         // Epoch of Last hit message (one per GET4 chip & channel)
         fvuLastHitEp[ uFullChId ] = fvuCurrEpoch2[uChipFullId];
         // Last hit message (one per GET4 chip & channel)
         fvmLastHit[ uFullChId ] = mess;

         // for the channel readout order in case of epoch jump
         if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk
                  && fuPulserFee == (uChipFullId/kuNbChipFee))
            fvChanOrder[uChipFullId%kuNbChipFee].push_back( mess.getGet4ChNum() );
      } // else of if( 1 == mess.getGet4Edge() )
   } // if( kTRUE == fbOldReadoutOk )
}
void CbmGet4FastMonitor::MonitorMessage_sys(    get4v1x::Message mess, uint16_t EqID)
{
   uint8_t  cRocId    = mess.getRocNumber();
   LOG(debug3)<<"CbmGet4FastMonitor::MonitorMessage_sys => System message: EqId "
               <<EqID<<" roc "
               <<cRocId;

   if( mess.getSysMesType() < get4v1x::SYSMSG_GET4V1_32BIT_0 )
      fhSysMessTypePerRoc->Fill( cRocId, mess.getSysMesType() );

   switch( mess.getSysMesType() )
   {
      case get4v1x::SYSMSG_DAQ_START:
      {
         break;
      } // case get4v1x::SYSMSG_DAQ_START
      case get4v1x::SYSMSG_DAQ_FINISH:
      {
         break;
      } // case get4v1x::SYSMSG_DAQ_FINISH
      case get4v1x::SYSMSG_NX_PARITY:
      case get4v1x::SYSMSG_ADC:
      case get4v1x::SYSMSG_PCTIME:
      {
         // NXYTER related => don't care in this unpacker
         break;
      } // case get4v1x::SYSMSG_NX_PARITY
      case get4v1x::SYSMSG_SYNC_PARITY:
      {
         break;
      } // case get4v1x::SYSMSG_SYNC_PARITY
      case get4v1x::SYSMSG_DAQ_RESUME:
      {
         break;
      } // case get4v1x::SYSMSG_DAQ_RESUME
      case get4v1x::SYSMSG_FIFO_RESET:
      {
         break;
      } // case get4v1x::SYSMSG_FIFO_RESET
      case get4v1x::SYSMSG_USER:
      {
         if( get4v1x::SYSMSG_USER_ROCFEET_SYNC == mess.getSysMesData() )
         {
            LOG(info)<<"CbmGet4FastMonitor::MonitorMessage_sys => SYNC DLM!"
                     <<" RocId: " << Form("%02u", cRocId)
                     <<Form(" TS  #%12llu", fulTsNb)
                     <<Form(" MS  #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                     <<Form(" OF  #%5u/%5u", fuOffset, fuMaxOffset)
                     <<Form(" LO  #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
            if( kTRUE == fvbRocFeetFirstSyncDlm[cRocId] &&
                kFALSE == fvbRocFeetSyncStart[cRocId])
               fvbRocFeetSyncStart[cRocId] = kTRUE;
            else if( kFALSE == fvbRocFeetFirstSyncDlm[cRocId]  )
               fvbRocFeetFirstSyncDlm[cRocId] = kTRUE;
         } // if( get4v1x::SYSMSG_USER_ROCFEET_SYNC == mess.getSysMesData() )
         break;
      } // case get4v1x::SYSMSG_USER
      case get4v1x::SYSMSG_PACKETLOST:
      {
         break;
      } // case get4v1x::SYSMSG_PACKETLOST
      case get4v1x::SYSMSG_GET4_EVENT:
      {
         // GET4 v1.x error message (24b only for now)
         // TODO: check compatibility when 32b format without hack ready
         uint8_t  cChipId   = mess.getGet4V10R24ErrorChip();
         uint32_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;
         uint8_t  uErrorType= mess.getGet4V10R24ErrorData();

         // As long as the 1st epoch with sync flag after SYNC DLM is found,
         // consider data as not good
         if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )
            return;

         // Count error messages per ROC
         fvhRocErrorCntEvo[cRocId]->Fill(
               ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                 ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                *get4v1x::kdEpochInPs )
               *(1e-12) ); // Time since start in s
         // Count error messages per chip
         fhMessTypePerGet4->Fill( uChipFullId, get4v1x::GET4_32B_ERROR);
         fvhGet4ErrorCntEvo[uChipFullId]->Fill(
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs )
                  *(1e-12) ); // Time since start in s

         if( ( get4v1x::GET4_V1X_ERR_EP < uErrorType &&
               uErrorType < get4v1x::GET4_V1X_ERR_TOK_RING_ST ) )
            fvhGet4ChErrorCntEvo[uChipFullId]->Fill( mess.getGet4V10R24ErrorChan(),
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs )
                  *(1e-12) ); // Time since start in s

         if( uChipFullId < kuNbTdcMappedDet )
            if( kiTdcDetMap[uChipFullId] < kiNbDet )
               fvhDetErrorCntEvo[ kiTdcDetMap[uChipFullId] ]->Fill(
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs)
                  *(1e-12) ); // Time since start in s
         break;
      } // case get4v1x::SYSMSG_GET4_EVENT
      case get4v1x::SYSMSG_CLOSYSYNC_ERROR:
      {
         break;
      } // case get4v1x::SYSMSG_CLOSYSYNC_ERROR
      case get4v1x::SYSMSG_TS156_SYNC:
      {
         break;
      } // case get4v1x::SYSMSG_TS156_SYNC
      case get4v1x::SYSMSG_GET4V1_32BIT_0:
      case get4v1x::SYSMSG_GET4V1_32BIT_1:
      case get4v1x::SYSMSG_GET4V1_32BIT_2:
      case get4v1x::SYSMSG_GET4V1_32BIT_3:
      case get4v1x::SYSMSG_GET4V1_32BIT_4:
      case get4v1x::SYSMSG_GET4V1_32BIT_5:
      case get4v1x::SYSMSG_GET4V1_32BIT_6:
      case get4v1x::SYSMSG_GET4V1_32BIT_7:
      case get4v1x::SYSMSG_GET4V1_32BIT_8:
      case get4v1x::SYSMSG_GET4V1_32BIT_9:
      case get4v1x::SYSMSG_GET4V1_32BIT_10:
      case get4v1x::SYSMSG_GET4V1_32BIT_11:
      case get4v1x::SYSMSG_GET4V1_32BIT_12:
      case get4v1x::SYSMSG_GET4V1_32BIT_13:
      case get4v1x::SYSMSG_GET4V1_32BIT_14:
      case get4v1x::SYSMSG_GET4V1_32BIT_15:
      {
         // Ignore all messages before RocFeet system SYNC
         if (kFALSE == fvbRocFeetSyncStart[cRocId] )
            break;

         // GET4 v1.x 32b raw message using hack
         fhMessageTypePerRoc->Fill( cRocId, 15 );
         fhSysMessTypePerRoc->Fill( cRocId, 15 );

         MonitorMessage_Get4v1( mess, EqID);
         break;
      } // case get4v1x::SYSMSG_TS156_SYNC
      default:
      {
         if( kTRUE == fbVerbose )
            LOG(warning)<<"CbmGet4FastMonitor::MonitorMessage_sys => Unknown sysMess type ignored: "
                        <<mess.getSysMesType();
         break;
      } // default:
   } // switch( mess.getSysMesType() )
}
void CbmGet4FastMonitor::MonitorMessage_Get4v1( get4v1x::Message mess, uint16_t EqID)
{
   // GET4 v1.x 32b raw message using hack
   uint8_t  cRocId    = mess.getRocNumber();
   uint8_t  cChipId   = mess.getGet4V10R32ChipId();
   uint32_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;
   uint8_t  cMessType = mess.getGet4V10R32MessageType();

   LOG(debug3)<<"CbmGet4FastMonitor::MonitorMessage_Get4v1 => 32b GET4 message: EqId "
               <<EqID<<" roc "
               <<cRocId;

   // Reject data of chips above defined limit to avoid overflow/seg fault
   if( fuNbGet4 <= uChipFullId )
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmGet4FastMonitor::MonitorMessage_Get4v1 => Ignored message with ChipId above limit!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId
                  <<" Limit: " <<fuNbGet4;
      return;
   } // if( fuNbGet4 <= cChipId + kuMaxGet4PerRoc*mess.getRocNumber() )

   // Reject data of Inactive chips
   if( kFALSE == fvbActiveChips[uChipFullId])
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmGet4FastMonitor::MonitorMessage_Get4v1 => Ignored message with Inactive Chip!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId ;
      return;
   } // if( kFALSE == fvbActiveChips[uChipFullId])

   // Count messages type per chip
   fhMessTypePerGet4->Fill( uChipFullId, cMessType);

   switch( cMessType )
   {
      case get4v1x::GET4_32B_EPOCH: // => Epoch message
      {
         // As long as the 1st epoch with sync flag after SYNC DLM is found,
         // consider data as not good
         if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )
         {
            if( 1 == mess.getGet4V10R32SyncFlag() )
            {
               fvbGet4WaitFirstSync[uChipFullId] = kFALSE;

               LOG(info) << "First SYNC Epoch after DLM in chip "
                         <<Form("%3u: ", uChipFullId)
                         <<Form(" EP #%6u", mess.getGet4V10R32EpochNumber() )
                         <<Form(" TS #%12llu", fulTsNb)
                         <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                         <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                         <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
            } // if( 1 == mess.getGet4V10R32SyncFlag() )
               else return;
         } // if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )

         if( fvuCurrEpoch2[uChipFullId] +1 != mess.getGet4V10R32EpochNumber() &&
               0 != fvuCurrEpoch2[uChipFullId] )
         {
            Int_t iEpJump = mess.getGet4V10R32EpochNumber();
            iEpJump      -= fvuCurrEpoch2[uChipFullId];
            if( TMath::Abs(iEpJump) < 200 )
            {
               if( kTRUE == fbDebug )
                  LOG(info) << "Epoch nb jump in chip "
                       <<Form("%3u: ", uChipFullId)
                       << Form(" %3d (%6u -> %6u)", iEpJump, fvuCurrEpoch2[uChipFullId],
                                                    mess.getGet4V10R32EpochNumber() )
                       <<Form(" TS  #%12llu", fulTsNb)
                       <<Form(" MS  #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                       <<Form(" OF  #%5u/%5u", fuOffset, fuMaxOffset)
                       <<Form(" LO  #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
            } // if( TMath::Abs(iEpJump) < 200 )
            else LOG(info) << "Epoch nb jump in chip "
                       <<Form("%3u: ", uChipFullId)
                       << Form(" %3d (%6u -> %6u)", iEpJump, fvuCurrEpoch2[uChipFullId],
                                                    mess.getGet4V10R32EpochNumber() )
                       <<Form(" TS  #%12llu", fulTsNb)
                       <<Form(" MS  #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                       <<Form(" OF  #%5u/%5u", fuOffset, fuMaxOffset)
                       <<Form(" LO  #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);

            fhGet4EpochJumps->Fill(uChipFullId, iEpJump);
            fhGet4BadEpochRatio->Fill(0);
            fhGet4BadEpRatioChip->Fill( uChipFullId, 0);

            if( kTRUE == fbPulserMode && fuPulserFee == (uChipFullId/kuNbChipFee) )
            {
               UInt_t uChipFeeA = uChipFullId%kuNbChipFee;
               for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )
                  if( 0ull != fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getData() )
               {
                  fhFullCtEpJumpFeeA[uChipFeeA]->Fill(
                        fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTs(),
                        fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTs() +
                        fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTot()/
                        static_cast<Int_t>(get4v1x::kdClockCycleSize/get4v1x::kdTotBinSize) );
                  fhFullCtEpJumpFeeACh[uChan]->Fill(
                        fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTs(),
                        fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTs() +
                        fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTot()/
                        static_cast<Int_t>(get4v1x::kdClockCycleSize/get4v1x::kdTotBinSize) );

                  fhFullCtEpQualityCh[uChan]->Fill(
                        fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTs(), 0);
                  fhFullCtEpQualityChZoom[uChan]->Fill(
                        fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTs(), 0);

                  if( get4v1x::kuChanPerGet4 == fvChanOrder[uChipFeeA].size() )
                  {
                     UInt_t uChOrder;
                     for( uChOrder = 0; uChOrder < get4v1x::kuChanPerGet4; uChOrder++ )
                        if( uChan == fvChanOrder[uChipFeeA][uChOrder] )
                           break;
                     fhFullCtChOrderCh[uChan]->Fill(
                           fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTs(), uChOrder);
                  } // if( get4v1x::kuChanPerGet4 == fvChanOrder[uChipFeeA].size() )
               } // for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )

               if( get4v1x::kuChanPerGet4 == fvChanOrder[uChipFeeA].size() )
                  for( UInt_t uChOrder = 0; uChOrder < fvChanOrder[uChipFeeA].size(); uChOrder++ )
                  {
                     UInt_t uChanOrdered = fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ fvChanOrder[uChipFeeA][uChOrder];
                     fhFullCtEpJumpFeeAChSort[uChOrder]->Fill(
                           fvmLastHit[ uChanOrdered ].getGet4V10R32HitTs(),
                           fvmLastHit[ uChanOrdered].getGet4V10R32HitTs() +
                           fvmLastHit[ uChanOrdered ].getGet4V10R32HitTot() / static_cast<Int_t>(get4v1x::kdClockCycleSize/get4v1x::kdTotBinSize) );
                     fhFullCtEpJumpFeeAChOrder->Fill( uChOrder, fvChanOrder[uChipFeeA][uChOrder]);
                  } // for( UInt_t uChOrder = 0; uChOrder < fvChanOrder[uChipFeeA].size(); uChOrder++ )

               if( 0 < fvChanOrder[uChipFeeA].size() )
                  fhEpQualityFirstChFeeA[uChipFeeA]->Fill( fvChanOrder[uChipFeeA][0], 0 );
            } // if( kTRUE == fbPulserMode && fuPulserFee == (uChipFullId/kuNbChipFee) )
         } // if( fvuCurrEpoch2[uChipFullId] +1 != mess.getGet4V10R32EpochNumber())
            else
            {
               fhGet4BadEpochRatio->Fill(1);
               fhGet4BadEpRatioChip->Fill( uChipFullId, 1);
               if( kTRUE == fbPulserMode && fuPulserFee == (uChipFullId/kuNbChipFee) )
               {
                  UInt_t uChipFeeA = uChipFullId%kuNbChipFee;

                  for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )
                     if( 0ull != fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getData() )
                  {
                     fhFullCtEpQualityCh[uChan]->Fill(
                           fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTs(), 1);
                     fhFullCtEpQualityChZoom[uChan]->Fill(
                           fvmLastHit[ fuPulserFee * kuNbChanFee+ uChipFeeA*get4v1x::kuChanPerGet4+ uChan].getGet4V10R32HitTs(), 1);
                  } // for( UInt_t uChan = 0; uChan < get4v1x::kuChanPerGet4; uChan++ )

                  if( 0 < fvChanOrder[uChipFeeA].size() )
                     fhEpQualityFirstChFeeA[uChipFeeA]->Fill( fvChanOrder[uChipFeeA][0], 1 );
               } // if( kTRUE == fbPulserMode && fuPulserFee == (uChipFullId/kuNbChipFee) )
            } // else of if( fvuCurrEpoch2[uChipFullId] +1 != mess.getGet4V10R32EpochNumber())

         // Epoch counter overflow book keeping
         if( (get4v1x::kulGet4EpochCycleSz - 2) < fvuCurrEpoch2[uChipFullId] &&
             mess.getEpoch2Number() < 2  )
            fvuCurrEpoch2Cycle[uChipFullId]++;

         fvuCurrEpoch2[uChipFullId] = mess.getGet4V10R32EpochNumber();

         fvhRocEpochCntEvo[cRocId]->Fill(
                  ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                  *get4v1x::kdEpochInPs*(1e-12) ); // Time since start in s

         if( 1 == mess.getGet4V10R32SyncFlag() )
         {
            fhGet4EpochFlags->Fill(uChipFullId , 0);
            fhGet4EpochSyncDist->Fill( uChipFullId, fvuCurrEpoch2[uChipFullId] % get4v1x::kuSyncCycleSzGet4 );
         } // if( 1 == mess.getGet4V10R32SyncFlag() )

         // Fill Pulser test histos if needed
         // Accepted pairs are when both messages are defined and they are at most
         // 1 epoch apart => possibility of double use is the pulse happens on top of
         // an epoch and more than once every 3 epochs. For example:
         // HHHHEHHHH.......E......HHHHEHHHH leads to
         // (HHHHHHHH)             (HHHHHHHH) and
         //     (HHHH              HHHH)
         if( kTRUE == fbPulserMode && 0 == (uChipFullId%kuNbChipFee)
               && fuPulserFee == (uChipFullId/kuNbChipFee) )
         {
            // Fill the time difference for all channels pairs in
            // the chosen Fee
            UInt_t uHistoFeeIdx = 0;
            for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
            {
               for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
               {
                  if( ( 0xF0 <= fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA].getSysMesType() ) &&
                      ( 0xF0 <= fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeB].getSysMesType() ) &&
                      (   fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA ]
                        < fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeB ] + 2 ) &&
                      (   fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA ] + 2
                        > fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeB ] ) )
                  {
                     Double_t dTimeDiff = 0.0;
                     dTimeDiff = fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA].CalcGet4V10R32HitTimeDiff(
                           fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA],
                           fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeB],
                           fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeB] );

                     fhTimeResFee[uHistoFeeIdx]->Fill( dTimeDiff );
                  } // if both channels have matching data

                  if( kTRUE == fbPulserFeeB )
                     if(
                      ( 0xF0 <= fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeA].getSysMesType() ) &&
                      ( 0xF0 <= fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB].getSysMesType() ) &&
                      (   fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ]
                        < fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ] + 2 ) &&
                      (   fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeA ] + 2
                        > fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ] ) )
                  {
                     Double_t dTimeDiffB = 0.0;
                     dTimeDiffB = fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeA].CalcGet4V10R32HitTimeDiff(
                           fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeA],
                           fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB],
                           fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB] );

                     fhTimeResFeeB[uHistoFeeIdx]->Fill( dTimeDiffB );
                  } // if both channels  in FEE B have matching data
                  uHistoFeeIdx++;
               } // for any unique pair of channel in chosen Fee

               if( kTRUE == fbPulserFeeB )
                  for( UInt_t uChanFeeB = 0; uChanFeeB < kuNbChanFee; uChanFeeB++)
               {
                  if(
                      ( 0xF0 <= fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA].getSysMesType() ) &&
                      ( 0xF0 <= fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB].getSysMesType() ) &&
                      (   fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA ]
                        < fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ] + 2 ) &&
                      (   fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA ] + 2
                        > fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB ] ) )
                  {
                     Double_t dTimeDiffAB = 0.0;
                     dTimeDiffAB = fvmLastHit[ fuPulserFee * kuNbChanFee+ uChanFeeA].CalcGet4V10R32HitTimeDiff(
                           fvuLastHitEp[ fuPulserFee * kuNbChanFee+ uChanFeeA],
                           fvuLastHitEp[ fuPulserFeeB * kuNbChanFee+ uChanFeeB],
                           fvmLastHit[ fuPulserFeeB * kuNbChanFee+ uChanFeeB] );

                     fhTimeResFeeAB[uChanFeeA * kuNbChanFee + uChanFeeB]->Fill( dTimeDiffAB );
                  } // if both channels in FEE A and B have matching data
               } // if( kTRUE == fbPulserFeeB ) && for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
            } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)

            // Fill the time difference for the chosen channel pairs
            UInt_t uHistoCombiIdx = 0;
            for( UInt_t uChanA = 0; uChanA < kuNbChanTest-1; uChanA++)
            {
               if( ( 0xF0 <= fvmLastHit[ fuPulserChan[uChanA]   ].getSysMesType() ) &&
                   ( 0xF0 <= fvmLastHit[ fuPulserChan[uChanA+1] ].getSysMesType() ) &&
                   ( fvuLastHitEp[ fuPulserChan[uChanA]   ]
                         < fvuLastHitEp[ fuPulserChan[uChanA+1] ] + 2 ) &&
                   ( fvuLastHitEp[ fuPulserChan[uChanA]   ] + 2
                         > fvuLastHitEp[ fuPulserChan[uChanA+1] ]     ) )
               {
                  Double_t dTimeDiff =
                        fvmLastHit[   fuPulserChan[uChanA]   ].CalcGet4V10R32HitTimeDiff(
                        fvuLastHitEp[ fuPulserChan[uChanA]   ],
                        fvuLastHitEp[ fuPulserChan[uChanA+1] ],
                        fvmLastHit[   fuPulserChan[uChanA+1] ] );
                  fhTimeResPairs[uChanA]->Fill( dTimeDiff );
                  Double_t dTotA = fvmLastHit[   fuPulserChan[uChanA]   ].getGet4V10R32HitTot();
                  Double_t dTotB = fvmLastHit[   fuPulserChan[uChanA+1] ].getGet4V10R32HitTot();
                  fhTimeResPairsTot[2*uChanA]->Fill(   dTimeDiff, dTotA );
                  fhTimeResPairsTot[2*uChanA+1]->Fill( dTimeDiff, dTotB );
               } // // if both channels have data

               for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
               {
                  if( ( 0xF0 <= fvmLastHit[ fuPulserChan[uChanA] ].getSysMesType() ) &&
                      ( 0xF0 <= fvmLastHit[ fuPulserChan[uChanB] ].getSysMesType() ) &&
                      ( fvuLastHitEp[ fuPulserChan[uChanA] ]
                            < fvuLastHitEp[ fuPulserChan[uChanB] ] + 2 ) &&
                      ( fvuLastHitEp[ fuPulserChan[uChanA] ] + 2
                            > fvuLastHitEp[ fuPulserChan[uChanB] ]     ) )
                  {
                     Double_t dTimeDiff =
                           fvmLastHit[   fuPulserChan[uChanA] ].CalcGet4V10R32HitTimeDiff(
                           fvuLastHitEp[ fuPulserChan[uChanA] ],
                           fvuLastHitEp[ fuPulserChan[uChanB] ],
                           fvmLastHit[   fuPulserChan[uChanB] ] );
                     fhTimeResCombi[uHistoCombiIdx]->Fill( dTimeDiff );

                     if( ( fvmLastHit[ fuPulserChan[uChanA] ].getGet4V10R32HitFt() < (get4v1x::kuFineCounterSize/2) ) &&
                         ( fvmLastHit[ fuPulserChan[uChanB] ].getGet4V10R32HitFt() < (get4v1x::kuFineCounterSize/2) ) )
                        fhTimeResCombiEncA[uHistoCombiIdx]->Fill( dTimeDiff );
                     else if( ( (get4v1x::kuFineCounterSize/2) <= fvmLastHit[ fuPulserChan[uChanA] ].getGet4V10R32HitFt() ) &&
                              ( (get4v1x::kuFineCounterSize/2) <= fvmLastHit[ fuPulserChan[uChanB] ].getGet4V10R32HitFt() ) )
                        fhTimeResCombiEncB[uHistoCombiIdx]->Fill( dTimeDiff );

                     Double_t dTotA = fvmLastHit[   fuPulserChan[uChanA] ].getGet4V10R32HitTot();
                     Double_t dTotB = fvmLastHit[   fuPulserChan[uChanB] ].getGet4V10R32HitTot();
                     fhTimeResCombiTot[2*uHistoCombiIdx]->Fill(   dTimeDiff, dTotA );
                     fhTimeResCombiTot[2*uHistoCombiIdx+1]->Fill( dTimeDiff, dTotB );
                  } // if both channels have data
                  uHistoCombiIdx++;
               } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
            } // for( UInt_t uChanA = 0; uChanA < kuNbChanTest; uChanA++)

         } // if( kTRUE == fbPulserMode )

         // for channel readout order in case of epoch jumps
         if( kTRUE == fbPulserMode && fuPulserFee == (uChipFullId/kuNbChipFee) )
            fvChanOrder[uChipFullId%kuNbChipFee].clear();
         break;
      } // case get4v1x::GET4_32B_EPOCH
      case get4v1x::GET4_32B_SLCM:  // => Slow control
      {
         Double_t dFullChId =
               get4v1x::kuChanPerGet4*( uChipFullId )
               + mess.getGet4V10R32SlChan()
               + 0.5*mess.getGet4V10R32SlEdge();

         // As long as the 1st epoch with sync flag after SYNC DLM is found,
         // consider data as not good
         if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )
            return;

         fhGet4ChanSlowContM->Fill( dFullChId, mess.getGet4V10R32SlType() );

         switch( mess.getGet4V10R32SlType() )
         {
            case get4v1x::GET4_32B_SLC_SCALER:
            {
               fhGet4ChanEdgesCounts->Fill( dFullChId, mess.getGet4V10R32SlData() );
               break;
            }
            case get4v1x::GET4_32B_SLC_DEADT:
            {
               fhGet4ChanDeadtime->Fill( dFullChId, mess.getGet4V10R32SlData() );
               break;
            }
            case get4v1x::GET4_32B_SLC_SPIREAD:
            {
               break;
            }
            case get4v1x::GET4_32B_SLC_START_SEU:
            {
               // If Ch = 0 and Edge = 1 => SEU counter
               // Otherwise => Start message!
               if( 0 == mess.getGet4V10R32SlChan() &&
                   1 == mess.getGet4V10R32SlEdge() )
                  fhGet4SeuCounter->Fill( uChipFullId, mess.getGet4V10R32SlData() );
               break;
            }
            default:
            {
               break;
            }
         } // switch( mess.getGet4V10R32SlType() )
         break;
      } // case get4v1x::GET4_32B_SLCM
      case get4v1x::GET4_32B_ERROR: // => Error message
      {
         Double_t dFullChId =
               get4v1x::kuChanPerGet4*( uChipFullId )
               + mess.getGet4V10R32ErrorChan()
               + 0.5*mess.getGet4V10R32ErrorEdge();

         UInt_t   uErrorType = mess.getGet4V10R32ErrorData();

         // As long as the 1st epoch with sync flag after SYNC DLM is found,
         // consider data as not good
         if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )
            return;

         // If debug mode, print the message
         if( kTRUE == fbDebug )
         {
            LOG(info) <<Form(" TS  #%12llu ", fulTsNb);
            mess.printDataLog();
         } // if( kTRUE == fbDebug )

         // Count error messages per ROC
         fvhRocErrorCntEvo[cRocId]->Fill(
               ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                 ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                *get4v1x::kdEpochInPs )
               *(1e-12) ); // Time since start in s
         if( 0x11 <= uErrorType && uErrorType <= 0x13 )
            fvhRocTotErrorCntEvo[cRocId]->Fill(
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs )
                  *(1e-12) ); // Time since start in s

         fvhGet4ErrorCntEvo[uChipFullId]->Fill(
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs)
                  *(1e-12) ); // Time since start in s

         if( ( get4v1x::GET4_V1X_ERR_EP < uErrorType &&
               uErrorType < get4v1x::GET4_V1X_ERR_TOK_RING_ST ) ||
               get4v1x::GET4_V1X_ERR_TOT_OVERWRT == uErrorType ||
               get4v1x::GET4_V1X_ERR_TOT_RANGE == uErrorType )
            fvhGet4ChErrorCntEvo[uChipFullId]->Fill( mess.getGet4V10R32ErrorChan(),
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs)
                  *(1e-12) ); // Time since start in s

         if( uChipFullId < kuNbTdcMappedDet )
            if( kiTdcDetMap[uChipFullId] < kiNbDet )
               fvhDetErrorCntEvo[ kiTdcDetMap[uChipFullId] ]->Fill(
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs)
                  *(1e-12) ); // Time since start in s

         switch( uErrorType )
         {
            case get4v1x::GET4_V1X_ERR_READ_INIT:
               fhGet4ChanErrors->Fill( dFullChId, 0);
               LOG(info)<<"CbmGet4FastMonitor::MonitorMessage_Get4v1 => Readout Init!"
                        <<" ChipId: "<<Form("%02u", cChipId)
                        <<" RocId: " <<Form("%02u", cRocId)
                        <<" TS: " <<fulTsNb;
               break;
            case get4v1x::GET4_V1X_ERR_SYNC:
               fhGet4ChanErrors->Fill( dFullChId, 1);
               break;
            case get4v1x::GET4_V1X_ERR_EP_CNT_SYNC:
               fhGet4ChanErrors->Fill( dFullChId, 2);
               break;
            case get4v1x::GET4_V1X_ERR_EP:
               fhGet4ChanErrors->Fill( dFullChId, 3);
               break;
            case get4v1x::GET4_V1X_ERR_FIFO_WRITE:
               fhGet4ChanErrors->Fill( dFullChId, 4);
               break;
            case get4v1x::GET4_V1X_ERR_LOST_EVT:
               fhGet4ChanErrors->Fill( dFullChId, 5);
               break;
            case get4v1x::GET4_V1X_ERR_CHAN_STATE:
               fhGet4ChanErrors->Fill( dFullChId, 6);
               break;
            case get4v1x::GET4_V1X_ERR_TOK_RING_ST:
               fhGet4ChanErrors->Fill( dFullChId, 7);
               break;
            case get4v1x::GET4_V1X_ERR_TOKEN:
               fhGet4ChanErrors->Fill( dFullChId, 8);
               break;
            case get4v1x::GET4_V1X_ERR_READOUT_ERR:
               fhGet4ChanErrors->Fill( dFullChId, 9);
               break;
            case get4v1x::GET4_V1X_ERR_SPI:
               fhGet4ChanErrors->Fill( dFullChId, 10);
               break;
            case get4v1x::GET4_V1X_ERR_DLL_LOCK:
               fhGet4ChanErrors->Fill( dFullChId, 11);
               break;
            case get4v1x::GET4_V1X_ERR_DLL_RESET:
               fhGet4ChanErrors->Fill( dFullChId, 12);
               break;
            case get4v1x::GET4_V1X_ERR_TOT_OVERWRT:
               fhGet4ChanErrors->Fill( dFullChId, 13);
               fhGet4ChanTotOwErrorsProp->Fill( dFullChId, 100.0);
               break;
            case get4v1x::GET4_V1X_ERR_TOT_RANGE:
               fhGet4ChanErrors->Fill( dFullChId, 14);
               fhGet4ChanTotOrErrorsProp->Fill( dFullChId, 100.0);
               break;
            case get4v1x::GET4_V1X_ERR_EVT_DISCARD:
               fhGet4ChanErrors->Fill( dFullChId, 15);
               fhGet4ChanTotEdErrorsProp->Fill( dFullChId, 100.0);
               break;
            case get4v1x::GET4_V1X_ERR_UNKNOWN:
               fhGet4ChanErrors->Fill( dFullChId, 16);
               break;
            default: // Corrupt error or not yet supported error
               fhGet4ChanErrors->Fill( dFullChId, 17);
               break;
         } // switch( mess.getGet4V10R32ErrorData() )
         break;
      } // case get4v1x::GET4_32B_ERROR
      case get4v1x::GET4_32B_DATA:  // => Hit Data
      {
         UInt_t uFullChId =
               get4v1x::kuChanPerGet4*( uChipFullId )
               + mess.getGet4V10R32HitChan();

         // As long as the 1st epoch with sync flag after SYNC DLM is found,
         // consider data as not good
         if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )
            return;

         fvhRocDataCntEvo[cRocId]->Fill(
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs
                   + mess.getGet4V10R32HitTimeBin() *get4v1x::kdBinSize )
                  *(1e-12) ); // Time since start in s
         fvhGet4DataCntEvo[uChipFullId]->Fill(
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs
                   + mess.getGet4V10R32HitTimeBin() *get4v1x::kdBinSize )
                  *(1e-12) ); // Time since start in s
         fvhGet4ChDataCntEvo[uChipFullId]->Fill( mess.getGet4V10R32HitChan(),
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs
                   + mess.getGet4V10R32HitTimeBin() *get4v1x::kdBinSize )
                  *(1e-12) ); // Time since start in s
         if( uChipFullId < kuNbTdcMappedDet )
            if( kiTdcDetMap[uChipFullId] < kiNbDet )
               fvhDetDataCntEvo[ kiTdcDetMap[uChipFullId] ]->Fill(
                  ( ( static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
                    ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24) )
                   *get4v1x::kdEpochInPs
                   + mess.getGet4V10R32HitTimeBin() *get4v1x::kdBinSize )
                  *(1e-12) ); // Time since start in s

         fhGet4ChanDataCount->Fill( uFullChId );
         fhGet4ChanDllStatus->Fill( uFullChId, mess.getGet4V10R32HitDllFlag() );
         fhGet4ChanTotMap->Fill(    uFullChId, mess.getGet4V10R32HitTot()
                                               *get4v1x::kdTotBinSize/1000.0 );

         if( kTRUE == fbEnaCalibOutput )
            fvhFineTime[ uChipFullId ][ mess.getGet4V10R32HitChan() ]->Fill( mess.getGet4V10R32HitFt() );

         if( kTRUE == fbPulserMode )
         {
            // First fill time interval histos
            if( fuPulserFee == (uFullChId/kuNbChanFee) )
            {
               Double_t dHitsDt = mess.CalcGet4V10R32HitTimeDiff(
                           fvuCurrEpoch2[uChipFullId],
                           fvuLastHitEp[ uFullChId ],
                           fvmLastHit[   uFullChId ] );
               if( 0 == fvuLastHitEp[uFullChId])
                  {}
               else if( dHitsDt < 1e6 )
                  fhPulserHitDistNs->Fill( uFullChId%kuNbChanFee, dHitsDt / 1e3 );
               else if( dHitsDt < 1e9)
                  fhPulserHitDistUs->Fill( uFullChId%kuNbChanFee, dHitsDt / 1e6 );
               else
                  fhPulserHitDistMs->Fill( uFullChId%kuNbChanFee, dHitsDt / 1e9 );

               if( 0 < fvuLastHitEp[uFullChId] && dHitsDt < 5e3 )
                  LOG(debug) <<uFullChId<<" "<<dHitsDt<<" "
                      <<fvuLastHitEp[ uFullChId ]<<" "<<fvuCurrEpoch2[uChipFullId]<<" "
                      <<fvmLastHit[   uFullChId ].getGet4V10R32HitTot()*get4v1x::kdTotBinSize/1000.0<<" "
                      <<mess.getGet4V10R32HitTot()*get4v1x::kdTotBinSize/1000.0;;

               // Fill the DNL histos
               fhPulserFeeDnl->Fill( uFullChId%kuNbChanFee, mess.getGet4V10R32HitFt() );

               // Fill the CT histo
               fhPulserFeeDistCT[ uChipFullId%kuNbChipFee ]->Fill( mess.getGet4V10R32HitTs(), mess.getGet4V10R32HitChan());
            } // if( fuPulserFee == (uFullChId/kuNbChanFee) )
         } // if( kTRUE == fbPulserMode )

         // Fill time coincidence map
         for( UInt_t uFeeA = 0; uFeeA < fuNbFee; uFeeA++)
            for( UInt_t uFeeChan = 0; uFeeChan < get4v1x::kuChanPerFee; uFeeChan++ )
               if( ( 0xF0 <= fvmLastHit[ uFeeA*get4v1x::kuChanPerFee + uFeeChan ].getSysMesType() ) &&
                   ( fvuLastHitEp[ uFeeA*get4v1x::kuChanPerFee + uFeeChan/get4v1x::kuChanPerGet4 ]
                         < fvuCurrEpoch2[uChipFullId] + 2 ) &&
                   ( fvuLastHitEp[ uFeeA*get4v1x::kuChanPerFee + uFeeChan/get4v1x::kuChanPerGet4 ] + 2
                         > fvuCurrEpoch2[uChipFullId]     ) )
            {
               // compare to last hit in other channekl
               Double_t dHitsDt = mess.CalcGet4V10R32HitTimeDiff(
                           fvuCurrEpoch2[uChipFullId],
                           fvuLastHitEp[ uFeeA*get4v1x::kuChanPerFee + uFeeChan/get4v1x::kuChanPerGet4 ],
                           fvmLastHit[   uFeeA*get4v1x::kuChanPerFee + uFeeChan ] );
               if( TMath::Abs(dHitsDt) < fdMaxCoincDist  )
               {
                  if( uFeeA*get4v1x::kuChanPerFee + uFeeChan < uFullChId )
                     fvvhChannelsCoinc[uFeeA][uFullChId/get4v1x::kuChanPerFee]->Fill(
                        uFeeChan, uFullChId%get4v1x::kuChanPerFee);
                     else fvvhChannelsCoinc[uFullChId/get4v1x::kuChanPerFee][uFeeA]->Fill(
                           uFullChId%get4v1x::kuChanPerFee, uFeeChan);
               } // if( TMath::Abs(dHitsDt) < fdMaxCoincDist  )
            } // for each pair, if both hits valid

         // Multiple hits (reflections, ...) detection
         if( 1 == fiMode)
         {
            Double_t dHitsDt = mess.CalcGet4V10R32HitTimeDiff(
                        fvuCurrEpoch2[uChipFullId],
                        fvuLastHitEp[ uFullChId ],
                        fvmLastHit[   uFullChId ] );
            if( dHitsDt < fdMaxDtMultiHit )
            {
               if( kFALSE == fvbChanSecondHit[uChipFullId][mess.getGet4V10R32HitChan()] )
               {
                  fvhGet4MultipleHits[uChipFullId]->Fill( mess.getGet4V10R32HitChan(), 2);
                  fvhGet4DistDoubleHits[uChipFullId]->Fill( mess.getGet4V10R32HitChan(), dHitsDt);
                  fvbChanSecondHit[uChipFullId][mess.getGet4V10R32HitChan()] = kTRUE;

                  fvhGet4MultipleHitsVsTot[uChipFullId]->Fill(
                        fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()], 2);
               }
               else if ( kFALSE == fvbChanThirdHit[uChipFullId][mess.getGet4V10R32HitChan()] )
               {
                  fvhGet4MultipleHits[uChipFullId]->Fill( mess.getGet4V10R32HitChan(), 3);
                  fvhGet4DistTripleHits[uChipFullId]->Fill( mess.getGet4V10R32HitChan(), dHitsDt);
                  fvbChanThirdHit[uChipFullId][mess.getGet4V10R32HitChan()] = kTRUE;

                  fvhGet4MultipleHitsVsTot[uChipFullId]->Fill(
                        fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()], 3);
               }
               else
               {
                  fvhGet4MultipleHits[uChipFullId]->Fill( mess.getGet4V10R32HitChan(), 4);
                  fvhGet4DistMultipleHits[uChipFullId]->Fill( mess.getGet4V10R32HitChan(), dHitsDt);

                  fvhGet4MultipleHitsVsTot[uChipFullId]->Fill(
                        fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()], 4);
               }
            } // if( dHitsDt < fdMaxDtMultiHit )
               else
               {
                  fvhGet4MultipleHits[uChipFullId]->Fill( mess.getGet4V10R32HitChan(), 1);
                  fvbChanSecondHit[uChipFullId][mess.getGet4V10R32HitChan()] = kFALSE;
                  fvbChanThirdHit[uChipFullId][mess.getGet4V10R32HitChan()]  = kFALSE;

                  fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()] =
                        mess.getGet4V10R32HitTot() * get4v1x::kdTotBinSize/1000.0;
                  fvhGet4MultipleHitsVsTot[uChipFullId]->Fill(
                        fvdChanFirstHitTot[uChipFullId][mess.getGet4ChNum()], 1);
               } // else of if( dHitsDt < fdMaxDtMultiHit )
         } // if( 1 == fiMode)

         // Save the hit info in order to fill later the histos
         // Epoch of Last hit message (one per GET4 chip & channel)
         fvuLastHitEp[ uFullChId ] = fvuCurrEpoch2[uChipFullId];
         // Last hit message (one per GET4 chip & channel)
         fvmLastHit[ uFullChId ] = mess;

         // for the channel readout order in case of epoch jump
         if( kTRUE == fbPulserMode && fuPulserFee == (uChipFullId/kuNbChipFee))
            fvChanOrder[uChipFullId%kuNbChipFee].push_back( mess.getGet4V10R32HitChan() );

         fhGet4ChanTotOwErrorsProp->Fill( uFullChId, 0.0);
         fhGet4ChanTotOrErrorsProp->Fill( uFullChId, 0.0);
         fhGet4ChanTotEdErrorsProp->Fill( uFullChId, 0.0);

         break;
      } // case get4v1x::GET4_32B_DATA
   } // switch( cMessType )

}
//**********************************************/

Bool_t CbmGet4FastMonitor::WriteCalibrationFile()
{
   TTimeStamp timeCurrent;
   TDirectory* oldDir = gDirectory;
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

      UInt_t uNbChan  = get4v1x::kuChanPerGet4;

      if( 0 == uNbChan  )
      {
         LOG(info)<<"CbmGet4FastMonitor::WriteCalibrationFile Undefined tdc parameters for type ";
         LOG(info)<<"get4";
         LOG(info)<<" => No calib histo saving!";
         return kFALSE;
      }

      TString sInfoSaving = "Saved initial calibration histograms for following get4 channels:";
      LOG(info)<<sInfoSaving;

      sInfoSaving = "          ";
      for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
         sInfoSaving += Form("%3u ", uChanInd);
      LOG(info)<<sInfoSaving;

      for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4 ++)
//         if( kTRUE == fvbActiveChips[uGet4] ) // Calibration TDC indexing not compatible
      {
         sInfoSaving = Form("tdc #%3u: ",uGet4 );
         for( UInt_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
         {
            TString sCalibHistoOutputName = Form("tof_get4_ft_b%03u_ch%03u",
                                                   uGet4, uChanInd );

            // New Calibration histo using only new data
//            if( 0 < (fvhFineTime[ uGet4 ][ uChanInd ]->GetEntries() ) ) // Calibration TDC requires all channels there
            {
               fvhFineTime[ uGet4 ][ uChanInd ]->Write( sCalibHistoOutputName, TObject::kOverwrite);
               sInfoSaving += "  1 ";
            } // if( 0 < (fvhFineTime[ uGet4 ][ uChanInd ]->GetEntries() ) )
//               else sInfoSaving += "  0 ";
         } // for( Int_t uChanInd = 0; uChanInd< uNbChan; uChanInd++)
         LOG(info)<<sInfoSaving;
      } // for( Int_t uGet4 = 0; uGet4 < fuNbGet4; uGet4 ++)

      //fileCalibrationOut->Write("",TObject::kOverwrite);
      fileCalibrationOut->Close();
      LOG(info)<<"Calibration data saved in "<<sCalibOutFilename;
      LOG(info)<<"Parameter line: InitialCalibFilename: Text_t   "
               << (static_cast<TString>( sCalibOutFilename(0, sCalibOutFilename.Length() -2 ) ));
   } // if( kTRUE == fileCalibrationOut->IsOpen() )
      else LOG(warning)<<"CbmGet4FastMonitor::WriteCalibrationFile => Unable to open root file "
                        <<sCalibOutFilename
                        <<" to save calibration data, please check settings";
   gDirectory->Cd(oldDir->GetPath());

   return kTRUE;
}

ClassImp(CbmGet4FastMonitor)
