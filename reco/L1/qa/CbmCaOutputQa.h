/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaOutputQa.h
/// @brief  Tracking output QA-task (header)
/// @since  24.02.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#ifndef CbmCaOutputQa_h
#define CbmCaOutputQa_h 1

#include "CbmCaMCModule.h"
#include "CbmCaTimeSliceReader.h"
#include "CbmCaTrackTypeQa.h"
#include "CbmL1DetectorID.h"
#include "CbmL1Hit.h"
#include "CbmQaTask.h"

#include <array>
#include <memory>

#include "CaToolsDebugger.h"
#include "L1Parameters.h"

namespace cbm::ca
{
  /// @brief  QA-task for CA tracking output results
  ///
  class OutputQa : public CbmQaTask {
  public:
    /// @brief Enumeration of track category
    enum ETrackType
    {
      kAll,   ///< all tracks
      kPrim,  ///< primary tracks
      kSec,   ///< secondary tracks
      // kPrimEP,  ///< primary electron tracks
      // kPrimEM,  ///< primary positron tracks
      // kSecEP,   ///< secondary electron tracks
      // kSecEM,   ///< secondary positron tracks
      // kPrimMUP, ///< primary .....
      // kPrimMUM,
      // kSecMUP,
      // kSecMUM,
      kPrimPIP,
      kPrimPIM,
      kSecPIP,
      kSecPIM,
      // kPrimKP,
      // kPrimKM,
      // kSecKP,
      // kSecKM,
      // kPrimP,
      // kPrimPBAR,
      // kSecP,
      // kSecPBAR,
      // kPrimD,
      // kPrimDBAR,
      // kSecD,
      // kSecDBAR,
      // kPrimT,
      // kPrimTBAR,
      // kSecT,
      // kSecTBAR,
      // kPrim3HE,
      // kPrim3HEBAR,
      // kSec3HE,
      // kSec3HEBAR,
      // kPrim4HE,
      // kPrim4HEBAR,
      // kSec4HE,
      // kSec4HEBAR,
      kEND
    };


    /// @brief  Constructor from parameters
    /// @param  verbose   Verbosity level
    /// @param  isMCUsed  Flag, if MC information is available for this task
    OutputQa(int verbose, bool isMCUsed);

    /// @brief Enables debugger
    /// @param filename  Name of output ROOT file
    ///
    /// Creates a debugger and enables its usage inside a QA task
    void EnableDebugger(const char* filename);

    /// @brief Reads defined parameters object from file
    /// @param filename  Name of parameter file
    /// @note  TEMPORARY FUNCTION, A SEPARATE PARAMETERS INITIALIZATION CLASS IS TO BE USED
    void ReadParameters(const char* filename);

    /// @brief Sets MVD use flag
    /// @param flag  Boolean flag: true - detector subsystem is used, false - detector subsystem is ignored
    void SetUseMvd(bool flag = true) { fbUseMvd = flag; }

    /// @brief Sets STS use flag
    /// @param flag  Boolean flag: true - detector subsystem is used, false - detector subsystem is ignored
    void SetUseSts(bool flag = true) { fbUseSts = flag; }

    /// @brief Sets MuCh use flag
    /// @param flag  Boolean flag: true - detector subsystem is used, false - detector subsystem is ignored
    void SetUseMuch(bool flag = true) { fbUseMuch = flag; }

    /// @brief Sets TRD use flag
    /// @param flag  Boolean flag: true - detector subsystem is used, false - detector subsystem is ignored
    void SetUseTrd(bool flag = true) { fbUseTrd = flag; }

    /// @brief Sets TOF use flag
    /// @param flag  Boolean flag: true - detector subsystem is used, false - detector subsystem is ignored
    void SetUseTof(bool flag = true) { fbUseTof = flag; }

    /// @brief Sets STS tracking mode
    void SetStsTrackingMode() { fTrackingMode = ECbmTrackingMode::kSTS; }

