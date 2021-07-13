/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dennis Spicker, Florian Uhlig [committer], Pascal Raisig */

/**
  * @file CbmMcbm2018UnpackerTaskTrdR.h
  * @author Dennis Spicker <dspicker@ikf.uni-frankfurt.de>
  * @date 2020-01-20
 **/

#ifndef CbmMcbm2018UnpackerTaskTrdR_H
#define CbmMcbm2018UnpackerTaskTrdR_H

#include "CbmMcbm2020TrdTshiftPar.h"
#include "CbmMcbmUnpack.h"
#include "CbmTrdDigi.h"
#include "CbmTrdRawMessageSpadic.h"

#include "TString.h"

class CbmMcbm2018UnpackerAlgoTrdR;

/**
  *  @class CbmMcbm2018UnpackerTaskTrdR
  *  @brief Timeslice unpacker FairTask for Spadic v.2.2 .
  *
  *
 **/
class CbmMcbm2018UnpackerTaskTrdR : public CbmMcbmUnpack {
public:
  /** Default Constructor */
  CbmMcbm2018UnpackerTaskTrdR();

  virtual ~CbmMcbm2018UnpackerTaskTrdR();

  /** Copy Constructor */
  CbmMcbm2018UnpackerTaskTrdR(const CbmMcbm2018UnpackerTaskTrdR&);
  /** Assignment Operator */
  CbmMcbm2018UnpackerTaskTrdR operator=(const CbmMcbm2018UnpackerTaskTrdR&);

  /**
	 * @brief Registers output-data containers at the FairRootManager.
	 *
	 * Called in CbmMcbm2018Source::Init() .
	 * @return kTRUE if successfull, kFALSE if not.
	 **/
  virtual Bool_t Init();

  /** Called in CbmMcbm2018Source::FillBuffer() for each timesclice */
  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

  /** Called in CbmMcbm2018Source::Reset() */
  virtual void Reset();

  // Called in CbmMcbm2018Source::Close() */
  virtual void Finish();

  /** Called in CbmMcbm2018Source::SetParUnpackers() */
  virtual void SetParContainers();

  virtual Bool_t InitContainers();

  virtual Bool_t ReInitContainers();

  /**
	 * @brief Adds an input component to the list of active components for this unpacker.
	 **/
  virtual void AddMsComponentToList(size_t component, UShort_t usDetectorId);

  /**
	 * @brief Sets numbers of Core Microslices and overlap Microslices per Timeslice.
	 **/
  virtual void SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb);

  void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  void SetDebugMonitorMode(Bool_t bFlagIn = kTRUE) { fbDebugMonitorMode = bFlagIn; }
  void SetWriteOutput(Bool_t bFlagIn = kTRUE) { fbWriteOutput = bFlagIn; }
  void SetDebugWriteOutput(Bool_t bFlagIn = kTRUE) { fbDebugWriteOutput = bFlagIn; }
  void SetDebugSortOutput(Bool_t bFlagIn = kTRUE) { fbDebugSortOutput = bFlagIn; }
  void SetSystemIdentifier(std::uint8_t id) { fSystemIdentifier = id; }
  void SetTimeOffsetNs(Double_t dOffsetIn = 0.0);

  /**
	 *  @brief Call this when Spadic Average-Baseline feature is enabled.
	 **/
  void SetBaselineAvg(Bool_t bFlagIn = kTRUE) { fbBaselineAvg = bFlagIn; }

  void SetActiveHistograms(std::vector<bool> isActiveHistoVec) { fIsActiveHistoVec = isActiveHistoVec; }
  void SetHistoFileName(TString filename);

  void SetMsSizeInNs(Double_t msSizeInNs) { fdMsSizeInNs = msSizeInNs; }  // TODO handle this with asic parameter files

  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE);

  void SetFirstChannelsElinkEven(bool isEven)
  {
    fIsFirstChannelsElinkEven = isEven;
  }  /// < Set wether channels 00..15 are on the even (true) or the odd (false and default) elink

private:
  // Control flags
  Bool_t fbMonitorMode;              ///< Switch ON the filling of a minimal set of histograms.
  Bool_t fbDebugMonitorMode;         ///< Switch ON the filling of a additional set of histograms.
  Bool_t fbWriteOutput;              ///< If ON the output Vector of digis is written to disk.
  Bool_t fbDebugWriteOutput;         ///< If ON the output vector of raw messages is filled and written to disk.
  Bool_t fbDebugSortOutput = kTRUE;  ///< If ON the output vector of raw messages is sorted in time.
  Bool_t fbBaselineAvg;              ///< Set to true if Baseline Averaging is activated in Spadic.
  std::uint8_t fSystemIdentifier;    ///< by default set to: fles::SubsystemIdentifier::TRD, changable via setter
  Double_t
    fdMsSizeInNs;  ///< microslice size in ns to be passed to the unpacker // TODO handle this with asic parameter files

  bool fIsFirstChannelsElinkEven =
    false;  ///< define if the first 16 channels (00..15) are found on the even (set true) or odd (false) eLinkId, default for mCbm2020 is false thus, initialized as false


  TString fMonitorHistoFileName;
  std::vector<bool> fIsActiveHistoVec;  // Define active histos in algo

  /// mCbm2020 timeshift correction parameters
  CbmMcbm2020TrdTshiftPar* fTimeshiftPar = nullptr;

  /// Output Digi vector
  std::vector<CbmTrdDigi>* fTrdDigiVector;

  /// Output Spadic raw messages for debugging
  std::vector<CbmTrdRawMessageSpadic>* fTrdRawMessageVector;

  /// vector< pair< fulltime, word > >
  std::vector<std::pair<size_t, size_t>>* fSpadicInfoMsgVector;

  /// Processing algo
  CbmMcbm2018UnpackerAlgoTrdR* fUnpackerAlgo;

  ClassDef(CbmMcbm2018UnpackerTaskTrdR, 1)
};

#endif
