/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaTrackTypeQa.h
/// @brief  QA submodule for different track types (header)
/// @since  27.03.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#ifndef CbmCaTrackTypeQa_h
#define CbmCaTrackTypeQa_h 1

#include "CbmCaTrackFitQa.h"
#include "CbmL1Hit.h"
#include "CbmQaIO.h"

#include <map>
#include <string>

#include "L1Parameters.h"

// Forward declarations
namespace ca::tools
{
  class MCData;
  class MCTrack;
}  // namespace ca::tools
class CbmL1Track;
class CbmL1HitDebugInfo;

class TH1F;
class TH2F;
class TProfile;
class TProfile2D;

namespace cbm::ca
{
  /// @brief Unified QA for a group of tracks
  ///
  /// The class provides set of histograms, counters, and efficiencies for monitoring tracks of a particular type
  /// (reco_fast_long, mc_pi, reco_ghost)
  class TrackTypeQa : virtual public CbmQaIO {
  public:
    /// @brief Constructor
    /// @param typeName   Name of tracks type (used as a prefix for histogram)
    /// @note  Track type is stored as a root directory name in the CbmQaIO base class
    /// @param prefixName Name of unique prefix
    /// @param bUseMC     Flag: true - MC is used
    /// @param pParentFolder Pointer to parent folder
    TrackTypeQa(const char* typeName, const char* prefixName, bool bUseMC, TFolder* pParentFolder);

    /// @brief Destructor
    ~TrackTypeQa() = default;

    /// @brief Copy constructor
    TrackTypeQa(const TrackTypeQa&) = delete;

    /// @brief Move constructor
    TrackTypeQa(TrackTypeQa&&) = delete;

    /// @brief Copy assignment operator
    TrackTypeQa& operator=(const TrackTypeQa&) = delete;

    /// @brief Move assignment operator
    TrackTypeQa& operator=(TrackTypeQa&&) = delete;

    /// @brief Gets title of track group
    const char* GetTitle() const { return fsTitle.Data(); }

    /// @brief Initializes histograms
    void Init();

    /// @brief Flag on MC usage by the track type
    /// @return true  Histograms relied on MC info are filled
    /// @return false Histograms relied on MC info are not filled
    bool IsMCUsed() const { return fbUseMC; }

    /// @brief Fills histograms with various track information
    /// @note  To be called in the loop over reconstructed tracks full sample
    /// @param iTrkReco  Index of reconstructed track
    /// @param weight    Weight
    void FillRecoTrack(int iTrkReco, double weight = 1);

    /// @brief Fills histograms with recon tracks
    /// @note  To be called once per event/TS
    void FillRecoTracks();

    /// @brief Fills histograms with mc track information
    /// @note  To be called in the loop over MC tracks full sample
    /// @param iTrkMC  Index of MC track
    /// @param weight  Weight
    void FillMCTrack(int iTrkMC, double weight = 1);

    /// @brief Fills histograms with recon tracks
    /// @note  To be called once per event/TS
    void FillMCTracks();

    /// @brief Registers the reconstructed tracks container
    /// @param vTracks  Reference to reconstructed tracks container
    void RegisterRecoTracks(L1Vector<CbmL1Track>& vTracks) { fpvRecoTracks = &vTracks; }

    /// @brief Register the reconstructed hits container
    /// @param vHits  Reference to reconstructed hits container
    void RegisterRecoHits(L1Vector<CbmL1HitDebugInfo>& vHits) { fpvHits = &vHits; }

    /// @brief Registers the MC data
    /// @param mcData  Reference to MC data object
    void RegisterMCData(::ca::tools::MCData& mcData) { fpMCData = &mcData; }

    /// @brief Registers CA parameters object
    /// @param pParameters  A shared pointer to the parameters object
    void RegisterParameters(std::shared_ptr<L1Parameters>& pParameters) { fpParameters = pParameters; }

    /// @brief Sets drawing attributes for histograms
    /// @param markerCol  Marker color
    /// @param markerSty  Marker style
    /// @param lineCol    Line color (-1: the same color as marker)
    /// @param lineSty    Line style
    void SetDrawAtt(Color_t markerCol = 1, Style_t markerSty = 20, Color_t lineCol = -1, Style_t lineSty = 1);

    /// @brief Sets title, which is to be reflected on legends and titles
    /// @param title  Title of track type
    void SetTitle(const char* title) { fsTitle = title; }

    /// @brief Sets selection cuts on MC tracks
    /// @param cut  Functor <bool(const MCTrack&)> defining cut on MC track
    ///
    /// When function returns false,
    void SetMCTrackCut(std::function<bool(const ::ca::tools::MCTrack&)> cut) { fMCTrackCut = cut; }

    /// @brief Sets selection cuts on reconstructed tracks
    /// @param cut  Functor <bool(const CbmL1Track&)> defining cut on reconstructed track
    ///
    /// When function returns false,
    void SetRecoTrackCut(std::function<bool(const CbmL1Track&)> cut) { fRecoTrackCut = cut; }

    // ************************
    // ** List of histograms **
    // ************************

