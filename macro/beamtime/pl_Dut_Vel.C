void pl_Dut_Vel(const char* cDut = "900",
                Double_t Tstart  = 0.,
                Double_t Tend    = 50.,
                TString sysinfo  = "") {
  gROOT->LoadMacro("pl_Datime.C");
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 700, 700);
  can->Divide(1, 2);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);
  // cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

  TH1* h1;
  TH1* h1_1;
  TH1* h1_2;

  TString hname = "";

  can->cd(1);

  gROOT->cd();
  hname = Form("hDutVel_Found_%s", cDut);
  cout << " Look for histo " << hname << endl;
  h1_1 = (TH1*) gROOT->FindObjectAny(hname);
  if (h1_1 != NULL) {
    h1_1->Draw("");
  } else {
    cout << hname << " not found" << endl;
  }

  hname = Form("hDutVel_Missed_%s", cDut);
  h1_2  = (TH1*) gROOT->FindObjectAny(hname);
  if (h1_2 != NULL) {
    h1_2->Draw("same");
    h1_2->SetLineColor(kRed);
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  h1 = (TH1*) h1_1->Clone();
  h1->Add(h1_1, h1_2, 1., 1.);
  TEfficiency* pEffVel = new TEfficiency(*h1_1, *h1);
  pEffVel->SetTitle("Efficiency");
  pEffVel->Draw("AP");
  gPad->Update();
  auto graph = pEffVel->GetPaintedGraph();
  graph->GetXaxis()->SetRangeUser(Tstart, Tend);
  gPad->Update();


  can->SaveAs(Form("pl_Dut_Vel_%s.pdf", cDut));
}
