void fit_yPos(Int_t SmT = 0, Int_t iSm = 0, Int_t iRpc = 0, Double_t dLini=0.) {
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 1200, 1000);
  can->Divide(2, 3);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptFit(111);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH1* h2y;
  void fit_ybox(const char* hname, Double_t ysize);
  void fit_ybox(const char* hname);
  // if (h!=NULL) h->Delete();

  can->cd(1);
  gROOT->cd();
  gROOT->LoadMacro("fit_ybox.h");
  ROOT::Math::Minimizer* minimum =
    ROOT::Math::Factory::CreateMinimizer("Minuit", "Migrad");
  minimum->SetMaxFunctionCalls(100000);
  minimum->SetTolerance(0.1);
  minimum->SetPrintLevel(3);

  TFitter::SetMaxIterations(500000);
  TFitter::SetPrecision(0.1);
  TFitter::SetDefaultFitter("Minuit2");

  TString hname2 = Form("cl_SmT%d_sm%03d_rpc%03d_Pos", SmT, iSm, iRpc);
  //TString hname2 = Form("cal_SmT%d_sm%03d_rpc%03d_Position", SmT, iSm, iRpc);
  h2             = (TH2*) gROOT->FindObjectAny(hname2);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");

    can->cd(2);
    h2y = h2->ProjectionY();
    h2y->Draw();
    cout << " Fit with ybox " << h2y->GetName() << endl;
    fit_ybox((const char*) (h2y->GetName()));
    if (0) {  //NULL != gMinuit ) {
      cout << "Minuit ended with " << gMinuit->fCstatu << endl;
    }
    
    can->cd(3);
    Int_t NStrips=h2->GetNbinsX();
    Double_t aStr[NStrips];
    Double_t aYLength[NStrips];
    Double_t aYMean[NStrips];
    Double_t aYRes[NStrips];
    Double_t aStrE[NStrips];
    Double_t aYLengthE[NStrips];
    Double_t aYMeanE[NStrips];
    Double_t aYResE[NStrips];
    
    for (Int_t i=0; i<NStrips; i++) {
	  h2y = h2->ProjectionY(Form("%s_py%d",h2->GetName(),i),i+1,i+1);
	  if( i==0 ) h2y->Draw();
	  else h2y->Draw("same");
      cout << " Fit with ybox " << h2y->GetName() << endl;
      fit_ybox((const char*) (h2y->GetName()),dLini);
      TF1* ff = h2y->GetFunction("YBox");
	  aStr[i]=i;  
	  aStrE[i]=0;  
      if ( NULL != ff) {
		aYLength[i]=2.*ff->GetParameter(1);
		aYRes[i]=ff->GetParameter(2);
		aYMean[i]=ff->GetParameter(3);
		aYLengthE[i]=2.*ff->GetParError(1);
		aYResE[i]=ff->GetParError(2);
		aYMeanE[i]=ff->GetParError(3);		
	  } else {	  
		aYLength[i]=0.;
		aYRes[i]=0.;
		aYMean[i]=0.;
		aYLengthE[i]=0.;
		aYResE[i]=0.;
		aYMeanE[i]=0.;		
      }
    }  
    
    can->cd(4);
    Double_t dLMargin   = 0.25;
    Double_t dTitOffset = 1.8;
    gPad->SetLeftMargin(dLMargin);
    TGraphErrors* grl = new TGraphErrors(NStrips, aStr, aYLength, aStrE, aYLengthE);
    grl->SetTitle("YLength");
    grl->GetXaxis()->SetTitle("Strip number");
    grl->GetYaxis()->SetTitleOffset(dTitOffset);
    grl->GetXaxis()->SetLimits(-0.5, NStrips - 0.5);
    grl->SetMarkerStyle(24);
    grl->Draw("APLE");
    
    can->cd(5);
    gPad->SetLeftMargin(dLMargin);
    TGraphErrors* grr = new TGraphErrors(NStrips, aStr, aYRes, aStrE, aYResE);
    grr->SetTitle("YEdgeResolution");
    grr->GetXaxis()->SetTitle("Strip number");
    grr->GetYaxis()->SetTitleOffset(dTitOffset);
    grr->GetXaxis()->SetLimits(-0.5, NStrips - 0.5);
    grr->SetMarkerStyle(24);
    grr->Draw("APLE");    
  
    can->cd(6);
    gPad->SetLeftMargin(dLMargin);
    TGraphErrors* grm = new TGraphErrors(NStrips, aStr, aYMean, aStrE, aYMeanE);
    grm->SetTitle("YMean");
    grm->GetXaxis()->SetTitle("Strip number");
    grm->GetYaxis()->SetTitleOffset(dTitOffset);
    grm->GetXaxis()->SetLimits(-0.5, NStrips - 0.5);
    grm->SetMarkerStyle(24);
    grm->Draw("APLE");    
          
  } else {
    cout << hname2 << " not found" << endl;
  }


  can->SaveAs(Form("Ypos%01d_%01d_%01d.pdf", SmT, iSm, iRpc));
}
