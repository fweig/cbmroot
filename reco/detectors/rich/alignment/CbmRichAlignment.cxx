/* Copyright (C) 2016-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jordan Bendarouach [committer] */

// ---------- Original Headers ---------- //
#include "CbmRichAlignment.h"

#include "CbmDrawHist.h"
#include "CbmRichHit.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"

#include <iostream>
#include <vector>

// ---------- Included Headers ---------- //
#include "CbmMCTrack.h"
#include "CbmRichConverter.h"
#include "CbmRichPoint.h"
#include "CbmRichRingFitterCOP.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmRichRingLight.h"
#include "CbmTrackMatchNew.h"
#include "CbmUtils.h"

#include "FairTrackParam.h"
#include "FairVolume.h"

#include "TEllipse.h"
#include "TGeoManager.h"

#include <algorithm>
#include <fstream>
#include <iomanip>

#include <stdlib.h>
//#include <stdio.h>
#include "CbmGlobalTrack.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHitProducer.h"

//#include "TLorentzVector.h"
#include "TGeoSphere.h"
#include "TVirtualMC.h"
class TGeoNode;
class TGeoVolume;
class TGeoShape;
class TGeoMatrix;
#include <sstream>

CbmRichAlignment::CbmRichAlignment()
  : FairTask()
  , fRichHits(NULL)
  , fRichRings(NULL)
  , fRichProjections(NULL)
  , fRichMirrorPoints(NULL)
  , fMCTracks(NULL)
  , fRichRingMatches(NULL)
  , fRichPoints(NULL)
  , fHM(NULL)
  ,
  //fGP(),
  fEventNum(0)
  , fOutputDir("")
  , fRunTitle("")
  , fAxisRotTitle("")
  , fNumbAxis(0)
  , fTile(0)
  , fDrawAlignment(kTRUE)
  , fCopFit(NULL)
  , fTauFit(NULL)
  , fPhi()
{
}

CbmRichAlignment::~CbmRichAlignment() {}

InitStatus CbmRichAlignment::Init()
{
  FairRootManager* manager = FairRootManager::Instance();

  fRichHits = (TClonesArray*) manager->GetObject("RichHit");
  if (NULL == fRichHits) { Fatal("CbmRichAlignment::Init", "No RichHit array !"); }

  fRichRings = (TClonesArray*) manager->GetObject("RichRing");
  if (NULL == fRichRings) { Fatal("CbmRichAlignment::Init", "No RichRing array !"); }

  fRichProjections = (TClonesArray*) manager->GetObject("RichProjection");
  if (NULL == fRichProjections) { Fatal("CbmRichAlignment::Init", "No RichProjection array !"); }

  fRichPoints = (TClonesArray*) manager->GetObject("RichPoint");
  if (NULL == fRichPoints) { Fatal("CbmRichAlignment::Init", "No RichPoint array !"); }

  fMCTracks = (TClonesArray*) manager->GetObject("MCTrack");
  if (NULL == fMCTracks) { Fatal("CbmRichAlignment::Init", "No MCTracks array !"); }

  fRichRingMatches = (TClonesArray*) manager->GetObject("RichRingMatch");
  if (NULL == fRichRingMatches) { Fatal("CbmRichAlignment::Init", "No RichRingMatches array !"); }

  fRichMirrorPoints = (TClonesArray*) manager->GetObject("RichMirrorPoint");
  if (NULL == fRichMirrorPoints) { Fatal("CbmRichAlignment::Init", "No RichMirrorPoints array !"); }

  /*	fRichRefPlanePoints  = (TClonesArray*) manager->GetObject("RefPlanePoint");
	if (NULL == fRichRefPlanePoints) { Fatal("CbmRichAlignment::Init", "No RichRefPlanePoint array !"); }

	fRichMCPoints = (TClonesArray*) manager->GetObject("RichPoint");
	if (NULL == fRichMCPoints) { Fatal("CbmRichAlignment::Init", "No RichMCPoints array !"); }

	fGlobalTracks = (TClonesArray*) manager->GetObject("GlobalTrack");
	if (NULL == fGlobalTracks) { Fatal("CbmAnaDielectronTask::Init","No GlobalTrack array!"); }
*/

  fCopFit = new CbmRichRingFitterCOP();
  fTauFit = new CbmRichRingFitterEllipseTau();
  CbmRichConverter::Init();

  InitHistAlignment();

  return kSUCCESS;
}

