/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMCHECKEVENTS_H
#define CBMCHECKEVENTS_H

#include "FairTask.h"

#include "CbmDefs.h"

#include "CbmTofDigi.h"
#include "TString.h"
#include <vector>

class CbmEvent;

class TClonesArray;
class TH1;
class TH2;
class TProfile;
class CbmDigiManager;

class CbmCheckEvents : public FairTask {
public:
  /** Default constructor **/
  CbmCheckEvents();

  CbmCheckEvents(const CbmCheckEvents&) = delete;
  CbmCheckEvents operator=(const CbmCheckEvents&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmCheckEvents(Int_t verbose);


  /** Destructor **/
  ~CbmCheckEvents();


  /** Initialization of task at the beginning of a run **/
  virtual InitStatus Init();

  /** ReInitiliazation of task when the runID changes **/
  virtual InitStatus ReInit();


  /** Executed for each event. **/
  virtual void Exec(Option_t*);

  /** Load the parameter container from the runtime database **/
  virtual void SetParContainers();

  /** Finish task called at the end of the run **/
  virtual void Finish();

private:
  void CreateHistos();
  void AnalyseEvent(CbmEvent* event);

  void GetTimeDiffT0(CbmEvent*, TH1*, TH1*);

  template<class Digi>
  void GetTimeDiff(CbmEvent* event, TH1*, TH1*, ECbmDataType type);

  CbmDigiManager* fDigiMan = nullptr;  //! Interface to digi data
  /** T0 is not included in CbmDigiManager, so add it explicitly here **/
  const std::vector<CbmTofDigi>* fT0DigiVec = nullptr;  //!
  TClonesArray* fT0DigiArr {nullptr};                   //!
  TClonesArray* fEvents {nullptr};

  // Variables to store the previous digi time
  Double_t fPrevEventTime {0.};

  //
  Int_t fNrTs {0};

  // Histograms
  TH1* fEventSize   = nullptr;
  TH1* fEventLength = nullptr;
  TH1* fEventsPerTS = nullptr;
  TH1* fT0InEvent   = nullptr;
  TH1* fStsInEvent  = nullptr;
  TH1* fMuchInEvent = nullptr;
  TH1* fTofInEvent  = nullptr;

  TH1* fT0DeltaT   = nullptr;
  TH1* fStsDeltaT  = nullptr;
  TH1* fMuchDeltaT = nullptr;
  TH1* fTofDeltaT  = nullptr;

  TH2* fEventsvsTS      = nullptr;
  TProfile* fLengthvsTS = nullptr;

  ClassDef(CbmCheckEvents, 1);
};

#endif
