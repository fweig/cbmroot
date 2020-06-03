void drawAlHistograms(TString filename="../output/sum_WLS_off.root", TString picturesFolder="pictures")
{
   TFile f(filename);

   TIter iter(f.GetListOfKeys());
   TKey* key;
   TObject* obj;

   TCanvas* c;
   c = new TCanvas("canvas", "", 1000, 800);

   TString canvasTitle;
   TString outFileName;

   while (key = (TKey*)iter())
   {
      obj = key->ReadObj();

      if (obj->InheritsFrom("TH1")) {
         TH1D* histo1 = (TH1D*)obj;

         histo1->Rebin(10); //TODO tune this

         TString histoName = histo1->GetName();

         printf ("%s\n", histoName.Data());
         canvasTitle.Form("canvas_%s", histoName.Data());
         c->SetTitle(canvasTitle);

         histo1->Draw();

         outFileName.Form("%s/%s.png", picturesFolder.Data(), canvasTitle.Data());
         c->SaveAs(outFileName);

      }
   }

   f.Close();
}
