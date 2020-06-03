void pl_over_2hit(Int_t iLog=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,1000,800);
  can->Divide(3,4); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);
gStyle->SetOptFit(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);
 // cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

 TH1 *h;
 TH2 *h2;
 Double_t NEvents;
 Double_t NFinalHits;
 Double_t dTRes;
 Double_t dTRMS;

 // if (hPla!=NULL) hPla->Delete();


can->cd(1);
 gROOT->cd();
 TString hname="hDT04DX0_1";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(2);
 gROOT->cd();
 TString hname="hDT04DY0_1";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(3);
 gROOT->cd();
 TString hname="hDT04DT0_1";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }


can->cd(4);
 gROOT->cd();
 TString hname="hDT04DX4_1";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(5);
 gROOT->cd();
 TString hname="hDT04DY4_1";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(6);
 gROOT->cd();
 TString hname="hDT04DT4_1";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }


can->cd(7);
 gROOT->cd();
 TString hname="hDT04DX0_2";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(8);
 gROOT->cd();
 TString hname="hDT04DY0_2";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(9);
 gROOT->cd();
 TString hname="hDT04DT0_2";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }


can->cd(10);
 gROOT->cd();
 TString hname="hDT04DX4_2";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(11);
 gROOT->cd();
 TString hname="hDT04DY4_2";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(12);
 gROOT->cd();
 TString hname="hDT04DT4_2";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

}