void CbmRichAlignment::InitHistAlignment()
{
  fHM = new CbmHistManager();

  fHM->Create1<TH1D>("fHCenterDistance", "fHCenterDistance;Distance C-C';Nb of entries", 100, -0.1, 5.);
  fHM->Create1<TH1D>("fHPhi", "fHPhi;Phi_Ch [rad];Nb of entries", 200, -3.4, 3.4);
  fHM->Create1<TH1D>("fHThetaDiff", "fHThetaDiff;Th_Ch-Th_0 [cm];Nb of entries", 252, -5., 5.);
  fHM->Create2<TH2D>("fHCherenkovHitsDistribTheta0", "fHCherenkovHitsDistribTheta0;Phi_0 [rad];Theta_0 [cm];Entries",
                     200, -2., 2., 600, 2., 8.);
  fHM->Create2<TH2D>("fHCherenkovHitsDistribThetaCh",
                     "fHCherenkovHitsDistribThetaCh;Phi_Ch [rad];Theta_Ch [cm];Entries", 200, -3.4, 3.4, 600, 0., 20);
  fHM->Create2<TH2D>("fHCherenkovHitsDistribReduced",
                     "fHCherenkovHitsDistribReduced;Phi_Ch [rad];Th_Ch-Th_0 [cm];Entries", 200, -3.4, 3.4, 500, -5.,
                     5.);
}

void CbmRichAlignment::Exec(Option_t* option)
{
  cout << endl
       << "//"
          "--------------------------------------------------------------------"
          "-----------------------------------------------//"
       << endl;
  cout << "//------------------------------------------ CbmRichAlignment: EXEC "
          "Function ------------------------------------------//"
       << endl;
  cout << "//"
          "--------------------------------------------------------------------"
          "--------------------------------------------------//"
       << endl;
  fEventNum++;
  //LOG(debug2) << "CbmRichAlignment : Event #" << fEventNum;
  cout << "CbmRichAlignment : Event #" << fEventNum << endl;

  Int_t nofRingsInEvent = fRichRings->GetEntriesFast();
  Int_t nofMirrorPoints = fRichMirrorPoints->GetEntriesFast();
  Int_t nofHitsInEvent  = fRichHits->GetEntriesFast();
  Int_t NofMCTracks     = fMCTracks->GetEntriesFast();
  //	Int_t NofMCPoints = fRichMCPoints->GetEntriesFast();
  cout << "Nb of rings in evt = " << nofRingsInEvent << ", nb of mirror points = " << nofMirrorPoints
       << ", nb of hits in evt = " << nofHitsInEvent << " and nb of Monte-Carlo tracks = " << NofMCTracks << endl
       << endl;  //", nb of Monte-Carlo points = " << NofMCPoints <<

  if (nofRingsInEvent == 0) { cout << "Error no rings registered in event." << endl << endl; }
  else {
    CalculateAnglesAndDrawDistrib();
  }
}

