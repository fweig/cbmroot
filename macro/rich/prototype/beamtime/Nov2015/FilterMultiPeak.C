#define THRESHOLD 45. // Check the (un)commented code at the end of the macro
#define KOEF 5 // Rebinning koef
#define WINMIN 1.5 // ns
#define WINMAX 3.0 // ns

void FilterMultiPeak(TString filename="sum.root", TString picturesFolder="pictures")
{
   TH1D* hDevi1 = new TH1D("hDevi1", "hDevi1", 1000, -10., 10.);
   TH1D* hDevi2 = new TH1D("hDevi2", "hDevi2", 1000, -10., 10.);

   TCanvas* c = new TCanvas("canvas", "", 1600, 1000);
   TString canvasTitle;
   TString outFileName;

   gStyle->SetOptStat(111111111);
   gPad->SetGrid(1, 1);

   TFile f(filename);
   //TFile f(filename, "UPDATE"); // if you want to change the original root file - i.e. remove histograms

   char LINE[256];

   TIter iter(f.GetListOfKeys());
   TKey* key;
   TObject* obj;

   //UInt_t totalCounter=0;
   //UInt_t counter=0;

   //std::vector<TString> listToDelete;

   // Loop over the histograms of the file
   while (key = (TKey*)iter())
   {
      obj = key->ReadObj();

      // Work only with 1D histograms
      if (obj->InheritsFrom("TH1")) {
         TH1D* histo1 = (TH1D*)obj;

         // Work only with LeadingEdgeDiff histograms
         TString histoName = histo1->GetName();
         //TString objName(histo1->GetName());
         if (!(histoName.Contains("LeadingEdgeDiff")))
            continue;

         Double_t num = histo1->GetEntries(); // Get the number of entries in the histo
         if (num == 0)
            continue;

         UInt_t tdc1, ch1, tdc2, ch2;
         strcpy(LINE, histoName.Data());
         sscanf(LINE, "LeadingEdgeDiff_TDC%d_ch%d_TDC%d_ch%d", &tdc1, &ch1, &tdc2, &ch2);

         if ((tdc1==tdc2) && (ch1==ch2))
            continue;

         // Condition for WLS off
         if ( (tdc1==10 && ch1==4) || (tdc2==10 && ch2==4) ||
              (tdc1==11 && ch1==0) || (tdc2==11 && ch2==0) ||
              (tdc1==12 && ch1==0) || (tdc2==12 && ch2==0) ||
              (tdc1==12 && ch1==1) || (tdc2==12 && ch2==1) ||
              (tdc1==12 && ch1==7) || (tdc2==12 && ch2==7) ||
              (tdc1==12 && ch1==8) || (tdc2==12 && ch2==8) ||
              (tdc1==13 && ch1==3) || (tdc2==13 && ch2==3) ||
              (tdc1==13 && ch1==4) || (tdc2==13 && ch2==4) ||
              (tdc1==13 && ch1==10) || (tdc2==13 && ch2==10) ||
              (tdc1==20 && ch1==10) || (tdc2==20 && ch2==10) ||
              (tdc1==20 && ch1==14) || (tdc2==20 && ch2==14) ||
              (tdc1==22 && ch1==0) || (tdc2==22 && ch2==0) ||
              (tdc1==23 && ch1==14) || (tdc2==23 && ch2==14) ||
              (tdc1==52 && ch1==1) || (tdc2==52 && ch2==1) ||
              (tdc1==60 && ch1==1) || (tdc2==60 && ch2==1) ||
              (tdc1==61 && ch1==4) || (tdc2==61 && ch2==4) ||
              (tdc1==61 && ch1==7) || (tdc2==61 && ch2==7) ||
              (tdc1==61 && ch1==8) || (tdc2==61 && ch2==8) ) {
            continue;
         }

         // Condition for WLS on
         if ( tdc1==50 || tdc1==51 || tdc1==52 || tdc1==53 ||
              tdc1==60 || tdc1==61 || tdc1==62 || tdc1==63 ||
              tdc2==50 || tdc2==51 || tdc2==52 || tdc2==53 ||
              tdc2==60 || tdc2==61 || tdc2==62 || tdc2==63 ||
              
              (tdc1==10 && ch1==4) || (tdc2==10 && ch2==4) ||
              (tdc1==11 && ch1==0) || (tdc2==11 && ch2==0) ||
              (tdc1==11 && ch1==4) || (tdc2==11 && ch2==4) ||
              
              (tdc1==13 && ch1==3) || (tdc2==13 && ch2==3) ||
              (tdc1==13 && ch1==6) || (tdc2==13 && ch2==6) ||
              (tdc1==13 && ch1==8) || (tdc2==13 && ch2==8) ||
              (tdc1==13 && ch1==9) || (tdc2==13 && ch2==9) ||
              (tdc1==13 && ch1==10) || (tdc2==13 && ch2==10) ||
              (tdc1==13 && ch1==11) || (tdc2==13 && ch2==11) ||
              (tdc1==13 && ch1==13) || (tdc2==13 && ch2==13) ||
              
              (tdc1==20 && ch1==0) || (tdc2==20 && ch2==0) ||
              (tdc1==20 && ch1==4) || (tdc2==20 && ch2==4) ||
              (tdc1==20 && ch1==7) || (tdc2==20 && ch2==7) ||
              (tdc1==20 && ch1==14) || (tdc2==20 && ch2==14) ||
              (tdc1==20 && ch1==10) || (tdc2==20 && ch2==10) ||
              
              (tdc1==21 && ch1==0) || (tdc2==21 && ch2==0) ||
              (tdc1==21 && ch1==5) || (tdc2==21 && ch2==5) ||
              (tdc1==21 && ch1==7) || (tdc2==21 && ch2==7) ||
              (tdc1==21 && ch1==8) || (tdc2==21 && ch2==8) ||
              (tdc1==21 && ch1==14) || (tdc2==21 && ch2==14) ||
              (tdc1==21 && ch1==15) || (tdc2==21 && ch2==15) ||
              
              (tdc1==22 && ch1==0) || (tdc2==22 && ch2==0) ||
              (tdc1==22 && ch1==4) || (tdc2==22 && ch2==4) ||
              (tdc1==23 && ch1==14) || (tdc2==23 && ch2==14) ) {
            continue;
         }

         histo1->Rebin(KOEF);

         //totalCounter++; // Count how many histograms are there in the file
         
         // Find global maximum
         Int_t glMaxBin = histo1->GetMaximumBin();
         Double_t glMax = histo1->GetMaximum();

/*
         Int_t numberOfBins = histo1->GetNbinsX(); // Get the number of bins in the current histo after reinning (should be 600)
         Double_t binSizeAfterRebinning = histo1->GetBinWidth(1);

         // Try to find the second peak
         Int_t maxInWin1bin = glMaxBin - WINMAX/binSizeAfterRebinning;
         Int_t maxInWin2bin = glMaxBin + WINMIN/binSizeAfterRebinning;
         Double_t maxInWin1 = histo1->GetBinContent(maxInWin1bin);
         Double_t maxInWin2 = histo1->GetBinContent(maxInWin2bin);

         // Window 1 - from -WINMAX to -WINMIN
         for (UInt_t i=glMaxBin - WINMAX/binSizeAfterRebinning + 1; i<glMaxBin - WINMIN/binSizeAfterRebinning; i++) {
            if (histo1->GetBinContent(i) > maxInWin1) {
               maxInWin1bin = i;
               maxInWin1 = histo1->GetBinContent(maxInWin1bin);
            }
         }

         // Window 2 - from WINMIN to WINMAX
         for (UInt_t i=glMaxBin + WINMIN/binSizeAfterRebinning + 1; i<glMaxBin + WINMAX/binSizeAfterRebinning; i++) {
            if (histo1->GetBinContent(i) > maxInWin2) {
               maxInWin2bin = i;
               maxInWin2 = histo1->GetBinContent(maxInWin2bin);
            }
         }

         // Check if there is really a second peak
         Double_t maxPercnt;
         Bool_t IsRightPeak;
         if (maxInWin2 > maxInWin1) { maxPercnt = 100.*maxInWin2/glMax; IsRightPeak = kTRUE; }
         else { maxPercnt = 100.*maxInWin1/glMax; IsRightPeak = kFALSE; }

         if (TMath::IsNaN(maxPercnt)) {
//            printf("%s\tcan not compute\n", histoName.Data());
            continue;
         }
*/
         Double_t devia = histo1->GetBinCenter(glMaxBin) - histo1->GetMean();

         // Yes, there is
//         if (maxPercnt > THRESHOLD) {

//            printf("BAD:  %d\t%d\t%d\t%d\t%03d\t-\t%03.0f\t\t%03d\t-\t%03.0f\t\t%03d\t-\t%03.0f\t\t%.0f%%\t%d\n",
//                   tdc1, ch1, tdc2, ch2, glMaxBin, glMax, maxInWin1bin, maxInWin1, maxInWin2bin, maxInWin2, maxPercnt, IsRightPeak);
//            hDevi1->Fill(devia);
/*
            canvasTitle.Form("canvas_%s", histoName.Data());
            c->SetTitle(canvasTitle);
            histo1->Draw();
            outFileName.Form("%s/%s.png", picturesFolder.Data(), canvasTitle.Data());
            c->SaveAs(outFileName);
*/
//         } else {

            hDevi1->Fill(devia);
            if (devia < -1.0 || devia > 1.0) continue;

            // For correct distributions - print name, maximum position and mean value. They should be close.
            printf("GOOD: %d\t%d\t%d\t%d\t%d\t%0.8f\t%0.8f\t%0.8f\t%d\n",
                   tdc1, ch1, tdc2, ch2, glMaxBin, histo1->GetMean(), histo1->GetBinCenter(glMaxBin), devia, num);
            hDevi2->Fill(devia);

//            TString nameToDelete;
//            nameToDelete.Form("%s;1", histo1->GetName());
//            listToDelete.push_back(nameToDelete);
//            counter++;

//         }

      }

   }

//   printf("Total %d histograms. %d histograms have correct one-peak structure.\nDo you want to delete them?\n", totalCounter, counter);
//   if (getchar() == 'y') {
//      std::vector<TString>::iterator veciter;
//      for (veciter=listToDelete.begin(); veciter!=listToDelete.end(); ++veciter) {
//         f.Delete((*veciter).Data());
//         printf("%s\n", (*veciter).Data());
//      }
//      f.Close();
//      printf("Removed %d histograms\n", counter);
//   } else {
      f.Close();
//   }

   c->SetTitle("Deviation1");
   hDevi1->Draw();
   outFileName.Form("%s/Deviation1.png", picturesFolder.Data());
   c->SaveAs(outFileName);

   c->SetTitle("Deviation2");
   hDevi2->Draw();
   outFileName.Form("%s/Deviation2.png", picturesFolder.Data());
   c->SaveAs(outFileName);

}
