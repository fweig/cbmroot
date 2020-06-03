#define THRESHOLD 35.

#define KOEF 2 // Rebinning koef

void drawNonShityHistos(TString filename="../output/sum_WLS_off.root", TString picturesFolder="pictures")
{
   TH1D* hDevi = new TH1D("hDevi", "hDevi", 400, -2., 2.);

   TCanvas* c = new TCanvas("canvas", "", 1000, 800);
   TString canvasTitle;
   TString outFileName;

   TFile f(filename);


   TIter iter(f.GetListOfKeys());
   TKey* key;
   TObject* obj;






   // Loop over the histograms of the file
   while (key = (TKey*)iter())
   {
      obj = key->ReadObj();

      // Work only with 1D histograms
      if (obj->InheritsFrom("TH1")) {
         TH1D* histo1 = (TH1D*)obj;

         // Work only with LeadingEdgeDiff histograms
         TString objName(histo1->GetName());
         if (!(objName.Contains("LeadingEdgeDiff")))
            continue;

         histo1->Rebin(KOEF); //TODO tune this

         //FIXME - use it instead of fixed number 6000
         //Int_t numberOfBins = histo1->GetNbinsX(); // Get the number of bins in the current histo after reinning (should be 6000/KOEF)

         // Find global maximum
         Int_t glMaxBin = histo1->GetMaximumBin();
         Double_t glMax = histo1->GetMaximum();

         // One bin has size of (150/K) ns

         // Try to find the second peak
         Int_t maxInWin1bin = glMaxBin - (2*150/KOEF);
         Int_t maxInWin2bin = glMaxBin + (150/KOEF);
         Double_t maxInWin1 = histo1->GetBinContent(maxInWin1bin);
         Double_t maxInWin2 = histo1->GetBinContent(maxInWin2bin);

         // Window 1 - from -3.0ns to -1.5ns
         for (UInt_t i=glMaxBin - (2*150/KOEF) + 1; i<glMaxBin - (150/KOEF); i++) {
            if (histo1->GetBinContent(i) > maxInWin1) {
               maxInWin1bin = i;
               maxInWin1 = histo1->GetBinContent(maxInWin1bin);
            }
         }

         // Window 2 - from 1.5ns to 3.0ns
         for (UInt_t i=glMaxBin + (150/KOEF) + 1; i<glMaxBin + (2*150/KOEF); i++) {
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

         TString histoName = histo1->GetName();

         if (TMath::IsNaN(maxPercnt)) {
//            printf("%s\tcan not compute\n", histoName.Data());
            continue;
         }

         //TODO choose > - shity or <= - nonShity
         if (maxPercnt > THRESHOLD) {

            Double_t devia = ((Double_t)glMaxBin*KOEF/100. - 30.) - histo1->GetMean();
            printf ("%s\t%d\t%.6f\t%.6f\t%.6f\n",
                    histoName.Data(), glMaxBin, histo1->GetMean(), ((Double_t)glMaxBin*KOEF/100. - 30.), devia);
            hDevi->Fill(devia);
/*
            canvasTitle.Form("canvas_%s", histoName.Data());
            c->SetTitle(canvasTitle);
            histo1->Draw();
            outFileName.Form("%s/%s.png", picturesFolder.Data(), canvasTitle.Data());
            c->SaveAs(outFileName);
*/
         }

      }
   }

   f.Close();

   c->SetTitle("Deviation");
   hDevi->Draw();

}
