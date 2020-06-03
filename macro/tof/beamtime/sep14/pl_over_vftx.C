{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  can->Divide(2,3); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);
 cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

 TH1 *hRaw;
 // if (hRaw!=NULL) hRaw->Delete();
 TH1 *hCal;
 // if (hCal!=NULL) hCal->Delete();
 TH2 *hMap0;
 // if (hMap0!=NULL) hMap0->Delete();
 TH2 *hMap1;
 // if (hMap1!=NULL) hMap1->Delete();
 TH2 *hMap2;
 // if (hMap2!=NULL) hMap2->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="tdc_unp_cnts";
 hRaw=(TH1 *)gROOT->FindObjectAny(hname);
 if (hRaw!=NULL) {
  hRaw->Draw();
 }

can->cd(2);
 gROOT->cd();
 TString hname="hCalHits2";
 hCal=(TH1 *)gROOT->FindObjectAny(hname);
 if (hCal!=NULL) {
  hCal->Draw();
 } else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
 gROOT->cd();
 TString hname="tof_ty0_sm000_rpc000_ch_evt_mul";
 hMap0=(TH2 *)gROOT->FindObjectAny(hname);
 if (hMap0!=NULL) {
  hMap0->Draw("colz");
 } else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(4);
 gROOT->cd();
 TString hname="tof_ty4_sm000_rpc000_ch_tot_mul";
 hMap1=(TH2 *)gROOT->FindObjectAny(hname);
 if (hMap1!=NULL) {
  hMap1->Draw("colz");
 } else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(5);
 gROOT->cd();
 TString hname="tof_ty2_sm000_rpc000_ch_tot_mul";
 hMap2=(TH2 *)gROOT->FindObjectAny(hname);
 if (hMap2!=NULL) {
  hMap2->Draw("colz");
 } else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(6);
 gROOT->cd();
 TString hname="tof_ty2_sm001_rpc000_ch_tot_mul";
 hMap2=(TH2 *)gROOT->FindObjectAny(hname);
 if (hMap2!=NULL) {
  hMap2->Draw("colz");
 } else 
   {
     cout << hname << " not found" << endl;
   }

}
