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

  void SetReferenceDetector(ECbmModuleId refDet);
  void AddDetector(ECbmModuleId selDet);
  void RemoveDetector(ECbmModuleId selDet);

  void SetTriggerMinNumber(ECbmModuleId selDet, UInt_t uVal);
  void SetTriggerMaxNumber(ECbmModuleId selDet, Int_t iVal);

  void SetTriggerWindow(ECbmModuleId det, Double_t dWinBeg, Double_t dWinEnd);

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
  void InitTs();
  void BuildEvents();

  void CreateHistograms();
  void FillHistos();

  template<class DigiSeed>
  void LoopOnSeeds();
  void CheckSeed(Double_t dSeedTime, UInt_t uSeedDigiIdx);
  template<class DigiCheck>
  void
  SearchMatches(Double_t dSeedTime, ECbmModuleId detMatch, UInt_t& uStartIndex);
  void AddDigiToEvent(ECbmModuleId det, Int_t uIdx);
  Bool_t HasTrigger(CbmEvent*);
  Bool_t CheckTriggerConditions(CbmEvent* event,
                                ECbmModuleId det,
                                ECbmDataType dataType);

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
  ECbmModuleId fRefDet {ECbmModuleId::kT0};
  std::vector<ECbmModuleId> fvDets {ECbmModuleId::kSts,
                                    ECbmModuleId::kMuch,
                                    ECbmModuleId::kTrd,
                                    ECbmModuleId::kTof,
                                    ECbmModuleId::kRich,
                                    ECbmModuleId::kPsd};
  /// Event building trigger parameters
  /** Minimum number of T0 digis needed to generate a trigger, 0 means don't use T0 for trigger generation **/
  UInt_t fuTriggerMinT0Digis {0};
  /** Minimum number of Sts digis needed to generate a trigger, 0 means don't use Sts for trigger generation **/
  UInt_t fuTriggerMinStsDigis {0};
  /** Minimum number of Much digis needed to generate a trigger, 0 means don't use Much for trigger generation **/
  UInt_t fuTriggerMinMuchDigis {0};
  /** Minimum number of Trd digis needed to generate a trigger, 0 means don't use Trd for trigger generation **/
  UInt_t fuTriggerMinTrdDigis {0};
  /** Minimum number of Tof digis needed to generate a trigger, 0 means don't use Tof for trigger generation **/
  UInt_t fuTriggerMinTofDigis {0};
  /** Minimum number of Rich digis needed to generate a trigger, 0 means don't use Rich for trigger generation **/
  UInt_t fuTriggerMinRichDigis {0};
  /** Minimum number of Psd digis needed to generate a trigger, 0 means don't use Psd for trigger generation **/
  UInt_t fuTriggerMinPsdDigis {0};
  /// Maximum number of digis per detector to generate an event, -1 means no cut, 0 means anti-coinc trigger
  Int_t fiTriggerMaxT0Digis   = -1;
  Int_t fiTriggerMaxStsDigis  = -1;
  Int_t fiTriggerMaxMuchDigis = -1;
  Int_t fiTriggerMaxTrdDigis  = -1;
  Int_t fiTriggerMaxTofDigis  = -1;
  Int_t fiTriggerMaxRichDigis = -1;
  Int_t fiTriggerMaxPsdDigis  = -1;
  /// Trigger Window
  Double_t fdRefTimeWinBeg  = 0.0;
  Double_t fdT0TimeWinBeg   = 0.0;
  Double_t fdStsTimeWinBeg  = kdDefaultTimeWinBeg;
  Double_t fdMuchTimeWinBeg = kdDefaultTimeWinBeg;
  Double_t fdTrdTimeWinBeg  = kdDefaultTimeWinBeg;
  Double_t fdTofTimeWinBeg  = kdDefaultTimeWinBeg;
  Double_t fdRichTimeWinBeg = kdDefaultTimeWinBeg;
  Double_t fdPsdTimeWinBeg  = kdDefaultTimeWinBeg;

  Double_t fdRefTimeWinEnd  = 0.0;
  Double_t fdT0TimeWinEnd   = 0.0;
  Double_t fdStsTimeWinEnd  = kdDefaultTimeWinEnd;
  Double_t fdMuchTimeWinEnd = kdDefaultTimeWinEnd;
  Double_t fdTrdTimeWinEnd  = kdDefaultTimeWinEnd;
  Double_t fdTofTimeWinEnd  = kdDefaultTimeWinEnd;
  Double_t fdRichTimeWinEnd = kdDefaultTimeWinEnd;
  Double_t fdPsdTimeWinEnd  = kdDefaultTimeWinEnd;

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
  CbmEvent* fCurrentEvent {
    nullptr};  //! pointer to the event which is currently build
  std::vector<CbmEvent*> fEventVector;  //! vector with all created events

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

  /// Internal state variables
  UInt_t fuCurEv {0};           //! Event Counter
  UInt_t fuErrors {0};          //! Error Counter
  UInt_t fuNrTs {0};            //! Timeslice Counter
  Double_t fdPrevEvtTime {0.};  //! Save previous time information
  Double_t fdPrevEvtEndTime {
    0.};  //! Save previous event last digi time information
  UInt_t fuStartIndexT0 {
    0};  //! Start index for first Digi matching previous seed
  UInt_t fuStartIndexSts {
    0};  //! Start index for first Digi matching previous seed
  UInt_t fuStartIndexMuch {
    0};  //! Start index for first Digi matching previous seed
  UInt_t fuStartIndexTrd {
    0};  //! Start index for first Digi matching previous seed
  UInt_t fuStartIndexTof {
    0};  //! Start index for first Digi matching previous seed
  UInt_t fuStartIndexRich {
    0};  //! Start index for first Digi matching previous seed
  UInt_t fuStartIndexPsd {
    0};  //! Start index for first Digi matching previous seed
  UInt_t fuEndIndexT0 {
    0};  //! End index for first Digi not matching previous seed
  UInt_t fuEndIndexSts {
    0};  //! End index for first Digi not matching previous seed
  UInt_t fuEndIndexMuch {
    0};  //! End index for first Digi not matching previous seed
  UInt_t fuEndIndexTrd {
    0};  //! End index for first Digi not matching previous seed
  UInt_t fuEndIndexTof {
    0};  //! End index for first Digi not matching previous seed
  UInt_t fuEndIndexRich {
    0};  //! End index for first Digi not matching previous seed
  UInt_t fuEndIndexPsd {
    0};  //! End index for first Digi not matching previous seed

  ClassDefNV(CbmMcbm2019TimeWinEventBuilderAlgo, 1);
};

#endif  // CBMMCBM2019TIMEWINEVENTBUILDERALGO_H
