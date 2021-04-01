/** @file CbmDigitizationSource.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 09.11.2018
 **/

#ifndef CBMDIGITIZATIONSOURCE_H
#define CBMDIGITIZATIONSOURCE_H 1

#include "CbmDefs.h"
#include "CbmMCInputSet.h"

#include "FairEventHeader.h"
#include "FairMCEventHeader.h"
#include "FairSource.h"
#include <Logger.h>

#include "TObject.h"
#include "TString.h"

#include <map>
#include <set>

class FairEventHeader;

/** @class CbmDigitizationSource
 ** @brief Source class for the input to digitization in CBM
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 9 November 2018
 **
 ** This class serves as input source for CBM digitization runs.
 ** It allows to mix an arbitrary number of different inputs
 ** (results of transport simulations) with different event rates.
 ** For each input chain, a sequence of event times is calculated.
 ** The respective next event is delivered to FairRootManager.
 **
 ** The API is the method AddInput, specifying a TChain with an
 ** event rate. The source object has to be registered to the run.
 ** The large number of non-API public methods is due to the
 ** implementation of the base class FairSource.
 **
 ** The class is based on FairMixedSource by M. Al-Turany, stripping
 ** unneeded functionality and introducing a different concept of
 ** input mixing.
 **/
class CbmDigitizationSource : public FairSource {

public:
  /** @brief Constructor **/
  CbmDigitizationSource();


  /** @brief Destructor **/
  virtual ~CbmDigitizationSource();


  /** @brief Activate a branch and set its address
     ** @param object  Pointer to pointer to branch class
     ** @param branchName  Name of branch
     **
     ** Inherited from FairSource. This method will be called from
     ** FairRootManager::ActivateBranch(branchName) when an input
     ** branch is demanded by a task.
     **
     ** All input trees have to be connected to the argument object.
     **/
  virtual Bool_t ActivateObject(TObject** object, const char* branchName);


  /** @brief Add a transport input
     ** @param inputId   Input number (identifier)
     ** @param chain     Pointer to input chain
     ** @param mode      Tree access mode (kRegular / kRepeat / kRandom)
     **/
  void AddInput(UInt_t inputId,
                TChain* chain,
                Double_t rate,
                ECbmTreeAccess mode = ECbmTreeAccess::kRegular);


  /** @brief Maximal entry number the source can run to
     ** @param lastEntry  Last entry as specified by FairRunAna. Ignored.
     ** @value Last entry possible with this source
     **
     ** Inherited from FairSource. Since there can be several inputs
     ** being randomly mixed together, a maximal entry number cannot
     ** be specified. Thus, a practically infinite value is returned.
     ** The run is then terminated by ReadEvent() returning non-zero.
     ** If lastEntry is specified by FairRunAna (i.e., by the user),
     ** this is the return value.
     **/
  virtual Int_t CheckMaxEventNo(Int_t lastEntry = 0);


  /** @brief Abstract in base class. No implementation here.
     **
     ** Is actually not called at all from FairRunAna.
     **/
  virtual void Close() {}


  /** @brief Embed a transport input
     ** @param inputId   Input number (identifier)
     ** @param chain     Pointer to input chain
     ** @param targetInputId  ID of the input to be embedded into
     ** @param mode      Tree access mode (kRegular / kRepeat / kRandom)
     **/
  void EmbedInput(UInt_t inputId,
                  TChain* chain,
                  UInt_t targetInputId,
                  ECbmTreeAccess mode = ECbmTreeAccess::kRegular);


  /** @brief Fill the output event header
     ** @param event Pointer to event header
     **
     ** Fills run ID, input ID, entry ID and event time.
     **/
  virtual void FillEventHeader(FairEventHeader* event);


  /** @brief List of branch names
     ** @value Reference to set of branch names
     **/
  const std::set<TString>& GetBranchList() const { return fBranches; }


  /** @brief First input from the first input set
     ** @value Pointer to first input
     **/
  CbmMCInput* GetFirstInput();


  /** @brief Source type is kFILE **/
  virtual Source_Type GetSourceType() {
    LOG(fatal) << "GetSourceTpye";

    return kFILE;
  }


  /** @brief Abstract in base class. No implementation here. **/
  virtual Bool_t Init();


