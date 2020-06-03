

Bool_t GeomHitMapComp()
{

   // Open the input files
   TFile * filePntOld = new TFile( "data/tofqa_sis100_electron_old_auau_10gev_centr__qa_hst_all.root", "READ" );
   TFile * filePntNew = new TFile( "data/tofqa_sis100_electron_auau_10gev_centr__qa_hst_all.root", "READ" );


   TH2 * hHitMapAngOld;
   TH2 * hHitMapAngNew;

   TH2 * tempTwoDimHist = NULL;
   tempTwoDimHist = (TH2*) (filePntOld->FindObjectAny( "TofTests_HitsMapAng" ) );
      if( NULL != tempTwoDimHist )
         hHitMapAngOld = (TH2*)(tempTwoDimHist->Clone( "TofTests_HitsMapAng_old" ) );
         else return kFALSE;
   tempTwoDimHist = (TH2*) (filePntNew->FindObjectAny( "TofTests_HitsMapAng" ) );
      if( NULL != tempTwoDimHist )
         hHitMapAngNew = (TH2*)(tempTwoDimHist->Clone( "TofTests_HitsMapAng_new" ) );
         else return kFALSE;

   TCanvas * pCanvas= new TCanvas( 
      "Canvas", "Hit map in angular coord. for old and new geometries" );
   pCanvas->Divide( 2);
   
   hHitMapAngOld->SetTitle("v13-5b");
   
   hHitMapAngNew->SetTitle("v16a_1e");
   
   pCanvas->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   hHitMapAngOld->Draw("colz");
   hHitMapAngOld->SetStats(0);
   hHitMapAngOld->GetZaxis()->SetRangeUser(0, 4500);
   
   pCanvas->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   hHitMapAngNew->Draw("colz");
   hHitMapAngNew->SetStats(0);
   hHitMapAngNew->GetZaxis()->SetRangeUser(0, 4500);
   
   return kTRUE;
}
