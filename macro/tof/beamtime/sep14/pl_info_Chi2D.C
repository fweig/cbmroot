void pl_info_Chi2D(char *cHist="hChiDT04D4best", Int_t iDBin=1){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,3); 

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
h=(TH2 *)gROOT->FindObjectAny("hNMatchD4sel");
if(h==NULL) {
   cout << " No normalization possible "<< endl;
   return;
}
 Double_t dNTrig = h->GetEntries(); 
 NEvents=dNTrig;
 Double_t NSel=dNTrig;

can->cd(1);

 gROOT->cd();
 TString hname=cHist;
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");


can->cd(2);
  TH1D *hDT04=h2->ProjectionY();
 gROOT->cd();
 TFitResultPtr fRes=hDT04->Fit("gaus","S");
 // fRes->Print();
 dTRes=fRes->Parameter(2);
 dTRMS=hDT04->GetRMS();

can->cd(3);
  TH1D *hDTD4=h2->ProjectionX();
  hDTD4->Draw();
  NFinalHits=hDTD4->GetEntries();

can->cd(4);
  hDT04->Draw("");
  gPad->SetLogy();
  gStyle->SetPalette(1);

  static const NbinsX = 100;
  Int_t iNbinsX = h2->GetNbinsX();
  cout<< " Histo "<<hname<<" has "<< iNbinsX << " Xbins"<<endl;
  if( iNbinsX > NbinsX ) return;

  Double_t BRes=1.E8;
  Int_t BL=0;
  Double_t BEntries=0.;
  Double_t Brms=0.;
  Double_t BNorm=0.;
  TH1D *hDT04proY[NbinsX];
  TFitResultPtr fFitRes[NbinsX];

  TH1D *hEff=(TH1D *)hDTD4->Clone("Efficiency");
  hEff->Reset();
  hEff->SetTitle("Efficiency of #Deltat spectrum");

  TH1D *hRms=(TH1D *)hDTD4->Clone("Rms");
  hRms->Reset();
  hRms->SetTitle("RMS of #Deltat spectrum");

  TH1D *hSigma=(TH1D *)hDTD4->Clone("GaussRms");
  hSigma->Reset();
  hSigma->SetTitle("#sigma of Gaussian fit");

  TH1D *hMean=(TH1D *)hDTD4->Clone("Mean value");
  hMean->Reset();
  hMean->SetTitle("Mean value");

  TH1D *hGMean=(TH1D *)hDTD4->Clone("GaussMean");
  hGMean->Reset();
  hGMean->SetTitle("Mean of Gaussian fit");

  TH1D *hCnts=(TH1D *)hDTD4->Clone("IntCnts");
  hCnts->Reset();
  hCnts->SetTitle("Counts in selected interval");

  TH1D *hInt=(TH1D *)hDTD4->Clone("GaussInt");
  hInt->Reset();
  hInt->SetTitle("Integral of Gaussian fit");

  TH1D *hOut=(TH1D *)hDTD4->Clone("GaussOut");
  hOut->Reset();
  hOut->SetTitle("fraction of hits outside of 3 #sigma of Gaussian fit");

  Int_t iCol=1;
  Double_t DTMax=1000.;
  TF1 *gfit = new TF1("Gaus","gaus",-DTMax,DTMax);
  for (Int_t iBinL=0; iBinL<iNbinsX; iBinL += iDBin){
  // for (Int_t iBinL=0; iBinL<2; iBinL++){
      const char *cnam=Form("%s_py%d",(char *)hname,iBinL);
      hDT04proY[iBinL]=h2->ProjectionY(cnam,0,iBinL+iDBin-1,"");

      //    if (100>hDT04proY[iBinL]->GetEntries()) continue; // limit stat. fluctuations

      fFitRes[iBinL]=(TFitResultPtr )hDT04proY[iBinL]->Fit("Gaus","S");
      cout <<Form("Fit with iBinL = %d : %6.1f, Lim %6.1f, RMS : %6.1f ",
		  iBinL,fFitRes[iBinL]->Parameter(2),BRes,hDT04proY[iBinL]->GetRMS());
      cout<<endl;

      //      if (NULL == fFitRes[iBinL]) continue;

      if(fFitRes[iBinL]->Parameter(2)<BRes){
	BRes=fFitRes[iBinL]->Parameter(2);
	BNorm=fFitRes[iBinL]->Parameter(0);
	Brms=hDT04proY[iBinL]->GetRMS();
	BEntries=hDT04proY[iBinL]->GetSum();
	BL=iBinL;
	cout << " Update best "<< BL << ", BEntries:" << BEntries << endl;
      }

      Double_t param[3];
      gfit->GetParameters(param);
      cout << "Fit Res: "<< param[0] << ", "<< param[1] << ", "<<param[2] << ", "<< gfit->Integral(-DTMax,DTMax,&param[0])
	   <<endl;
 
      Double_t BinWidth = hDT04proY[iBinL]->GetBinWidth(0);


      Double_t dOut=hDT04proY[iBinL]->Integral(0,hDT04proY[iBinL]->FindBin(param[1]-3.*param[2]))
	           +hDT04proY[iBinL]->Integral(hDT04proY[iBinL]->FindBin(param[1]+3.*param[2]),iNbinsX-1);

      cout << "dOut ="<<dOut<<endl;

      if (dOut==0) continue;
      //	           +(Double_t)gfit->Integral(param[1]+3.*param[2],DTMax,&param[0]);

      for (Int_t iB=iBinL; iB<iBinL+iDBin; iB++)
      {
       hEff->SetBinContent(iB,(Double_t)hDTD4->Integral(0.,iB)/dNTrig);
       hRms->SetBinContent(iB,(Double_t)hDT04proY[iBinL]->GetRMS());
       hMean->SetBinContent(iB,(Double_t)hDT04proY[iBinL]->GetMean());
       hSigma->SetBinContent(iB,fFitRes[iBinL]->Parameter(2));
       hGMean->SetBinContent(iB,fFitRes[iBinL]->Parameter(1));
       hCnts->SetBinContent(iB,hDT04proY[iBinL]->Integral());
       hInt->SetBinContent(iB,(Double_t)gfit->Integral(-DTMax,DTMax,&param[0])/BinWidth);
       hOut->SetBinContent(iB,dOut/hInt->GetBinContent(iBinL));
      }

      //      if(hDT04proY[iBinL]->GetEntries()<100) break;
    }
    //    gPad->SetOptFit(1);
