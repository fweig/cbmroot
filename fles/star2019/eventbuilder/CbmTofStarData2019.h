// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTofStarData2019                         -----
// -----               Created 13.01.2017 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTofStarData2019_H
#define CbmTofStarData2019_H

#include "gDpbMessv100.h"

#include "Rtypes.h"

#include <vector>

class CbmTofStarTrigger2019 {
public:
  // Constructors
  CbmTofStarTrigger2019(ULong64_t ulGdpbTsFullIn,
                        ULong64_t ulStarTsFullIn,
                        UInt_t uStarTokenIn,
                        UInt_t uStarDaqCmdIn,
                        UInt_t uStarTrigCmdIn,
                        UShort_t usGdpbId = 0);

  // Destructor
  ~CbmTofStarTrigger2019() {};

  // Setters
  inline void SetFullGdpbTs(ULong64_t ulGdpbTsFullIn) {
    fulGdpbTsFull = ulGdpbTsFullIn;
  }
  inline void SetFullStarTs(ULong64_t ulStarTsFullIn) {
    fulStarTsFull = ulStarTsFullIn;
  }
  inline void SetStarToken(UInt_t uStarTokenIn) { fuStarToken = uStarTokenIn; }
  inline void SetStarDaqCmd(UInt_t uStarDaqCmdIn) {
    fusStarDaqCmd = uStarDaqCmdIn;
  }
  inline void SetStarTRigCmd(UInt_t uStarTrigCmdIn) {
    fusStarTrigCmd = uStarTrigCmdIn;
  }

  // Accessors
  inline ULong64_t GetFullGdpbTs() const { return fulGdpbTsFull; }
  inline ULong64_t GetFullStarTs() const { return fulStarTsFull; }
  inline UInt_t GetStarToken() const { return fuStarToken; }
  inline UShort_t GetStarDaqCmd() const { return fusStarDaqCmd; }
  inline UShort_t GetStarTrigCmd() const { return fusStarTrigCmd; }
  UInt_t GetStarTrigerWord() const;
  UInt_t GetFullGdpbEpoch() const;
  std::vector<gdpbv100::FullMessage> GetGdpbMessages() const;

  // Operators
  bool operator<(const CbmTofStarTrigger2019& other) const;

private:
  UShort_t fusGdpbId;
  ULong64_t fulGdpbTsFull;
  ULong64_t fulStarTsFull;
  UInt_t fuStarToken;
  UShort_t fusStarDaqCmd;
  UShort_t fusStarTrigCmd;

  //      CbmTofStarTrigger2019(const CbmTofStarTrigger2019&);
  //      CbmTofStarTrigger2019 operator=(const CbmTofStarTrigger2019&);

  //   ClassDef(CbmTofStarTrigger2019, 1)
};

class CbmTofStarSubevent2019 {
  // Methods implementation can be found at the end of CbmTsMonitorTofStar.cxx!
public:
  // Constructors
  CbmTofStarSubevent2019();
  CbmTofStarSubevent2019(CbmTofStarTrigger2019 triggerIn, UShort_t sourceIdIn);
  CbmTofStarSubevent2019(const CbmTofStarSubevent2019&);
  CbmTofStarSubevent2019 operator=(const CbmTofStarSubevent2019&);

  // Destructor
  ~CbmTofStarSubevent2019();

  // Setters
  inline void SetTrigger(CbmTofStarTrigger2019 triggerIn) {
    fTrigger     = triggerIn;
    fbTriggerSet = kTRUE;
  }
  inline void SetSource(UShort_t sourceIdIn) {
    fusSourceId = sourceIdIn;
    fulEventStatusFlags =
      (fulEventStatusFlags & !(kulSourceIdMask << kulSourceIdOffset))
      | ((sourceIdIn & kulSourceIdMask) << kulSourceIdOffset);
  }
  inline void SetBadEventFlag(Bool_t bFlagState = kTRUE) {
    bFlagState ? (fulEventStatusFlags |= kulFlagBadEvt)
               : (fulEventStatusFlags &= ~(kulFlagBadEvt));
  }
  inline void SetOverlapEventFlag(Bool_t bFlagState = kTRUE) {
    bFlagState ? (fulEventStatusFlags |= kulFlagOverlapEvt)
               : (fulEventStatusFlags &= ~(kulFlagOverlapEvt));
  }
  inline void SetEmptyEventFlag(Bool_t bFlagState = kTRUE) {
    bFlagState ? (fulEventStatusFlags |= kulFlagEmptyEvt)
               : (fulEventStatusFlags &= ~(kulFlagEmptyEvt));
  }
  inline void SetStartBorderEventFlag(Bool_t bFlagState = kTRUE) {
    bFlagState ? (fulEventStatusFlags |= kulFlagStartBorderEvt)
               : (fulEventStatusFlags &= ~(kulFlagStartBorderEvt));
  }
  inline void SetEndBorderEventFlag(Bool_t bFlagState = kTRUE) {
    bFlagState ? (fulEventStatusFlags |= kulFlagEndBorderEvt)
               : (fulEventStatusFlags &= ~(kulFlagEndBorderEvt));
  }
  inline void SetIncompleteEventFlag(Bool_t bFlagState = kTRUE) {
    bFlagState ? (fulEventStatusFlags |= kulFlagIncompleteEvt)
               : (fulEventStatusFlags &= ~(kulFlagIncompleteEvt));
  }
  inline void AddMsg(const gdpbv100::FullMessage& msgIn) {
    fvMsgBuffer.push_back(msgIn);
    fuEventSizeBytes += 2 * sizeof(ULong64_t);
  }

