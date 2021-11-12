/* Copyright (C) 2011-2018 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva, Andrey Lebedev, Semen Lebedev [committer], Florian Uhlig */

/** CbmAnaDielectronTaskDrawAll.h
 * @author Elena Lebedeva <e.lebedeva@gsi.de>
 * @since 2011
 * @version 2.0
 **/

#ifndef CBM_ANA_DIELECTRON_TASK_DRAW_ALL
#define CBM_ANA_DIELECTRON_TASK_DRAW_ALL

#include "CbmLmvmHist.h"

#include "TObject.h"

#include <map>
#include <string>
#include <vector>

class TH1;
class TH2D;
class TH1D;
class TFile;
class TCanvas;
class CbmHistManager;

enum SignalType
{
  kInmed  = 0,
  kQgp    = 1,
  kOmega  = 2,
  kPhi    = 3,
  kOmegaD = 4
};

class CbmAnaDielectronTaskDrawAll : public TObject {

public:
  /**
     * \brief Default constructor.
     */
  CbmAnaDielectronTaskDrawAll()
    : TObject()
    , fUseMvd(false)
    , fDrawQgp(false)
    , fHM()
    , fh_mean_bg_minv()
    , fh_mean_eta_minv()
    , fh_mean_pi0_minv()
    , fh_mean_eta_minv_pt()
    , fh_mean_pi0_minv_pt()
    , fh_mean_sbg_vs_minv()
    , fh_sum_s_minv()
    , fOutputDir("")
  {
    ;
  }

  /**
     * \brief Destructor.
     */
  virtual ~CbmAnaDielectronTaskDrawAll() { ; }

  /**
     * \brief Implement functionality of drawing histograms in the macro
     * from the specified files, this function should be called from macro.
     * \param[in] fileNameRho0 Name of the file with rho0 histograms.
     * \param[in] fileNameOmega Name of the file with omega histograms.
     * \param[in] fileNamePhi Name of the file with phi histograms.
     * \param[in] fileNameOmegaDalitz Name of the file with omegaDalitz histograms.
     * \param[in] outputDir Output directory for figures and .json file.
     * \param useMvd draw histograms related to the MVD detector?
     **/
  void DrawHistosFromFile(const std::string& fileNameInmed, const std::string& fileNameQgp,
                          const std::string& fileNameOmega, const std::string& fileNamePhi,
                          const std::string& fileNameOmegaDalitz, const std::string& outputDir = "",
                          Bool_t useMvd = false);

private:
  static const int fNofSignals = 5;

  int nRebin;

  Bool_t fUseMvd;   // do you want to draw histograms related to the MVD detector?
  Bool_t fDrawQgp;  // do you wan to draq QGP signal

  //[0]=rho0, [1]=omega, [2]=phi, [3]=omegaDalitz
  std::vector<CbmHistManager*> fHM;

  // index: AnalysisSteps
  std::vector<TH1D*> fh_mean_bg_minv;  //mean histograms from all files
  std::vector<TH1D*> fh_mean_eta_minv;
  std::vector<TH1D*> fh_mean_pi0_minv;
  std::vector<TH2D*> fh_mean_eta_minv_pt;
  std::vector<TH2D*> fh_mean_pi0_minv_pt;
  std::vector<TH1D*> fh_mean_sbg_vs_minv;  //Coctail/BG vs. invariant mass for different analysis steps

