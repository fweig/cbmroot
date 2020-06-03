void pl_calib_trk(){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,4); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);

 TH1 *h;
 TH1 *h1;
 TH2 *h2;
 TString hname="";

 Double_t dTOff=10000000.;
 Double_t dTRMS=10000000.;

 // if (h!=NULL) h->Delete();

can->cd(1);
 gROOT->cd();
 hname=Form("hPullT_Smt");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
 gROOT->cd();
 hname=Form("hTOff_Smt");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
  TH1D *hTOff=h2->ProjectionY();
  dTOff=hTOff->GetMean();
  dTRMS=hTOff->GetRMS();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
 gROOT->cd();
 hname=Form("hDeltaTt_Smt");
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
 hname=Form("hPullX_Smt");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(5);
 gROOT->cd();
 hname=Form("hPullY_Smt");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(6);
 gROOT->cd();
 hname=Form("hPullZ_Smt");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(7);
 gROOT->cd();
 hname=Form("hPullT_Smt_Off");
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  h1->GetYaxis()->SetTitleOffset(1.3);
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(8);
 gROOT->cd();
 hname=Form("hPullX_Smt_Off");
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  h1->SetMaximum(2.);
  h1->SetMinimum(-2.);
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(9);
 gROOT->cd();
 gROOT->cd();
 hname=Form("hPullY_Smt_Off");
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  h1->SetMaximum(2.);
  h1->SetMinimum(-2.);
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(10);
 gROOT->cd(); gROOT->cd();
 hname=Form("hPullT_Smt_Width");
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  h1->SetMinimum(0.);
  h1->GetYaxis()->SetTitleOffset(1.3);
  //gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(11);
 gROOT->cd();
 hname=Form("hPullX_Smt_Width");
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  h1->SetMaximum(2.5);
  h1->SetMinimum(0.);
  h1->GetYaxis()->SetTitleOffset(1.3);
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }
 hname=Form("hPullY_Smt_Width");
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("same");
  h1->SetLineColor(2);
 }else cout << hname << " not found" << endl;
 hname=Form("hPullZ_Smt_Width");
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("same");
  h1->SetLineColor(3);
 }else cout << hname << " not found" << endl;
   


can->cd(12);
 gROOT->cd();
 hname=Form("hTrackingTimeNhits");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2->GetYaxis()->SetTitleOffset(1.3);
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }


 dTOff = TMath::Abs(dTOff);
 Int_t iTOff=((Int_t)(dTOff*1000.)*1000) + (Int_t)(dTRMS*1000.);
 cout << " dTOff = "<<dTOff<<", dRMS = "<<dTRMS<<" -> iTOff = "<<iTOff<<endl;

 gROOT->ProcessLine(Form(".! echo %d  > TCalib.res", iTOff)); 

 can->SaveAs("pl_calib_trk.pdf");

}
