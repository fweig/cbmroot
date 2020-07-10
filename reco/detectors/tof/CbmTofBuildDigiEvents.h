/**
 * @file
 * @author Christian Simon <csimon@physi.uni-heidelberg.de>
 * @since 2018-05-31
 */

#ifndef CBMTOFBUILDDIGIEVENTS_H
#define CBMTOFBUILDDIGIEVENTS_H 1


#include "FairTask.h"

#include <map>
#include <set>
#include <tuple>
#include <vector>


class FairFileSource;
class CbmTimeSlice;
class CbmMCEventList;
class TClonesArray;
class CbmTofDigi;

/**
 * @brief ...
 * @author Christian Simon <csimon@physi.uni-heidelberg.de>
 * @since 2018-05-31
 * @version 1.0
 * @details ...
 */
class CbmTofBuildDigiEvents : public FairTask {
public:
  CbmTofBuildDigiEvents();

  virtual ~CbmTofBuildDigiEvents();

  virtual void Exec(Option_t* option);

  //    virtual void FinishEvent();

  void SetEventWindow(Double_t dWindow) { fdEventWindow = dWindow; }
  void SetTriggerCounter(Int_t iModuleType,
                         Int_t iModuleIndex,
                         Int_t iCounterIndex,
                         Int_t iNCounterSides);
  void SetTriggerMultiplicity(Int_t iMultiplicity) {
    fiTriggerMultiplicity = iMultiplicity;
  }
  void SetPreserveMCBacklinks(Bool_t bPreserve) {
    fbPreserveMCBacklinks = bPreserve;
  }
  void SetDigiTotOffset(Double_t dOffset) { fdDigiToTOffset = dOffset; }
  void SetIgnoreCounterSide(Int_t iModuleType,
                            Int_t iModuleIndex,
                            Int_t iCounterIndex,
                            Int_t iCounterSide);


protected:
  virtual InitStatus Init();

  //    virtual void SetParContainers();

  virtual void Finish();


private:
  CbmTofBuildDigiEvents(const CbmTofBuildDigiEvents&);

  CbmTofBuildDigiEvents& operator=(const CbmTofBuildDigiEvents&);

  void ProcessIdealEvents(Double_t dProcessingTime);
  void FillMCEventList();

  FairFileSource* fFileSource;
  CbmTimeSlice* fTimeSliceHeader;
  TClonesArray* fTofTimeSliceDigis;
  TClonesArray* fDigiMatches;
  CbmMCEventList* fInputMCEventList;
  CbmMCEventList* fOutputMCEventList;
  TClonesArray* fTofEventDigis;
  Double_t fdEventWindow;
  std::map<std::tuple<Int_t, Int_t, Int_t>, UChar_t>
    fNominalTriggerCounterMultiplicity;
  Int_t fiTriggerMultiplicity;
  Bool_t fbPreserveMCBacklinks;
  Bool_t fbMCEventBuilding;
  Double_t fdEventStartTime;
  std::map<std::tuple<Int_t, Int_t, Int_t>, UChar_t> fCounterMultiplicity;
  Double_t fdIdealEventWindow;
  std::set<std::pair<Int_t, Int_t>> fProcessedIdealEvents;
  std::map<std::pair<Int_t, Int_t>, Double_t> fIdealEventStartTimes;
  std::map<std::pair<Int_t, Int_t>, std::vector<CbmTofDigi*>> fIdealEventDigis;
  Int_t fiNEvents;
  Double_t fdDigiToTOffset;
  std::set<std::tuple<Int_t, Int_t, Int_t, Int_t>> fInactiveCounterSides;


  ClassDef(CbmTofBuildDigiEvents, 0);
};

#endif
