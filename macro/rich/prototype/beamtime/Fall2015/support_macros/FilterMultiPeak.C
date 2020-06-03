#define THRESHOLD 50.

void FilterMultiPeak(TString filename="output/sum_WLS_off.root")
{
   TFile f(filename, "UPDATE");

   TIter iter(f.GetListOfKeys());
   TKey* key;
   TObject* obj;

   UInt_t totalCounter=0;
   UInt_t counter=0;

   std::vector<TString> listToDelete;

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

         Double_t num = histo1->GetEntries(); // Get the number of entries in the histo
         totalCounter++; // Count how many histograms are there in the file

         histo1->Rebin(10); // Make the bin side 100ps (from 10ps)

         Int_t numberOfBins = histo1->GetNbinsX(); // Get the number of bins in the current histo after reinning (should be 600)

         // Find global maximum
         Int_t glMaxBin = histo1->GetMaximumBin();
         Double_t glMax = histo1->GetMaximum();

         // Try to find the second peak
         Int_t maxInWin1bin = glMaxBin-30;
         Int_t maxInWin2bin = glMaxBin+15;
         Double_t maxInWin1 = histo1->GetBinContent(maxInWin1bin);
         Double_t maxInWin2 = histo1->GetBinContent(maxInWin2bin);

         // Window 1 - from -3.0ns to -1.5ns
         for (UInt_t i=glMaxBin-29; i<glMaxBin-15; i++) {
            if (histo1->GetBinContent(i) > maxInWin1) {
               maxInWin1bin = i;
               maxInWin1 = histo1->GetBinContent(maxInWin1bin);
            }
         }

         // Window 2 - from 1.5ns to 3.0ns
         for (UInt_t i=glMaxBin+16; i<glMaxBin+30; i++) {
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

         // Yes, there is
         if (maxPercnt > THRESHOLD) {
            printf("%s:\t%03d\t-\t%03.0f\t\t%03d\t-\t%03.0f\t\t%03d\t-\t%03.0f\t\t%.0f%%\t%d\n", histo1->GetName(), glMaxBin, glMax, maxInWin1bin, maxInWin1, maxInWin2bin, maxInWin2, maxPercnt, IsRightPeak);
         } else {

            // For correct distributions - print name, maximum position and mean value. They should be close.
            printf("%s\t%d\t%0.8f\n", objName, glMaxBin, histo1->GetMean());

            TString nameToDelete;
            nameToDelete.Form("%s;1", histo1->GetName());
            listToDelete.push_back(nameToDelete);
            counter++;
         }

      }

   }

   printf("Total %d histograms. %d histograms have correct one-peak structure.\nDo you want to delete them?\n", totalCounter, counter);
   if (getchar() == 'y') {
      std::vector<TString>::iterator veciter;
      for (veciter=listToDelete.begin(); veciter!=listToDelete.end(); ++veciter) {
         f.Delete((*veciter).Data());
         printf("%s\n", (*veciter).Data());
      }
      f.Close();
      printf("Removed %d histograms\n", counter);
   } else {
      f.Close();
   }

}
