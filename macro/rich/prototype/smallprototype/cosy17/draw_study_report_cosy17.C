/* Copyright (C) 2017 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

using namespace std;
void draw_study_report()
{
  gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
  loadlibs();


  std::vector<std::string> fileNames, studyNames;
  std::string outputDir = "results_reports_quartzt/";

  fileNames.push_back("/Users/slebedev/Development/cbm/data/sim/rich/"
                      "richprototype/reco.d.quartz3mm.1.2gev.1.root");
  fileNames.push_back("/Users/slebedev/Development/cbm/data/sim/rich/"
                      "richprototype/reco.d.quartz3mm.1.2gev.2.root");

  studyNames.push_back("30 mm");
  studyNames.push_back("40 mm");

  if (outputDir != "") {
    gSystem->mkdir(outputDir.c_str(),
                   true);  // create directory if it does not exist
  }

  CbmRichSmallPrototypeStudyReport* reportQa = new CbmRichSmallPrototypeStudyReport();
  reportQa->Create(fileNames, studyNames, outputDir);
}
