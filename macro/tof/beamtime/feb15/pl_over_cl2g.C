{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can23","can23",48,55,700,900);
  can->Divide(2,4); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(2);

 TH1 *h;
 TH2 *h2;
 // if (h!=NULL) h->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="cl_SmT2_sm002_rpc000_Pos";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  TProfile *h2_pfx =h2->ProfileX();
  h2_pfx->Draw("same");
  Int_t NBin=h2_pfx->GetNbinsX();
  Double_t YMean=h2_pfx->GetBinContent(1);
  Double_t YErr=h2_pfx->GetBinError(1);
  cout << " Number of bins "<< NBin<<", Mean Y "<<YMean<<", Error "<<YErr<<endl;

 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
 gROOT->cd();
 TString hname="cl_SmT2_sm003_rpc000_Pos";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
 gROOT->cd();
 TString hname="cl_SmT2_sm002_rpc000_TOff";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(4);
 gROOT->cd();
 TString hname="cl_SmT2_sm003_rpc000_TOff";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else 
   {
     cout << hname << " not found" << endl;
   }


can->cd(5);
 gROOT->cd();
 TString hname="cl_SmT2_sm002_rpc000_Tot";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(6);
 gROOT->cd();
 TString hname="cl_SmT2_sm003_rpc000_Tot";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(7);
 gROOT->cd();
 TString hname="cl_SmT2_sm002_rpc000_Ch000_Walk";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2prof=h2->ProfileX();
  h2prof->Draw("same");
 }else {cout << hname << " not found" << endl;}

 TString hname="cl_SmT2_sm002_rpc000_Ch000_Walk_px";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("same");
 }else {cout << hname << " not found" << endl;}

can->cd(8);
 gROOT->cd();
 TString hname="cl_SmT2_sm003_rpc000_Ch000_Walk";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2prof=h2->ProfileX();
  h2prof->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

 TString hname="cl_SmT2_sm003_rpc000_Ch000_Walk_px";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("same");
 }else {cout << hname << " not found" << endl;}

}
