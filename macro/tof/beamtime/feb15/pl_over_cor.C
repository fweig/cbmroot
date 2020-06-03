void pl_over_cor(Int_t SmT=3, Int_t iSm=0, Int_t iRpc=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,3); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

// gROOT->cd();
// gROOT->SetDirLevel(1);

 TH1 *h;
 TH2 *h2;
 // if (h!=NULL) h->Delete();
 Int_t iNch=0;

can->cd(1);
// gROOT->cd();
 gDirectory->pwd();

 TString hname=Form("cl_CorSmT%d_sm%03d_rpc%03d_Pos_pfx",SmT,iSm,iRpc);
 h1=(TH1 *)gDirectory->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("colz");
  gPad->SetLogz();
  iNch=h1->GetNbinsX();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
// gROOT->cd();
 TString hname=Form("cl_CorSmT%d_sm%03d_rpc%03d_TOff_pfx",SmT,iSm,iRpc);
 h2=(TH2 *)gDirectory->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
// gROOT->cd();
 TString hname=Form("cl_CorSmT%d_sm%03d_rpc%03d_Tot_Mean",SmT,iSm,iRpc);
 h2=(TH2 *)gDirectory->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw();
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(4);
// gROOT->cd();
 for (Int_t iCh=0; iCh<iNch; iCh++){
   TString hname=Form("Cor_SmT%d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",SmT,iSm,iRpc,iCh);
 h2=(TH2 *)gDirectory->FindObjectAny(hname);
 if (h2!=NULL) {
  if (iCh==0) {
    h2->Draw("");
    h2->SetMinimum(-500.);
    h2->SetMaximum(500.);
  }else{
    h2->Draw("same");
  }
  h2->SetLineColor(iCh+1);
  //  h2->UseCurrentStyle(); 
  //  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }
 }

can->cd(5);
 for (Int_t iCh=0; iCh<iNch; iCh++){
   TString hname=Form("Cor_SmT%d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",SmT,iSm,iRpc,iCh);
 h2=(TH2 *)gDirectory->FindObjectAny(hname);
 if (h2!=NULL) {
  if (iCh==0) {
    h2->Draw("");
    h2->SetMinimum(-500.);
    h2->SetMaximum(500.);
  }else{
    h2->Draw("same");
  }
  h2->SetLineColor(iCh+1);
  //  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }
 }

can->cd(6);
// gROOT->cd();
// TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Mul",SmT,iSm,iRpc);
 TString hname=Form("cl_CorSmT%d_sm%03d_rpc%03d_Tot_Off",SmT,iSm,iRpc);
 h1=(TH1 *)gDirectory->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
 }else { cout << hname << " not found" << endl;  }

can->cd(7);
 Int_t iNSel=4;
 for (Int_t iTr=0; iTr<iNSel; iTr++){
   TString hname=Form("cl_CorSmT%d_sm%03d_rpc%03d_Sel%02d_DelTof",SmT,iSm,iRpc,iTr);
 h2=(TH2 *)gDirectory->FindObjectAny(hname);
 if (h2!=NULL) {
  if (iTr==0) {
    h2->Draw("");
    h2->SetMinimum(-500.);
    h2->SetMaximum(500.);
  }else{
    h2->Draw("same");
  }
  h2->SetLineColor(iTr+1);
  //  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }
 }

}
