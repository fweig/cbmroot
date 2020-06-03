
void set_plot_style()
{
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    cout << "set_plot_style finished"<<endl;
}

void pl_cor_walk(Int_t SmT=9, Int_t iSm=0, Int_t iRpc=1, Double_t yRange=1., Double_t dYShift=0.1, Double_t Xmin=0., Double_t Xmax=10.){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(2,1); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
set_plot_style();
gStyle->SetOptStat(kTRUE);
//TExec *ex1 = new TExec("ex1","Pal1();");

// gROOT->cd();
// gROOT->SetDirLevel(1);

 TH1 *h;
 TH1 *h1;
 TH2 *h2;
 TH2 *h2C;
 // if (h!=NULL) h->Delete();
 Int_t iNch=0;

can->cd(1);
// gROOT->cd();
 gDirectory->pwd();
 //TString hname=Form("Cor_SmT%d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",SmT,iSm,iRpc,0);
 TString hname=Form("cl_CorSmT%d_sm%03d_rpc%03d_Pos_pfx",SmT,iSm,iRpc);
 h1=(TH1 *)gDirectory->FindObjectAny(hname);
 if (h1!=NULL) {
   //  h1->Draw("colz");
   // gPad->SetLogz();
  iNch=h1->GetNbinsX();
 }else 
   {
     cout << hname << " not found" << endl;
   }

 for (Int_t iCh=0; iCh<iNch; iCh++){
   TString hname=Form("Cor_SmT%d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",SmT,iSm,iRpc,iCh);
 h2=(TH2 *)gDirectory->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->SetLineColor(iCh+1);
  if (iCh==0) {
    h2->Draw("Lhist");
    h2->SetMinimum(-yRange);
    h2->SetMaximum(yRange+iNch*dYShift);
    h2->GetXaxis()->SetRangeUser(Xmin,Xmax);
  }else{
    h2C=(TH2 *)h2->Clone();
    Int_t iNB=h2C->GetNbinsX();
    for (Int_t iB=0; iB<iNB; iB++){
      h2C->SetBinContent(iB+1,h2C->GetBinContent(iB+1)+dYShift*iCh);
    }
    h2C->Draw("same Lhist");
  }
  //  h2->UseCurrentStyle(); 
  //  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }
 }

can->cd(2);
 for (Int_t iCh=0; iCh<iNch; iCh++){
   TString hname=Form("Cor_SmT%d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",SmT,iSm,iRpc,iCh);
 h2=(TH2 *)gDirectory->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->SetLineColor(iCh+1);
  if (iCh==0) {
    h2->Draw("Lhist");
    h2->SetMinimum(-yRange);
    h2->SetMaximum(yRange+iNch*dYShift);
    h2->GetXaxis()->SetRangeUser(Xmin,Xmax);
  }else{
    h2C=(TH2 *)h2->Clone();
    Int_t iNB=h2C->GetNbinsX();
    for (Int_t iB=0; iB<iNB; iB++){
      h2C->SetBinContent(iB+1,h2C->GetBinContent(iB+1)+dYShift*iCh);
    }
    h2C->Draw("same Lhist");
  }
  //  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }
 }

}
