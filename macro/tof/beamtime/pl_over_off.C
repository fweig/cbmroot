void pl_over_off(Int_t SmT=3, Int_t iSm=0, Int_t iRpc=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,5); 

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
 TString hname=Form("cl_SmT%d_Pos",SmT);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2->ProfileX()->Draw("same");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_TOff",SmT);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

 for (Int_t iSel=0; iSel<4; iSel++){
can->cd(4+3*iSel);
 gROOT->cd();
 TString hname=Form("cl_TSmT%d_Sel%02d_Pos",SmT,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(5+3*iSel);
 gROOT->cd();
 TString hname=Form("cl_TSmT%d_Sel%02d_TOff",SmT,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(6+3*iSel);
 gROOT->cd();
 TString hname=Form("cl_TSmT%d_Sel%02d_TRun",SmT,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }
 
} // iSel loop end 

}
