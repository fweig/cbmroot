// calib = 0 - see results before calibration
// calib = 1 - see results after calibration
// ind = [0;15] - index of the canvas to plot

void drawAllHisto2(TString filename="Wuppertal_analysis.histo.root", Int_t ind=0, Int_t calib=1)
{
	TFile* f = new TFile(filename);

	//f->ls();

	TCanvas* c[64];
	TString canvasName;
	TString histoName;
	TH1D* histo;
	Int_t k=0;
	TString outFileName;

	for (Int_t i=0; i<8; i++) {
		for (Int_t j=i*2+1; j<16; j++) {

			canvasName.Form("canvas_%d", k);
			c[k] = new TCanvas(canvasName, canvasName, 10, 10, 1240, 940);
			c[k]->Divide(2, 2);

			c[k]->cd(1);
			histoName.Form("LeadingEdgeDiff_%d_%d;%d", i*2, j, calib+1);
			histo = (TH1D*)f->Get(histoName);
			histo->Draw();

			c[k]->cd(3);
			histoName.Form("LeadingEdgeDiff_%d_%d;%d", j, i*2, calib+1);
			histo = (TH1D*)f->Get(histoName);
			histo->Draw();

			c[k]->cd(2);
			histoName.Form("LeadingEdgeDiff_%d_%d;%d", i*2+1, j, calib+1);
			histo = (TH1D*)f->Get(histoName);
			histo->Draw();

			c[k]->cd(4);
			histoName.Form("LeadingEdgeDiff_%d_%d;%d", j, i*2+1, calib+1);
			histo = (TH1D*)f->Get(histoName);
			histo->Draw();

			//printf ("canvas_%d:\t%d-%d\t%d-%d\t%d-%d\t%d-%d\n", k, i*2, j, j, i*2, i*2+1, j, j, i*2+1);

			outFileName.Form("canvas_%d.png", k);
			c[k]->SaveAs(outFileName);

			k++;
		}
	}

}
