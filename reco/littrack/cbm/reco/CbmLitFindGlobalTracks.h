/* Copyright (C) 2009-2021 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Volker Friese */

/**
 * \file CbmLitFindGlobalTracks.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 * \brief CBM task for global track reconstruction.
 *
 * Output is reconstructed global tracks CbmGlobalTrack and
 * local track segments CbmMuchTrack, CbmTrdTrack.
 * The task automatically determines the setup based on the
 * geometry stored in the MC transport file.
 * The tracking is performed in the MUCH and TRD detectors, than
 * the hit-to-track merger attaches the TOF hit finally the track is refitted.
 * If TRD detector presences in the muon detector setup,
 * than the tracking is done in MUCH+TRD detectors in one goal.
 **/

#ifndef CBMLITFINDGLOBALTRACKS_H_
#define CBMLITFINDGLOBALTRACKS_H_

#include "CbmVertex.h"
#include "base/CbmLitDetectorSetup.h"
#include "base/CbmLitPtrTypes.h"
#include "base/CbmLitTypes.h"
#include "data/CbmLitHit.h"

#include "FairTask.h"

#include "TStopwatch.h"

#include <string>
#include <vector>

class TClonesArray;
class CbmEvent;

using std::string;
using std::vector;

class CbmLitFindGlobalTracks : public FairTask {
public:
  /**
    * \brief Constructor.
    */
  CbmLitFindGlobalTracks();

  /**
    * \brief Destructor.
    */
  virtual ~CbmLitFindGlobalTracks();

  /**
    * \brief Inherited from FairTask.
    */
  virtual InitStatus Init();

  /**
    * \brief Inherited from FairTask.
    */
  virtual void Exec(Option_t* opt);

  /**
    * \brief Inherited from FairTask.
    */
  virtual void Finish();

  /**
    * \brief Inherited from FairTask.
    */
  virtual void SetParContainers();

  /* Setters */
  void SetTrackingType(const string& trackingType) { fTrackingType = trackingType; }
  void SetMergerType(const string& mergerType) { fMergerType = mergerType; }
  void SetFitterType(const string& fitterType) { fFitterType = fitterType; }

private:
  /**
    * \brief Create and initialize track finder and track merger objects
    */
  void InitTrackReconstruction();

  /**
    * \brief Convert input data from CBMROOT data classes to LIT data classes.
    */
  void ConvertInputData(CbmEvent* event);

  /**
    * \brief Convert output data LIT data classes to CBMROOT data classes.
    */
  void ConvertOutputData(CbmEvent* event);

  /*
    * \brief Calculate length of the global track
    */
  void CalculateLength(CbmEvent* event);

  /*
    * \brief Calculate and set the parameter set in the primary vertex of the global track
    */
  void CalculatePrimaryVertexParameters(CbmEvent* event);

  /**
    * \brief Clear arrays and frees the memory.
    */
  void ClearArrays();

  /**
    * \brief Accesses input data branches (hits, tracks, events) from the tree
    */
  void ReadInputBranches();

  /**
    * \brief Register output data branches (tracks) to the tree
    */
  void RegisterOutputBranches();

  /**
    * \brief Run the track reconstruction
    */
  void RunTrackReconstruction();

  /**
    * \brief Print output stopwatch statistics for track-finder and hit-to-track merger.
    */
  void PrintStopwatchStatistics();

  /**
    * \brief Select tracks for further merging with TOF.
    */
  void SelectTracksForTofMerging();

  CbmLitDetectorSetup fDet;

  // Pointers to data arrays
  // Input
  TClonesArray* fMvdHits;        // CbmMvdHit array
  TClonesArray* fStsTracks;      // CbmStsTrack array
  TClonesArray* fStsHits;        // CbmStsHit
  TClonesArray* fMuchPixelHits;  // CbmMuchPixelHit array
  TClonesArray* fTrdHits;        // CbmTrdHit array
  TClonesArray* fTofHits;        // CbmTofHit array
  TClonesArray* fEvents;
  // Output
  TClonesArray* fMuchTracks;    // output CbmMuchTrack array
  TClonesArray* fTrdTracks;     // output CbmTrdTrack array
  TClonesArray* fTofTracks;     // output CbmTofTrack array
  TClonesArray* fGlobalTracks;  //output CbmGlobalTrack array

  CbmVertex* fPrimVertex;  // Pointer to the primary vertex

  // LIT data arrays
  TrackPtrVector fLitStsTracks;           // STS tracks
  HitPtrVector fLitHits;                  // MUCH+TRD hits
  HitPtrVector fLitTofHits;               // TOF hits
  TrackPtrVector fLitOutputTracks;        // output Lit tracks
  TofTrackPtrVector fLitOutputTofTracks;  // output Lit TOF tracks

  // Tools
  TrackFinderPtr fFinder;       // track finder
  HitToTrackMergerPtr fMerger;  // hit-to-track merger
  TrackFitterPtr fFitter;       // track fitter
  // track propagator
  // Used to propagate STS track to the last STS station!!!
  // Since this cannot be done in parallel mode!!!
  TrackPropagatorPtr fPropagator;

  // Settings
  // Tracking method to be used
  // "branch" - branching method
  // "nn" - nearest neighbor method
  string fTrackingType;

  // Merger method to be used
  // "nearest_hit" - assigns nearest hit to the track
  string fMergerType;

  // Track fitter to be used for the final track fit
  // "lit_kalman" - forward Kalman track fit with LIT propagation and TGeo navigation
  string fFitterType;

  // stopwatches
  TStopwatch fTrackingWatch;  // stopwatch for tracking
  TStopwatch fMergerWatch;    // stopwatch for merger

  // counters
  Long64_t fNofTs        = 0;   // timeslice counter
  Long64_t fNofEvents    = 0;   // event counter
  Long64_t fNofStsTracks = 0;   // STS tracks
  Long64_t fNofMuchHits  = 0;   // MUCH hits
  Long64_t fNofTrdHits   = 0;   // TRD hits
  Long64_t fNofTofHits   = 0;   // TOF hits
  Long64_t fNofGlbTracks = 0;   // Global tracks
  Double_t fTime         = 0.;  // Processing time
  Long64_t fEventNo;            // event counter (old)

  CbmLitFindGlobalTracks(const CbmLitFindGlobalTracks&);
  CbmLitFindGlobalTracks& operator=(const CbmLitFindGlobalTracks&);

  ClassDef(CbmLitFindGlobalTracks, 1);
};

#endif /* CBMLITFINDGLOBALTRACKS_H_ */
