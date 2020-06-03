void pl_all_trbtdc(Int_t iNDet=4)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas *can = new TCanvas("can","can",48,56,1400,1800);
  can->Divide(2,2,0.01,0.01); 
  //  can->Divide(2,2,0,0); 
  Float_t lsize=0.07;

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
   TString hname=Form("cl_SmT%03d_rpc%03d_DigiCor",iCh);
   h1=(TH1 *)gROOT->FindObjectAny(hname);
   if (h1!=NULL) {
     h1->Draw("");
     gPad->SetLogy();
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
 } 
} 
