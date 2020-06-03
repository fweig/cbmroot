void pl_DigiCor(Int_t SmT=1, Int_t iSm=0, Int_t iRpc=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,3); 

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
 TString hname=Form("tof_ty%d_sm%03d_rpc%03d_ch_evt_mul",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  //  h2->ProfileX()->Draw("same");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
 gROOT->cd();
 TString hname=Form("tof_ty%d_sm%03d_rpc%03d_ch_tot_mul",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
 gROOT->cd();
 TString hname=Form("tof_ty%d_sm%03d_rpc%03d_ch_left_hitsperevt",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(4);
 gROOT->cd();
 TString hname=Form("tof_ty%d_sm%03d_rpc%03d_ch_right_hitsperevt",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(5);
 gROOT->cd();
 TString hname=Form("tof_ty%d_sm%03d_rpc%03d_coinc_sides",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  //h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(6);
 gROOT->cd();
 TString hname=Form("tof_ty%d_sm%03d_rpc%03d_coinc_left",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  //  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(7);
 gROOT->cd();
 TString hname=Form("tof_ty%d_sm%03d_rpc%03d_coinc_right",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }

}
