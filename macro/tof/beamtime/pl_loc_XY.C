void pl_loc_XY(Int_t iNDet=10)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas *can = new TCanvas("can","can",0,0,800,800);
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
 Int_t ic=1;
 // if (h!=NULL) h->Delete();
 for(Int_t iCh=0; iCh<9; iCh++){
   gROOT->cd();
   TString hname=Form("hXY%dD4best",iCh);
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     can->cd(ic++);
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
   }//else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
 } 

   gROOT->cd();
   TString hname=Form("hXX04D4best");
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     can->cd(ic++);
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}

   gROOT->cd();
   TString hname=Form("hYY04D4best");
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     can->cd(ic++);
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
 
} 
