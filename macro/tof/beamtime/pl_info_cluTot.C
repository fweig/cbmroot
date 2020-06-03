void pl_info_cluTot(Int_t SmT=0, Int_t iSm=0, Int_t iRpc=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,4); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);

 TH1 *h;
 TH2 *h2;
 // if (h!=NULL) h->Delete();

can->cd(1);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Tot",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2->ProfileX()->Draw("same");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

 can->cd(2);
 gPad->SetLogy();
 TH1 *hbins[200];   //or whatever you need, ie nbins
 Int_t nbins=h2->GetNbinsX();
 //nbins=5;
 cout << "processing "<<nbins<<" slices"<<endl;
 const Double_t XpreRange=2000.;
 for (int i=0;i<nbins;i++) {
   hbins[i] = h2->ProjectionY(Form("bin%d",i+1),i+1,i+1);
   can->cd(i+2);
   hbins[i]->Draw();
   Double_t Ymax=hbins[i]->GetMaximum();
   Int_t iBmax=hbins[i]->GetMaximumBin();
   TAxis *xaxis = hbins[i]->GetXaxis();
   Double_t Xmax=xaxis->GetBinCenter(iBmax);
   Double_t Xstart=Xmax-XpreRange;
   Xstart = (Double_t)(Int_t)Xstart;
   if(Xstart<0) Xstart=0;
   cout <<" slice "<<i<<" has max "<<Ymax<<" counts in bin "<<iBmax<<" at TOT = "<<Xmax<<" => Xstart = "<< Xstart <<endl;
 }
}