    // NOTE: Naming convention:
    //  "fph_" + <"reco"/"mc"> + <quantity> + <""/"MC">. Here <"reco"/"mc"> stands for reconstructed track or true MC
    //  track, <quantity> is a name of a quantity, versus which one wants to build the dependency, <""/"MC"> stands
    //  for an origin of the quantity - reconstructed or true Monte-Carlo. If there are several quantities, they are
    //  to be separated with "_" (example: "p_yMC" -- reconstructed momentum vs MC rapidity)

    // ** Histograms from reconstructed information only **
    TH1F* fph_reco_p        = nullptr;  ///< Total momentum over charge of reconstructed tracks
    TH1F* fph_reco_pt       = nullptr;  ///< Transverse momentum over charge of reconstructed tracks
    TH1F* fph_reco_phi      = nullptr;  ///< Azimuthal angle of reconstructed tracks
    TH1F* fph_reco_theta    = nullptr;  ///< Polar angle of reconstructed tracks
    TH1F* fph_reco_tx       = nullptr;  ///< Slope along x-axis of reconstructed tracks
    TH1F* fph_reco_ty       = nullptr;  ///< Slope along y-axis of reconstructed tracks
    TH1F* fph_reco_eta      = nullptr;  ///< Pseudo-rapidity of reconstructed tracks
    TH1F* fph_reco_fhitR    = nullptr;  ///< Distance of the first hit from z-axis for reconstructed tracks
    TH1F* fph_reco_nhits    = nullptr;  ///< Hit number of reconstructed tracks
    TH1F* fph_reco_fsta     = nullptr;  ///< First station index of reconstructed tracks
    TH1F* fph_reco_chi2_ndf = nullptr;  ///< Fit chi2 over NDF of reconstructed tracks

    // ** Reconstructed track distributions, requiring MC information **
    TH1F* fph_reco_pMC     = nullptr;  ///< MC total momentum over charge of reconstructed tracks
    TH1F* fph_reco_yMC     = nullptr;  ///< MC rapidity of reconstructed tracks
    TH1F* fph_reco_etaMC   = nullptr;  ///< MC pseudo-rapidity of reconstructed tracks
    TH2F* fph_reco_p_yMC   = nullptr;  ///< Total momentum vs MC rapidity of reconstructed tracks
    TH2F* fph_reco_pMC_yMC = nullptr;  ///< MC total momentum vs MC rapidity of reconstructed tracks
    TH1F* fph_reco_ptMC    = nullptr;  ///< Transverse momentum over charge of reconstructed tracks
    TH1F* fph_reco_phiMC   = nullptr;  ///< Azimuthal angle of reconstructed tracks
    TH1F* fph_reco_thetaMC = nullptr;  ///< Polar angle of reconstructed tracks
    TH1F* fph_reco_txMC    = nullptr;  ///< Slope along x-axis of reconstructed tracks
    TH1F* fph_reco_tyMC    = nullptr;  ///< Slope along y-axis of reconstructed tracks

    // ** MC track distributions **
    TH1F* fph_mc_pMC     = nullptr;  ///< MC total momentum over charge of MC tracks
    TH1F* fph_mc_yMC     = nullptr;  ///< MC rapidity of MC tracks
    TH1F* fph_mc_etaMC   = nullptr;  ///< MC pseudo-rapidity of MC tracks
    TH2F* fph_mc_pMC_yMC = nullptr;  ///< MC total momentum vs. MC rapidity of MC tracks
    TH1F* fph_mc_ptMC    = nullptr;  ///< Transverse momentum over charge of reconstructed tracks
    TH1F* fph_mc_phiMC   = nullptr;  ///< Azimuthal angle of reconstructed tracks
    TH1F* fph_mc_thetaMC = nullptr;  ///< Polar angle of reconstructed tracks
    TH1F* fph_mc_txMC    = nullptr;  ///< Slope along x-axis of reconstructed tracks
    TH1F* fph_mc_tyMC    = nullptr;  ///< Slope along y-axis of reconstructed tracks

    // ** Efficiencies **
    TProfile* fph_eff_int     = nullptr;  ///< Integrated efficiency
    TProfile* fph_eff_pMC     = nullptr;  ///< Efficiency vs. MC momentum
    TProfile* fph_eff_yMC     = nullptr;  ///< Efficiency vs. MC rapidity
    TProfile* fph_eff_ptMC    = nullptr;  ///< Efficiency vs. MC transverse momentum
    TProfile* fph_eff_thetaMC = nullptr;  ///< Efficiency vs. MC polar angle
    TProfile* fph_eff_phiMC   = nullptr;  ///< Efficiency vs. MC azimuthal angle
    TProfile* fph_eff_nhitsMC = nullptr;  ///< Efficiency vs. MC number of hits

    TProfile2D* fph_eff_thetaMC_phiMC = nullptr;  ///< Efficiency vs. MC theta and MC phi
    TProfile2D* fph_eff_pMC_yMC       = nullptr;  ///< Efficiency vs. MC momentum and MC rapidity
    TProfile2D* fph_eff_tyMC_txMC     = nullptr;  ///< Efficiency vs. MC slopes

