void pl_eff_XY(Int_t iMode=0)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas *can = new TCanvas("can","can",0,0,800,800);
  can->Divide(3,3,0.01,0.01); 
  //  can->Divide(2,2,0,0); 
  Float_t lsize=0.03;

 gROOT->cd();
 gPad->SetFillColor(0);
 gStyle->SetPalette(1);
 gStyle->SetLabelSize(lsize);

 //gStyle->SetOptStat(kTRUE);
 //gROOT->cd();
 //gROOT->SetDirLevel(2);

 TH1 *h;
 TH2 *h2;

 TH2 *hs4;
 TH2 *hsS;
 TH2 *hs0;

 TH2 *heff4;
 TH2 *heffS;
 TH2 *heff0;

 can->cd(1);
   gROOT->cd();
   TString hname=Form("hXY4D4sel");
   hs4=(TH2 *)gROOT->FindObjectAny(hname);
   if (hs4!=NULL) {
     hs4->UseCurrentStyle(); hs4->GetYaxis()->SetLabelSize(lsize);
     hs4->Draw("colz");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

 can->cd(2);
   gROOT->cd();
   TString hname=Form("hXYSel2D4sel");
   hsS=(TH2 *)gROOT->FindObjectAny(hname);
   if (hsS!=NULL) {
     hsS->UseCurrentStyle(); hsS->GetYaxis()->SetLabelSize(lsize);
     hsS->Draw("colz");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

 can->cd(3);
   gROOT->cd();
   TString hname=Form("hXY0D4sel");
   hs0=(TH2 *)gROOT->FindObjectAny(hname);
   if (hs0!=NULL) {
     hs0->UseCurrentStyle(); hs0->GetYaxis()->SetLabelSize(lsize);
     hs0->Draw("colz");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

 can->cd(4);
   gROOT->cd();
   TString hname=Form("hXY4D4best");
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
     heff4=(TH2 *)h2->Clone();
     heff4->SetName(hname);
     heff4->SetTitle("Selection efficiency on reference counter");
     heff4->Divide(h2,hs4,1.,1.,"B");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

 can->cd(5);
   gROOT->cd();
   TString hname=Form("hXYSel2D4best");
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
     heffS=(TH2 *)h2->Clone();
     heffS->SetName(hname);
     heffS->SetTitle("Selection efficiency on Sel2 counter");
     heffS->Divide(h2,hsS,1.,1.,"B");   
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

 can->cd(6);
   gROOT->cd();
   TString hname=Form("hXY0D4best");
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
     heff0=(TH2 *)h2->Clone();
     heff0->SetName(hname);
     heff0->SetTitle("Selection efficiency on Dut counter");
     heff0->Divide(h2,hs0,1.,1.,"B");  
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

 can->cd(7);
   if (heff4!=NULL) {
     heff4->SetMaximum(1.);
     heff4->UseCurrentStyle(); heff4->GetYaxis()->SetLabelSize(lsize);
     heff4->Draw("colz");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

 can->cd(8);
   if (heffS!=NULL) {
     heffS->SetMaximum(1.);
     heffS->UseCurrentStyle(); heffS->GetYaxis()->SetLabelSize(lsize);
     heffS->Draw("colz");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

 can->cd(9);
   if (heff0!=NULL) {
     heff0->SetMaximum(1.);
     heff0->UseCurrentStyle(); heff0->GetYaxis()->SetLabelSize(lsize);
     heff0->Draw("colz");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

} 
