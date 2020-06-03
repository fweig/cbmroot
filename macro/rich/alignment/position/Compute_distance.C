#include <Compute_distance.h>
#include <iomanip>
#include <vector>
//#include <TH3D.h>

/* Draw histograms of radius, dR, a, b and b over a, on a same canvass. */

void Compute_distance(TString geom_nb = "", Int_t Flag = 0)
{
    LoadLibs();

    if (Flag == 0) {TString outDir = "/u/jbendar/Sim_Outputs/Ring_Track_VS_Position/Misaligned/";}
    else if (Flag == 1) {TString outDir = "/u/jbendar/Sim_Outputs/Ring_Track_VS_Position/Aligned/";}
    LoadSimFiles(geom_nb, outDir);
    PrepareHistos();

    cbmrec = (TTree*) RecFile->Get("cbmsim");

    hits = new TClonesArray("CbmRichHit");
    cbmrec->SetBranchAddress("RichHit", &hits);

    rings = new TClonesArray("CbmRichRing");
    cbmrec->SetBranchAddress("RichRing", &rings);

    ringmatch = new TClonesArray("CbmRichRingMatch");
    cbmrec->SetBranchAddress("RichRingMatch", &ringmatch);

    richProj = new TClonesArray("FairTrackParam");
    cbmrec->SetBranchAddress("RichProjection", &richProj);

    int nEv = cbmrec->GetEntries();
    cout << "Number of events: " << nEv << endl;

    for(Int_t iev = 0; iev < nEv; iev++) {
        cbmrec->GetEntry(iev);
        int nHits = hits->GetEntriesFast();
        int nRings = rings->GetEntriesFast();
        int nRingMatch = ringmatch->GetEntriesFast();
        cout << endl;
        cout << "Event ID : " << iev+1 << "; nb of total Hits = " << nHits << endl;

        for (int ir = 0; ir < nRings; ir++) {
            CbmRichRing* ring = (CbmRichRing*) rings->At(ir);
            float radius = ring->GetRadius();
            if(radius<=0. || radius>10.){continue;}		// With ideal finder --> many rings with radius -1
            if(!(radius>0)) {continue;}	// Test if radius is NAN - if(!(radius<=1. || radius>1.))
            //cout << "For ring number: " << ir << ", radius = " << radius << endl;

            //float aA = ring->GetAaxis();
            //float bA = ring->GetBaxis();
            double CentX = ring->GetCenterX();
            double CentY = ring->GetCenterY();
            int nAllHitsInR = ring->GetNofHits();
            cout << "ir = " << ir << " ; Number of hits = " << nAllHitsInR << endl;

            // Calculation of dR:
/*            for(int iH=0;iH<nAllHitsInR;iH++) {
                CbmRichHit* hit = (CbmRichHit*) hits->At(ring->GetHit(iH));
                double xH=hit->GetX();
                double yH=hit->GetY();
                double dR=aA-TMath::Sqrt( (CentX-xH)*(CentX-xH) + (CentY-yH)*(CentY-yH) );
            }
*/
            int ringTrackID = ring->GetTrackID();
            if(ringTrackID==-1){continue;}
            cout << "ringTrackID: " << ringTrackID << endl;
            FairTrackParam* pTrack = (FairTrackParam*) richProj->At(ringTrackID);
            double xTrack = pTrack->GetX();
            double yTrack = pTrack->GetY();
            double dist = TMath::Sqrt(TMath::Power(xTrack-CentX,2)+TMath::Power(yTrack-CentY,2));

            cout << "PMT position: [" << CentX << "; " << CentY << "] and distance = " << dist << endl;
            //sleep(3);
            H_distance->Fill(dist);
            cout << "---------- H_distance filled ----------" << endl;
            H_position->Fill(CentX, CentY, dist);
            cout << "---------- H_position filled ----------" << endl;
        }
        cout << "Event " << iev+1 << " done." << endl;
    }

    WriteHistos(outDir);

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

    // Plots multiple histograms on one window
    TCanvas *c1 = new TCanvas();
    c1->Divide(2,1); // First param = column; Second = row
    c1->cd(1);
    H_distance->SetLineColor(2);
    H_distance->Draw();
    c1->cd(2);
    H_position->SetLineColor(3);
    //H_position->Draw("LEGO2Z");
    H_position->Draw("colz");

    TString HistosFile = "Histos." + geom_nb;
    TString outDir2 = outDir + HistosFile;
    c1->SaveAs(outDir2 + ".png");
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
//    TString ParFileTxt = outDir + "param." + geom_nb + ".root";
//    TString McFileTxt = outDir + "mc." + geom_nb + ".root";
//    TString RecoFileTxt = outDir + "reco." + geom_nb + ".root";

    TString ParFileTxt = outDir + "param." + geom_nb + "root";
    TString McFileTxt = outDir + "mc." + geom_nb + "root";
    TString RecoFileTxt = outDir + "reco." + geom_nb + "root";

    ParFile = new TFile(ParFileTxt);
    McFile = new TFile(McFileTxt);
    RecFile = new TFile(RecoFileTxt);

    cout << ParFileTxt << endl;
    cout << McFileTxt << endl;
    cout << RecoFileTxt << endl;
}

// ------------------------------------------------------------------- //

void PrepareHistos()
{
    TString HistText1 = "Track-Ring Distance";
    H_distance = new TH1D(HistText1,HistText1,2001,0,5);

    TString HistText2 = "Track-Ring VS PMT position";
    H_position = new TH2D(HistText2,HistText2,2001, -100., 100.,2001,-200.,200.);

    cout << "Histos ready!" << endl;
}

// ------------------------------------------------------------------- //

void WriteHistos(TString& outDir, Int_t Flag = 0)
{
    TString buff = "";
    if (Flag = 0) { buff = "Misaligned"; }
    else if (Flag = 1) { buff = "Aligned"; }
    TString HistosFile = outDir + "Ring-Track_Study" + buff + ".root";
    TFile *Histos = new TFile(HistosFile, "RECREATE");

    cout << endl << "Writing Histograms in the following file: " << HistosFile << endl;

    H_distance->Write();
    H_position->Write();

    Histos->Close();
}
