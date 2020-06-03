void draw_hitsPerEvent(TString filename="output_fine_2/output_file_list.txt", TString picturesFolder="pictures_fine_2", TString analysisResultsFilename="output_fine_2/analysisResults.root")
{
	// Open input file
	FILE* inFile = fopen(filename.Data(), "r");
	if (!inFile) {
		printf("Failed to open file %s. Abort.\n", filename.Data());
		return 1;
	}

	int getLineRes;
	char curRootFilename[255];

	TString curRootFile;
	TString histoName("fTotalNumOfLightHitsInPixel");
	TString outFileName;
	TString canvasName("c1");
	TString canvasTitle("c1");

	TCanvas* c1 = new TCanvas(canvasName.Data(), canvasTitle.Data(), 40, 40, 1800, 900);
	gStyle->SetOptStat(11);
//	c1->Divide(4, 3);

   TH1D* histoNorm = new TH1D("histoNorm", "histoNorm", 100, 0., 100.);

   TH1D* histoPixel[64];
   for (UInt_t i=1; i<=128; i+=2) {
      TString outHistoName = Form("histoPixel%d", (i+1)/2);
      TString outHistoTitle = Form("histoPixel%d", (i+1)/2);
      histoPixel[i/2] = new TH1D(outHistoName.Data(), outHistoTitle.Data(), 100, 0., 100.);
   }

   TH1* histo[100]; //FIXME number of histograms is unknown - set 100 as possible limit

	UInt_t canvPos = 1;
	while (fscanf(inFile, "%s", curRootFilename) == 1) {
//		c1->cd(canvPos);

		gPad->SetGrid(1, 1);
		//gPad->SetLogy();

		curRootFile.Form("output_fine_2/%s", curRootFilename);
		TFile* f1 = new TFile(curRootFile);
		histo[canvPos-1] = (TH1*)f1->Get(histoName);

      Double_t normFactor = histo[canvPos-1]->GetBinContent(257);
      //printf ("%0.8f\n", normFactor);
      histoNorm->SetBinContent(canvPos, normFactor);
      for (UInt_t i=1; i<=128; i+=2) {
         histoPixel[i/2]->SetBinContent(canvPos, histo[canvPos-1]->GetBinContent(i+1));
      }

		histo[canvPos-1]->SetTitle(curRootFilename);
		histo[canvPos-1]->SetLineWidth(2);
		histo[canvPos-1]->GetXaxis()->SetRangeUser(0, 256);
//		histo[canvPos-1]->GetYaxis()->SetRangeUser(0, 250000);
		histo[canvPos-1]->Draw();

		outFileName.Form("%s/hitsPerEvent_%d.png", picturesFolder.Data(), canvPos);
		c1->SaveAs(outFileName);

		canvPos++;
	}

   for (UInt_t i=1; i<=128; i+=2) {
      histoPixel[i/2]->Draw();
   	outFileName.Form("%s/histoPixel%d.png", picturesFolder.Data(), (i+1)/2);
   	c1->SaveAs(outFileName);
   }

   histoNorm->Draw();
	outFileName.Form("%s/histoNorm.png", picturesFolder.Data());
	c1->SaveAs(outFileName);

   TFile* analysisResultsFile = new TFile(analysisResultsFilename, "RECREATE");
   histoNorm->Write();
   for (UInt_t i=1; i<=128; i+=2) {
      histoPixel[i/2]->Write();
   }
   for (UInt_t i=1; i<canvPos; i++) {
      histo[i-1]->Write();
   }
   analysisResultsFile->Close();

}

