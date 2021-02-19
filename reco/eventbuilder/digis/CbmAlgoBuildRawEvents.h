/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMALGOBUILDRAWEVENTS_H
#define CBMALGOBUILDRAWEVENTS_H

/// CBM headers
#include "CbmMuchBeamTimeDigi.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

/// FAIRROOT headers
#include "FairTask.h"

/// FAIRSOFT headers (geant, boost, ...)

/// C/C++ headers
#include <tuple>

#include <boost/any.hpp>

#include <array>
#include <map>
#include <set>
#include <vector>

class TimesliceMetaData;
class CbmEvent;
class TClonesArray;
class TH1;
class TH2;
class TNamed;
class TCanvas;

enum class EOverlapModeRaw
{
  NoOverlap,
  MergeOverlap,
  AllowOverlap
};

class RawEventBuilderDetector {
public:
  RawEventBuilderDetector() = default;

  RawEventBuilderDetector(ECbmModuleId detIdIn, ECbmDataType dataTypeIn, std::string sNameIn)
    : detId {detIdIn}
    , dataType {dataTypeIn}
    , sName {sNameIn}
  {
  }

  RawEventBuilderDetector(ECbmModuleId detIdIn, ECbmDataType dataTypeIn, std::string sNameIn, UInt_t uTriggerMinDigisIn,
                          Int_t iTriggerMaxDigisIn, Double_t fdTimeWinBegIn, Double_t fdTimeWinEndIn)
    : RawEventBuilderDetector(detIdIn, dataTypeIn, sNameIn)
  {
    fuTriggerMinDigis = uTriggerMinDigisIn;
    fiTriggerMaxDigis = iTriggerMaxDigisIn;
    fdTimeWinBeg      = fdTimeWinBegIn;
    fdTimeWinEnd      = fdTimeWinEndIn;
  }

  bool operator==(const RawEventBuilderDetector& other) const { return (other.detId == this->detId); }
  bool operator!=(const RawEventBuilderDetector& other) const { return (other.detId != this->detId); }

  Double_t GetTimeWinRange() { return fdTimeWinEnd - fdTimeWinBeg; }

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
static const RawEventBuilderDetector kRawEventBuilderDetSts =
  RawEventBuilderDetector(ECbmModuleId::kSts, ECbmDataType::kStsDigi, "Sts");
static const RawEventBuilderDetector kRawEventBuilderDetMuch =
  RawEventBuilderDetector(ECbmModuleId::kMuch, ECbmDataType::kMuchDigi, "Much");
static const RawEventBuilderDetector kRawEventBuilderDetTrd =
  RawEventBuilderDetector(ECbmModuleId::kTrd, ECbmDataType::kTrdDigi, "Trd");
static const RawEventBuilderDetector kRawEventBuilderDetTof =
  RawEventBuilderDetector(ECbmModuleId::kTof, ECbmDataType::kTofDigi, "Tof");
static const RawEventBuilderDetector kRawEventBuilderDetRich =
  RawEventBuilderDetector(ECbmModuleId::kRich, ECbmDataType::kRichDigi, "Rich");
static const RawEventBuilderDetector kRawEventBuilderDetPsd =
  RawEventBuilderDetector(ECbmModuleId::kPsd, ECbmDataType::kPsdDigi, "Psd");
static const RawEventBuilderDetector kRawEventBuilderDetT0 =
  RawEventBuilderDetector(ECbmModuleId::kT0, ECbmDataType::kT0Digi, "T0");
static const RawEventBuilderDetector kRawEventBuilderDetUndef = RawEventBuilderDetector();


class CbmAlgoBuildRawEvents {
public:
  /** Default constructor **/
  CbmAlgoBuildRawEvents() = default;

  CbmAlgoBuildRawEvents(const CbmAlgoBuildRawEvents&) = delete;
  CbmAlgoBuildRawEvents operator=(const CbmAlgoBuildRawEvents&) = delete;

  /** Destructor **/
  ~CbmAlgoBuildRawEvents() {};

  /** Initiliazation at the beginning of a run **/
  Bool_t InitAlgo();

  /** Executed for each TS. **/
  void ProcessTs();

  /** Finish called at the end of the run **/
  void Finish();

  void SetFillHistos(Bool_t var) { fbFillHistos = var; }
  void ResetHistograms(Bool_t bResetTime = kTRUE);