    /// @brief Sets mCBM global tracking mode
    void SetMcbmTrackingMode() { fTrackingMode = ECbmTrackingMode::kMCBM; }

    /// @brief Sets performance mode
    /// @param mode  Performance mode (3 is default: TODO - test)
    void SetPerformanceMode(int mode) { fPerformanceMode = mode; }

    ClassDef(OutputQa, 0);

  protected:
    /// @brief Checks results of the QA and returns a success flag
    /// @return  true   QA is passed
    /// @return  false  QA is failed
    bool Check() { return true; }

    /// @brief Initializes canvases
    InitStatus InitCanvases();

    /// @brief Initialises data branches in the beginning of the run
    InitStatus InitDataBranches();

    /// @brief Initializes histograms
    InitStatus InitHistograms();

    /// @brief Initializes time slice
    /// @note  Is called in the FairTask::Exec function
    InitStatus InitTimeSlice();

    /// @brief Fills histograms from time slice or event
    void FillHistograms();

    /// @brief De-initializes histograms
    void DeInit() {}

  private:
    // Flags for detector subsystems being used
    bool fbUseMvd  = false;  ///< is MVD used
    bool fbUseSts  = false;  ///< is STS used
    bool fbUseMuch = false;  ///< is MuCh used
    bool fbUseTrd  = false;  ///< is TRD used
    bool fbUseTof  = false;  ///< is TOF used

    int fPerformanceMode = 3;  ///< Performance mode

    ECbmTrackingMode fTrackingMode = ECbmTrackingMode::kSTS;  ///< Tracking mode

    std::unique_ptr<TimeSliceReader> fpTSReader       = nullptr;  ///< Reader of the time slice
    std::unique_ptr<CbmCaMCModule> fpMCModule         = nullptr;  ///< MC module
    std::shared_ptr<L1IODataManager> fpDataManager    = nullptr;  ///< Data manager
    std::shared_ptr<::ca::tools::Debugger> fpDebugger = nullptr;  ///< Debugger
    std::shared_ptr<L1Parameters> fpParameters        = nullptr;  ///< Tracking parameters object

    L1Vector<CbmL1HitId> fvHitIds {"CbmCaOutputQa::fvHitIds"};
    L1Vector<CbmL1HitDebugInfo> fvHits {"CbmCaOutputQa::fvHits"};
    L1Vector<CbmL1Track> fvRecoTracks {"CbmCaOutputQa::fvRecoTracks"};
    ::ca::tools::MCData fMCData;  ///< Input MC data (points and tracks)


    // *************************
    // **  List of histograms **
    // *************************

    /// Histograms of different track types
    std::array<std::unique_ptr<TrackTypeQa>, ETrackType::kEND> fvpTrackHistograms;

    std::array<std::tuple<Style_t, Marker_t, Color_t>, ETrackType::kEND> fvpTrackHistogramStyles;

    // Reconstructed tracks
    TH1F* fph_reco_p        = nullptr;  ///< Total momentum over charge of reconstructed tracks
    TH1F* fph_reco_pt       = nullptr;  ///< Transverse momentum over charge of reconstructed tracks
    TH1F* fph_reco_phi      = nullptr;  ///< Azimuthal angle of reconstructed tracks
    TH1F* fph_reco_tx       = nullptr;  ///< Slope along x-axis of reconstructed tracks
    TH1F* fph_reco_ty       = nullptr;  ///< Slope along y-axis of reconstructed tracks
    TH1F* fph_reco_fhitR    = nullptr;  ///< Distance of the first hit from z-axis for reconstructed tracks
    TH1F* fph_reco_nhits    = nullptr;  ///< Hit number of reconstructed tracks
    TH1F* fph_reco_fsta     = nullptr;  ///< First station index of reconstructed tracks
    TH1F* fph_reco_chi2_ndf = nullptr;  ///< Fit chi2 over NDF of reconstructed tracks
    TH1F* fph_reco_prob     = nullptr;  ///< Fit probability of reconstructed tracks
    TH1F* fph_rest_chi2_ndf = nullptr;  ///< Fit chi2 over NDF of non-reconstructable tracks
    TH1F* fph_rest_prob     = nullptr;  ///< Fit probability of non-reconstructable tracks

