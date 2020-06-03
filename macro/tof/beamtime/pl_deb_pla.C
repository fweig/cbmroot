{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  can->Divide(4,4); 

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
 TString hname="tof_vftx_ft_b008_ch050";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
 gROOT->cd();
 TString hname="tof_vftx_ft_b008_ch051";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
 gROOT->cd();
 TString hname="tof_vftx_ft_b008_ch052";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(4);
 gROOT->cd();
 TString hname="tof_vftx_ft_b008_ch053";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }


can->cd(5);
 gROOT->cd();
 TString hname="tof_vftx_ft_b009_ch050";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(6);
 gROOT->cd();
 TString hname="tof_vftx_ft_b009_ch051";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(7);
 gROOT->cd();
 TString hname="tof_vftx_ft_b009_ch052";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(8);
 gROOT->cd();
 TString hname="tof_vftx_ft_b009_ch053";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

 // corrections
 
can->cd(9);
 gROOT->cd();
 TString hname="tof_vftx_tot_b008_ch025";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(10);
 gROOT->cd();
 TString hname="tof_vftx_tot_b008_ch026";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(11);
 gROOT->cd();
 TString hname="tof_vftx_tot_b009_ch025";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(12);
 gROOT->cd();
 TString hname="tof_vftx_tot_b009_ch026";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

 //
can->cd(13);
 gROOT->cd();
 TString hname="tof_vftx_dnlsum_b008_ch025";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(14);
 gROOT->cd();
 TString hname="tof_vftx_dnlsum_b008_ch026";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(15);
 gROOT->cd();
 TString hname="tof_vftx_dnlsum_b009_ch025";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(16);
 gROOT->cd();
 TString hname="tof_vftx_dnlsum_b009_ch026";
 hPla=(TH1 *)gROOT->FindObjectAny(hname);
 if (hPla!=NULL) {
  hPla->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }
}
