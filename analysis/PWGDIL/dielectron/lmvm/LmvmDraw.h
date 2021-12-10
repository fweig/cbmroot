/* Copyright (C) 2011-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Elena Lebedeva */

#ifndef LMVM_DRAW_H
#define LMVM_DRAW_H

#include "TObject.h"

#include <functional>
#include <string>
#include <vector>

#include "LmvmCuts.h"
#include "LmvmDef.h"
#include "LmvmHist.h"

//#include "CbmHistManager.h"

class TH1;
class TH2D;
class TH1D;
class TFile;
class TCanvas;
//class CbmHistManager;


class LmvmDraw : public TObject {

public:
  LmvmDraw();

  virtual ~LmvmDraw() { ; }

  /**
     * \brief Implement functionality of drawing histograms in the macro
     * from the specified file, this function should be called from macro.
     * \param[in] fileName Name of the input file with histograms.
     * \param[in] outputDir Path to the output directory (if it does not exist, it will be created automatically).
     * \param[in] useMvd draw histograms related to the MVD detector?
     * \param[in] drawSig Do you want to draw significance histograms?
     **/
  void DrawHistFromFile(const std::string& fileName, const std::string& outputDir = "", bool useMvd = true,
                        bool drawSig = true);

private:
  Int_t fNofEvents = 0;  // number of events of current job

  bool fUseMvd           = false;  // do you want to draw histograms related to the MVD detector?
  bool fDrawSignificance = false;  // do you want to draw significance histograms of 1D cuts?

  LmvmCuts fCuts;  // electron identification and analysis cuts

  LmvmHist fH;
  std::string fOutputDir = "";  // output directory for results

  /**
     * \brief Rebin minv histograms for better drawing. Should be called after
     * calculation of S/BG ratios.
     */
  void RebinMinvHist();

  /**
     * \brief Save all created canvases to images.
     */
  void SaveCanvasToImage();

  /**
     * Produce 1D significance histogram Significance=S/sqrt(S+BG).
     * \param[in] s Histogram with signal.
     * \param[in] bg Histogram eith background.
     * \param[in] name Name of new significance histogram.
     * \param[in] option Could be "right" or "left".
     */
  TH1D* CreateSignificanceH1(TH1D* s, TH1D* bg, const std::string& name, const std::string& option);

  /**
     * Produce 2D significance histogram Significance=S/sqrt(S+BG).
     */
  TH2D* CreateSignificanceH2(TH2D* signal, TH2D* bg, const std::string& name, const std::string& title);

  void DrawCutEffH1(const std::string& hist, const std::string& option);

  void DrawAnaStepMany(const std::string& cName, std::function<void(ELmvmAnaStep)> drawFunc);

  void DrawPtY(ELmvmAnaStep step);
  void DrawRapidity(ELmvmAnaStep step);
  void DrawPtYEfficiency(ELmvmAnaStep step);
  void DrawMinvSBg(ELmvmAnaStep step);
  void DrawMinvBgPairSrc(ELmvmAnaStep step);
  void DrawMinvMatching(ELmvmAnaStep step);
  void DrawSrcAnaStepEpEmH1(const std::string& cName, ELmvmAnaStep step);

  void DrawMisc();

  void DrawSrcH1(const std::string& hist, ELmvmAnaStep step = ELmvmAnaStep::Undefined, bool doScale = true);
  void DrawAnaStepH1(const std::string& hist, bool logy = false);

  void Draw1DCut(const std::string& hist, const std::string& sigOption, double cut = -999999.);
  void Draw2DCut(const std::string& hist, double cutCrossX = -999999., double cutCrossY = -999999.);
  void DrawCuts();

  void DrawSrcBgPairs(ELmvmAnaStep step, bool inPercent, bool drawAnaStep = true);
  void DrawSrcBgPairsAll();

  void DrawMomAccEpEm();

  void DrawPiMom();

  void Draw2DCutTriangle(double xCr, double yCr);

  void DrawMismatchesAndGhosts();

  void DrawGammaVertex();

  void DrawMinvAll();

  void DrawBgSource2D(const std::string& cName, const std::string& hName, const std::vector<std::string>& yLabels,
                      double scale, const std::string& zTitle);


  void DrawBgSourceTracks();

  void SetAnalysisStepAxis(TH1* h);

  void DrawMinvPt(ELmvmAnaStep step);

  void DrawSrcAnaStepH1(const std::string& hName, ELmvmAnaStep step);

  void DrawMvdCutQa();

  void DrawMvdAndStsHist();

  void DrawPmtXY();

  bool SkipMvd(ELmvmAnaStep step);

  LmvmDraw(const LmvmDraw&);
  LmvmDraw& operator=(const LmvmDraw&);

  ClassDef(LmvmDraw, 1);
};

#endif
