void pl_over_trk(Int_t NSt=4){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(4,4); 

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
 TString hname=Form("hTrklMulNhits");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
 gROOT->cd();
 TString hname=Form("hTrklHMul");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
 gROOT->cd();
 TString hname=Form("hTrklChi2");
 h1=(TH2 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(4);
 gROOT->cd();
 TString hname=Form("hTrklMul34");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(5);
 gROOT->cd();
 Int_t iCol=1;
 for (Int_t iSt=0; iSt<NSt; iSt++){
  TString hname=Form("hPullX_Station_%d",iSt);
  h1=(TH1 *)gROOT->FindObjectAny(hname);
  if (h1!=NULL) {
    if(iSt==0){
      h1->Draw("");
      h1->SetMinimum(0.5);
      gPad->SetLogy();
    }else{
      h1->Draw("same");
    }
    if(iCol == 5) iCol++;
    h1->SetLineColor(iCol++);
  }else 
   {
     cout << hname << " not found" << endl;
   }
  }

can->cd(6);
 gROOT->cd();
 Int_t iCol=1;
 for (Int_t iSt=0; iSt<NSt; iSt++){
  TString hname=Form("hPullY_Station_%d",iSt);
  h1=(TH1 *)gROOT->FindObjectAny(hname);
  if (h1!=NULL) {
    if(iSt==0){
      h1->Draw("");
      h1->SetMinimum(0.5);
      gPad->SetLogy();
    }else{
      h1->Draw("same");
    }
    if(iCol == 5) iCol++;
    h1->SetLineColor(iCol++);
  }else 
   {
     cout << hname << " not found" << endl;
   }
  }

can->cd(7);
 gROOT->cd();
 Int_t iCol=1;
 for (Int_t iSt=0; iSt<NSt; iSt++){
  TString hname=Form("hPullZ_Station_%d",iSt);
  h1=(TH1 *)gROOT->FindObjectAny(hname);
  if (h1!=NULL) {
    if(iSt==0){
      h1->Draw("");
      h1->SetMinimum(0.5);
      gPad->SetLogy();
    }else{
      h1->Draw("same");
    }
    if(iCol == 5) iCol++;
    h1->SetLineColor(iCol++);
  }else 
   {
     cout << hname << " not found" << endl;
   }
  }

can->cd(8);
 gROOT->cd();
 Int_t iCol=1;
 for (Int_t iSt=0; iSt<NSt; iSt++){
  TString hname=Form("hPullT_Station_%d",iSt);
  h1=(TH1 *)gROOT->FindObjectAny(hname);
  if (h1!=NULL) {
    if(iSt==0){
      h1->Draw("");
      h1->SetMinimum(0.5);
      gPad->SetLogy();
    }else{
      h1->Draw("same");
    }
    if(iCol == 5) iCol++;
    h1->SetLineColor(iCol++);
  }else 
   {
     cout << hname << " not found" << endl;
   }
  }

can->cd(9);
 gROOT->cd();
 TString hname=Form("hTrklZ0xHMul");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(10);
 gROOT->cd();
 TString hname=Form("hTrklZ0yHMul");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(11);
 gROOT->cd();
 TString hname=Form("hTrklTxHMul");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(12);
 gROOT->cd();
 TString hname=Form("hTrklTyHMul");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(13);
 gROOT->cd();
 TString hname=Form("hTrklTtHMul");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(14);
 gROOT->cd();
 TString hname=Form("hAllHitsStation");
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("");
  h1->SetMinimum(0.5);
  gPad->SetLogy();
  TH1 *hAll=h1->Clone();
 }else 
   {
     cout << hname << " not found" << endl;
   }
 TString hname=Form("hUsedHitsStation");
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw("same");
  h1->SetLineColor(2);
  TH1 *hEff=h1->Clone();
  hEff->Divide(h1,hAll,1.,1.,"B");

can->cd(15);
   hEff->Draw();

 }else 
   {
     cout << hname << " not found" << endl;
   }
can->cd(16);
 gROOT->cd();
 TString hname=Form("hTrklVelHMul");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

}

