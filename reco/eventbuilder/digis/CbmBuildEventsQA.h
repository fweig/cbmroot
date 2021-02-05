/** @file CbmBuildEventsQa.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 20.09.2016
 **/
#ifndef CBMBUILDEVENTSQA_H_
#define CBMBUILDEVENTSQA_H 1


#include <FairTask.h>

class TClonesArray;
class CbmDigiManager;
class CbmEvent;

/** @class CbmStsBuildEventsQA
 ** @brief Simple QA task for associating digis to events
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 20.09.2016
 ** @version 1.0
 **
 ** The task constructs a match from CbmEvent to CbmMCEvent
 ** and calculates the percentage of correctly assigned digis
 ** based on their matched link (largest weight).
 **/
class CbmBuildEventsQA : public FairTask {

public:
  /** Constructor **/
  CbmBuildEventsQA();

  /** Destructor **/
  virtual ~CbmBuildEventsQA();

  /** Task execution **/
  virtual void Exec(Option_t* opt);


private:
  CbmDigiManager* fDigiMan = nullptr;  //!
  TClonesArray* fEvents;               ///< Input array (class CbmEvent)
  Int_t fNofEntries;                   ///< Number of processed entries

  /** Task initialisation **/
  virtual InitStatus Init();


  /** Match a reconstructed event to MC events+
		 ** @param event Pointer to reconstructed event
		 **/
  void MatchEvent(CbmEvent* event);

  CbmBuildEventsQA(const CbmBuildEventsQA&);
  CbmBuildEventsQA& operator=(const CbmBuildEventsQA&);

  ClassDef(CbmBuildEventsQA, 1);
};

#endif /* CBMBUILDEVENTSQA_H */