  // Accessors
  inline CbmTofStarTrigger2019 GetTrigger() const { return fTrigger; }
  inline UShort_t GetSource() const { return fusSourceId; }
  inline Bool_t GetBadEventFlag() const {
    return 0 < (fulEventStatusFlags & kulFlagBadEvt);
  }
  inline Bool_t GetOverlapEventFlag() const {
    return 0 < (fulEventStatusFlags & kulFlagOverlapEvt);
  }
  inline Bool_t GetEmptyEventFlag() const {
    return 0 < (fulEventStatusFlags & kulFlagEmptyEvt);
  }
  inline Bool_t GetStartBorderEventFlag() const {
    return 0 < (fulEventStatusFlags & kulFlagStartBorderEvt);
  }
  inline Bool_t GetEndBorderEventFlag() const {
    return 0 < (fulEventStatusFlags & kulFlagEndBorderEvt);
  }
  inline Bool_t GetIncompleteEventFlag() const {
    return 0 < (fulEventStatusFlags & kulFlagIncompleteEvt);
  }
  inline Int_t GetEventSize() const { return fuEventSizeBytes; }
  inline gdpbv100::Message GetMsg(UInt_t uMsgIdx) const;
  inline UInt_t GetMsgBuffSize() const { return fvMsgBuffer.size(); }
  inline static uint32_t GetMaxOutputSize() { return kuMaxOutputSize; }
  inline Double_t GetEventTimeSec() const {
    return (1e-9) * gdpbv100::kdClockCycleSizeNs * fTrigger.GetFullGdpbTs();
  }

  // Content clearing
  void ClearSubEvent();

  // Sort content of output buffer
  void SortMessages();

  // Sub-event output
  void* BuildOutput(Int_t& iOutputSizeBytes);

  // Sub-event input
  Bool_t LoadInput(void* pBuff, Int_t iInputSizeBytes);

  // Print the heade and the messages in the buffer
  void PrintSubEvent();

private:
  static const uint32_t kuMaxOutputSize = 131072;  // 2^17
  static const uint32_t kuMaxNbMsgs =
    8190;  // 4 * 64b in header => floor( (2^17 / 8 ) - 4) / 2
  static const uint64_t kulFlagBadEvt = 0x1
                                        << 0;  //! General flag for bad event
  static const uint64_t kulFlagOverlapEvt =
    0x1 << 1;  //! Event in overlap with the previous event
  static const uint64_t kulFlagEmptyEvt = 0x1 << 2;  //! Empty event
  static const uint64_t kulFlagStartBorderEvt =
    0x1 << 3;  //! Event containing data from previous overlap MS
  static const uint64_t kulFlagEndBorderEvt =
    0x1 << 4;  //! Event containing data from following overlap MS
  static const uint64_t kulFlagIncompleteEvt =
    0x1 << 5;  //! Incomplete event: at least one gDPB missed the trigger signal
  static const uint64_t kulSourceIdOffset  = 16;
  static const uint64_t kulSourceIdMask    = 0xFFFF;
  static const uint64_t kulEventSizeOffset = 32;
  static const uint64_t kulEventSizeMask   = 0xFFFFFFFF;

  Bool_t fbTriggerSet;
  CbmTofStarTrigger2019 fTrigger;
  UShort_t fusSourceId;
  ULong64_t fulEventStatusFlags;
  UInt_t fuEventSizeBytes;
  std::vector<gdpbv100::FullMessage> fvMsgBuffer;
  ULong64_t fpulBuff[kuMaxOutputSize];


  //   ClassDef(CbmTofStarSubevent2019, 1)
};

#endif
