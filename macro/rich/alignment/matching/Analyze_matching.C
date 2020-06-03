#include <Analyze_matching.h>
#include <iomanip>
#include <vector>

/* Draw histograms of radius, dR, a, b and b over a, on a same canvass. */

void Analyze_matching(TString geom_nb = "00001")
{
  LoadLibs();

  TString outDir = "/data/misalignment_correction/Sim_Outputs/Matching/test/reference/";
  LoadSimFiles(geom_nb, outDir);
  PrepareHistos(geom_nb, outDir);

  cbmrec = (TTree*) RecFile->Get("cbmsim");

  hits = new TClonesArray("CbmRichHit");
  cbmrec->SetBranchAddress("RichHit", &hits);

  rings = new TClonesArray("CbmRichRing");
  cbmrec->SetBranchAddress("RichRing", &rings);

  ringmatch = new TClonesArray("CbmRichRingMatch");
  cbmrec->SetBranchAddress("RichRingMatch", &ringmatch);

  int nEv = cbmrec->GetEntries();
  cout << "We have " << nEv << " events" << endl;

  cout << endl << "---------- Filling the ring-track VS PMT position histogram ----------" << endl << endl;
  sleep(3);

  for(Int_t iev = 0; iev < nEv; iev++) {

    cbmrec->GetEntry(iev);
    int nHits = hits->GetEntriesFast();
    int nRings = rings->GetEntriesFast();
    //int nRingMatch = ringmatch->GetEntriesFast();
    cout << endl;
    cout << "Event ID : " << iev+1 << "; nb of total Hits = " << nHits << endl;

    for (int ir = 0; ir < nRings; ir++) {
      CbmRichRing* ring = (CbmRichRing*) rings->At(ir);
      float radius = ring->GetRadius();
      if(radius<=0.){continue;}		// With ideal finder --> many rings with radius -1
      if(!(radius>0)) {continue;}	// Test if radius is NAN - if(!(radius<=1. || radius>1.))
      //cout << "For ring number: " << ir << ", radius = " << radius << endl;

      float aA = ring->GetAaxis();
      float bA = ring->GetBaxis();
      H_Radius->Fill(radius); H_aAxis->Fill(aA); H_bAxis->Fill(bA); H_boa->Fill(bA/aA);

      float CentX = ring->GetCenterX();
      float CentY = ring->GetCenterY();
      int nAllHitsInR = ring->GetNofHits();
      cout << "ir = " << ir << " ; Number of hits = " << nAllHitsInR << endl;
      H_nbHits->Fill(nAllHitsInR);

      for(int iH=0;iH<nAllHitsInR;iH++) {
        CbmRichHit* hit = (CbmRichHit*) hits->At(ring->GetHit(iH));
        double xH=hit->GetX();
        double yH=hit->GetY();
        double dR=aA-TMath::Sqrt( (CentX-xH)*(CentX-xH) + (CentY-yH)*(CentY-yH) );
        H_dR->Fill(dR);

        }
      }
    cout << "---------------------------------------------------" << endl;
    }

/*  TCanvas *Can_R = new TCanvas("Can_nbHits","nbHits", 10, 10, 510, 510);
    TPad *cpad = new TPad("cpad","Pad", 0.1, 0.1, 0.9, 0.9);
    gPad->SetLogy(0);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLeftMargin(0.1);
    gPad->SetRightMargin(0.05);
    gPad->SetBottomMargin(0.08);
    gPad->SetTopMargin(0.1);

  TH1F* frame = cpad->DrawFrame(10.,0.,40.,7000.); // Set x and y scales 270.
    frame->SetLabelSize(0.03,"X");
    frame->SetTitleSize(0.04,"X");
    frame->SetTitleOffset(0.8,"X");
    frame->SetXTitle("nbHits");
    frame->SetLabelSize(0.03,"Y");
    frame->SetTitleSize(0.04,"Y");
    frame->SetTitleOffset(1.2,"Y");
    frame->SetYTitle("number of events");
    frame->Draw("mMrRuo");

  H_nbHits->Draw("same");*/

  WriteHistos(geom_nb);

 // Plots multiple histograms on one window
  TCanvas *c1 = new TCanvas();
  c1->Divide(2,3); // First param = column; Second = row
  c1->cd(1);
  H_Radius->SetLineColor(2);
  H_Radius->Draw();
  c1->cd(2);
  H_dR->SetLineColor(3);
  H_dR->Draw();
  c1->cd(3);
  H_aAxis->SetLineColor(4);
  H_aAxis->Draw();
  c1->cd(4);
  H_bAxis->SetLineColor(5);
  H_bAxis->Draw();
  c1->cd(5);
  H_boa->SetLineColor(6);
  H_boa->Draw();

  TString outDir = "/data/cbm/cbmroot/macro/rich/alignment/misalignment_correction/Sim_Outputs/Analysis_Outputs/";
  TString HistosFile = "RingHistos." + geom_nb;
  TString outDir2 = outDir + HistosFile;
  c1->SaveAs(outDir2 + ".png");

/*  // Draw multiple histograms on same graph
  H_Radius->SetLineColor(1);
  H_Radius->SetLineWidth(2);
  H_Radius->Draw("same");

  H_aAxis->SetLineColor(2);
  H_aAxis->SetLineWidth(2);
  H_aAxis->Draw("same");

  H_bAxis->SetLineColor(3);
  H_bAxis->SetLineWidth(2);
  H_bAxis->Draw("same");

  H_boa->SetLineColor(4);
  H_boa->SetLineWidth(2);
  H_boa->Draw("same");

  H_dR->SetLineColor(5);
  H_dR->SetLineWidth(2);
  H_dR->Draw("same");
*/
}

