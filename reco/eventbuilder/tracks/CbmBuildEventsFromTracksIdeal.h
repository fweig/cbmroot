/** @file CbmBuildEventsFromTracksIdeal.h
 ** @author Valentina Akishina <v.akishina@gsi.de>
 ** @date 14.03.2017
 **/
#ifndef CBMBUILDEVENTSFROMTRACKSIDEAL_H_
#define CBMBUILDEVENTSFROMTRACKSIDEAL_H 1


#include <FairTask.h>

class TClonesArray;
class CbmMCEventList;
#include "CbmMCDataArray.h"

/** @class CbmBuildEventsFromTracksIdeal
 ** @brief Task class for associating digis to events
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 ** @version 1.0
 **
 ** The event association used MC truth (CbmMatch of CbmDigi).
 ** It operates within one time slice; splitting of events between
 ** time slice is not treated.
 **/
class CbmBuildEventsFromTracksIdeal: public FairTask 
{
 public:

  /** Constructor **/
  CbmBuildEventsFromTracksIdeal();

  CbmBuildEventsFromTracksIdeal(const CbmBuildEventsFromTracksIdeal&) = delete;
  CbmBuildEventsFromTracksIdeal& operator=(const CbmBuildEventsFromTracksIdeal&) = delete;

  /** Destructor **/
  virtual ~CbmBuildEventsFromTracksIdeal();

  /** Task execution **/
  virtual void Exec(Option_t* opt);

 private:

  TClonesArray* fStsDigis;   ///< Input array (class CbmStsDigi)
  TClonesArray* fStsTracks;   ///< Input array (class CbmStsDigi)
  CbmMCDataArray* fMCTracks;   ///< Input array (class CbmStsDigi)
  TClonesArray* fStsHits;   ///< Input array (class CbmStsDigi)
  TClonesArray* fMcPoints;   ///< Input array (class CbmStsDigi)
  TClonesArray* fEvents;     ///< Output array (class CbmEvent)
  TClonesArray* fStsTrackMatchArray;

  CbmMCEventList* fEventList;

  /** Task initialisation **/
  virtual InitStatus Init();

 
  ClassDef(CbmBuildEventsFromTracksIdeal, 1);

};

#endif /* CBMBUILDEVENTSFROMTRACKS_H */
