/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaTrackFitQa.h
/// @brief  QA submodule for track fit results (residuals and puls) at selected z-coordinate (header)
/// @since  03.04.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#ifndef CbmCaTrackFitQa_h
#define CbmCaTrackFitQa_h 1

#include "CbmQaIO.h"

#include "L1Constants.h"
#include "L1Field.h"
#include "L1Fit.h"
#include "L1Vector.h"

// Forward declarations
namespace ca::tools
{
  class MCData;
  class MCPoint;
}  // namespace ca::tools
class CbmL1Track;
class CbmL1HitDebugInfo;

class TFolder;
class TH1F;

namespace cbm::ca
{
  /// @brief Set of histograms to monitor track parameters
  ///
  /// Class provides histograms to monitor track fit parameters at a selected z-coordinate.
  /// The parameters include x, y, tx, ty, time, q/p, vi (inverse speed).
  class TrackFitQa : public CbmQaIO {
  public:
    /// @brief Constructor
    /// @param pointTag       Tag for point, in which the parameters are analyzed
    /// @param prefixName     Name of unique prefix
    /// @param pParentFolder  Pointer to parent folder
    TrackFitQa(const char* pointTag, const char* prefixName, TFolder* pParentFolder);

    /// @brief Destructor
    ~TrackFitQa() = default;

    /// @brief Copy constructor
    TrackFitQa(const TrackFitQa&) = delete;

    /// @brief Move constructor
    TrackFitQa(TrackFitQa&&) = delete;

    /// @brief Copy assignment operator
    TrackFitQa& operator=(const TrackFitQa&) = delete;

    /// @brief Move assignment operator
    TrackFitQa& operator=(TrackFitQa&&) = delete;

    /// @brief Gets title of fit parameters
    const char* GetTitle() const { return fsTitle; }

    /// @brief Initializes histograms
    void Init();

    /// @brief Fills pull and residual histograms
    /// @note  To be called in the loop over reconstructed tracks full sample
    /// @param iTrkReco  Index of reconstructed track
    /// @param weight    Weight
    void Fill(const L1TrackPar& trPar, const ::ca::tools::MCPoint& mcPoint, bool bTimeMeasured, double weight = 1);

    /// @brief Sets particle mass, used for fitting a track
    /// @param mass  Particle mass [GeV/c2]
    void SetParticleMass(double mass) { fMass = mass; }

    /// @brief Sets title, which is to be reflected on legends and titles
    /// @param title  Title of fit distributions
    void SetTitle(const char* title) { fsTitle = title; }

    // ************************
    // ** List of histograms **
    // ************************

    // ** Residuals **
    TH1F* fph_res_x  = nullptr;  ///< Residual of x-coordinate [cm]
    TH1F* fph_res_y  = nullptr;  ///< Residual of y-coordinate [cm]
    TH1F* fph_res_tx = nullptr;  ///< Residual of slope along x-axis
    TH1F* fph_res_ty = nullptr;  ///< Residual of slope along y-axis
    TH1F* fph_res_qp = nullptr;  ///< Residual of q/p [ec/GeV]
    TH1F* fph_res_t  = nullptr;  ///< Residual of time [ns]
    TH1F* fph_res_vi = nullptr;  ///< Residual of inverse speed [1/c]

    // ** Pulls **
    TH1F* fph_pull_x  = nullptr;  ///< Pull of x-coordinate
    TH1F* fph_pull_y  = nullptr;  ///< Pull of y-coordinate
    TH1F* fph_pull_tx = nullptr;  ///< Pull of slope along x-axis
    TH1F* fph_pull_ty = nullptr;  ///< Pull of slope along y-axis
    TH1F* fph_pull_qp = nullptr;  ///< Pull of q/p
    TH1F* fph_pull_t  = nullptr;  ///< Pull of time
    TH1F* fph_pull_vi = nullptr;  ///< Pull of inverse speed


    // **************************
    // ** Histogram properties **
    // **************************

