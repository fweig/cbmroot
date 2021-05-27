/** @file CbmMCEventFilter.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 07.08.2019
 **/

#ifndef CBMMCEVENTFILTER_H
#define CBMMCEVENTFILTER_H 1

#include "CbmDefs.h"

#include "FairMCApplication.h"
#include "FairTask.h"

#include "TClonesArray.h"

#include <map>
#include <string>
#include <vector>

class TClonesArray;


/** @class CbmMCEventFilter
 ** @brief Class deciding whether to store an MC event
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 7 August 2019
 **
 ** The event selection is performed in the method SelectEvent()
 ** (nomen est omen...). If this method returns false, the event
 ** will not be filled to the output tree.
 **
 ** SelectEvent() can be re-implemented in derived classes.
 ** Here, the selection is based on the number of MC data
 ** objects (MCTrack, MCPoints), on which the user can specify
 ** a minimum by SetMinNofObjects.
 **/
class CbmMCEventFilter : public FairTask {

public:
  /** brief Constructor **/
  CbmMCEventFilter();


  /** @brief Destructor **/
  virtual ~CbmMCEventFilter() {};


  /** @brief Number of processed events
		 ** @return Number of processed input events
		 **/
  Int_t GetNofInputEvents() const { return fNofEventsIn; }


  /** @brief Set a cut on the minimum number of data of a given type
		 ** @param type  CBM data type
		 ** @param value Minimum for number of objects in array
		 **
		 ** In case cuts are defined for more than one data type,
		 ** they are logically additive (each of them has to be passed).
		 **/
  void SetMinNofData(ECbmDataType type, Int_t value) { fMinNofData[type] = value; }


protected:
  /** @brief Get a data object by index
		 ** @param type CBM data type
		 ** @param index  Index in data array
		 **/
  TObject* GetData(ECbmDataType type, Int_t index) const;


  /** @brief Number of data in a branch
		 ** @param type  CBM data type
		 ** @return  Number of objects in array
		 **/
  Int_t GetNofData(ECbmDataType type) const
  {
    return (fData.at(type) == nullptr ? 0 : fData.at(type)->GetEntriesFast());
  }


  /** @brief Event selector method
		 ** @return If kTRUE, the event will be stored
		 **/
  Bool_t SelectEvent() const;


private:
  std::map<ECbmDataType, TClonesArray*> fData;  //!  Data arrays
  //		std::vector<TClonesArray*> fData;          //!  Data arrays
  std::map<ECbmDataType, Int_t> fMinNofData;  ///< Cut values
  Int_t fNofEventsIn;                         ///< Counter: input events
  Int_t fNofEventsOut;                        ///< Counter: output events

  /** @brief Execution
		 **
		 ** Sets the storage of current event in FairMCApplication according to
		 ** the outcome of SeelectEvent().
		 **/
  virtual void Exec(Option_t*);


  /** @brief Finish (end of run)
		 **
		 ** Prints event statistics
		 **/
  virtual void Finish();


  /** @brief Initialisation
		 **
		 ** Gets MC data branches from FairRootManager
		 **/
  virtual InitStatus Init();


  /** @brief Get a branch from FairRootManager
		 ** @param type  CBM data type
		 **/
  void GetBranch(ECbmDataType type);


  /** @brief Info on number of MC objects in the arrays **/
  std::string Statistics() const;


  /** @brief Get branch name from data type
		 ** @param type  CBM data type
		 ** @return Branch name
		 **/
  TString GetBranchName(ECbmDataType type) const;


  ClassDef(CbmMCEventFilter, 2);
};

#endif /* CBMMCEVENTFILTER_H */
