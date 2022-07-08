/* Copyright (C) 2015-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Eoin Clerkin, Florian Uhlig, Volker Friese [committer], David Emschermann */

/*
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FairRadLenPoint.h"
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
*/

// need to set this root include path:
// include path: -I/opt/cbm/fairsoft_jul15p1/installation/include/root -I/opt/cbm/fairroot_v-15.07-fairsoft_jul15p1/include -I/opt/cbm/fairsoft_jul15p1/installation/include
//.include $SIMPATH/include
//.include $FAIRROOTPATH/include

//Int_t matbudget_ana(Int_t nEvents = 10      , const char* stsGeo = "v16v")
//Int_t matbudget_ana(Int_t nEvents = 1000000 , const char* stsGeo = "v16v")
Int_t matbudget_ana(Int_t nEvents = 1000000, const char* stsGeo = "v21e")
{

  // Input file (MC)
  TString stsVersion(stsGeo);
  TString inFile = "data/matbudget." + stsVersion + ".mc.root";
  TFile* input   = new TFile(inFile);
  if (!input) {
    cout << "*** matbudget_ana: Input file " << inFile << " not found!\n"
         << "Be sure to run matbudget_mc.C before for the respective STS geometry!" << endl;
    exit(1);
  }

  // Output file (material maps)
  TString outFile = "sts_matbudget_" + stsVersion + ".root";

  // Input tree and branch
  TTree* tree          = (TTree*) input->Get("cbmsim");
  TClonesArray* points = new TClonesArray("FairRadLenPoint");
  tree->SetBranchAddress("RadLen", &points);

  // Create output histograms
  TH1D* hisRadLen     = new TH1D("hisRadLen", "Radiation Length", 1000, 0, 100);

  const int nStations =
    8;  // 8;     // number of STS stations. I'll be adding 2 often below to account for material before the first station and after the last station.

  const int nBins     = 1000;  // number of bins in histograms (in both x and y)
  const int rMax      = 50;    // maximal radius for histograms (for both x and y)
  const double zRange = 1.4;   // 1.4;

  TProfile2D* hStationRadLen[nStations + 3];
  TProfile2D* hStsRadLen;
  double StsRadThick = 0;

  TString stsname = "Material Budget x/X_{0} [%], STS";
  hStsRadLen      = new TProfile2D(stsname, stsname, nBins, -rMax, rMax, nBins, -rMax, rMax);

  int iStation = 0, iStationOut = 0;

  for (int i = 0; i < nStations + 3; ++i) {
    TString hisname = "Radiation Thickness [%],";
    hisname += " Station";
    hisname += i - 1;
    TString name = "Material Budget x/X_{0} [%],";
    name += " Station ";
    name += i - 1;
    hStationRadLen[i] = new TProfile2D(hisname, name, nBins, -rMax, rMax, nBins, -rMax, rMax);
  }

  // Auxiliary variables
  TVector3 vecs, veco;
  std::map<int, int> trackHitMap;

  // Event loop
  int firstEvent = 0;
  for (Int_t event = firstEvent; event < (firstEvent + nEvents) && event < tree->GetEntriesFast(); event++) {
    // std::cout << "EVENT STARTS: " << event << std::endl;

    tree->GetEntry(event);
    // std::cout << "# Entries : " << tree->GetEntry(event) << std::endl;

    if (0 == event % 10000) cout << "*** Processing event " << event << endl;
    const int nTracks = 1;
    // std::cout << "# Tracks : " << nTracks << std::endl;

    std::vector<double> RadLengthOnTrack(nTracks, 0.0);  //trackID, vector with points on track
    std::vector<double> TrackLength(nTracks, 0.0);       //trackID, vector with points on track

    // std::cout << "RadThick Reset : " << nStations+2 << std::endl;

    vector<double> RadThick(nStations + 3, 0);
    Double_t x = 0;
    Double_t y = 0;

    // For this implementation to be correct, there should be only one MCTrack per event.

    // Point loop
    for (Int_t iPoint = 0; iPoint < points->GetEntriesFast(); iPoint++) {
      // std::cout << "iPoint: " << iPoint << std::endl;
      FairRadLenPoint* point = (FairRadLenPoint*) points->At(iPoint);

      // Get track position at entry and exit of material
      TVector3 posIn, posOut, posDif;
      posIn  = point->GetPosition();
      posOut = point->GetPositionOut();
      posDif = posOut - posIn;

      // Midpoint between in and out
      const TVector3 sum = (posOut + posIn);
      x                  = sum.X() / 2;
      y                  = sum.Y() / 2;
      const double z     = sum.Z() / 2;

      // Material budget per unit length
      const double radThick = posDif.Mag() / point->GetRadLength();
      RadLengthOnTrack[point->GetTrackID()] += radThick;
      TrackLength[point->GetTrackID()] += posDif.Mag();

      // start of the STS, start of Station 1, start of Station 2, start of Station 3, ...., start of Station 8, end of Station 8, end of STS
      double boundaries[nStations + 3] = {-42.5, -20.5, -8.5, 2.0, 12.5, 23.0, 33.5, 44.0, 54.25, 64.75, 80.5};

      iStation = 0;
      while (posIn.Z() > boundaries[iStation] && iStation <= nStations + 1)
        iStation++;  // iStation 2 is the first station, iStation 1 is the material before the station
                     // inside the STS box, iStation 0 is before the STS box. Should be empty.

      iStationOut = iStation;
      while (posOut.Z() > boundaries[iStationOut] && iStationOut <= nStations + 1)
        iStationOut++;

      if (iStationOut == iStation) { RadThick[iStation] += radThick; }
      else {
        // Distributes part of the radThick to two stations across the boundary.
        RadThick[iStation] += radThick * (boundaries[iStation] - posIn.Z()) / posDif.Mag();
        int j = iStation + 1;
        // If the material transverses more than one boundary
        while (j < iStationOut) {
          RadThick[j] += radThick * (boundaries[j] - boundaries[j - 1]) / posDif.Mag();
          ++j;
        };
        RadThick[j] += radThick * (posOut.Z() - boundaries[j - 1]) / posDif.Mag();
      };
    };

    StsRadThick = 0;
    // Fill material budget map for each station
    for (int i = 0; i <= nStations + 2; i++) {
      hStationRadLen[i]->Fill(x, y, RadThick[i] * 100);
      StsRadThick += RadThick[i];
    }

    hStsRadLen->Fill(x, y, StsRadThick * 100);

    for (int k = 0; k < RadLengthOnTrack.size(); k++) {
      if (RadLengthOnTrack[k] > 0) hisRadLen->Fill(RadLengthOnTrack[k]);
    };

  }  // event loop

  std::cout << "EVENT LOOP ENDS " << std::endl;

  // Plotting the results
  //single  TCanvas* can1 = new TCanvas("c","c",800,800);
  TCanvas* can1 = new TCanvas("c", "c", 2000, 800);
  can1->Divide(nStations / 2 + 2, 2);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);

  // Open output file
  TFile* output = new TFile(outFile, "RECREATE");
  output->cd();

  for (int iStation = 0; iStation <= nStations + 2; iStation++) {
    can1->cd(iStation + 1);
    //single    int iStation = 7;
    hStationRadLen[iStation]->GetXaxis()->SetTitle("x [cm]");
    hStationRadLen[iStation]->GetYaxis()->SetTitle("y [cm]");
    hStationRadLen[iStation]->GetZaxis()->SetTitle("x/X_{0} [%]");
    //  hStationRadLen[iStation]->GetZaxis()->SetTitle("radiation thickness [%]");
    hStationRadLen[iStation]->SetAxisRange(0, zRange, "Z");
    hStationRadLen[iStation]->Draw("colz");
    hStationRadLen[iStation]->Write();
  }

  // Plot file
  TString plotFile = "sts_" + stsVersion + "_matbudget.png";
  can1->SaveAs(plotFile);

  //================================================================

  // Plotting the results
  TCanvas* can2 = new TCanvas("c", "c", 1600, 800);
  can2->Divide(2, 1);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);

  //  can2->cd();
  hStsRadLen->GetXaxis()->SetTitle("x [cm]");
  hStsRadLen->GetYaxis()->SetTitle("y [cm]");
  hStsRadLen->SetAxisRange(0, 10, "Z");
  hStsRadLen->Draw("colz");

  // Plot file
  plotFile = "sts_" + stsVersion + "_total_matbudget.png";
  can2->SaveAs(plotFile);

  //================================================================

  TString thisStation(0);
  //  can2->Clear();
  for (int iStation = 0; iStation < nStations + 3; iStation++) {
    hStationRadLen[iStation]->Draw("colz");
    // Plot file
    thisStation.Form("%d", iStation - 1);
    plotFile = "sts_" + stsVersion + "_station_" + thisStation + "_matbudget.png";
    can2->SaveAs(plotFile);
  }

  // Close files
  input->Close();
  output->Close();
  cout << "Material budget maps written to " << outFile << endl;
  return 0;
}