void CbmRichAlignment::CalculateAnglesAndDrawDistrib()
{
  cout << "//------------------------------ CbmRichAlignment: Calculate Angles "
          "& Draw Distrib ------------------------------//"
       << endl
       << endl;

  Double_t trackX = 0., trackY = 0.;
  GetTrackPosition(trackX, trackY);

  Int_t nofRingsInEvent = fRichRings->GetEntriesFast();
  Float_t DistCenters, Theta_Ch, Theta_0, Angles_0;
  Float_t Pi    = 3.14159265;
  Float_t TwoPi = 2. * 3.14159265;
  fPhi.resize(kMAX_NOF_HITS);

  // ------------------------- Loop to get ring center coordinates and photon hit coordinates per ring and per event ------------------------- //
  if (nofRingsInEvent >= 1) {
    cout << "Number of Rings in event: " << nofRingsInEvent << endl;
    //sleep(2);
    for (Int_t iR = 0; iR < nofRingsInEvent; iR++) {
      // ----- Convert Ring to Ring Light ----- //
      CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(iR));
      CbmRichRingLight ringL;
      CbmRichConverter::CopyHitsToRingLight(ring, &ringL);
      fCopFit->DoFit(&ringL);
      // ----- Distance between mean center and fitted center calculation ----- //
      DistCenters = sqrt(TMath::Power(ringL.GetCenterX() - trackX, 2) + TMath::Power(ringL.GetCenterY() - trackY, 2));
      fHM->H1("fHCenterDistance")->Fill(DistCenters);
      // ----- Declaration of new variables ----- //
      Int_t NofHits = ringL.GetNofHits();
      Float_t xRing = ringL.GetCenterX();
      Float_t yRing = ringL.GetCenterY();

      for (Int_t iH = 0; iH < NofHits; iH++) {
        // ----- Phi angle calculation ----- //
        Int_t HitIndex  = ring->GetHit(iH);
        CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(HitIndex));
        Float_t xHit    = hit->GetX();
        Float_t yHit    = hit->GetY();
        Angles_0        = TMath::ATan2((hit->GetX() - ringL.GetCenterX()),
                                (ringL.GetCenterY() - hit->GetY()));  //* TMath::RadToDeg();
        //cout << "Angles_0 = " << Angles_0[iH] << endl;

        if (xRing - xHit == 0 || yRing - yHit == 0) continue;
        fPhi[iH] = TMath::ATan2(yHit - yRing, xHit - xRing);
        fHM->H1("fHPhi")->Fill(fPhi[iH]);

        // ----- Theta_Ch and Theta_0 calculations ----- //
        Theta_Ch = sqrt(TMath::Power(trackX - hit->GetX(), 2) + TMath::Power(trackY - hit->GetY(), 2));
        Theta_0 =
          sqrt(TMath::Power(ringL.GetCenterX() - hit->GetX(), 2) + TMath::Power(ringL.GetCenterY() - hit->GetY(), 2));
        //cout << "Theta_0 = " << Theta_0 << endl;
        fHM->H1("fHThetaDiff")->Fill(Theta_Ch - Theta_0);

        // ----- Filling of final histograms ----- //
        fHM->H2("fHCherenkovHitsDistribTheta0")->Fill(Angles_0, Theta_0);
        fHM->H2("fHCherenkovHitsDistribThetaCh")->Fill(fPhi[iH], Theta_Ch);
        fHM->H2("fHCherenkovHitsDistribReduced")->Fill(fPhi[iH], (Theta_Ch - Theta_0));
      }
      //cout << endl;
    }
  }
}

void CbmRichAlignment::GetTrackPosition(Double_t& x, Double_t& y)
{
  Int_t NofProjections = fRichProjections->GetEntriesFast();
  //cout << "!!! NB PROJECTIONS !!! " << NofProjections << endl;
  for (Int_t iP = 0; iP < NofProjections; iP++) {
    FairTrackParam* pr = (FairTrackParam*) fRichProjections->At(iP);
    if (NULL == pr) {
      x = 0.;
      y = 0.;
      cout << "Error: CbmRichAlignment::GetTrackPosition. No fair track param "
              "found."
           << endl;
    }
    x = pr->GetX();
    y = pr->GetY();
    //cout << "Center X: " << *x << " and Center y: " << *y << endl;
  }
}

