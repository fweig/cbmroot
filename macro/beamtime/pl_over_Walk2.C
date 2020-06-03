void pl_over_Walk2(Int_t iSel=0, Int_t iSmT=9, Int_t iSm=0, Int_t iRpc=0, Double_t Tmax=0.2){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",48,25,700,800);
  can->Divide(1,2); 

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  //  gStyle->SetOptStat(kTRUE);
  //  gStyle->SetOptStat(1111);
  //  gROOT->cd();
  //  gROOT->SetDirLevel(1);

  TH1 *h;
  TH1 *h1;
  TH2 *h2;
  TH3 *h3;
  TH1D *h2py;
  TH1D *h2tpy;
  TH1D *h2posy;
  TH1 *hDX;
  TH1 *hDY;
  
  Double_t dAvCluSize;

can->cd(1);
{
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Walk2",iSmT,iSm,iRpc,iSel);
 h3=(TH3 *)gROOT->FindObjectAny(hname);
 if (h3!=NULL) {
  h3->Project3D("yx")->Draw("colz");
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }
}

can->cd(2);
{
 if (h3!=NULL) {
   h2 = (TH2 *)h3->Project3DProfile("yx");
   h2->SetMinimum(-Tmax);
   h2->SetMaximum(Tmax);
   h2->Draw("colz");
 }
}


 can->SaveAs(Form("pl_over_Walk2_%01d_%01d_%01d_%01d.pdf",iSel,iSmT,iSm,iRpc));

}
