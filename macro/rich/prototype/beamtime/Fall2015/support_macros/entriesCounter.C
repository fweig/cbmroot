void entriesCounter(TString filename="../output/sum_WLS_off.root")
{
   TFile f(filename, "UPDATE");

   TIter iter(f.GetListOfKeys());
   TKey* key;
   TObject* obj;

   UInt_t totalCounter=0;
   UInt_t counter=0;

   std::vector<TString> listToDelete;

   while (key = (TKey*)iter())
   {
      obj = key->ReadObj();

      if (obj->InheritsFrom("TH1")) {
         TH1D* histo1 = (TH1D*)obj;
         UInt_t num = histo1->GetEntries();
         totalCounter++;
         if (num > 100) {
            //printf("%s: %d\n", histo1->GetName(), num);
            ;
         } else {
            TString nameToDelete;
            nameToDelete.Form("%s;1", histo1->GetName());
            listToDelete.push_back(nameToDelete);
            counter++;
         }
      }
      /*else if (obj->InheritsFrom("TH2")) {
         TH2D* histo2 = (TH2D*)obj;
         printf("%s: %d\n", histo2->GetName(), histo2->GetEntries());
      }*/
   }

   printf("Total %d histograms. %d histograms have less than 101 entries.\nDo you want to delete them?\n", totalCounter, counter);
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
