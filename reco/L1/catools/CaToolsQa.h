/* Copyright (C) 2022-2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CaToolsQa.h
/// \brief  Tracking performance class (header)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsQa_h
#define CaToolsQa_h 1

#include "Logger.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TROOT.h"

#include <string_view>

#include "L1Vector.h"

class L1Parameters;
class TDirectory;
class CbmL1HitDebugInfo;
class CbmL1Track;

namespace ca::tools
{
  class MCData;

  /// Class ca::tools::Qa defines CA tracking internal performance measurement
  ///
  class Qa {
  public:
    // *****************************************
    // **     Constructors and destructor     **
    // *****************************************

    /// Default constructor
    Qa() = default;

    /// Destructor
    ~Qa() = default;

    /// Copy constructor
    Qa(const Qa& other) = delete;

    /// Move constructor
    Qa(Qa&& other) = delete;

    /// Copy assignment operator
    Qa& operator=(const Qa& other) = delete;

    /// Move assignment operator
    Qa& operator=(Qa&& other) = delete;

    /// Initializes histograms
    /// \note should be called in the beginning of the run
    void InitHistograms();

    /// Fills histograms
    /// \note should be called in the end of the event
    void FillHistograms();

    /// Saves histograms to the file
    /// \note Should be called in the end of the run
    void SaveHistograms();

    // ***********************
    // **     Accessors     **
    // ***********************

    /// Sets pointer to hit container from reference
    void SetHitContainer(const L1Vector<CbmL1HitDebugInfo>& vHits) { fpvHits = &vHits; }

    /// Sets pointer to hit container from pointer
    void SetHitContainer(const L1Vector<CbmL1HitDebugInfo>* pvHits)
    {
      assert(pvHits);
      fpvHits = pvHits;
    }


    /// Sets pointer to MC data object
    void SetMCData(const MCData* pMCData) { fpMCData = pMCData; }

    /// Sets output filename
    void SetOutputFilename(const char* filename);

    /// Sets pointer to tracking parameters object
    void SetParameters(const L1Parameters* pParameters) { fpParameters = pParameters; }

    /// Sets pointer to the vector of reconstructed tracks from reference
    void SetRecoTrackContainer(const L1Vector<CbmL1Track>& vRecoTracks) { fpvRecoTracks = &vRecoTracks; }

    /// Sets pointer to the vector of reconstructed tracks from pointer
    void SetRecoTrackContainer(const L1Vector<CbmL1Track>* pvRecoTracks)
    {
      assert(pvRecoTracks);
      fpvRecoTracks = pvRecoTracks;
    }


  private:
    /// Function, which creates a histogram and initializes it
    /// \tparam  T     Type of the histogram (\note T should not be a pointer)
    /// \param   name  Name of histogram
    /// \param   args  The rest of the arguments, which will be passed to the histogram constructor
    template<typename T, typename... Args>
    T* MakeHisto(const char* name, Args... args);

    /// Writes recursively all registered objects inside a current directory
    /// \param  pObj  Pointer to object to write
    void RecursiveWrite(TObject* pObj);

    const L1Parameters* fpParameters           = nullptr;  ///< Instance of the tracking core class parameters
    const MCData* fpMCData                     = nullptr;  ///< Container for MC information on the event
    const L1Vector<CbmL1Track>* fpvRecoTracks  = nullptr;  ///< Pointer to external reconstructed track container
    const L1Vector<CbmL1HitDebugInfo>* fpvHits = nullptr;  ///< Pointer to external hit container

    // ***********************
    // **    Histograms     **
    // ***********************

    TDirectory* fpHistoDir = nullptr;  ///< directory to store QA histograms

    std::unique_ptr<TFile> fpOutFile = nullptr;

    // **  Number of bins for different quantities

    static constexpr int kNbinsRecoP   = 50;   ///< momentum of reconstructed tracks
    static constexpr int kNbinsRecoPt  = 50;   ///< transverse momentum of reconstructed tracks
    static constexpr int kNbinsRecoPhi = 68;   ///< azimuthal angle of reconstructed tracks
    static constexpr int kNbinsNofHits = 10;   ///< number of hits of reconstructed tracks
    static constexpr int kNbinsPurity  = 100;  ///< track purity
    static constexpr int kNbinsChi2NDF = 50;   ///< fit chi-square over NDF
    static constexpr int kNbinsProb    = 505;  ///< fit probability
    static constexpr int kNbinsNofSta  = 15;   ///< number of stations
    static constexpr int kNbinsTx      = 50;   ///< slope along x-axis
    static constexpr int kNbinsTy      = 50;   ///< slope along y-axis
    static constexpr int kNbinsHitDist = 50;   ///< distance from transverse hit position to z-axis

    // ** Upper bound for quantity range for histograms
    static constexpr double kAxMaxRecoP   = 5.;    ///< momentum of reconstructed tracks [GeV/c]
    static constexpr double kAxMaxRecoPt  = 5.;    ///< transverse momentum of reconstructed tracks [GeV/c]
    static constexpr double kAxMaxRecoPhi = 3.2;   ///< azimuthal angle of reconstructed tracks [rad]
    static constexpr double kAxMaxChi2NDF = 100.;  ///< fit chi-square over NDF


    // **  Pointers to histogram objects **

    // Reconstructed tracks
    TH1F* fph_reco_p        = nullptr;  ///< Total momentum over charge of reconstructed tracks
    TH1F* fph_reco_pt       = nullptr;  ///< Transverse momentum over charge of reconstructed tracks
    TH1F* fph_reco_phi      = nullptr;  ///< Azimuthal angle of reconstructed tracks
    TH1F* fph_reco_nhits    = nullptr;  ///< Hit number of reconstructed tracks
    TH1F* fph_reco_fsta     = nullptr;  ///< First station index of reconstructed tracks
    TH1F* fph_reco_purity   = nullptr;  ///< Purity of reconstructed tracks (\note purity requires MC information)
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

    // TODO: Add other histograms

    // Registered primary tracks
    // Registered secondary tracks
    // Reconstructable primary tracks
    // Reconstructable secondary tracks
    // Reconstructed primary tracks
    // Reconstructed secondary tracks
    // Reconstructed tracks
    // Ghost tracks

    // ** Histograms appearance attributes
    static constexpr double kHtextSize = 0.04;


    /// A map of the histogram pointers. The key is a name of the histogram
    std::unordered_map<std::string_view, TH1*> fmpHistoList;  ///< List of pointers to histograms

    // *******************************
    // **     Utility variables     **
    // *******************************

    int fNofEvents = 0;  ///< Event counter
  };
}  // namespace ca::tools

// **********************************************************
// **     Inline and template function implementations     **
// **********************************************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, typename... Args>
T* ca::tools::Qa::MakeHisto(const char* name, Args... args)
{
  // Check, if the histogram with a given name was already created. If so, delete it
  if (gROOT->FindObject(name)) {
    LOG(warn) << "CA QA: A histogram with name \"" << name << "\" was previously created and will be deleted now "
              << "to avoid memory leaks";
    T* pHisto = (T*) gROOT->FindObject(name);
    delete pHisto;
  }

  // Create a new histogram or profile
  T* pHisto = new T(name, args...);

  // Register histogram in the current TDirectory
  pHisto->SetDirectory(fpHistoDir);

  // Register histogram in the list
  fmpHistoList[pHisto->GetName()] = (TH1*) pHisto;

  pHisto->Sumw2();

  // Appearance settings
  std::array<TAxis*, 2> vpAxes = {pHisto->GetXaxis(), pHisto->GetYaxis()};
  for (auto* pAxis : vpAxes) {
    pAxis->SetTitleSize(kHtextSize);
    pAxis->SetLabelSize(kHtextSize);
  }

  return pHisto;
}


#endif  // CaToolsQa_h
