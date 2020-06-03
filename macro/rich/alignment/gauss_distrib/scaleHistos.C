#include "TH3D.h"
#include <string>

int Counter=0;
int flagHisto=0;
TH3D* LoadHisto(TString rootPath, TString histoName, TString dirName);
void HistosQa(TH3D* h1, TH3D* h2, TH3D* h3);
void HistosUpAndDown(TH3D* h1, TH3D* h2, TH3D* h3, TH3D* h4, TH3D* h5, TH3D* h6, Double_t range);
void DrawRefHistos(TH2D* h1, TH2D* h2, TH2D* h3, TH2D* h4, TH2D* h5, TH2D* h6, TString str);
void Resize(TH2D* h1, TString str);
void DrawDivide(TH2D* h1, TH2D* h2, TH2D* h3, TString str);
TH2D* DrawH3Profile(TH3* h, Bool_t drawMean, Bool_t doGaussFit, Double_t zMin, Double_t zMax, Bool_t drawOpt);
void ShiftHisto(TH2D* h1);

void scaleHistos()
{
	flagHisto = 3;
	TH3D *h1, *h2, *h3, *h4, *h5, *h6;
	TString dirName = "", histoName = "", histoNameUp = "", histoNameDown = "";
	if ( flagHisto == 1 ) {
		dirName = "HistosQa";
		histoName = "fhRingTrackDistVsXYTruematchPrimel";
	}
	else if ( flagHisto == 2 || flagHisto == 3 ) {
		dirName = "HistosUpAndDown";
		histoNameUp = "fhRingTrackDistVsXYHalfUpTruematchPrimel";
		histoNameDown = "fhRingTrackDistVsXYHalfDownTruematchPrimel";
	}

	TString rootName = "histos.root";
	TString rootPath1 = "/data/Sim_Outputs/Align/Uncorrected/" + rootName;
	TString rootPath2 = "/data/Sim_Outputs/Gauss_sigma_1/Uncorrected/" + rootName;
	TString rootPath3 = "/data/Sim_Outputs/Gauss_sigma_1/Corrected/" + rootName;
	TString rootPath4 = "/data/Sim_Outputs/Gauss_sigma_3/Uncorrected/" + rootName;
	TString rootPath5 = "/data/Sim_Outputs/Gauss_sigma_3/Corrected/" + rootName;

	if ( flagHisto == 1 ) {
		h1 = LoadHisto(rootPath1, histoName, dirName);
		h2 = LoadHisto(rootPath2, histoName, dirName);
		h3 = LoadHisto(rootPath3, histoName, dirName);
		HistosQa(h1, h2, h3);
	}
	else if ( flagHisto == 2 ) {
		h1 = LoadHisto(rootPath1, histoNameUp, dirName);
		h2 = LoadHisto(rootPath2, histoNameUp, dirName);
		h3 = LoadHisto(rootPath3, histoNameUp, dirName);
		h4 = LoadHisto(rootPath1, histoNameDown, dirName);
		h5 = LoadHisto(rootPath2, histoNameDown, dirName);
		h6 = LoadHisto(rootPath3, histoNameDown, dirName);
		HistosUpAndDown(h1, h2, h3, h4, h5, h6, 1.);
	}
	else if ( flagHisto == 3 ) {
		h1 = LoadHisto(rootPath1, histoNameUp, dirName);
		h2 = LoadHisto(rootPath4, histoNameUp, dirName);
		h3 = LoadHisto(rootPath5, histoNameUp, dirName);
		h4 = LoadHisto(rootPath1, histoNameDown, dirName);
		h5 = LoadHisto(rootPath4, histoNameDown, dirName);
		h6 = LoadHisto(rootPath5, histoNameDown, dirName);
		HistosUpAndDown(h1, h2, h3, h4, h5, h6, 2.5);
	}

}