// Functions
// ------------------------------------------------------------------- //

void LoadLibs()
{
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
//  basiclibs();
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libCbmBase");
  gSystem->Load("libCbmData");
//  gSystem->Load("libField");
  gSystem->Load("libGen");
  gSystem->Load("libPassive");
//  gSystem->Load("libMvd");
//  gSystem->Load("libSts");
//  gSystem->Load("libEcal");
  gSystem->Load("libKF");
//  gSystem->Load("libRich");
//  gSystem->Load("libTrd");
//  gSystem->Load("libTof");
  gSystem->Load("libEve");
  gSystem->Load("libEventDisplay");
  gSystem->Load("libMinuit2"); // Nedded for rich ellipse fitter
}

// ------------------------------------------------------------------- //

void LoadSimFiles(TString geom_nb, TString outDir)
{
  TString ParFileTxt = outDir + "param." + geom_nb + ".root";
  TString McFileTxt = outDir + "mc." + geom_nb + ".root";
  TString RecoFileTxt = outDir + "reco." + geom_nb + ".root";

  ParFile = new TFile(ParFileTxt);
  McFile = new TFile(McFileTxt);
  RecFile = new TFile(RecoFileTxt);

  cout << ParFileTxt << endl;
  cout << McFileTxt << endl;
  cout << RecoFileTxt << endl;
}

// ------------------------------------------------------------------- //

void PrepareHistos(TString geom_nb, TString outDir)
{
  TString HistText1 = "Radius_" + geom_nb;
  H_Radius = new TH1F(HistText1, HistText1, 2001, 1., 10.);

  HistText1 = "dR_" + geom_nb;
  H_dR = new TH1F(HistText1, HistText1, 2001, -2., 4.);

  HistText1 = "a_Axis_" + geom_nb;
  H_aAxis = new TH1F(HistText1, HistText1, 2001, 1., 10.);

  HistText1 = "b_Axis_" + geom_nb;
  H_bAxis = new TH1F(HistText1, HistText1, 2001, 0., 10.);

  HistText1 = "b_over_a_" + geom_nb;
  H_boa = new TH1F(HistText1, HistText1, 2001, -0.5, 1.5);

  HistText1 = "nb_Hits_" + geom_nb;
  H_nbHits = new TH1F(HistText1, HistText1, 50., 0., 50.);

  // TH2F *H_Points = new TH2F("rings","rings",2001, -100., 100.,2001, 50.,250.);
}

// ------------------------------------------------------------------- //

void WriteHistos(TString geom_nb, TString outDir)
{
  TString HistosFile = outDir + "RingHistos." + geom_nb + ".root";
  TFile *Histos = new TFile(HistosFile, "RECREATE");

  cout << endl << "Writing Histograms in the following file: " << HistosFile << endl;

  H_Radius->Write();
  H_dR->Write();
  H_aAxis->Write();
  H_bAxis->Write();
  H_boa->Write();
  H_nbHits->Write();

  Histos->Close();
}
