void pl_all_XY(Int_t iNDet=5)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas *can = new TCanvas("can","can",0,0,1200,800);
  can->Divide(3,2,0.01,0.01); 
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
 // if (h!=NULL) h->Delete();
 for(Int_t iCh=0; iCh<iNDet; iCh++){
   can->cd(iCh+1);
   gROOT->cd();
   TString hname=Form("hXY_SmT%d",iCh);
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
 } 
} 