    // ** Binning **
    int fBinsRESX   = 200;     ///< Number of bins, residual of x
    double fLoRESX  = -4.e-3;  ///< Lower boundary, residual of x [cm]
    double fUpRESX  = +4.e-3;  ///< Upper boundary, residual of x [cm]
    int fBinsRESY   = 200;     ///< Number of bins, residual of y
    double fLoRESY  = -4.e-2;  ///< Lower boundary, residual of y [cm]
    double fUpRESY  = +4.e-2;  ///< Upper boundary, residual of y [cm]
    int fBinsRESTX  = 200;     ///< Number of bins, residual of slope along x-axis
    double fLoRESTX = -4.e-3;  ///< Lower boundary, residual of slope along x-axis
    double fUpRESTX = +4.e-3;  ///< Upper boundary, residual of slope along x-axis
    int fBinsRESTY  = 200;     ///< Number of bins, residual of slope along y-axis
    double fLoRESTY = -4.e-3;  ///< Lower boundary, residual of slope along y-axis
    double fUpRESTY = +4.e-3;  ///< Upper boundary, residual of slope along y-axis
    int fBinsRESQP  = 200;     ///< Number of bins, residual of q/p
    double fLoRESQP = -10.;    ///< Lower boundary, residual of q/p [ec/GeV]
    double fUpRESQP = +10.;    ///< Upper boundary, residual of q/p [ec/GeV]
    int fBinsREST   = 200;     ///< Number of bins, residual of time
    double fLoREST  = -10.;    ///< Lower boundary, residual of time [ns]
    double fUpREST  = +10.;    ///< Upper boundary, residual of time [ns]
    int fBinsRESVI  = 200;     ///< Number of bins, residual of inverse speed
    double fLoRESVI = -2.;     ///< Lower boundary, residual of inverse speed [1/c]
    double fUpRESVI = +2.;     ///< Upper boundary, residual of inverse speed [1/c]

    int fBinsPULLX   = 200;   ///< Number of bins, pull of x
    double fLoPULLX  = -1.;   ///< Lower boundary, pull of x [cm]
    double fUpPULLX  = +1.;   ///< Upper boundary, pull of x [cm]
    int fBinsPULLY   = 200;   ///< Number of bins, pull of y
    double fLoPULLY  = -1.;   ///< Lower boundary, pull of y [cm]
    double fUpPULLY  = +1.;   ///< Upper boundary, pull of y [cm]
    int fBinsPULLTX  = 200;   ///< Number of bins, pull of slope along x-axis
    double fLoPULLTX = -4.;   ///< Lower boundary, pull of slope along x-axis
    double fUpPULLTX = +4.;   ///< Upper boundary, pull of slope along x-axis
    int fBinsPULLTY  = 200;   ///< Number of bins, pull of slope along y-axis
    double fLoPULLTY = -4.;   ///< Lower boundary, pull of slope along y-axis
    double fUpPULLTY = +4.;   ///< Upper boundary, pull of slope along y-axis
    int fBinsPULLQP  = 200;   ///< Number of bins, pull of q/p
    double fLoPULLQP = -10.;  ///< Lower boundary, pull of q/p [ec/GeV]
    double fUpPULLQP = +10.;  ///< Upper boundary, pull of q/p [ec/GeV]
    int fBinsPULLT   = 200;   ///< Number of bins, pull of time
    double fLoPULLT  = -10.;  ///< Lower boundary, pull of time [ns]
    double fUpPULLT  = +10.;  ///< Upper boundary, pull of time [ns]
    int fBinsPULLVI  = 200;   ///< Number of bins, pull of inverse speed
    double fLoPULLVI = -2.;   ///< Lower boundary, pull of inverse speed [1/c]
    double fUpPULLVI = +2.;   ///< Upper boundary, pull of inverse speed [1/c]

  private:
    TString fsTitle = "";  ///< Title of the point

    double fMass = L1Constants::phys::kMuonMass;  /// Mass of particle
  };


}  // namespace cbm::ca


#endif  // CbmCaTrackFitQa_h
