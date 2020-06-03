void draw_overlap(TString filename1, TString filename2, TString tdc1N = "50", TString tdc2N = "50", Int_t ch1 = 0, Int_t ch2 = 2, TString outFolder = "pics_comparison")
{
   TFile* f1 = new TFile(filename1);
   TFile* f2 = new TFile(filename2);

   TString histoName;
   histoName.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d;1", tdc1N.Data(), ch1, tdc2N.Data(), ch2);

   printf("%s\n", histoName.Data());

   TH1D* h1 = NULL; h1 = (TH1D*)f1->Get(histoName);
   TH1D* h2 = NULL; h2 = (TH1D*)f2->Get(histoName);

   if (h1 == NULL || h2 == NULL) {
      printf("Histogram not found.\n");
      return;
   }

   if (h1->GetEntries() < 1000 || h2->GetEntries() < 1000) {
      printf("Less than 1000 entries in one of histograms (or both). Skipping.\n");
      return;
   }

   TCanvas *c1 = new TCanvas("c1","transparent pad",10,10,1800,1100);

   gStyle->SetOptStat(11111111);
   gPad->SetGrid(1, 1);

   h1->SetLineColor(kBlue);
   h1->Rebin(8);
   h1->Draw();
   gPad->Update();
   TPaveStats *ps1 = (TPaveStats*)h1->GetListOfFunctions()->FindObject("stats");
   ps1->SetX1NDC(0.15); ps1->SetX2NDC(0.35);
   ps1->SetTextColor(kBlue);
   gPad->Modified();

   h2->SetLineColor(kRed);
   h2->Rebin(8);
   h2->Draw("][sames");
   gPad->Update();
   TPaveStats *ps2 = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
   ps2->SetX1NDC(0.65); ps2->SetX2NDC(0.85);
   ps2->SetTextColor(kRed);
   gPad->Modified();

   TString outFileName;
   outFileName.Form("%s/canvas_TDC%s_ch%d_TDC%s_ch%d.png", outFolder.Data(), tdc1N.Data(), ch1, tdc2N.Data(), ch2);
   c1->SaveAs(outFileName);
}
