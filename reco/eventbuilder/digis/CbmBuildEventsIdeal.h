/** @file CbmBuildEventsIdeal.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 ** @date 15.^^.1010
 **/

#ifndef CBMBUILDEVENTSIDEAL_H
#define CBMBUILDEVENTSIDEAL_H 1


#include "CbmDefs.h"
#include <FairTask.h>
#include <vector>

class TClonesArray;
class CbmDigiManager;
class CbmMatch;


/** @class CbmStsBuildEventsIdeal
 ** @brief Task class for associating digis to events
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 ** @date 15.11.2020
 **
 ** The digi event builder creates objects of class CbmEvent and fills them
 ** with digi objects. For the association of the digis to the events, the
 ** MC-truth information in the digi match objects is employed. A digi
 ** is attributed to the event of its link with the largest weight.
 **
 ** The event builder operates within one time slice; splitting of events
 ** between time slices is not treated.
 **/
class CbmBuildEventsIdeal : public FairTask {

public:
  /** @brief Constructor **/
  CbmBuildEventsIdeal();

  /** @brief Copy constructor (disabled) **/
  CbmBuildEventsIdeal(const CbmBuildEventsIdeal&) = delete;

  /** @brief Destructor **/
  virtual ~CbmBuildEventsIdeal();

  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);

  /** @brief Assignment operator (disabled) **/
  CbmBuildEventsIdeal& operator=(const CbmBuildEventsIdeal&) = delete;


private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();


  /** @brief Number of different MC events in a match object **/
  UInt_t EventsInMatch(const CbmMatch* match);


private:                                  // members
  CbmDigiManager* fDigiMan = nullptr;     //!
  std::vector<ECbmModuleId> fSystems {};  //  List of detector systems
  TClonesArray* fEvents = nullptr;        //! Output array (class CbmEvent)
  Int_t fNofEntries     = 0;              //  Number of processed time slices


  ClassDef(CbmBuildEventsIdeal, 3);
};

#endif /* CBMBUILDEVENTSIDEAL_H */
