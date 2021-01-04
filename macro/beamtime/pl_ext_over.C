void pl_ext_over() {
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 1200, 800);
  can->Divide(4, 3);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TString hname = "";

  // if (h!=NULL) h->Delete();

  can->cd(1);
  gROOT->cd();
  hname = Form("hMulCorTrkTof");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  gROOT->cd();
  hname = Form("hMulCorTrkSts");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(3);
  gROOT->cd();
  hname = Form("hMulCorTrkMuch");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(4);
  gROOT->cd();
  hname = Form("hMulCorTrkRich");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(5);
  gROOT->cd();
  hname = Form("hPosCorTrkTof");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(6);
  gROOT->cd();
  hname = Form("hPosCorTrkSts");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(7);
  gROOT->cd();
  hname = Form("hPosCorTrkMuch");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(8);
  gROOT->cd();
  hname = Form("hPosCorTrkRich");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }
  
  can->cd(9);
  gROOT->cd();
  hname = Form("hExt_TrkSizChiSq1");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(10);
  gROOT->cd();
  hname = Form("hExt_TrkSizVel1");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  } else {
    cout << hname << " not found" << endl;
  }
        
  can->SaveAs("pl_ext_over.pdf");
}
