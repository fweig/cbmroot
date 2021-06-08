/* Copyright (C) 2018 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Egor Ovcharenko [committer] */

void drawHistos(TString inFilename = "~/TSA_files/result/test60001.root")
{
  TFile* inFile = new TFile(inFilename, "READ");
  if (inFile->IsZombie()) {
    cerr << "Could not open input file '" << inFilename << "'. Aborting." << endl;
    return;
  }

  TString histoName = "fhToT";
  TH2* histoToT     = (TH2*) (inFile->Get(histoName));
  if (histoToT == NULL) {
    cerr << "Could not extract histogram '" << histoName << "' from file '" << inFilename << "'. Aborting." << endl;
    return;
  }

  TBrowser* bro = new TBrowser("bro", "bro", 1800, 1000);
  gStyle->SetOptStat(111111);

  histoToT->Draw("COLZ");
}
