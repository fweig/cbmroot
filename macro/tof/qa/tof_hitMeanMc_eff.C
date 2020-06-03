
Bool_t tof_hitMeanMc_eff( TString sFilename = "data/tofqa.cbm100_pbar_auau.25gev.centr._qa.hst.all.root",
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
   
   TFile *file    = TFile::Open( sFilename );
   
   TH2 * tempTwoDimHist = NULL;
   TH2 * fhRealPntMapAng;
   TH2 * fhHitMapAng;
   
   gROOT->cd();

   //-----------------------------------
   tempTwoDimHist = (TH2*) (file->FindObjectAny( "TofTests_RealPointsMapAng" ) );
   if( NULL != tempTwoDimHist )
      fhRealPntMapAng = (TH2*)(tempTwoDimHist->Clone( "RealPntMapAng" ) );
      else return kFALSE;

   //-----------------------------------
   tempTwoDimHist = (TH2*) (file->FindObjectAny( "TofTests_HitsMapAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapAng = (TH2*)(tempTwoDimHist->Clone( "HitMapAng" ) );
      else return kFALSE;
      
   //-----------------------------------
      
   file->Close();

   Int_t iNbBinThetaX  = 1200;
   Double_t dThetaXMin = -  60.0;
   Double_t dThetaXMax =    60.0;
   Int_t iNbBinThetaY  = 800;
   Double_t dThetaYMin = -  40.0;
   Double_t dThetaYMax =    40.0;
   TH2 * fhHitMeanPntAng = new TH2D( "TofQa_fHitMeanPntAng", 
         "Ratio of hits to mean MC points; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Eff. [\%]", 
         iNbBinThetaX, dThetaXMin, dThetaXMax, 
         iNbBinThetaY, dThetaYMin, dThetaYMax);
         
   fhHitMeanPntAng->Divide( fhHitMapAng, fhRealPntMapAng);
   fhHitMeanPntAng->Scale( 100.0 );
  
   TCanvas * canvEff = new TCanvas("canvEff", "Comparison of W/ and W/o TRD", 1920, 986  );
   
//   TCanvas * canvSingPnt = new TCanvas("CanvSingPnt");
//   canvSingPnt->cd();
   canvEff->cd(1);
   fhHitMeanPntAng->SetStats(0);
   fhHitMeanPntAng->GetZaxis()->SetRangeUser( dMinRangeZ, dMaxRangeZ);
   fhHitMeanPntAng->Draw("colz");
   
   canvEff->SaveAs( Form("EffHitMeanPnt_%s%s.png", sPalName.Data(), sOutFileTag.Data() ) );
   canvEff->SaveAs( Form("EffHitMeanPnt_%s%s.pdf", sPalName.Data(), sOutFileTag.Data() ) );
   
}
