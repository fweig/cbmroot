const UInt_t   kuNbFiles = 4;
const Color_t	kcFileColor[kuNbFiles] = { kBlue, kRed, kGreen, kOrange };
const Int_t    kiLineWidth    = 2;
const Double_t kdLegTextSize  = 0.04;
const Double_t kdPadTitleSize = 0.1;

Bool_t qaPlotsComp( TString sFilenameA = "data/tofqa.sis300_electron_auau.25gev.centr._plots.hst.all.root",
                     TString sFilenameB = "data/tofqa.sis300_electron_auau.25gev.centr.noTRD_plots.hst.all.root",
                     TString sFilenameC = "data/tofqa.cbm100_pbar_auau.25gev.centr._plots.hst.all.root",
                     TString sFilenameD = "data/tofqa.cbm100_pbar_auau.25gev.centr.noTRD_plots.hst.all.root",
                     TString sTagA = "sis300e_Trd_25gev", TString sTagB = "sis300e_NoTrd_25gev",
                     TString sTagC = "cbm100p_Trd_25gev", TString sTagD = "cbm100p_NoTrd_25gev",
                     TString sOutFileTag = "_25gev" )
{
//   gStyle->SetPalette(51); TString sPalName = "DeepSea";// Deep sea (Dark blue to light blue)
//   gStyle->SetPalette(52); TString sPalName = "GreySca"; // Grey scale
//   gStyle->SetPalette(53); TString sPalName = "DarkBody"; // Dark body radiator (black to white through yellow)
//   gStyle->SetPalette(54); TString sPalName = "TwoCol"; // two-color hue (dark blue through neutral gray to bright yellow)
   gStyle->SetPalette(55); TString sPalName = "RainBow"; // Rain Bow
//   gStyle->SetPalette(56); TString sPalName = "DarkBodyInv"; // inverted Dark Body Radiator
   // Set size of PAD title for all pads
   gStyle->SetTitleW(0.9); //title width
   gStyle->SetTitleH(0.1); //title height 
   Double_t dMinRangeEffZ =  50.0;  
   Double_t dMaxRangeEffZ = 100.0;
   Double_t dMinRangePntZ =  35.0;  
   Double_t dMaxRangePntZ = 100.0;
   Double_t dMinRangeTrkZ =  60.0;  
   Double_t dMaxRangeTrkZ = 100.0;
   
   // Open the input files
   TFile * filePnt[kuNbFiles];
   filePnt[0] = new TFile( sFilenameA, "READ" );
   filePnt[1] = new TFile( sFilenameB, "READ" );
   filePnt[2] = new TFile( sFilenameC, "READ" );
   filePnt[3] = new TFile( sFilenameD, "READ" );
   TString sTagFiles[kuNbFiles];
   sTagFiles[0] = sTagA;
   sTagFiles[1] = sTagB;
   sTagFiles[2] = sTagC;
   sTagFiles[3] = sTagD;
   
   if( NULL == filePnt[0] || NULL == filePnt[1] || NULL == filePnt[2] || NULL == filePnt[3] )
   {
      cout<<"One of the input files could not be opened: "
          <<sFilenameA<<" -> "<<filePnt[0]<<" "
          <<sFilenameB<<" -> "<<filePnt[1]<<" "
          <<sFilenameC<<" -> "<<filePnt[2]<<" "
          <<sFilenameD<<" -> "<<filePnt[3]<<" "
          <<endl;
      return kFALSE;    
   } // if( NULL == filePnt[0] || NULL == filePnt[1] || NULL == filePnt[2] || NULL == filePnt[3] )
   
   // First recover pointers on input hitograms from files
   TH1 * hHitsEffMapAng[kuNbFiles];
   TH1 * hHitsRatioMapSingPntAng[kuNbFiles];
   TH1 * hHitsRatioMapSingTrkAng[kuNbFiles];
   TH1 * hHitsRatioMapMultTrkMultPntAng[kuNbFiles];
   
   TH2 * tempTwoDimHist = NULL;
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   {
      tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( "TofQa_HitsEffMapAng" ) );
      if( NULL != tempTwoDimHist )
         hHitsEffMapAng[uFile] = tempTwoDimHist;
         else return kFALSE;
         
      tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( "TofQa_HitsRatioMapSingPntAng" ) );
      if( NULL != tempTwoDimHist )
         hHitsRatioMapSingPntAng[uFile] = tempTwoDimHist;
         else return kFALSE;
         
      tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( "TofQa_HitsRatioMapSingTrkAng" ) );
      if( NULL != tempTwoDimHist )
         hHitsRatioMapSingTrkAng[uFile] = tempTwoDimHist;
         else return kFALSE;
         
      tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( "TofQa_HitsRatioMapMultTrkMultPntAng" ) );
      if( NULL != tempTwoDimHist )
         hHitsRatioMapMultTrkMultPntAng[uFile] = tempTwoDimHist;
         else return kFALSE;
   } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   
   // Prepare canvases, draw histos, save canvases
   TCanvas *canvHitsEffMap = new TCanvas( "canvHitsEffMap", 
            "Efficiency of Pnt->Hit conversion vs Ang. Pos., L = w/ TRD, R = w/o, T = v13, B = v14",
            1920, 986 );
   TCanvas *canvRatioMapSingPnt = new TCanvas( "canvRatioMapSingPnt", 
            "Fraction of Hits from a single Point vs Ang. Pos., L = w/ TRD, R = w/o, T = v13, B = v14",
            1920, 986 );
   TCanvas *canvRatioMapSingTrk = new TCanvas( "canvRatioMapSingTrk", 
            "Fraction of Hits from a single Track vs Ang. Pos., L = w/ TRD, R = w/o, T = v13, B = v14",
            1920, 986 );
   TCanvas *canvRatioMapMultMult = new TCanvas( "canvRatioMapMultMult", 
            "Fraction of Hits from multi. track among hits from multi. points, L = w/ TRD, R = w/o, T = v13, B = v14",
            1920, 986 );
            
   canvHitsEffMap->Divide(2, 2);
   canvRatioMapSingPnt->Divide(2, 2);
   canvRatioMapSingTrk->Divide(2, 2);
   canvRatioMapMultMult->Divide(2, 2);
   
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   {
      canvHitsEffMap->cd( 1 + uFile );
      hHitsEffMapAng[uFile]->SetStats(0);
      hHitsEffMapAng[uFile]->GetZaxis()->SetRangeUser( dMinRangeEffZ, dMaxRangeEffZ);
      hHitsEffMapAng[uFile]->Draw("colz");
      
      canvRatioMapSingPnt->cd( 1 + uFile );
      hHitsRatioMapSingPntAng[uFile]->SetStats(0);
      hHitsRatioMapSingPntAng[uFile]->GetZaxis()->SetRangeUser( dMinRangePntZ, dMaxRangePntZ);
      hHitsRatioMapSingPntAng[uFile]->Draw("colz");
      
      canvRatioMapSingTrk->cd( 1 + uFile );
      hHitsRatioMapSingTrkAng[uFile]->SetStats(0);
      hHitsRatioMapSingTrkAng[uFile]->GetZaxis()->SetRangeUser( dMinRangeTrkZ, dMaxRangeTrkZ);
      hHitsRatioMapSingTrkAng[uFile]->Draw("colz");
      
      canvRatioMapMultMult->cd( 1 + uFile );
      hHitsRatioMapMultTrkMultPntAng[uFile]->SetStats(0);
//      hHitsRatioMapMultTrkMultPntAng[uFile]->GetZaxis()->SetRangeUser( dMinRangeZ, dMaxRangeZ);
      hHitsRatioMapMultTrkMultPntAng[uFile]->Draw("colz");
   } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   
   canvHitsEffMap->SaveAs( Form("HitsEffMap_%s%s.png", sPalName.Data(), sOutFileTag.Data() ) );
   canvHitsEffMap->SaveAs( Form("HitsEffMap_%s%s.pdf", sPalName.Data(), sOutFileTag.Data() ) );
   
   canvRatioMapSingPnt->SaveAs( Form("MapFracSingPnt_%s%s.png", sPalName.Data(), sOutFileTag.Data() ) );
   canvRatioMapSingPnt->SaveAs( Form("MapFracSingPnt_%s%s.pdf", sPalName.Data(), sOutFileTag.Data() ) );
   
   canvRatioMapSingTrk->SaveAs( Form("MapFracSingTrk_%s%s.png", sPalName.Data(), sOutFileTag.Data() ) );
   canvRatioMapSingTrk->SaveAs( Form("MapFracSingTrk_%s%s.pdf", sPalName.Data(), sOutFileTag.Data() ) );
   
   canvRatioMapMultMult->SaveAs( Form("MapFracMultMult_%s%s.png", sPalName.Data(), sOutFileTag.Data() ) );
   canvRatioMapMultMult->SaveAs( Form("MapFracMultMult_%s%s.pdf", sPalName.Data(), sOutFileTag.Data() ) );
   
   
   return kTRUE;
}
