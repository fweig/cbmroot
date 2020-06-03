pl_TIS(Double_t dFracMax=0.1){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",48,55,700,700);
  can->Divide(2,3); 

 gPad->SetFillColor(0);
 gStyle->SetPalette(1);
 gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);
 // cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

 TH1 *h;
 TH1 *h1;
 TH2 *h2;
 // if (hPla!=NULL) hPla->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="tof_trb_time_in_spill";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("");
 }else  { cout << hname << " not found" << endl; }

 TString hname="TIS_all";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("same");
  h1->SetLineColor(3);
  h1->GetXaxis()->SetTitle("time [10ns]");
 }else  { cout << hname << " not found" << endl; }
 TH1 *hTIS_all = (TH1 *)h1->Clone();

can->cd(2);
 gROOT->cd();
 TString hname="TIS_sel";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
   h1->Draw();
   h1->GetXaxis()->SetTitle("time [10ns]");
 }else  { cout << hname << " not found" << endl; }
 TH1 *hTIS_sel = (TH1 *)h1->Clone();

 TString hname="TIS_sel1";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
   h1->Draw("same");
   h1->SetLineColor(2); 
 }else  { cout << hname << " not found" << endl; }
 TH1 *hTIS_sel1 = (TH1 *)h1->Clone();

 TString hname="TIS_sel2";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
   h1->Draw("same");
   h1->SetLineColor(7); 
 }else  { cout << hname << " not found" << endl; }
 TH1 *hTIS_sel2 = (TH1 *)h1->Clone();

can->cd(3);
 gROOT->cd();
 TH1F *hTISselfrac = (TH1 *)hTIS_all->Clone();
 hTISselfrac->SetName("hTISselfrac");
 hTISselfrac->SetTitle("MRef - selector probability");
 hTISselfrac->Divide(hTIS_sel, hTIS_all, 1., 1., "B");
 hTISselfrac->SetMaximum(dFracMax);
 hTISselfrac->SetMinimum(0.0001);
 hTISselfrac->Draw();
 hTISselfrac->SetLineColor(hTIS_sel->GetLineColor()); 

 TH1F *hTISsel1frac = (TH1 *)hTIS_all->Clone();
 hTISsel1frac->SetName("hTISsel1frac");
 hTISsel1frac->SetTitle("Dut & MRef coinicidence probability");
 hTISsel1frac->Divide(hTIS_sel1, hTIS_all, 1., 1., "B");
 hTISsel1frac->Draw("same");
 hTISsel1frac->SetLineColor(hTIS_sel1->GetLineColor()); 

 // gPad->SetLogy();
can->cd(4);
 TH1F *hselsel1frac = (TH1 *)hTIS_sel->Clone();
 hselsel1frac->SetName("hselsel1frac");
 hselsel1frac->SetTitle("Relative efficiency of DUT");
 hselsel1frac->Divide(hTIS_sel1, hTIS_sel, 1., 1., "B");
 hselsel1frac->Draw("");
 hselsel1frac->SetLineColor(hTIS_sel1->GetLineColor()); 

can->cd(5);
 gROOT->cd();
 TString hname="hTISDT04D4best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  TProfile *h2pfx=h2->ProfileX("hTISDT04D4best_pfs",1,-1,"s");
 }else  { cout << hname << " not found" << endl; }

can->cd(6);
 h2pfx->Draw();
 TH1D *h2pfxes=h2pfx->ProjectionX("hTISDT04D4best_RMS","C=E");
 h2pfxes->Draw("");
 h2pfxes->GetYaxis()->SetTitle("RMS(#Deltat) [ps]");
}

