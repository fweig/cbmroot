/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackingInputQaSts.h
 * @brief  Base class for the tracking input QA (declaration)
 * @since  24.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmTrackingInputQaSts_h
#define CbmTrackingInputQaSts_h 1

#include "CbmMCDataManager.h"
#include "CbmQaCanvas.h"
#include "CbmQaHist.h"
#include "CbmSetup.h"
#include "CbmTrackingDetectorInterfaceBase.h"  // Communicate via tracking detector interface

#include <FairTask.h>

#include "TFolder.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TParameter.h"
#include "TProfile.h"
#include "TString.h"

#include <memory>
#include <tuple>
#include <vector>

class CbmDigiManager;
class CbmMCEventList;
class CbmMCDataArray;
class CbmTimeSlice;
class TClonesArray;
class CbmCluster;
class CbmMatch;
class CbmStsHit;

/// Class CbmTrackingInputQaSts is an abstract FairTask inherited class, providing interface for a
/// tracking detector QA including input geometry and data checks. One should inherit this class
/// for a particular detector subsystem.
///
class CbmTrackingInputQaSts : public FairTask {
public:
  /// Constructor from the task name and verbosity level
  CbmTrackingInputQaSts(int verbosity = 0);

  /// Destructor
  ~CbmTrackingInputQaSts();

  /// Suppress copy and move
  CbmTrackingInputQaSts(const CbmTrackingInputQaSts&) = delete;
  CbmTrackingInputQaSts(CbmTrackingInputQaSts&&)      = delete;
  CbmTrackingInputQaSts& operator=(const CbmTrackingInputQaSts&) = delete;
  CbmTrackingInputQaSts& operator=(CbmTrackingInputQaSts&&) = delete;

  /// TTask: Executes the task (processes a timeslice)
  /// \param option  Specify action for the TTask::Exec (not defined yet)
  void Exec(Option_t* /*option*/);

  /// FairTask: Action at hte end of the run
  void Finish();

  /// Gets maximum allowed distance between z-components of hit/MC-point position and the station center [cm]
  double GetMaxDistanceZ() const { return fMaxDistanceZ; }

  /// Gets maximum allowed difference between pulls distributions RMS and the expected RMS value of RMS = 1
  double GetMaxPullsRmsDiff() const { return fMaxPullsRmsDiff; }

  /// Gets minimum particle momentum
  double GetMinMomentum() const { return fMinMomentum; }

  /// Draws histograms
  TFolder& GetQa();

  /// FairTask: Task initialization in the beginning of the run
  InitStatus Init();

  /// FairTask: Task reinitialization
  InitStatus ReInit() { return Init(); }

  /// Sets maximum allowed distance between z-components of hit/MC-point position and the station center [cm]
  void SetMaxDistanceZ(double dist) { fMaxDistanceZ = dist; }

  /// Sets maximum allowed difference between pulls distributions RMS and the expected RMS value of RMS = 1
  void SetMaxPullsRmsDiff(double rmsMaxDiff) { fMaxPullsRmsDiff = rmsMaxDiff; }

  /// Sets minimum particle momentum
  void SetMinMomentum(double pMin) { fMinMomentum = pMin; }

  /// FairTask: Set parameter containers (if any)
  void SetParContainers();

  ClassDef(CbmTrackingInputQaSts, 0);  // Base class for tracking input QA

private:
  // ***************
  // ** Functions **
  // ***************

  /// Checks accumulated distributions
  bool CheckDistributions();

  /// Deinitialize data
  void DeInit();

  /// Fills distributions of residuals
  /// \return Success status
  bool FillHistosResiduals();

  /// Fits histograms
  /// \return Success status
  bool FitHistograms();

  /// Checks geometry
  InitStatus GeometryQa();

  /// Returns particle mass [GeV/c2] by its pdg code
  static double ParticleMass(int pdg);

  /// Initializes canvases
  InitStatus InitCanvases();

  /// Initializes base histograms
  InitStatus InitHistograms();

  /// Match hits with MC points and fills out the vector
  /// \param  pHit  A pointer to the hit to be matched
  /// \param  iHit  An index of the hit in the hits array
  /// \return Match object
  CbmMatch MatchHits(const CbmStsHit* pHit, int iHit);

  /// Initializes input data branches
  InitStatus ReadAndCreateDataBranches();

  /// Registers histogram
  /// \param  pHisto
  void RegisterHist(TH1* pHisto)
  {
    fHistList.push_back(pHisto);
    pHisto->SetDirectory(0);
  }

  /// Checks resolution
  /// A general scheme of resolution checks is provided in this method. One should specify particular operations in the
  /// derived classes for each particular detector....
  void ResolutionQa();

  // *****************
  // ** Data fields **
  // *****************

  // ** Flags and cuts **
  /// Flag: true - all procedures involving MC will be processed, otherwise data will be processed only
  bool fIsMcPresent {false};

  bool fIsQaPassed {true};  ///< Flag: true - QA is successful, false - QA is failed

  /// Maximum allowed distance between z-components of hit/MC-point position and the station center [cm]
  double fMaxDistanceZ {1.0};

