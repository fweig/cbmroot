void pl_XY_trk(Int_t NSt=4){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,500,800);
  can->Divide(2,NSt); 

 gPad->SetFillColor(0);
 gStyle->SetPalette(1);
 gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);

 TH1 *h;
 TH1 *h1;
 TH2 *h2;


 // if (h!=NULL) h->Delete();
 Int_t iCan=1;

 for (Int_t iSt=0; iSt<NSt; iSt++) {
 can->cd(iCan++);
 gROOT->cd();
 TString hname=Form("hXY_AllStations_%d",iSt);
  h2=(TH2 *)gROOT->FindObjectAny(hname);
  if (h2!=NULL) {
      h2->Draw("colz");
      TH2D *h2acc=h2->Clone(Form("Acc_%d",iSt));
      h2acc->Divide(h2,h2,1.,1.,"B"); 
      Double_t Nall=h2->GetEntries();
  }else 
   {
     cout << hname << " not found" << endl;
   }

 can->cd(iCan++);
 gROOT->cd();
 TString hname=Form("hXY_MissedStation_%d",iSt);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
      h2->Draw("colz");
      Double_t Nmis=h2->GetEntries();
      Double_t NmisI=h2->Integral();
      TH2D *h2missed=h2->Clone(Form("Missed_%d",iSt));
      h2missed->Multiply(h2,h2acc,1.,1.,"B");
      Double_t NmisaI=h2missed->Integral();
      Double_t eff=1. - NmisaI/(Nall+NmisaI);
      cout << "Effficiency of Station "<<iSt<<": all "<<Nall<<", mis "<<Nmis<<", "<<NmisI<<", "<<NmisaI
	   <<" -> "<<eff<<endl;
 }else 
   {
     cout << hname << " not found" << endl;
   }

 }

}
