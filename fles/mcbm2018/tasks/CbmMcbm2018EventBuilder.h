/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMMCBM2018EVENTBUILDER_H
#define CBMMCBM2018EVENTBUILDER_H

#include "FairTask.h"

#include "CbmDefs.h"
#include "CbmEvent.h"

#include "CbmTofDigi.h"
#include <array>
#include <map>
#include <set>
#include <tuple>
#include <vector>

class TClonesArray;
class TH1;
class TH2;
class CbmDigiManager;


enum class EventBuilderAlgo { FixedTimeWindow, MaximumTimeGap };

//typedef std::tuple<ECbmModuleId, Int_t> digituple;

typedef std::pair<ECbmModuleId, Int_t> digituple;

/*
struct classcomp {
  bool operator() (const digituple& lhs, const digituple& rhs) const
  {
//    CbmDigi* digi_lhs = std::get<0>(lhs);
//    CbmDigi* digi_rhs = std::get<0>(rhs);
//    Double_t time_lhs = std::get<0>(lhs)->GetTime();
//    Double_t time_rhs = std::get<0>(rhs)->GetTime();
    return std::get<0>(lhs)->GetTime() < std::get<0>(rhs)->GetTime();
  }
};
*/

class CbmMcbm2018EventBuilder : public FairTask {
public:
  /** Default constructor **/
  CbmMcbm2018EventBuilder();

