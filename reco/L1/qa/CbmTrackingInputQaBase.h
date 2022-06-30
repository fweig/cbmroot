/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackingInputQaBase.h
 * @brief  Base class for the tracking input QA (declaration)
 * @since  24.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmTrackingInputQaBase_h
#define CbmTrackingInputQaBase_h 1

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
#include <tuple>

#include <memory>
#include <vector>

class CbmDigiManager;
class CbmMCEventList;
class CbmMCDataArray;
class CbmTimeSlice;
class TClonesArray;
class CbmCluster;
class CbmMatch;

/// Class CbmTrackingInputQaBase is an abstract FairTask inherited class, providing interface for a
/// tracking detector QA including input geometry and data checks. One should inherit this class
/// for a particular detector subsystem.
///
class CbmTrackingInputQaBase : public FairTask {
public:
  /// Constructor from the task name and verbosity level
  CbmTrackingInputQaBase(const char* name = "CbmTrackingInputQaBase", int verbose = 1);

  /// Destructor
  virtual ~CbmTrackingInputQaBase();

  /// Suppress copy and move
  CbmTrackingInputQaBase(const CbmTrackingInputQaBase&) = delete;
  CbmTrackingInputQaBase(CbmTrackingInputQaBase&&)      = delete;
  CbmTrackingInputQaBase& operator=(const CbmTrackingInputQaBase&) = delete;
  CbmTrackingInputQaBase& operator=(CbmTrackingInputQaBase&&) = delete;

  /// TTask: Clears all data structures created by a previous execution of the task
  /// \param option  Specify action for the TTask::Exec (not defined yet)
  void Clear(Option_t* option = "");

  /// Ttask: Executes the task (processes a timeslice)
  /// \param option  Specify action for the TTask::Exec (not defined yet)
  void Exec(Option_t* /*option*/);

  /// FairTask: Action at hte end of the run
  void Finish();

  //TFolder& GetQa();

  /// FairTask: Task initialization an the beginning of the run
  InitStatus Init();

  /// FairTask: Task reinitialization
  InitStatus ReInit() { return Init(); }

  /// FairTask: Set parameter containers (if any)
  void SetParContainers();

  /// Resets data fields
  void Reset();

  /// Gets pointer to hits array of a particular detector type
  virtual TClonesArray* GetHitsInput() = 0;

  /// Gets pointer to clusters array of a particular detector type
  virtual TClonesArray* GetClustersInput() = 0;

  /// Gets pointer to hit matches array for a particular detector type
  virtual TClonesArray* GetHitMatchesInput() = 0;

  /// Gets pointer to MC points for a particular detector type
  virtual CbmMCDataArray* GetMcPointsInput() = 0;

  ClassDef(CbmTrackingInputQaBase, 0);  // Base class for tracking input QA

protected:
  /// Registers histogram
  void RegisterHist(TH1* histo)
  {
    fHistList.push_back(histo);
    histo->SetDirectory(0);
  }

  /// Checks geometry
  InitStatus GeometryQa();  // TODO: Final or not final? (S.Zharko)

  /// Checks resolution
  /// A general scheme of resolution checks is provided in this method. One should specify particular operations in the
  /// derived classes for each particular detector....
  void ResolutionQa();  // TODO: Final or not final? (S.Zharko)


  // ***********
  // ** Setup **
  // ***********


  CbmTrackingDetectorInterfaceBase* fpDetectorInterface {nullptr};  ///< Pointer to current tracking detector I/F
  CbmTimeSlice* fpTimeSlice {nullptr};                              ///< Pointer to current time slice
  CbmDigiManager* fpDigiManager {nullptr};                          ///< Pointer to digi manager

  CbmMCEventList* fpMcEventList {nullptr};
  CbmMCDataManager* fpMcManager {nullptr};

  // **********
  // ** Data **
  // **********

  TClonesArray* fpClusters {nullptr};
  TClonesArray* fpHits {nullptr};
  TClonesArray* fpHitMatches {nullptr};

  CbmMCDataArray* fpMcTracks {nullptr};
  CbmMCDataArray* fpMcPoints {nullptr};

  // ********************************
  // ** List of base QA histograms **
  // ********************************

  // NOTE: One can add extra detector specific histograms in the Init method of the derived class. In that case, the
  //       histograms should be registered via RegisterHisto method

  std::vector<CbmQaHist<TH1D>> fHistResidualX;  ///< Residual distributions for X vs index of a station
  std::vector<CbmQaHist<TH1D>> fHistResidualY;  ///< Residual distributions for X vs index of a station
  std::vector<CbmQaHist<TH1D>> fHistResidualT;  ///< Residual distributions for T vs index of a station

  std::vector<CbmQaHist<TH1D>> fHistPullX;  ///< Pulls distribution for X vs. index of a station
  std::vector<CbmQaHist<TH1D>> fHistPullY;  ///< Pulls distribution for Y vs. index of a station
  std::vector<CbmQaHist<TH1D>> fHistPullT;  ///< Pulls distribution for T vs. index of a station

  std::vector<CbmQaHist<TH1I>> fHistPointsPerHit;  ///< Distribution of MC points per one hit vs. index of a station
  std::vector<CbmQaHist<TH1I>> fHistHitsPerPoint;  ///< Distribution of hits per one MC point vs. index of a station

  // Histogram properties, can be modified in the constructor of a derived class
  std::tuple<int, double, double> fRangeResidualX {100, -5., 5.};
  std::tuple<int, double, double> fRangeResidualY {100, -10., 10.};
  std::tuple<int, double, double> fRangeResidualT {100, -50., 50.};

  std::tuple<int, double, double> fRangePullX {100, -5., 5.};
  std::tuple<int, double, double> fRangePullY {100, -5., 5.};
  std::tuple<int, double, double> fRangePullT {100, -5., 5.};

  std::tuple<int, double, double> fRangePointsPerHit {10, -0.5, 9.5};
  std::tuple<int, double, double> fRangeHitsPerPoint {10, -0.5, 9.5};

  TParameter<long> fNevents {"nEvents", 0};  ///< Number of processed events

  // ************
  // ** Output **
  // ************

  std::shared_ptr<TFolder> fpOutFolder {nullptr};  ///< Output folder for this QA task
  TFolder* fpOutFolderHists {nullptr};             ///< Subfolder for raw histograms

  ECbmModuleId fDetId;     ///< ID of the detector subsystem
  TString fDetName {""};   ///< Name of the detector subsystem, used for i/o (e.g., mvd, sts, much, trd1d, trd2d, tof)
  TString fDetTitle {""};  ///< Title of the detector subsystem, used for logs (e.g., MVD, STS, MuCh, TRD-1D, ...)

private:
  /// Initializes base histograms
  InitStatus InitBaseHistograms();

  /// Initializes input data arrays
  InitStatus InitInputData();

  std::vector<TH1*> fHistList;  ///< List of the pointers to all histograms contained in the class

  bool fIsMcPresent {false};
};

#endif  // CbmTrackingInputQaBase_h
