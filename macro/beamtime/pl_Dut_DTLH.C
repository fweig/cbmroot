/** @file pl_Dut_DTLH.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

void pl_Dut_DTLH(Int_t iDut = 910, TString sysinfo = "")
{
  gROOT->LoadMacro("pl_Datime.C");
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 700, 700);
  can->Divide(2, 4);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);
  // cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH2* h2f;
  TH2* h2m;
  // if (hPla!=NULL) hPla->Delete();
  TString hname   = "";
  TProfile* h2pfx = NULL;

  can->cd(1);

  gROOT->cd();
  hname = Form("hDutDTLH_CluSize_%d", iDut);
  cout << " Look for histo " << hname << endl;
  h2 = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    h2->ProfileX()->Draw("same");
    Double_t dMeanCluSize = h2->ProjectionY()->GetMean();
    cout << " Mean Cluster size: " << dMeanCluSize << endl;
    gPad->SetLogz();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  hname = Form("hDutDTLH_Tot_%d", iDut);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    h2->ProfileX()->Draw("same");
    gPad->SetLogz();
  }
  else {
    cout << hname << " not found" << endl;
  }


  can->cd(3);
  hname = Form("hDutDTLH_Mul_%d", iDut);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    h2->ProfileX()->Draw("same");
    gPad->SetLogz();
  }
  else {
    cout << hname << " not found" << endl;
  }


  can->cd(4);
  hname = Form("hDutDTLH_DD_Found_%d", iDut);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    h2->ProfileX()->Draw("same");
    gPad->SetLogz();
  }
  else {
    cout << hname << " not found" << endl;
  }


  can->cd(5);
  hname = Form("hDutDTLH_DDH_Found_%d", iDut);
  h2f   = (TH2*) gROOT->FindObjectAny(hname);
  if (h2f != NULL) {
    h2f->Draw("colz");
    h2f->ProfileX()->Draw("same");
    gPad->SetLogz();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(6);
  hname = Form("hDutDTLH_DD_Missed_%d", iDut);
  h2m   = (TH2*) gROOT->FindObjectAny(hname);
  if (h2m != NULL) {
    h2m->Draw("colz");
    h2m->ProfileX()->Draw("same");
    gPad->SetLogz();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(7);
  hname = Form("hDutDTLH_TIS_%d", iDut);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2m != NULL) {
    h2->Draw("colz");
    h2->ProfileX()->Draw("same");
    gPad->SetLogz();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(8);
  TH2* DDeff = (TH2*) h2f->Clone("DDeff");
  DDeff->Add(h2f, h2m, 1., 1.);
  DDeff->Divide(h2f, DDeff, 1., 1., "B");
  DDeff->Draw("colz");

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  gInterpreter->ProcessLine(FADD.Data());

  can->SaveAs(Form("pl_Dut_DTLH_%d.pdf", iDut));
}
