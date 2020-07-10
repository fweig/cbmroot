// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018UnpackerTaskSts                    -----
// -----               Created 26.01.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018UnpackerTaskSts_H
#define CbmMcbm2018UnpackerTaskSts_H

#include "Timeslice.hpp"

#include "CbmErrorMessage.h"
#include "CbmMcbmUnpack.h"
#include "CbmStsDigi.h"

class CbmMcbm2018UnpackerAlgoSts;
//class TClonesArray;

struct FebChanMask {
  UInt_t uFeb;
  UInt_t uChan;
  Bool_t bMasked;
};

class CbmMcbm2018UnpackerTaskSts : public CbmMcbmUnpack {
public:
  CbmMcbm2018UnpackerTaskSts(UInt_t uNbSdpb = 1);

  CbmMcbm2018UnpackerTaskSts(const CbmMcbm2018UnpackerTaskSts&) = delete;
  CbmMcbm2018UnpackerTaskSts
  operator=(const CbmMcbm2018UnpackerTaskSts&) = delete;

  virtual ~CbmMcbm2018UnpackerTaskSts();

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
  void MaskNoisyChannel(UInt_t uFeb, UInt_t uChan, Bool_t bMasked = kTRUE);
  void SetAdcCut(UInt_t uAdc);

  /// => Quick and dirty hack for binning FW!!!
  void SetBinningFwFlag(Bool_t bEnable = kTRUE);

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
  std::vector<FebChanMask> fvChanMasks;

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Output vectors
  std::vector<CbmStsDigi>* fpvDigiSts       = nullptr;
  std::vector<CbmErrorMessage>* fpvErrorSts = nullptr;

  /// Processing algo
  CbmMcbm2018UnpackerAlgoSts* fUnpackerAlgo;


  ClassDef(CbmMcbm2018UnpackerTaskSts, 2)
};

#endif
