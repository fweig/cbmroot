{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,4); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);
gStyle->SetOptFit(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);
 cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

 TH1 *h;
 TH2 *h2;
 Double_t NEvents;
 Double_t NFinalHits;
 Double_t dTRes;
 Double_t dTRMS;

 // if (hPla!=NULL) hPla->Delete();

can->cd(1);
 gROOT->cd();
 TString hname="hDXDY04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else {  cout << hname << " not found" << endl;  }

can->cd(2);
 gROOT->cd();
 TString hname="hDXDT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(3);
 gROOT->cd();
 TString hname="hDYDT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
 }else  { cout << hname << " not found" << endl; }

can->cd(4);
 gROOT->cd();
 TString hname="hDT24DT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  TH1D *hDT04=h2->ProjectionY();
  can->cd(9);
  Int_t iNbinsX=h2->GetNbinsX();
  cout<< " Histo "<<hname<<" has "<< iNbinsX << " Xbins"<<endl;
  Double_t BRes=1.E8;
  Int_t BL=0;
  Double_t BEntries=0.;
  Double_t Brms=0.;
  Double_t BNorm=0.;
   for (Int_t iBinL=0; iBinL<iNbinsX/2-1; iBinL++){
     //   for (Int_t iBinL=0; iBinL<iNbinsX; iBinL++){
      const char *cnam=Form("%s_py%d",(char *)hname,iBinL);
      TH1D *hDT04tmp=h2->ProjectionY(cnam,iBinL,iNbinsX-iBinL,"");
      TFitResultPtr fRes=hDT04tmp->Fit("gaus","S");
      cout <<Form("Fit with iBinL = %d : %6.1f, RMS : %6.1f ",iBinL,fRes->Parameter(2),hDT04tmp->GetRMS());
      cout<<endl;
      if(fRes->Parameter(2)<BRes){
	BRes=fRes->Parameter(2);
	BNorm=fRes->Parameter(0);
	Brms=hDT04tmp->GetRMS();
	BEntries=hDT04tmp->GetSum();
	BL=iBinL;
      }

      if (hDT04tmp->GetEntries()<1000) break;
    }
    //    gPad->SetOptFit(1);
 }else  { cout << hname << " not found" << endl; }


can->cd(5);
 gROOT->cd();
 TString hname="hChi04best";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  NFinalHits=h1->GetEntries();
 }else  { cout << hname << " not found" << endl; }

can->cd(6);
 gROOT->cd();
 TString hname="hNMatch24sel";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
  NEvents=h1->GetEntries();
 }else  { cout << hname << " not found" << endl; }

can->cd(7);
 gROOT->cd();
 TFitResultPtr fRes=hDT04->Fit("gaus","S");
 // fRes->Print();
 dTRes=fRes->Parameter(2);
 dTRMS=hDT04->GetRMS();

can->cd(8);
 gROOT->cd();
 TString hname="hDT24sel";
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  NSel=h1->Integral(BL,h1->GetNbinsX(),"");
 }else  { cout << hname << " not found" << endl; }

can->cd(9);
 gROOT->cd();
 TString hname="hY0DT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
   //  h2->Draw("colz");
  //  TH1D *hDT04=h2->ProjectionY();
 }else  { cout << hname << " not found" << endl; }

can->cd(10);
 gROOT->cd();
 TString hname="hDTD4DT04best";
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  can->cd(11);
  Int_t iNbinsX=h2->GetNbinsX();
  cout<< " Histo "<<hname<<" has "<< iNbinsX << " Xbins"<<endl;
  Double_t BDRes=1.E8;
  Int_t BDL=0;
  Double_t BDEntries=0.;
  Double_t BDrms=0.;
  Double_t BDNorm=0.;

  for (Int_t iBinL=0; iBinL<(iNbinsX/2-1); iBinL++){
     //   for (Int_t iBinL=0; iBinL<iNbinsX; iBinL++){
      const char *cnam=Form("%s_py%d",(char *)hname,iBinL);
      TH1D *hDT04tmp=h2->ProjectionY(cnam,iBinL,iNbinsX-iBinL,"");
      TFitResultPtr fRes=hDT04tmp->Fit("gaus","S");
      cout <<Form("Fit with iBinL = %d : %6.1f, RMS : %6.1f ",iBinL,fRes->Parameter(2),hDT04tmp->GetRMS());
      cout<<endl;
      if(fRes->Parameter(2)<BDRes){
	BDRes=fRes->Parameter(2);
	BDNorm=fRes->Parameter(0);
	BDrms=hDT04tmp->GetRMS();
	BDEntries=hDT04tmp->GetSum();
	BDL=iBinL;
      }

      if (hDT04tmp->GetEntries()<1000) break;
    }
    //    gPad->SetOptFit(1);
 }else  { cout << hname << " not found" << endl; }

 //report summary
 Double_t dEff=NFinalHits/NEvents;
 cout << Form("<I> Efficiency: %6.3f, overall 1-sigma timing resolution: %6.1f ps, RMS: %6.1f ps",dEff,dTRes,dTRMS) <<endl; 
 cout << Form("    best 1-sigma timing resolution at lower index %d: %6.1f ps, RMS: %6.1f ps from  %6.0f entries of %6.0f (%6.3f)",BL,BRes,Brms,BEntries,NSel,BEntries/NSel) <<endl; 
}
