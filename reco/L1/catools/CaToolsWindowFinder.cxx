/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsWindowFinder.cxx
/// \brief  Framework for CA tracking hit-finder window estimation from MC (implementation)
/// \since  14.10.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CaToolsWindowFinder.h"

#include "Logger.h"

#include "TCanvas.h"
#include "TChain.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TString.h"

#include <boost/archive/text_oarchive.hpp>

#include <fstream>
#include <sstream>

#include <yaml-cpp/yaml.h>

#include "CaToolsWFExpression.h"
#include "L1ConfigRW.h"
#include "L1Constants.h"
#include "L1SearchWindow.h"

using namespace ca::tools;
using namespace L1Constants::clrs;  // for colored logs

ClassImp(ca::tools::WindowFinder);

// ---------------------------------------------------------------------------------------------------------------------
//
WindowFinder::WindowFinder() : fpMcTripletsTree(new TChain(kTreeName))
{
  LOG(info) << kGNb << "Call" << kCLb << "WindowFinder constructor\n" << kCL;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::AddInputFile(const char* filename)
{
  assert(fpMcTripletsTree);
  auto addingRes = fpMcTripletsTree->AddFile(filename);
  if (addingRes != 1) {
    LOG(error) << "WindowFinder::AddInputFile: File \"" << filename << "\" cannot be added to the MC triplets chain, "
               << "some errors occurred\n";
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
L1SearchWindow WindowFinder::CreateSW(int iStation, const L1CAIteration& caIter)
{

  // Get a cut
  TCut cut = GetTrackSelectionCut(iStation, caIter);

  if (TString(fExtraCut.GetTitle()).Length()) { cut = cut && fExtraCut; }

  // Set a unique to a canvas using the cut expression
  TString sUniqueName = cut.GetTitle();
  sUniqueName.ReplaceAll(" ", "");

  // Prepare a canvas and pads to draw the histograms
  TString canvName = TString("canv_") + sUniqueName;
  TCanvas* canv    = new TCanvas(canvName, canvName, 3000, 2000);
  fvpCanvases.push_back(canv);  // Register canvas

  // Pads for base distributions
  TPad* padBase[EExpr::kEND];  /// dx vs. x0, dx vs. y0, dy vs. x0, dy vs. y0
  padBase[EExpr::kDxVsX0] = new TPad("padBase_dx_x0", "padBase_dx_x0", 0.00, 0.50, 0.25, 0.80);
  padBase[EExpr::kDxVsY0] = new TPad("padBase_dx_y0", "padBase_dx_y0", 0.25, 0.50, 0.50, 0.80);
  padBase[EExpr::kDyVsX0] = new TPad("padBase_dy_x0", "padBase_dy_x0", 0.50, 0.50, 0.75, 0.80);
  padBase[EExpr::kDyVsY0] = new TPad("padBase_dy_y0", "padBase_dy_y0", 0.75, 0.50, 1.00, 0.80);

  // Pads for slices
  TPad* padSlices[EExpr::kEND];  /// projections
  padSlices[EExpr::kDxVsX0] = new TPad("padSlices_dx_x0", "padSlices_dx_x0", 0.00, 0.00, 0.25, 0.50);
  padSlices[EExpr::kDxVsY0] = new TPad("padSlices_dx_y0", "padSlices_dx_y0", 0.25, 0.00, 0.50, 0.50);
  padSlices[EExpr::kDyVsX0] = new TPad("padSlices_dy_x0", "padSlices_dy_x0", 0.50, 0.00, 0.75, 0.50);
  padSlices[EExpr::kDyVsY0] = new TPad("padSlices_dy_y0", "padSlices_dy_y0", 0.75, 0.00, 1.00, 0.50);

  for (int iExpr = 0; iExpr < EExpr::kEND; ++iExpr) {
    padSlices[iExpr]->Draw();
    padBase[iExpr]->Draw();
  }

  // A pad for text
  TPad* padDescr = new TPad("padDescr", "padDescr", 0.00, 0.80, 1.00, 1.00);  /// Definitions of dx and dy, cut
  padDescr->Draw();

  PrintCaseInformation(padDescr, iStation, caIter);


  // Process expressions
  WFExpression exprDxVsX0(fpMcTripletsTree, GetDistExpr(EExpr::kDxVsX0), "x0");
  exprDxVsX0.SetEps(fEps);
  exprDxVsX0.SetNbins(fNbinsY, fNbinsX);
  exprDxVsX0.SetNslices(fNslices);
  exprDxVsX0.SetCut(cut);
  exprDxVsX0.SetTitle("dx vs. x0;x0 [cm];dx [cm]");
  exprDxVsX0.SetPadBase(padBase[EExpr::kDxVsX0]);
  exprDxVsX0.SetPadSlices(padSlices[EExpr::kDxVsX0]);

  WFExpression exprDxVsY0(fpMcTripletsTree, GetDistExpr(EExpr::kDxVsY0), "y0");
  exprDxVsY0.SetEps(fEps);
  exprDxVsY0.SetNbins(fNbinsY, fNbinsX);
  exprDxVsY0.SetNslices(fNslices);
  exprDxVsY0.SetCut(cut);
  exprDxVsY0.SetTitle("dx vs. y0;y0 [cm];dx [cm]");
  exprDxVsY0.SetPadBase(padBase[EExpr::kDxVsY0]);
  exprDxVsY0.SetPadSlices(padSlices[EExpr::kDxVsY0]);

  WFExpression exprDyVsX0(fpMcTripletsTree, GetDistExpr(EExpr::kDyVsX0), "x0");
  exprDyVsX0.SetEps(fEps);
  exprDyVsX0.SetNbins(fNbinsY, fNbinsX);
  exprDyVsX0.SetNslices(fNslices);
  exprDyVsX0.SetCut(cut);
  exprDyVsX0.SetTitle("dy vs. x0;x0 [cm];dy [cm]");
  exprDyVsX0.SetPadBase(padBase[EExpr::kDyVsX0]);
  exprDyVsX0.SetPadSlices(padSlices[EExpr::kDyVsX0]);

  WFExpression exprDyVsY0(fpMcTripletsTree, GetDistExpr(EExpr::kDyVsY0), "y0");
  exprDyVsY0.SetEps(fEps);
  exprDyVsY0.SetNbins(fNbinsY, fNbinsX);
  exprDyVsY0.SetNslices(fNslices);
  exprDyVsY0.SetCut(cut);
  exprDyVsY0.SetTitle("dy vs. y0;y0 [cm];dy [cm]");
  exprDyVsY0.SetPadBase(padBase[EExpr::kDyVsY0]);
  exprDyVsY0.SetPadSlices(padSlices[EExpr::kDyVsY0]);

  auto [parsUpperDxVsX0, parsLowerDxVsX0] = exprDxVsX0.CalculateParameters();
  auto [parsUpperDxVsY0, parsLowerDxVsY0] = exprDxVsY0.CalculateParameters();
  auto [parsUpperDyVsX0, parsLowerDyVsX0] = exprDyVsX0.CalculateParameters();
  auto [parsUpperDyVsY0, parsLowerDyVsY0] = exprDyVsY0.CalculateParameters();

  // Prepare a searching window
  int iCaIter = &caIter - &fvCaIters[0];
  L1SearchWindow sw(iStation, iCaIter);
  sw.SetTag(caIter.GetName().c_str());
  assert(fNparams == 1);  // TMP: At the moment only constant windows available
  for (int iP = 0; iP < fNparams; ++iP) {
    sw.SetParamDxMaxVsX0(iP, parsUpperDxVsX0[iP]);
    sw.SetParamDxMinVsX0(iP, parsLowerDxVsX0[iP]);
    sw.SetParamDxMaxVsY0(iP, parsUpperDxVsY0[iP]);
    sw.SetParamDxMinVsY0(iP, parsLowerDxVsY0[iP]);
    sw.SetParamDyMaxVsX0(iP, parsUpperDyVsX0[iP]);
    sw.SetParamDyMinVsX0(iP, parsLowerDyVsX0[iP]);
    sw.SetParamDyMaxVsY0(iP, parsUpperDyVsY0[iP]);
    sw.SetParamDyMinVsY0(iP, parsLowerDyVsY0[iP]);
  }
  return sw;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::DumpCanvasesToPdf(const char* filename) const
{
  for (int iC = 0; iC < (int) fvpCanvases.size(); ++iC) {
    TString sCanvName = Form("%s_%d.pdf", filename, iC);
    fvpCanvases[iC]->SaveAs(sCanvName);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
const char* WindowFinder::GetDistExpr(EExpr expr) const
{
  assert(fTargetPos[0] == 0);
  assert(fTargetPos[1] == 0);
  switch (expr) {
    case EExpr::kDxVsX0:
    case EExpr::kDxVsY0: return Form("x1 - x0 * (z1 - (%f)) / (z0 - (%f))", fTargetPos[2], fTargetPos[2]);
    case EExpr::kDyVsX0:
    case EExpr::kDyVsY0: return Form("y1 - y0 * (z1 - (%f)) / (z0 - (%f))", fTargetPos[2], fTargetPos[2]);
    default: return "";  // TODO: throw an exception
  }
}

// ---------------------------------------------------------------------------------------------------------------------
// TODO: SZh 10.11.2022: Should it be a static function??
TCut WindowFinder::GetTrackSelectionCut(int iStation, const L1CAIteration& caIter) const
{
  TCut cut;

  // Select station
  // NOTE: iStation stands for global index of an active tracking station for which a searching window is estimated.
  //       In the MC triplets tree the station index ("s") is stored for the left station of the triplet. Thus, for
  //       doublets the station is to be selected as "iStation - 1" (search window is estimated on the middle station),
  //       and as "iStation - 2" for the triplets (search window is estimated on the right station).
  cut = cut && Form("s==%d", iStation - 1);

  // Select q/p
  cut = cut && Form("abs(q/p)<%f", caIter.GetMaxQp());

  // Select origin (primary/secondary)
  cut = cut && (caIter.GetPrimaryFlag() ? "processId==0" : "processId!=0");

  // TODO: SZh 10.11.2022: Try to apply other cuts (electron, slope etc.)
  return cut;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::PrintCaseInformation(TPad* pPad, int iStation, const L1CAIteration& caIter) const
{
  pPad->cd();

  TPaveText* textL = new TPaveText(0.01, 0.01, 0.49, 0.99);
  textL->SetBorderSize(1);
  textL->SetTextFont(42);
  textL->SetFillColor(kWhite);
  textL->SetTextAlign(11);
  textL->SetTextSize(0.1);
  textL->AddText(Form("Global index of active station: %d", iStation));
  textL->AddText(Form("Track finder iteration: %s", caIter.GetName().c_str()));
  textL->AddText(Form("  - |q/p| < %.2f e(Gev/c)^{-1}", caIter.GetMaxQp()));
  textL->AddText(Form("  - primary: %s", caIter.GetPrimaryFlag() ? "yes" : "no"));
  if (TString(fExtraCut.GetTitle()).Length()) { textL->AddText(Form("Optional cut: %s", fExtraCut.GetTitle())); }
  textL->Draw();

  TPaveText* textR = new TPaveText(0.51, 0.01, 0.99, 0.99);
  textR->SetBorderSize(1);
  textR->SetTextFont(42);
  textR->SetTextAlign(11);
  textR->SetTextSize(0.1);
  textR->SetFillColor(kWhite);
  textR->AddText("Distance expressions:");
  textR->AddText(Form("  dx = %s", GetDistExpr(EExpr::kDxVsY0)));
  textR->AddText(Form("  dy = %s", GetDistExpr(EExpr::kDyVsY0)));

  textR->Draw();
  //pPad->Draw();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::Process(Option_t* /*opt*/)
{
  std::ofstream ofs(fsOutputName);
  if (!ofs) {
    LOG(error) << "WindowFinder: failed opening file \"" << fsOutputName << " for writing search windows\"";
    return;
  }

  boost::archive::text_oarchive oa(ofs);
  oa << fNparams;
  oa << int(fvStationIndexes.size() * fvCaIters.size());
  for (auto iStation : fvStationIndexes) {
    for (const auto& iter : fvCaIters) {
      LOG(info) << "WindowFinder: processing global active station index " << kCLb << iStation << kCL
                << " and track finder iteration " << kCLb << iter.GetName() << kCL;
      auto sw = CreateSW(iStation, iter);
      oa << sw;
    }
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::ReadTrackingIterationsFromYAML(const char* filename)
{
  L1ConfigRW reader(/*pInitManager = */ nullptr, 0);
  YAML::Node config;
  try {
    config = YAML::LoadFile(filename);
  }
  catch (const YAML::BadFile& exc) {
    throw std::runtime_error("file does not exist");
  }
  catch (const YAML::ParserException& exc) {
    throw std::runtime_error("file is formatted improperly");
  }

  // Get iterations
  fvCaIters = reader.ReadCAIterations(config["l1"]["algorithm"]["iterations"]);

  // Print iterations:
  for (const auto& iter : fvCaIters) {
    LOG(info) << iter.ToString() << '\n';
  }
}


// ****************************
// ** Setters implementation **
// ****************************

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::SetBinning(int nBinsX, int nBinsY)
{
  assert(nBinsX > 0);
  assert(nBinsY > 0);
  fNbinsX = nBinsX;
  fNbinsY = nBinsY;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::SetEpsilon(float eps)
{
  assert(eps > 0. && eps <= 1.);
  fEps = eps;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::SetNslices(int nSlices)
{
  assert(nSlices > 0);
  fNslices = nSlices;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::SetStationIndexes(const std::vector<int>& vStationIndexes)
{
  fvStationIndexes.clear();
  for (auto iSt : vStationIndexes) {
    if (iSt < 1) {
      LOG(warn) << "WindowFinder::SetStationIndexes: attempt to estimate searching window for station with index "
                << iSt << " < 1. This index will be omitted";
      continue;
    }
    fvStationIndexes.push_back(iSt);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::SetTarget(double x, double y, double z)
{
  fTargetPos[0] = x;
  fTargetPos[1] = y;
  fTargetPos[2] = z;
}
