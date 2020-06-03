{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  // Clusterizer diagnostics 
  TCanvas *can = new TCanvas("can23","can23",48,55,700,900);
  can->Divide(2,3); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);

 TH1 *h;
 TH2 *h2;
 // if (h!=NULL) h->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="Clus_NbDigiPerChan";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }
 gPad->SetLogy();

can->cd(2);
 gROOT->cd();
 TString hname="Clus_NbSameSide";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }
 gPad->SetLogy();

can->cd(3);


can->cd(4);


can->cd(5);

can->cd(6);

}
