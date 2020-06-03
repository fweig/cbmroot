void pl_all_cluWalkSel(Int_t iNch=1, Int_t i1=0, Int_t iSel=0, Int_t SmT=0, Int_t iSm=0)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas *can = new TCanvas("can","can",48,56,1000,1400);
  Float_t lsize=0.04;
  switch(iNch){
  case 1:
    can->Divide(1,2,0.01,0.01); 
    lsize=0.035;
    break;
  case 4:
    can->Divide(2,2,0.01,0.01); 
    lsize=0.03;
    break;
  case 16:
    can->Divide(4,4,0.01,0.01); 
    lsize=0.07;
    break;
  case 56:
    can->Divide(7,8,0.01,0.01);
    break;
  case 72:
    break;
  default:
  }
  //  can->Divide(2,2,0,0); 

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
 Int_t ic=0;
 for(Int_t iCh=i1; iCh<i1+iNch/2; iCh++){
   can->cd(++ic);
   gROOT->cd();
   TString hname=Form("cl_SmT%d_sm%03d_rpc000_Ch%03d_S0_Sel%02d_Walk",SmT,iSm,iCh,iSel);
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
     gPad->SetLogz();
     TProfile *h2_pfx=h2->ProfileX();
     h2_pfx->Draw("same");
   }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
   TString hname=Form("Cor_SmT%d_sm%03d_rpc000_Ch%03d_S0_Walk_px",SmT,iSm,iCh);
   h1=(TH1 *)gROOT->FindObjectAny(hname);
   if (h1!=NULL) {
     h1->Draw("same");
   }else{cout<<"Histogram "<<hname<<" not found. "<<endl;}

   can->cd(++ic);
   gROOT->cd();
   TString hname=Form("cl_SmT%d_sm%03d_rpc000_Ch%03d_S1_Sel%02d_Walk",SmT,iSm,iCh,iSel);
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
     gPad->SetLogz();
     TProfile *h2_pfx=h2->ProfileX();
     h2_pfx->Draw("same");
   }
   TString hname=Form("Cor_SmT%d_sm%03d_rpc000_Ch%03d_S1_Walk_px",SmT,iSm,iCh);
   h1=(TH1 *)gROOT->FindObjectAny(hname);
   if (h1!=NULL) {
     h1->Draw("same");
   }else{cout<<"Histogram "<<hname<<" not found. "<<endl;}
 } 
} 
