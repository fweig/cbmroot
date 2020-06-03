void pl_page(Int_t iSel=0, Int_t SmT=0, Int_t iSm=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",48,25,700,800);
  can->Divide(1,1); 
  can->cd(1);
  gPad->SetTopMargin(0.1);
  return;
  gPad->Divide(2,4); 
  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  //  gStyle->SetOptStat(kTRUE);
  //  gStyle->SetOptStat(1111);
  //  gROOT->cd();
  //  gROOT->SetDirLevel(1);

  TH1 *h;
  TH2 *h2;

  return;
can->cd(1);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Sel%02d_Size",SmT,iSm,iSel);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(2);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Sel%02d_Pos",SmT,iSm,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
 }else { cout << hname << " not found" << endl; }

can->cd(3);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Sel%02d_TOff",SmT,iSm,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
  TH1D * h2py=h2->ProjectionY();
  cout << "Average TOff :"<<h2py->GetMean()<<endl;

 }else { cout << hname << " not found" << endl; }

can->cd(4);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Sel%02d_Tot",SmT,iSm,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(5);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Sel%02d_AvWalk",SmT,iSm,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(6);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Sel%02d_Mul",SmT,iSm,iSel);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
 }else { cout << hname << " not found" << endl;  }

can->cd(7);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Sel%02d_DelTof",SmT,iSm,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(8);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Sel%02d_dXdY",SmT,iSm,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }

}
