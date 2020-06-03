void pl_over_trb_ft(Int_t iBoard=0)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas *can = new TCanvas("can","can",48,56,1400,1500);
  //can->Divide(8,4,0.01,0.01); 
  //  can->Divide(8,8,0.01,0.01); 
  can->Divide(4,4,0.01,0.01); 

 gPad->SetFillColor(0);
 gStyle->SetPalette(1);
 //gStyle->SetOptStat(kTRUE);

 // gROOT->cd();
 //gROOT->SetDirLevel(2);

 TH1 *h;
 TH2 *h2;
 // if (h!=NULL) h->Delete();
 for(Int_t iCh=0; iCh<16; iCh++){
   gROOT->cd();
   TString hname=Form("tof_trb_ft_b%03d_ch%03d",iBoard,iCh);
   //TString hname=Form("tof_trb_dnlsum_b%03d_ch%03d",iBoard,iCh);
   h1=(TH1 *)gROOT->FindObjectAny(hname);
   if (h1!=NULL) {
     can->cd(iCh+1);
     h1->Draw("");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
 } 
} 
