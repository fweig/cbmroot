void pl_over_clu(Int_t SmT=0, Int_t iSm=0, Int_t iRpc=0){
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2); 
  TCanvas *can = new TCanvas("can","can",50,0,800,800);
  can->Divide(3,4); 

gPad->SetFillColor(0);
gStyle->SetPalette(1);
gStyle->SetOptStat(kTRUE);

 gROOT->cd();
 gROOT->SetDirLevel(1);

 TH1 *h;
 TH2 *h2;
 // if (h!=NULL) h->Delete();

can->cd(1);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Size",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  h2->ProfileX()->Draw("same");
  gPad->SetLogz();
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(2);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Pos",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(3);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_TOff",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else 
   {
     cout << hname << " not found" << endl;
   }

can->cd(4);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Tot",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(5);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_AvWalk",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(6);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_AvLnWalk",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(7);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Mul",SmT,iSm,iRpc);
 h1=(TH1 *)gROOT->FindObjectAny(hname);
 if (h1!=NULL) {
  h1->Draw();
  gPad->SetLogy();
 }else { cout << hname << " not found" << endl;  }

can->cd(8);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_Trms",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(9);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_DelPos",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(10);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_DelTOff",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(11);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_DelMatPos",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

can->cd(12);
 gROOT->cd();
 TString hname=Form("cl_SmT%d_sm%03d_rpc%03d_DelMatTOff",SmT,iSm,iRpc);
 h2=(TH2 *)gROOT->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->Draw("colz");
  gPad->SetLogz();
  h2->ProfileX()->Draw("same");
 }else { cout << hname << " not found" << endl;  }

}
