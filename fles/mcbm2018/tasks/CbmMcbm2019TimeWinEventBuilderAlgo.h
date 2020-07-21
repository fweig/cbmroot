/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMMCBM2019TIMEWINEVENTBUILDERALGO_H
#define CBMMCBM2019TIMEWINEVENTBUILDERALGO_H

/// CBM headers
#include "CbmTofDigi.h"

/// FAIRROOT headers
#include "FairTask.h"

/// FAIRSOFT headers (geant, boost, ...)

/// C/C++ headers
#include <array>
#include <map>
#include <set>
#include <tuple>
#include <vector>

class TimesliceMetaData;
class CbmEvent;
class CbmDigiManager;
class TClonesArray;
class TH1;
class TH2;
class TNamed;
class TCanvas;

enum class EOverlapMode { NoOverlap, MergeOverlap, AllowOverlap };

class EventBuilderDetector {
public:
  EventBuilderDetector() { ; }
  EventBuilderDetector(ECbmModuleId detIdIn,
                       ECbmDataType dataTypeIn,
                       std::string sNameIn) {
    detId    = detIdIn;
    dataType = dataTypeIn;
    sName    = sNameIn;
  }
  EventBuilderDetector(ECbmModuleId detIdIn,
                       ECbmDataType dataTypeIn,
                       std::string sNameIn,
                       UInt_t uTriggerMinDigisIn,
                       Int_t iTriggerMaxDigisIn,
                       Double_t fdTimeWinBegIn,
                       Double_t fdTimeWinEndIn)
    : EventBuilderDetector(detIdIn, dataTypeIn, sNameIn) {
    fuTriggerMinDigis = uTriggerMinDigisIn;
    fiTriggerMaxDigis = iTriggerMaxDigisIn;

    fdTimeWinBeg = fdTimeWinBegIn;
    fdTimeWinEnd = fdTimeWinEndIn;
  }

  bool operator==(const EventBuilderDetector& other) const {
    return (other.detId == this->detId);
  }
  bool operator!=(const EventBuilderDetector& other) const {
    return (other.detId != this->detId);
  }

  /// Settings
  ECbmModuleId detId    = ECbmModuleId::kNotExist;
  ECbmDataType dataType = ECbmDataType::kUnknown;
  std::string sName     = "Invalid";
  /// Minimum number of T0 digis needed to generate a trigger, 0 means don't use for trigger generation
  UInt_t fuTriggerMinDigis = 0;
  /// Maximum number of digis per detector to generate an event, -1 means no cut, 0 means anti-coinc trigger
  Int_t fiTriggerMaxDigis = -1;
  /// Selection Window
  Double_t fdTimeWinBeg = -100;
  Double_t fdTimeWinEnd = 100;

  /// Book-keeping variables
  UInt_t fuStartIndex = 0;
  UInt_t fuEndIndex   = 0;
};

/// Pre-defined detector types
static const EventBuilderDetector kEventBuilderDetSts =
  EventBuilderDetector(ECbmModuleId::kSts, ECbmDataType::kStsDigi, "Sts");
static const EventBuilderDetector kEventBuilderDetMuch =
  EventBuilderDetector(ECbmModuleId::kMuch, ECbmDataType::kMuchDigi, "Much");
static const EventBuilderDetector kEventBuilderDetTrd =
  EventBuilderDetector(ECbmModuleId::kTrd, ECbmDataType::kTrdDigi, "Trd");
static const EventBuilderDetector kEventBuilderDetTof =
  EventBuilderDetector(ECbmModuleId::kTof, ECbmDataType::kTofDigi, "Tof");
static const EventBuilderDetector kEventBuilderDetRich =
  EventBuilderDetector(ECbmModuleId::kRich, ECbmDataType::kRichDigi, "Rich");
static const EventBuilderDetector kEventBuilderDetPsd =
  EventBuilderDetector(ECbmModuleId::kPsd, ECbmDataType::kPsdDigi, "Psd");
static const EventBuilderDetector kEventBuilderDetT0 =
  EventBuilderDetector(ECbmModuleId::kT0, ECbmDataType::kT0Digi, "T0");
static const EventBuilderDetector kEventBuilderDetUndef =
  EventBuilderDetector();

class CbmMcbm2019TimeWinEventBuilderAlgo {
public:
  /** Default constructor **/
  CbmMcbm2019TimeWinEventBuilderAlgo();

