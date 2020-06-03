// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmTSUnpackGet4v1x                           -----
// -----                    Created 15.12.2014 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                  Based on CbmTSUnpackGet4v1x                       -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTSUnpackGet4v1x.h"

// Specific headers
#include "CbmGet4v1xDef.h"
#include "CbmGet4EpochBuffer.h"

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
 * TODO
 * 1) Use the time sorted data => ~OK
 * 2) Counts as function of time (s bin, per ch/chip/ROC/Det) => / OK / OK / OK
 * 3) Split class
 * 4) Time sorted data output => ~OK
 * 5) Monitor size of closed epochs => OK
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

CbmTSUnpackGet4v1x::CbmTSUnpackGet4v1x()
: CbmTSUnpack(),
  fbVerbose(kFALSE),
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
  fhGet4ChanDataCount(NULL),
  fhGet4ChanDllStatus(NULL),
  fhGet4ChanTotMap(NULL),
  fhGet4ChanErrors(NULL),
  fhGet4ChanSlowContM(NULL),
  fdEvoBinSizeS(kdEvoBinSizeS),
  fdEvoLengthSec(kdEvoLengthSec),
  fdMsLengthSec(kdMsSizeSec),
  fhMsSizeEvo(NULL),
  fvhRocDataCntEvo(),
  fvhRocEpochCntEvo(),
  fvhRocErrorCntEvo(),
  fvhRocDataLossCntEvo(),
  fvhRocEpochLossCntEvo(),
  fvhGet4DataCntEvo(),
  fvhGet4ErrorCntEvo(),
  fvhGet4DataLossCntEvo(),
  fvhGet4EpochLossCntEvo(),
  fvhGet4ChDataCntEvo(),
  fvhGet4ChErrorCntEvo(),
  fvhDetDataCntEvo(),
  fvhDetErrorCntEvo(),
  fvhDetDataLossCntEvo(),
  fvhDetEpochLossCntEvo(),
  fhClosedEpSizeEvo(NULL),
/*
  fbPulserMode(kFALSE),
  fuPulserFee(0),
  fuPulserChan(),
  fhTimeResFee(),
  fhTimeRmsAllFee(NULL),
  fhTimeResAllFee(NULL),
  fhTimeResPairs(),
  fhTimeResCombi(),
  fhPulserHitDistNs(NULL),
  fhPulserHitDistUs(NULL),
  fhPulserHitDistMs(NULL),
  fhPulserFeeDnl(NULL),
  fhPulserFeeInl(NULL),
  fbOldReadoutOk(kFALSE),
  fhGet4ChanTotCount(NULL),
  fhPulserFeeTotDnl(NULL),
  fhPulserFeeTotInl(NULL),
*/
  fvvhChannelsCoinc(),
  fdMaxCoincDist(100.0),
  fvuLastHitEp(),
  fvmLastHit(),
  fvuLastOldTotEp(),
  fvmLastOldTot(),
  fmsOrderedEpochsData(),
  fmsOrderedEpochsBuffStat(),
  fvuCurrEpochBuffer(),
  fvuCurrEpochBufferIt(),
  fuLastEpBufferReady(0),
  fuCurrEpReadBuffer(0),
  fEpochBuffer(NULL),
  fbEnaCalibOutput(kFALSE),
  fsCalibOutFoldername(""),
  fsCalibFilename(""),
  fvhFineTime()
{
}

CbmTSUnpackGet4v1x::~CbmTSUnpackGet4v1x()
{
}

Bool_t CbmTSUnpackGet4v1x::Init()
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
     LOG(error) << "CbmTSUnpackGet4v1x::Init => Nb of ROCs or GET4s not defined!!! ";
     LOG(error) << "Nb of ROCs:"<< fuNbRocs << " Nb of GET4s : " << fuNbGet4;
     LOG(fatal) << "Please use the functions SetRocNb and/or SetGet4Nb before running!!";
  }

  // Initialize TS counter
  fulTsNb = 0;

  // At start all ROCs and GET4 are considered "unsynchronized"
  // Stay so until the DLM 10 is issued: ~/flesnet/build/sync_frontend
  // The GET4 are synchronized after reception of 1st epoch2 with SYNC
  fvbRocFeetSyncStart.resize( fuNbRocs);
  for( UInt_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
     fvbRocFeetSyncStart[uRoc] = kFALSE;
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
  fvuCurrEpochBuffer.resize(fuNbGet4);
  fvuCurrEpochBufferIt.resize(fuNbGet4);
  for( UInt_t uChip = 0; uChip < fuNbGet4; uChip++)
  {
     fvuCurrEpoch2[uChip] = 0;
     fvuCurrEpoch2Cycle[uChip] = 0;
     fvuCurrEpochBuffer[uChip] = 0;
     fvuCurrEpochBufferIt[uChip] = fmsOrderedEpochsData.end();
  } // for( UInt_t uChip = 0; uChip < fuNbGet4; uChip++)

  LOG(info) << "Initializing Histos";
  InitMonitorHistograms();
  LOG(info) << "Initialization done";

  if( 2 == fiMode )
  {
     LOG(info) << "Initializing Epoch buffer";
     fEpochBuffer = CbmGet4EpochBuffer::Instance();
     if( NULL == fEpochBuffer )
        LOG(fatal) << "Failed to Obtain the epoch buffer instance!";
        else LOG(info) << "Initialization done";
  } // if( 2 == fiMode )

  return kTRUE;
}

