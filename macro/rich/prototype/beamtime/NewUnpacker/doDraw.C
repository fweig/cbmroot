void doDraw(int tdc, int ch)
{
	TFile* f = new TFile("output/Wuppertal_analysis.histo.root", "READ");

	TCanvas* c1 = new TCanvas("cDelay", "Delay", 20, 50, 1200, 600);
	c1->Divide(2);

	gStyle->SetOptStat(111111);

	// ----------------------------------------------------------------------------------

	c1->cd(1);

	TH1D* hist1 = (TH1D*)f->Get("fhDelay");

	hist1->SetStats(kTRUE);
	gPad->SetLogy(1);
	gPad->SetGrid(1, 1);
	hist1->SetMinimum(0.1);
	hist1->SetMaximum(1000000);
	hist1->GetXaxis()->SetLabelFont(63);
	hist1->GetXaxis()->SetLabelSize(14); // labels will be 14 pixels
	hist1->GetYaxis()->SetLabelFont(63);
	hist1->GetYaxis()->SetLabelSize(14); // labels will be 14 pixels
	hist1->SetStats(kTRUE);
	hist1->Draw();

	// ----------------------------------------------------------------------------------

	c1->cd(2);

	TString histoName;
	histoName.Form("fhTOT_channel_%d-%d", ch, ch+1);
	TH1D* hist2 = (TH1D*)f->Get(histoName.Data());

	hist2->SetStats(kTRUE);
	gPad->SetLogy(1);
	gPad->SetGrid(1, 1);
	hist2->SetMinimum(0.1);
	hist2->SetMaximum(1000000);
	hist2->GetXaxis()->SetLabelFont(63);
	hist2->GetXaxis()->SetLabelSize(14); // labels will be 14 pixels
	hist2->GetYaxis()->SetLabelFont(63);
	hist2->GetYaxis()->SetLabelSize(14); // labels will be 14 pixels
	hist2->SetStats(kTRUE);
	hist2->Draw();

	// ----------------------------------------------------------------------------------

	TImage *img = TImage::Create();
	img->FromPad(c1);
	TString filename;
	filename.Form("res/tdc001%d_ch%d.png", tdc, ch);
	img->WriteImage(filename.Data());

	std::cout << "tdc " << tdc << " ch " << ch
		  << " Delay_mean " << hist1->GetMean() << " Delay_rms " << hist1->GetRMS()
		  << " ToT_mean " << hist2->GetMean() << " ToT_rms " << hist2->GetRMS()
		  << " Delay_underflow " << hist1->GetBinContent(0) << " Delay_overflow " << hist1->GetBinContent(1001)
		  << " ToT_underflow " << hist2->GetBinContent(0) << " ToT_overflow " << hist2->GetBinContent(801)
		  << std::endl;

}
