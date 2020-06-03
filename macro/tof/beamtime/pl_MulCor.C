void pl_MulCor(Int_t SmT=1, Int_t iSm=0, Int_t iRpc=0){
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
 TH2 *h2;
 // if (h!=NULL) h->Delete();

can->cd(1);
 gROOT->cd();
 TString hname=Form("hCluMul04D4best");
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
 TString hname=Form("hDigiMul4D4best");
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
 TString hname=Form("hDigiMul0D4best");
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
 TString hname=Form("hStrMul04D4best");
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
 TString hname=Form("hCluSize04D4best");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else {     cout << hname << " not found" << endl;   }

can->cd(6);
 gROOT->cd();
 TString hname=Form("hCluMulTSig0D4best");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else {     cout << hname << " not found" << endl;   }

can->cd(7);
 gROOT->cd();
 TString hname=Form("hCluMulTSig4D4best");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else {     cout << hname << " not found" << endl;   }

can->cd(8);
 gROOT->cd();
 TString hname=Form("hCluMulTrel0D4best");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else {     cout << hname << " not found" << endl;   }

can->cd(9);
 gROOT->cd();
 TString hname=Form("hCluMulTrel4D4best");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else {     cout << hname << " not found" << endl;   }

can->cd(10);
 gROOT->cd();
 TString hname=Form("hCluSizeSigT0D4best");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else {     cout << hname << " not found" << endl;   }

can->cd(11);
 gROOT->cd();
 TString hname=Form("hCluSizeSigT4D4best");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else {     cout << hname << " not found" << endl;   }

}
