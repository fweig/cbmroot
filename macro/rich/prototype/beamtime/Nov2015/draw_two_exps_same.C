void draw_two_exps_same(TString filename1, TString filename2, TString histoName1, TString histoName2, TString outFolder = "pics_comparison")
{
   TFile* f1 = new TFile(filename1);
   TFile* f2 = new TFile(filename2);

   TH1D* h1 = NULL; h1 = (TH1D*)f1->Get(histoName1);
   TH1D* h2 = NULL; h2 = (TH1D*)f2->Get(histoName2);

   if (h1 == NULL || h2 == NULL) {
      printf("Histogram not found.\n");
      return;
   }

   TCanvas *c1 = new TCanvas("c1","transparent pad",10,10,1800,1100);

   gStyle->SetOptStat(11111111);
   gPad->SetGrid(1, 1);
   gPad->SetLogy();

   h1->SetLineColor(kBlue);
   h1->Draw();
   gPad->Update();
   TPaveStats *ps1 = (TPaveStats*)h1->GetListOfFunctions()->FindObject("stats");
   ps1->SetX1NDC(0.15); ps1->SetX2NDC(0.35);
   ps1->SetTextColor(kBlue);
   gPad->Modified();

   h2->SetLineColor(kRed);
   h2->Draw("][sames");
   gPad->Update();
   TPaveStats *ps2 = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
   ps2->SetX1NDC(0.65); ps2->SetX2NDC(0.85);
   ps2->SetTextColor(kRed);
   gPad->Modified();
/*
   TString outFileName;
   outFileName.Form("%s/TwoExps.png", outFolder.Data());
   c1->SaveAs(outFileName);
*/
}
