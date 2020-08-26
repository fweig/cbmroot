void pl_all_CluPosEvol(Int_t iNSt=2, Int_t iTmax=0)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas *can = new TCanvas("can","can",48,56,900,900);
  can->Divide(3,4,0.01,0.01); 
  //  can->Divide(4,4,0.01,0.01); 
  //  can->Divide(2,2,0,0); 
  Float_t lsize=0.09;

 gPad->SetFillColor(0);
 gStyle->SetPalette(1);
 gStyle->SetLabelSize(lsize);

 //gStyle->SetStatY(0.5);
 //gStyle->SetStatX(0.5);
 gStyle->SetStatW(0.5);
 gStyle->SetStatH(0.3); 

 gStyle->SetOptStat(kFALSE);
 //gROOT->cd();
 //gROOT->SetDirLevel(2);

 TH1 *h;
 TH2 *h2;
 const Int_t   iType[7]={9,6,1,2,9,8,1};
 const Int_t  iSmNum[7]={3,2,1,2,3,3,3};
 const Int_t iRpcNum[7]={2,2,3,1,2,1,3};
 
 Int_t iCanv=0;
 // if (h!=NULL) h->Delete();

 for(Int_t iSt=0; iSt<iNSt; iSt++){
   // cout << "plot station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
  for(Int_t iSm=0; iSm<iSmNum[iSt];iSm++){
    //cout << "plot module at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
   for(Int_t iRp=0; iRp<iRpcNum[iSt];iRp++){
     //cout << "plot rpc at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
    gROOT->cd();
    TString hname=Form("cl_SmT%01d_sm%03d_rpc%03d_PosEvol",iType[iSt],iSm,iRp);
    h=(TH1 *)gROOT->FindObjectAny(hname);
    if (h!=NULL) {
      can->cd(iCanv+1); iCanv++;
      Double_t dLMargin=0.35;
      Double_t dTitOffset=1.6;
      gPad->SetLeftMargin(dLMargin);
      h->UseCurrentStyle();
      h->GetYaxis()->SetTitleOffset(dTitOffset);
      if(iTmax>0) h->GetXaxis()->SetRange(0.,iTmax);
      h->Draw("");
       //gPad->SetLogy();
    }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
   }
  }
 }
 can->SaveAs(Form("pl_all_CluPosEvol.pdf"));

} 
