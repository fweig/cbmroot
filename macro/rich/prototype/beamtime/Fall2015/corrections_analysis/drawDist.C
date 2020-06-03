void drawDist(TString filename)
{
	FILE* f = fopen(filename.Data(), "r");

	if (f) {
		TCanvas* c1 = new TCanvas("c1", "c1", 20, 50, 900, 600);
		gStyle->SetOptStat(111111);

		TH1D* histo = new TH1D("Delay for all pairs", "Mean delay distribution for all pairs after correction;ps;Entries", 100, -50, 50);
		Double_t curValue;
		char buf[32];
		while (fscanf(f, "%s", buf) == 1) {
			curValue = atof(buf);
			histo->Fill(curValue*1000.);
//			printf ("%f\n", curValue);
		}
		fclose(f);

		histo->SetLineWidth(2);
		gPad->SetGrid(1, 1);
		histo->Draw();
	}

}
