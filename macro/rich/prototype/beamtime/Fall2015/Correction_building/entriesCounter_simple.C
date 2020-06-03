void entriesCounter_simple(TString filename="alphas.root", TString picturesFolder="pictures")
{
   char LINE[256];

   TH1F* hNum = new TH1F("hNum", "hNum", 300, 0., 300.);
   TH1F* hMean = new TH1F("hMean", "hMean", 600, -30., 30.);

   TFile f(filename);

   TIter iter(f.GetListOfKeys());
   TKey* key;
   TObject* obj;

   UInt_t totalCounter=0;

   TString canvasTitle;
   TString outFileName;
   TCanvas* c;
   c = new TCanvas("canvas", "", 1000, 800);

   gStyle->SetOptStat(111111111);
   gPad->SetGrid(1, 1);

   while (key = (TKey*)iter())
   {
      obj = key->ReadObj();

      if (obj->InheritsFrom("TH1")) {
         TH1D* histo1 = (TH1D*)obj;
         totalCounter++;

         TString histoName = histo1->GetName();
         UInt_t tdc1, ch1, tdc2, ch2;
         strcpy(LINE, histoName.Data());
         sscanf(LINE, "LeadingEdgeDiff_TDC%d_ch%d_TDC%d_ch%d", &tdc1, &ch1, &tdc2, &ch2);
         //printf ("%d/%d - %d/%d\n", tdc1, ch1, tdc2, ch2);

//         if ((tdc1==23 && ch1==14) || (tdc2==23 && ch2==14) ||
//             (tdc1==61 && ch1==8) || (tdc2==61 && ch2==8) ||
//             (tdc1==61 && ch1==4) || (tdc2==61 && ch2==4) ||
//             ((tdc1==tdc2) && (ch1==ch2))) continue;

         Double_t m = histo1->GetMean();
         //printf("%f\t%f\n", m, m);
         if ((m < -1.2 || m > 1.2))
         {
            //printf("%s: %d\t%f\n", histoName.Data(), histo1->GetEntries(), m);
         }


         hNum->Fill(histo1->GetEntries());
         hMean->Fill(m);

         histo1->Rebin(5);

         canvasTitle.Form("canvas_%s", histoName.Data());
         c->SetTitle(canvasTitle);
         histo1->Draw();
         outFileName.Form("%s/%s.png", picturesFolder.Data(), canvasTitle.Data());
         c->SaveAs(outFileName);

      }
   }

   printf("Total %d histograms.\n", totalCounter);
   f.Close();

   //hNum->Draw();
   //hMean->Draw();
}
