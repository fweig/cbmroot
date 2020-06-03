void pl_ana_DTXY(Int_t iDut=9,  Int_t NewNbinsX=3, Int_t NewNbinsY=3, Int_t iOpt=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(2,2); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kFALSE);

 gROOT->cd();
 gROOT->SetDirLevel(1);

 TH1 *h;
 TH1 *h1;
 TH2 *h2;
 TH3 *h3;

can->cd(1);
 gROOT->cd();
 TString hname=Form("hDutXYDT_%d",iDut);
 h3f=(TH3 *)gROOT->FindObjectAny(hname);
 if (h3f!=NULL) {
   h3f->ProjectionZ()->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);

 Int_t Nxbins=h3f->GetNbinsX();
 Int_t Nybins=h3f->GetNbinsY();
 Int_t Nzbins=h3f->GetNbinsZ();

 Int_t iDeltaX=Nxbins/NewNbinsX;
 Int_t iDeltaY=Nybins/NewNbinsY;

 h2n = new TH2D("hTH3FitMean","#Deltat Fit Entries; x (cm); y (cm)",NewNbinsX,h3f->GetXaxis()->GetXmin(),h3f->GetXaxis()->GetXmax(),
		NewNbinsY,h3f->GetYaxis()->GetXmin(),h3f->GetYaxis()->GetXmax());
 h2m = new TH2D("hTH3FitMean","#Deltat Fit Mean; x (cm); y (cm)",NewNbinsX,h3f->GetXaxis()->GetXmin(),h3f->GetXaxis()->GetXmax(),
		NewNbinsY,h3f->GetYaxis()->GetXmin(),h3f->GetYaxis()->GetXmax());
 h2s = new TH2D("hTH3FitSigma","#Deltat Fit Sigma; x (cm); y (cm)",NewNbinsX,h3f->GetXaxis()->GetXmin(),h3f->GetXaxis()->GetXmax(),
		NewNbinsY,h3f->GetYaxis()->GetXmin(),h3f->GetYaxis()->GetXmax());

 for(Int_t ix=0; ix<NewNbinsX; ix++){
   for(Int_t iy=0; iy<NewNbinsY; iy++){
     TH1D* hsel = h3f->ProjectionZ(Form("Deltat_%d_%d",ix,iy),ix*iDeltaX,(ix+1)*iDeltaX,iy*iDeltaY,(iy+1)*iDeltaY);
     Double_t dFMean=hsel->GetMean();
     Double_t dFLim=2.5*hsel->GetRMS();
     if(hsel->Integral()>10){
       TFitResultPtr fRes=hsel->Fit("gaus","S","",dFMean-dFLim,dFMean+dFLim);
       h2n->SetBinContent(ix+1,iy+1,hsel->Integral());
       h2m->SetBinContent(ix+1,iy+1,fRes->Parameter(1));
       h2s->SetBinContent(ix+1,iy+1,fRes->Parameter(2));
     }
   }
 }

 can->cd(2);
 {
   h2n->Draw("colz");
   //gPad->SetLinz();
 }

 can->cd(3);
 {
 h2m->Draw("colz");
 }

 can->cd(4);
 {
 h2s->Draw("colz");
 }

}
