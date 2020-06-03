void FitExpo(TString filename="sum_WLS_on_G1.root", TString histoName="ExponentaBudetTut")
{
	TFile* f1 = new TFile(filename);

	TH1D* histo1 = (TH1D*)f1->Get(histoName);

	TH1D* histo2 = histo1->Clone();

	histo1->GetXaxis()->SetRangeUser(1.5, 5.0);
	histo1->Fit("expo");

	histo2->GetXAxis()->SetRangeUser(5.0, 15.0);
	histo2->Fit("expo");

	TCanvas *c1 = new TCanvas("c1","transparent pad",10,10,1800,1100);


	gStyle->SetOptStat(11111111);
	gPad->SetGrid(1, 1);
	gPad->SetLogy();

	histo1->SetLineColor(kBlue);
	histo1->Draw();
	gPad->Update();
	gPad->Modified();


}
