/* Copyright (C) 2011-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva, Andrey Lebedev, Semen Lebedev [committer], Florian Uhlig */

#ifndef LMVM_DRAW_ALL
#define LMVM_DRAW_ALL

#include "TObject.h"

#include <map>
#include <string>
#include <vector>

#include "LmvmHist.h"

class TH1;
class TH2D;
class TH1D;
class TFile;
class TCanvas;
class CbmHistManager;

class LmvmDrawAll : public TObject {

public:
  LmvmDrawAll() { ; }
  virtual ~LmvmDrawAll() { ; }


  void DrawHistFromFile(const std::string& fileInmed, const std::string& fileQgp, const std::string& fileOmega,
                        const std::string& filePhi, const std::string& fileOmegaD, const std::string& outputDir = "",
                        bool useMvd = false);

private:
  bool fUseMvd;  // do you want to draw histograms related to the MVD detector?

  std::vector<LmvmHist*> fH;
  LmvmHist fHMean;

  int fRebMinv = 5;  // Rebin for minv histograms (binSize in Task.cxx is 10 MeV)

  std::string fOutputDir;  // output directory for figures

  LmvmHist* H(ELmvmSignal signal);

  int GetNofTotalEvents();


  template<class T>
  T* GetCocktailMinv(const std::string& name, ELmvmAnaStep step);

  TH1D* GetCocktailMinvH1(const std::string& name, ELmvmAnaStep step);

  /**
     * \brief Draw S/Bg vs minv.
     */
  void DrawSBgVsMinv();
  void DrawMinvAll();
  void DrawMinv(ELmvmAnaStep step);
  void DrawMinvPtAll();
  void DrawMomRecoPrecision();
  void DrawMinvBgSourcesAll();

  /**
     * \brief Draw invariant mass spectra for all signal types for specified analysis step with BG reduced by combinatorial BG.
     */
  void DrawMinvCombBgAndSignal();

  /**
     * \brief Draw invariant mass spectra in official style.
     */
  void DrawMinvOfficialStyle();  

  template<class T>
  void CreateMeanHist(const std::string& name, int nofEvents, int nofRebins = -1);  // if nRebin = -1, no rebin
  void CreateMeanHistAll();

  /**
     * \brief Save histograms for the study report
     */
  void SaveHist();

  /**
     * \brief Calculate cut efficiency in specified invariant mass region.
     * \param[in] min Minimum invariant mass.
     * \param[in] max Maximum invariant mass.
     */
  void CalcCutEffRange(double minMinv, double maxMinv);

  /**
     * \brief Calculate combinatorial BG contribution.
     */
  void CalcCombBGHistos();

  /**
     * \brief Create S/BG vs cuts for specified invariant mass range.
     * \param[in] min Minimum invariant mass.
     * \param[in] max Maximum invariant mass.
     */
  TH1D* SBgRange(double minMinv, double maxMinv);

  /**
     * \brief Draw S/BG vs plots for different mass ranges.
     */
  void SBgRangeAll();

  /**
     * \brief Draw properties of misidentified particles.
     */
  void InvestigateMisid();

  void DrawBetaMomSpectra();

  void DrawMomPluto();

  void DrawTofM2();

  void DrawGTrackVertex();

  void DrawSignificancesAll();
  void DrawSignificance(TH2D* hEl, TH2D* hBg, const std::string& name, double minZ, double maxZ, const std::string& option);

  void DrawCutEffSignal();

  void DrawPionSuppression();

  // investigate misidentifications
  void DrawMomentum();
  void DrawPtYAndTofM2Elid();
  void DrawTofPilePids();
  void DrawTofHitXY();
  void DrawPurity();
  void DrawPurityHistText(TH2D* h);
  void DrawChi2();

  void DrawMinvScaleValues();

  /**
     * \brief Draw properties of misidentified particles in comparison with not-misidentified.
     */

  void DrawSBgResults();

  LmvmSBgResultData CalculateSBgResult(ELmvmSignal signal, ELmvmAnaStep step);

  /**
     * \brief Save all created canvases to images.
     */
  void SaveCanvasToImage();

  double fZ = -44.; // z-position of target

  LmvmDrawAll(const LmvmDrawAll&);
  LmvmDrawAll operator=(const LmvmDrawAll&);

  ClassDef(LmvmDrawAll, 1);
};

#endif