  // Combinatorial histograms
  std::vector<TH1D*> fh_mean_combPairsPM_sameEvent_minv;
  std::vector<TH1D*> fh_mean_combPairsPP_sameEvent_minv;
  std::vector<TH1D*> fh_mean_combPairsMM_sameEvent_minv;
  std::vector<TH1D*> fh_mean_combPairsPM_mixedEvents_minv;
  std::vector<TH1D*> fh_mean_combPairsPP_mixedEvents_minv;
  std::vector<TH1D*> fh_mean_combPairsMM_mixedEvents_minv;
  std::vector<TH1D*> fh_mean_combPairsPM_sameEvent_minv_raw;  // won't be scaled with binWidth
  std::vector<TH1D*> fh_mean_combPairsPP_sameEvent_minv_raw;
  std::vector<TH1D*> fh_mean_combPairsMM_sameEvent_minv_raw;
  std::vector<TH1D*> fh_mean_combPairsPM_mixedEvents_minv_raw;
  std::vector<TH1D*> fh_mean_combPairsPP_mixedEvents_minv_raw;
  std::vector<TH1D*> fh_mean_combPairsMM_mixedEvents_minv_raw;
  std::vector<TH1D*> fh_mean_combBg_errProp_minv;  // to observe error propagation
  std::vector<TH1D*> fh_mean_combSignal_errProp_minv;
  std::vector<TH1D*> fh_mean_combBg_GeomMeanSame_minv;   // geom. mean of comb. BG ( := SQRT[(B++) * (B--)] )
  std::vector<TH1D*> fh_mean_combBg_GeomMeanMixed_minv;  // geom. mean of comb. BG ( := SQRT[(b++) * (b--)] )
  std::vector<TH1D*> fh_mean_combBg_k_minv;              // k = (b+-) / ( 2 * Sqrt[(b++) * (b--)] )
  std::vector<TH1D*> fh_mean_combBg_minv;                // combinatorial BG ( := B = 2 * geomMean * k )
  std::vector<TH1D*> fh_mean_combBg_raw_minv;            // won't be scaled; for error propagation
  std::vector<TH1D*>
    fh_mean_combBg_assemb_minv;  // as previous, but with assembled same (0 - 0.3 GeV) and mixed (0.3 - 2 GeV) data
  std::vector<TH1D*> fh_mean_combSignalNpm_minv;  // combinatorial signal ( := cSig = (N+-) - B )
  std::vector<TH1D*>
    fh_mean_combSignalNpm_assemb_minv;  // as previous, but with assembled same (0 - 0.3 GeV) and mixed (0.3 - 2 GeV) data
  std::vector<TH1D*> fh_mean_combSignalBCoc_minv;  // combinatorial signal ( := cSig = (Coc + BG) - B )
  std::vector<TH1D*>
    fh_mean_combSignalBCoc_assemb_minv;  // as previous, but with assembled same (0 - 0.3 GeV) and mixed (0.3 - 2 GeV) data
  std::vector<TH1D*> fh_mean_combSBg_vs_minv;  // cocktail/combBG

  // Number of charged particles vs. momentum
  std::vector<TH1D*> fh_mean_nof_plutoElectrons;
  std::vector<TH1D*> fh_mean_nof_plutoPositrons;
  std::vector<TH1D*> fh_mean_nof_urqmdElectrons;
  std::vector<TH1D*> fh_mean_nof_urqmdPositrons;

  // index: AnalysisSteps
  std::vector<TH1D*> fh_sum_s_minv;  // sum of all signals

  std::string fOutputDir;  // output directory for figures

  TH1D* H1(int signalType, const std::string& name);

  TH2D* H2(int signalType, const std::string& name);

  /**
     * \brief Create and return cotail vs. minv
     */
  TH1D* GetCoctailMinv(CbmLmvmAnalysisSteps step);

  /**
     * \brief Draw S/Bg vs minv.
     */
  void DrawSBgVsMinv();

  /**
     * \brief Draw invariant mass histograms.
     */
  void DrawMinvAll();

  /**
     * \brief Draw invariant mass spectra for all signal types for specified analysis step.
     * \param[in] step Analysis step.
     */
  void DrawMinv(CbmLmvmAnalysisSteps step);

  /**
     * \brief Draw invariant mass spectra for all signal types for specified analysis step with BG reduced by combinatorial BG.
     * \param[in] step Analysis step.
     */
  void DrawMinvCombSignalAndBg();

  /**
     * \brief Draw invariant mass vs Pt histograms.
     */
  void DrawMinvPtAll();

  /**
     * \brief Draw invariant mass spectra vs Pt for all signal types for specified analysis step.
     * \param[in] step Analysis step.
     */
  void DrawMinvPt(CbmLmvmAnalysisSteps step);

  /**
     * \brief It creates a mean histogram from 4 files.
     */
  void FillMeanHist();

  /**
     * \brief Save histograms for the study report
     */
  void SaveHist();

  /**
     * \brief Fill sum signals.
     */
  void FillSumSignalsHist();

  /**
     * \brief Calculate cut efficiency in specified invariant mass region.
     * \param[in] min Minimum invariant mass.
     * \param[in] max Maximum invariant mass.
     */
  void CalcCutEffRange(Double_t minMinv, Double_t maxMinv);

  /**
     * \brief Calculate combinatorial BG contribution.
     */
  void CalcCombBGHistos();

  /**
     * \brief To compare outputs from simulations with different STS versions
     */
  void CompareSTSversions();

  /**
     * \brief Create S/BG vs cuts for specified invariant mass range.
     * \param[in] min Minimum invariant mass.
     * \param[in] max Maximum invariant mass.
     */
  TH1D* SBgRange(Double_t minMinv, Double_t maxMinv);

  /**
     * \brief Draw S/BG vs plots for different mass ranges.
     */
  void SBgRangeAll();

  /**
     * \brief Draw S/BG vs plots for different signals.
     */
  void DrawSBgSignals();

  /**
     * \brief Save all created canvases to images.
     */
  void SaveCanvasToImage();


  CbmAnaDielectronTaskDrawAll(const CbmAnaDielectronTaskDrawAll&);
  CbmAnaDielectronTaskDrawAll operator=(const CbmAnaDielectronTaskDrawAll&);

  ClassDef(CbmAnaDielectronTaskDrawAll, 1);
};

#endif
