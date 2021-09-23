/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifndef CBMMCBMCHECKTIMINGTASK_H
#define CBMMCBMCHECKTIMINGTASK_H

/// CBM headers
#include "CbmMcbmCheckTimingAlgo.h"

/// FAIRROOT headers
#include "FairTask.h"

/// FAIRSOFT headers (geant, boost, ...)

/// C/C++ headers

class TClonesArray;

class CbmMcbmCheckTimingTask : public FairTask {
public:
  /** Default constructor **/
  CbmMcbmCheckTimingTask();

  CbmMcbmCheckTimingTask(const CbmMcbmCheckTimingTask&) = delete;
  CbmMcbmCheckTimingTask operator=(const CbmMcbmCheckTimingTask&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmMcbmCheckTimingTask(Int_t verbose);


  /** Destructor **/
  ~CbmMcbmCheckTimingTask();


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

  void SetOutFilename(TString sNameIn);

  void SetReferenceDetector(ECbmModuleId refDetIn, std::string sNameIn, Double_t dTimeRangeBegIn = -1000.0,
                            Double_t dTimeRangeEndIn = 1000.0, UInt_t uRangeNbBinsIn = 320, UInt_t uChargeCutMinIn = 0,
                            UInt_t uChargeCutMaxIn = 0);
  void AddCheckDetector(ECbmModuleId detIn, std::string sNameIn, Double_t dTimeRangeBegIn = -1000.0,
                        Double_t dTimeRangeEndIn = 1000.0, UInt_t uRangeNbBinsIn = 320, UInt_t uChargeCutMinIn = 0,
                        UInt_t uChargeCutMaxIn = 0);
  void RemoveCheckDetector(ECbmModuleId detIn);

private:
  void SaveHistos();

  CbmMcbmCheckTimingAlgo* fpAlgo = nullptr;

  /** Name of the histogram output file **/
  TString fsOutFileName = "data/HistosCheckTiming.root";

  ClassDef(CbmMcbmCheckTimingTask, 1);
};

#endif  // CBMMCBM2019TIMEWINEVENTBUILDERTASK_H
