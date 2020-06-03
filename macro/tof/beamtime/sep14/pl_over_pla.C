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

 TH1 *hPla;
 TH2 *hPla2;
 // if (hPla!=NULL) hPla->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="hXX2";
 hPla2=(TH2 *)gROOT->FindObjectAny(hname);
 if (hPla2!=NULL) {
  hPla2->Draw("colz");
 }else {  cout << hname << " not found" << endl;  }

can->cd(2);
 gROOT->cd();
 TString hname="hYY2";
 hPla2=(TH2 *)gROOT->FindObjectAny(hname);
 if (hPla2!=NULL) {
  hPla2->Draw("colz");
 }else {  cout << hname << " not found" << endl;  }


can->cd(3);
 gROOT->cd();
 TString hname="Cor_SmT2_sm000_rpc000_Ch000_S0_Walk_px";
 hPla1=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla1!=NULL) {
  hPla1->Draw();
 }else  { cout << hname << " not found" << endl; }

can->cd(4);
 gROOT->cd();
 TString hname="Cor_SmT2_sm000_rpc000_Ch000_S1_Walk_px";
 hPla1=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla1!=NULL) {
  hPla1->Draw();
 }else  { cout << hname << " not found" << endl; }

can->cd(5);
 gROOT->cd();
 TString hname="Cor_SmT2_sm001_rpc000_Ch000_S0_Walk_px";
 hPla1=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla1!=NULL) {
  hPla1->Draw();
 }else  { cout << hname << " not found" << endl; }


can->cd(6);
 gROOT->cd();
 TString hname="Cor_SmT2_sm001_rpc000_Ch000_S1_Walk_px";
 hPla1=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla1!=NULL) {
  hPla1->Draw();
 }else  { cout << hname << " not found" << endl; }

}
