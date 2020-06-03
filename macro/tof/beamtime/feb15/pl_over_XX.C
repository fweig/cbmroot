{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can23","can23",48,55,700,900);
  can->Divide(2,3); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);
 cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

 TH1 *h;
 TH2 *h2;
 // if (hPla!=NULL) hPla->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="hXX04";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else {  cout << hname << " not found" << endl;  }

can->cd(2);
 gROOT->cd();
 TString hname="hYY04";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(3);
 gROOT->cd();
 TString hname="hXX02_0";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(4);
 gROOT->cd();
 TString hname="hYY02_0";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(5);
 gROOT->cd();
 TString hname="hXX02_1";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }


can->cd(6);
 gROOT->cd();
 TString hname="hYY02_1";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

}
