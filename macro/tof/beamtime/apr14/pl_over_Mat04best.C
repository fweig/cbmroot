{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,3); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);
 cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

 TH1 *h;
 TH2 *h2;
 Double_t NEvents;
 Double_t NFinalHits;
 Double_t dTRes;
 Double_t dTRMS;

 // if (hPla!=NULL) hPla->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="hDXDY04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else {  cout << hname << " not found" << endl;  }

can->cd(2);
 gROOT->cd();
 TString hname="hDXDT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(3);
 gROOT->cd();
 TString hname="hDYDT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(4);
 gROOT->cd();
 TString hname="hDT24DT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  TH1D *hDT04=h2->ProjectionY();
 }else  { cout << hname << " not found" << endl; }


can->cd(5);
 gROOT->cd();
 TString hname="hChi04best";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  NFinalHits=h1->GetEntries();
 }else  { cout << hname << " not found" << endl; }

can->cd(6);
 gROOT->cd();
 TString hname="hNMatch24sel";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
  NEvents=h1->GetEntries();
 }else  { cout << hname << " not found" << endl; }

can->cd(7);
 gROOT->cd();
 TFitResultPtr fRes=hDT04->Fit("gaus","S");
 // fRes->Print();
 dTRes=fRes->Parameter(2);
 dTRMS=hDT04->GetRMS();
can->cd(8);
 gROOT->cd();
 TString hname="hX0DT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  //  TH1D *hDT04=h2->ProjectionY();
 }else  { cout << hname << " not found" << endl; }

can->cd(9);
 gROOT->cd();
 TString hname="hY0DT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  //  TH1D *hDT04=h2->ProjectionY();
 }else  { cout << hname << " not found" << endl; }
 Double_t dEff=NFinalHits/NEvents;
 cout << Form("<I> Efficiency: %6.3f, 1-sigma timing resolution: %6.1f ps, RMS: %6.1f ps",dEff,dTRes,dTRMS) <<endl; 
}
