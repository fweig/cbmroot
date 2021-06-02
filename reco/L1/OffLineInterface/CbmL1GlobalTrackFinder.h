/** @file CbmL1GlobalTrackFinder.h
  * @copyright Copyright (C) 2019 IKF-UFra, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Valentina Akishina [committer] **/

/*
 *====================================================================
 *
 *  CBM Level 1 Global Reconstruction 
 *  
 *  Authors: V.Akishina
 *
 *  e-mail : v.akishina@gsi.de
 *
 *====================================================================
 *
 *  CbmL1GlobalTrackFinder header
 *
 *====================================================================
 */

#ifndef CBML1GLOBALTRACKFINDER
#define CBML1GLOBALTRACKFINDER 1


#include "CbmL1.h"

//#include "CbmStsTrackFinder.h"
#include "CbmGlobalTrack.h"
#include "CbmMuchTrack.h"
#include "CbmStsTrack.h"
#include "CbmTofTrack.h"
#include "CbmTrdTrack.h"

#include "FairTask.h"

#include "TStopwatch.h"

class TClonesArray;
class CbmEvent;


class CbmL1GlobalTrackFinder :
  public FairTask  //, CbmStsTrackFinder
{

public:
  /** Default constructor **/
  CbmL1GlobalTrackFinder();


  /** Destructor **/
  virtual ~CbmL1GlobalTrackFinder();


  /** Initialisation **/
  //virtual InitStatus Init();
  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();
  void ProcessEvent(CbmEvent* event);


  /** Track finding algorithm
   **/
  virtual Int_t DoFind();

  /** Execute track finding on one event
   ** @param event  Pointer to event object
   ** @value Number of created tracks
   **/
  virtual Int_t FindTracks(CbmEvent* event);


private:
  TClonesArray* fMuchTracks;    // output CbmMuchTrack array
  TClonesArray* fTrdTracks;     // output CbmTrdTrack array
  TClonesArray* fTofTracks;     // output CbmTofTrack array
  TClonesArray* fGlobalTracks;  //output CbmGlobalTrack array

  TClonesArray* fMvdHits;  // MvdHit array
  TClonesArray* fStsHits;  // StsHit array
  TClonesArray* fTracks;   // StsTrack array

  TClonesArray* fEvents;  //! Array of CbmEvent objects

  TStopwatch fTimer;    //! Timer
  Int_t fNofEvents;     ///< Number of events with success
  Double_t fNofHits;    ///< Number of hits
  Double_t fNofTracks;  ///< Number of tracks created
  Double_t fTime;       ///< Total real time used for good events
  Int_t fEventNo;       // event counter

  /** Copy the tracks from the L1-internal format and array
   ** to the output TClonesArray.
   ** @value  Number of created tracks
   **/
  Int_t CopyL1Tracks(CbmEvent* event = NULL);

  Int_t CreateGlobalTrackArray(CbmEvent* event, TClonesArray* globalTracks, TClonesArray* stsTracks,
                               TClonesArray* trdTracks, TClonesArray* muchTracks, TClonesArray* tofTracks);


  ClassDef(CbmL1GlobalTrackFinder, 1);
};


#endif
