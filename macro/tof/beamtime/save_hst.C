void save_hst(TString cstr="hst/default.hst.root"){
 TIter next(gDirectory->GetList());
 // Write histogramms to the file
 TFile *fHist = new TFile(cstr,"RECREATE");
 {
  TH1 *h;
  TObject* obj;
  while((obj= (TObject*)next())){
      if(obj->InheritsFrom(TH1::Class())){
         h = (TH1*)obj;
 //        cout << "Write histo " << h->GetTitle() << endl;
         h->Write();
      }
  }
 }
 // fHist->ls();
 fHist->Close();
}
