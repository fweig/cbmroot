/* Copyright (C) 2007-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#ifndef CBMTASKBUILDRAWEVENTS_H
#define CBMTASKBUILDRAWEVENTS_H

/// FAIRROOT headers
#include "FairTask.h"

/// FAIRSOFT headers (geant, boost, ...)

/// CBMROOT headers
#include "CbmAlgoBuildRawEvents.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

/// C/C++ headers
#include <tuple>

#include <array>
#include <map>
#include <set>
#include <vector>

class CbmDigiManager;
class CbmMatch;
class CbmSeedFinderSlidingWindow;
class RawEventBuilderDetector;
class TClonesArray;
class TStopwatch;

enum class EOverlapModeRaw;

class CbmTaskBuildRawEvents : public FairTask {
public:
  /** Default constructor **/
  CbmTaskBuildRawEvents();

  CbmTaskBuildRawEvents(const CbmTaskBuildRawEvents&) = delete;
  CbmTaskBuildRawEvents operator=(const CbmTaskBuildRawEvents&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmTaskBuildRawEvents(Int_t verbose);

  /** Destructor **/
  ~CbmTaskBuildRawEvents();

  /** Initiliazation of task at the beginning of a run **/
  virtual InitStatus Init();

  /** ReInitiliazation of task when the runID changes **/
  virtual InitStatus ReInit();

  /** Executed for each event. **/
  virtual void Exec(Option_t*);

  /** Finish task called at the end of the run **/
  virtual void Finish();

  /** Setters **/
  void SetOutFilename(TString sNameIn) { fsOutFileName = sNameIn; }
  void SetWriteHistosToFairSink(Bool_t var) { fbWriteHistosToFairSink = var; }

  void SetFillHistos(Bool_t bFlag = kTRUE)
  {
    fbFillHistos = bFlag;
    if (nullptr != fpAlgo) fpAlgo->SetFillHistos(fbFillHistos);
  }
  void SetReferenceDetector(RawEventBuilderDetector refDet)
  {
    if (nullptr != fpAlgo) fpAlgo->SetReferenceDetector(refDet);
  }
  void AddDetector(RawEventBuilderDetector selDet)
  {
    if (nullptr != fpAlgo) fpAlgo->AddDetector(selDet);
  }
  void RemoveDetector(RawEventBuilderDetector selDet)
  {
    if (nullptr != fpAlgo) fpAlgo->RemoveDetector(selDet);
  }
  void SetTriggerMinNumber(ECbmModuleId selDet, UInt_t uVal)
  {
    if (nullptr != fpAlgo) fpAlgo->SetTriggerMinNumber(selDet, uVal);
  }
  void SetTriggerMaxNumber(ECbmModuleId selDet, Int_t iVal)
  {
    if (nullptr != fpAlgo) fpAlgo->SetTriggerMaxNumber(selDet, iVal);
  }
  void SetTriggerWindow(ECbmModuleId det, Double_t dWinBeg, Double_t dWinEnd)
  {
    if (nullptr != fpAlgo) fpAlgo->SetTriggerWindow(det, dWinBeg, dWinEnd);
  }
  void SetTsParameters(Double_t dTsStartTime, Double_t dTsLength, Double_t dTsOverLength)
  {
    if (nullptr != fpAlgo) fpAlgo->SetTsParameters(dTsStartTime, dTsLength, dTsOverLength);
  }
  void SetEventOverlapMode(EOverlapModeRaw mode)
  {
    if (nullptr != fpAlgo) fpAlgo->SetEventOverlapMode(mode);
  }
  void SetIgnoreTsOverlap(Bool_t bFlagIn)
  {
    if (nullptr != fpAlgo) fpAlgo->SetIgnoreTsOverlap(bFlagIn);
  }
  void ChangeMuchBeamtimeDigiFlag(Bool_t bFlagIn = kFALSE)
  {
    if (nullptr != fpAlgo) fpAlgo->ChangeMuchBeamtimeDigiFlag(bFlagIn);
    fbUseMuchBeamtimeDigi = bFlagIn;
  }
  void SetTimings(Bool_t bFlagIn = kTRUE)
  {
    if (nullptr != fpAlgo) fpAlgo->SetTimings(bFlagIn);
    fbGetTimings = bFlagIn;
  }

  void SetSeedFinderQa(Bool_t bFlagIn = kTRUE);
  void PrintTimings();
  void AddSeedTimeFillerToList(RawEventBuilderDetector seedDet);
  void SetSlidingWindowSeedFinder(int32_t minDigis, double dWindDur, double dDeadT, double dOffset = 0.0);
  void SetIdealSeedFinder(const int32_t fileId = -1);

  void DumpSeedTimesFromDetList();
  void SetSeedTimeWindow(Double_t beg, Double_t end) { fpAlgo->SetSeedTimeWindow(beg, end); }

private:
  void FillOutput();
  void SaveHistos();

  Bool_t fbUseMuchBeamtimeDigi = kTRUE;  //! Switch between MUCH digi classes

  CbmSeedFinderSlidingWindow* fSeedFinderSlidingWindow = nullptr;

  CbmDigiManager* fDigiMan                             = nullptr;
  const std::vector<CbmTofDigi>* fT0Digis              = nullptr;
  std::vector<CbmMuchDigi>* fMuchDigis                 = nullptr;
  std::vector<CbmMuchBeamTimeDigi>* fMuchBeamTimeDigis = nullptr;
  std::vector<CbmStsDigi>* fStsDigis                   = nullptr;
  std::vector<CbmTrdDigi>* fTrdDigis                   = nullptr;
  std::vector<CbmTofDigi>* fTofDigis                   = nullptr;
  std::vector<CbmRichDigi>* fRichDigis                 = nullptr;
  std::vector<CbmPsdDigi>* fPsdDigis                   = nullptr;
  std::vector<Double_t>* fSeedTimes                    = nullptr;

  std::vector<Double_t>* fTempDigiTimes =
    nullptr;  //used when multiple seed detectors are combined with sliding window seed finder

  std::vector<RawEventBuilderDetector> fSeedTimeDetList;  //for multiple seed detectors

  // Store digi matches for QA tasks
  std::vector<CbmMatch>* fvDigiMatchQa = nullptr;

  Double_t GetDigiTime(ECbmModuleId _system, UInt_t _entry);
  UInt_t GetNofDigis(ECbmModuleId _system);

  void FillSeedTimesFromDetList(std::vector<Double_t>* vdSeedTimes, std::vector<CbmMatch>* vDigiMatch = nullptr);
  void FillSeedTimesFromSlidingWindow();
  void FillSeedTimesFromSlidingWindow(const RawEventBuilderDetector* seedDet);

  TStopwatch* fTimer     = nullptr;  //! is created when fbGetTimings is set before init
  TStopwatch* fCopyTimer = nullptr;  //! timing only for filling of std::vector<Digi> fields

  CbmAlgoBuildRawEvents* fpAlgo = nullptr;

  TClonesArray* fEvents = nullptr;  //! output container of CbmEvents

  Bool_t fbFillHistos {kTRUE};             //! Switch ON/OFF filling of histograms
  Bool_t fbWriteHistosToFairSink {kTRUE};  //! Write histos to FairRootManager instead of separate file
  Bool_t fbGetTimings = kFALSE;            //! Measure CPU time using stopwatch

  /** Name of the histogram output file **/
  TString fsOutFileName {"data/HistosEvtWin.root"};

  Int_t fNofTs        = 0;
  Long64_t fNofEvents = 0;
  Double_t fTime      = 0.;

  /** Name of the histogram output file **/
  uint64_t fTotalSeedCount = 0;

  ClassDef(CbmTaskBuildRawEvents, 1);
};

#endif  // CBMTASKBUILDRAWEVENTS_H