    // ** Fit QA **
    std::unique_ptr<TrackFitQa> fpFitQaFirstMCpoint = nullptr;
    std::unique_ptr<TrackFitQa> fpFitQaFirstHit     = nullptr;
    std::unique_ptr<TrackFitQa> fpFitQaLastHit      = nullptr;
    std::unique_ptr<TrackFitQa> fpFitQaVertex       = nullptr;

    // TODO: Provide integrated efficiencies

  private:
    /// @brief Overrided virtual function of the CbmQaIO class, defines properties of the histograms
    /// @param pHist Pointer to a histogram, which properties are to be set
    virtual void SetHistoProperties(TH1* pHist);


    bool fbUseMC    = false;  ///< Flag: true - MC information is used
    TString fsTitle = "";     ///< Title of the track category

    L1Vector<CbmL1Track>* fpvRecoTracks        = nullptr;  ///< Pointer to vector of reconstructed tracks
    L1Vector<CbmL1HitDebugInfo>* fpvHits       = nullptr;  ///< Pointer to vector of reconstructed hits
    ::ca::tools::MCData* fpMCData              = nullptr;  ///< Pointer to MC data object
    std::shared_ptr<L1Parameters> fpParameters = nullptr;  ///< Pointer to parameters object

    // ** Cuts on tracks for a given track class **

    /// Cut function on MC tracks
    std::function<bool(const ::ca::tools::MCTrack&)> fMCTrackCut = [](const ::ca::tools::MCTrack&) { return true; };

    /// Cut function on reconstructed tracks
    std::function<bool(const CbmL1Track&)> fRecoTrackCut = [](const CbmL1Track&) { return true; };


    // **************************
    // ** Histogram properties **
    // **************************

    // ** Binning **
    static constexpr int kBinsP      = 100;   ///< Number of bins, total momentum
    static constexpr double kLoP     = 0.;    ///< Lower boundary, total momentum [GeV/c]
    static constexpr double kUpP     = 10.;   ///< Upper boundary, total momentum [GeV/c]
    static constexpr int kBinsPT     = 100;   ///< Number of bins, transverse momentum
    static constexpr double kLoPT    = 0.;    ///< Lower boundary, transverse momentum [GeV/c]
    static constexpr double kUpPT    = 10.;   ///< Upper boundary, transverse momentum [GeV/c]
    static constexpr int kBinsETA    = 40;    ///< Number of bins, pseudo-rapidity
    static constexpr double kLoETA   = 0.;    ///< Lower boundary, pseudo-rapidity
    static constexpr double kUpETA   = 4.;    ///< Upper boundary, pseudo-rapidity
    static constexpr int kBinsY      = 40;    ///< Number of bins, rapidity
    static constexpr double kLoY     = 0.;    ///< Lower boundary, rapidity
    static constexpr double kUpY     = 4.;    ///< Upper boundary, rapidity
    static constexpr int kBinsPHI    = 68;    ///< Number of bins, azimuthal angle
    static constexpr double kLoPHI   = -3.2;  ///< Lower boundary, azimuthal angle [rad]
    static constexpr double kUpPHI   = +3.2;  ///< Upper boundary, azimuthal angle [rad]
    static constexpr int kBinsTHETA  = 68;    ///< Number of bins, polar angle
    static constexpr double kLoTHETA = 0.;    ///< Lower boundary, polar angle [rad]
    static constexpr double kUpTHETA = 3.2;   ///< Upper boundary, polar angle [rad]
    static constexpr int kBinsTX     = 80;    ///< Number of bins, slope along x
    static constexpr double kLoTX    = -2.;   ///< Lower boundary, slope along x
    static constexpr double kUpTX    = +2.;   ///< Upper boundary, slope along x
    static constexpr int kBinsTY     = 80;    ///< Number of bins, slope along y
    static constexpr double kLoTY    = -2.;   ///< Lower boundary, slope along y
    static constexpr double kUpTY    = +2.;   ///< Upper boundary, slope along y
    static constexpr int kBinsNHITS  = 15;    ///< Number of bins, number of hits
    static constexpr double kLoNHITS = -0.5;  ///< Lower boundary, number of hits
    static constexpr double kUpNHITS = 14.5;  ///< Upper boundary, number of hits
    static constexpr int kBinsFHITR  = 50;    ///< Number of bins, transverse distance of the 1st hit from z-axis
    static constexpr double kLoFHITR = 0.;    ///< Lower boundary, transverse distance of the 1st hit from z-axis [cm]
    static constexpr double kUpFHITR = 150.;  ///< Upper boundary, transverse distance of the 1st hit from z-axis [cm]

    // ** Drawing attributes **
    Color_t fMarkerColor = 1;   ///< Marker color
    Style_t fMarkerStyle = 20;  ///< Marker style
    Color_t fLineColor   = 1;   ///< Line color
    Style_t fLineStyle   = 1;   ///< Line style
  };
}  // namespace cbm::ca

#endif  // CbmCaOutput
