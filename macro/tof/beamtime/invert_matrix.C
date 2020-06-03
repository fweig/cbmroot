void invert_matrix(Int_t NSt=3, char* var="X", Int_t iFit=0){
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

 const Int_t MSt=5;
 Double_t vSt[MSt];
 Double_t vMean[MSt];
 //Double_t vSig[MSt];
 Double_t vSig[3]={125.9,122.6,185.6};
 Double_t vRes[MSt];
 // if (h!=NULL) h->Delete();
 Int_t iCan=1;


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
 Double_t A[9]={1,1,0,0,1,1,1,0,1}; 
 Int_t k=0;
 for(Int_t i=0; i<NSt; i++) for(Int_t j=0; j<NSt; j++) {
       a[i][j]=A[k++];
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
 TMatrixD xRes = ainv * aSig;
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
 for(Int_t i=0; i<NSt; i++) cout<<"Gaussian fit width: "<<vSig[i]<<" => resolution of counter "<<i<<": "<<vRes[i]<<endl;

}
