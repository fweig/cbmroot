/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */


#include "CaToolsWFExpression.h"

#include "Logger.h"

#include "TBox.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TPad.h"
#include "TTree.h"

#include <algorithm>

using namespace ca::tools;

// ---------------------------------------------------------------------------------------------------------------------
//
WFExpression::WFExpression(TTree* pTree, const char* exprDist, const char* exprParam)
  : fpTree(pTree)
  , fsExprDist(exprDist)
  , fsExprParam(exprParam)
{
  // Assertions
  // TODO: SZh 11.11.2022: REPLACE ALL THE ASSERTIONS WITH EXCEPTIONS!!!!
  assert(fsExprDist.Length());
  assert(fsExprParam.Length());
  assert(fpTree);
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::tuple<std::vector<float>, std::vector<float>> WFExpression::CalculateParameters()
{
  // Check, if data were initialized
  assert(fpPadBase);
  assert(fpPadSlices);

  // Unique name, used to access a histogram
  fsName = Form("%s:%s_%s", fsExprDist.Data(), fsExprParam.Data(), fCut.GetTitle());
  fsName.ReplaceAll(" ", "");

  // Expression to be drawn from the tree
  TString sExpr = Form("%s:%s>>htemp(%d,,,%d)", fsExprDist.Data(), fsExprParam.Data(), fNbinsParam, fNbinsDist);

  // Get base distribution
  fpPadBase->cd();
  fpTree->Draw(sExpr.Data(), fCut, "colz");
  fpHistBase = (TH2F*) gPad->GetPrimitive("htemp");
  fpHistBase->Sumw2();
  fpHistBase->SetStats(kFALSE);
  fpHistBase->SetName(fsName);
  fpHistBase->SetTitle(fsTitle);

  // Create slices
  int sliceSize = fNbinsParam / fNslices;
  fpPadSlices->cd();
  fpPadSlices->Divide(fNslices == 1 ? 1 : 2, (fNslices + 1) / 2);

  /// Vectors to store the upper and lower boundaries from each slice
  for (int iS = 0; iS < fNslices; ++iS) {
    fpPadSlices->cd(iS + 1);
    int iBinMin                                                        = sliceSize * iS;
    int iBinMax                                                        = sliceSize * (iS + 1) - 1;
    std::tie(fvLoSBoundaries[iS], fvUpSBoundaries[iS], fvSCenters[iS]) = ProcessSlice(iBinMin, iBinMax);
    assert(fvLoSBoundaries[iS] <= fvUpSBoundaries[iS]);
  }

  // Draw edges, obtained for different slices
  fpPadBase->cd();
  TGraph* pGrUpper = new TGraph(fNslices, fvSCenters.data(), fvUpSBoundaries.data());
  pGrUpper->SetMarkerColor(kOrange + 2);
  pGrUpper->SetMarkerStyle(20);
  pGrUpper->Draw("PSAME");
  TGraph* pGrLower = new TGraph(fNslices, fvSCenters.data(), fvLoSBoundaries.data());
  pGrLower->SetMarkerColor(kOrange + 2);
  pGrLower->SetMarkerStyle(20);
  pGrLower->Draw("PSAME");

  // Get window parameters
  GetConstWindowParams();

  return std::tie(fvLoParams, fvUpParams);
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::tuple<float, float, float> WFExpression::ProcessSlice(int iBinMin, int iBinMax)
{
  // ----- Create a slice
  assert(iBinMin <= iBinMax);
  float lEdge          = fpHistBase->GetXaxis()->GetBinLowEdge(iBinMin + 1);
  float uEdge          = fpHistBase->GetXaxis()->GetBinUpEdge(iBinMax + 1);
  float sliceCenter    = (lEdge + uEdge) * 0.5;
  const char* parTitle = fpHistBase->GetXaxis()->GetTitle();
  TString sSliceTitle  = Form("%.2f < %s < %.2f (bins %d-%d)", lEdge, parTitle, uEdge, iBinMin, iBinMax);
  TString sSliceName   = Form("%s_slice%d-%d", fpHistBase->GetName(), iBinMin, iBinMax);
  TH1F* pHistSlice     = (TH1F*) fpHistBase->ProjectionY(sSliceName, iBinMin, iBinMax);
  pHistSlice->SetTitle(sSliceTitle);

  // Slice drawing attributes
  constexpr float maximumScale = 1.1;
  pHistSlice->SetMaximum(pHistSlice->GetMaximum() * maximumScale);
  pHistSlice->Draw("");

  // ----- Find selection boundaries
  // A simple algorithm, which finds boundaries containing eps/2 entries to the left and right from the central region.
  int contentL      = 0;
  int contentR      = 0;
  float windowMin   = 0.f;
  float windowMax   = 0.f;
  float nEntriesTot = pHistSlice->GetEntries();

  // left side
  for (int iBin = 1; contentL < nEntriesTot * fEps * 0.5; ++iBin) {
    windowMin = pHistSlice->GetXaxis()->GetBinLowEdge(iBin);
    contentL += pHistSlice->GetBinContent(iBin);
  }

  // right side
  for (int iBin = pHistSlice->GetXaxis()->GetNbins(); contentR < nEntriesTot * fEps * 0.5; --iBin) {
    windowMax = pHistSlice->GetXaxis()->GetBinUpEdge(iBin);
    contentR += pHistSlice->GetBinContent(iBin);
  }

  // ----- Draw selection box on the histogram
  TBox* pWindowBox = new TBox(windowMin, pHistSlice->GetMinimum(), windowMax, pHistSlice->GetMaximum());
  pWindowBox->SetFillColor(kGreen);
  pWindowBox->Draw("SAME");
  pHistSlice->Draw("SAME");

  return std::tie(windowMin, windowMax, sliceCenter);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WFExpression::GetConstWindowParams()
{
  assert(kNpars == 1);  // Cannot be used for a different number of parameters
  assert(fvUpParams.size() == 1);
  assert(fvLoParams.size() == 1);
  // In case of several slices we select the most outlying point as a boundary of the window
  fvUpParams[0] = *(std::max_element(fvUpSBoundaries.begin(), fvUpSBoundaries.end()));
  fvLoParams[0] = *(std::min_element(fvLoSBoundaries.begin(), fvLoSBoundaries.end()));

  // ----- Draw the estimated parameterisations
  fpPadBase->cd();
  float xMin = fpHistBase->GetXaxis()->GetBinLowEdge(1);
  float xMax = fpHistBase->GetXaxis()->GetBinUpEdge(fpHistBase->GetXaxis()->GetNbins());
  TF1* fMin  = new TF1("fMin", "[0]", xMin, xMax);
  fMin->SetParameter(0, fvLoParams[0]);
  fMin->SetLineColor(kOrange + 2);
  fMin->Draw("LSAME");
  TF1* fMax = new TF1("fMax", "[0]", xMin, xMax);
  fMax->SetParameter(0, fvUpParams[0]);
  fMax->SetLineColor(kOrange + 2);
  fMax->Draw("LSAME");
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WFExpression::SetEps(float eps)
{
  assert(eps > 0 && eps < 1.0);
  fEps = eps;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WFExpression::SetNslices(int nSlices)
{
  assert(nSlices > 0);
  fNslices = nSlices;
  fvUpSBoundaries.clear();
  fvLoSBoundaries.clear();
  fvSCenters.clear();
  fvUpSBoundaries.resize(fNslices);
  fvLoSBoundaries.resize(fNslices);
  fvSCenters.resize(fNslices);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WFExpression::SetNbins(int nBinsDist, int nBinsParam)
{
  assert(nBinsDist > 0);
  assert(nBinsParam > 0);
  fNbinsDist  = nBinsDist;
  fNbinsParam = nBinsParam;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WFExpression::SetPadBase(TPad* pad)
{
  assert(pad);
  fpPadBase = pad;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WFExpression::SetPadSlices(TPad* pad)
{
  assert(pad);
  fpPadSlices = pad;
}