  /** @brief Abstract in base class. No implementation here.
     **
     ** Is actually not called at all from FairRunAna.
     **/
  virtual Bool_t InitUnpackers() { return kTRUE; }


  /** @brief Provide one tree entry
     ** @param event Event number. Has no effect here.
     ** @value 0 for success, 1 is end of tree is reached.
     **
     ** The input with the smallest next event time is chosen.
     ** From each input, the entries are provided consecutively.
     ** If the end of the respective input tree is reached, such
     ** that no more entries can be read, 1 is returned, causing
     ** FairRunAna to stop the run.
     **/
  virtual Int_t ReadEvent(UInt_t event = 0);


  /** @brief Abstract in base class. No implementation here.
     **
     ** Is actually not called at all from FairRunAna.
     **/
  virtual Bool_t ReInitUnpackers() { return kTRUE; }


  /** @brief Abstract in base class. No implementation here. **/
  virtual void Reset() {}


  /** @brief Implement abstract function from base class
     **
     ** Read the first event from input file to fill the
     ** event header and get the RunID
     **/
  virtual Bool_t SpecifyRunId() {
    if (ReadEvent(0)) return true;

    return false;
  }

  /** @brief Set event-by-event mode
     ** @value choice  kTRUE if event-by-event mode
     **
     ** In the event-by-event mode, only the first input is processed.
     ** No event start time is generated; the event time is always zero.
     **/
  void SetEventMode(Bool_t choice = kTRUE) { fEventMode = choice; }


  /** @brief Abstract in base class. No implementation here.
     **
     ** Is actually not called at all from FairRunAna.
     **/
  virtual void SetParUnpackers() {}


  /** @brief Set the offset for the first event time
     ** @param time  Time offset for first event
     **
     ** If the event times start with or close to zero, negative digi times
     ** can occur because of the finite time resolution of the detectors.
     ** However, the first time slice always start from zero. To avoid
     ** conflicts or loss of data, a time offset is introduced, preventing
     ** negative digi times. The default value of this offset is set in the
     ** constructor. It can be changed by this method.
     **/
  void SetTimeStart(Double_t time) { fTimeStart = time; }


private:
  std::vector<CbmMCInputSet*> fInputSets;
  std::map<UInt_t, CbmMCInputSet*> fInputMap;     //! input ID -> inputSet
  std::map<Double_t, CbmMCInputSet*> fNextEvent;  //! time -> inputSet
  FairMCEventHeader* fMCEventHeader;
  TObjArray* fListOfFolders;
  std::set<TString> fBranches;  // List of branches names
  Double_t fTimeStart;          // Time offset first first event
  Double_t fCurrentTime;
  Int_t fCurrentEntryId;
  Int_t fCurrentInputId;
  Int_t fCurrentRunId;
  Bool_t fFirstCall;
  Bool_t fEventMode;
  CbmMCInputSet* fCurrentInputSet;
  Bool_t fSwitchInputSet;  // Flag to switch the input set at next ReadEvent


  /** @brief Compare an input set branch list with the reference list
     ** @param input Pointer to CbmMCInputSet
     ** @value kTRUE if the branch list of the input set is compatible
     **
     ** The branch list of the input set is considered compatible if all
     ** branches of the reference list are present in the input set. Additional
     ** branches in the input set are not considered harmful. The reference
     ** branch list is defined by the first input set.
     **/
  Bool_t CheckBranchList(CbmMCInputSet* input);


  /** @brief Get next entry in event-by-event mode
     ** @param event Entry number
     ** @value 0 if successful, 1 if requested entry does not exist
     **
     ** In the event-by-event mode, only the first input set is used.
     ** The event time is zero for all events.
     **/
  Int_t ReadEventByEvent(UInt_t event);


  /** @brief Read run ID from the first entry in the first input
     **
     ** This is used for the first call to ReadEvent, which happens
     ** from FairRunAna::Init() to get the run ID. The run ID is read
     ** from FairMCEventHeader and copied to the FairEventHeader by
     ** FillEventHeader, which is also called from FairRunAna.
     **/
  void ReadRunId();


  ClassDef(CbmDigitizationSource, 1);
};

#endif /* CBMDIGITIZATIONSOURCE_H */