void CbmRichAlignment::DrawHistAlignment()
{
  TCanvas* c1 = new TCanvas(fRunTitle + "_Data_Histograms_" + fAxisRotTitle,
                            fRunTitle + "_Data_Histograms_" + fAxisRotTitle, 800, 400);
  c1->Divide(2, 1);
  c1->cd(1);
  /*c1->SetGridx(1);
	c1->SetGridy(1);
	fHM2->H1("fHCenterDistance")->GetXaxis()->SetLabelSize(0.03);
	fHM2->H1("fHCenterDistance")->GetXaxis()->SetTitleSize(0.03);
	fHM2->H1("fHCenterDistance")->GetXaxis()->CenterTitle();
	fHM2->H1("fHCenterDistance")->GetXaxis()->SetNdivisions(612,kTRUE);
	fHM2->H1("fHCenterDistance")->GetYaxis()->CenterTitle();
	fHM2->H1("fHCenterDistance")->GetYaxis()->SetTitleSize(0.03);
	fHM2->H1("fHCenterDistance")->GetYaxis()->SetTitleOffset(1.);
	fHM2->H1("fHCenterDistance")->Draw();*/
  DrawH1(fHM->H1("fHCenterDistance"));
  c1->cd(2);
  DrawH2(fHM->H2("fHCherenkovHitsDistribReduced"));
  /*	c1->cd(3);
	DrawH1(fHM->H1("fHThetaDiff"));
	c1->cd(4);
	DrawH2(fHM->H2("fHCherenkovHitsDistribTheta0"));
	c1->cd(5);
	DrawH1(fHM->H1("fHPhi"));
	c1->cd(6);
	DrawH2(fHM->H2("fHCherenkovHitsDistribThetaCh"));
*/
  Cbm::SaveCanvasAsImage(c1, string(fOutputDir.Data()), "png");
}

