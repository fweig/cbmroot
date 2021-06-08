/* Copyright (C) 2011-2021 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Andrey Lebedev [committer], Florian Uhlig */

/**
 * \file CbmStudyReport.cxx
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmStudyReport.h"

#include "CbmHistManager.h"  // for CbmHistManager

#include <TDirectory.h>  // for TDirectory, gDirectory
#include <TFile.h>       // for TFile, gFile

#include <cassert>  // for assert
#include <fstream>  // for string, ofstream, stringstream
#include <string>   // for basic_string
#include <vector>   // for vector

using std::ofstream;
using std::string;
using std::stringstream;
using std::vector;

CbmStudyReport::CbmStudyReport() : CbmReport(), fHM(), fStudyNames() {}

CbmStudyReport::~CbmStudyReport() {}

void CbmStudyReport::Create(const vector<CbmHistManager*>& histManagers, const vector<string>& studyNames,
                            const string& outputDir)
{
  assert(histManagers.size() == studyNames.size());
  fHM         = histManagers;
  fStudyNames = studyNames;
  SetOutputDir(outputDir);
  CreateReports();
}

void CbmStudyReport::Create(const vector<string>& fileNames, const vector<string>& studyNames, const string& outputDir)
{
  assert(fileNames.size() == studyNames.size());
  Int_t nofStudies = fileNames.size();
  vector<TFile*> files(nofStudies);
  fHM.resize(nofStudies);

  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  for (Int_t i = 0; i < nofStudies; i++) {
    fHM[i]   = new CbmHistManager();
    files[i] = new TFile(fileNames[i].c_str());
    fHM[i]->ReadFromFile(files[i]);
  }
  fStudyNames = studyNames;
  SetOutputDir(outputDir);

  CreateReports();

  // Do not delete histogram managers and files.
  // Otherwise histograms disappear from canvases
  // and are not saved to file.
  //	for (Int_t i = 0; i < nofStudies; i++) {
  //		delete fHM[i];
  //		delete files[i];
  //	}
  //	fHM.clear();
  //	files.clear();

  gFile      = oldFile;
  gDirectory = oldDir;
}

ClassImp(CbmStudyReport)
