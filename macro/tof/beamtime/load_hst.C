void load_hst(TString cstr="hst/default.hst.root"){
 // Read histogramms from the file
 TFile fHist = TFile(cstr,"READ");
 TIter next(fHist.GetListOfKeys());
 
 gROOT->cd();
 gDirectory->pwd();

  TH1 *h;
  TObject* obj;
  while(obj= (TObject*)next()){
    // cout << "Inspect object " << obj->GetName() << " Inherits "<< obj->InheritsFrom(TH1::Class())<< endl;
    // if(obj->InheritsFrom(TH1::Class())){
    h = (TH1*)obj;
    cout << "Load histo " << h->GetName() << endl;
    TH1 *hn = (TH1 *)fHist.Get(h->GetName());
    TH1 *hnn = hn->Clone();
    if(NULL != hnn) hnn->Draw();
    //  }
  }
 
  fHist.Close();
}
