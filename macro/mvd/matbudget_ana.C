/** @file matbudget_ana.C
  * @copyright Copyright (C) 2020 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Philipp Klaus [committer] **/

// Philipp Klaus, 30.04.2020
//
// derived from version created for STS

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

#include "TCanvas.h"
#include "TGaxis.h"
#include "TH2.h"
#include "TRandom.h"

int station_num(float pos, float pitch, float first_z, int n_stations)
{
  float range_from = first_z - pitch / 2;
  float range_to   = range_from + n_stations * pitch;
  if (pos < range_from || pos > range_to) return -1;
  else
    return (pos - range_from) / (range_to - range_from) * n_stations;
}

// Int_t matbudget_ana(Int_t nEvents = 10      , const char* mvdGeo = "v17a_tr")
// Int_t matbudget_ana(Int_t nEvents = 10000 , const char* mvdGeo = "v17a_tr")
Int_t matbudget_ana(Int_t nEvents = 10000000, const char* mvdGeo = "v17a_tr")
{

  // Input file (MC)
  TString mvdVersion(mvdGeo);
  TString inFile = "data/matbudget." + mvdVersion + ".mc.root";
  TFile* input   = new TFile(inFile);
  if (!input) {
    cout << "*** matbudget_ana: Input file " << inFile << " not found!\n"
         << "Be sure to run matbudget_mc.C before for the respective MVD "
            "geometry!"
         << endl;
    exit(1);
  }

  // Output file (material maps)
  TString outFile = "mvd_matbudget_" + mvdVersion + ".root";

  // Input tree and branch
  TTree* tree          = (TTree*) input->Get("cbmsim");
  TClonesArray* points = new TClonesArray("FairRadLenPoint");
  tree->SetBranchAddress("RadLen", &points);

  // Create output histograms
  TH1D* hisRadLen           = new TH1D("hisRadLen", "Radiation Length", 1000, 0, 100);
  const int nStations       = 4;     // number of MVD stations
  const int nBins           = 1000;  // number of bins in histograms (in both x and y)
  const int rMax            = 21;    // maximal radius for histograms (for both x and y)
  const float stationPitch  = 5.;
  const float firstStationZ = 1.;
  // % x/X0 the individual plots should have as limit:
  // const double zRange = 22.5; // full-scale (including aluminium heatsinks)
  const double zRange = 0.7;  // fixed-scale to pin range to values found in acceptance
  TProfile2D* hStationRadLen[nStations];
  TProfile2D* hMvdRadLen;
  double MvdRadThick = 0;

  TString mvdname = "Material Budget x/X_{0} [%], MVD";
  hMvdRadLen      = new TProfile2D(mvdname, mvdname, nBins, -rMax, rMax, nBins, -rMax, rMax);

  for (int i = 0; i < nStations; ++i) {
    TString hisname = "Radiation Thickness [%],";
    hisname += " Station";
    hisname += i;
    TString name = "Material Budget x/X_{0} [%],";
    name += " Station ";
    name += i;
    hStationRadLen[i] = new TProfile2D(hisname, name, nBins, -rMax, rMax, nBins, -rMax, rMax);
  }

  // Auxiliary variables
  TVector3 vecs, veco;
  std::map<int, int> trackHitMap;

  // Event loop
  int firstEvent = 0;
  for (Int_t event = firstEvent; event < (firstEvent + nEvents) && event < tree->GetEntriesFast(); event++) {
    tree->GetEntry(event);
    if (0 == event % 10000) cout << "*** Processing event " << event << endl;

    const int nTracks = 1;
    std::vector<double> RadLengthOnTrack(nTracks, 0.0);  // trackID, vector with points on track
    std::vector<double> TrackLength(nTracks, 0.0);       // trackID, vector with points on track

    double OverallRadThick = 0.0;
    vector<double> RadThick(nStations, 0);
    Double_t x = 0;
    Double_t y = 0;

    // For this implementation to be correct, there should be only one MCTrack
    // per event.

    // Point loop
    for (Int_t iPoint = 0; iPoint < points->GetEntriesFast(); iPoint++) {
      FairRadLenPoint* point = (FairRadLenPoint*) points->At(iPoint);

      if (point->GetDetectorID() != 1)  // 1 == MVD
        continue;

      // Get track position at entry and exit of material
      TVector3 posIn, posOut, posDif;
      posIn  = point->GetPosition();
      posOut = point->GetPositionOut();
      posDif = posOut - posIn;

      // Midpoint between in and out
      const TVector3 middle = (posOut + posIn);
      x                     = middle.X() / 2;
      y                     = middle.Y() / 2;
      const double z        = middle.Z() / 2;

      // Material budget per unit length
      const double radThick = posDif.Mag() / point->GetRadLength();
      RadLengthOnTrack[point->GetTrackID()] += radThick;
      TrackLength[point->GetTrackID()] += posDif.Mag();

      // cout << "detectorID: " << point->GetDetectorID() << "  Zin="
      //      << posIn.Z() << "  Zout=" << posOut.Z() << endl;
      // cout << "  length (cm): " << posDif.Mag()
      //      << "  radlength (cm): " << point->GetRadLength()
      //      << "  radThick (%):" << radThick * 100 << endl;
      // if (event == 20) return -1;

      // Determine station number
      int iStation    = station_num(posIn.Z(), stationPitch, firstStationZ, nStations);
      int iStationOut = station_num(posOut.Z(), stationPitch, firstStationZ, nStations);

      // cout << "station in: " << iStation << "station out: " << iStationOut <<
      // endl;

      if (iStationOut != iStation || (iStation == -1 && iStationOut == -1)) {
        // cannot be assigned to a specific station
        OverallRadThick += radThick;
        continue;
      }
      // If still around, iStation == iStationOut, <- a proper station ID
      RadThick[iStation] += radThick;
    }

    MvdRadThick = 0;
    // Fill material budget map for each station
    for (int i = 0; i < nStations; ++i) {
      if (RadThick[i] == 0.0)
        // this avoids shadows of other stations in the plots
        continue;
      hStationRadLen[i]->Fill(x, y, RadThick[i] * 100);
      MvdRadThick += RadThick[i];
    }
    MvdRadThick += OverallRadThick;

    hMvdRadLen->Fill(x, y, MvdRadThick * 100);

    for (int k = 0; k < RadLengthOnTrack.size(); k++)
      if (RadLengthOnTrack[k] > 0) hisRadLen->Fill(RadLengthOnTrack[k]);

  }  // event loop

  // Plotting the results
  // single  TCanvas* can1 = new TCanvas("c","c",800,800);
  TCanvas* can1 = new TCanvas("c", "c", 1600, 800);
  can1->Divide(nStations / 2, 2);
  gStyle->SetPalette(55);
  gStyle->SetOptStat(0);

  // Open output file
  TFile* output = new TFile(outFile, "RECREATE");
  output->cd();

  for (int iStation = 0; iStation < nStations; iStation++) {
    can1->cd(iStation + 1);
    // single    int iStation = 7;
    hStationRadLen[iStation]->GetXaxis()->SetTitle("x [cm]");
    hStationRadLen[iStation]->GetYaxis()->SetTitle("y [cm]");
    // hStationRadLen[iStation]->GetZaxis()->SetTitle("x/X_{0} [%]");
    // hStationRadLen[i]->GetZaxis()->SetTitle("radiation thickness [%]");
    hStationRadLen[iStation]->SetAxisRange(0, zRange, "Z");
    gStyle->SetPalette(55);
    hStationRadLen[iStation]->Draw("colz");
    hStationRadLen[iStation]->Write();
  }

  // Plot file
  TString plotFile = "mvd_" + mvdVersion + "_matbudget.pdf";
  can1->SaveAs(plotFile);
  plotFile = "mvd_" + mvdVersion + "_matbudget.jpg";
  can1->SaveAs(plotFile);

  //================================================================

  // Plotting the results
  TCanvas* can2 = new TCanvas("c", "c", 800, 800);
  gStyle->SetPalette(55);
  gStyle->SetOptStat(0);

  can2->cd();
  hMvdRadLen->GetXaxis()->SetTitle("x [cm]");
  hMvdRadLen->GetYaxis()->SetTitle("y [cm]");
  hMvdRadLen->SetAxisRange(0, zRange * nStations, "Z");
  gStyle->SetPalette(55);
  hMvdRadLen->Draw("colz");

  // Plot file
  plotFile = "mvd_" + mvdVersion + "_total_matbudget.pdf";
  can2->SaveAs(plotFile);
  plotFile = "mvd_" + mvdVersion + "_total_matbudget.jpg";
  can2->SaveAs(plotFile);

  //================================================================

  TString thisStation(0);
  can2->Clear();
  for (int iStation = 0; iStation < nStations; iStation++) {
    hStationRadLen[iStation]->Draw("colz");
    // Plot file
    thisStation.Form("%d", iStation);
    plotFile = "mvd_" + mvdVersion + "_station_" + thisStation + "_matbudget.pdf";
    can2->SaveAs(plotFile);
    plotFile = "mvd_" + mvdVersion + "_station_" + thisStation + "_matbudget.jpg";
    can2->SaveAs(plotFile);
  }

  // Close files
  input->Close();
  output->Close();
  cout << "Material budget maps written to " << outFile << endl;

  return 0;
}
