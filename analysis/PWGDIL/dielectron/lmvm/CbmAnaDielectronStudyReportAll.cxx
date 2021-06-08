/* Copyright (C) 2012-2019 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva, Semen Lebedev [committer] */

#include "CbmAnaDielectronStudyReportAll.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmLmvmHist.h"
#include "CbmReportElement.h"
#include "CbmUtils.h"

#include "TSystem.h"

#include <boost/assign/list_of.hpp>

#include <sstream>
using boost::assign::list_of;
using std::string;
using std::stringstream;
using std::vector;

CbmAnaDielectronStudyReportAll::CbmAnaDielectronStudyReportAll() : CbmStudyReport()
{
  SetReportName("lmvm_analysis_study_report");
  SetReportTitle("LMVM analysis study report");
}

CbmAnaDielectronStudyReportAll::~CbmAnaDielectronStudyReportAll() {}

void CbmAnaDielectronStudyReportAll::Create()
{
  Out().precision(3);
  Out() << R()->DocumentBegin();
  Out() << R()->Title(0, GetTitle());
  PrintCanvases();
  Out() << R()->DocumentEnd();
}

void CbmAnaDielectronStudyReportAll::Draw()
{
  SetDefaultDrawStyle();
  DrawBgMinv();
  DrawSBgMinv();
}

void CbmAnaDielectronStudyReportAll::DrawBgMinv()
{
  Int_t nofStudies = HM().size();
  CreateCanvas("lmvm_study_report_bg_minv_ptcut", "lmvm_study_report_bg_minv_ptcut", 600, 600);
  vector<TH1*> histos1(nofStudies);
  vector<string> legendNames;
  for (Int_t iStudy = 0; iStudy < nofStudies; iStudy++) {
    histos1[iStudy] = HM()[iStudy]->H1("fh_bg_minv_ptcut");
    histos1[iStudy]->Rebin(20);
    ;
    histos1[iStudy]->Scale(1. / 20.);
    histos1[iStudy]->GetXaxis()->SetRangeUser(0, 2.);
    legendNames.push_back(GetStudyNames()[iStudy]);
  }
  DrawH1(histos1, legendNames, kLinear, kLinear, true, 0.6, 0.75, 0.99, 0.99);
}

void CbmAnaDielectronStudyReportAll::DrawSBgMinv()
{
  Int_t nofStudies = HM().size();
  CreateCanvas("lmvm_study_report_sbg_vs_minv_ptcut", "lmvm_study_report_sbg_vs_minv_ptcut", 600, 600);
  vector<TH1*> histos1(nofStudies);
  vector<string> legendNames;
  for (Int_t iStudy = 0; iStudy < nofStudies; iStudy++) {
    histos1[iStudy] = HM()[iStudy]->H1("fh_sbg_vs_minv_ptcut");
    // histos1[iStudy]->Rebin(4);;
    // histos1[iStudy]->Scale(1./4.);
    histos1[iStudy]->GetXaxis()->SetRangeUser(0, 2.);
    legendNames.push_back(GetStudyNames()[iStudy]);
  }
  DrawH1(histos1, legendNames, kLinear, kLog, true, 0.6, 0.75, 0.99, 0.99);
}
