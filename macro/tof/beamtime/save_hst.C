/** @file save_hst.C
  * @copyright Copyright (C) 2016-2017 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer], Christian Simon **/

void save_hst(TString cstr = "hst/default.hst.root")
{
  TIter next(gDirectory->GetList());
  // Write histogramms to the file
  TFile* fHist = new TFile(cstr, "RECREATE");
  {
    TH1* h;
    TObject* obj;
    while ((obj = (TObject*) next())) {
      if (obj->InheritsFrom(TH1::Class())) {
        h = (TH1*) obj;
        //        cout << "Write histo " << h->GetTitle() << endl;
        h->Write();
      }
    }
  }
  // fHist->ls();
  fHist->Close();
}
