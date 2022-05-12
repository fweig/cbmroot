/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina  [committer], David Emschermann */
// --------------------------------------------------------------------------

#if !defined(__CINT__) || defined(__MAKECINT__)
//#include "FairRadLenPoint.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include "TVector3.h"

#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
#endif

// need to set this root include path:
// include path: -I/opt/cbm/fairsoft_jul15p1/installation/include/root -I/opt/cbm/fairroot_v-15.07-fairsoft_jul15p1/include -I/opt/cbm/fairsoft_jul15p1/installation/include
// .include $SIMPATH/include
// .include $FAIRROOTPATH/include
// .x 'matbudget_ana_mcbm_sts.C++("sts_v18e")'

Int_t matbudget_ana_trd(const TString geoVersion = "", const TString inFile = "matbudget", Int_t nEvents = -1)
{

  // TRD stations Z and thickness

  const std::vector<double> trdZ = {441.6, 486.6, 531.6, 576.6};

  double trdZthick = 20;  // station material is collected at trdZ +/- trdZthick

  const int nBins = 300;  // number of bins in histograms (in both x and y)
  const int maxXY = 500;  // +/- X and Y size for histograms (must be the same for X and Y)

  const int nStations = trdZ.size();  // number of stations

  for (int i = 1; i < nStations; i++) {
    if (trdZ[i] - trdZthick <= trdZ[i - 1] + trdZthick) {
      cerr << "Error:: TRD stations are not ordered in Z !!! " << endl;
      exit(-1);
    }
  }

  // Input file (MC)
  TFile* input = new TFile(inFile + ".tra.root");
  if (!input) {  // this is not checked in compiled mode
    cout << "*** matbudget_ana: Input file " << inFile << " not found!\n"
         << "Be sure to run matbudget_mc.C before for the respective " + geoVersion + " geometry!" << endl;
    exit(-1);
  }

  // Output file (material maps)
  TString outFile = "trd_matbudget_" + geoVersion + ".root";

  // Input tree and branch
  TTree* tree          = (TTree*) input->Get("cbmsim");
  TClonesArray* points = new TClonesArray("FairRadLenPoint");
  tree->SetBranchAddress("RadLen", &points);

  // Create output histograms

  TProfile2D* hStaRadLen[nStations];
  for (int i = 0; i < nStations; ++i) {
    TString name = "Radiation Thickness [%], Station";
    name += i + 1;
    hStaRadLen[i] = new TProfile2D(name, name, nBins, -maxXY, maxXY, nBins, -maxXY, maxXY);
  }

  // Event loop
  if (nEvents < 0 || nEvents > tree->GetEntriesFast()) { nEvents = tree->GetEntriesFast(); }

  vector<double> RadThick(nStations, 0.);

  for (Int_t event = 0; event < nEvents; event++) {
    if (0 == event % 100000) { std::cout << "*** Processing event " << event << std::endl; }
    tree->GetEntry(event);

    RadThick.assign(nStations, 0.);

    // For this implementation to be correct, there should be only one MCTrack per event.

    // Point loop. all points must xave the same (x,y) coordinates

    double x    = 2 * maxXY;
    double y    = 2 * maxXY;
    int trackID = -1;

    for (Int_t iPoint = 0; iPoint < points->GetEntriesFast(); iPoint++) {
      FairRadLenPoint* point = (FairRadLenPoint*) points->At(iPoint);

      // Get track position at entry and exit of material
      TVector3 posIn, posOut, posDif;
      posIn  = point->GetPosition();
      posOut = point->GetPositionOut();
      posDif = 0.5 * (posOut - posIn);

      // Midpoint between in and out
      const TVector3 middle = 0.5 * (posOut + posIn);
      const double xp       = middle.X();
      const double yp       = middle.Y();
      const double zp       = middle.Z();
      const int idp         = point->GetTrackID();

      if (iPoint == 0) {
        trackID = idp;
        x       = xp;
        y       = yp;
      }

      if (fabs(xp - x) > 1.e8 || fabs(yp - y) > 1.e8) {
        cerr << "Error:  the track is not horisontal xy old: " << x << " " << y << " xy new: " << x << " " << y << endl;
        exit(-1);
      }
      if (idp != trackID) {
        cerr << "Error: more than one MC track in event" << endl;
        exit(-1);
      }

      // Material budget per unit length
      const double radThick = posDif.Mag() / point->GetRadLength();

      // Determine station number
      int iStationIn  = -1;
      int iStationOut = -1;

      double zIn  = posIn.Z();
      double zOut = posOut.Z();

      for (int i = 0; i < nStations; i++) {
        if (zIn > trdZ[i] - trdZthick) iStationIn = i;
        if (zOut > trdZ[i] - trdZthick) iStationOut = i;
      }
      if (zIn > trdZ[nStations - 1] + trdZthick) iStationIn = -1;
      if (zOut > trdZ[nStations - 1] + trdZthick) iStationOut = -1;

      if (iStationOut != iStationIn || iStationIn < 0) continue;
      assert(iStationIn >= nStations);

      RadThick[iStationIn] += radThick;
    }

    // Fill material budget map for each station in [%] of the radiation length
    for (int i = 0; i < nStations; ++i) {
      hStaRadLen[i]->Fill(x, y, RadThick[i] * 100);
    }
  }  // event loop


  // Plotting the results
  TCanvas* can1 = new TCanvas();
  //  can1->Divide(nStations/2,2);
  can1->Divide(2, nStations / 2);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);

  // Open output file
  TFile* output = new TFile(outFile, "RECREATE");
  output->cd();
  for (int iStation = 0; iStation < nStations; iStation++) {
    can1->cd(iStation + 1);
    hStaRadLen[iStation]->GetXaxis()->SetTitle("x [cm]");
    hStaRadLen[iStation]->GetYaxis()->SetTitle("y [cm]");
    //hStaRadLen[i]->GetZaxis()->SetTitle("radiation thickness [%]");
    hStaRadLen[iStation]->SetAxisRange(0, 5, "Z");
    hStaRadLen[iStation]->DrawCopy("colz");
    output->cd();
    hStaRadLen[iStation]->Write();
  }

  // Plot file
  TString plotFile = geoVersion + "_matbudget.png";
  can1->SaveAs(plotFile);

  // Close files
  input->Close();
  output->Close();
  cout << "Material budget maps written to " << outFile << endl;

  return 0;
}