  CbmMcbm2019TimeWinEventBuilderAlgo(
    const CbmMcbm2019TimeWinEventBuilderAlgo&) = delete;
  CbmMcbm2019TimeWinEventBuilderAlgo
  operator=(const CbmMcbm2019TimeWinEventBuilderAlgo&) = delete;

  /** Destructor **/
  ~CbmMcbm2019TimeWinEventBuilderAlgo();

  /** Initiliazation at the beginning of a run **/
  Bool_t InitAlgo();

  /** Executed for each TS. **/
  void ProcessTs();

  /** Finish called at the end of the run **/
  void Finish();

  void SetFillHistos(Bool_t var) { fbFillHistos = var; }
  void ResetHistograms(Bool_t bResetTime = kTRUE);

  void SetReferenceDetector(ECbmModuleId refDet,
                            ECbmDataType dataTypeIn,
                            std::string sNameIn,
                            UInt_t uTriggerMinDigisIn = 0,
                            Int_t iTriggerMaxDigisIn  = -1,
                            Double_t fdTimeWinBegIn   = -100,
                            Double_t fdTimeWinEndIn   = 100);
  void AddDetector(ECbmModuleId selDet,
                   ECbmDataType dataTypeIn,
                   std::string sNameIn,
                   UInt_t uTriggerMinDigisIn = 0,
                   Int_t iTriggerMaxDigisIn  = -1,
                   Double_t fdTimeWinBegIn   = -100,
                   Double_t fdTimeWinEndIn   = 100);

  void SetReferenceDetector(ECbmModuleId refDet) __attribute__((deprecated));
  void AddDetector(ECbmModuleId selDet) __attribute__((deprecated));
  void RemoveDetector(ECbmModuleId selDet) __attribute__((deprecated));

  void SetReferenceDetector(EventBuilderDetector refDetIn);
  void AddDetector(EventBuilderDetector selDet);
  void RemoveDetector(EventBuilderDetector selDet);

  void SetTriggerMinNumber(ECbmModuleId selDet, UInt_t uVal);
  void SetTriggerMaxNumber(ECbmModuleId selDet, Int_t iVal);

  void
  SetTriggerWindow(ECbmModuleId selDet, Double_t dWinBeg, Double_t dWinEnd);

  /// Control flags
  void SetEventOverlapMode(EOverlapMode mode) { fOverMode = mode; }
  void SetIgnoreTsOverlap(Bool_t bFlagIn = kTRUE) {
    fbIgnoreTsOverlap = bFlagIn;
  }

