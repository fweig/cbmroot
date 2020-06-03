void pl_rate_trkl(){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(2,4); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);

 TH1 *h;
 TH2 *h2;
 // if (h!=NULL) h->Delete();

can->cd(1);
 gROOT->cd();
 TString hname=Form("hTrklNofHitsRate");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2->ProfileX()->Draw("same");
  gPad->SetLogz();
  can->cd(2);
  h2->ProjectionX()->Draw("");
 }else 
   {     cout << hname << " not found" << endl;   }

can->cd(3);
 gROOT->cd();
 TString hname=Form("hTrklDetHitRate");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  // h2->ProfileX()->Draw("same");
 }else 
   {     cout << hname << " not found" << endl;   }

can->cd(4);
 Int_t NDet=h2->GetNbinsY();
 TH1 * hDetRatePx[10];
 TH1 * hDetRateRatioPx[10];
 Int_t iCol=1;
 for (Int_t iDet=0; iDet<NDet; iDet++){
   hDetRatePx[iDet]=h2->ProjectionX(Form("hDetRatePx%d",iDet),iDet+1,iDet+1);

   hDetRatePx[iDet]->SetLineColor(iCol++);
   if(iCol==5) iCol++;
   if(iDet==0){
     hDetRatePx[iDet]->Draw();
   }else{
     hDetRatePx[iDet]->Draw("same");
   }
 }

can->cd(5);
 Int_t iCol=1;
 for (Int_t iDet=0; iDet<NDet; iDet++){
   hDetRateRatioPx[iDet]= (TH1 *)hDetRatePx[iDet]->Clone(); // copy 
   hDetRateRatioPx[iDet]->Divide(hDetRatePx[iDet],hDetRatePx[0],1.,1.,"B");

   hDetRateRatioPx[iDet]->SetLineColor(iCol++);
   if(iCol==5) iCol++;
   if(iDet==0){
     hDetRateRatioPx[iDet]->Draw();
     hDetRateRatioPx[iDet]->SetMaximum(2.);
   }else{
     hDetRateRatioPx[iDet]->Draw("same");
   }
 }

can->cd(6);
 gROOT->cd();
 TString hname=Form("hTrklNofHitsRateInSpill");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2->ProfileX()->Draw("same");
  gPad->SetLogz();
 }else 
   {     cout << hname << " not found" << endl;   }

can->cd(7);
 gROOT->cd();
 TString hname=Form("hTrklDetHitRateInSpill");
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  // h2->ProfileX()->Draw("same");
 }else 
   {     cout << hname << " not found" << endl;   }

can->cd(8);
 Int_t iCol=1;
 TH1 * hDetRateInSpillPx[10];
 TH1 * hDetRateInSpillRatioPx[10];

 for (Int_t iDet=0; iDet<NDet; iDet++){
   hDetRateInSpillPx[iDet]=h2->ProjectionX(Form("hDetRateInSpillPx%d",iDet),iDet+1,iDet+1);
 }

 for (Int_t iDet=0; iDet<NDet; iDet++){
   hDetRateInSpillRatioPx[iDet]= (TH1 *)hDetRateInSpillPx[iDet]->Clone(); // copy 
   hDetRateInSpillRatioPx[iDet]->Divide(hDetRateInSpillPx[iDet],hDetRateInSpillPx[0],1.,1.,"B");

   hDetRateInSpillRatioPx[iDet]->SetLineColor(iCol++);
   if(iCol==5) iCol++;
   if(iDet==0){
     hDetRateInSpillRatioPx[iDet]->Draw();
     hDetRateInSpillRatioPx[iDet]->SetMaximum(2.);
   }else{
     hDetRateInSpillRatioPx[iDet]->Draw("same");
   }
 }
}


