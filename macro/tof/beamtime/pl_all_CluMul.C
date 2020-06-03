void pl_all_CluMul(Int_t iNDet=8)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas *can = new TCanvas("can","can",48,56,900,900);
  can->Divide(4,4,0.01,0.01); 
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
 Int_t   iType[8]={1,3,4,6,2,5,8,9};
 Int_t    iNum[8]={1,1,1,1,2,2,1,1};
 Int_t iRpcNum[8]={1,1,1,1,1,1,6,1};
 Int_t iCanv=0;
 // if (h!=NULL) h->Delete();

 for(Int_t iCh=0; iCh<iNDet; iCh++){
  for(Int_t iSm=0; iSm<iNum[iCh];iSm++){
   for(Int_t iRp=0; iRp<iRpcNum[iCh];iRp++){
    can->cd(iCanv+1); iCanv++;
    gROOT->cd();
    TString hname=Form("cl_SmT%01d_sm%03d_rpc%03d_Mul",iType[iCh],iSm,iRp);
    h=(TH1 *)gROOT->FindObjectAny(hname);
    if (h!=NULL) {
     h->Draw("");
     //     gPad->SetLogy();
    }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
   }
  }
 } 
} 