  /// For monitor algos
  void AddHistoToVector(TNamed* pointer, std::string sFolder = "") {
    fvpAllHistoPointers.push_back(
      std::pair<TNamed*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TNamed*, std::string>> GetHistoVector() {
    return fvpAllHistoPointers;
  }
  void AddCanvasToVector(TCanvas* pointer, std::string sFolder = "") {
    fvpAllCanvasPointers.push_back(
      std::pair<TCanvas*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TCanvas*, std::string>> GetCanvasVector() {
    return fvpAllCanvasPointers;
  }

  /// Data output access
  std::vector<CbmEvent*>& GetEventVector() { return fEventVector; }
  void ClearEventVector();

private:
  /// Internal methods
  Bool_t CheckDataAvailable(EventBuilderDetector& det);
  void InitTs();
  void BuildEvents();

  void CreateHistograms();
  void FillHistos();

  template<class DigiSeed>
  void LoopOnSeeds();
  void CheckSeed(Double_t dSeedTime, UInt_t uSeedDigiIdx);
  template<class DigiCheck>
  void SearchMatches(Double_t dSeedTime, EventBuilderDetector& detMatch);
  void AddDigiToEvent(EventBuilderDetector& det, Int_t uIdx);
  Bool_t HasTrigger(CbmEvent*);
  Bool_t CheckTriggerConditions(CbmEvent* event, EventBuilderDetector& det);

  void UpdateTimeWinBoundariesExtrema();
  void UpdateWidestTimeWinRange();

  /// Constants
  static constexpr Double_t kdDefaultTimeWinBeg = -100.0;
  static constexpr Double_t kdDefaultTimeWinEnd = 100.0;

  /// User parameters
  /// Control flags
  Bool_t fbIgnoreTsOverlap = kFALSE;  //! Ignore data in Overlap part of the TS
  Bool_t fbFillHistos {kTRUE};        //! Switch ON/OFF filling of histograms
    /// Event building mode and detectors selection
  EOverlapMode fOverMode {EOverlapMode::AllowOverlap};

  EventBuilderDetector fRefDet =
    EventBuilderDetector(ECbmModuleId::kT0, ECbmDataType::kT0Digi, "T0");
  std::vector<EventBuilderDetector> fvDets = {
    EventBuilderDetector(ECbmModuleId::kSts, ECbmDataType::kStsDigi, "kSts"),
    EventBuilderDetector(ECbmModuleId::kMuch, ECbmDataType::kMuchDigi, "kMuch"),
    EventBuilderDetector(ECbmModuleId::kTrd, ECbmDataType::kTrdDigi, "kTrd"),
    EventBuilderDetector(ECbmModuleId::kTof, ECbmDataType::kTofDigi, "kTof"),
    EventBuilderDetector(ECbmModuleId::kRich, ECbmDataType::kRichDigi, "kRich"),
    EventBuilderDetector(ECbmModuleId::kPsd, ECbmDataType::kPsdDigi, "kPsd")};

  Double_t fdEarliestTimeWinBeg = kdDefaultTimeWinBeg;
  Double_t fdLatestTimeWinEnd   = kdDefaultTimeWinEnd;
  Double_t fdWidestTimeWinRange = kdDefaultTimeWinEnd - kdDefaultTimeWinBeg;

  /// Data input
  /// FIXME: usage of CbmDigiManager in FairMq context?!?
  ///        => Maybe by registering vector (or vector reference) to ioman in Device?
  CbmDigiManager* fDigiMan                  = nullptr;  //!
  const std::vector<CbmTofDigi>* fT0DigiVec = nullptr;  //!
  TClonesArray* fTimeSliceMetaDataArray     = nullptr;  //!
  const TimesliceMetaData* pTsMetaData      = nullptr;

  /// Data ouptut
  CbmEvent* fCurrentEvent =
    nullptr;  //! pointer to the event which is currently build
  std::vector<CbmEvent*> fEventVector = {};  //! vector with all created events

  /// Monitoring histograms
  /// => Pointers should be filled with TH1*, TH2*, TProfile*, ...
  /// ==> To check if object N is of type T, use "T ObjectPointer = dynamic_cast<T>( fvpAllHistoPointers[N].first );" and check for nullptr
  /// ==> To get back the original class name use "fvpAllHistoPointers[N].first->ClassName()" which returns a const char * (e.g. "TH1I")
  /// ===> Usage example with feeding a THttpServer:
  /// ===> #include "TH2.h"
  /// ===> std::string sClassName = vHistos[ uHisto ].first.ClassName();
  /// ===> if( !strncmp( sClassName, "TH1", 3 ) )
  /// ===>    server->Register( vHistos[ uHisto ].second.data(), dynamic_cast< TH1 * >(vHistos[ uHisto ].first) );
  /// ===> else if( !strncmp( sClassName, "TH2", 3 ) )
  /// ===>    server->Register( vHistos[ uHisto ].second.data(), dynamic_cast< TH2 * >(vHistos[ uHisto ].first) );
  std::vector<std::pair<TNamed*, std::string>>
    fvpAllHistoPointers;  //! Vector of pointers to histograms + optional folder name
  std::vector<std::pair<TCanvas*, std::string>>
    fvpAllCanvasPointers;  //! Vector of pointers to canvases + optional folder name

  TH1* fhEventTime = nullptr;  //! histogram with the seed time of the events
  TH1* fhEventDt =
    nullptr;  //! histogram with the interval in seed time of consecutive events
  TH1* fhEventSize =
    nullptr;  //! histogram with the nb of all  digis in the event
  TH2* fhNbDigiPerEvtTime =
    nullptr;  //! histogram with the nb of all  digis per event vs seed time of the events
  std::vector<TH2*> fvhNbDigiPerEvtTimeDet =
    {};  //! histograms with the nb of digis in each detector per event vs seed time of the events

  /// Internal state variables
  UInt_t fuCurEv         = 0;   //! Event Counter
  UInt_t fuErrors        = 0;   //! Error Counter
  UInt_t fuNrTs          = 0;   //! Timeslice Counter
  Double_t fdPrevEvtTime = 0.;  //! Save previous time information
  Double_t fdPrevEvtEndTime =
    0.;  //! Save previous event last digi time information

  ClassDefNV(CbmMcbm2019TimeWinEventBuilderAlgo, 1);
};

#endif  // CBMMCBM2019TIMEWINEVENTBUILDERALGO_H
