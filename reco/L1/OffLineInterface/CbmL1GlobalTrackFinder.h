/* Copyright (C) 2019 IKF-UFra, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina [committer] */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: V. Akishina
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
#include "CbmMuchTrack.h"
#include "CbmStsTrackFinder.h"
//#include "CbmTofTrack.h"
#include "CbmTrack.h"
#include "CbmTrdTrack.h"

class TClonesArray;
class CbmEvent;
class CbmTrack;
class CbmGlobalTrack;
class CbmTofTrack;


class CbmL1GlobalTrackFinder : public CbmStsTrackFinder {

public:
  /** Default constructor **/
  CbmL1GlobalTrackFinder();


  /** Destructor **/
  virtual ~CbmL1GlobalTrackFinder();


  /** Initialisation **/
  virtual void Init();


  /** Track finding algorithm
   **/
  virtual Int_t DoFind();

  /** Execute track finding on one event
   ** @param event  Pointer to event object
   ** @value Number of created tracks
   **/
  virtual Int_t FindTracks(CbmEvent* event);

  /// set a default particle mass for the track fit
  /// it is used during reconstruction
  /// for the multiple scattering and energy loss estimation
  void SetDefaultParticlePDG(int pdg = 13);  // muon
  void SetGlobalTracksArray(TClonesArray* tracks) { fGlobalTracks = tracks; }
  void SetStsTracksArray(TClonesArray* tracks) { fStsTracks = tracks; }
  void SetMuchTracksArray(TClonesArray* tracks) { fMuchTracks = tracks; }
  void SetTrdTracksArray(TClonesArray* tracks) { fTrdTracks = tracks; }
  void SetTofTracksArray(TClonesArray* tracks) { fTofTracks = tracks; }

protected:
  TClonesArray* fGlobalTracks;  // GlobalTrack array
  TClonesArray* fStsTracks;     // StsTrack array
  TClonesArray* fMuchTracks;    // MuchTrack array
  TClonesArray* fTrdTracks;     // TrdTrack array
  TClonesArray* fTofTracks;     // TofTrack array

private:
  /** Copy the tracks from the L1-internal format and array
   ** to the output TClonesArray.
   ** @value  Number of created tracks
   **/
  Int_t CopyL1Tracks(CbmEvent* event = NULL);

  /** Convert detector specific track info to a detector track
   **/
  void CbmL1TrackToCbmTrack(CbmL1Track T, CbmTrack* track, int systemIdT);
  void CbmL1TrackToCbmStsTrack(CbmL1Track T, CbmStsTrack* track);
  void CbmL1TrackToCbmMuchTrack(CbmL1Track T, CbmMuchTrack* track, int systemIdT);
  void CbmL1TrackToCbmTrdTrack(CbmL1Track T, CbmTrdTrack* track, int systemIdT);
  void CbmL1TrackToCbmTofTrack(CbmL1Track T, CbmTofTrack* track, int systemIdT);

  CbmL1GlobalTrackFinder(const CbmL1GlobalTrackFinder&);
  CbmL1GlobalTrackFinder& operator=(const CbmL1GlobalTrackFinder&);

  ClassDef(CbmL1GlobalTrackFinder, 1);
};


#endif