  void SetReferenceDetector(ECbmModuleId refDet, ECbmDataType dataTypeIn, std::string sNameIn,
                            UInt_t uTriggerMinDigisIn = 0, Int_t iTriggerMaxDigisIn = -1,
                            Double_t fdTimeWinBegIn = -100, Double_t fdTimeWinEndIn = 100);
  void AddDetector(ECbmModuleId selDet, ECbmDataType dataTypeIn, std::string sNameIn, UInt_t uTriggerMinDigisIn = 0,
                   Int_t iTriggerMaxDigisIn = -1, Double_t fdTimeWinBegIn = -100, Double_t fdTimeWinEndIn = 100);

  void SetReferenceDetector(RawEventBuilderDetector refDetIn);
  void AddDetector(RawEventBuilderDetector selDet);
  void RemoveDetector(RawEventBuilderDetector selDet);

  void SetTriggerMinNumber(ECbmModuleId selDet, UInt_t uVal);
  void SetTriggerMaxNumber(ECbmModuleId selDet, Int_t iVal);
  void SetTriggerWindow(ECbmModuleId selDet, Double_t dWinBeg, Double_t dWinEnd);

  void SetTsParameters(Double_t dTsStartTime, Double_t dTsLength, Double_t dTsOverLength)
  {
    fdTsStartTime  = dTsStartTime;
    fdTsLength     = dTsLength;
    fdTsOverLength = dTsOverLength;
  }

  /// Control flags
  void SetEventOverlapMode(EOverlapModeRaw mode) { fOverMode = mode; }
  void SetIgnoreTsOverlap(Bool_t bFlagIn = kTRUE) { fbIgnoreTsOverlap = bFlagIn; }

  void ChangeMuchBeamtimeDigiFlag(Bool_t bFlagIn = kFALSE) { fbUseMuchBeamtimeDigi = bFlagIn; }

