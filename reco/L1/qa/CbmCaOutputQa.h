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
#include "CbmL1DetectorID.h"
#include "CbmQaTask.h"

#include "CaToolsDebugger.h"
#include "L1Parameters.h"

namespace cbm::ca
{
  /// @brief  QA-task for CA tracking output results
  ///
  class OutputQa : public CbmQaTask {
  public:
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
    InitStatus InitCanvases() { return kSUCCESS; }

    /// @brief Initialises data branches in the beginning of the run
    InitStatus InitDataBranches();

    /// @brief Initializes histograms
    InitStatus InitHistograms() { return kSUCCESS; }

    /// @brief Initializes time slice
    /// @note  Is called in the FairTask::Exec function
    InitStatus InitTimeSlice();

    /// @brief Fills histograms
    void FillHistograms() {}

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

    L1Vector<CbmL1HitDebugInfo> fvHitIds {"CbmCaOutputQa::fvHitIds"};
    L1Vector<CbmL1HitDebugInfo> fvDbgHits {"CbmCaOutputQa::fvDbgHits"};
    L1Vector<CbmL1Track> fvRecoTracks {"CbmCaOutputQa::fvRecoTracks"};
    L1InputData fInputData;       ///< Input hits to CA algo
    ::ca::tools::MCData fMCData;  ///< Input MC data (points and tracks)
  };
}  // namespace cbm::ca


#endif  // CbmCaOutputQa_h
