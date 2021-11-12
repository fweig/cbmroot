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

  std::string fOutputDir;  // output directory for figures

  LmvmHist* H(ELmvmSignal signal);

  int GetNofTotalEvents();


  template<class T>
  T* GetCocktailMinv(const std::string& name, ELmvmAnaStep step);

  TH1D* GetCocktailMinvH1(ELmvmAnaStep step);

  /**
     * \brief Draw S/Bg vs minv.
     */
  void DrawSBgVsMinv();
  void DrawMinvAll();
  void DrawMinv(ELmvmAnaStep step);
  void DrawMinvPtAll();

  /**
     * \brief Draw invariant mass spectra for all signal types for specified analysis step with BG reduced by combinatorial BG.
     * \param[in] step Analysis step.
     */
  void DrawMinvCombSignalAndBg();

  template<class T>
  void CreateMeanHist(const std::string& name, int nofEvents);
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


  void DrawSBgResults();

  LmvmSBgResultData CalculateSBgResult(ELmvmSignal signal, ELmvmAnaStep step);

  /**
     * \brief Save all created canvases to images.
     */
  void SaveCanvasToImage();


  LmvmDrawAll(const LmvmDrawAll&);
  LmvmDrawAll operator=(const LmvmDrawAll&);

  ClassDef(LmvmDrawAll, 1);
};

#endif
