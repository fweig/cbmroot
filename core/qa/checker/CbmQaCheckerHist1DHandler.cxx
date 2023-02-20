/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerHist1DHandler.cxx
/// @brief  Handler class for 1D-histograms (including TProfile objects) (implementation)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  09.02.2023

#include "CbmQaCheckerHist1DHandler.h"

#include "CbmQaCanvas.h"
#include "CbmQaCheckerTypedefs.h"

#include "Logger.h"

#include "TCanvas.h"
#include "TError.h"
#include "TFolder.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TLegend.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TObject.h"
#include "TStyle.h"

#include <bitset>
#include <limits>

using cbm::qa::checker::CmpResult_t;
using cbm::qa::checker::Hist1DHandler;

// ---------------------------------------------------------------------------------------------------------------------
//
Hist1DHandler::Hist1DHandler(int iObject, int iFile, int iDataset) : ObjectHandler(iObject, iFile, iDataset, "TH1") {}

// ---------------------------------------------------------------------------------------------------------------------
//


// ---------------------------------------------------------------------------------------------------------------------
//
CmpResult_t Hist1DHandler::Compare(const TNamed* pNum, const TNamed* pDenom) const
{
  std::bitset<sizeof(CmpResult_t) * 8> res;
  auto* pNumerator   = static_cast<const TH1*>(pNum);
  auto* pDenominator = static_cast<const TH1*>(pDenom);
  if (fOptionBits[EFlags::kPOINT] && ComparePointToPoint(pNumerator, pDenominator)) { res[EFlags::kPOINT] = true; }
  if (fOptionBits[EFlags::kRATIO] && CompareRatioDeviation(pNumerator, pDenominator)) { res[EFlags::kRATIO] = true; }
  if (fOptionBits[EFlags::kCHI2] && CompareWithChi2(pNumerator, pDenominator)) { res[EFlags::kCHI2] = true; }
  if (fOptionBits[EFlags::kKOLM] && CompareWithKolmogorov(pNumerator, pDenominator)) { res[EFlags::kKOLM] = true; }
  return res.to_ulong();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void Hist1DHandler::CreateCanvases()
{
  int nVersions = fpObjDB->GetNofVersions();
  int iDef      = fpObjDB->GetDefaultID();

  // ----- Canvas: comparison of original histograms, ratios and subtractions
  std::string sCanvName  = fsBaseName + "_cmp_canvas";
  std::string sCanvTitle = "Comparison result for \"" + fsBaseName + "\"";
  fpCanvas               = std::make_shared<CbmQaCanvas>(sCanvName.data(), sCanvTitle.data(), 1500, 500);
  fpCanvas->cd();
  auto* pPadOrig = new TPad("p1", "p1", 0.0, 0.0, 0.3333, 1.0);
  pPadOrig->SetMargin(0.20, 0.05, 0.20, 0.10);
  fpCanvas->cd();
  auto* pPadRatio = new TPad("p2", "p2", 0.3333, 0.0, 0.6666, 1.0);
  pPadRatio->SetMargin(0.20, 0.05, 0.20, 0.10);
  fpCanvas->cd();
  auto* pPadDiff = new TPad("p3", "p3", 0.6666, 0.0, 1.0, 1.0);
  pPadDiff->SetMargin(0.20, 0.05, 0.20, 0.10);

  // Title definitions
  const char* title      = fvpObjects[0]->GetTitle();
  const char* titleRatio = Form("Ratio to %s", fpObjDB->GetVersionLabel(iDef).data());
  const char* titleDiff  = Form("Difference from %s", fpObjDB->GetVersionLabel(iDef).data());
  const char* xAxisTitle = static_cast<TH1*>(fvpObjects[0])->GetXaxis()->GetTitle();
  const char* yAxisTitle = static_cast<TH1*>(fvpObjects[0])->GetYaxis()->GetTitle();

  // Original histograms
  pPadOrig->cd();
  auto* pMultiGraphOrig = new TMultiGraph(fsBaseName.data(), title);
  for (int iV = 0; iV < nVersions; ++iV) {
    auto* pCopy = new TGraphAsymmErrors((TH1*) fvpObjects[iV]);
    pCopy->SetMarkerStyle(20);
    pCopy->SetTitle(fpObjDB->GetVersionLabel(iV).data());
    pMultiGraphOrig->Add(pCopy, "P");
  }
  pMultiGraphOrig->GetXaxis()->SetTitle(xAxisTitle);
  pMultiGraphOrig->GetYaxis()->SetTitle(yAxisTitle);
  pMultiGraphOrig->Draw("A pmc plc");
  pPadOrig->BuildLegend();

  auto* pDefault = static_cast<TH1*>(fvpObjects[iDef]);

  // Histogram ratios to default
  pPadRatio->cd();
  auto* pMultiGraphRatio = new TMultiGraph((fsBaseName + "_ratio").data(), titleRatio);
  for (int iV = 0; iV < nVersions; ++iV) {
    if (iV == iDef) { continue; }
    auto* pRatio = static_cast<TH1*>(fvpObjects[iV]->Clone());
    pRatio->SetDirectory(fpOutDir);
    auto currErrorLevel = gErrorIgnoreLevel;
    gErrorIgnoreLevel   = kError;
    auto* pCopy         = new TGraphAsymmErrors(pRatio, pDefault, "pois");
    gErrorIgnoreLevel   = currErrorLevel;
    pCopy->SetMarkerStyle(20);
    pMultiGraphRatio->Add(pCopy, "P");
    if (pRatio) {
      delete pRatio;
      pRatio = nullptr;
    }
  }
  pMultiGraphRatio->GetYaxis()->SetTitle("ratio");
  pMultiGraphRatio->GetXaxis()->SetTitle(xAxisTitle);
  pMultiGraphRatio->Draw("A pmc plc");

  // Histogram ratios to default
  pPadDiff->cd();
  auto* pMultiGraphDiff = new TMultiGraph((fsBaseName + "_diff").data(), titleDiff);
  for (int iV = 0; iV < nVersions; ++iV) {
    if (iV == iDef) { continue; }
    auto* pDiff = static_cast<TH1*>(fvpObjects[iV]->Clone());
    pDiff->SetDirectory(fpOutDir);
    pDiff->Add(pDefault, -1.);
    auto* pCopy = new TGraphAsymmErrors(pDiff);
    pCopy->SetMarkerStyle(20);
    pMultiGraphDiff->Add(pCopy, "P");
    if (pDiff) {
      delete pDiff;
      pDiff = nullptr;
    }
  }
  pMultiGraphDiff->GetYaxis()->SetTitle("difference");
  pMultiGraphDiff->GetXaxis()->SetTitle(xAxisTitle);
  pMultiGraphDiff->Draw("A pmc plc");
  fpCanvas->cd();
  pPadOrig->Draw();
  pPadRatio->Draw();
  pPadDiff->Draw();
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool Hist1DHandler::ComparePointToPoint(const TH1* pNumerator, const TH1* pDenominator)
{
  bool ifDifferent = false;
  // Compare content and error of each bin
  // TODO: write a comment about
  for (int iBinX = 0; iBinX <= pNumerator->GetNbinsX(); ++iBinX) {
    for (int iBinY = 0; iBinY <= pNumerator->GetNbinsY(); ++iBinY) {
      for (int iBinZ = 0; iBinZ <= pNumerator->GetNbinsZ(); ++iBinZ) {
        auto numBinContent = pNumerator->GetBinContent(iBinX, iBinY, iBinZ);
        auto denBinContent = pDenominator->GetBinContent(iBinX, iBinY, iBinZ);
        // Check bin content
        if (!TMath::IsNaN(numBinContent) && !TMath::IsNaN(denBinContent)) {
          if (numBinContent != denBinContent) { return true; }
        }
        else {
          if (TMath::IsNaN(numBinContent) != TMath::IsNaN(denBinContent)) { return true; }
        }
        auto numBinError = pNumerator->GetBinError(iBinX, iBinY, iBinZ);
        auto denBinError = pDenominator->GetBinError(iBinX, iBinY, iBinZ);
        // Check bin error
        if (!TMath::IsNaN(numBinError) && !TMath::IsNaN(denBinError)) {
          if (numBinError != denBinError) { return true; }
        }
        else {
          if (TMath::IsNaN(numBinError) != TMath::IsNaN(denBinError)) { return true; }
        }
      }
    }
  }
  return ifDifferent;
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool Hist1DHandler::CompareRatioDeviation(const TH1* /*pNumerator*/, const TH1* /*pDenominator*/, double /*allowedDev*/)
{
  LOG(warn) << "Hist1DHandler::CompareRatioDeviation function was not implemented";
  return false;
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool Hist1DHandler::CompareWithChi2(const TH1* /*pNumerator*/, const TH1* /*pDenominator*/)
{
  LOG(warn) << "Hist1DHandler::CompareWthChi2 function was not implemented";
  return false;
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool Hist1DHandler::CompareWithKolmogorov(const TH1* /*pNumerator*/, const TH1* /*pDenominator*/)
{
  LOG(warn) << "Hist1DHandler::CompareWithKolmogorov function was not implemented";
  return false;
}
