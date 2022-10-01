/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov[committer]*/

/// @file CbmTrackerInputQaTof.h
/// @author Sergey Gorbunov
/// @date 02.11.2021

#ifndef CbmTrackerInputQaTof_H
#define CbmTrackerInputQaTof_H

#include "CbmQaCanvas.h"
#include "CbmQaHist.h"

#include <FairTask.h>

#include <TFolder.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TH1I.h>
#include <TH2F.h>
#include <TParameter.h>
#include <TProfile.h>
#include <TProfile2D.h>

class CbmDigiManager;
class CbmMCDataManager;
class CbmMCEventList;
class CbmMCDataArray;
class CbmTimeSlice;
class CbmTofDigiPar;
class CbmTofParSetGeo;
class CbmTofPoint;

class TClonesArray;

///
/// CbmTrackerInputQaTof class represents all the CA tracker requirements for the Tof detector.
/// When this QA test is passed, the tracker must work (at least that is the idea).
///
/// The class ensures that the tracker has the correct understanding of the Tof geometry and interfaces
/// and checks the quality of all tracking-related aspects of the Tof data.
///
class CbmTrackerInputQaTof : public FairTask {

public:
  /// Constructor
  CbmTrackerInputQaTof(const char* name = "TrackerInputQaTof", Int_t verbose = 1);

  /// Destructor
  ~CbmTrackerInputQaTof();

  /// FairTask: Intialisation at begin of run.
  InitStatus Init() { return ReInit(); }

  /// FairTask: Reinitialisation.
  InitStatus ReInit();

  /// FairTask:  Intialise parameter containers.
  void SetParContainers();

  /// FairTask: Action at end of run. For this task and all of the subtasks.
  void Finish();

  /// TTask: Clear all data structures created by a previous execution of a task.
  void Clear(Option_t* /*option*/ = "") {}

  /// TTask: Process a timeslice
  void Exec(Option_t*);

  /// Prepare the Qa output and return it as a reference to an internal TFolder.
  /// The reference is non-const, because FairSink can not write const objects
  TFolder& GetQa();

private:
  /// Check the geometry
  InitStatus GeometryQa();

  /// Analysis of hit uncertainty (pull) distributions
  void ResolutionQa();

  /// Free the memory etc.
  void DeInit();

  int GetStationIndex(CbmTofPoint* p);


  // Setup

  Bool_t fIsTofInSetup {false};
  Bool_t fIsMcPresent {false};

  Int_t fNtrackingStations {0};

  CbmTimeSlice* fTimeSlice {nullptr};
  CbmTofParSetGeo* fTofGeoPar {nullptr};
  CbmTofDigiPar* fTofDigiPar {nullptr};

  CbmDigiManager* fDigiManager {nullptr};

  /// MC data
  CbmMCEventList* fMcEventList {nullptr};  // list of MC events connected to the current time slice
  CbmMCDataManager* fMcManager {nullptr};
  CbmMCDataArray* fMcTracks {nullptr};
  CbmMCDataArray* fMcPoints {nullptr};

  /// Data
  TClonesArray* fClusters {nullptr};
  TClonesArray* fHits {nullptr};
  TClonesArray* fHitMatches {nullptr};

  TClonesArray* fHitDigiMatches {nullptr};
  TClonesArray* fDigis {nullptr};
  TClonesArray* fTofCalDigiMatch {nullptr};

  /// Output


  TFolder fOutFolder {"TrackerInputQaTof", "TrackerInputQaTof"};  /// output folder with histos and canvases
  TFolder* fHistFolder {nullptr};                                 /// subfolder for histograms

  TParameter<int> fNevents {"nEvents", 0};  /// number of processed events

  /// Histogram for Residual Distribution
  CbmQaHist<TH1D> fhResidualX {"hResidualX", "Tof: Residual X;(X_{reco} - X_{MC})(cm)", 100, -10, 10};
  CbmQaHist<TH1D> fhResidualY {"hRresidualY", "Tof: Residual Y;(Y_{reco} - Y_{MC})(cm)", 100, -10, 10};
  CbmQaHist<TH1D> fhResidualT {"hRresidualT", "Tof: Residual T;(T_{reco} - T_{MC})(ns)", 100, -50, 50};
  CbmQaHist<TH1D> fhResidualZ {"hRresidualZ", "Tof: Residual Z;(Z_{reco} - Z_{MC})(cm)", 100, -100, 100};

  /// Histogram for PULL Distribution
  CbmQaHist<TH1D> fhPullX {"hPullX", "Tof: Pull X;(X_{reco} - X_{MC}) / #sigmaU_{reco}", 100, -5, 5};
  CbmQaHist<TH1D> fhPullY {"hPullX", "Tof: Pull Y;(Y_{reco} - Y_{MC}) / #sigmaV_{reco}", 100, -5, 5};
  CbmQaHist<TH1D> fhPullT {"hPullT", "Tof: Pull T;(T_{reco} - T_{MC}) / #sigmaT_{reco}", 100, -5, 5};

  /// List of the above histogramms
  std::vector<CbmQaHist<TH1D>*> fHistList;

  /// hits purity
  std::vector<CbmQaHist<TH1I>> fhPointsPerHit;

  /// hits efficiency
  std::vector<CbmQaHist<TH1I>> fhHitsPerPoint;

  /// hits efficiency
  std::vector<CbmQaHist<TProfile2D>> fhEfficiencyXY;
  std::vector<CbmQaHist<TProfile>> fhEfficiencyR;

  /// Canvaces: collection of histogramms

  CbmQaCanvas fCanvResidual {"cResidual", "Residual Distribution", 3 * 500, 2 * 500};
  CbmQaCanvas fCanvPull {"cPull", "Pull Distribution", 3 * 500, 2 * 500};
  CbmQaCanvas fCanvEfficiencyXY {"cEfficiencyXY", "Efficiency XY: % reconstructed McPoint", 2 * 500, 2 * 500};
  CbmQaCanvas fCanvEfficiencyR {"cEfficiencyR", "Efficiency R: % reconstructed McPoint", 2 * 500, 2 * 500};
  CbmQaCanvas fCanvHitsPerPoint {"cHitsPerMcPoint", "Efficiency: Hits Per McPoint", 2 * 500, 2 * 500};
  CbmQaCanvas fCanvPointsPerHit {"cMcPointsPerHit", "Purity: McPoints per Hit", 2 * 500, 2 * 500};

private:
  /// Suppressed copy constructor
  CbmTrackerInputQaTof(const CbmTrackerInputQaTof&) = delete;

  /// Suppressed assignment operator
  CbmTrackerInputQaTof& operator=(const CbmTrackerInputQaTof&) = delete;

  ClassDef(CbmTrackerInputQaTof, 0);
};

#endif
