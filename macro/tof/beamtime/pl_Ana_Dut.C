void pl_Ana_Dut(Int_t iDut=3, Int_t iFit=1){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,3); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);

 TH1 *h;
 TH1 *h1;
 TH2 *h2;
 // if (h!=NULL) h->Delete();

can->cd(1);
 gROOT->cd();
 TString hname=Form("hDutXY_Found_%d",iDut);
 h2f=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2f!=NULL) {
  h2f->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
 gROOT->cd();
 TString hname=Form("hDutXY_Missed_%d",iDut);
 h2m=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2m!=NULL) {
  h2m->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
 gROOT->cd();
 TString hname=Form("hDutChi_Found_Sm_%d",iDut);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
 }else 
   {
     cout << hname << " not found" << endl;
   }

 gROOT->cd();
 TString hname=Form("hDutChi_Missed_Sm_%d",iDut);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("same");
  h1->SetLineColor(2);
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(4);
 gROOT->cd();

  TString hname=Form("hDutPullX_Sm_%d",iDut);
  h1=(TH1 *)gROOT->FindObjectAny(hname);
  if (h1!=NULL) {
      h1->Draw("");
      gPad->SetLogy();
      if (iFit>0){
	Double_t dFMean=h1->GetMean();
	Double_t dFLim=1.5*h1->GetRMS();
	TFitResultPtr fRes=h1->Fit("gaus","S","",dFMean-dFLim,dFMean+dFLim);
	DXMean=fRes->Parameter(1);
	DXRMS=fRes->Parameter(2);
      }

  }else 
   {
     cout << hname << " not found" << endl;
   }
  

can->cd(5);
 gROOT->cd();

  TString hname=Form("hDutPullY_Sm_%d",iDut);
  h1=(TH1 *)gROOT->FindObjectAny(hname);
  if (h1!=NULL) {
      h1->Draw("");
      gPad->SetLogy();
      if (iFit>0){
	Double_t dFMean=h1->GetMean();
	Double_t dFLim=1.5*h1->GetRMS();
	TFitResultPtr fRes=h1->Fit("gaus","S","",dFMean-dFLim,dFMean+dFLim);
	DYMean=fRes->Parameter(1);
	DYRMS=fRes->Parameter(2);
      }

  }else 
   {
     cout << hname << " not found" << endl;
   }
  

can->cd(6);
 gROOT->cd();

  TString hname=Form("hDutPullT_Sm_%d",iDut);
  h1=(TH1 *)gROOT->FindObjectAny(hname);
  if (h1!=NULL) {
      h1->Draw("");
      gPad->SetLogy();
      if (iFit>0){
	Double_t dFMean=h1->GetMean();
	Double_t dFLim=1.5*h1->GetRMS();
	TFitResultPtr fRes=h1->Fit("gaus","S","",dFMean-dFLim,dFMean+dFLim);
	DTMean=fRes->Parameter(1);
	DTRMS=fRes->Parameter(2);
      }

  }else 
   {
     cout << hname << " not found" << endl;
   }


can->cd(7);
 gROOT->cd();

  TString hname=Form("hDutPullTB_Sm_%d",iDut);
  h1=(TH1 *)gROOT->FindObjectAny(hname);
  if (h1!=NULL) {
      h1->Draw("");
      gPad->SetLogy();
      if (iFit>0){
	Double_t dFMean=h1->GetMean();
	Double_t dFLim=1.5*h1->GetRMS();
	TFitResultPtr fRes=h1->Fit("gaus","S","",dFMean-dFLim,dFMean+dFLim);
	DTBMean=fRes->Parameter(1);
	DTBRMS=fRes->Parameter(2);
      }

  }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(8);

 gROOT->cd();
 TString hname=Form("hDutChi_Match_Sm_%d",iDut);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
 }else 
   {
     cout << hname << " not found" << endl;
   }


can->cd(9);

      TH2D *h2acc=h2f->Clone(Form("Acc_%d",iDut));
      h2acc->Divide(h2f,h2f,1.,1.,"B"); 
      Double_t Nall=h2f->GetEntries();     
      TH2D *h2missed=h2m->Clone(Form("Missed_%d",iDut));
      h2missed->Multiply(h2m,h2acc,1.,1.,"B");
      Double_t NmisaI=h2missed->Integral();
      Double_t eff=1. - NmisaI/(Nall+NmisaI);

      TH2D *h2tot=h2m->Clone(Form("Total_%d",iDut));
      TH2D *h2frac=h2f->Clone(Form("FractionMissed_%d",iDut));
      h2tot->Add(h2tot,h2f,1.,1.);
      h2frac->Divide(h2m,h2tot,1.,1.,"B");
      h2frac->Draw("colz");
      h2frac->SetTitle(Form("Fraction of hits missing, Sm %d",iDut));
      h2frac->SetMaximum(0.8);

  //results

 cout << "NHits: "<<h2f->GetEntries();
 cout << ", DX "<<DXMean<<" +/- "<<DXRMS;
 cout << ", DY "<<DYMean<<" +/- "<<DYRMS;
 cout << ", DT "<<DTMean<<" +/- "<<DTRMS;
 cout << ", DTB "<<DTBMean<<" +/- "<<DTBRMS;
 cout << ", Eff: "<<eff<<endl;


  
}

