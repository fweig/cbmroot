/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, David Emschermann [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018UnpackerAlgoPsd                    -----
// -----                 Created 09.10.2019 by N.Karpushkin                -----
// -----        based on CbmMcbm2018UnpackerAlgoTof by P.-A. Loizeau       -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018UnpackerAlgoPsd_H
#define CbmMcbm2018UnpackerAlgoPsd_H

#include "CbmStar2019Algo.h"

// Data
#include "CbmPsdDigi.h"
#include "CbmPsdDsp.h"

#include "PronyFitter.h"
#include "PsdGbtReader-v0.00.h"
#include "PsdGbtReader-v1.00.h"

// CbmRoot

// C++11
#include <chrono>
#include <numeric>

// C/C++
#include <map>
#include <vector>

class CbmMcbm2018PsdPar;

class CbmMcbm2018UnpackerAlgoPsd : public CbmStar2019Algo<CbmPsdDigi> {
public:
  CbmMcbm2018UnpackerAlgoPsd();
  ~CbmMcbm2018UnpackerAlgoPsd();

  /** \brief Copy constructor - not implemented **/
  CbmMcbm2018UnpackerAlgoPsd(const CbmMcbm2018UnpackerAlgoPsd&) = delete;
  /** \brief Copy assignment operator - not implemented **/
  CbmMcbm2018UnpackerAlgoPsd& operator=(const CbmMcbm2018UnpackerAlgoPsd&) = delete;

  virtual Bool_t Init();
  virtual void Reset();
  virtual void Finish();

  Bool_t InitContainers();
  Bool_t ReInitContainers();
  TList* GetParList();

  Bool_t InitParameters();

  Bool_t ProcessTs(const fles::Timeslice& ts);
  Bool_t ProcessTs(const fles::Timeslice& ts, size_t /*component*/) { return ProcessTs(ts); }
  Bool_t ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx);

  void AddMsComponentToList(size_t component, UShort_t usDetectorId);

  Bool_t SetDigiOutputPointer(std::vector<CbmPsdDigi>* const pVector);
  Bool_t SetDspOutputPointer(std::vector<CbmPsdDsp>* const pVector);
  std::unique_ptr<CbmPsdDigi> MakeDigi(CbmPsdDsp dsp);

  Bool_t CreateHistograms();
  Bool_t FillHistograms();
  Bool_t ResetHistograms();

  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  inline void SetDspWriteMode(Bool_t bFlagIn = kTRUE) { fbDebugWriteOutput = bFlagIn; }
  inline void SetTimeOffsetNs(Double_t dOffsetIn = 0.0) { fdTimeOffsetNs = dOffsetIn; }


private:
  /// Control flags
  Bool_t fbMonitorMode      = kFALSE;  //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode = kFALSE;  //! Switch ON the filling of a additional set of histograms
  Bool_t fbDebugWriteOutput = kFALSE;  //! If ON the output vector of dsp debug information is written to disk

  /// Output vectors
  std::vector<CbmPsdDigi>* fPsdDigiVector =
    nullptr;  //! Output Digi vector /* TODO CHECK The exclamation mark signals the transientness */
  std::vector<CbmPsdDsp>* fPsdDspVector =
    nullptr;  //! Output Dsp vector  /* TODO CHECK The exclamation mark signals the transientness */

  std::vector<Bool_t> fvbMaskedComponents = {};

  /// Settings from parameter file
  CbmMcbm2018PsdPar* fUnpackPar = nullptr;        //!
  UInt_t fuRawDataVersion       = 0;              //! Raw data versioning
                                                  /// Readout chain dimensions and mapping
  UInt_t fuNrOfGdpbs                       = 0;   //! Total number of GDPBs in the system
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap = {};  //! gDPB ID to index map
  UInt_t fuNrOfFeePerGdpb                  = 0;   //! Number of FEBs per GDPB
  UInt_t fuNrOfChannelsPerFee              = 0;   //! Number of channels in each FEE
  UInt_t fuNrOfChannelsPerGdpb             = 0;   //! Number of channels per GDPB

  /// Detector Mapping
  UInt_t fuNrOfGbtx              = 0;
  UInt_t fuNrOfModules           = 0;
  std::vector<Int_t> fviPsdChUId = {};

  /// User settings: Data correction parameters
  Double_t fdTimeOffsetNs = 0.0;

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 8;
  static const UInt_t kuDetMask         = 0x0001FFFF;

  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx = 0;     //! Idx of the current TS
  ULong64_t fulCurrentMsIdx = 0;     //! Idx of the current MS in TS (0 to fuTotalMsNb)
  size_t fuCurrentMsSysId   = 0;     //! SysId of the current MS in TS (0 to fuTotalMsNb)
  Double_t fdTsStartTime    = -1.0;  //! Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore =
    -1.0;                    //! End Time in ns of current TS Core from the index of the first MS first component
  Double_t fdMsTime = -1.0;  //! Start Time in ns of current MS from its index field in header
  UInt_t fuMsIndex  = 0;     //! Index of current MS within the TS

  /// Current data properties
  UInt_t fuCurrentEquipmentId = 0;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId          = 0;  //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx         = 0;  //! Index of the DPB from which the MS currently unpacked is coming


  /// Starting state book-keeping
  Double_t fdStartTime = -1.0; /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Double_t fdStartTimeMsSz = 0.0; /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point ftStartTimeUnix = std::chrono::steady_clock::
    now(); /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  ClassDef(CbmMcbm2018UnpackerAlgoPsd, 2)
};

#endif