void CbmTSUnpackGet4v1x::SetActiveGet4( UInt_t uChipsIndex, Bool_t bActiveFlag)
{
   if( 0 == fvbActiveChips.size() && 0 < fuNbGet4 )
   {
      fvbActiveChips.resize(fuNbGet4);
      for( UInt_t uChip = 0; uChip < fuNbGet4; uChip++)
         fvbActiveChips[uChip] = kTRUE;
   } // if( 0 == fvbActiveChips.size()

   if( uChipsIndex < fuNbGet4 )
      fvbActiveChips[uChipsIndex] = bActiveFlag;
      else LOG(error)<<" CbmTSUnpackGet4v1x::SetActiveGet4 => Invalid chip index "
                     << uChipsIndex;
}
/*
void CbmTSUnpackGet4v1x::SetPulserChans(
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
*/
void CbmTSUnpackGet4v1x::SetHistoFilename( TString sNameIn )
{
   fsHistoFilename = sNameIn;

   LOG(info)<<" CbmTSUnpackGet4v1x::SetHistoFilename => Histograms output file is now \n"
             <<fsHistoFilename;
}
Bool_t CbmTSUnpackGet4v1x::DoUnpack(const fles::Timeslice& ts, size_t component)
{
   fulTsNb++;
   if( 0 == fiMode || kTRUE == fbVerbose )
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
/*
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
*/
            case 2:
            {
               // Normal mode, unpack data and fill TClonesArray of CbmRawMessage
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

                        this->ProcessMessage_epoch(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_EPOCH:
                     case get4v1x::MSG_SYNC:
                     {
                        // Ignore all messages before RocFeet system SYNC
                        if (kFALSE == fvbRocFeetSyncStart[cur_DTM_header.ROC_ID] )
                           break;

                        this->ProcessMessage_sync(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_SYNC:
                     case get4v1x::MSG_AUX:
                     {
                        // Ignore all messages before RocFeet system SYNC
                        if (kFALSE == fvbRocFeetSyncStart[cur_DTM_header.ROC_ID] )
                           break;

                        this->ProcessMessage_aux(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_AUX:
                     case get4v1x::MSG_EPOCH2:
                     {
                        // Ignore all messages before RocFeet system SYNC
                        if (kFALSE == fvbRocFeetSyncStart[cur_DTM_header.ROC_ID] )
                           break;

                        this->ProcessMessage_epoch2(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_EPOCH2:
                     case get4v1x::MSG_GET4:
                     {
                        // Ignore all messages before RocFeet system SYNC
                        if (kFALSE == fvbRocFeetSyncStart[cur_DTM_header.ROC_ID] )
                           break;

                        this->ProcessMessage_get4(mess, msDescriptor.eq_id);
                        break;
                     } // case get4v1x::MSG_GET4:
                     case get4v1x::MSG_SYS:
                        this->ProcessMessage_sys(mess, msDescriptor.eq_id);
                        break;
                     default:
                        break;
                  } // switch( mess.getMessageType() )

                  local_offset += 6; // next message
               } // while (local_offset < packageSize)
               break;
            } // case 2
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

void CbmTSUnpackGet4v1x::FinishUnpack()
{
   // Something to do only in Processing mode
   // TODO: Use the stored data for something usefull !!!
   if( 2 == fiMode )
   {
      if( kFALSE == fvbGet4WaitFirstSync[0] )
      {
      LOG(debug)<<" CbmTSUnpackGet4v1x::FinishUnpack => Stored epochs: "
               <<fmsOrderedEpochsData.size()<<" "
               <<fmsOrderedEpochsBuffStat.size();
      UInt_t uNbClosedEpochs = 0;
//      Bool_t bOneEpochPrinted = kFALSE;
      auto itBuffer = fmsOrderedEpochsData.begin();
      for( auto itFlag = fmsOrderedEpochsBuffStat.begin();  itFlag!=fmsOrderedEpochsBuffStat.end(); ++itFlag )
      {
         if( kTRUE == itFlag->second )
         {
            uNbClosedEpochs++;
            LOG(debug)<<" CbmTSUnpackGet4v1x::FinishUnpack => Epochs: "<<uNbClosedEpochs
//                    <<" Size: "<< (itBuffer->second).size(); // OLD
            <<" Size: "<< (itBuffer->second)->size();

            fhClosedEpSizeEvo->Fill(
                  (itBuffer->first)*get4v1x::kdEpochInPs*(1e-12),
//                  (itBuffer->second).size() ); // OLD
                  (itBuffer->second)->size() );

            // Fill the closed epochs to the epoch buffer
            // We transmit only epoch with data
            // TODO: move this to the place where Epochs are "closed"
            if( 0 < (itBuffer->second)->size() )
               fEpochBuffer->InsertEpoch( itBuffer->second );

               // Use data for monitoring
/*
            if( kTRUE == fbPulserMode )
            {
               // Loop on data
//               for( auto itData = (itBuffer->second).begin(); itData != (itBuffer->second).end(); ++itData) // OLD
               for( auto itData = (itBuffer->second)->begin(); itData != (itBuffer->second)->end(); ++itData)
               {
                  get4v1x::FullMessage mess = *itData;
                  uint8_t  cRocId    = mess.getRocNumber();
                  uint8_t  cChipId   = mess.getGet4V10R32ChipId();
                  uint32_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;
//                  uint8_t  cMessType = mess.getGet4V10R32MessageType();
                  UInt_t uFullChId =
                     get4v1x::kuChanPerGet4*( uChipFullId )
                     + mess.getGet4V10R32HitChan();

                  // As long as the 1st epoch with sync flag after SYNC DLM is found,
                  // consider data as not good
                  if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )
                     return;

                  fhGet4ChanDataCount->Fill( uFullChId );
                  fhGet4ChanDllStatus->Fill( uFullChId, mess.getGet4V10R32HitDllFlag() );
                  fhGet4ChanTotMap->Fill(    uFullChId, mess.getGet4V10R32HitTot()
                                                        *get4v1x::kdTotBinSize/1000.0 );

                  // Save the hit info in order to fill later the pulser histos

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
                  } // if( fuPulserFee == (uFullChId/kuNbChanFee) )

                  // Epoch of Last hit message (one per GET4 chip & channel)
                  fvuLastHitEp[ uFullChId ] = itData->getExtendedEpoch();
                  // Last hit message (one per GET4 chip & channel)
                  fvmLastHit[ uFullChId ] = mess;
               } // for( auto itData = (itBuffer->second).begin(); itData != (itBuffer->second).begin(); ++itData)

               // Fill the time difference for all channels pairs in
               // the chosen Fee
               UInt_t uHistoFeeIdx = 0;
               for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
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
                     uHistoFeeIdx++;
                  } // for any unique pair of channel in chosen Fee

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
                     } // if both channels have data
                     uHistoCombiIdx++;
                  } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
               } // for( UInt_t uChanA = 0; uChanA < kuNbChanTest; uChanA++)
            } // if( kTRUE == fbPulserMode )
*/
         }
         /*
         if( kFALSE == bOneEpochPrinted && 0 < (itBuffer->second).size() )
         {
            for( auto itData = (itBuffer->second).begin(); itData != (itBuffer->second).end(); ++itData)
            {
               LOG(debug)<<" Epoch: "<< itData->getExtendedEpoch() <<" ";
               itData->printDataLog();
            } // for( auto itData = (itBuffer->second).begin(); itData != (itBuffer->second).begin(); ++itData)
            bOneEpochPrinted = kTRUE;
         } // if( kFALSE == bOneEpochPrinted && 0 < (itBuffer->second).size() )
         */
         ++itBuffer;
      } // for( auto itFlag = fmsOrderedEpochsBuffStat.begin();  itFlag!=fmsOrderedEpochsBuffStat.end(); ++itFlag )
      LOG(debug)<<" CbmTSUnpackGet4v1x::FinishUnpack => Closed epochs: "<<uNbClosedEpochs;
      } // if( kFALSE == fvbGet4WaitFirstSync[0] )


      auto itBuffer = fmsOrderedEpochsData.begin();
      for( auto itFlag = fmsOrderedEpochsBuffStat.begin();  itFlag!=fmsOrderedEpochsBuffStat.end(); ++itFlag )
      {
         if( kTRUE == itFlag->second )
         {
//            (itBuffer->second).clear(); // OLD, now done in Epoch buffer or Epoch consumer!!
            fmsOrderedEpochsData.erase( itBuffer );
            fmsOrderedEpochsBuffStat.erase( itFlag );
         } // if( kTRUE == itFlag->second )
         ++itBuffer;
      } // for( auto itFlag = fmsOrderedEpochsBuffStat.begin();  itFlag!=fmsOrderedEpochsBuffStat.end(); ++itFlag )
      if( 0 < fmsOrderedEpochsBuffStat.size() )
         fuLastEpBufferReady = (fmsOrderedEpochsBuffStat.begin())->first ;
         else fuLastEpBufferReady = 0;


      LOG(debug)<<" ++++++++++++ Ts # "<<fulTsNb<<" Done!";
   } // if( 2 == fiMode )
}

void CbmTSUnpackGet4v1x::Reset()
{
//  fGet4Raw->Clear();
//  fGet4RawSync->Clear();
}

void CbmTSUnpackGet4v1x::Finish()
{
   WriteMonitorHistograms();
   if( kTRUE == fbEnaCalibOutput )
      WriteCalibrationFile();
   DeleteMonitorHistograms();
}

/*
void CbmTSUnpackGet4v1x::Register()
{
}
*/
/*
void CbmTSUnpackGet4v1x::Print6bytesMessage(const uint8_t* msContent_shifted)
{
   if( kTRUE == fbVerbose )
      printf("0x%02X%02X%02X%02X%02X%02X :\t",
            msContent_shifted[0], msContent_shifted[1],
            msContent_shifted[2], msContent_shifted[3],
            msContent_shifted[4], msContent_shifted[5]);
}
*/

//************** Monitor functions *************/


void CbmTSUnpackGet4v1x::InitMonitorHistograms()
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

   fhGet4ChanSlowContM = new TH2I("hGet4ChanSlowContM",
         "Slow control messages per GET4 channel; GET4 channel # ; Type",
         fuNbGet4*get4v1x::kuChanPerGet4*2, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5,
         4, -0.5, 3.5);
   fhGet4ChanSlowContM->GetYaxis()->SetBinLabel(1,  "0: Scaler event            ");
   fhGet4ChanSlowContM->GetYaxis()->SetBinLabel(2,  "1: Dead time counter event ");
   fhGet4ChanSlowContM->GetYaxis()->SetBinLabel(3,  "2: SPI receiver data        ");
   fhGet4ChanSlowContM->GetYaxis()->SetBinLabel(4,  "3: Start message/Hamming Er.");

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

/*
   if( kTRUE == fbPulserMode )
   {
      // Full Fee test
      UInt_t uHistoFeeIdx =   0;
      UInt_t uNbBinsDt    = 500;
      Double_t dMinDt     = -1.*(uNbBinsDt*get4v1x::kdBinSize/2.) -get4v1x::kdBinSize/2.;
      Double_t dMaxDt     =  1.*(uNbBinsDt*get4v1x::kdBinSize/2.) +get4v1x::kdBinSize/2.;
      uNbBinsDt ++; // To account for extra bin due to shift by 1/2 bin of both ranges
      for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
         for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
         {
            fhTimeResFee[uHistoFeeIdx] = new TH1I(
                  Form("hTimeResFee_%03u_%03u", uChanFeeA, uChanFeeB),
                  Form("Time difference for channels %03u an %03u in chosen Fee; DeltaT [ps]; Counts",
                        uChanFeeA, uChanFeeB),
                  uNbBinsDt, dMinDt, dMaxDt);
            uHistoFeeIdx++;
         } // for any unique pair of channel in chosen Fee
      fhTimeRmsAllFee = new TH2D( "hTimeRmsAllFee",
            "Time difference RMS for any channels pair in chosen Fee; Ch A; Ch B",
            kuNbChanFee - 1, -0.5, kuNbChanFee - 1.5,
            kuNbChanFee - 1,  0.5, kuNbChanFee - 0.5);
      fhTimeResAllFee = new TH2D( "hTimeResAllFee",
            "Time resolution for any channels pair in chosen Fee; Ch A; Ch B",
            kuNbChanFee - 1, -0.5, kuNbChanFee - 1.5,
            kuNbChanFee - 1,  0.5, kuNbChanFee - 0.5);

      // Chosen channels test
      UInt_t uHistoCombiIdx = 0;
      for( UInt_t uChanA = 0; uChanA < kuNbChanTest-1; uChanA++)
      {
         fhTimeResPairs[uChanA]  = new TH1I(
               Form("hTimeResPairs_%03u_%03u", fuPulserChan[uChanA], fuPulserChan[uChanA+1]),
               Form("Time difference for selected channels %03u an %03u; DeltaT [ps]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanA+1]),
               uNbBinsDt, dMinDt, dMaxDt);
         for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
         {
            fhTimeResCombi[uHistoCombiIdx]  = new TH1I(
               Form("hTimeResCombi_%03u_%03u", fuPulserChan[uChanA], fuPulserChan[uChanB]),
               Form("Time difference for selected channels %03u an %03u; DeltaT [ps]; Counts",
                     fuPulserChan[uChanA], fuPulserChan[uChanB]),
               uNbBinsDt, dMinDt, dMaxDt);
            uHistoCombiIdx++;
         } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
      } // for( UInt_t uChanA = 0; uChanA < kuNbChanTest; uChanA++)

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
   } // if( kTRUE == fbPulserMode )

   if( kTRUE == fbOldReadoutOk )
   {
      fhGet4ChanTotCount = new TH1I("hGet4ChanTotCount",
            "Data Messages per GET4 TOT channel; GET4 channel # ; Data Count",
            fuNbGet4*get4v1x::kuChanPerGet4, -0.5, fuNbGet4*get4v1x::kuChanPerGet4 -0.5);

      fhPulserFeeTotDnl = new TH2D( "fhPulserFeeTotDnl",
            "DNL for all TOT channels in chosen FEE board; Chan # ; FT Bin; DNL [bin]",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);

      fhPulserFeeTotInl = new TH2D( "hPulserFeeTotInl",
            "INL for all TOT channels in chosen FEE board; Chan # ; FT Bin; INL [bin]",
            kuNbChanFee, -0.5, kuNbChanFee - 0.5,
            get4v1x::kuFineTime+1 ,  -0.5, get4v1x::kuFineTime + 0.5);
   } // if( kTRUE == fbOldReadoutOk )
*/
   // Prepare the vector storing the hit data for time diff calculation
   fvuLastHitEp.resize( fuNbGet4 * get4v1x::kuChanPerGet4);
   fvmLastHit.resize(   fuNbGet4 * get4v1x::kuChanPerGet4);
//   if( kTRUE == fbOldReadoutOk )
   {
      fvuLastOldTotEp.resize(   fuNbGet4 * get4v1x::kuChanPerGet4);
      fvmLastOldTot.resize(   fuNbGet4 * get4v1x::kuChanPerGet4);
   } // if( kTRUE == fbOldReadoutOk )

   // Now clear the hits
   for( UInt_t uChan = 0; uChan < fuNbGet4 * get4v1x::kuChanPerGet4; uChan++)
   {
      fvuLastHitEp[uChan] = 0;
      fvmLastHit[uChan].reset();
//      if( kTRUE == fbOldReadoutOk )
      {
         fvuLastOldTotEp[uChan] = 0;
         fvmLastOldTot[uChan].reset();
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
/*
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
                  10, 0.0, 10.0,
                  4, 0.5, 4.5 );
            fvdChanFirstHitTot[uGet4].resize(get4v1x::kuChanPerGet4, 0.0);
         } // if( kTRUE == fvbActiveChips[uGet4] )
      } // for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
   } // if( 1 == fiMode )
*/
}
void CbmTSUnpackGet4v1x::FillMonitorHistograms()
{
}
void CbmTSUnpackGet4v1x::WriteMonitorHistograms()
{
   TDirectory * oldir = gDirectory;
   TFile *fHist;
   fHist = new TFile( fsHistoFilename,"RECREATE");

   fHist->cd();

   fhMessageTypePerRoc->Write();
   fhRocSyncTypePerRoc->Write();
   fhRocAuxTypePerRoc ->Write();
   fhSysMessTypePerRoc->Write();
   fhMessTypePerGet4  ->Write();
   fhGet4EpochFlags   ->Write();
   fhGet4EpochSyncDist->Write();
   fhGet4EpochJumps   ->Write();
   fhGet4ChanDataCount->Write();
   fhGet4ChanDllStatus->Write();
   fhGet4ChanTotMap   ->Write();
   fhGet4ChanErrors   ->Write();
   fhGet4ChanSlowContM->Write();

   fhMsSizeEvo->Write();
   for( UInt_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
   {
      // FIXME: problem with scale function: on at least 1 computer it acts as if the
      // "width" option was given!!!!!!!!
      fvhRocDataCntEvo[uRoc]     ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocEpochCntEvo[uRoc]    ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocErrorCntEvo[uRoc]    ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocDataLossCntEvo[uRoc] ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocEpochLossCntEvo[uRoc]->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhRocDataCntEvo[uRoc]     ->Write();
      fvhRocEpochCntEvo[uRoc]    ->Write();
      fvhRocErrorCntEvo[uRoc]    ->Write();
      fvhRocDataLossCntEvo[uRoc] ->Write();
      fvhRocEpochLossCntEvo[uRoc]->Write();
   } // for( Int_t uRoc = 0; uRoc < fuNbRocs; uRoc++)
         // GET4
   for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++)
      if( kTRUE == fvbActiveChips[uGet4] )
      {
         fvhGet4DataCntEvo[uGet4]     ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4ErrorCntEvo[uGet4]    ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4DataLossCntEvo[uGet4] ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4EpochLossCntEvo[uGet4]->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4ChDataCntEvo[uGet4]   ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4ChErrorCntEvo[uGet4]  ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
         fvhGet4DataCntEvo[uGet4]     ->Write();
         fvhGet4ErrorCntEvo[uGet4]    ->Write();
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
      fvhDetDataLossCntEvo[iDet] ->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhDetEpochLossCntEvo[iDet]->Scale( 1/fdEvoBinSizeS ); // Weight factor to account for bin size in s
      fvhDetDataCntEvo[iDet]     ->Write();
      fvhDetErrorCntEvo[iDet]    ->Write();
      fvhDetDataLossCntEvo[iDet] ->Write();
      fvhDetEpochLossCntEvo[iDet]->Write();
   } // for( Int_t iDet = 0; iDet < kiNbDet; iDet++)
   fhClosedEpSizeEvo->Write();
/*
   if( kTRUE == fbPulserMode )
   {
      // First make a gauss fit to obtain the time resolution data
      // for all Fee channels pairs
      UInt_t uHistoFeeIdx = 0;
      TF1 *fitFunc[kuNbChanFee*(kuNbChanFee-1)/2];
      for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
         for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < kuNbChanFee; uChanFeeB++)
         {
            Double_t dRes = 0.0;

            // No need to fit if not data in histo
            if( 0 == fhTimeResFee[uHistoFeeIdx]->Integral() )
            {

               LOG(info)<<" FMC histo empty: "<<uHistoFeeIdx<<" "
                     <<uChanFeeA<<" "<<uChanFeeB;
               uHistoFeeIdx++;
               continue;
            } //  if( 0 == fhTimeResFee[uHistoFeeIdx]->Integral() )

            fitFunc[uHistoFeeIdx] = new TF1( Form("f_%03d_%03d",uChanFeeA,uChanFeeB), "gaus",
                  fhTimeResFee[uHistoFeeIdx]->GetMean() - 5*fhTimeResFee[uHistoFeeIdx]->GetRMS() ,
                  fhTimeResFee[uHistoFeeIdx]->GetMean() + 5*fhTimeResFee[uHistoFeeIdx]->GetRMS());

            fhTimeResFee[uHistoFeeIdx]->Fit( Form("f_%03d_%03d",uChanFeeA,uChanFeeB), "IQRM0");

            dRes = fitFunc[uHistoFeeIdx]->GetParameter(2);

            // If needed uncomment for debugging
            //(WARNING: this adds 1024 histos to the file!)
            fhTimeResFee[uHistoFeeIdx]->Write();

            delete fitFunc[uHistoFeeIdx];

            fhTimeRmsAllFee->Fill(uChanFeeA, uChanFeeB, fhTimeResFee[uHistoFeeIdx]->GetRMS() );
            fhTimeResAllFee->Fill(uChanFeeA, uChanFeeB, dRes/TMath::Sqrt2() );
//            fhTimeResAllFee->Fill(uChanFeeA, uChanFeeB, dRes/TMath::Sqrt2() );

            uHistoFeeIdx++;
         } // for any unique pair of channel in chosen Fee

      // Then write the Fee summary histo
      fhTimeRmsAllFee->Write();
      fhTimeResAllFee->Write();

      // Write the histos for the test on chosen channel pairs
      UInt_t uHistoCombiIdx = 0;
      for( UInt_t uChanA = 0; uChanA < kuNbChanTest-1; uChanA++)
      {
         fhTimeResPairs[uChanA]->Write();
         for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
         {
            fhTimeResCombi[uHistoCombiIdx]->Write();
            uHistoCombiIdx++;
         } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
      } // for( UInt_t uChanA = 0; uChanA < kuNbChanTest; uChanA++)

      // Write the hists interval histos
      fhPulserHitDistNs->Write();
      fhPulserHitDistUs->Write();
      fhPulserHitDistMs->Write();

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


      if( kTRUE == fbOldReadoutOk )
      {
         fhGet4ChanTotCount->Write();

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
      } // if( kTRUE == fbOldReadoutOk )
   } // if( kTRUE == fbPulserMode )
*/
   for( UInt_t uFeeA = 0; uFeeA < fuNbFee; uFeeA++)
      for( UInt_t uFeeB = uFeeA; uFeeB < fuNbFee; uFeeB++)
         fvvhChannelsCoinc[uFeeA][uFeeB]->Write();
/*
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
*/
   gDirectory->cd( oldir->GetPath() );
   fHist->Close();
}
void CbmTSUnpackGet4v1x::DeleteMonitorHistograms()
{

}
/*
void CbmTSUnpackGet4v1x::MonitorMessage_epoch(  get4v1x::Message mess, uint16_t EqID)
{
   // ROC epoch message: 250 MHz clock for now
   // TODO: check compatibility when ROC clock changed to 200 MHz
   uint8_t  cRocId    = mess.getRocNumber();
   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_epoch => ROC Epoch: EqId "
               <<EqID<<" roc "
               <<cRocId;

   // Epoch counter overflow book keeping
   if( (get4v1x::kulMainEpochCycleSz - 2) < fvuCurrEpoch[cRocId] &&
       mess.getEpochNumber() < 2  )
      fvuCurrEpochCycle[cRocId]++;

   fvuCurrEpoch[cRocId] = mess.getEpochNumber();
}
void CbmTSUnpackGet4v1x::MonitorMessage_sync(   get4v1x::Message mess, uint16_t EqID)
{
   // ROC sync message: TS in 250 MHz clock for now
   // TODO: check compatibility when ROC clock changed to 200 MHz
   uint8_t  cRocId    = mess.getRocNumber();
   fhRocSyncTypePerRoc->Fill( cRocId, mess.getSyncChNum() );
   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_sync => ROC Sync: EqId "
               <<EqID<<" roc "
               <<cRocId;
}
void CbmTSUnpackGet4v1x::MonitorMessage_aux(    get4v1x::Message mess, uint16_t EqID)
{
   // ROC aux message: TS in 250 MHz clock for now
   // TODO: check compatibility when ROC clock changed to 200 MHz
   uint8_t  cRocId    = mess.getRocNumber();
   fhRocAuxTypePerRoc->Fill( cRocId, mess.getAuxChNum() );
   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_aux => ROC Aux: EqId "
               <<EqID<<" roc "
               <<cRocId;
}
void CbmTSUnpackGet4v1x::MonitorMessage_epoch2( get4v1x::Message mess, uint16_t EqID)
{
   // GET4 v1.x epoch message (24b only for now)
   // TODO: check compatibility when 32b format without hack ready
   uint8_t  cRocId    = mess.getRocNumber();
   uint8_t  cChipId   = mess.getEpoch2ChipNumber();
   uint32_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;

   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_epoch2 => GET4 Epoch2: EqId "
               <<EqID<<" roc "
               <<cRocId<<" chip "
               <<cChipId;

   // Reject data of chips above defined limit to avoid overflow/seg fault
   if( fuNbGet4 <= uChipFullId )
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_epoch2 => Ignored message with ChipId above limit!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId
                  <<" Limit: " <<fuNbGet4;
      return;
   } // if( fuNbGet4 <= cChipId + kuMaxGet4PerRoc*mess.getRocNumber() )

   // Reject data of Inactive chips
   if( kFALSE == fvbActiveChips[uChipFullId])
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_epoch2 => Ignored message with Inactive Chip!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId;
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
                   <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
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
      LOG(debug) << "Epoch nb jump in chip "
                 <<Form("%3u: ", uChipFullId)
                 << Form(" %3d (%6u -> %6u)", iEpJump, fvuCurrEpoch2[uChipFullId],
                                         mess.getEpoch2Number() )
                 <<Form(" TS #%12llu", fulTsNb)
                 <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
                 <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                 <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);

      fhGet4EpochJumps->Fill(uChipFullId, iEpJump);
   } // if( fvuCurrEpoch2[uChipFullId] +1 != mess.getGet4V10R32EpochNumber())

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
/ *
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
* /

   if( 1 == mess.getEpoch2Sync() )
   {
      fhGet4EpochFlags->Fill(uChipFullId, 0);
      fhGet4EpochSyncDist->Fill( uChipFullId, fvuCurrEpoch2[uChipFullId] % get4v1x::kuSyncCycleSzGet4 );
   } // if( 1 == mess.getEpoch2Sync() )

   if( 1 == mess.getEpoch2EpochMissmatch() )
   {
      fhGet4EpochFlags->Fill(uChipFullId, 1);
      LOG(debug) << "Epoch missmatch in chip "
                <<Form("%3u: ", uChipFullId)
                <<Form(" EP #%6u", mess.getEpoch2Number() )
                <<Form(" TS #%12llu", fulTsNb)
                <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
                <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
      // TODO: evlution plot
   } // if( 1 == mess.getEpoch2EpochMissmatch() )
   if( 1 == mess.getEpoch2EpochLost() )
   {
      fhGet4EpochFlags->Fill(uChipFullId, 2);
      LOG(debug) << "Epoch loss in chip      "
                <<Form("%3u: ", uChipFullId)
                <<Form(" EP #%6u", mess.getEpoch2Number() )
                <<Form(" TS #%12llu", fulTsNb)
                <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
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
      LOG(debug) << "Data Loss in chip       "
                <<Form("%3u: ", uChipFullId)
                <<Form(" EP #%6u", mess.getEpoch2Number() )
                <<Form(" TS #%12llu", fulTsNb)
                <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
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
   if( kTRUE == fbPulserMode && 0 == uChipFullId && kTRUE == fbOldReadoutOk )
   {
      // Fill the time difference for all channels pairs in
      // the chosen Fee
      UInt_t uHistoFeeIdx = 0;
      for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
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
            } // if both channels have matching data
            uHistoFeeIdx++;
         } // for any unique pair of channel in chosen Fee

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
            } // if both channels have data
            uHistoCombiIdx++;
         } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
      } // for( UInt_t uChanA = 0; uChanA < kuNbChanTest; uChanA++)

   } // if( kTRUE == fbPulserMode && 0 == uChipFullId && kTRUE == fbOldReadoutOk )

}
void CbmTSUnpackGet4v1x::MonitorMessage_get4(   get4v1x::Message mess, uint16_t EqID)
{
   // GET4 v1.x data message (24b only for now)
   // TODO: check compatibility when 32b format without hack ready
   uint8_t  cRocId    = mess.getRocNumber();
   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_get4 => GET4 Data: EqId "
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
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_get4 => Ignored message with ChipId above limit!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId
                  <<" Limit: " <<fuNbGet4;
      return;
   } // if( fuNbGet4 <= cChipId + kuMaxGet4PerRoc*mess.getRocNumber() )

   // Reject data of Inactive chips
   if( kFALSE == fvbActiveChips[uChipFullId])
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_get4 => Ignored message with Inactive Chip!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId;
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
            // Fill the DNL histos
            fhPulserFeeTotDnl->Fill( uFullChId%kuNbChanFee, uTimeStamp & get4v1x::kuFineTime );
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
            } // if( fuPulserFee == (uFullChId/kuNbChanFee) )
         } // else of if( 1 == mess.getGet4Edge() )
   } // if( kTRUE == fbPulserMode && kTRUE == fbOldReadoutOk )

   if( kTRUE == fbOldReadoutOk )
   {
      if( 1 == mess.getGet4Edge() )
      {
         fhGet4ChanTotCount->Fill( uFullChId );
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

         // Epoch of Last hit message (one per GET4 chip & channel)
         fvuLastHitEp[ uFullChId ] = fvuCurrEpoch2[uChipFullId];
         // Last hit message (one per GET4 chip & channel)
         fvmLastHit[ uFullChId ] = mess;
      } // else of if( 1 == mess.getGet4Edge() )
   } // if( kTRUE == fbOldReadoutOk )
}
void CbmTSUnpackGet4v1x::MonitorMessage_sys(    get4v1x::Message mess, uint16_t EqID)
{
   uint8_t  cRocId    = mess.getRocNumber();
   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_sys => System message: EqId "
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
            LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_sys => SYNC DLM!"
                     <<" RocId: " << Form("%02u", cRocId)
                     <<Form(" TS  #%12llu", fulTsNb)
                     <<Form(" MS  #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
                     <<Form(" OF  #%5u/%5u", fuOffset, fuMaxOffset)
                     <<Form(" LO  #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
            fvbRocFeetSyncStart[cRocId] = kTRUE;
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
            LOG(warning)<<"CbmTSUnpackGet4v1x::MonitorMessage_sys => Unknown sysMess type ignored: "
                        <<mess.getSysMesType();
         break;
      } // default:
   } // switch( mess.getSysMesType() )
}
void CbmTSUnpackGet4v1x::MonitorMessage_Get4v1( get4v1x::Message mess, uint16_t EqID)
{
   // GET4 v1.x 32b raw message using hack
   uint8_t  cRocId    = mess.getRocNumber();
   uint8_t  cChipId   = mess.getGet4V10R32ChipId();
   uint32_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;
   uint8_t  cMessType = mess.getGet4V10R32MessageType();

   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_Get4v1 => 32b GET4 message: EqId "
               <<EqID<<" roc "
               <<cRocId;

   // Reject data of chips above defined limit to avoid overflow/seg fault
   if( fuNbGet4 <= uChipFullId )
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_Get4v1 => Ignored message with ChipId above limit!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId
                  <<" Limit: " <<fuNbGet4;
      return;
   } // if( fuNbGet4 <= cChipId + kuMaxGet4PerRoc*mess.getRocNumber() )

   // Reject data of Inactive chips
   if( kFALSE == fvbActiveChips[uChipFullId])
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_Get4v1 => Ignored message with Inactive Chip!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId;
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
                         <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
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
               LOG(debug) << "Epoch nb jump in chip "
                       <<Form("%3u: ", uChipFullId)
                       << Form(" %3d (%6u -> %6u)", iEpJump, fvuCurrEpoch2[uChipFullId],
                                                    mess.getGet4V10R32EpochNumber() )
                       <<Form(" TS  #%12llu", fulTsNb)
                       <<Form(" MS  #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
                       <<Form(" OF  #%5u/%5u", fuOffset, fuMaxOffset)
                       <<Form(" LO  #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
            else LOG(info) << "Epoch nb jump in chip "
                       <<Form("%3u: ", uChipFullId)
                       << Form(" %3d (%6u -> %6u)", iEpJump, fvuCurrEpoch2[uChipFullId],
                                                    mess.getGet4V10R32EpochNumber() )
                       <<Form(" TS  #%12llu", fulTsNb)
                       <<Form(" MS  #%5llu/%5lu", fulMsNb, fsMaxMsNb)
                       <<Form(" OF  #%5u/%5u", fuOffset, fuMaxOffset)
                       <<Form(" LO  #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);

            fhGet4EpochJumps->Fill(uChipFullId, iEpJump);
         } // if( fvuCurrEpoch2[uChipFullId] +1 != mess.getGet4V10R32EpochNumber())

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
         if( kTRUE == fbPulserMode && 0 == uChipFullId )
         {
            // Fill the time difference for all channels pairs in
            // the chosen Fee
            UInt_t uHistoFeeIdx = 0;
            for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)
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
                  uHistoFeeIdx++;
               } // for any unique pair of channel in chosen Fee

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
                  } // if both channels have data
                  uHistoCombiIdx++;
               } // for( UInt_t uChanB = uChanA+1; uChanB < kuNbChanComb; uChanB++)
            } // for( UInt_t uChanA = 0; uChanA < kuNbChanTest; uChanA++)

         } // if( kTRUE == fbPulserMode )
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

         // Count error messages per ROC
         fvhRocErrorCntEvo[cRocId]->Fill(
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
               LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_Get4v1 => Readout Init!"
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
               break;
            case get4v1x::GET4_V1X_ERR_TOT_RANGE:
               fhGet4ChanErrors->Fill( dFullChId, 14);
               break;
            case get4v1x::GET4_V1X_ERR_EVT_DISCARD:
               fhGet4ChanErrors->Fill( dFullChId, 15);
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

         break;
      } // case get4v1x::GET4_32B_DATA
   } // switch( cMessType )

}
*/
//**********************************************/

//************ Processing functions ************/
void CbmTSUnpackGet4v1x::ProcessMessage_epoch(  get4v1x::Message mess, uint16_t EqID)
{
   // ROC epoch message: 250 MHz clock for now
   // TODO: check compatibility when ROC clock changed to 200 MHz
   uint8_t  cRocId    = mess.getRocNumber();
   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_epoch => ROC Epoch: EqId "
               <<EqID<<" roc "
               <<cRocId;

   // Epoch counter overflow book keeping
   if( (get4v1x::kulMainEpochCycleSz - 2) < fvuCurrEpoch[cRocId] &&
       mess.getEpochNumber() < 2  )
      fvuCurrEpochCycle[cRocId]++;

   fvuCurrEpoch[cRocId] = mess.getEpochNumber();
}
void CbmTSUnpackGet4v1x::ProcessMessage_sync(   get4v1x::Message mess, uint16_t EqID)
{
   // ROC sync message: TS in 250 MHz clock for now
   // TODO: check compatibility when ROC clock changed to 200 MHz
   uint8_t  cRocId    = mess.getRocNumber();
   fhRocSyncTypePerRoc->Fill( cRocId, mess.getSyncChNum() );

   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_sync => ROC Sync: EqId "
               <<EqID<<" roc "
               <<cRocId;
}
void CbmTSUnpackGet4v1x::ProcessMessage_aux(    get4v1x::Message mess, uint16_t EqID)
{
   // ROC aux message: TS in 250 MHz clock for now
   // TODO: check compatibility when ROC clock changed to 200 MHz
   uint8_t  cRocId    = mess.getRocNumber();
   fhRocSyncTypePerRoc->Fill( cRocId, mess.getSyncChNum() );

   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_aux => ROC Aux: EqId "
               <<EqID<<" roc "
               <<cRocId;
}
void CbmTSUnpackGet4v1x::ProcessMessage_epoch2( get4v1x::Message mess, uint16_t EqID)
{
   // GET4 v1.x epoch message (24b only for now)
   // TODO: check compatibility when 32b format without hack ready
   uint8_t  cRocId    = mess.getRocNumber();
   uint8_t  cChipId   = mess.getEpoch2ChipNumber();
   uint32_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;

   if( kTRUE == fbVerbose )
      LOG(info)<<"CbmTSUnpackGet4v1x::ProcessMessage_epoch2 => Found a GET4 24b message: ignore it";

   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_epoch2 => GET4 Epoch2: EqId "
               <<EqID<<" roc "
               <<cRocId;

   // Reject data of chips above defined limit to avoid overflow/seg fault
   if( fuNbGet4 <= uChipFullId )
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_epoch2 => Ignored message with ChipId above limit!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId
                  <<" Limit: " <<fuNbGet4;
      return;
   } // if( fuNbGet4 <= cChipId + kuMaxGet4PerRoc*mess.getRocNumber() )

   // Reject data of Inactive chips
   if( kFALSE == fvbActiveChips[uChipFullId])
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_epoch2 => Ignored message with Inactive Chip!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId;
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
      LOG(info) << "Epoch nb jump in chip "
                 <<Form("%3u: ", uChipFullId)
                 << Form(" %3d (%6u -> %6u)", iEpJump, fvuCurrEpoch2[uChipFullId],
                                         mess.getEpoch2Number() )
                 <<Form(" TS #%12llu", fulTsNb)
                 <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                 <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                 <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);

      fhGet4EpochJumps->Fill(uChipFullId, iEpJump);
   } // if( fvuCurrEpoch2[uChipFullId] +1 != mess.getGet4V10R32EpochNumber())

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
      LOG(info) << "Epoch missmatch in chip "
                <<Form("%3u: ", uChipFullId)
                <<Form(" EP #%6u", mess.getEpoch2Number() )
                <<Form(" TS #%12llu", fulTsNb)
                <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb) )
                <<Form(" OF #%5u/%5u", fuOffset, fuMaxOffset)
                <<Form(" LO #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
   } // if( 1 == mess.getEpoch2EpochMissmatch() )
   if( 1 == mess.getEpoch2EpochLost() )
   {
      fhGet4EpochFlags->Fill(uChipFullId, 2);
      LOG(info) << "Epoch loss in chip      "
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
      LOG(info) << "Data Loss in chip       "
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

   // TODO: real processing ?
   // For now do nothing to go faster
}
void CbmTSUnpackGet4v1x::ProcessMessage_get4(   get4v1x::Message mess, uint16_t EqID)
{
   // GET4 v1.x data message (24b only for now)
   // TODO: check compatibility when 32b format without hack ready
   uint8_t  cRocId    = mess.getRocNumber();
   if( kTRUE == fbVerbose )
      LOG(debug)<<"CbmTSUnpackGet4v1x::ProcessMessage_get4   => Found a GET4 24b message: ignore it";

   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_get4 => GET4 Data: EqId "
               <<EqID<<" roc "
               <<cRocId;

   uint8_t  cChipId   = mess.getGet4Number();
   UInt_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;
/*   UInt_t uFullChId =
         get4v1x::kuChanPerGet4*( uChipFullId )
         + mess.getGet4ChNum();*/
   UInt_t uTimeStamp = mess.getGet4Ts();

   // Reject data of chips above defined limit to avoid overflow/seg fault
   if( fuNbGet4 <= uChipFullId )
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_get4 => Ignored message with ChipId above limit!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId
                  <<" Limit: " <<fuNbGet4;
      return;
   } // if( fuNbGet4 <= cChipId + kuMaxGet4PerRoc*mess.getRocNumber() )

   // Reject data of Inactive chips
   if( kFALSE == fvbActiveChips[uChipFullId])
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_get4 => Ignored message with Inactive Chip!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId;
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

   // TODO: real processing ?
   // For now do nothing to go faster
}
void CbmTSUnpackGet4v1x::ProcessMessage_sys(    get4v1x::Message mess, uint16_t EqID)
{
   uint8_t  cRocId    = mess.getRocNumber();
   LOG(debug3)<<"CbmTSUnpackGet4v1x::MonitorMessage_sys => System message: EqId "
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
            LOG(info)<<"CbmTSUnpackGet4v1x::MonitorMessage_sys => SYNC DLM!"
                     <<" RocId: " << Form("%02u", cRocId)
                     <<Form(" TS  #%12llu", fulTsNb)
                     <<Form(" MS  #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
                     <<Form(" OF  #%5u/%5u", fuOffset, fuMaxOffset)
                     <<Form(" LO  #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);
            fvbRocFeetSyncStart[cRocId] = kTRUE;
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
                   *get4v1x::kdEpochInPs)
                  *(1e-12) ); // Time since start in s

         if( ( get4v1x::GET4_V1X_ERR_EP < uErrorType &&
               uErrorType < get4v1x::GET4_V1X_ERR_TOK_RING_ST ) )
            fvhGet4ChErrorCntEvo[uChipFullId]->Fill( mess.getGet4V10R24ErrorChan(),
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
         break;
      } // case get4v1x::SYSMSG_GET4_EVENT
      case get4v1x::SYSMSG_CLOSYSYNC_ERROR:
      {
         // GET4 v1.x error message (24b only for now)
         // TODO: check compatibility when 32b format without hack ready
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

         ProcessMessage_Get4v1( mess, EqID);
         break;
      } // case get4v1x::SYSMSG_TS156_SYNC
      default:
      {
         if( kTRUE == fbVerbose )
            LOG(warning)<<"CbmTSUnpackGet4v1x::ProcessMessage_sys => Unknown sysMess type ignored: "
                        <<mess.getSysMesType();
         break;
      } // default:
   } // switch( mess.getSysMesType() )
}
void CbmTSUnpackGet4v1x::ProcessMessage_Get4v1( get4v1x::Message mess, uint16_t EqID)
{
   // GET4 v1.x 32b raw message using hack
   uint8_t  cRocId    = mess.getRocNumber();
   uint8_t  cChipId   = mess.getGet4V10R32ChipId();
   uint32_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;
   uint8_t  cMessType = mess.getGet4V10R32MessageType();

   LOG(debug3)<<"CbmTSUnpackGet4v1x::ProcessMessage_Get4v1 => 32b GET4 message: EqId "
               <<EqID<<" roc "
               <<cRocId;

   // Reject data of chips above defined limit to avoid overflow/seg fault
   if( fuNbGet4 <= uChipFullId )
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::ProcessMessage_Get4v1 => Ignored message with ChipId above limit!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId
                  <<" Limit: " <<fuNbGet4;
      return;
   } // if( fuNbGet4 <= cChipId + kuMaxGet4PerRoc*mess.getRocNumber() )

   // Reject data of Inactive chips
   if( kFALSE == fvbActiveChips[uChipFullId])
   {
      if( kTRUE == fbVerbose )
         LOG(info)<<"CbmTSUnpackGet4v1x::ProcessMessage_Get4v1 => Ignored message with Inactive Chip!"
                  <<" ChipId: "<<cChipId
                  <<" RocId: " <<cRocId;
      return;
   } // if( kFALSE == fvbActiveChips[uChipFullId])

   // Count epoch messages per chip
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
                         <<Form(" MS #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
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
            LOG(debug) << "Epoch nb jump in chip "
                       <<Form("%3u: ", uChipFullId)
                       << Form(" %3d (%6u -> %6u)", iEpJump, fvuCurrEpoch2[uChipFullId],
                                                    mess.getGet4V10R32EpochNumber() )
                       <<Form(" TS  #%12llu", fulTsNb)
                       <<Form(" MS  #%5llu/%5lu", fulMsNb, static_cast<ULong_t>(fsMaxMsNb))
                       <<Form(" OF  #%5u/%5u", fuOffset, fuMaxOffset)
                       <<Form(" LO  #%5u/%5u", fuLocalOffset, fuMaxLocalOffset);

            fhGet4EpochJumps->Fill(uChipFullId, iEpJump);
         } // if( fvuCurrEpoch2[uChipFullId] +1 != mess.getGet4V10R32EpochNumber())

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

         // Check other active chips current epoch and close all epochs which all chips passed
         fvuCurrEpochBuffer[uChipFullId] = static_cast<ULong64_t>(fvuCurrEpoch2[uChipFullId]) +
               ( static_cast<ULong64_t>(fvuCurrEpoch2Cycle[uChipFullId]) << 24);
         ULong64_t uLowerCurrentReadyEp = fvuCurrEpochBuffer[uChipFullId];
//         LOG(debug2)<<"CbmTSUnpackGet4v1x::MonitorMessage_Get4v1 => Check close "
//               << uLowerCurrentReadyEp << " => ";
         for( UInt_t uChipScan = 0; uChipScan < fuNbGet4; uChipScan++)
         {
//            LOG(debug2)<<"( "<<fvbActiveChips[uChipScan]<<", "<<fvuCurrEpochBuffer[uChipScan]<<") ";
            if( kTRUE == fvbActiveChips[uChipScan] &&
                  fvuCurrEpochBuffer[uChipScan] < uLowerCurrentReadyEp )
               uLowerCurrentReadyEp = fvuCurrEpochBuffer[uChipScan];
         } // for( UInt_t uChipScan = 0; uChipScan < fuNbGet4; uChipScan++)

         //

//         LOG(info)<<" => ( "<<fuLastEpBufferReady<<", "<<uLowerCurrentReadyEp<<") ";

         // The map is ordered so we can just loop over
         if( fuLastEpBufferReady < uLowerCurrentReadyEp )
         {
//            for( std::map<ULong64_t, Bool_t>::iterator it= fmsOrderedEpochsBuffStat.find(fuLastEpBufferReady);
            // Check to avoid starting at an entry already removed in previous loops
            auto it_Start = fmsOrderedEpochsBuffStat.find(fuLastEpBufferReady);
            if( fmsOrderedEpochsBuffStat.end() == it_Start )
               it_Start = fmsOrderedEpochsBuffStat.begin();

            for( auto it= it_Start;
                 it!=fmsOrderedEpochsBuffStat.find(uLowerCurrentReadyEp);
                 ++it)
               it->second = kTRUE;
            auto it = fmsOrderedEpochsBuffStat.find(uLowerCurrentReadyEp);
            --it; // Go back to previous element
            fuLastEpBufferReady = it->first;
         }

         // Start new buffer for this epoch if it does not already exist
         if( fmsOrderedEpochsData.end() == fmsOrderedEpochsData.find(fvuCurrEpochBuffer[uChipFullId]) )
         {
            // new entry in map with empty multiset
/*
            fmsOrderedEpochsData.insert( fmsOrderedEpochsData.end(),
                  std::pair< ULong64_t, std::multiset< get4v1x::FullMessage > >(
                        fvuCurrEpochBuffer[uChipFullId],
                        std::multiset< get4v1x::FullMessage >() ) );
*/
            std::multiset< get4v1x::FullMessage > * newEpochBuff = new std::multiset< get4v1x::FullMessage >();
            fmsOrderedEpochsData.insert( fmsOrderedEpochsData.end(),
                  std::pair< ULong64_t, std::multiset< get4v1x::FullMessage >* >(
                        fvuCurrEpochBuffer[uChipFullId], newEpochBuff) );

            // new entry in map with kFALSE flag
            fmsOrderedEpochsBuffStat.insert( fmsOrderedEpochsBuffStat.end(),
                  std::pair<ULong64_t, Bool_t>(fvuCurrEpochBuffer[uChipFullId], kFALSE) );
         } // if( fmsOrderedEpochsData.end() == fmsOrderedEpochsData.find(fvuCurrEpochBuffer[uChipFullId]) )

         fvuCurrEpochBufferIt[uChipFullId] = fmsOrderedEpochsData.find(fvuCurrEpochBuffer[uChipFullId]);

         break;
      } // case get4v1x::GET4_32B_EPOCH
      case get4v1x::GET4_32B_SLCM:  // => Slow control
      {
         Double_t dFullChId =
               get4v1x::kuChanPerGet4*2*( uChipFullId )
               + mess.getGet4V10R32SlChan()
               + 0.5*mess.getGet4V10R32SlEdge();

         // As long as the 1st epoch with sync flag after SYNC DLM is found,
         // consider data as not good
         if( kTRUE == fvbGet4WaitFirstSync[uChipFullId] )
            return;

         fhGet4ChanSlowContM->Fill( dFullChId, mess.getGet4V10R32SlType() );
         break;
      } // case get4v1x::GET4_32B_SLCM
      case get4v1x::GET4_32B_ERROR: // => Error message
      {
         Double_t dFullChId =
               get4v1x::kuChanPerGet4*2*( uChipFullId )
               + mess.getGet4V10R32ErrorChan()
               + 0.5*mess.getGet4V10R32ErrorEdge();
         UInt_t   uErrorType = mess.getGet4V10R32ErrorData();

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
               LOG(debug)<<"CbmTSUnpackGet4v1x::MonitorMessage_Get4v1 => Readout Init!"
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
               break;
            case get4v1x::GET4_V1X_ERR_TOT_RANGE:
               fhGet4ChanErrors->Fill( dFullChId, 14);
               break;
            case get4v1x::GET4_V1X_ERR_EVT_DISCARD:
               fhGet4ChanErrors->Fill( dFullChId, 15);
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

         LOG(debug)<<"CbmTSUnpackGet4v1x::MonitorMessage_Get4v1 => Data!"
                  <<" ChipId: "<<Form("%02u", cChipId)
                  <<" RocId: " <<Form("%02u", cRocId)
                  <<" TS: " <<fulTsNb;

         if( kTRUE == fbEnaCalibOutput )
            fvhFineTime[ uChipFullId ][ mess.getGet4V10R32HitChan() ]->Fill( mess.getGet4V10R32HitFt() );

         // Save the message in the corresponding epoch vector
         get4v1x::FullMessage fullMess( mess, fvuCurrEpochBuffer[uChipFullId] );
//         std::map< ULong64_t, std::multiset< get4v1x::FullMessage > >::iterator it;
//         auto it = fmsOrderedEpochsData.find( fvuCurrEpochBuffer[uChipFullId] );
         auto it = fvuCurrEpochBufferIt[uChipFullId];
//         (it->second).insert( (it->second).end(), fullMess ); // OLD
         (it->second)->insert( (it->second)->end(), fullMess );

         break;
      } // case get4v1x::GET4_32B_DATA
   } // switch( cMessType )
}
//**********************************************/

Bool_t CbmTSUnpackGet4v1x::WriteCalibrationFile()
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
         LOG(info)<<"CbmTSUnpackGet4v1x::WriteCalibrationFile Undefined tdc parameters for type ";
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
      else LOG(warning)<<"CbmTSUnpackGet4v1x::WriteCalibrationFile => Unable to open root file "
                        <<sCalibOutFilename
                        <<" to save calibration data, please check settings";
   gDirectory->Cd(oldDir->GetPath());

   return kTRUE;
}

ClassImp(CbmTSUnpackGet4v1x)
