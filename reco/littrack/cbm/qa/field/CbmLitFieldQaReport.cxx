/* Copyright (C) 2011-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file CbmLitFieldQaReport.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmLitFieldQaReport.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmReportElement.h"
#include "CbmRichDraw.h"
#include "CbmUtils.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TH2D.h"

#include <boost/assign/list_of.hpp>

using boost::assign::list_of;
using Cbm::FindAndReplace;
using Cbm::Split;
using Cbm::ToString;
using namespace std;

CbmLitFieldQaReport::CbmLitFieldQaReport() { SetReportName("field_qa"); }

CbmLitFieldQaReport::~CbmLitFieldQaReport() {}

void CbmLitFieldQaReport::Create()
{
  Out() << R()->DocumentBegin() << std::endl;
  Out() << R()->Title(0, "Magnetic field QA") << std::endl;
  PrintCanvases();
  Out() << R()->DocumentEnd();
}

void CbmLitFieldQaReport::Draw()
{
  //gStyle->SetPalette(55, 0);
  SetDefaultDrawStyle();
  DrawFieldSlices();
  DrawFieldAlongZ();
  DrawFieldRichPmtPlane();
}

void CbmLitFieldQaReport::DrawFieldSlices()
{
  vector<TGraph2D*> graphsBx = HM()->G2Vector("hmf_Bx_Graph2D_.*");
  for (Int_t i = 0; i < graphsBx.size(); i++) {
    string name       = graphsBx[i]->GetName();
    string canvasName = GetReportName() + "_map_at_z_" + Split(name, '_')[3];
    TCanvas* canvas   = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 1000, 1000);
    canvas->Divide(2, 2);
    TGraph2D* graphBy  = HM()->G2(FindAndReplace(name, "_Bx_", "_By_"));
    TGraph2D* graphBz  = HM()->G2(FindAndReplace(name, "_Bx_", "_Bz_"));
    TGraph2D* graphMod = HM()->G2(FindAndReplace(name, "_Bx_", "_Mod_"));
    canvas->cd(1);
    DrawGraph2D(graphsBx[i]);
    canvas->cd(2);
    DrawGraph2D(graphBy);
    canvas->cd(3);
    DrawGraph2D(graphBz);
    canvas->cd(4);
    DrawGraph2D(graphMod);
  }
}

void CbmLitFieldQaReport::DrawFieldAlongZ()
{
  // Draw for different angles
  vector<TGraph*> graphsBx = HM()->G1Vector("hmf_BxAlongZAngle_Graph_.*");
  for (Int_t i = 0; i < graphsBx.size(); i++) {
    string name       = graphsBx[i]->GetName();
    string canvasName = GetReportName() + "_map_along_z_angle_" + Split(name, '_')[3];
    TCanvas* canvas   = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 1000, 1000);
    TGraph* graphBy   = HM()->G1(FindAndReplace(name, "_Bx", "_By"));
    TGraph* graphBz   = HM()->G1(FindAndReplace(name, "_Bx", "_Bz"));
    TGraph* graphMod  = HM()->G1(FindAndReplace(name, "_Bx", "_Mod"));
    DrawGraph({graphsBx[i], graphBy, graphBz, graphMod}, {"B_{x}", "B_{y}", "B_{z}", "|B|"}, kLinear, kLinear, true,
              0.7, 0.5, 0.9, 0.3);
    gPad->SetGrid(true, true);
  }

  // Draw for different XY positions
  graphsBx = HM()->G1Vector("hmf_BxAlongZXY_Graph_.*");
  for (Int_t i = 0; i < graphsBx.size(); i++) {
    string name           = graphsBx[i]->GetName();
    vector<string> splits = Split(name, '_');
    string canvasName     = GetReportName() + "_map_along_z_xy_" + splits[3] + "_" + splits[4];
    TCanvas* canvas       = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 1000, 1000);
    TGraph* graphBy       = HM()->G1(FindAndReplace(name, "_Bx", "_By"));
    TGraph* graphBz       = HM()->G1(FindAndReplace(name, "_Bx", "_Bz"));
    TGraph* graphMod      = HM()->G1(FindAndReplace(name, "_Bx", "_Mod"));
    DrawGraph({graphsBx[i], graphBy, graphBz, graphMod}, {"B_{x}", "B_{y}", "B_{z}", "|B|"}, kLinear, kLinear, true,
              0.7, 0.5, 0.9, 0.3);
    gPad->SetGrid(true, true);
  }

  // Draw integral for different XY positions
  graphsBx = HM()->G1Vector("hmf_BxAlongZXYIntegral_Graph_.*");
  for (Int_t i = 0; i < graphsBx.size(); i++) {
    string name           = graphsBx[i]->GetName();
    vector<string> splits = Split(name, '_');
    string canvasName     = GetReportName() + "_map_along_z_integral_xy_" + splits[3] + "_" + splits[4];
    TCanvas* canvas       = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 1000, 1000);
    TGraph* graphBy       = HM()->G1(FindAndReplace(name, "_Bx", "_By"));
    TGraph* graphBz       = HM()->G1(FindAndReplace(name, "_Bx", "_Bz"));
    TGraph* graphMod      = HM()->G1(FindAndReplace(name, "_Bx", "_Mod"));
    DrawGraph({graphsBx[i], graphBy, graphBz, graphMod}, {"B_{x}", "B_{y}", "B_{z}", "|B|"}, kLinear, kLinear, true,
              0.7, 0.5, 0.9, 0.3);
    gPad->SetGrid(true, true);
  }
}

void CbmLitFieldQaReport::DrawFieldRichPmtPlane()
{
  string names[] = {"Bx", "By", "Bz", "Mod"};

  for (Int_t iName = 0; iName < 4; iName++) {
    TGraph2D* grU = HM()->G2("hmf_" + names[iName] + "_Rich_Pmt_up");
    TGraph2D* grD = HM()->G2("hmf_" + names[iName] + "_Rich_Pmt_down");
    if (grU->GetN() == 0 || grD->GetN() == 0) continue;
    string canvasName = GetReportName() + "_rich_pmt_" + names[iName];
    TCanvas* canvas   = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 1000, 1000);
    CbmRichDraw::DrawPmtGraph2D(grU, grD, canvas);
  }

  string canvasName = GetReportName() + "_rich_pmt_projection_edge";
  TCanvas* canvas   = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 1000, 1000);
  TH2D* h2U         = HM()->G2("hmf_Mod_Rich_Pmt_up")->GetHistogram();
  TH2D* h2D         = HM()->G2("hmf_Mod_Rich_Pmt_down")->GetHistogram();
  Int_t nBins       = h2D->GetNbinsY();
  TH1D* hPrU        = h2U->ProjectionX("_px", 1, 1);
  TH1D* hPrD        = h2D->ProjectionX("_px", nBins, nBins);
  hPrU->GetYaxis()->SetTitle(h2U->GetZaxis()->GetTitle());
  hPrD->GetYaxis()->SetTitle(h2D->GetZaxis()->GetTitle());

  DrawH1({hPrU, hPrD}, {"Top plane", "Bottom plane"}, kLinear, kLinear, true, 0.7, 0.8, 0.99, 0.99, "hist");
}

ClassImp(CbmLitFieldQaReport)
