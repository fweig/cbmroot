void integrate(TString filename1, TString filename2, TString histoName1, TString histoName2)
{

   TFile* f1 = new TFile(filename1);
   TFile* f2 = new TFile(filename2);

   TH1D* h1 = NULL; h1 = (TH1D*)f1->Get(histoName1);
   TH1D* h2 = NULL; h2 = (TH1D*)f2->Get(histoName2);

   if (h1 == NULL || h2 == NULL) {
      printf("Histogram not found.\n");
      return;
   }

   TH1D* hInteg1 = new TH1D("hInteg1", "hInteg1", 10000, -50., 50.);

   
}
