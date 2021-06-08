/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Anna Senger [committer] */

void draw_histo(TString input = "sis100_muon_lmvm/8gev/centr_010_sup_histo.root")
{
  gStyle->SetHistLineWidth(6);
  gStyle->SetOptStat(0);

  Int_t type;

  if (input.Contains("sup")) type = 0;
  else if (input.Contains("eff"))
    type = 1;
  else if (input.Contains("YPt"))
    type = 2;

  TFile* f = new TFile(input);

  if (type == 0) {

    TH1D* h1 = (TH1D*) f->Get("BgSup_Vtx");
    h1->SetLineWidth(6);
    TH1D* h2 = (TH1D*) f->Get("BgSup_VtxSts");
    h2->SetLineWidth(6);
    TH1D* h3 = (TH1D*) f->Get("BgSup_VtxStsMuch");
    h3->SetLineWidth(6);
    TH1D* h4 = (TH1D*) f->Get("BgSup_VtxStsMuchTrd");
    h4->SetLineWidth(6);
    TH1D* h5 = (TH1D*) f->Get("BgSup_VtxStsMuchTrdTof");
    h5->SetLineWidth(6);

    TLegend* legend = legend = new TLegend(0.2, 0.7, 0.48, 0.9);
    legend->SetHeader("Background suppression:", "");
    legend->AddEntry(h1, "after vertex (Vtx) cut", "l");
    legend->AddEntry(h2, "after Vtx+STS cuts", "l");
    legend->AddEntry(h3, "after Vtx+STS+MUCH cuts", "l");
    legend->AddEntry(h4, "after Vtx+STS+MUCH+TRD cuts", "l");
    legend->AddEntry(h5, "after Vtx+STS+MUCH+TRD+TOF cuts", "l");

    TCanvas* myc1 = new TCanvas("myc1", "myc1");
    gPad->SetLogy();
    gPad->SetGridx();
    gPad->SetGridy();
    gStyle->SetOptTitle(0);

    h5->GetYaxis()->SetRangeUser(0.1, 1e6);
    h5->Draw("");
    h4->Draw("same");
    h3->Draw("same");
    h2->Draw("same");
    h1->Draw("same");

    legend->Draw();
  }
  else if (type == 1) {

    TProfile* p1 = (TProfile*) f->Get("mu Plus/accepted mu Plus/muPl_accP_Sts");
    TProfile* p2 = (TProfile*) f->Get("mu Plus/accepted mu Plus/muPl_accP_StsMuch");
    TProfile* p3 = (TProfile*) f->Get("mu Plus/accepted mu Plus/muPl_accP_StsMuchTrd");
    TProfile* p4 = (TProfile*) f->Get("mu Plus/accepted mu Plus/muPl_accP_StsMuchTrdTof");

    p1->Add((TProfile*) f->Get("mu Minus/accepted mu Minus/muMn_accP_Sts"));
    p2->Add((TProfile*) f->Get("mu Minus/accepted mu Minus/muMn_accP_StsMuch"));
    p3->Add((TProfile*) f->Get("mu Minus/accepted mu Minus/muMn_accP_StsMuchTrd"));
    p4->Add((TProfile*) f->Get("mu Minus/accepted mu Minus/muMn_accP_StsMuchTrdTof"));

    TProfile* p1a = (TProfile*) f->Get("signal/accepted signal/signal_accP_Sts");
    TProfile* p2a = (TProfile*) f->Get("signal/accepted signal/signal_accP_StsMuch");
    TProfile* p3a = (TProfile*) f->Get("signal/accepted signal/signal_accP_StsMuchTrd");
    TProfile* p4a = (TProfile*) f->Get("signal/accepted signal/signal_accP_StsMuchTrdTof");

    //-------------------------------------------------------------------------------

    TProfile* pp1 = (TProfile*) f->Get("mu Plus/reconstructed mu Plus/muPl_effRecoP_VtxSts");
    TProfile* pp2 = (TProfile*) f->Get("mu Plus/reconstructed mu Plus/muPl_effRecoP_VtxStsMuch");
    TProfile* pp3 = (TProfile*) f->Get("mu Plus/reconstructed mu Plus/muPl_effRecoP_VtxStsMuchTrd");
    TProfile* pp4 = (TProfile*) f->Get("mu Plus/reconstructed mu Plus/muPl_effRecoP_VtxStsMuchTrdTof");

    pp1->Add((TProfile*) f->Get("mu Minus/reconstructed mu Minus/muMn_effRecoP_VtxSts"));
    pp2->Add((TProfile*) f->Get("mu Minus/reconstructed mu Minus/muMn_effRecoP_VtxStsMuch"));
    pp3->Add((TProfile*) f->Get("mu Minus/reconstructed mu Minus/muMn_effRecoP_VtxStsMuchTrd"));
    pp4->Add((TProfile*) f->Get("mu Minus/reconstructed mu Minus/muMn_effRecoP_VtxStsMuchTrdTof"));

    TProfile* pp1a = (TProfile*) f->Get("signal/reconstructed signal/signal_effRecoP_VtxSts");
    TProfile* pp2a = (TProfile*) f->Get("signal/reconstructed signal/signal_effRecoP_VtxStsMuch");
    TProfile* pp3a = (TProfile*) f->Get("signal/reconstructed signal/signal_effRecoP_VtxStsMuchTrd");
    TProfile* pp4a = (TProfile*) f->Get("signal/reconstructed signal/signal_effRecoP_VtxStsMuchTrdTof");

    //-------------------------------------------------------------------------------

    TProfile* ppp0 = (TProfile*) f->Get("mu Plus/reconstructed mu Plus/muPl_eff4piP_Vtx");
    TProfile* ppp1 = (TProfile*) f->Get("mu Plus/reconstructed mu Plus/muPl_eff4piP_VtxSts");
    TProfile* ppp2 = (TProfile*) f->Get("mu Plus/reconstructed mu Plus/muPl_eff4piP_VtxStsMuch");
    TProfile* ppp3 = (TProfile*) f->Get("mu Plus/reconstructed mu Plus/muPl_eff4piP_VtxStsMuchTrd");
    TProfile* ppp4 = (TProfile*) f->Get("mu Plus/reconstructed mu Plus/muPl_eff4piP_VtxStsMuchTrdTof");

    ppp0->Add((TProfile*) f->Get("mu Minus/reconstructed mu Minus/muMn_eff4piP_Vtx"));
    ppp1->Add((TProfile*) f->Get("mu Minus/reconstructed mu Minus/muMn_eff4piP_VtxSts"));
    ppp2->Add((TProfile*) f->Get("mu Minus/reconstructed mu Minus/muMn_eff4piP_VtxStsMuch"));
    ppp3->Add((TProfile*) f->Get("mu Minus/reconstructed mu Minus/muMn_eff4piP_VtxStsMuchTrd"));
    ppp4->Add((TProfile*) f->Get("mu Minus/reconstructed mu Minus/muMn_eff4piP_VtxStsMuchTrdTof"));

    TProfile* ppp0a = (TProfile*) f->Get("signal/reconstructed signal/signal_eff4piP_Vtx");
    TProfile* ppp1a = (TProfile*) f->Get("signal/reconstructed signal/signal_eff4piP_VtxSts");
    TProfile* ppp2a = (TProfile*) f->Get("signal/reconstructed signal/signal_eff4piP_VtxStsMuch");
    TProfile* ppp3a = (TProfile*) f->Get("signal/reconstructed signal/signal_eff4piP_VtxStsMuchTrd");
    TProfile* ppp4a = (TProfile*) f->Get("signal/reconstructed signal/signal_eff4piP_VtxStsMuchTrdTof");

    //-------------------------------------------------------------------------------

    ppp0->SetLineColor(kBlack);
    ppp0->SetLineWidth(6);
    ppp0a->SetLineColor(kBlack);
    ppp0a->SetLineWidth(6);

    p1->SetLineColor(kRed);
    p1->SetMarkerColor(kRed);
    p1->SetLineWidth(6);

    pp1->SetLineColor(kRed);
    pp1->SetMarkerColor(kRed);
    pp1->SetLineWidth(6);

    ppp1->SetLineColor(kRed);
    ppp1->SetMarkerColor(kRed);
    ppp1->SetLineWidth(6);

    p2->SetLineColor(kBlue);
    p2->SetMarkerColor(kBlue);
    p2->SetLineWidth(6);

    pp2->SetLineColor(kBlue);
    pp2->SetMarkerColor(kBlue);
    pp2->SetLineWidth(6);

    ppp2->SetLineColor(kBlue);
    ppp2->SetMarkerColor(kBlue);
    ppp2->SetLineWidth(6);

    p3->SetLineColor(kGreen);
    p3->SetMarkerColor(kGreen);
    p3->SetLineWidth(6);

    pp3->SetLineColor(kGreen);
    pp3->SetMarkerColor(kGreen);
    pp3->SetLineWidth(6);

    ppp3->SetLineColor(kGreen);
    ppp3->SetMarkerColor(kGreen);
    ppp3->SetLineWidth(6);

    p4->SetLineColor(kMagenta);
    p4->SetMarkerColor(kMagenta);
    p4->SetLineWidth(6);

    pp4->SetLineColor(kMagenta);
    pp4->SetMarkerColor(kMagenta);
    pp4->SetLineWidth(6);

    ppp4->SetLineColor(kMagenta);
    ppp4->SetMarkerColor(kMagenta);
    ppp4->SetLineWidth(6);

    p1a->SetLineColor(kRed);
    p1a->SetMarkerColor(kRed);
    p1a->SetLineWidth(6);

    pp1a->SetLineColor(kRed);
    pp1a->SetMarkerColor(kRed);
    pp1a->SetLineWidth(6);

    ppp1a->SetLineColor(kRed);
    ppp1a->SetMarkerColor(kRed);
    ppp1a->SetLineWidth(6);

    p2a->SetLineColor(kBlue);
    p2a->SetMarkerColor(kBlue);
    p2a->SetLineWidth(6);

    pp2a->SetLineColor(kBlue);
    pp2a->SetMarkerColor(kBlue);
    pp2a->SetLineWidth(6);

    ppp2a->SetLineColor(kBlue);
    ppp2a->SetMarkerColor(kBlue);
    ppp2a->SetLineWidth(6);

    p3a->SetLineColor(kGreen);
    p3a->SetMarkerColor(kGreen);
    p3a->SetLineWidth(6);

    pp3a->SetLineColor(kGreen);
    pp3a->SetMarkerColor(kGreen);
    pp3a->SetLineWidth(6);

    ppp3a->SetLineColor(kGreen);
    ppp3a->SetMarkerColor(kGreen);
    ppp3a->SetLineWidth(6);

    p4a->SetLineColor(kMagenta);
    p4a->SetMarkerColor(kMagenta);
    p4a->SetLineWidth(6);

    pp4a->SetLineColor(kMagenta);
    pp4a->SetMarkerColor(kMagenta);
    pp4a->SetLineWidth(6);

    ppp4a->SetLineColor(kMagenta);
    ppp4a->SetMarkerColor(kMagenta);
    ppp4a->SetLineWidth(6);

    //-------------------------------------------------------------------------------

    TLegend* legend1 = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend1->SetHeader("acceptance:", "");
    legend1->AddEntry(p1, "STS", "l");
    legend1->AddEntry(p2, "STS+MUCH", "l");
    legend1->AddEntry(p3, "STS+MUCH+TRD", "l");
    legend1->AddEntry(p4, "STS+MUCH+TRD+TOF", "l");

    TCanvas* myc1 = new TCanvas("myc1", "Acceptance");
    myc1->Divide(2, 1);
    myc1->cd(1);
    gPad->SetLogy();
    gPad->SetGridx();
    gPad->SetGridy();
    p1->GetYaxis()->SetRangeUser(0.1, 120);
    p1->SetTitle("single muons from #omega");
    p1->Draw();
    p2->Draw("same");
    p3->Draw("same");
    p4->Draw("same");
    legend1->Draw();

    myc1->cd(2);
    gPad->SetLogy();
    gPad->SetGridx();
    gPad->SetGridy();
    p1a->GetYaxis()->SetRangeUser(0.1, 120);
    p1a->SetTitle("#omega");
    p1a->Draw();
    p2a->Draw("same");
    p3a->Draw("same");
    p4a->Draw("same");
    legend1->Draw();

    //-------------------------------------------------------------------------------

    TLegend* legend2 = new TLegend(0.4, 0.7, 0.9, 0.9);
    legend2->SetHeader("reconstructed/accepted:", "");
    legend2->AddEntry(pp1, "after Vtx+STS cuts", "l");
    legend2->AddEntry(pp2, "after Vtx+STS+MUCH cuts", "l");
    legend2->AddEntry(pp3, "after Vtx+STS+MUCH+TRD cuts", "l");
    legend2->AddEntry(pp4, "after Vtx+STS+MUCH+TRD+TOF cuts", "l");

    TCanvas* myc2 = new TCanvas("myc2", "Reconstruction efficiency");
    myc2->Divide(2, 1);
    myc2->cd(1);
    gPad->SetLogy();
    gPad->SetGridx();
    gPad->SetGridy();
    pp1->GetYaxis()->SetRangeUser(0.1, 9e2);
    pp1->SetTitle("single muons from #omega");
    pp1->Draw();
    pp2->Draw("same");
    pp3->Draw("same");
    pp4->Draw("same");
    legend2->Draw();

    myc2->cd(2);
    gPad->SetLogy();
    gPad->SetGridx();
    gPad->SetGridy();
    pp1a->GetYaxis()->SetRangeUser(0.1, 9e2);
    pp1a->SetTitle("#omega");
    pp1a->Draw();
    pp2a->Draw("same");
    pp3a->Draw("same");
    pp4a->Draw("same");
    legend2->Draw();

    //-------------------------------------------------------------------------------

    TLegend* legend3 = new TLegend(0.4, 0.7, 0.9, 0.9);
    legend3->SetHeader("reconstructed/4#pi:", "");
    legend3->AddEntry(ppp0, "after Vtx cut", "l");
    legend3->AddEntry(ppp1, "after Vtx+STS cuts", "l");
    legend3->AddEntry(ppp2, "after Vtx+STS+MUCH cuts", "l");
    legend3->AddEntry(ppp3, "after Vtx+STS+MUCH+TRD cuts", "l");
    legend3->AddEntry(ppp4, "after Vtx+STS+MUCH+TRD+TOF cuts", "l");

    TCanvas* myc3 = new TCanvas("myc3", "Reconstruction efficiency");
    myc3->Divide(2, 1);
    myc3->cd(1);
    gPad->SetLogy();
    gPad->SetGridx();
    gPad->SetGridy();
    ppp0->GetYaxis()->SetRangeUser(0.1, 9e2);
    ppp0->SetTitle("single muons from #omega");
    ppp0->Draw();
    ppp1->Draw("same");
    ppp2->Draw("same");
    ppp3->Draw("same");
    ppp4->Draw("same");
    legend3->Draw();

    myc3->cd(2);
    gPad->SetLogy();
    gPad->SetGridx();
    gPad->SetGridy();
    ppp0a->GetYaxis()->SetRangeUser(0.1, 9e2);
    ppp0a->SetTitle("#omega");
    ppp0a->Draw();
    ppp1a->Draw("same");
    ppp2a->Draw("same");
    ppp3a->Draw("same");
    ppp4a->Draw("same");
    legend3->Draw();
  }
  else if (type == 2) {
    TH2D* h0 = (TH2D*) f->Get("YPt_pluto");

    TH2D* h1 = (TH2D*) f->Get("YPt_StsAcc");
    TH2D* h2 = (TH2D*) f->Get("YPt_StsMuchAcc");
    TH2D* h3 = (TH2D*) f->Get("YPt_StsMuchTrdAcc");
    TH2D* h4 = (TH2D*) f->Get("YPt_StsMuchTrdTofAcc");

    TH2D* h0a = (TH2D*) f->Get("YPt_VtxReco");
    TH2D* h1a = (TH2D*) f->Get("YPt_VtxStsReco");
    TH2D* h2a = (TH2D*) f->Get("YPt_VtxStsMuchReco");
    TH2D* h3a = (TH2D*) f->Get("YPt_VtxStsMuchTrdReco");
    TH2D* h4a = (TH2D*) f->Get("YPt_VtxStsMuchTrdTofReco");

    Double_t max = h0->GetMaximum();
    Double_t min = 1e-7;

    h0->SetMaximum(max);
    h1->SetMaximum(max);
    h2->SetMaximum(max);
    h3->SetMaximum(max);
    h4->SetMaximum(max);

    h0->SetMinimum(min);
    h1->SetMinimum(min);
    h2->SetMinimum(min);
    h3->SetMinimum(min);
    h4->SetMinimum(min);

    h0->GetYaxis()->SetRangeUser(0, 3);
    h1->GetYaxis()->SetRangeUser(0, 3);
    h2->GetYaxis()->SetRangeUser(0, 3);
    h3->GetYaxis()->SetRangeUser(0, 3);
    h4->GetYaxis()->SetRangeUser(0, 3);

    TCanvas* myc1 = new TCanvas("myc1", "Acceptance");
    myc1->Divide(5, 1);
    myc1->cd(1);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h0->Draw("colz");
    myc1->cd(2);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h1->Draw("colz");
    myc1->cd(3);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h2->Draw("colz");
    myc1->cd(4);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h3->Draw("colz");
    myc1->cd(5);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h4->Draw("colz");

    //-------------------------------------------

    h0a->SetMaximum(max);
    h1a->SetMaximum(max);
    h2a->SetMaximum(max);
    h3a->SetMaximum(max);
    h4a->SetMaximum(max);

    h0a->SetMinimum(min);
    h1a->SetMinimum(min);
    h2a->SetMinimum(min);
    h3a->SetMinimum(min);
    h4a->SetMinimum(min);

    h0a->GetYaxis()->SetRangeUser(0, 3);
    h1a->GetYaxis()->SetRangeUser(0, 3);
    h2a->GetYaxis()->SetRangeUser(0, 3);
    h3a->GetYaxis()->SetRangeUser(0, 3);
    h4a->GetYaxis()->SetRangeUser(0, 3);

    TCanvas* myc1a = new TCanvas("myc1a", "Reconstructed");
    myc1a->Divide(5, 1);
    myc1a->cd(1);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h0a->Draw("colz");
    myc1a->cd(2);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h1a->Draw("colz");
    myc1a->cd(3);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h2a->Draw("colz");
    myc1a->cd(4);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h3a->Draw("colz");
    myc1a->cd(5);
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();
    h4a->Draw("colz");
  }
}
