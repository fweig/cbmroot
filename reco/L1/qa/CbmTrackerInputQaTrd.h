/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov[committer]*/

/// @file CbmTrackerInputQaTrd.h
/// @author Sergey Gorbunov
/// @date 02.11.2021

#ifndef CbmTrackerInputQaTrd_H
#define CbmTrackerInputQaTrd_H

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
class CbmTrdParSetGeo;
class CbmTrdParSetDigi;

class TClonesArray;

///
/// CbmTrackerInputQaTrd class represents all the CA tracker requirements for the TRD detector.
/// When this QA test is passed, the tracker must work (at least that is the idea).
///
/// The class ensures that the tracker has the correct understanding of the TRD geometry and interfaces
/// and checks the quality of all tracking-related aspects of the TRD data.
///
class CbmTrackerInputQaTrd : public FairTask {

public:
  /// Constructor
  CbmTrackerInputQaTrd(const char* name = "TrackerInputQaTrd", Int_t verbose = 1);

  /// Destructor
  ~CbmTrackerInputQaTrd();

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


  // Setup

  Bool_t fIsTrdInSetup {false};
  Bool_t fIsMcPresent {false};

  Int_t fNtrackingStations {0};

  CbmTimeSlice* fTimeSlice {nullptr};
  CbmTrdParSetGeo* fTrdGeoPar {nullptr};
  CbmTrdParSetDigi* fTrdDigiPar {nullptr};
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

  /// Output


  TFolder fOutFolder {"TrackerInputQaTrd", "TrackerInputQaTrd"};  /// output folder with histos and canvases
  TFolder* fHistFolder {nullptr};                                 /// subfolder for histograms

  TParameter<int> fNevents {"nEvents", 0};  /// number of processed events

  /// Histogram for Residual Distribution
  CbmQaHist<TH1D> fh1DresidualU {"h1DresidualU", "Trd1D: Residual U;(U_{reco} - U_{MC})(cm)", 100, -.5, .5};
  CbmQaHist<TH1D> fh1DresidualV {"h1DresidualV", "Trd1D: Residual V;(V_{reco} - V_{MC})(cm)", 100, -10, 10};
  CbmQaHist<TH1D> fh1DresidualT {"h1DresidualT", "Trd1D: Residual T;(T_{reco} - T_{MC})(ns)", 100, -50, 50};

  CbmQaHist<TH1D> fh2DresidualX {"h2DresidualX", "Trd2D: Residual X;(X_{reco} - X_{MC})(cm)", 100, -5, 5};
  CbmQaHist<TH1D> fh2DresidualY {"h2DresidualY", "Trd2D: Residual Y;(Y_{reco} - Y_{MC})(cm)", 100, -5, 5};
  CbmQaHist<TH1D> fh2DresidualT {"h2DresidualT", "Trd2D: Residual T;(T_{reco} - T_{MC})(ns)", 100, -1000, 1000};

  /// Histogram for PULL Distribution
  CbmQaHist<TH1D> fh1DpullU {"h1DpullU", "Trd1D: Pull U;(U_{reco} - U_{MC}) / #sigmaU_{reco}", 100, -5, 5};
  CbmQaHist<TH1D> fh1DpullV {"h1DpullV", "Trd1D: Pull V;(V_{reco} - V_{MC}) / #sigmaV_{reco}", 100, -5, 5};
  CbmQaHist<TH1D> fh1DpullT {"h1DpullT", "Trd1D: Pull T;(T_{reco} - T_{MC}) / #sigmaT_{reco}", 100, -5, 5};

  CbmQaHist<TH1D> fh2DpullX {"h2DpullX", "Trd2D: Pull X;(X_{reco} - X_{MC}) / #sigmaX_{reco}", 100, -5, 5};
  CbmQaHist<TH1D> fh2DpullY {"h2DpullY", "Trd2D: Pull Y;(Y_{reco} - Y_{MC}) / #sigmaY_{reco}", 100, -5, 5};
  CbmQaHist<TH1D> fh2DpullT {"h2DpullT", "Trd2D: Pull T;(T_{reco} - T_{MC}) / #sigmaT_{reco}", 100, -5, 5};

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
  CbmTrackerInputQaTrd(const CbmTrackerInputQaTrd&) = delete;

  /// Suppressed assignment operator
  CbmTrackerInputQaTrd& operator=(const CbmTrackerInputQaTrd&) = delete;

  ClassDef(CbmTrackerInputQaTrd, 0);
};

#endif
