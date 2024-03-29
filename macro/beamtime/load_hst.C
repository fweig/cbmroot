/* Copyright (C) 2017-2018 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Christian Simon, Norbert Herrmann [committer] */

void load_hst(TString cstr = "hst/default.hst.root")
{
  // Read histogramms from the file
  TFile* fHist = TFile::Open(cstr, "READ");
  if (NULL == fHist) {
    cout << " File " << cstr.Data() << " not existing " << endl;
    return;
  }
  TIter next(fHist->GetListOfKeys());

  gROOT->cd();
  gDirectory->pwd();

  TObject* obj;
  TKey* key;
  TClass* cls;
  while ((key = (TKey*) next())) {
    cls = TClass::GetClass(key->GetClassName());
    if (cls->InheritsFrom(TH1::Class())) {
      obj = fHist->Get(key->GetName());
      dynamic_cast<TH1*>(obj)->SetDirectory(gROOT);
    }
    else if (cls->InheritsFrom(TEfficiency::Class())) {
      obj = fHist->Get(key->GetName());
      dynamic_cast<TEfficiency*>(obj)->SetDirectory(gROOT);
    }
  }

  fHist->Close();
}
