void pl_all_DigiDTFD( Int_t iOpt = 0, Int_t iNDet = 2, Double_t dDTthr = 2.) {
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 900, 900);
  //TCanvas *can = new TCanvas("can","can",48,56,900,700);
  //can->Divide(4,4,0.01,0.01);
  //  can->Divide(2,3,0.01,0.01);
  can->Divide(5, 7, 0.01, 0.01);
  Float_t lsize = 0.07;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH1* h;
  TH2* h2;
  TH1* hTime;
  TString hnameT;

  const Int_t iType[6]   = {0, 5, 9, 7, 6, 8};
  const Int_t iSmNum[6]  = {5, 1, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {5, 1, 2, 1, 2, 8};

  Double_t dTime = 0.;
  Int_t iCanv    = 0;

  Int_t jSmType = 5;
  Int_t jSm     = 0;
  Int_t jRp     = 0;

  // if (h!=NULL) h->Delete();

  for (Int_t iCh = 0; iCh < iNDet; iCh++) {
    for (Int_t iSm = 0; iSm < iSmNum[iCh]; iSm++) {
      for (Int_t iRpc = 0; iRpc < iRpcNum[iCh]; iRpc++) {
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        TString hname ="";
        switch (iOpt%10) {
        case 0:
          hname=Form("cl_SmT%01d_sm%03d_rpc%03d_DigiDTFD", iType[iCh], iSm, iRpc);
	  break;
	  
	case 1:
          hname=Form("cl_SmT%01d_sm%03d_rpc%03d_DigiDTMul", iType[iCh], iSm, iRpc);       
	  break;
	    
          default:;
        }

        h2 = (TH2*) gROOT->FindObjectAny(hname);
        TH1D* hx;
        TH1D* hy;
        if (h2 != NULL) {
	  Int_t iOpt2=(iOpt-iOpt%10)/10;
	  switch(iOpt2) {
	  case 0:
              h2->Draw("colz");
              gPad->SetLogz();
	      break;
	  case 1:
	     h2->ProjectionY()->Draw();
	     //gPad->SetLogz();
	      break;
	  case 2:
	     h2->ProfileX()->Draw();
	     //gPad->SetLogz();
	      break;	      
	  case 3:
	    for(Int_t iCh=0; iCh<h2->GetNbinsX(); iCh++) {
	      if(iCh==0) 
	        h2->ProjectionY(Form("%s_py%d",h2->GetName(),iCh),iCh+1,iCh+1)->Draw();
	      else {
		 h2->ProjectionY(Form("%s_py%d",h2->GetName(),iCh),iCh+1,iCh+1)->Draw("same");
	      }
	    }
	    break;
	  }
        } else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
      }
    }
  }
  can->SaveAs(Form("pl_all_DigiDTFD.pdf"));
}