TH3D* LoadHisto(TString rootPath, TString histoName, TString dirName)
{
	TH3D* h1;
	TFile* Histograms = new TFile(rootPath, "READ");
	TDirectory* dir = gFile->GetDirectory(dirName);
//	dir->ls();
	h1 = (TH3D*) dir->Get(histoName);
	return(h1);
}

void HistosQa(TH3D* h1, TH3D* h2, TH3D* h3)
{
	Double_t range = 1.;
	TH2D* h4 = (TH2D*) DrawH3Profile(h1, true, false, 0., range, false);
	TH2D* h5 = (TH2D*) DrawH3Profile(h2, true, false, 0., range, false);
	TH2D* h6 = (TH2D*) DrawH3Profile(h3, true, false, 0., range, false);
	TH2D* h7 = (TH2D*) h4->Clone("h6");
	TH2D* h8 = (TH2D*) h5->Clone("h7");
	TH2D* h9 = (TH2D*) h6->Clone("h8");

	DrawRefHistos(h4, h5, h6, h7, h8, h9, "Up_Down");

	Resize(h4, "up");
	Resize(h5, "up");
	Resize(h6, "up");
	Resize(h7, "down");
	Resize(h8, "down");
	Resize(h9, "down");

	h4->SetTitle("Align");
	h5->SetTitle("Uncorrected");
	h6->SetTitle("Corrected");

	DrawDivide(h4, h5, h6, "Histos_Up");
	DrawDivide(h7, h8, h9, "Histos_Down");
}

void HistosUpAndDown(TH3D* h1, TH3D* h2, TH3D* h3, TH3D* h4, TH3D* h5, TH3D* h6, Double_t range)
{
	TH2D* h13 = (TH2D*) h1->Project3D("yx")->Clone();
	TH2D* h14 = (TH2D*) h2->Project3D("yx")->Clone();
	TH2D* h15 = (TH2D*) h3->Project3D("yx")->Clone();
	TH2D* h16 = (TH2D*) h4->Project3D("yx")->Clone();
	TH2D* h17 = (TH2D*) h5->Project3D("yx")->Clone();
	TH2D* h18 = (TH2D*) h6->Project3D("yx")->Clone();

//	DrawDivide(h13, h14, h15, "Histos_Up_Before_H3Profile");
//	DrawDivide(h16, h17, h18, "Histos_Down_Before_H3Profile");

	TH2D* h7 = (TH2D*) DrawH3Profile(h1, true, false, 0., range, false);
	TH2D* h8 = (TH2D*) DrawH3Profile(h2, true, false, 0., range, false);
	TH2D* h9 = (TH2D*) DrawH3Profile(h3, true, false, 0., range, false);
	TH2D* h10 = (TH2D*) DrawH3Profile(h4, true, false, 0., range, false);
	TH2D* h11 = (TH2D*) DrawH3Profile(h5, true, false, 0., range, false);
	TH2D* h12 = (TH2D*) DrawH3Profile(h6, true, false, 0., range, false);

	h7->SetTitle("Align Up");
	h8->SetTitle("Uncorrected Up");
	h9->SetTitle("Corrected Up");
	h10->SetTitle("Align Down");
	h11->SetTitle("Uncorrected Down");
	h12->SetTitle("Corrected Down");

	DrawRefHistos(h7, h8, h9, h10, h11, h12, "Up_Down");

	DrawDivide(h7, h8, h9, "Histos_Up_After_H3Profile");
	DrawDivide(h10, h11, h12, "Histos_Down_After_H3Profile");
}

void DrawRefHistos(TH2D* h1, TH2D* h2, TH2D* h3, TH2D* h4, TH2D* h5, TH2D* h6, TString str)
{
	stringstream ssName, ssTitle;
	ssName << "c1" << "_" << str << "_name";
	ssTitle << "c1" << "_" << str << "_title";
	TCanvas* c1 = new TCanvas(ssName.str().c_str(), ssTitle.str().c_str(), 1800, 900);
	c1->Divide(3,2);
	c1->cd(1);
	h1->Draw("colz");
	c1->cd(2);
	h2->Draw("colz");
	c1->cd(3);
	h3->Draw("colz");
	c1->cd(4);
	h4->Draw("colz");
	c1->cd(5);
	h5->Draw("colz");
	c1->cd(6);
	h6->Draw("colz");
}

