{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",48,55,700,700);
  can->Divide(2,2); 

 gPad->SetFillColor(0);
 gStyle->SetPalette(1);
 gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);
 cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

 TH1 *h;
 TH1 *h1;
 TH2 *h2;
 // if (hPla!=NULL) hPla->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="tof_trb_trigger_pattern";
 h1=(TH2 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("");
 }else {  cout << hname << " not found" << endl;  }

can->cd(2);
 gROOT->cd();
 TString hname="tof_trb_trigger_types";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("");
 }else  { cout << hname << " not found" << endl; }

can->cd(3);
 gROOT->cd();
 TString hname="tof_trb_time_in_spill";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(4);
 gROOT->cd();
 TString hname="";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

}