    // Ghost tracks
    TH1F* fph_ghost_p            = nullptr;  ///< Total momentum over charge of ghost tracks
    TH1F* fph_ghost_pt           = nullptr;  ///< Transverse momentum over charge of ghost tracks
    TH1F* fph_ghost_phi          = nullptr;  ///< Azimuthal angle of ghost tracks
    TH1F* fph_ghost_nhits        = nullptr;  ///< Hit number of ghost tracks
    TH1F* fph_ghost_fsta         = nullptr;  ///< First station index of ghost tracks
    TH1F* fph_ghost_purity       = nullptr;  ///< Purity of ghost tracks
    TH1F* fph_ghost_chi2_ndf     = nullptr;  ///< Fit chi2 over NDF of ghost tracks
    TH1F* fph_ghost_prob         = nullptr;  ///< Fit probability of ghost tracks
    TH1F* fph_ghost_tx           = nullptr;  ///< Slope along x-axis of ghost tracks
    TH1F* fph_ghost_ty           = nullptr;  ///< Slope along y-axis of ghost tracks
    TH1F* fph_ghost_fhitR        = nullptr;  ///< Distance of the first hit from z-axis for ghost tracks
    TH2F* fph_ghost_nhits_vs_p   = nullptr;  ///< Hit number vs. total momentum over charge of ghost tracks
    TH2F* fph_ghost_fsta_vs_p    = nullptr;  ///< First station index vs. total momentum over charge for ghost tracks
    TH2F* fph_ghost_lsta_vs_fsta = nullptr;  ///< Last station index vs. first station index of ghost tracks

    // Reco tracks vs MC variables
    TH1F* fph_reco_pMC        = nullptr;  ///< Reconstructed track distribution vs MC momemntum
    TH1F* fph_reco_yMC        = nullptr;  ///< Reconstructed track distribution vs MC rapidity
    TH2F* fph_reco_pMC_vs_yMC = nullptr;  ///< Reconstructed track phase space (MC mom vs rapidity)
    TH1F* fph_reco_purity     = nullptr;  ///< Purity of reconstructed tracks (\note purity requires MC information)

    // Residuals and pulls at the first track point
    TH1F* fph_fst_res_x    = nullptr;  ///< Residual of x at first track point [cm]
    TH1F* fph_fst_res_y    = nullptr;  ///< Residual of y at first track point [cm]
    TH1F* fph_fst_res_tx   = nullptr;  ///< Residual of tx at first track point
    TH1F* fph_fst_res_ty   = nullptr;  ///< Residual of ty at first track point
    TH1F* fph_fst_res_qp   = nullptr;  ///< Residual of q/p at first track point  [GeV/ec]
    TH1F* fph_fst_res_time = nullptr;  ///< Residual of time at first track point [ns]
    TH1F* fph_fst_res_v    = nullptr;  ///< Residual of velocity at first track point [c]

    TH1F* fph_fst_pull_x    = nullptr;  ///< Pull of x at first track point [cm]
    TH1F* fph_fst_pull_y    = nullptr;  ///< Pull of y at first track point [cm]
    TH1F* fph_fst_pull_tx   = nullptr;  ///< Pull of tx at first track point
    TH1F* fph_fst_pull_ty   = nullptr;  ///< Pull of ty at first track point
    TH1F* fph_fst_pull_qp   = nullptr;  ///< Pull of q/p at first track point  [GeV/ec]
    TH1F* fph_fst_pull_time = nullptr;  ///< Pull of time at first track point [ns]
    TH1F* fph_fst_pull_v    = nullptr;  ///< Pull of velocity at first track point [c]
  };
}  // namespace cbm::ca


#endif  // CbmCaOutputQa_h
