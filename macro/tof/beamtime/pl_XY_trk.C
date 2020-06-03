void pl_XY_trk(Int_t NSt=4){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,NSt); 

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

      can->cd(iCan++);
      gROOT->cd();
      TString hname=Form("hXY_MissedStation_%d",iSt);
      h2m=(TH2 *)gROOT->FindObjectAny(hname);
      if (h2m!=NULL) {
      h2m->Draw("colz");
      Double_t Nmis=h2->GetEntries();
      Double_t NmisI=h2->Integral();
      TH2D *h2missed=h2->Clone(Form("Missed_%d",iSt));
      h2missed->Multiply(h2m,h2acc,1.,1.,"B");
      Double_t NmisaI=h2missed->Integral();
      Double_t eff=1. - NmisaI/(Nall+NmisaI);
      cout << "Effficiency of Station "<<iSt<<": all "<<Nall<<", mis "<<Nmis<<", "<<NmisI<<", "<<NmisaI
	   <<" -> "<<eff<<endl;

      can->cd(iCan++);
      TH2D *h2frac=h2->Clone(Form("FractionMissed_%d",iSt));
      TH2D *h2tot=h2m->Clone(Form("Total_%d",iSt));
      h2tot->Add(h2tot,h2,1.,1.);
      h2frac->Divide(h2missed,h2tot,1.,1.,"B");
      h2frac->Draw("colz");
      h2frac->SetTitle(Form("Fraction of hits missing, station %d",iSt));
      h2frac->SetMaximum(0.8);
      }
 }else 
   {
     cout << hname << " not found" << endl;
   }
 }
}
