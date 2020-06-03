void pl_all_CluSizeDTLastHits(Int_t iNSt=6, Int_t iSel=0, Double_t Tstart=1., Double_t Tend=100.)
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

 TH2 *h;
 TH2 *h2;
 const Int_t   iType[6]={5,4,6,2,9,8};
 const Int_t  iSmNum[6]={1,1,1,2,3,3};
 const Int_t iRpcNum[6]={1,1,2,1,2,1};
 
 Int_t iCanv=0;
 // if (h!=NULL) h->Delete();

 for(Int_t iSt=0; iSt<iNSt; iSt++){
   //   cout << "plot station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
  for(Int_t iSm=0; iSm<iSmNum[iSt];iSm++){
    //cout << "plot module at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
   for(Int_t iRp=0; iRp<iRpcNum[iSt];iRp++){
     //cout << "plot rpc at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
    can->cd(iCanv+1); iCanv++;
    gROOT->cd();
    TString hname=Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Size_DTLH",iType[iSt],iSm,iRp,iSel);
    h=(TH2 *)gROOT->FindObjectAny(hname);
    if (h!=NULL) {
     h->GetXaxis()->SetRange(Tstart,Tend);
     h->Draw("colz");
     gPad->SetLogz();
     h->ProfileX()->Draw("same");
    }else{cout<<"Histogram "<<hname<<" not existing. "<<endl;}
    if(iRp==10) break;
   }
  }
 }
  can->SaveAs(Form("pl_all_CluSize_DTLH.pdf"));

} 
