void pl_over_clu(Int_t SmT=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",48,55,700,900);
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
 TString hname=Form("cl_SmT%d_sm000_rpc000_Size",SmT);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("colz");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm000_rpc000_Pos",SmT);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm000_rpc000_TOff",SmT);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(4);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm000_rpc000_Tot",SmT);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else { cout << hname << " not found" << endl;  }

can->cd(5);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm000_rpc000_AvWalk",SmT);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else { cout << hname << " not found" << endl;  }

can->cd(6);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm000_rpc000_Mul",SmT);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
 }else { cout << hname << " not found" << endl;  }

}
