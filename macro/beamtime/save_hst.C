void save_hst(TString cstr = "status.hst.root", Bool_t bROOT = kTRUE) {
  cout << "save all histograms to file " << cstr.Data() << endl;
  TList* tList(NULL);
  if (bROOT) {
    tList = gROOT->GetList();
  } else {
    tList = gDirectory->GetList();
  }
  TIter next(tList);
  // Write objects to the file
  TFile* fHist = new TFile(cstr, "RECREATE");
  {
    TObject* obj;
    while ((obj = (TObject*) next())) {
      if (obj->InheritsFrom(TH1::Class())
          || obj->InheritsFrom(TEfficiency::Class())) {
        obj->Write();
      }
    }
  }
  // fHist->ls();
  fHist->Close();
}
