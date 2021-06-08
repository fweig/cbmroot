/* Copyright (C) 2018 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTofStarData2019                         -----
// -----               Created 13.01.2017 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTofStarData2019.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

/********************** CbmTofStarTrigger2019 *************************/
CbmTofStarTrigger2019::CbmTofStarTrigger2019(ULong64_t ulGdpbTsFullIn, ULong64_t ulStarTsFullIn, UInt_t uStarTokenIn,
                                             UInt_t uStarDaqCmdIn, UInt_t uStarTrigCmdIn, UShort_t usGdpbId)
  : fusGdpbId(usGdpbId)
  , fulGdpbTsFull(ulGdpbTsFullIn)
  , fulStarTsFull(ulStarTsFullIn)
  , fuStarToken(uStarTokenIn)
  , fusStarDaqCmd(uStarDaqCmdIn)
  , fusStarTrigCmd(uStarTrigCmdIn)
{
}
//! strict weak ordering operator, assumes same TS cycle for both triggers
bool CbmTofStarTrigger2019::operator<(const CbmTofStarTrigger2019& other) const
{
  ULong64_t uThisTs  = this->GetFullGdpbTs();
  ULong64_t uOtherTs = other.GetFullGdpbTs();

  return uThisTs < uOtherTs;
}
UInt_t CbmTofStarTrigger2019::GetStarTrigerWord() const
{
  // trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
  UInt_t uTrigWord = ((fusStarTrigCmd & 0x00F) << 16) + ((fusStarDaqCmd & 0x00F) << 12) + ((fuStarToken & 0xFFF));
  return uTrigWord;
}
UInt_t CbmTofStarTrigger2019::GetFullGdpbEpoch() const
{
  // trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
  UInt_t uEpochIndex = (fulGdpbTsFull >> gdpbv100::kuCtSize) & gdpbv100::kuEpochCounterSz;
  return uEpochIndex;
}
std::vector<gdpbv100::FullMessage> CbmTofStarTrigger2019::GetGdpbMessages() const
{
  gdpbv100::FullMessage mCommonData((static_cast<ULong64_t>(fusGdpbId) << 48)  // GdpbId
                                      + gdpbv100::MSG_STAR_TRI_A,              // Message type
                                    (GetFullGdpbTs() >> gdpbv100::kuCtSize)    // gDPB TS in epochs
  );
  std::vector<gdpbv100::FullMessage> vMsgs(4, mCommonData);

  /// Subtype 0
  vMsgs[0].setStarTrigMsgIndex(0);              // Message Subtype
  vMsgs[0].setGdpbTsMsbStarA(GetFullGdpbTs());  // 40b MSB of GDPB TS b[ 24, 63 ]
  /// Subtype 1
  vMsgs[1].setStarTrigMsgIndex(1);              // Message Subtype
  vMsgs[1].setGdpbTsLsbStarB(GetFullGdpbTs());  // 24b LSB of GDPB TS b[  0, 23 ]
  vMsgs[1].setStarTsMsbStarB(GetFullStarTs());  // 16b MSB of STAR TS b[ 48, 63 ]
  /// Subtype 2
  vMsgs[2].setStarTrigMsgIndex(2);              // Message Subtype
  vMsgs[2].setStarTsMidStarC(GetFullStarTs());  // 40b mid bits of STAR TS b[  8, 47 ]
  /// Subtype 3
  vMsgs[3].setStarTrigMsgIndex(3);                 // Message Subtype
  vMsgs[3].setStarTsLsbStarD(GetFullStarTs());     // 8b LSB of STAR TS b[ 0, 7]
  vMsgs[3].setStarFillerD();                       // 12 bits in between are set to 0
  vMsgs[3].setStarTokenStarD(GetStarToken());      // 12b STAR Token
  vMsgs[3].setStarDaqCmdStarD(GetStarDaqCmd());    // 4b STAR DAQ CMD
  vMsgs[3].setStarTrigCmdStarD(GetStarTrigCmd());  // 4b STAR TRIG CMD

  return vMsgs;
}
//ClassImp(CbmTofStarTrigger2019)
/**********************************************************************/
/********************** CbmTofStarSubevent2019 ************************/
CbmTofStarSubevent2019::CbmTofStarSubevent2019()
  : fbTriggerSet(kFALSE)
  , fTrigger(CbmTofStarTrigger2019(0, 0, 0, 0, 0))
  , fusSourceId(0)
  , fulEventStatusFlags(0)
  , fuEventSizeBytes(4 * sizeof(ULong64_t))
  , fvMsgBuffer()
{
}
CbmTofStarSubevent2019::CbmTofStarSubevent2019(CbmTofStarTrigger2019 triggerIn, UShort_t sourceIdIn)
  : fbTriggerSet(kTRUE)
  , fTrigger(triggerIn)
  , fusSourceId(sourceIdIn)
  , fulEventStatusFlags(0)
  , fuEventSizeBytes(4 * sizeof(ULong64_t))
  , fvMsgBuffer()
{
  SetSource(sourceIdIn);
}
CbmTofStarSubevent2019::CbmTofStarSubevent2019(const CbmTofStarSubevent2019& eventIn)
  : fbTriggerSet(eventIn.fbTriggerSet)
  , fTrigger(eventIn.fTrigger)
  , fusSourceId(eventIn.fusSourceId)
  , fulEventStatusFlags(eventIn.fulEventStatusFlags)
  , fuEventSizeBytes(eventIn.fuEventSizeBytes)
  , fvMsgBuffer(eventIn.fvMsgBuffer)
{
  /// Buffer ignored as always refilled when accessed with BuildOutput
}
CbmTofStarSubevent2019 CbmTofStarSubevent2019::operator=(const CbmTofStarSubevent2019& eventIn)
{
  fbTriggerSet        = eventIn.fbTriggerSet;
  fTrigger            = eventIn.fTrigger;
  fusSourceId         = eventIn.fusSourceId;
  fulEventStatusFlags = eventIn.fulEventStatusFlags;
  fuEventSizeBytes    = eventIn.fuEventSizeBytes;
  fvMsgBuffer         = eventIn.fvMsgBuffer;
  /// Buffer ignored as always refilled when accessed with BuildOutput

  return *this;
}

