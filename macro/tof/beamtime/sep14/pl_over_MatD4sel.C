{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,4); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);
gStyle->SetOptFit(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);
 cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

 TH1 *h;
 TH2 *h2; 
 TH2F *h2f;

 Double_t NEvents;
 Double_t NFinalHits;
 Double_t dTRes;
 Double_t dTRMS;

 // if (hPla!=NULL) hPla->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="hDTD4sel";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  NEvents=h1->GetEntries();
 }else  { cout << hname << " not found" << endl; }

can->cd(2);
 gROOT->cd();
 TString hname="hD4XY";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(3);
 gROOT->cd();
 TString hname="hD4Tof";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(4);
 gROOT->cd();
 TString hname="hD4CluSize";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(5);
 TString hname="hD4Chi2DX";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(6);
 TString hname="hD4Chi2DY";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(7);
 TString hname="hD4Chi2DTof";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(8);
 TString hname="hD4DTofDia";
 h2f=(TH2F *)gROOT->FindObjectAny(hname);
 if (h2f!=NULL) {
  h2f->Draw("colz");
  h2f->FitSlicesY();
 }else  { cout << hname << " not found" << endl; }

can->cd(11);

can->cd(9);
 TString hname="hD4XYsel";
 h2=(TH2F *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(10);


 //report summary

}
