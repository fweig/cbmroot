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

#include "/cvmfs/fairroot.gsi.de/fairroot/v-15.03a_fairsoft-mar15p2/include/FairRadLenPoint.h"

#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
#endif

Int_t ana_radlength_mvd()
{

  Int_t nEvents=10000000;
  TString inFile = "data/radlength.mvd_v18a.mc.root";
  TFile* f = new TFile(inFile);
  TTree *t=(TTree *) f->Get("cbmsim") ;

  TString dir = gSystem->Getenv("VMCWORKDIR");
  TString file = dir + "/gconfig/basiclibs.C";
  gROOT->LoadMacro(file);
  gROOT->ProcessLine("basiclibs()");
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libCbmBase");


  TClonesArray* radlen_array = new TClonesArray("FairRadLenPoint");
  t->SetBranchAddress("RadLen", &radlen_array);

  TH1D* hisRadLen = new TH1D("hisRadLen","Radiation Length", 1000,0,100);

  const int NStations = 4;
  const int NBins = 1500;
  int RMax = 11;
  TProfile2D* hStaRadLen[NStations];

  for ( int i = 0; i < NStations; ++i ) {
    TString name = "Radiation Thickness [%],";
    name += " Station";
    name += i;
    if ( i > 1) RMax = 15;
    hStaRadLen[i] = new TProfile2D(name, name, NBins,-RMax,RMax, NBins,-RMax,RMax);
  }
  
  int startEvent = 0;

  TVector3 vecs,veco;
  std::map<int,int> trackHitMap;

  for (Int_t j=startEvent; j<(nEvents+startEvent) && j<t->GetEntriesFast(); j++) {
    t->GetEntry(j);
   if ( 0 == j%10000 ) { 
      cout<<">>>> Event No "<<j<<endl;
    }


    const int NTracks = 1;
    std::vector<double> RadLengthOnTrack (NTracks,0.0); //trackID, vector with points on track
    std::vector<double> TrackLength (NTracks,0.0); //trackID, vector with points on track

    vector<double> RadThick(NStations,0);
    double x,y;
    for (Int_t i=0; i<radlen_array->GetEntriesFast(); i++) {
//      cout<<"Point No "<<i<<endl;

      FairRadLenPoint *point=(FairRadLenPoint*)radlen_array->At(i);

//      cout << "Track ID: " << point->GetTrackID() << std::endl;

      TVector3 pos, posOut, res;
      pos = point->GetPosition();
      posOut = point->GetPositionOut();
      res = posOut - pos;

      const TVector3 middle2 = (posOut + pos);
      x = middle2.X()/2;
      y = middle2.Y()/2;
      const double z = middle2.Z()/2;
      
      const double radThick = res.Mag()/point->GetRadLength();
      RadLengthOnTrack[point->GetTrackID()] += radThick;
      TrackLength[point->GetTrackID()] += res.Mag();

      // cout << radThick << endl;
      // cout << pos.X() << " " << pos.Y() << " " << pos.Z() << " " << endl;
      // cout << posOut.X() << " " << posOut.Y() << " " << posOut.Z() << " " << endl;

      
      int iSta =  pos.Z()/5 - 1 + 0.5;
      int iStaOut = posOut.Z()/5 - 1 + 0.5;
   //   int iSta =  pos.Z()/10 - 3 + 0.5; // suppose equdistant stations at 30-100 cm
   //   int iStaOut = posOut.Z()/10 - 3 + 0.5;
      if ( iSta != iStaOut ) continue;
      if ( iSta >= NStations || iSta < 0 ) continue;
      RadThick[iSta] += radThick;

    }
    
    for ( int i = 0; i < NStations; ++i ) {
      // cout << i << " " << x << " " << y << " " << RadThick[i] << endl;
      hStaRadLen[i]->Fill( x, y, RadThick[i]*100 );
    }
    
    for (int k = 0; k < RadLengthOnTrack.size(); k++) {
      if (RadLengthOnTrack[k] > 0) {
//         std::cout << "Full TrackLength: " << TrackLength[k] << std::endl;
//         std::cout << "Full RadiationLength: " << RadLengthOnTrack[k] << std::endl;
	 hisRadLen->Fill(RadLengthOnTrack[k]); //%
      }
    }
  }

  TCanvas* can1 = new TCanvas();
  can1->Divide(NStations/2,2);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);

  TFile *rtFile = new TFile("mvd_matbudget_v18a.root","RECREATE");
  rtFile->cd();
  
  for ( int i = 0; i < NStations; ++i ) {
    can1->cd(i+1);
    hStaRadLen[i]->GetXaxis()->SetTitle("x [cm]");
    hStaRadLen[i]->GetYaxis()->SetTitle("y [cm]");
    //hStaRadLen[i]->GetZaxis()->SetTitle("radiation thickness [%]");
    hStaRadLen[i]->SetAxisRange(0, 2, "Z");
    hStaRadLen[i]->Draw("colz");
    hStaRadLen[i]->Write();
  }
  can1->SaveAs("mvd_matbudget_v18a.pdf");
  can1->SaveAs("mvd_matbudget_v18a.eps");
  can1->SaveAs("mvd_matbudget_v18a.png");

  cout << endl << "i am out, pease!" << endl;
  return 0;
}

