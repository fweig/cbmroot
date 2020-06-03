void pl_all_dTSel(Int_t iNSel=4)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas *can = new TCanvas("can","can",0,0,900,900);
  can->Divide(2,2,0.01,0.01); 
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
 for(Int_t iCh=0; iCh<iNSel; iCh++){
   can->cd(iCh+1);
   gROOT->cd();
   TString hname=Form("cl_dt_Sel%02d",iCh);
   h1=(TH1 *)gROOT->FindObjectAny(hname);
   if (h1!=NULL) {
     h1->UseCurrentStyle(); h1->GetYaxis()->SetLabelSize(lsize);
     h1->Draw("");
     gPad->SetLogy();
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
 } 
} 
