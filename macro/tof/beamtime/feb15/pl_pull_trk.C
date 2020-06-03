void pl_pull_trk(Int_t NSt=5, char* var="X", Int_t iFit=0){
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

 const Int_t MSt=10;
 Double_t vSt[MSt];
 Double_t vMean[MSt];
 Double_t vSig[MSt];
 Double_t vRes[MSt];
 // if (h!=NULL) h->Delete();
 Int_t iCan=1;

 for (Int_t iSt=0; iSt<NSt; iSt++) {
 can->cd(iCan++);
 gROOT->cd();
 TString hname=Form("hPull%s_Station_%d",var,iSt);
  h1=(TH1 *)gROOT->FindObjectAny(hname);
  if (h1!=NULL) {
      h1->Draw("");
      gPad->SetLogy();
      gPad->SetGridx();
      if (iFit>0){
	Double_t dFMean=h1->GetMean();
	Double_t dFLim=2.5*h1->GetRMS();
	TFitResultPtr fRes=h1->Fit("gaus","S","",dFMean-dFLim,dFMean+dFLim);
	vSt[iSt]=iSt;
	vMean[iSt]=fRes->Parameter(1);
	vSig[iSt]=fRes->Parameter(2);
	//vSig[iSt]=TMath::Max(20.,vSig[iSt]);
      }
  }else 
   {
     cout << hname << " not found" << endl;
   }
 }
 if(0==iFit) return;
 can->cd(iCan++);
 TGraph *grm = new TGraph(NSt, vSt, vMean);
 grm->GetXaxis()->SetTitle("Station number");
 grm->GetYaxis()->SetTitle("Mean deviation");
 grm->Draw("APL");
 grm->SetMarkerStyle(24);

 can->cd(iCan++);
 TGraph *grs = new TGraph(NSt, vSt, vSig);
 grs->GetXaxis()->SetTitle("Station number");
 grs->GetYaxis()->SetTitle("Gaussian Sigma");
 grs->Draw("APL");
 grs->SetMarkerStyle(24);

 can->cd(iCan++);
 Double_t val=(NSt-1)*(NSt-1);
 TMatrixD a(NSt,NSt); 
 for(Int_t i=0; i<NSt; i++) for(Int_t j=0; j<NSt; j++) {
     if (i==j) {
       a[i][j]=val;
     }else{
       a[i][j]=1;
     }
   }
 a.Draw("colz");
 a.Print();

 can->cd(iCan++);
 TMatrixD ainv=a; 
 ainv.Invert();
 ainv.Draw("colz");
 ainv.Print();
 TMatrixD aSig(NSt,1);
 for(Int_t i=0; i<NSt; i++) aSig[i][0]=vSig[i]*vSig[i];

 cout << "Measured gaussian widths: "<<endl; 
 aSig.Print();
 TMatrixD xRes = ainv * aSig * val;
 cout << "Resolution of counters: "<<endl; 
 xRes.Print();

 can->cd(iCan++);
 for(Int_t i=0; i<NSt; i++) vRes[i]=TMath::Sqrt(TMath::Abs(xRes[i][0]));
 TGraph *grr = new TGraph(NSt, vSt, vRes);
 grr->GetXaxis()->SetTitle("Station number");
 switch(var){
 case "X":
 case "Y":
 case "Z":
 grr->GetYaxis()->SetTitle("resolution (cm)");
   break;
 default:
 grr->GetYaxis()->SetTitle("resolution (ps)");
 }
 grr->Draw("APL");
 grr->SetMarkerStyle(20);
 for(Int_t i=0; i<NSt; i++) cout<<" Resolution of counter "<<i<<": "<<vRes[i]<<endl;

}
