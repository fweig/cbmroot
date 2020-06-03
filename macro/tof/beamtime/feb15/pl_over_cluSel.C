void pl_over_cluSel(Int_t iSel=0, Int_t iSmT=0, Int_t iSm=0, Int_t iRpc=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",48,25,700,800);
  can->Divide(2,4); 

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  //  gStyle->SetOptStat(kTRUE);
  //  gStyle->SetOptStat(1111);
  //  gROOT->cd();
  //  gROOT->SetDirLevel(1);

  TH1 *h;
  TH1 *h1;
  TH2 *h2;
  Double_t dAvCluSize;

can->cd(1);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Size",iSmT,iSm,iRpc,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2->ProfileX("_pfx",1,h2->GetNbinsY())->Draw("same");
  Double_t dNbinsX=h2->GetNbinsX();
  h1=(TH1 *)h2->ProjectionY("cluSize",1,dNbinsX);
  dAvCluSize=h1->GetMean();
  gPad->SetLogz();
 }else  { cout << hname << " not found" << endl; }

can->cd(2);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Pos",iSmT,iSm,iRpc,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
  TH1D *h2posy=h2->ProjectionY();
 }else { cout << hname << " not found" << endl; }

can->cd(3);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_TOff",iSmT,iSm,iRpc,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
  TH1D * h2py=h2->ProjectionY("_py",0,h2->GetNbinsY()-1);
  //  cout << "Average TOff :"<<h2py->GetMean()<<endl;

 }else { cout << hname << " not found" << endl; }

can->cd(4);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Tot",iSmT,iSm,iRpc,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(5);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_AvWalk",iSmT,iSm,iRpc,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(6);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Mul",iSmT,iSm,iRpc,iSel);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
 }else { cout << hname << " not found" << endl;  }

can->cd(7);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_DelTof",iSmT,iSm,iRpc,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  TProfile *h2pfx=h2->ProfileX();
  h2pfx->Draw("same");
  TH1D * h2tpy=h2->ProjectionY();

 }else { cout << hname << " not found" << endl;  }

can->cd(8);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_dXdY",iSmT,iSm,iRpc,iSel);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }

 // summarize status 
 cout << "<IStat> SmType "<< iSmT <<" Sm "<<iSm<<" Rpc "<<iRpc<<" Sel "<<iSel<<": average Cluster Size: "<<dAvCluSize 
      << ", #hits "<<h2py->GetEntries()
      << ", av. TOff :"<<h2py->GetMean()
      << ", RMS: "<< h2tpy->GetRMS()<<", av. Pos: "<<h2posy->GetMean()<<endl;

 can->SaveAs(Form("pl_over_cluSel%01d_%01d_%01d_%01d.pdf",iSel,iSmT,iSm,iRpc));

}