CbmTofStarSubevent2019::~CbmTofStarSubevent2019() { ClearSubEvent(); }
void CbmTofStarSubevent2019::ClearSubEvent()
{
  fbTriggerSet        = kFALSE;
  fulEventStatusFlags = 0;
  fvMsgBuffer.clear();
}
void CbmTofStarSubevent2019::SortMessages() { std::sort(fvMsgBuffer.begin(), fvMsgBuffer.end()); }
void* CbmTofStarSubevent2019::BuildOutput(Int_t& iOutputSizeBytes)
{
  // If trigger not set, something is wrong, we just pass
  if (kFALSE == fbTriggerSet) {
    iOutputSizeBytes = 0;
    return NULL;
  }  // if( kFALSE == fbTriggerSet )

  iOutputSizeBytes = 0;

  // Size of output is
  //     3 * Long64 for Header Star Token Info
  //     1 * Long64 for Header Event Status Flags (version, bad/good, ...) & source ID
  // 2 * n * Long64 for the n messages in Buffer: 1 extended Epoch + message data
  iOutputSizeBytes = 4 + 2 * fvMsgBuffer.size();
  iOutputSizeBytes *= sizeof(ULong64_t);
  UInt_t uMsgsToRead = fvMsgBuffer.size();

  // Check if max size passed
  // If it is the case, limit the number of data read + set Bad Event flag
  if (kuMaxOutputSize < static_cast<uint32_t>(iOutputSizeBytes)) {
    iOutputSizeBytes = kuMaxOutputSize;
    uMsgsToRead      = kuMaxNbMsgs;
    fulEventStatusFlags |= kulFlagBadEvt;
  }  // if( kuMaxOutputSize < iOutputSizeBytes )

  /// Store the event size in the free part of the status member for consistency checks
  fuEventSizeBytes    = iOutputSizeBytes & kulEventSizeMask;
  fulEventStatusFlags = (fulEventStatusFlags & !(kulEventSizeMask << kulEventSizeOffset))
                        | (static_cast<ULong64_t>(fuEventSizeBytes) << kulEventSizeOffset);

  // Fills header info
  fpulBuff[0] = fTrigger.GetFullGdpbTs();
  fpulBuff[1] = fTrigger.GetFullStarTs();
  fpulBuff[2] = (static_cast<ULong64_t>(fTrigger.GetStarToken()) << 32)
                + (static_cast<ULong64_t>(fTrigger.GetStarDaqCmd()) << 16)
                + (static_cast<ULong64_t>(fTrigger.GetStarTrigCmd()));
  fpulBuff[3] = fulEventStatusFlags;

  // does not work due to "error: cannot convert ‘gdpbv100::Message’ to ‘long unsigned int’ in assignment"
  //  std::copy( fvMsgBuffer.begin(), fvMsgBuffer.begin() + uMsgsToRead, pulBuff + 4 );

  // Unoptimized replacement: item by item copy
  for (UInt_t uMsgIdx = 0; uMsgIdx < uMsgsToRead; uMsgIdx++) {
    fpulBuff[4 + 2 * uMsgIdx]     = fvMsgBuffer[uMsgIdx].getData();
    fpulBuff[4 + 2 * uMsgIdx + 1] = fvMsgBuffer[uMsgIdx].getExtendedEpoch();
  }  // for( UInt_t uMsgIdx = 0; uMsgIdx < uMsgsToRead; uMsgIdx++)

  return static_cast<void*>(fpulBuff);
}
Bool_t CbmTofStarSubevent2019::LoadInput(void* pBuff, Int_t iInputSizeBytes)
{
  // Check input variables are properly defined
  if (NULL == pBuff || 0 == iInputSizeBytes) return kFALSE;

  Int_t iRestBytes = iInputSizeBytes % sizeof(ULong64_t);
  Int_t iInputSzLg = iInputSizeBytes / sizeof(ULong64_t);


  // Check event header is complete and buffer is multiple of 64b
  if (iInputSzLg < 4 || 0 < iRestBytes) return kFALSE;

  // First clear subEvent content
  ClearSubEvent();

  // Read the header to the SubEvent members
  ULong64_t* pulLongBuff    = static_cast<ULong64_t*>(pBuff);
  ULong64_t ulTrgGdpbFullTs = pulLongBuff[0];
  ULong64_t ulTrgStarFullTs = pulLongBuff[1];
  UInt_t uStarToken         = (pulLongBuff[2] >> 32) & 0xFFF;
  UInt_t uStarDaqCmdIn      = (pulLongBuff[2] >> 16) & 0x00F;
  UInt_t uStarTrigCmdIn     = (pulLongBuff[2]) & 0x00F;
  fulEventStatusFlags       = pulLongBuff[3];

  fusSourceId = (fulEventStatusFlags >> kulSourceIdOffset) & kulSourceIdMask;

  fuEventSizeBytes = (fulEventStatusFlags >> kulEventSizeOffset) & kulEventSizeMask;

  fTrigger.SetFullGdpbTs(ulTrgGdpbFullTs);
  fTrigger.SetFullStarTs(ulTrgStarFullTs);
  fTrigger.SetStarToken(uStarToken);
  fTrigger.SetStarDaqCmd(uStarDaqCmdIn);
  fTrigger.SetStarTRigCmd(uStarTrigCmdIn);

  // Read as many messages as left in the buffer and store them in the vector
  UInt_t uMsgsToRead = (iInputSzLg - 4) / 2;
  for (UInt_t uMsgIdx = 0; uMsgIdx < uMsgsToRead; uMsgIdx++) {
    gdpbv100::FullMessage mess(pulLongBuff[4 + 2 * uMsgIdx], pulLongBuff[4 + 2 * uMsgIdx + 1]);
    fvMsgBuffer.push_back(mess);
  }  // for( UInt_t uMsgIdx = 0; uMsgIdx < uMsgsToRead; uMsgIdx++)

  return kTRUE;
}
void CbmTofStarSubevent2019::PrintSubEvent()
{
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "Printing CbmTofStarSubevent2019 dump" << std::endl;
  // Print the header
  std::cout << "Trigger info: gDPB TS = " << fTrigger.GetFullGdpbTs() << " STAR TS = " << fTrigger.GetFullStarTs()
            << " Token = " << fTrigger.GetStarToken() << " DAQ CMD = " << fTrigger.GetStarDaqCmd()
            << " TRIG CMD = " << fTrigger.GetStarTrigCmd() << std::endl;
  std::cout << "Status flags = " << std::hex << std::setw(16) << fulEventStatusFlags << std::dec << std::endl;

  // Print the messages in the buffer

  for (UInt_t uMsgIdx = 0; uMsgIdx < fvMsgBuffer.size(); uMsgIdx++) {
    fvMsgBuffer[uMsgIdx].PrintMessage(gdpbv100::msg_print_Prefix | gdpbv100::msg_print_Data);
  }  // for( UInt_t uMsgIdx = 0; uMsgIdx < uMsgsToRead; uMsgIdx++)

  std::cout << "-------------------------------------------------------" << std::endl;
}
//ClassImp(CbmTofStarSubevent2019)
/**********************************************************************/
