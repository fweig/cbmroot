/* Copyright (C) 2013-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Semen Lebedev */

/**
 * \file CbmLitRadLengthQaReport.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 */
#include "CbmLitRadLengthQaReport.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmReportElement.h"

#include "TProfile2D.h"

CbmLitRadLengthQaReport::CbmLitRadLengthQaReport() : CbmSimulationReport() { SetReportName("radlength_qa"); }

CbmLitRadLengthQaReport::~CbmLitRadLengthQaReport() {}

void CbmLitRadLengthQaReport::Create()
{
  Out().precision(3);
  Out() << R()->DocumentBegin();
  Out() << R()->Title(0, GetTitle());

  PrintCanvases();

  Out() << R()->DocumentEnd();
}

void CbmLitRadLengthQaReport::Draw()
{
  SetDefaultDrawStyle();

  DrawDetector("Total");
  DrawDetector("Mvd");
  DrawDetector("Sts");
  DrawDetector("Rich");
  DrawDetector("Trd");
  DrawDetector("Much");
  DrawDetector("Tof");

  DrawDetectorStation("Mvd");
  DrawDetectorStation("Sts");
  DrawDetectorStation("Trd");
  DrawDetectorStation("Much");
  DrawDetectorStation("MuchAbsorber");
}

void CbmLitRadLengthQaReport::DrawDetector(const string& detName)
{
  if (HM()->Exists("hrl_Thickness_" + detName + "_H1")) {
    CreateCanvas(string("hrl_Thickness_" + detName + "_H1").c_str(), string("hrl_Thickness_" + detName + "_H1").c_str(),
                 1200, 1000);
    DrawH1(HM()->H1("hrl_Thickness_" + detName + "_H1"));
  }

  if (HM()->Exists("hrl_Thickness_" + detName + "_P2")) {
    CreateCanvas(string("hrl_Thickness_" + detName + "_P2").c_str(), string("hrl_Thickness_" + detName + "_P2").c_str(),
                 1200, 1000);
    DrawH2(HM()->P2("hrl_Thickness_" + detName + "_P2"));
  }

  if (HM()->Exists("hrl_RadThickness_" + detName + "_H1")) {
    CreateCanvas(string("hrl_RadThickness_" + detName + "_H1").c_str(),
                 string("hrl_RadThickness_" + detName + "_H1").c_str(), 1200, 1000);
    DrawH1(HM()->H1("hrl_RadThickness_" + detName + "_H1"));
  }

  if (HM()->Exists("hrl_RadThickness_" + detName + "_P2")) {
    CreateCanvas(string("hrl_RadThickness_" + detName + "_P2").c_str(),
                 string("hrl_RadThickness_" + detName + "_P2").c_str(), 1200, 1000);
    DrawH2(HM()->P2("hrl_RadThickness_" + detName + "_P2"));


    CreateCanvas(string("hrl_RadThickness_" + detName + "_P2_zoom").c_str(),
                 string("hrl_RadThickness_" + detName + "_P2_zoom").c_str(), 1200, 1000);
    TProfile2D* pr = (TProfile2D*) HM()->P2("hrl_RadThickness_" + detName + "_P2")->Clone();
    pr->GetZaxis()->SetRangeUser(0, 100);
    DrawH2(pr);
  }

  if (HM()->Exists("hrl_ThicknessSilicon_" + detName + "_H1")) {
    CreateCanvas(string("hrl_ThicknessSilicon_" + detName + "_H1").c_str(),
                 string("hrl_ThicknessSilicon_" + detName + "_H1").c_str(), 1200, 1000);
    DrawH1(HM()->H1("hrl_ThicknessSilicon_" + detName + "_H1"));
  }

  if (HM()->Exists("hrl_ThicknessSilicon_" + detName + "_P2")) {
    CreateCanvas(string("hrl_ThicknessSilicon_" + detName + "_P2").c_str(),
                 string("hrl_ThicknessSilicon_" + detName + "_P2").c_str(), 1200, 1000);
    DrawH2(HM()->P2("hrl_ThicknessSilicon_" + detName + "_P2"));
  }
}

void CbmLitRadLengthQaReport::DrawDetectorStation(const string& detName)
{
  DrawH1ByPattern("hrl_RadThickness_" + detName + "_.+_H1");
  DrawH2ByPattern("hrl_RadThickness_" + detName + "_.+_P2");
  DrawH1ByPattern("hrl_Thickness_" + detName + "_.+_H1");
  DrawH2ByPattern("hrl_Thickness_" + detName + "_.+_P2");
  DrawH1ByPattern("hrl_ThicknessSilicon_" + detName + "_.+_H1");
  DrawH2ByPattern("hrl_ThicknessSilicon_" + detName + "_.+_P2");
}

ClassImp(CbmLitRadLengthQaReport)
