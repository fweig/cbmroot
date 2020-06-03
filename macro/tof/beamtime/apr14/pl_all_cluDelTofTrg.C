void pl_all_cluDelTofTrg(Int_t iNch=1, Int_t iTrg=0, Int_t SmT=0, Int_t iSm=0)
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
    lsize=0.05;
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

   can->cd(1);
   gROOT->cd();
   TString hname=Form("cl_SmT%d_sm%03d_rpc000_Trg%02d_DelTof",SmT,iSm,iTrg);
   h2=(TH2 *)gROOT->FindObjectAny(hname);
   if (h2!=NULL) {
     h2->UseCurrentStyle(); h2->GetYaxis()->SetLabelSize(lsize);
     h2->Draw("colz");
     gPad->SetLogz();
     TProfile *h2_pfx=h2->ProfileX();
     h2_pfx->Draw("same");

    TString hname=Form("cl_CorSmT%d_sm%03d_rpc000_DelTof",SmT,iSm,iTrg);
    h1=(TH1 *)gROOT->FindObjectAny(hname);
    if (h1!=NULL) {
     h1->Draw("same");
    }
   }
} 
