/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifndef CBMMCBM2019TIMEWINEVENTBUILDERTASK_H
#define CBMMCBM2019TIMEWINEVENTBUILDERTASK_H

/// CBM headers
#include "CbmMcbm2019TimeWinEventBuilderAlgo.h"

/// FAIRROOT headers
#include "FairTask.h"

/// FAIRSOFT headers (geant, boost, ...)

/// C/C++ headers
#include "CbmTofDigi.h"

#include <tuple>

#include <array>
#include <map>
#include <set>
#include <vector>

class TClonesArray;

class CbmMcbm2019TimeWinEventBuilderTask : public FairTask {
public:
  /** Default constructor **/
  CbmMcbm2019TimeWinEventBuilderTask();

  CbmMcbm2019TimeWinEventBuilderTask(const CbmMcbm2019TimeWinEventBuilderTask&) = delete;
  CbmMcbm2019TimeWinEventBuilderTask operator=(const CbmMcbm2019TimeWinEventBuilderTask&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmMcbm2019TimeWinEventBuilderTask(Int_t verbose);


  /** Destructor **/
  ~CbmMcbm2019TimeWinEventBuilderTask();


  /** Initiliazation of task at the beginning of a run **/
  virtual InitStatus Init();

  /** ReInitiliazation of task when the runID changes **/
  virtual InitStatus ReInit();


  /** Executed for each event. **/
  virtual void Exec(Option_t*);

  /** Load the parameter container from the runtime database **/
  virtual void SetParContainers();

  /** Finish task called at the end of the run **/
  virtual void Finish();

  void SetFillHistos(Bool_t bFlag = kTRUE);
  void SetOutFilename(TString sNameIn);

  void SetReferenceDetector(EventBuilderDetector refDet);
  void AddDetector(EventBuilderDetector selDet);
  void RemoveDetector(EventBuilderDetector selDet);

  void SetTriggerMinNumber(ECbmModuleId selDet, UInt_t uVal);
  void SetTriggerMaxNumber(ECbmModuleId selDet, Int_t iVal);

  void SetTriggerWindow(ECbmModuleId det, Double_t dWinBeg, Double_t dWinEnd);

  void SetTsParameters(Double_t dTsStartTime, Double_t dTsLength, Double_t dTsOverLength);

  void SetEventOverlapMode(EOverlapMode mode);
  void SetIgnoreTsOverlap(Bool_t bFlagIn);

  void ChangeMuchBeamtimeDigiFlag(Bool_t bFlagIn = kFALSE);

private:
  void FillOutput();
  void SaveHistos();

  CbmMcbm2019TimeWinEventBuilderAlgo* fpAlgo = nullptr;

  TClonesArray* fEvents = nullptr;  //! output container of CbmEvents

  Bool_t fbFillHistos {kTRUE};  //! Switch ON/OFF filling of histograms

  /** Name of the histogram output file **/
  TString fsOutFileName {"data/HistosEvtWin.root"};

  ClassDef(CbmMcbm2019TimeWinEventBuilderTask, 1);
};

#endif  // CBMMCBM2019TIMEWINEVENTBUILDERTASK_H
