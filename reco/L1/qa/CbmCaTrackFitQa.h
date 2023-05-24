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

#include <array>

#include "L1Constants.h"
#include "L1EArray.h"
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
class TProfile;
class CbmQaCanvas;

namespace cbm::ca
{
  /// TODO: SZh 02.05.2023: Move ETrackParType to another class and apply it everywhere to track parameters
  /// @brief Enumeration for track parameter type
  enum class ETrackParType
  {
    kX,         ///< x-position
    kY,         ///< y-position
    kTX,        ///< slope along x-axis
    kTY,        ///< slope along y-axis
    kQP,        ///< charge over total momentum
    kTIME,      ///< time
    kVI,        ///< inverse speed
    kEND,       ///< end of enumeration
    kBEGIN = 0  ///< begin of enumeration
  };

  /// @brief Prefix increment operator for ETrackParType
  ETrackParType operator++(ETrackParType& type);

  /// @brief Postfix increment operator for ETrackParType
  ETrackParType operator++(ETrackParType& type, int);


  /// @brief Alias to array, indexed with ETrackParType enumeration
  template<typename T>
  using TrackParArray_t = L1EArray<ETrackParType, T>;

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

    /// @brief Fit histograms
    void FitHistograms() {}

    /// @brief Creates residuals plot
    CbmQaCanvas* CreateResidualPlot();

    /// @brief Creates pulls plot
    CbmQaCanvas* CreatePullPlot();

    /// @brief Creates resolutionis plot
    CbmQaCanvas* CreateResolutionPlot() { return nullptr; }

    /// @brief Sets properties for a residual histogram
    /// @param type   Type of track parameter
    /// @param nBins  Number of bins
    /// @param lo     Lower boundary
    /// @param up     Upper boundary
    void SetResidualHistoProperties(ETrackParType type, int nBins, double lo, double up);

    /// @brief Sets properties for a pull histogram
    /// @param type   Type of track parameter
    /// @param nBins  Number of bins
    /// @param lo     Lower boundary
    /// @param up     Upper boundary
    void SetPullHistoProperties(ETrackParType type, int nBins, double lo, double up);

    // ************************
    // ** List of histograms **
    // ************************


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

    // ** Resolution profiles **
    TProfile* fph_res_p_pMC         = nullptr;  ///< Resolution of momentum [GeV/c]
    TProfile* fph_res_phi_phiMC     = nullptr;  ///< Resolution of azimuthal angle [rad]
    TProfile* fph_res_theta_thetaMC = nullptr;  ///< Resolution of polar angle [rad]

    // **************************
    // ** Histogram properties **
    // **************************

    // ** Binning **
    static constexpr int kCXSIZEPX = 600;  ///< Canvas size along x-axis [px]
    static constexpr int kCYSIZEPX = 600;  ///< Canvas size along y-axis [px]

  private:
    /// @brief Sets default histogram and track parameter properties
    void SetDefaultProperties();

    using FnVal_t = std::function<double()>;
    /// @brief Fills residual and pull for a given track parameter
    /// @param type     Type of the track parameter
    /// @param recoVal  Reconstructed error of quantity
    /// @param recoErr  Error of quantity
    /// @param trueVal  True value of quantity
    /// @param weight   Weight
    void FillResAndPull(ETrackParType type, double recoVal, double recoErr, double trueVal, double w);

    TrackParArray_t<TH1F*> fvphResiduals = {{0}};  ///< Residuals for different track parameters
    TrackParArray_t<TH1F*> fvphPulls     = {{0}};  ///< Pulls for different track parameters

    TrackParArray_t<bool> fvbParIgnored = {{0}};  ///< Flag: true - parameter is ignored

    // ** Distribution properties **
    TrackParArray_t<int> fvRBins  = {{0}};  ///< Number of bins, residuals
    TrackParArray_t<double> fvRLo = {{0}};  ///< Lower boundary, residuals
    TrackParArray_t<double> fvRUp = {{0}};  ///< Upper boundary, residuals

    TrackParArray_t<int> fvPBins  = {{0}};  ///< Number of bins, pulls
    TrackParArray_t<double> fvPLo = {{0}};  ///< Lower boundary, pulls
    TrackParArray_t<double> fvPUp = {{0}};  ///< Upper boundary, pulls

    TString fsTitle = "";  ///< Title of the point

    double fMass = L1Constants::phys::kMuonMass;  /// Mass of particle
  };

  // *****************************
  // **  Inline implementation  **
  // *****************************

  // ---------------------------------------------------------------------------------------------------------------------
  // TODO: Test this function for performance penalties
  inline void TrackFitQa::FillResAndPull(ETrackParType type, double recoVal, double recoErr, double trueVal, double w)
  {
    if (fvbParIgnored[type]) { return; }
    double res  = recoVal - trueVal;
    double pull = res / recoErr;
    fvphResiduals[type]->Fill(res, w);
    fvphPulls[type]->Fill(pull, w);
  }
}  // namespace cbm::ca


#endif  // CbmCaTrackFitQa_h