  double fMinMomentum {0.01};  ///< Minimum value of particle momentum z component [GeV/c]

  double fMaxPullsRmsDiff {0.05};  ///< Maximum difference of the pulls RMS from unity

  // ** Input arrays **

  CbmTrackingDetectorInterfaceBase* fpDetectorInterface {nullptr};  ///< Pointer to current tracking detector I/F
  CbmTimeSlice* fpTimeSlice {nullptr};                              ///< Pointer to current time slice
  CbmDigiManager* fpDigiManager {nullptr};                          ///< Pointer to digi manager

  CbmMCEventList* fpMcEventList {nullptr};
  CbmMCDataManager* fpMcManager {nullptr};

  TClonesArray* fpClusters {nullptr};        ///< Clusters
  TClonesArray* fpHits {nullptr};            ///< Hits
  TClonesArray* fpClusterMatches {nullptr};  ///< Matches for clusters (used only in STS)
  TClonesArray* fpHitMatches {nullptr};      ///< Matches for hits, provided with CbmMatchRecoToMC task

  CbmMCDataArray* fpMcTracks {nullptr};
  CbmMCDataArray* fpMcPoints {nullptr};


  // ** Output folders **

  TFolder fOutFolder {"TrackingInputQaSts", "TrackingInputQaSts"};  ///< Output folder for this QA task
  TFolder* fpOutFolderHists {nullptr};                              ///< Subfolder for raw histograms

  std::vector<TH1*> fHistList;  ///< List of the pointers to all histograms contained in the class

  // ** Histograms **

  TParameter<long> fNevents {"nEvents", 0};  ///< Number of processed events

  std::vector<CbmQaHist<TH1D>> fHistResidualX;  ///< Residual distributions for X vs index of a station
  std::vector<CbmQaHist<TH1D>> fHistResidualY;  ///< Residual distributions for X vs index of a station
  std::vector<CbmQaHist<TH1D>> fHistResidualT;  ///< Residual distributions for T vs index of a station

  std::vector<CbmQaHist<TH1D>> fHistPullX;  ///< Pulls distribution for X vs. index of a station
  std::vector<CbmQaHist<TH1D>> fHistPullY;  ///< Pulls distribution for Y vs. index of a station
  std::vector<CbmQaHist<TH1D>> fHistPullT;  ///< Pulls distribution for T vs. index of a station

  std::vector<CbmQaHist<TH1D>> fHistPointsPerHit;  ///< Distribution of MC points per one hit vs. index of a station
  std::vector<CbmQaHist<TH1D>> fHistHitsPerPoint;  ///< Distribution of hits per one MC point vs. index of a station

  // TODO: There is a class TEfficiency in the ROOT, probably it's a good idea to use it here (S.Zharko)
  std::vector<CbmQaHist<TProfile2D>> fHistEfficiencyXY;  ///<
  std::vector<CbmQaHist<TProfile>> fHistEfficiencyR;     ///<

  static constexpr std::tuple<int, double, double> fRangeResidualX {100, -0.01, 0.01};
  static constexpr std::tuple<int, double, double> fRangeResidualY {100, -0.03, 0.03};
  static constexpr std::tuple<int, double, double> fRangeResidualT {100, -25., 25.};

  static constexpr std::tuple<int, double, double> fRangePullX {100, -5., 5.};
  static constexpr std::tuple<int, double, double> fRangePullY {100, -5., 5.};
  static constexpr std::tuple<int, double, double> fRangePullT {100, -5., 5.};

  static constexpr std::tuple<int, double, double> fRangePointsPerHit {10, -0.5, 9.5};
  static constexpr std::tuple<int, double, double> fRangeHitsPerPoint {10, -0.5, 9.5};


  // ** Canvases **

  CbmQaCanvas fCanvResidualX {"c_sts_residualX", "STS X Residual distributions", 1500, 1000};
  CbmQaCanvas fCanvResidualY {"c_sts_residualY", "STS Y Residual distributions", 1500, 1000};
  CbmQaCanvas fCanvResidualT {"c_sts_residualT", "STS T Residual distributions", 1500, 1000};

  CbmQaCanvas fCanvPullX {"c_sts_pullX", "STS X Pull distributions", 1500, 1000};
  CbmQaCanvas fCanvPullY {"c_sts_pullY", "STS Y Pull distributions", 1500, 1000};
  CbmQaCanvas fCanvPullT {"c_sts_pullT", "STS T Pull distributions", 1500, 1000};

  CbmQaCanvas fCanvPointsPerHit {"c_sts_PointsPerHit", "STS MC Points per Hit", 1500, 1000};
  CbmQaCanvas fCanvHitsPerPoint {"c_sts_HitsPerPoint", "STS Hits per MC Point", 1500, 1000};

  CbmQaCanvas fCanvEfficiencyR {"c_sts_EfficiencyR", "STS Efficiency vs. R", 1500, 1000};
  CbmQaCanvas fCanvEfficiencyXY {"c_sts_EfficiencyXY", "STS Efficiency vs. X and Y", 1500, 1000};


  // ************
  // ** Output **
  // ************
};

#endif
