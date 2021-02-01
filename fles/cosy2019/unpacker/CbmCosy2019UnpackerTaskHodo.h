// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmCosy2019UnpackerTaskHodo                     -----
// -----              Created 31/07/19  by P.-A. Loizeau                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmCosy2019UnpackerTaskHodo_H
#define CbmCosy2019UnpackerTaskHodo_H

#include "Timeslice.hpp"

#include "CbmErrorMessage.h"
#include "CbmMcbmUnpack.h"
#include "CbmStsDigi.h"

class CbmCosy2019UnpackerAlgoHodo;
class CbmMcbm2018UnpackerAlgoSts;

struct FebChanMaskSts {
  UInt_t uFeb;
  UInt_t uChan;
  Bool_t bMasked;
};

class CbmCosy2019UnpackerTaskHodo : public CbmMcbmUnpack {
public:
  CbmCosy2019UnpackerTaskHodo(UInt_t uNbSdpb = 1);

  CbmCosy2019UnpackerTaskHodo(const CbmCosy2019UnpackerTaskHodo&) = delete;
  CbmCosy2019UnpackerTaskHodo
  operator=(const CbmCosy2019UnpackerTaskHodo&) = delete;

  virtual ~CbmCosy2019UnpackerTaskHodo();

  virtual Bool_t Init();
  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  virtual void Reset();

  virtual void Finish();

  void SetParContainers();

  Bool_t InitContainers();

  Bool_t ReInitContainers();

  /// Temp until we change from CbmMcbmUnpack to something else
  void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  void SetNbMsInTs(size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/) {};

  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) {
    fbMonitorMode = bFlagIn;
  }
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE);

  void SetTimeOffsetNs(Double_t dOffsetIn = 0.0);
  void SetTimeOffsetNsAsic(UInt_t uAsicIdx, Double_t dOffsetIn = 0.0);

  void SetTimeOffsetNsSts(Double_t dOffsetIn = 0.0);
  void SetTimeOffsetNsAsicSts(UInt_t uAsicIdx, Double_t dOffsetIn = 0.0);
  void MaskNoisyChannelSts(UInt_t uFeb, UInt_t uChan, Bool_t bMasked = kTRUE);
  void SetAdcCutSts(UInt_t uAdc);

  /// Task settings
  void SetWriteOutputFlag(Bool_t bFlagIn) { fbWriteOutput = bFlagIn; }

private:
  /// Control flags
  Bool_t
    fbMonitorMode;  //! Switch ON the filling of a minimal set of histograms
  Bool_t
    fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms
  Bool_t
    fbWriteOutput;  //! If ON the output TClonesArray of digi is written to disk

  /// Temporary storage of user parameters
  std::vector<FebChanMaskSts> fvChanMasks;

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Output vectors
  std::vector<CbmStsDigi>* fpvDigiSts       = nullptr;
  std::vector<CbmErrorMessage>* fpvErrorSts = nullptr;

  /// Processing algo
  CbmCosy2019UnpackerAlgoHodo* fUnpackerAlgo;
  CbmMcbm2018UnpackerAlgoSts* fUnpackerAlgoSts = nullptr;

  ClassDef(CbmCosy2019UnpackerTaskHodo, 1)
};

#endif
