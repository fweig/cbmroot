// calib = 0 - see results before calibration
// calib = 1 - see results after calibration
// ind = [0;15] - index of the canvas to plot

void drawAllHisto(TString filename="Wuppertal_analysis.histo.root", Int_t ind=0, Int_t calib=1)
{
	TFile* f = new TFile(filename);

	//f->ls();

	TCanvas* c[16];
	TString canvasName;
	TString histoName;
	TH1D* histo;

	//Int_t i = ind;
	for (Int_t i=0; i<16; i++)
	{

		canvasName.Form("canvas_%d", i);
		c[i] = new TCanvas(canvasName, canvasName, 10, 10, 1240, 940);
		c[i]->Divide(4, 4);

		for (Int_t j=0; j<4; j++) {
			for (Int_t k=0; k<4; k++) {
				c[i]->cd(j*4+k+1);
				histoName.Form("LeadingEdgeDiff_%d_%d;%d", i, j*4+k, calib+1);
				histo = (TH1D*)f->Get(histoName);
				histo->Draw();
			}
		}

		TString outFileName;
		outFileName.Form("canvas_%d.png", i);
		c[i]->SaveAs(outFileName);

	}

}