  CbmMcbm2018EventBuilder(const CbmMcbm2018EventBuilder&) = delete;
  CbmMcbm2018EventBuilder operator=(const CbmMcbm2018EventBuilder&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmMcbm2018EventBuilder(Int_t verbose);


  /** Destructor **/
  ~CbmMcbm2018EventBuilder();


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

  void SetFillHistos(Bool_t var) { fFillHistos = var; }
  void SetOutFilename(TString sNameIn) { fOutFileName = sNameIn; }

  void SetEventBuilderAlgo(
    EventBuilderAlgo algo = EventBuilderAlgo::FixedTimeWindow) {
    fEventBuilderAlgo = algo;
  }
  void SetFixedTimeWindow(Double_t val) { fFixedTimeWindow = val; }
  void SetMaximumTimeGap(Double_t val) { fMaximumTimeGap = val; }

  /** Minimum number of T0 digis needed to generate a trigger, 0 means don't use T0 for trigger generation **/
  void SetTriggerMinNumberT0(Int_t val) { fTriggerMinT0Digis = val; }
  /** Minimum number of Sts digis needed to generate a trigger, 0 means don't use Sts for trigger generation **/
  void SetTriggerMinNumberSts(Int_t val) { fTriggerMinStsDigis = val; }
  /** Minimum number of Much digis needed to generate a trigger, 0 means don't use Much for trigger generation **/
  void SetTriggerMinNumberMuch(Int_t val) { fTriggerMinMuchDigis = val; }
  /** Minimum number of Trd digis needed to generate a trigger, 0 means don't use Trd for trigger generation **/
  void SetTriggerMinNumberTrd(Int_t val) { fTriggerMinTrdDigis = val; }
  /** Minimum number of Tof digis needed to generate a trigger, 0 means don't use Tof for trigger generation **/
  void SetTriggerMinNumberTof(Int_t val) { fTriggerMinTofDigis = val; }
  /** Minimum number of Rich digis needed to generate a trigger, 0 means don't use Rich for trigger generation **/
  void SetTriggerMinNumberRich(Int_t val) { fTriggerMinRichDigis = val; }
  /** Minimum number of Psd digis needed to generate a trigger, 0 means don't use Psd for trigger generation **/
  void SetTriggerMinNumberPsd(Int_t val) { fTriggerMinPsdDigis = val; }

  /** Maximum number of T0 digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  void SetTriggerMaxNumberT0(Int_t val) { fTriggerMaxT0Digis = val; }
  /** Maximum number of Sts digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  void SetTriggerMaxNumberSts(Int_t val) { fTriggerMaxStsDigis = val; }
  /** Maximum number of Much digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  void SetTriggerMaxNumberMuch(Int_t val) { fTriggerMaxMuchDigis = val; }
  /** Maximum number of Trd digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  void SetTriggerMaxNumberTrd(Int_t val) { fTriggerMaxTrdDigis = val; }
  /** Maximum number of Tof digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  void SetTriggerMaxNumberTof(Int_t val) { fTriggerMaxTofDigis = val; }
  /** Maximum number of Rich digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  void SetTriggerMaxNumberRich(Int_t val) { fTriggerMaxRichDigis = val; }
  /** Maximum number of Psd digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  void SetTriggerMaxNumberPsd(Int_t val) { fTriggerMaxPsdDigis = val; }

  void SetUseBaseMuchDigi(Bool_t bFlag = kTRUE) { fbUseBaseMuchDigi = bFlag; }

private:
  void InitSorter();
  void BuildEvents();
  void FillHisto();
  void DefineGoodEvents();
  void FillOutput();
  template<class Digi>
  void AddDigiToSorter(ECbmModuleId, Int_t);
  void AddDigiToEvent(ECbmModuleId, Int_t);

  Bool_t IsDigiInEvent(Double_t);
  Bool_t HasTrigger(CbmEvent*);


  Int_t fCurEv {0};         //! Event Counter
  Int_t fErrors {0};        //! Error Counter
  Int_t fNrTs {0};          //! Timeslice Counter
  Double_t fPrevTime {0.};  //! Save previous time information

  Bool_t fbUseBaseMuchDigi                  = kFALSE;
  CbmDigiManager* fDigiMan                  = nullptr;  //!
  const std::vector<CbmTofDigi>* fT0DigiVec = nullptr;  //!
  TClonesArray* fT0DigiArr = nullptr;  //! input container of TO digis
  TClonesArray* fEvents    = nullptr;  //! output container of CbmEvents

  std::array<TClonesArray*, ToIntegralType(ECbmModuleId::kNofSystems)>
    fLinkArray;  //! array with pointers to input containers

  //std::multiset<digituple, classcomp> fSorter; //! std::set to sort the digis time wise
  std::multimap<Double_t, digituple> fSorter;  //! for digi sorting

  std::vector<std::pair<ECbmModuleId, Int_t>> fVect;  //!

  CbmEvent* fCurrentEvent {
    nullptr};  //! pointer to the event which is currently build
  std::vector<CbmEvent*> fEventVector;  //! vector with all created events

  TH1* fDiffTime {
    nullptr};  //! histogram with the time difference between two consecutive digis
  TH1* fhEventTime {nullptr};  //! histogram with the seed time of the events
  TH1* fhEventDt {
    nullptr};  //! histogram with the interval in seed time of consecutive events
  TH1* fhEventSize {
    nullptr};  //! histogram with the nb of all  digis in the event
  TH2* fhNbDigiPerEvtTime {
    nullptr};  //! histogram with the nb of all  digis per event vs seed time of the events
  TH2* fhNbDigiPerEvtTimeT0 {
    nullptr};  //! histogram with the nb of T0   digis per event vs seed time of the events
  TH2* fhNbDigiPerEvtTimeSts {
    nullptr};  //! histogram with the nb of STS  digis per event vs seed time of the events
  TH2* fhNbDigiPerEvtTimeMuch {
    nullptr};  //! histogram with the nb of MUCH digis per event vs seed time of the events
  TH2* fhNbDigiPerEvtTimeTrd {
    nullptr};  //! histogram with the nb of TRD  digis per event vs seed time of the events
  TH2* fhNbDigiPerEvtTimeTof {
    nullptr};  //! histogram with the nb of TOF  digis per event vs seed time of the events
  TH2* fhNbDigiPerEvtTimeRich {
    nullptr};  //! histogram with the nb of RICH digis per event vs seed time of the events
  TH2* fhNbDigiPerEvtTimePsd {
    nullptr};  //! histogram with the nb of PSD  digis per event vs seed time of the events
  Bool_t fFillHistos {kTRUE};  //! Switch ON/OFF filling of histograms

  /** Used event building algorithm **/
  EventBuilderAlgo fEventBuilderAlgo {EventBuilderAlgo::FixedTimeWindow};
  /** Size of the time window used for the FixedTimeWindow event building algorithm **/
  Double_t fFixedTimeWindow {100.};
  /** Start time of the event, needed for the FixedTimeWindow event building algorithm **/
  Double_t fStartTimeEvent {0.};
  /** Maximum gap allowed between two consecutive digis  used for the MaximumTimeGap event building algorithm **/
  Double_t fMaximumTimeGap {100.};

  /** Minimum number of T0 digis needed to generate a trigger, 0 means don't use T0 for trigger generation **/
  Int_t fTriggerMinT0Digis {0};
  /** Minimum number of Sts digis needed to generate a trigger, 0 means don't use Sts for trigger generation **/
  Int_t fTriggerMinStsDigis {0};
  /** Minimum number of Much digis needed to generate a trigger, 0 means don't use Much for trigger generation **/
  Int_t fTriggerMinMuchDigis {0};
  /** Minimum number of Trd digis needed to generate a trigger, 0 means don't use Trd for trigger generation **/
  Int_t fTriggerMinTrdDigis {0};
  /** Minimum number of Tof digis needed to generate a trigger, 0 means don't use Tof for trigger generation **/
  Int_t fTriggerMinTofDigis {0};
  /** Minimum number of Rich digis needed to generate a trigger, 0 means don't use Rich for trigger generation **/
  Int_t fTriggerMinRichDigis {0};
  /** Minimum number of Psd digis needed to generate a trigger, 0 means don't use Psd for trigger generation **/
  Int_t fTriggerMinPsdDigis {0};
  /** Maximum number of T0 digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  Int_t fTriggerMaxT0Digis = -1;
  /** Maximum number of Sts digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  Int_t fTriggerMaxStsDigis = -1;
  /** Maximum number of Much digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  Int_t fTriggerMaxMuchDigis = -1;
  /** Maximum number of Trd digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  Int_t fTriggerMaxTrdDigis = -1;
  /** Maximum number of Tof digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  Int_t fTriggerMaxTofDigis = -1;
  /** Maximum number of Rich digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  Int_t fTriggerMaxRichDigis = -1;
  /** Maximum number of Psd digis needed to generate a trigger, -1 means no cut, 0 means anti-coinc trigger **/
  Int_t fTriggerMaxPsdDigis = -1;

  /** Name of the histogram output file **/
  TString fOutFileName {"HistosEventBuilder.root"};

  ClassDef(CbmMcbm2018EventBuilder, 2);
};

#endif
