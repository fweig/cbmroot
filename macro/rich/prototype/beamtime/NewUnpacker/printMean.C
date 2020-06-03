// calib = 0 - see results before calibration
// calib = 1 - see results after calibration

void printMean(TString filename="Wuppertal_analysis.histo.root", Int_t calib=1)
{
	TFile* f = new TFile(filename);

	//f->ls();

	TCanvas* c[16];
	TString canvasName;
	TString histoName;
	TH1D* histo;

	for (Int_t i=0; i<16; i++)
	{

		for (Int_t j=0; j<4; j++) {
			for (Int_t k=0; k<4; k++) {
				histoName.Form("LeadingEdgeDiff_%d_%d;%d", i, j*4+k, calib+1);
				histo = (TH1D*)f->Get(histoName);
				printf ("%d\t%d:\t%f\n", i, j*4+k, histo->GetMean());
			}
		}

	}

}
