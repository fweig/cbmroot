/** @file mcbm_analyze_delta_electrons.C
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Dominik Smith [committer] **/

/** @file mcbm_analyze_delta_electrons.C
 ** @author Dominik Smith <d.smith@gsi.de>
 ** @since 22 July 2020
 **/

/** @brief Main method
 ** @param dataSet    Prefix for input file names
 ** @param beamAngle  Angle of beam in global c.s. in x-z plane
 ** @param targetDz   Thickness of target [cm]
 **
 ** Reads output of a mcbm_transport_beam.C run. Finds 
 ** all MC tracks which hit a STS detector, then creates
 ** 3D histogram of their origin cooridinates, as well as 2D
 ** histograms in the x-z and y-z plane. Stores filtered
 ** MC tracks and histograms in output file, as well as a 
 ** TCanvas in which the path of the beam and the boundaries
 ** of the target are marked as lines in the x-z histogram. 
 **/

int mcbm_analyze_delta_electrons(TString dataSet = "mcbm_beam", Double_t beamAngle = 25., Double_t targetDz = 0.025)
{

  TString inFile = "./" + dataSet + ".tra.root";
  TFile* input   = new TFile(inFile);
  if (!input) {
    cout << "*** analyze_delta_electrons: Input file " << inFile << " not found!\n"
         << "Be sure to run mcbm_transport_beam.C before!" << endl;
    exit(1);
  }

  // Output file
  TString outFile = "./" + dataSet + "_delta_analysis.root";

  // Input tree and branch
  TTree* tree             = (TTree*) input->Get("cbmsim");
  TClonesArray* stsPoints = new TClonesArray("CbmStsPoint");
  TClonesArray* mcTracks  = new TClonesArray("CbmMCTrack");

  tree->SetBranchAddress("StsPoint", &stsPoints);
  tree->SetBranchAddress("MCTrack", &mcTracks);

  /* histogram creation */

  TH1* h1 = new TH1I("stsHisto", "STS points; number of points; count", 12, 0.0, 12.0);

  for (Int_t event = 0; event < tree->GetEntriesFast(); event++) {
    tree->GetEntry(event);
    cout << "*** Processing event " << event << endl;
    cout << "Number of MC tracks: " << mcTracks->GetEntriesFast() << endl;
    cout << "Number of STS points: " << stsPoints->GetEntriesFast() << endl;
    cout << endl;
    h1->Fill(stsPoints->GetEntriesFast());
  }

  /* collect mc tracks */
  CbmMCTrack* thisTrack = NULL;
  TTree* stsTrackList   = new TTree("stsTrackList", "MC tracks which hit STS");
  stsTrackList->Branch("StsTrack", &thisTrack);

  /* clang-format off */
  TH3* h3 = new TH3I("trackHisto", "MCtrack origin points; x; y; z", 140, -0.3, 0.3, 140, -0.3, 0.3, 140, -0.3, 0.3 );
  TH2* h2xz = new TH2I("trackHisto", "MCtrack origin points; x; z", 140, -0.3, 0.3, 140, -0.3, 0.3 );
  TH2* h2yz = new TH2I("trackHisto", "MCtrack origin points; y; z", 140, -0.3, 0.3, 140, -0.3, 0.3 );
  /* clang-format on */

  for (Int_t event = 0; event < tree->GetEntriesFast(); event++) {
    tree->GetEntry(event);
    cout << "*** Processing event " << event << endl;
    cout << "Number of STS points: " << stsPoints->GetEntriesFast() << endl;

    for (Int_t point = 0; point < stsPoints->GetEntriesFast(); point++) {
      CbmStsPoint* thisPoint = static_cast<CbmStsPoint*>(stsPoints->At(point));
      cout << "Track ID :" << thisPoint->GetTrackID() << endl;
      cout << "PID :" << thisPoint->GetPid() << endl;

      if (thisPoint->GetPid() == 11) {
        thisTrack = static_cast<CbmMCTrack*>(mcTracks->At(thisPoint->GetTrackID()));
        cout << "StartX = " << thisTrack->GetStartX() << " , StartY = " << thisTrack->GetStartY()
             << " , StartZ = " << thisTrack->GetStartZ() << endl;
        stsTrackList->Fill();
        h3->Fill(thisTrack->GetStartX(), thisTrack->GetStartY(), thisTrack->GetStartZ());
        h2xz->Fill(thisTrack->GetStartX(), thisTrack->GetStartZ());
        h2yz->Fill(thisTrack->GetStartY(), thisTrack->GetStartZ());
      }
    }
    cout << endl;
  }

  TCanvas* hisCanvas = new TCanvas("hisCanvas", "Histograms", 150, 10, 1200, 600);
  hisCanvas->Divide(2, 1);
  hisCanvas->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  h2xz->Draw("COLZ");

  Double_t beamStartX = -1.0 * sin(beamAngle * TMath::DegToRad());
  Double_t beamStartY = -1.0 * cos(beamAngle * TMath::DegToRad());
  TLine* beam         = new TLine(beamStartX, beamStartY, 0., 0.);
  beam->SetLineColor(kRed);
  beam->Draw("same");

  TVector2 p1(-0.3, targetDz / 2.);
  TVector2 p2(0.3, targetDz / 2.);
  p1               = p1.Rotate(-beamAngle * TMath::DegToRad());
  p2               = p2.Rotate(-beamAngle * TMath::DegToRad());
  TLine* targetTop = new TLine(p1.X(), p1.Y(), p2.X(), p2.Y());
  targetTop->SetLineColor(kBlack);
  targetTop->Draw("same");

  TVector2 p3(-0.3, -targetDz / 2.);
  TVector2 p4(0.3, -targetDz / 2.);
  p3                  = p3.Rotate(-beamAngle * TMath::DegToRad());
  p4                  = p4.Rotate(-beamAngle * TMath::DegToRad());
  TLine* targetBottom = new TLine(p3.X(), p3.Y(), p4.X(), p4.Y());
  targetBottom->SetLineColor(kBlack);
  targetBottom->Draw("same");

  hisCanvas->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  h2yz->Draw("COLZ");

  TFile f(outFile, "recreate");
  stsTrackList->Write();
  hisCanvas->Write();
  h1->Write();
  h3->Write();
  h2xz->Write();
  h2yz->Write();

  return 0;
}
