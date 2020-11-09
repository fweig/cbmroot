void pl_vert_trk(Double_t dFitWidth=0.) {
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  can->Divide(3, 4);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  // if (h!=NULL) h->Delete();

  can->cd(1);
  gROOT->cd();
  TString hname = Form("hTrklXY0_0");
  h2            = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  gROOT->cd();
  Double_t MeanX0=0.;
  Double_t SigX0=0.;
  Double_t MeanX0Err=0.;
  Double_t SigX0Err=0.;
  h1    = (TH1*) h2->ProjectionX();
  if (h1 != NULL) {
    Double_t dFMean = h1->GetMean();
    Double_t dFLim  = 2.0 * h1->GetRMS();
    dFMean=h1->GetBinCenter( h1-> GetMaximumBin() ); 
    if(dFitWidth > 0.) dFLim=dFitWidth;
    TFitResultPtr fRes =
        h1->Fit("gaus", "QS", "", dFMean - dFLim, dFMean + dFLim);
    //cout << " fRes = "<< fRes <<endl;
    if (-1 != fRes) {
      MeanX0    = fRes->Parameter(1);
      SigX0     = fRes->Parameter(2);
      MeanX0Err   = 0.;
      MeanX0Err = fRes->ParError(1);
      SigX0Err  = fRes->ParError(2);
	}
  }

  Double_t MeanY0=0.;
  Double_t SigY0=0.;
  Double_t MeanY0Err=0.;
  Double_t SigY0Err=0.;
  h1    = (TH1*) h2->ProjectionY();
  if (h1 != NULL) {
    Double_t dFMean = h1->GetMean();
    Double_t dFLim  = 2.0 * h1->GetRMS();
    dFMean=h1->GetBinCenter( h1-> GetMaximumBin() ); 
    if(dFitWidth > 0.) dFLim=dFitWidth;
    TFitResultPtr fRes =
        h1->Fit("gaus", "QS", "same", dFMean - dFLim, dFMean + dFLim);
        h1->SetLineColor(7);
    //cout << " fRes = "<< fRes <<endl;
    if (-1 != fRes) {
      MeanY0    = fRes->Parameter(1);
      SigY0     = fRes->Parameter(2);
      MeanY0Err   = 0.;
      MeanY0Err = fRes->ParError(1);
      SigY0Err  = fRes->ParError(2);
	}
  }
  
  
  cout << Form("TrkXY0_0: MeanX %6.2f (%5.2f), SigX %6.2f (%5.2f) ",MeanX0, MeanX0Err, SigX0, SigX0Err)
       << Form(" MeanY %6.2f (%5.2f), SigY %6.2f (%5.2f) ",MeanY0, MeanY0Err, SigY0, SigY0Err) << endl; 
   
  can->cd(3);
  gROOT->cd();
  hname = Form("hTrklXY0_1");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    Double_t MeanX1=0.;
	Double_t SigX1=0.;
	Double_t MeanX1Err=0.;
	Double_t SigX1Err=0.;
	h1    = (TH1*) h2->ProjectionX();
	if (h1 != NULL) {
      Double_t dFMean = h1->GetMean();
      Double_t dFLim  = 2.0 * h1->GetRMS();
      dFMean=h1->GetBinCenter( h1-> GetMaximumBin() ); 
      if(dFitWidth > 0.) dFLim=dFitWidth;
      TFitResultPtr fRes =
        h1->Fit("gaus", "QS", "", dFMean - dFLim, dFMean + dFLim);
      //cout << " fRes = "<< fRes <<endl;
      if (-1 != fRes) {
		MeanX1    = fRes->Parameter(1);
		SigX1     = fRes->Parameter(2);
		MeanX1Err   = 0.;
		MeanX1Err = fRes->ParError(1);
		SigX1Err  = fRes->ParError(2);
	  }
	
	  Double_t MeanY1=0.;
	  Double_t SigY1=0.;
	  Double_t MeanY1Err=0.;
	  Double_t SigY1Err=0.;
	  h1    = (TH1*) h2->ProjectionY();
	  if (h1 != NULL) {
		Double_t dFMean = h1->GetMean();
		Double_t dFLim  = 2.0 * h1->GetRMS();
		dFMean=h1->GetBinCenter( h1-> GetMaximumBin() ); 
        if(dFitWidth > 0.) dFLim=dFitWidth;
		TFitResultPtr fRes =
          h1->Fit("gaus", "QS", "same", dFMean - dFLim, dFMean + dFLim);
        h1->SetLineColor(7);
        //cout << " fRes = "<< fRes <<endl;
        if (-1 != fRes) {
		  MeanY1    = fRes->Parameter(1);
		  SigY1     = fRes->Parameter(2);
		  MeanY1Err   = 0.;
		  MeanY1Err = fRes->ParError(1);
		  SigY1Err  = fRes->ParError(2);
	    }
	  }
  
	  cout << Form("TrkXY0_1: MeanX %6.2f (%5.2f), SigX %6.2f (%5.2f) ",MeanX1, MeanX1Err, SigX1, SigX1Err)
           << Form(" MeanY %6.2f (%5.2f), SigY %6.2f (%5.2f) ",MeanY1, MeanY1Err, SigY1, SigY1Err) << endl; 
     
	}

  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(4);
  gROOT->cd();
  hname = Form("hVTXNorm");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw();
    gPad->SetLogy();
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(5);
  gROOT->cd();
  hname = Form("hVTX_XY0");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
  } else {
    cout << hname << " not found" << endl;
  }
  
  can->cd(10);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    Double_t MeanX1=0.;
	Double_t SigX1=0.;
	Double_t MeanX1Err=0.;
	Double_t SigX1Err=0.;
	h1    = (TH1*) h2->ProjectionX();
	if (h1 != NULL) {
      Double_t dFMean = h1->GetMean();
      Double_t dFLim  = 2.0 * h1->GetRMS();
      dFMean=h1->GetBinCenter( h1-> GetMaximumBin() ); 
      if(dFitWidth > 0.) dFLim=dFitWidth;
      TFitResultPtr fRes =
        h1->Fit("gaus", "QS", "", dFMean - dFLim, dFMean + dFLim);
      //cout << " fRes = "<< fRes <<endl;
      if (-1 != fRes) {
		MeanX1    = fRes->Parameter(1);
		SigX1     = fRes->Parameter(2);
		MeanX1Err   = 0.;
		MeanX1Err = fRes->ParError(1);
		SigX1Err  = fRes->ParError(2);
	  }
	
	  Double_t MeanY1=0.;
	  Double_t SigY1=0.;
	  Double_t MeanY1Err=0.;
	  Double_t SigY1Err=0.;
	  h1    = (TH1*) h2->ProjectionY();
	  if (h1 != NULL) {
		Double_t dFMean = h1->GetMean();
		Double_t dFLim  = 2.0 * h1->GetRMS();
		dFMean=h1->GetBinCenter( h1-> GetMaximumBin() ); 
        if(dFitWidth > 0.) dFLim=dFitWidth;
		TFitResultPtr fRes =
          h1->Fit("gaus", "QS", "same", dFMean - dFLim, dFMean + dFLim);
        h1->SetLineColor(7);
        //cout << " fRes = "<< fRes <<endl;
        if (-1 != fRes) {
		  MeanY1    = fRes->Parameter(1);
		  SigY1     = fRes->Parameter(2);
		  MeanY1Err   = 0.;
		  MeanY1Err = fRes->ParError(1);
		  SigY1Err  = fRes->ParError(2);
	    }
	  }
  
	  cout << Form("VTX0: MeanX %6.2f (%5.2f), SigX %6.2f (%5.2f) ",MeanX1, MeanX1Err, SigX1, SigX1Err)
           << Form(" MeanY %6.2f (%5.2f), SigY %6.2f (%5.2f) ",MeanY1, MeanY1Err, SigY1, SigY1Err) << endl; 
    }     
  }


  can->cd(6);
  gROOT->cd();
  hname = Form("hVTX_DT0_Norm");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(7);
  gROOT->cd();
  hname = Form("hTrklT0Mul");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(8);
  gROOT->cd();
  hname = Form("hTrklDT0SmMis");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(9);
  gROOT->cd();
  hname = Form("hTrklDT0StMis2");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
  } else {
    cout << hname << " not found" << endl;
  }
  can->SaveAs(Form("pl_vtx.pdf"));

}
