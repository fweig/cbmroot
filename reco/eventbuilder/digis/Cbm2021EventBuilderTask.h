/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMM2021EVENTBUILDERTASK_H
#define CBMM2021EVENTBUILDERTASK_H 

/// FAIRROOT headers
#include "FairTask.h"

/// FAIRSOFT headers (geant, boost, ...)

/// C/C++ headers
#include "CbmMuchBeamTimeDigi.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

#include <array>
#include <map>
#include <set>
#include <tuple>
#include <vector>

class CbmDigiManager;
class Cbm2021EventBuilderAlgo;
class EventBuilderDetector;
class TClonesArray;

class Cbm2021EventBuilderTask : public FairTask {
public:
  /** Default constructor **/
  Cbm2021EventBuilderTask();

  Cbm2021EventBuilderTask(
    const Cbm2021EventBuilderTask&) = delete;
  Cbm2021EventBuilderTask
  operator=(const Cbm2021EventBuilderTask&) = delete;

  /** Constructor with parameters (Optional) **/
  //  Cbm2021EventBuilderTask(Int_t verbose);


  /** Destructor **/
  ~Cbm2021EventBuilderTask();


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

  void SetTsParameters(Double_t dTsStartTime,
                       Double_t dTsLength,
                       Double_t dTsOverLength);

  void SetEventOverlapMode(EOverlapMode mode);
  void SetIgnoreTsOverlap(Bool_t bFlagIn);

  void ChangeMuchBeamtimeDigiFlag(Bool_t bFlagIn = kFALSE);

private:
  void FillOutput();
  void SaveHistos();

  CbmDigiManager* fDigiMan = nullptr;  
  std::vector<CbmMuchDigi>* fMuchDigis = nullptr;
  std::vector<CbmMuchBeamTimeDigi>* fMuchBeamTimeDigis = nullptr;
  std::vector<CbmStsDigi>* fStsDigis = nullptr;
  std::vector<CbmTrdDigi>* fTrdDigis = nullptr;
  std::vector<CbmTofDigi>* fTofDigis = nullptr;
  std::vector<CbmRichDigi>* fRichDigis = nullptr;
  std::vector<CbmPsdDigi>* fPsdDigis = nullptr;

  Bool_t fbUseMuchBeamtimeDigi = kTRUE;  //! Switch between MUCH digi classes

  Cbm2021EventBuilderAlgo* fpAlgo = nullptr;

  TClonesArray* fEvents = nullptr;  //! output container of CbmEvents

  Bool_t fbFillHistos {kTRUE};  //! Switch ON/OFF filling of histograms

  /** Name of the histogram output file **/
  TString fsOutFileName {"data/HistosEvtWin.root"};

  ClassDef(Cbm2021EventBuilderTask, 1);
};

#endif  // CBMM2021EVENTBUILDERTASK_H 
