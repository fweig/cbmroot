
Bool_t trd_notrd_comp( TString sFilenameTrd = "data/tofqa.cbm100_pbar_auau.25gev.centr._qa.hst.all.root",
                     TString sFilenameNoTrd = "data/tofqa.cbm100_pbar_auau.25gev.centr.noTRD_qa.hst.all.root",
                     TString sOutFileTag = "_cbm100_pbar_25gev"  )
{
//   gStyle->SetPalette(51); TString sPalName = "DeepSea";// Deep sea (Dark blue to light blue)
//   gStyle->SetPalette(52); TString sPalName = "GreySca"; // Grey scale
//   gStyle->SetPalette(53); TString sPalName = "DarkBody"; // Dark body radiator (black to white through yellow)
   gStyle->SetPalette(54); TString sPalName = "TwoCol"; // two-color hue (dark blue through neutral gray to bright yellow)
//   gStyle->SetPalette(55); TString sPalName = "RainBow"; // Rain Bow
//   gStyle->SetPalette(56); TString sPalName = "DarkBodyInv"; // inverted Dark Body Radiator
   // Set size of PAD title for all pads
   gStyle->SetTitleW(0.9); //title width
   gStyle->SetTitleH(0.1); //title height 
   Double_t dMinRangeZ =   0.0;  
   Double_t dMaxRangeZ = 200.0;
   
   TFile *fileTrd    = TFile::Open( sFilenameTrd );
   TFile *fileNotTrd = TFile::Open( sFilenameNoTrd );
   
   TH2 * tempTwoDimHist = NULL;
   TH2 * fhHitMapSingPntAngTrd;
   TH2 * fhHitMapSingPntAngNoTrd;
   TH2 * fhHitMapMultPntAngTrd;
   TH2 * fhHitMapMultPntAngNoTrd;
   TH2 * fhHitMapSingTrkAngTrd;
   TH2 * fhHitMapSingTrkAngNoTrd;
   TH2 * fhHitMapMultTrkAngTrd;
   TH2 * fhHitMapMultTrkAngNoTrd;
   
   gROOT->cd();

   //-----------------------------------
   tempTwoDimHist = (TH2*) (fileTrd->FindObjectAny( "TofTests_HitsMapSingPntAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingPntAngTrd = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingPntAng_Trd" ) );
      else return kFALSE;
   
   tempTwoDimHist = (TH2*) (fileNotTrd->FindObjectAny( "TofTests_HitsMapSingPntAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingPntAngNoTrd = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingPntAng_NoTrd" ) );
      else return kFALSE;

   //-----------------------------------
   tempTwoDimHist = (TH2*) (fileTrd->FindObjectAny( "TofTests_HitsMapMultPntAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultPntAngTrd = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultPntAng_Trd" ) );
      else return kFALSE;
   
   tempTwoDimHist = (TH2*) (fileNotTrd->FindObjectAny( "TofTests_HitsMapMultPntAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultPntAngNoTrd = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultPntAng_NoTrd" ) );
      else return kFALSE;

   //-----------------------------------
   tempTwoDimHist = (TH2*) (fileTrd->FindObjectAny( "TofTests_HitsMapSingTrkAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingTrkAngTrd = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingTrkAng_Trd" ) );
      else return kFALSE;
   
   tempTwoDimHist = (TH2*) (fileNotTrd->FindObjectAny( "TofTests_HitsMapSingTrkAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingTrkAngNoTrd = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingTrkAng_NoTrd" ) );
      else return kFALSE;

   //-----------------------------------
   tempTwoDimHist = (TH2*) (fileTrd->FindObjectAny( "TofTests_HitsMapMultTrkAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultTrkAngTrd = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultTrkAng_Trd" ) );
      else return kFALSE;
   
   tempTwoDimHist = (TH2*) (fileNotTrd->FindObjectAny( "TofTests_HitsMapMultTrkAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultTrkAngNoTrd = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultTrkAng_NoTrd" ) );
      else return kFALSE;
      
   //-----------------------------------
      
   fileTrd->Close();
   fileNotTrd->Close();

   Int_t iNbBinThetaX  = 1200;
   Double_t dThetaXMin = -  60.0;
   Double_t dThetaXMax =    60.0;
   Int_t iNbBinThetaY  = 800;
   Double_t dThetaYMin = -  40.0;
   Double_t dThetaYMax =    40.0;
   TH2 * fhTrdNoTrdSingPntAng = new TH2D( "TofQa_fhTrdNoTrdSingPntAng", 
         "Ratio of hits from single points, Trd VS NoTrd; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Ratio. [\%]", 
         iNbBinThetaX, dThetaXMin, dThetaXMax, 
         iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhTrdNoTrdMultiPntAng = new TH2D( "TofQa_fhTrdNoTrdMultiPntAng", 
         "Ratio of hits from multiple points, Trd VS NoTrd; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Ratio. [\%]", 
         iNbBinThetaX, dThetaXMin, dThetaXMax, 
         iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhTrdNoTrdSingTrkAng = new TH2D( "TofQa_fhTrdNoTrdSingTrkAng", 
         "Ratio of hits from single tracks, Trd VS NoTrd; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Ratio. [\%]", 
         iNbBinThetaX, dThetaXMin, dThetaXMax, 
         iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhTrdNoTrdMultiTrkAng = new TH2D( "TofQa_fhTrdNoTrdMultiTrkAng", 
         "Ratio of hits from multiple tracks, Trd VS NoTrd; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Ratio. [\%]", 
         iNbBinThetaX, dThetaXMin, dThetaXMax, 
         iNbBinThetaY, dThetaYMin, dThetaYMax);
         
   fhTrdNoTrdSingPntAng->Divide( fhHitMapSingPntAngTrd, fhHitMapSingPntAngNoTrd);
   fhTrdNoTrdSingPntAng->Scale( 100.0 );
   fhTrdNoTrdMultiPntAng->Divide( fhHitMapMultPntAngTrd, fhHitMapMultPntAngNoTrd);
   fhTrdNoTrdMultiPntAng->Scale( 100.0 );
         
   fhTrdNoTrdSingTrkAng->Divide( fhHitMapSingTrkAngTrd, fhHitMapSingTrkAngNoTrd);
   fhTrdNoTrdSingTrkAng->Scale( 100.0 );
   fhTrdNoTrdMultiTrkAng->Divide( fhHitMapMultTrkAngTrd, fhHitMapMultTrkAngNoTrd);
   fhTrdNoTrdMultiTrkAng->Scale( 100.0 );
  
   TCanvas * canvTrdComp = new TCanvas("canvTrdComp", "Comparison of W/ and W/o TRD", 1920, 986  );
   canvTrdComp->Divide( 2, 2);
   
//   TCanvas * canvSingPnt = new TCanvas("CanvSingPnt");
//   canvSingPnt->cd();
   canvTrdComp->cd(1);
   fhTrdNoTrdSingPntAng->SetStats(0);
   fhTrdNoTrdSingPntAng->GetZaxis()->SetRangeUser( dMinRangeZ, dMaxRangeZ);
   fhTrdNoTrdSingPntAng->Draw("colz");
   
//   TCanvas * canvMultPnt = new TCanvas("CanvMultPnt");
//   canvMultPnt->cd();
   canvTrdComp->cd(2);
   fhTrdNoTrdSingTrkAng->SetStats(0);
   fhTrdNoTrdSingTrkAng->GetZaxis()->SetRangeUser( dMinRangeZ, dMaxRangeZ);
   fhTrdNoTrdSingTrkAng->Draw("colz");
   
//   TCanvas * canvSingTrk = new TCanvas("CanvSingTrk");
//   canvSingTrk->cd();
   canvTrdComp->cd(3);
   fhTrdNoTrdMultiPntAng->SetStats(0);
   fhTrdNoTrdMultiPntAng->GetZaxis()->SetRangeUser( dMinRangeZ, dMaxRangeZ);
   fhTrdNoTrdMultiPntAng->Draw("colz");
   
//   TCanvas * canvMultTrk = new TCanvas("CanvMultTrk");
//   canvMultTrk->cd();
   canvTrdComp->cd(4);
   fhTrdNoTrdMultiTrkAng->SetStats(0);
   fhTrdNoTrdMultiTrkAng->GetZaxis()->SetRangeUser( dMinRangeZ, dMaxRangeZ);
   fhTrdNoTrdMultiTrkAng->Draw("colz");
   
   canvTrdComp->SaveAs( Form("TrdComp_%s%s.png", sPalName.Data(), sOutFileTag.Data() ) );
   canvTrdComp->SaveAs( Form("TrdComp_%s%s.pdf", sPalName.Data(), sOutFileTag.Data() ) );
   
}