void CbmRichAlignment::DrawFit(vector<Double_t>& outputFit, Int_t thresh)
{
  //vector<Double_t> paramVect;
  //paramVect.reserve(5);

  TCanvas* c3 = new TCanvas(fRunTitle + "_Fit_Histograms_" + fAxisRotTitle,
                            fRunTitle + "_Fit_Histograms_" + fAxisRotTitle, 1100, 600);
  c3->SetFillColor(42);
  c3->Divide(4, 2);
  gPad->SetTopMargin(0.1);
  gPad->SetFillColor(33);
  c3->cd(1);
  //	TH2D* CloneArr = (TH2D*)fHM2->H2("fHCherenkovHitsDistribThetaCh")->Clone();
  TH2D* CloneArr = (TH2D*) fHM->H2("fHCherenkovHitsDistribReduced")->Clone();
  CloneArr->GetXaxis()->SetLabelSize(0.03);
  CloneArr->GetXaxis()->SetTitleSize(0.03);
  CloneArr->GetXaxis()->CenterTitle();
  CloneArr->GetXaxis()->SetNdivisions(612, kTRUE);
  CloneArr->GetYaxis()->SetLabelSize(0.03);
  CloneArr->GetYaxis()->SetTitleSize(0.03);
  CloneArr->GetYaxis()->SetNdivisions(612, kTRUE);
  CloneArr->GetYaxis()->CenterTitle();
  //CloneArr->GetYaxis()->SetRangeUser(-2.5,2.5);
  CloneArr->GetZaxis()->SetLabelSize(0.03);
  CloneArr->GetZaxis()->SetTitleSize(0.03);
  CloneArr->GetYaxis()->SetTitleOffset(1.0);
  CloneArr->Draw("colz");
  //Double_t ymax = CloneArr->GetYaxis()->GetXmax();
  //Double_t ymin = CloneArr->GetYaxis()->GetXmin();
  //TF1 *fgauss = TF1 fgauss("gauss", "[0]*exp(-0.5*((x-[1])/[2])**2)", 0, 100);

  // ------------------------------ APPLY THRESHOLD TO 2D-HISTO ------------------------------ //
  //	TH2D* CloneArr_2 = (TH2D*)fHM2->H2("fHCherenkovHitsDistribThetaCh")->Clone();
  TH2D* CloneArr_2 = (TH2D*) fHM->H2("fHCherenkovHitsDistribReduced")->Clone();
  for (Int_t y_bin = 1; y_bin <= 500; y_bin++) {
    for (Int_t x_bin = 1; x_bin <= 200; x_bin++) {
      /*if (CloneArr_2->GetBinContent(x_bin, y_bin)!=0) {
			cout << "Bin Content: " << CloneArr_2->GetBinContent(x_bin, y_bin) << endl;
			sleep(1);
			}
			else;*/
      if (CloneArr_2->GetBinContent(x_bin, y_bin) < thresh) { CloneArr_2->SetBinContent(x_bin, y_bin, 0); }
    }
  }
  c3->cd(2);
  CloneArr_2->GetXaxis()->SetLabelSize(0.03);
  CloneArr_2->GetXaxis()->SetTitleSize(0.03);
  CloneArr_2->GetXaxis()->CenterTitle();
  CloneArr_2->GetXaxis()->SetNdivisions(612, kTRUE);
  CloneArr_2->GetYaxis()->SetLabelSize(0.03);
  CloneArr_2->GetYaxis()->SetTitleSize(0.03);
  CloneArr_2->GetYaxis()->SetNdivisions(612, kTRUE);
  CloneArr_2->GetYaxis()->CenterTitle();
  //CloneArr_2->GetYaxis()->SetRangeUser(-2.5,2.5);
  CloneArr_2->GetZaxis()->SetLabelSize(0.03);
  CloneArr_2->GetZaxis()->SetTitleSize(0.03);
  CloneArr_2->GetYaxis()->SetTitleOffset(1.0);
  CloneArr_2->Draw("colz");
  CloneArr_2->Write();

  // -------------------- FIT SLICES AND FIT THE MEAN OF THE RESULT TO A SIN FUNCTION -------------------- //
  CloneArr_2->FitSlicesY(0, 0, -1, 1);
  c3->cd(3);
  TH1D* histo_0 = gDirectory->Get<TH1D>("fHCherenkovHitsDistribReduced_0");
  histo_0->Draw();
  c3->cd(4);
  TH1D* histo_1 = gDirectory->Get<TH1D>("fHCherenkovHitsDistribReduced_1");
  //histo_1->GetYaxis()->SetRangeUser(-2.5, 2.5);
  histo_1->Draw();
  c3->cd(5);
  TH1D* histo_2 = gDirectory->Get<TH1D>("fHCherenkovHitsDistribReduced_2");
  histo_2->Draw();
  c3->cd(6);
  TH1D* histo_chi2 = gDirectory->Get<TH1D>("fHCherenkovHitsDistribReduced_chi2");
  histo_chi2->Draw();

  c3->cd(7);
  TF1* f1 = new TF1("f1", "[2]+[0]*cos(x)+[1]*sin(x)", -3.5, 3.5);
  f1->SetParameters(0, 0, 0);
  f1->SetParNames("Delta_phi", "Delta_lambda", "Offset");
  histo_1->Fit("f1", "", "");
  TF1* fit      = histo_1->GetFunction("f1");
  Double_t p1   = fit->GetParameter("Delta_phi");
  Double_t p2   = fit->GetParameter("Delta_lambda");
  Double_t p3   = fit->GetParameter("Offset");
  Double_t chi2 = fit->GetChisquare();
  //cout << setprecision(6) << "Delta_phi = " << fit->GetParameter(0) << " and delta_lambda = " << fit->GetParameter(1) << endl;
  //cout << "Delta_phi error = " << fit->GetParError(0) << " and delta_lambda error = " << fit->GetParError(1) << endl;
  //cout << endl << "Chi2: " << chi2 << endl;

  /*	paramVect.push_back(fit->GetParameter("Delta_phi"));
	paramVect.push_back(fit->GetParameter("Delta_lambda"));
	paramVect.push_back(fit->GetParameter("Offset"));
	paramVect.push_back(fit->GetChisquare());
	//cout << "Vectors: Delta_phi = " << paramVect[0] << ", Delta_lambda = " << paramVect[1] << ", Offset = " << paramVect[2] << endl;
*/
  f1->SetParameters(fit->GetParameter(0), fit->GetParameter(1));
  char leg[128];
  f1->SetLineColor(2);
  f1->Draw();
  f1->Write();

  // ------------------------------ CALCULATION OF MISALIGNMENT ANGLE ------------------------------ //
  Double_t Focal_length = 150., q = 0., A = 0., Alpha = 0., mis_x = 0., mis_y = 0.;
  // mis_x && mis_y corresponds respect. to rotation angles around the Y and X axes.
  // !!! BEWARE: AXES INDEXES ARE SWITCHED !!!
  q = TMath::ATan(fit->GetParameter(0) / fit->GetParameter(1));
  cout << endl << "fit_1 = " << fit->GetParameter(0) << " and fit_2 = " << fit->GetParameter(1) << endl;
  //cout << "q = " << q << endl;
  A = fit->GetParameter(1) / TMath::Cos(q);
  //cout << "Parameter a = " << A << endl;
  Alpha =
    TMath::ATan(A / 1.5) * 0.5
    * TMath::Power(
      10,
      3);  // *0.5, because a mirror rotation of alpha implies a rotation in the particle trajectory of 2*alpha ; 1.5 meters = Focal length = Radius_of_curvature/2
  //cout << setprecision(6) << "Total angle of misalignment alpha = " << Alpha << endl;		// setprecision(#) gives the number of digits in the cout.
  mis_x = TMath::ATan(fit->GetParameter(0) / Focal_length) * 0.5 * TMath::Power(10, 3);
  mis_y = TMath::ATan(fit->GetParameter(1) / Focal_length) * 0.5 * TMath::Power(10, 3);
  //cout << "Horizontal displacement = " << outputFit[0] << " [mrad] and vertical displacement = " << outputFit[1] << " [mrad]." << endl;

  TLegend* LEG = new TLegend(0.27, 0.7, 0.85, 0.87);  // Set legend position
  LEG->SetBorderSize(1);
  LEG->SetFillColor(0);
  LEG->SetMargin(0.2);
  LEG->SetTextSize(0.03);
  sprintf(leg, "Fitted sinusoid");
  LEG->AddEntry(f1, leg, "l");
  sprintf(leg, "Rotation angle around X = %f", mis_y);
  LEG->AddEntry("", leg, "l");
  sprintf(leg, "Rotation angle around Y = %f", mis_x);
  LEG->AddEntry("", leg, "l");
  sprintf(leg, "Offset = %f", fit->GetParameter(2));
  LEG->AddEntry("", leg, "l");
  LEG->Draw();
  //Cbm::SaveCanvasAsImage(c3, string(fOutputDir.Data()), "png");

  TCanvas* c4 =
    new TCanvas(fRunTitle + "_Sinus_Fit_" + fAxisRotTitle, fRunTitle + "_Sinus_Fit_" + fAxisRotTitle, 400, 400);
  c4->SetGrid(1, 1);
  CloneArr_2->Draw("colz");
  f1->Draw("same");
  TLegend* LEG1 = new TLegend(0.35, 0.7, 0.72, 0.85);  // Set legend position
  LEG1->SetBorderSize(1);
  LEG1->SetFillColor(0);
  LEG1->SetMargin(0.2);
  LEG1->SetTextSize(0.03);
  sprintf(leg, "Fitted sinusoid");
  LEG1->AddEntry(f1, leg, "l");
  sprintf(leg, "Misalign in X = %f", mis_x);
  LEG1->AddEntry("", leg, "l");
  sprintf(leg, "Misalign in Y = %f", mis_y);
  LEG1->AddEntry("", leg, "l");
  sprintf(leg, "Offset = %f", fit->GetParameter(2));
  LEG1->AddEntry("", leg, "l");
  LEG1->Draw();
  Cbm::SaveCanvasAsImage(c4, string(fOutputDir.Data()), "png");

  // ------------------------------ APPLY SECOND FIT USING LOG-LIKELIHOOD METHOD ------------------------------ //
  /*	TCanvas* c4 = new TCanvas(fRunTitle + "_Second_Fit_" + fAxisRotTitle, fRunTitle + "_Second_Fit_" + fAxisRotTitle, 600, 600);
	c4->SetFillColor(42);
	gPad->SetTopMargin(0.1);
	gPad->SetFillColor(33);
	f1->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2));
	histo_1->Fit("f1","L","");
	TF1 *fit2 = histo_1->GetFunction("f1");
	f1->SetParameters(fit2->GetParameter(0), fit2->GetParameter(1), fit2->GetParameter(2));
	f1->Draw();

	Double_t q_2 = TMath::ATan(fit2->GetParameter(0)/fit2->GetParameter(1));
	//cout << endl << "fit2_1 = " << fit2->GetParameter(0) << " and fit2_2 = " << fit2->GetParameter(1) << endl;
	//cout << "q_2 = " << q_2 << endl;
	Double_t A_2 = fit2->GetParameter(1)/TMath::Cos(q_2);
	//cout << "Parameter a_2 = " << A_2 << endl;
	Double_t Alpha_2 = TMath::ATan(A_2/1.5)*0.5*TMath::Power(10,3);
	//cout << setprecision(6) << "Total angle of misalignment alpha_2 = " << Alpha_2 << endl;
	Double_t mis_x_2 = TMath::ATan(fit2->GetParameter(0)/Focal_length)*0.5*TMath::Power(10,3);
	Double_t mis_y_2 = TMath::ATan(fit2->GetParameter(1)/Focal_length)*0.5*TMath::Power(10,3);

	TLegend* LEG2= new TLegend(0.31,0.7,0.72,0.85); // Set legend position
	LEG2->SetBorderSize(1);
	LEG2->SetFillColor(0);
	LEG2->SetMargin(0.2);
	LEG2->SetTextSize(0.03);
	sprintf(leg, "Fitted sinusoid");
	LEG2->AddEntry(f1, leg, "l");
	sprintf(leg, "Misalign in X = %f", mis_x_2);
	LEG2->AddEntry("", leg, "l");
	sprintf(leg, "Misalign in Y = %f", mis_y_2);
	LEG2->AddEntry("", leg, "l");
	sprintf(leg, "Offset = %f", fit2->GetParameter(2));
	LEG2->AddEntry("", leg, "l");
	LEG2->Draw();
	Cbm::SaveCanvasAsImage(c4, string(fOutputDir.Data()), "png");*/

  outputFit.at(0) = mis_y;
  outputFit.at(1) = mis_x;
  outputFit.at(2) = fit->GetParameter(1);
  outputFit.at(3) = fit->GetParameter(0);
}