  /// For monitor algos
  void AddHistoToVector(TNamed* pointer, std::string sFolder = "")
  {
    fvpAllHistoPointers.push_back(std::pair<TNamed*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TNamed*, std::string>> GetHistoVector() { return fvpAllHistoPointers; }
  void AddCanvasToVector(TCanvas* pointer, std::string sFolder = "")
  {
    fvpAllCanvasPointers.push_back(std::pair<TCanvas*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TCanvas*, std::string>> GetCanvasVector() { return fvpAllCanvasPointers; }

  /// Set digi containers
  void SetT0Digis(const std::vector<CbmTofDigi>* T0DigiVec) { fT0DigiVec = T0DigiVec; }
  void SetStsDigis(std::vector<CbmStsDigi>* StsDigis) { fStsDigis = StsDigis; }
  void SetMuchDigis(std::vector<CbmMuchDigi>* MuchDigis) { fMuchDigis = MuchDigis; }
  void SetTrdDigis(std::vector<CbmTrdDigi>* TrdDigis) { fTrdDigis = TrdDigis; }
  void SetTofDigis(std::vector<CbmTofDigi>* TofDigis) { fTofDigis = TofDigis; }
  void SetRichDigis(std::vector<CbmRichDigi>* RichDigis) { fRichDigis = RichDigis; }
  void SetPsdDigis(std::vector<CbmPsdDigi>* PsdDigis) { fPsdDigis = PsdDigis; }
  void SetMuchBeamTimeDigis(std::vector<CbmMuchBeamTimeDigi>* MuchBeamTimeDigis)
  {
    fMuchBeamTimeDigis = MuchBeamTimeDigis;
  }

  /// Data output access
  std::vector<CbmEvent*>& GetEventVector() { return fEventVector; }
  void ClearEventVector();

private:
  /// Internal methods
  Bool_t CheckDataAvailable(RawEventBuilderDetector& det);
  void InitTs();
  void InitSeedWindow();
  void BuildEvents();

  void CreateHistograms();
  void FillHistos();

  template<class DigiSeed>
  void LoopOnSeeds();
  void CheckSeed(Double_t dSeedTime, UInt_t uSeedDigiIdx);
  void CheckTriggerCondition(Double_t dSeedTime);

  template<class DigiCheck>
  void SearchMatches(Double_t dSeedTime, RawEventBuilderDetector& detMatch);
  void SearchMatches(Double_t dSeedTime, RawEventBuilderDetector& detMatch);
  void AddDigiToEvent(RawEventBuilderDetector& det, Int_t uIdx);
  Bool_t HasTrigger(CbmEvent*);
  Bool_t CheckTriggerConditions(CbmEvent* event, RawEventBuilderDetector& det);

  void UpdateTimeWinBoundariesExtrema();
  void UpdateWidestTimeWinRange();

  /// Constants
  static constexpr Double_t kdDefaultTimeWinBeg = -100.0;
  static constexpr Double_t kdDefaultTimeWinEnd = 100.0;

  /// User parameters
  /// Control flags
  Bool_t fbIgnoreTsOverlap = kFALSE;     //! Ignore data in Overlap part of the TS
  Bool_t fbFillHistos {kTRUE};           //! Switch ON/OFF filling of histograms
  Bool_t fbUseMuchBeamtimeDigi = kTRUE;  //! Switch between MUCH digi classes
    /// Event building mode and detectors selection
  EOverlapModeRaw fOverMode {EOverlapModeRaw::AllowOverlap};

  RawEventBuilderDetector fRefDet             = RawEventBuilderDetector(ECbmModuleId::kT0, ECbmDataType::kT0Digi, "T0");
  std::vector<RawEventBuilderDetector> fvDets = {
    RawEventBuilderDetector(ECbmModuleId::kSts, ECbmDataType::kStsDigi, "kSts"),
    RawEventBuilderDetector(ECbmModuleId::kMuch, ECbmDataType::kMuchDigi, "kMuch"),
    RawEventBuilderDetector(ECbmModuleId::kTrd, ECbmDataType::kTrdDigi, "kTrd"),
    RawEventBuilderDetector(ECbmModuleId::kTof, ECbmDataType::kTofDigi, "kTof"),
    RawEventBuilderDetector(ECbmModuleId::kRich, ECbmDataType::kRichDigi, "kRich"),
    RawEventBuilderDetector(ECbmModuleId::kPsd, ECbmDataType::kPsdDigi, "kPsd")};

  Double_t fdEarliestTimeWinBeg = kdDefaultTimeWinBeg;
  Double_t fdLatestTimeWinEnd   = kdDefaultTimeWinEnd;
  Double_t fdWidestTimeWinRange = kdDefaultTimeWinEnd - kdDefaultTimeWinBeg;
  ///Seed window
  Double_t fdSeedWindowBeg = 0;
  Double_t fdSeedWindowEnd = 0;

  Double_t fdTsStartTime  = -1;
  Double_t fdTsLength     = -1;
  Double_t fdTsOverLength = -1;

  /// Data input
  /// FIXME: usage of CbmDigiManager in FairMq context?!?
  ///        => Maybe by registering vector (or vector reference) to ioman in Device?
  //CbmDigiManager* fDigiMan                  = nullptr;  //!
  TClonesArray* fTimeSliceMetaDataArray = nullptr;  //!
  const TimesliceMetaData* pTsMetaData  = nullptr;

  const std::vector<CbmTofDigi>* fT0DigiVec                  = nullptr;
  const std::vector<CbmMuchDigi>* fMuchDigis                 = nullptr;
  const std::vector<CbmMuchBeamTimeDigi>* fMuchBeamTimeDigis = nullptr;
  const std::vector<CbmStsDigi>* fStsDigis                   = nullptr;
  const std::vector<CbmTrdDigi>* fTrdDigis                   = nullptr;
  const std::vector<CbmTofDigi>* fTofDigis                   = nullptr;
  const std::vector<CbmRichDigi>* fRichDigis                 = nullptr;
  const std::vector<CbmPsdDigi>* fPsdDigis                   = nullptr;

  bool CheckDataAvailable(ECbmModuleId detId);
  UInt_t GetNofDigis(ECbmModuleId detId);
  template<class Digi>
  const Digi* GetDigi(UInt_t uDigi);

  /// Data ouptut
  CbmEvent* fCurrentEvent             = nullptr;  //! pointer to the event which is currently build
  std::vector<CbmEvent*> fEventVector = {};       //! vector with all created events

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

  TH1* fhEventTime        = nullptr;  //! histogram with the seed time of the events
  TH1* fhEventDt          = nullptr;  //! histogram with the interval in seed time of consecutive events
  TH1* fhEventSize        = nullptr;  //! histogram with the nb of all  digis in the event
  TH2* fhNbDigiPerEvtTime = nullptr;  //! histogram with the nb of all  digis per event vs seed time of the events
  std::vector<TH2*> fvhNbDigiPerEvtTimeDet =
    {};  //! histograms with the nb of digis in each detector per event vs seed time of the events

  /// Internal state variables
  UInt_t fuCurEv            = 0;   //! Event Counter
  UInt_t fuErrors           = 0;   //! Error Counter
  UInt_t fuNrTs             = 0;   //! Timeslice Counter
  Double_t fdPrevEvtTime    = 0.;  //! Save previous time information
  Double_t fdPrevEvtEndTime = 0.;  //! Save previous event last digi time information

  ClassDefNV(CbmAlgoBuildRawEvents, 1);
};

#endif  // CBMALGOBUILDRAWEVENTS_H
