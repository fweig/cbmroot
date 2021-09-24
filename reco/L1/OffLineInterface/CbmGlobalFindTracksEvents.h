/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina [committer] */

/** @file CbmGlobalFindTracksEvent.h
 ** @author V. Akishina <v.akishina@gsi.de>
 ** based on CbmStsFindTracksEvent.h by Volker Friese <v.friese@gsi.de>
 ** @since 07.05.2021
 ** @date 07.05.2021
 **/

#ifndef CBMGLOBALFINDTRACKSEVENTS
#define CBMGLOBALFINDTRACKSEVENTS 1


#include "CbmL1GlobalTrackFinder.h"

#include "FairTask.h"

#include "TStopwatch.h"


class TClonesArray;
class CbmEvent;
class CbmStsTrackFinderIdeal;


/** @class CbmGlobalFindTracksEvents
 ** @brief Task class for finding Global, STS, MUCH, TRD and TOF tracks in an event
 ** @author V. Akishina <v.akishina@gsi.de>
 ** based on CbmStsFindTracksEvent.h by Volker Friese <v.friese@gsi.de>
 ** @since 07.05.2021
 ** @date 07.05.2021
 ** @version 1.0
 **
 ** This task creates GlobalTrack, StsTrack, MuchTrack, TrdTrack, TofTrack objects 
 ** from hits.
 ** It uses as finding engine CbmL1GlobalTrackFinder.
 **/
class CbmGlobalFindTracksEvents : public FairTask {

public:
  /** Constructor
   ** @param finder  Track finder engine. Default: Ideal track finder.
   ** @param useMvd  Include MVD hits in track finding. Default kFALSE.
   **/
  CbmGlobalFindTracksEvents(CbmL1GlobalTrackFinder* finder = NULL, Bool_t useMvd = kFALSE);


  /** Destructor **/
  virtual ~CbmGlobalFindTracksEvents();


  /** Task execution **/
  virtual void Exec(Option_t* opt);


  /** Track finder engine
   ** @value  Pointer to track finding engine
   **/
  CbmL1GlobalTrackFinder* GetFinder() { return fFinder; };


  /** Usage of MVD hits
   ** @value  kTRUE if MVD hits are used for tracking
   **/
  Bool_t IsMvdUsed() const { return fUseMvd; }


  /** Set track finding engine
   ** @param finder  Pointer to track finding engine
   **/
  void UseFinder(CbmL1GlobalTrackFinder* finder)
  {
    if (fFinder) delete fFinder;
    fFinder = finder;
  };


private:
  Bool_t fUseMvd;                   //  Inclusion of MVD hits
  CbmL1GlobalTrackFinder* fFinder;  //  TrackFinder concrete class
  TClonesArray* fEvents;            //! Array of CbmEvent objects
  TClonesArray* fMvdHits;           //! Input array of MVD hits
  TClonesArray* fStsHits;           //! Input array of STS hits
  TClonesArray* fGlobalTracks;      //! Output array of CbmGlobalTracks
  TClonesArray* fStsTrackArray;     //! Output array of CbmStsTracks
  TClonesArray* fMuchTrackArray;    //! Output array of CbmMuchTracks
  TClonesArray* fTrdTrackArray;     //! Output array of CbmTrdTracks
  TClonesArray* fTofTrackArray;     //! Output array of CbmTofTracks
  TStopwatch fTimer;                //! Timer
  Int_t fNofTs = 0;                 ///< Number of processed timeslices
  Int_t fNofEvents;                 ///< Number of events with success
  Double_t fNofHits;                ///< Number of hits
  Double_t fNofTracks;              ///< Number of tracks created
  Double_t fTime;                   ///< Total real time used for good events


  /** Initialisation at beginning of each event **/
  virtual InitStatus Init();


  /** Finish at the end of each event **/
  virtual void Finish();


  /** @brief Process one event or timeslice
   ** @param event Pointer to event object. If null, entire timeslice is processed.
   ** @return Number of input hits and output tracks
   **/
  std::pair<UInt_t, UInt_t> ProcessEvent(CbmEvent* event);


  /** Prevent usage of copy constructor and assignment operator **/
  CbmGlobalFindTracksEvents(const CbmGlobalFindTracksEvents&);
  CbmGlobalFindTracksEvents operator=(const CbmGlobalFindTracksEvents&);


  ClassDef(CbmGlobalFindTracksEvents, 1);
};

#endif
