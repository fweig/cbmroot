/* Copyright (C) 2011-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer] */

#include "CbmAnaLmvmDrawStudy.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmUtils.h"

#include "TCanvas.h"
#include "TClass.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TKey.h"
#include "TMath.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TText.h"
#include <TLegend.h>

#include <boost/assign/list_of.hpp>

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;
using namespace Cbm;
using boost::assign::list_of;


void CbmAnaLmvmDrawStudy::DrawFromFile(const vector<string>& fileNames, const vector<string>& fileNamesMean,
                                       const vector<string>& studyNames, const string& outputDir)
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fOutputDir = outputDir;

  SetDefaultDrawStyle();

  fNofStudies = fileNames.size();
  fHM.resize(fNofStudies);
  for (int i = 0; i < fNofStudies; i++) {
    fHM[i]      = new CbmHistManager();
    TFile* file = new TFile(fileNames[i].c_str());
    LOG_IF(fatal, !file) << "Could not open file " << fileNames[i].c_str();
    fHM[i]->ReadFromFile(file);
  }

  // files with mean histograms
  //fMeanFiles.resize(fileNamesMean.size());
  //for (int i = 0; i < fileNamesMean.size(); i++){
  //fMeanFiles[i] = new TFile(fileNamesMean[i].c_str(), "READ");
  //}
  fMeanFiles  = fileNamesMean;
  fStudyNames = studyNames;

  DrawMinv();

  SaveCanvasToImage();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmAnaLmvmDrawStudy::SaveCanvasToImage()
{
  for (unsigned int i = 0; i < fCanvas.size(); i++) {
    Cbm::SaveCanvasAsImage(fCanvas[i], fOutputDir);
  }
}

void CbmAnaLmvmDrawStudy::DrawMinv()
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  vector<TH1*> hPtCut, hTtCut;
  hPtCut.resize(fMeanFiles.size());
  hTtCut.resize(fMeanFiles.size());
  int nRebin = 20;
  for (unsigned int i = 0; i < fMeanFiles.size(); i++) {
    TFile* f = new TFile(fMeanFiles[i].c_str(), "READ");
    LOG_IF(fatal, !f) << "Could not open file " << fMeanFiles[i].c_str();

    hPtCut[i] = f->Get<TH1D>("fh_bg_minv_ptcut")->Clone();
    LOG_IF(fatal, !hPtCut[i]) << "Could not get histogram " << hPtCut[i]->GetName() << "from file "
                              << fMeanFiles[i].c_str();
    hPtCut[i]->Rebin(nRebin);
    hPtCut[i]->SetMinimum(1e-6);

    hTtCut[i] = f->Get<TH1D>("fh_bg_minv_ttcut")->Clone();
    LOG_IF(fatal, !hTtCut[i]) << "Could not get histogram " << hTtCut[i]->GetName() << "from file "
                              << fMeanFiles[i].c_str();
    hTtCut[i]->Rebin(nRebin);
    hPtCut[i]->SetMinimum(1e-6);
    //f->Close();
  }
  fHM[0]->CreateCanvas("lmvm_study_minv_bg_ttcut", "lmvm_study_minv_bg_ttcut", 600, 600);
  DrawH1(hTtCut, fStudyNames, kLinear, kLog, true, 0.70, 0.55, 0.99, 0.99, "");

  fHM[0]->CreateCanvas("lmvm_study_minv_bg_ptcut", "lmvm_study_minv_bg_ptcut", 600, 600);
  DrawH1(hPtCut, fStudyNames, kLinear, kLog, true, 0.70, 0.55, 0.99, 0.99, "");


  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}


ClassImp(CbmAnaLmvmDrawStudy);
