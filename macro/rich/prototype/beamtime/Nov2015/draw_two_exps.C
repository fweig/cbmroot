void draw_two_exps(TString filename1, TString filename2, TString histoName1="ExponentaBudetTut", TString histoName2="ExponentaBudetTut", TString outFolder = "pictures")
{
   TFile* f1 = new TFile(filename1);
   TFile* f2 = new TFile(filename2);

   TH1D* h1 = NULL; h1 = (TH1D*)f1->Get(histoName1);
   TH1D* h2 = NULL; h2 = (TH1D*)f2->Get(histoName2);

   if (h1 == NULL || h2 == NULL) {
      printf("Histogram not found.\n");
      return;
   }

   TH1D* hdiff1 = h2->Clone();
   TH1D* hdiff2 = h2->Clone();

   // Original

   h1->SetTitle("DeltaT spectra");

   TCanvas *c1 = new TCanvas("c1","original distributions",10,10,1800,1000);
   gStyle->SetOptStat(11111111);
   gPad->SetGrid(1, 1);
   gPad->SetLogy();

   h1->SetLineColor(kBlue);
   h1->Rebin(5);
   h1->GetXaxis()->SetRangeUser(0.0, 30.0);
   h1->GetYaxis()->SetRangeUser(0.1, 60000);
   h1->Draw();
   gPad->Update();
   TPaveStats *ps1 = (TPaveStats*)h1->GetListOfFunctions()->FindObject("stats");
   ps1->SetX1NDC(0.45); ps1->SetX2NDC(0.65);
   ps1->SetTextColor(kBlue);
   gPad->Modified();

   h2->SetLineColor(kRed);
   h2->Rebin(5);
   h2->GetXaxis()->SetRangeUser(0.0, 30.0);
   h2->GetYaxis()->SetRangeUser(0.1, 60000);
   h2->Draw("][sames");
   gPad->Update();
   TPaveStats *ps2 = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
   ps2->SetX1NDC(0.65); ps2->SetX2NDC(0.85);
   ps2->SetTextColor(kRed);
   gPad->Modified();

   // Diff and fit

   hdiff1->SetTitle("DeltaT spectra diff and fit");

   TCanvas *c2 = new TCanvas("c2","WLS_on minus WLS_off",10,10,1800,1000);
   gStyle->SetOptStat(11111111);
   gPad->SetGrid(1, 1);
   gPad->SetLogy();
   hdiff1->Rebin(5);
   hdiff1->Add(h1, -1.);
   hdiff1->GetXaxis()->SetRangeUser(1.2, 5.0);
   hdiff1->GetYaxis()->SetRangeUser(0.1, 60000);
   hdiff1->Draw();
   hdiff1->Fit("expo");

//   TCanvas *c3 = new TCanvas("c3","WLS_on minus WLS_off",1260,10,600,600);
//   gStyle->SetOptStat(11111111);
//   gPad->SetGrid(1, 1);
//   gPad->SetLogy();
   hdiff2->Rebin(5);
   hdiff2->Add(h1, -1.);
   hdiff2->GetXaxis()->SetRangeUser(5.0, 15.0);
   hdiff2->GetYaxis()->SetRangeUser(0.1, 60000);
   hdiff2->Draw("][sames");
   hdiff2->Fit("expo");

   hdiff1->GetXaxis()->SetRangeUser(0.0, 30.0);
   hdiff2->GetXaxis()->SetRangeUser(0.0, 30.0);


   TString outFileName;
   outFileName.Form("%s/Original.png", outFolder.Data());
   c1->SaveAs(outFileName);
   outFileName.Form("%s/Diff_fit.png", outFolder.Data());
   c2->SaveAs(outFileName);

}
