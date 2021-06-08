/* Copyright (C) 2017-2018 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_Datime(TString sysinfo = "")
{
  TCanvas* can = (TCanvas*) gROOT->FindObjectAny("can");
  //cout <<" got can = "<<can<<endl;
  if (NULL == can) return;
  //can->Draw();
  can->cd();

  /*
    https://root-forum.cern.ch/t/writing-text-on-foreground-on-a-canvas/17435/2
*/
  TPad* newpad = new TPad("newpad", "a transparent pad", 0, 0, 1, 1);
  newpad->SetFillStyle(4000);
  newpad->Draw();
  newpad->cd();
  /*
TPaveLabel *title = new TPaveLabel(0.1,0.94,0.9,0.98,"Drawing options for one dimensional histograms");
title->SetFillColor(16);
title->SetTextFont(52);
title->Draw();
*/
  TDatime* pTime = new TDatime();
  //cout << " add time stamp to canvas " <<pTime->GetDate() << ", "<<pTime->GetTime() <<endl;
  TPaveText* pt = new TPaveText(.90, .975, .99, .99, "blNDC");
  pt->SetBorderSize(0);
  pt->SetFillColor(0);
  pt->AddText(Form("%d,%d", pTime->GetDate(), pTime->GetTime()));
  pt->Draw();
  if (sysinfo != "") {
    cout << " add to canvas: sysinfo = " << sysinfo.Data() << endl;
    TPaveText* st = new TPaveText(.96, .9, .995, .2, "blNDC");
    st->SetBorderSize(0);
    st->SetFillColor(0);
    TText* text = (TText*) st->AddText(Form("%s", sysinfo.Data()));
    text->SetTextAngle(-90);
    text->SetTextSize(0.016);
    text->SetTextColor(4);
    st->Draw();
  }
}
