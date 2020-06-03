void pl_over_get4(Int_t nRoc=6){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(2,nRoc); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);

 TH1 *h;
 TH1 *h1;
 TH2 *h2;
 // if (h!=NULL) h->Delete();

iCan=0;
for (Int_t iRoc=0; iRoc<nRoc; iRoc++){ 
 can->cd(iCan++);
 gROOT->cd();
 TString hname1=Form("Raw_Tot_gDPB_%02d",iRoc);
 h2=(TH2 *)gROOT->FindObjectAny(hname1);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2->ProfileX()->Draw("same");
  gPad->SetLogz();
 }else { cout << hname1 << " not found" << endl; }
 
can->cd(iCan++);
 gROOT->cd();
 
 TString hname2=Form("ChCount_gDPB_%02d",iRoc);
 h2=(TH2 *)gROOT->FindObjectAny(hname2);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }
}
can->SaveAs(Form("pl_over_get4.pdf"));

}
