#define NUMOFLINES 46

void drawSpectra(const char* filename)
{
	// Read the table from the file
	std::ifstream ifs;
	ifs.open(filename, std::ifstream::in);

	Double_t dr[NUMOFLINES][16]; // data rate for each channel

	for (int i=0; i<NUMOFLINES; i++) {
		ifs >> dr[i][0] >> dr[i][1] >> dr[i][2] >> dr[i][3] >> dr[i][4] >> dr[i][5] >> dr[i][6] >> dr[i][7]
			 >> dr[i][8] >> dr[i][9] >> dr[i][10] >> dr[i][11] >> dr[i][12] >> dr[i][13] >> dr[i][14] >> dr[i][15];
	}
/*
	for (int i=0; i<NUMOFLINES; i++) {
		for (j=0; j<16; j++) {
			std::cout << dr[i][j] << "\t";
		}
		std::cout << std::endl;
	}
*/
	// Prepare the histogram
	TString histoName;
	TString histoTitle;

	TH1D* histos[16];
	TH1D* sp[16];

	for (int j=0; j<16; j++) {
		histoName.Form("histo_%d", j);
		histoTitle.Form("histo_%d", j);
		histos[j] = new TH1D(histoName, histoTitle, NUMOFLINES, 0, NUMOFLINES);
		histoName.Form("spectrum_%d", j);
		histoTitle.Form("spectrum_%d", j);
		sp[j] = new TH1D(histoName, histoTitle, NUMOFLINES, 0, NUMOFLINES);
	}

	for (int j=0; j<16; j++) {
		histos[j]->SetBinContent(NUMOFLINES, dr[0][j]);
		for (int i=1; i<NUMOFLINES; i++) {
			histos[j]->SetBinContent(NUMOFLINES-i, dr[i][j]);
			Double_t diff = dr[i][j]-dr[i-1][j];
			if (diff > 0 && diff < 100000)
				sp[j]->SetBinContent(NUMOFLINES-i, diff);

		}
	}

	TCanvas* c[16];
	for (int j=0; j<16; j++) {
		histoName.Form("canvas_%d", j);
		histoTitle.Form("canvas_%d", j);
		c[j] = new TCanvas(histoName, histoTitle);
		c[j]->Divide(1, 2);
		c[j]->cd(1);
		histos[j]->Draw();
		c[j]->cd(2);
		sp[j]->Draw();
	}

}