can->cd(5);
  iCol=1;
  hEff->Draw();
  /*
  hDT04proY[0]->Draw();
  gPad->SetLogy();
  for (Int_t iBinL=1; iBinL<iNbinsX; iBinL++){
    //cout << "plot bin "<<iBinL<<endl;
    if(iBinL%iDBin==0) 
    {    hDT04proY[iBinL]->Draw("same");
         hDT04proY[iBinL]->SetLineColor(iCol++);
    }
  }
*/
 }else  { cout << hname << " not found" << endl; }

can->cd(6);
 hRms->Draw();
 hRms->SetMinimum(50.);
 hSigma->Draw("same");
 hSigma->SetLineColor(2);

can->cd(7);
 hMean->Draw();
 hMean->SetMinimum(-50.);
 hMean->SetMaximum(50.);
 hGMean->Draw("same");
 hGMean->SetLineColor(2);

can->cd(8);
 hCnts->Draw();
 hInt->Draw("same");
 hInt->SetLineColor(2);

can->cd(9);
 hOut->Draw();

 //report summary
 Double_t dEff=NFinalHits/NEvents;
 cout << Form("<I> Efficiency: %6.3f, overall 1-sigma timing resolution: %6.1f ps, RMS: %6.1f ps",dEff,dTRes,dTRMS) <<endl; 
 cout << Form("    D4best 1-sigma timing resolution at lower index %d: %6.1f ps, RMS: %6.1f ps from  %6.0f entries of %6.0f (%6.3f)",BL,BRes,Brms,BEntries,NEvents,BEntries/NEvents) <<endl; 
}
