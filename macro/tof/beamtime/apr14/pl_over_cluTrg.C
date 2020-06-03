void pl_over_cluTrg(Int_t iTrg=0, Int_t SmT=0, Int_t iSm=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  can->Divide(2,4); 

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1 *h;
  TH2 *h2;

can->cd(1);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Trg%02d_Size",SmT,iSm,iTrg);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(2);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Trg%02d_Pos",SmT,iSm,iTrg);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else { cout << hname << " not found" << endl; }

can->cd(3);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Trg%02d_TOff",SmT,iSm,iTrg);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else { cout << hname << " not found" << endl; }

can->cd(4);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Trg%02d_Tot",SmT,iSm,iTrg);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }

can->cd(5);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Trg%02d_AvWalk",SmT,iSm,iTrg);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }

can->cd(6);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Trg%02d_Mul",SmT,iSm,iTrg);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
 }else { cout << hname << " not found" << endl;  }

can->cd(7);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Trg%02d_DelTof",SmT,iSm,iTrg);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else { cout << hname << " not found" << endl;  }

can->cd(8);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc000_Trg%02d_dXdY",SmT,iSm,iTrg);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else { cout << hname << " not found" << endl;  }

}
