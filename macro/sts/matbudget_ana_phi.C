#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TH1.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TProfile2D.h"

#include "FairRadLenPoint.h"

#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
#endif

// need to set this root include path:
// include path: -I/opt/cbm/fairsoft_jul15p1/installation/include/root -I/opt/cbm/fairroot_v-15.07-fairsoft_jul15p1/include -I/opt/cbm/fairsoft_jul15p1/installation/include
//.include $SIMPATH/include
//.include $FAIRROOTPATH/include

//Int_t matbudget_ana_phi(Int_t nEvents=100     , const char* stsGeo = "v15c")
//Int_t matbudget_ana_phi(Int_t nEvents=1000000 , const char* stsGeo = "v15c")
Int_t matbudget_ana_phi(Int_t nEvents=10000000, const char* stsGeo = "v15c")
{
	
  // Input file (MC)
  TString stsVersion(stsGeo);
  TString inFile = "data/matbudget." + stsVersion + ".mc.root";
  TFile* input = new TFile(inFile);
	if ( ! input ) {
		cout << "*** matbudget_ana: Input file " << inFile << " not found!\n"
		     << "Be sure to run matbudget_mc.C before for the respective STS geometry!" << endl;
		exit;
	}

	// Output file (material maps)
	TString outFile = "sts_matbudget_" + stsVersion + ".root";

	// Input tree and branch
  TTree* tree = (TTree*) input->Get("cbmsim") ;
  TClonesArray* points = new TClonesArray("FairRadLenPoint");
  tree->SetBranchAddress("RadLen", &points);

  // Create output histograms
  TH1D* hisRadLen = new TH1D("hisRadLen","Radiation Length", 1000,0,100);
  const int nStations = 8;       // number of STS stations
  const int nBins     = 1000;    // number of bins in histograms (in both x and y)
  //  const int rMax      = 55;     // maximal radius for histograms (for both x and y)
  const int phiMax    = 180;      // maximal phi range for histograms
  const int thetaMax  = 50;       // maximal theta range for histograms
  //  const int radiusMax = 75;       // maximal radius for histograms
  double phiSta[nStations];
  double thetaSta[nStations];


  TProfile2D* hStaRadLen[nStations];
  for ( int i = 0; i < nStations; ++i ) {
    TString name = "Material Budget x/X_{0} [%],";
    name += " Station ";
    name += i+1;
    //    hStaRadLen[i] = new TProfile2D(name, name, nBins,-rMax, rMax, nBins,-rMax, rMax);
    hStaRadLen[i] = new TProfile2D(name, name, nBins, -phiMax, phiMax, nBins, 0, thetaMax);
    //    hStaRadLen[i] = new TProfile2D(name, name, nBins, -phiMax, phiMax, nBins, 0, radiusMax);
  }
  
  // Auxiliary variables
  TVector3 vecs,veco;
  std::map<int,int> trackHitMap;


  // Event loop
  int firstEvent = 0;
  for (Int_t event = firstEvent;
  		 event < (firstEvent + nEvents) && event<tree->GetEntriesFast();
  		 event++) {
    tree->GetEntry(event);
    if ( 0 == event%10000 )  cout << "*** Processing event "<< event << endl;

    const int nTracks = 1;
    std::vector<double> RadLengthOnTrack (nTracks,0.0); //trackID, vector with points on track
    std::vector<double> TrackLength (nTracks,0.0);      //trackID, vector with points on track

    vector<double> RadThick(nStations,0);
    double x,y;
    double theta, phi, radius;

    // For this implementation to be correct, there should be only one MCTrack per event.

    // Point loop
    for (Int_t iPoint = 0; iPoint < points->GetEntriesFast(); iPoint++) {
      FairRadLenPoint *point=(FairRadLenPoint*) points->At(iPoint);

      // Get track position at entry and exit of material
      TVector3 posIn, posOut, posDif;
      posIn  = point->GetPosition();
      posOut = point->GetPositionOut();
      posDif = 0.5* (posOut - posIn);

      // Midpoint between in and out
      const TVector3 middle = (posOut + posIn);
      x = middle.X()/2;
      y = middle.Y()/2;
      const double z = middle.Z()/2;

      if (x == 0)
      {
        if (y >= 0)
          phi = 90;
        else
          phi = -90;
      }
      else
        phi = atan(y/x) * 180 / acos(-1.);

      if (x < 0)
      {
        if (y >= 0)
          phi += 180;
        else
          phi -= 180;
      }
 
      radius = sqrt(x*x + y*y);

      if (z > 0)
        theta = atan( radius / z ) * 180 / acos(-1.);
      //        theta = atan( sqrt(x*x + y*y) / z ) * 180 / acos(-1.);

      //      cout << "x: " << x << " y: " << y << " r: " << radius << " z: " << z << " - phi: " << phi << " theta: " << theta;
      
      // Material budget per unit length
      const double radThick = posDif.Mag() / point->GetRadLength();
      RadLengthOnTrack[point->GetTrackID()] += radThick;
      TrackLength[point->GetTrackID()] += posDif.Mag();

      // Determine station number
      int iStation    =  posIn.Z()/10  - 3 + 0.5; // suppose equidistant stations at 30-100 cm
      int iStationOut =  posOut.Z()/10 - 3 + 0.5;
      if ( iStationOut != iStation) continue;
      if ( iStation >= nStations || iStation < 0 ) continue;

      RadThick[iStation] += radThick;
      phiSta[iStation]   = phi;
      //      thetaSta[iStation] = theta;
      thetaSta[iStation] = atan (radius / (iStation *10 + 30)) * 180 / acos(-1.);

//      cout << "i: " << iStation << " phi: " << phi << " z: " << z << " theta: " << theta
//           << " correct: " << atan (radius / (iStation *10 + 30)) * 180 / acos(-1.) 
//	   << " RadThick: " << RadThick[iStation]*100 << endl;
    }
    
    // Fill material budget map for each station
    for ( int i = 0; i < nStations; ++i )
    {
      //      hStaRadLen[i]->Fill( x, y, RadThick[i]*100 );
      //      hStaRadLen[i]->Fill( phi, theta, RadThick[i]*100 );
      hStaRadLen[i]->Fill( phiSta[i], thetaSta[i], RadThick[i]*100 );
      //      hStaRadLen[i]->Fill( phi, radius, RadThick[i]*100 );

      //      cout << "AA phi: " << phi << " theta: " << theta << endl;
    }
    
    for (int k = 0; k < RadLengthOnTrack.size(); k++)
      if (RadLengthOnTrack[k] > 0)
      	hisRadLen->Fill(RadLengthOnTrack[k]);

  } // event loop



  // Plotting the results
//single  TCanvas* can1 = new TCanvas("c","c",800,800);
  TCanvas* can1 = new TCanvas("c","c",1600,800);
  can1->Divide(nStations/2,2);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);

  // Open output file
  TFile* output = new TFile(outFile, "RECREATE");
  output->cd();
  
  for ( int iStation = 0; iStation < nStations; iStation++) {
    can1->cd(iStation+1);
//single    int iStation = 7;
//    hStaRadLen[iStation]->GetXaxis()->SetTitle("x [cm]");
//    hStaRadLen[iStation]->GetYaxis()->SetTitle("y [cm]");
    hStaRadLen[iStation]->GetXaxis()->SetTitle("phi [deg]");
    hStaRadLen[iStation]->GetYaxis()->SetTitle("theta [deg]");
    //    hStaRadLen[iStation]->GetYaxis()->SetTitle("radius [cm]");
    //hStaRadLen[iStation]->GetZaxis()->SetTitle("x/X_{0} [%]");
    //hStaRadLen[i]->GetZaxis()->SetTitle("radiation thickness [%]");
    hStaRadLen[iStation]->SetAxisRange(0, 1.2, "Z");
    //    hStaRadLen[iStation]->SetAxisRange(0, 2, "Z");
    hStaRadLen[iStation]->Draw("colz");
    hStaRadLen[iStation]->Write();
  }

  // Plot file
  TString plotFile = "sts_" + stsVersion + "_matbudget.png";
  can1->SaveAs(plotFile);

  // Close files
  input->Close();
  output->Close();
  cout << "Material budget maps written to " << outFile << endl;

  return 0;
}