void CbmRichAlignment::DrawHistFromFile(TString fileName)
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fHM         = new CbmHistManager();
  TFile* file = new TFile(fileName, "READ");
  LOG_IF(fatal, !file) << "Could not open file " << fileName;

  fHM->ReadFromFile(file);
  DrawHistAlignment();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmRichAlignment::Finish()
{
  cout << endl
       << "// "
          "-----------------------------------------------------------------------"
          "----------------------------------------------------------------- //"
       << endl;
  cout << "// -------------------------------------------------- CbmRichAlignment "
          "- Finish Function -------------------------------------------------- //"
       << endl;
  cout << "// "
          "-----------------------------------------------------------------------"
          "----------------------------------------------------------------- //"
       << endl
       << endl;

  if (fDrawAlignment) {
    DrawHistAlignment();
    Int_t thresh = 5;
    vector<Double_t> outputFit(4);
    DrawFit(outputFit, thresh);
    cout << setprecision(6) << endl;
    cout << "Horizontal displacement = " << outputFit[0] << " [mrad] and vertical displacement = " << outputFit[1]
         << " [mrad]." << endl;

    fHM->Create2<TH2D>("fHCherenkovHitsDistribReduced",
                       "fHCherenkovHitsDistribReduced;Phi_Ch [rad];Th_Ch-Th_0 [cm];Entries", 200, -3.4, 3.4, 500, -5.,
                       5.);

    ofstream corr_file;
    /*		// Converting double to string.
		TString s;
		std::ostringstream strs;
		strs << fNumb;
		std::string str = strs.str();
		s = "/data/misalignment_correction/Sim_Outputs/Alignment_Correction/correction_param" + str + ".txt";
*/
    TString s = fOutputDir + "correction_param_" + fNumbAxis + fTile + ".txt";
    corr_file.open(s);
    if (corr_file.is_open()) {
      corr_file << setprecision(7) << outputFit[0] << "\t";
      corr_file << setprecision(7) << outputFit[1] << "\t";
      corr_file << setprecision(7) << outputFit[2] << "\t";
      corr_file << setprecision(7) << outputFit[3] << "\t";
      corr_file.close();
      cout << "Wrote correction parameters to: " << s << endl;
    }
    else {
      cout << "Error in CbmRichAlignment::Finish ; unable to open parameter file!" << endl;
    }
  }

  //cout << setprecision(6) << endl;
}
ClassImp(CbmRichAlignment)
