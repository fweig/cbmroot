void drawInTime(TString filename, Int_t num)
{
	FILE* f = fopen(filename.Data(), "r");

	if (f) {
		TCanvas* c1 = new TCanvas("c1", "c1", 20, 50, 900, 600);
		
		gStyle->SetOptStat(111111);

		TH1I* histo = new TH1I("histo", "histo;iteration;real events in the buffer", num, 0, num);
		Int_t curValue;
		char buf[32];
		Int_t i = 1;
		while (fscanf(f, "%s", buf) == 1) {
			curValue = atoi(buf);
			histo->SetBinContent(i, curValue);
			//printf ("%d: %d\n", i, curValue);
			i++;
		}

		fclose(f);

		histo->SetLineWidth(2);
		histo->SetStats(kFALSE);
		gPad->SetGrid(1, 1);
		histo->Draw();
	}

}