void Resize(TH2D* h1, TString str)
{
	if ( str == "up" ) {
		h1->GetYaxis()->SetRangeUser(110., 200.);
	}
	else if ( str == "down" ) {
		h1->GetYaxis()->SetRangeUser(-200., -110.);
	}
}

void DrawDivide(TH2D* h1, TH2D* h2, TH2D* h3, TString str)
{
	stringstream ssName, ssTitle;
        ssName << "c" << Counter << "_" << str << "_name";
        ssTitle << "c" << Counter << "_" << str << "_title";
	TH2D* h4 = (TH2D*) h1->Clone();
	TH2D* h5 = (TH2D*) h1->Clone();

	TCanvas* c = new TCanvas(ssName.str().c_str(), ssTitle.str().c_str(), 1800, 900);
	c->Divide(3,2);
	c->cd(1);
	h1->Draw("colz");
	c->cd(2);
	h2->Draw("colz");
	c->cd(3);
	h3->Draw("colz");
	c->cd(4);
	h4->Divide(h2);
	ShiftHisto(h4);
	h4->Draw("colz");
	c->cd(5);
	h5->Divide(h3);
	ShiftHisto(h5);
	h5->Draw("colz");
	Counter++;
}

TH2D* DrawH3Profile(
		TH3* h,
		Bool_t drawMean,
		Bool_t doGaussFit,
		Double_t zMin,
		Double_t zMax,
		Bool_t drawOpt)
{
	Int_t nBinsX = h->GetNbinsX();
	Int_t nBinsY = h->GetNbinsY();
	TH2D* h2 = (TH2D*) h->Project3D("yx")->Clone();

	for (Int_t x = 1; x <= nBinsX; x++) {
		for (Int_t y = 1; y <= nBinsY; y++) {
			stringstream ss;
			ss << h->GetName() << "_z_" << x << "_" << y;
			TH1D* hz = h->ProjectionZ(ss.str().c_str(), x, x, y, y);
			Double_t ms = 0.;
			if (doGaussFit) {
				hz->Fit("gaus", "QO");
				TF1* func = hz->GetFunction("gaus");
				if (func != NULL) {
					ms = (drawMean)?func->GetParameter(1):func->GetParameter(2);
				}
			} else {
				ms = (drawMean)?hz->GetMean():hz->GetRMS();
			}
			h2->SetBinContent(x, y, ms);
		}
	}

	string zAxisTitle = string(h->GetZaxis()->GetTitle());
	string sigmaRms = (doGaussFit)?"Sigma.":"RMS.";
	zAxisTitle = (drawMean)?"Mean."+zAxisTitle:sigmaRms+zAxisTitle;

	h2->GetZaxis()->SetTitle(zAxisTitle.c_str());
	if (zMin < zMax) h2->GetZaxis()->SetRangeUser(zMin, zMax);

	if ( drawOpt ) { h2->Draw("colz"); }

	return h2;
}

void ShiftHisto(TH2D* h1)
{
	Int_t nBinsX = h1->GetNbinsX();
	Int_t nBinsY = h1->GetNbinsY();
	Double_t value = 0;

	for (Int_t x = 1; x <= nBinsX; x++) {
		for (Int_t y = 1; y <= nBinsY; y++) {
		value = h1->GetBinContent(x, y);
//		h1->SetBinContent(x, y, TMath::Abs(1-value) );
		if ( value > 0 ) {
			if ( flagHisto == 2 ) { h1->SetBinContent(x, y, TMath::Abs(1-value) ); }
			if ( flagHisto == 3 ) { h1->SetBinContent(x, y, TMath::Abs(2.5-value) ); }
			}
		}
	}
}

