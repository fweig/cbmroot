
const UInt_t   kuNbFiles = 4;
const UInt_t   kuNbDim   = 4;
const TString  ksDimName[kuNbDim] = { "X", "Y", "Z", "R"}; 
const Color_t	kcFileColor[kuNbFiles] = { kBlue, kRed, kGreen, kOrange };
const Int_t    kiLineWidth    = 2;
const Double_t kdLegTextSize  = 0.04;
const Double_t kdPadTitleSize = 0.1;

const UInt_t   kuNbStepsDist = 30;
const Double_t kdDistStep    =  1; //cm
const UInt_t   kuNbStepsTime = 10;
const Double_t kdTimeStep    = 100.0; //ps

Bool_t hitQualCompPull( TString sFilenameA = "data/tofqa.sis300_electron_auau.25gev.centr._qa.hst.all.root",
                     TString sFilenameB = "data/tofqa.sis300_electron_auau.25gev.centr.noTRD_qa.hst.all.root",
                     TString sFilenameC = "data/tofqa.cbm100_pbar_auau.25gev.centr._qa.hst.all.root",
                     TString sFilenameD = "data/tofqa.cbm100_pbar_auau.25gev.centr.noTRD_qa.hst.all.root",
                     TString sTagA = "sis300e_Trd_25gev", TString sTagB = "sis300e_NoTrd_25gev",
                     TString sTagC = "cbm100p_Trd_25gev", TString sTagD = "cbm100p_NoTrd_25gev",
                     TString sOutFileTag = "_25gev" )
{
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
   
   // First recover all input hitograms from files
   gROOT->cd();
   TH1 * hSinglePointHitPull[kuNbFiles][kuNbDim];
   TH2 * hMultiPntHitClosestPull[kuNbFiles][kuNbDim];
   TH2 * hMultiPntHitFurthestPull[kuNbFiles][kuNbDim];
   TH2 * hMultiPntHitMeanPull[kuNbFiles][kuNbDim];
   TH1 * hSingleTrackHitPull[kuNbFiles][kuNbDim];
   TH1 * hSingTrkMultiPntHitPull[kuNbFiles][kuNbDim];
   TH2 * hMultiTrkHitClosestPull[kuNbFiles][kuNbDim];
   TH2 * hMultiTrkHitFurthestPull[kuNbFiles][kuNbDim];
   TH2 * hMultiTrkHitMeanPull[kuNbFiles][kuNbDim];
   
   TH1 * hMultiPntHitClosestPull_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiPntHitFurthestPull_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiPntHitMeanPull_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiTrkHitClosestPull_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiTrkHitFurthestPull_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiTrkHitMeanPull_proj[kuNbFiles][kuNbDim];
   
   TH1 * hAllPntHitClosestPull[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitFurthestPull[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitMeanPull[kuNbFiles][kuNbDim];
   TH1 * hAllTrkHitClosestPull[kuNbFiles][kuNbDim];
   TH1 * hAllTrkHitFurthestPull[kuNbFiles][kuNbDim];
   TH1 * hAllTrkHitMeanPull[kuNbFiles][kuNbDim];
   
   TH1 * hRatioPntHitClosestPull[kuNbFiles][kuNbDim];
   TH1 * hRatioPntHitFurthestPull[kuNbFiles][kuNbDim];
   TH1 * hRatioPntHitMeanPull[kuNbFiles][kuNbDim];
   TH1 * hRatioTrkHitClosestPull[kuNbFiles][kuNbDim];
   TH1 * hRatioTrkHitFurthestPull[kuNbFiles][kuNbDim];
   TH1 * hRatioTrkHitMeanPull[kuNbFiles][kuNbDim];
   
   TH1 * tempOneDimHist = NULL;
   TH2 * tempTwoDimHist = NULL;
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
      {
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_SinglePointHitPull%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hSinglePointHitPull[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_SinglePointHitPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiPntHitClosestPull%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiPntHitClosestPull[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiPntHitClosestPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiPntHitFurthestPull%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiPntHitFurthestPull[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiPntHitFurthestPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiPntHitMeanPull%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiPntHitMeanPull[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiPntHitMeanPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_SingleTrackHitPull%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hSingleTrackHitPull[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_SingleTrackHitPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_SingTrkMultiPntHitPull%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hSingTrkMultiPntHitPull[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_SingTrkMultiPntHitPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiTrkHitClosestPull%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiTrkHitClosestPull[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiTrkHitClosestPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiTrkHitFurthestPull%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiTrkHitFurthestPull[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiTrkHitFurthestPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiTrkHitMeanPull%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiTrkHitMeanPull[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiTrkHitMeanPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         hMultiPntHitClosestPull_proj[uFile][uDim]  = hMultiPntHitClosestPull[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitClosestPull%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiPntHitFurthestPull_proj[uFile][uDim] = hMultiPntHitFurthestPull[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitFurthestPull%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiPntHitMeanPull_proj[uFile][uDim]     = hMultiPntHitMeanPull[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitMeanPull%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiTrkHitClosestPull_proj[uFile][uDim]  = hMultiTrkHitClosestPull[uFile][uDim]->ProjectionX(
               Form("%s_MultiTrkHitClosestPull%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiTrkHitFurthestPull_proj[uFile][uDim] = hMultiTrkHitFurthestPull[uFile][uDim]->ProjectionX(
               Form("%s_MultiTrkHitFurthestPull%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiTrkHitMeanPull_proj[uFile][uDim]     = hMultiTrkHitMeanPull[uFile][uDim]->ProjectionX(
               Form("%s_MultiTrkHitMeanPull%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );

         hAllPntHitClosestPull[uFile][uDim]  = (TH1*)(hMultiPntHitClosestPull_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitClosestPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitFurthestPull[uFile][uDim] = (TH1*)(hMultiPntHitFurthestPull_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitFurthestPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitMeanPull[uFile][uDim]     = (TH1*)(hMultiPntHitMeanPull_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitMeanPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllTrkHitClosestPull[uFile][uDim]  = (TH1*)(hMultiTrkHitClosestPull_proj[uFile][uDim]->Clone( 
                  Form("%s_AllTrkHitClosestPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllTrkHitFurthestPull[uFile][uDim] = (TH1*)(hMultiTrkHitFurthestPull_proj[uFile][uDim]->Clone( 
                  Form("%s_AllTrkHitFurthestPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllTrkHitMeanPull[uFile][uDim]     = (TH1*)(hMultiTrkHitMeanPull_proj[uFile][uDim]->Clone( 
                  Form("%s_AllTrkHitMeanPull%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) ); 
                  
         hAllPntHitClosestPull[uFile][uDim] ->Add( hSinglePointHitPull[uFile][uDim] );
         hAllPntHitFurthestPull[uFile][uDim]->Add( hSinglePointHitPull[uFile][uDim] );
         hAllPntHitMeanPull[uFile][uDim]    ->Add( hSinglePointHitPull[uFile][uDim] );
         hAllTrkHitClosestPull[uFile][uDim] ->Add( hSingleTrackHitPull[uFile][uDim] );
         hAllTrkHitFurthestPull[uFile][uDim]->Add( hSingleTrackHitPull[uFile][uDim] );
         hAllTrkHitMeanPull[uFile][uDim]    ->Add( hSingleTrackHitPull[uFile][uDim] );
      } // Loop on files and dimensions

   // Close files now that we have all histos
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      filePnt[uFile]->Close();
   
   // Display MultiPnt/MultiTrk Projections
   THStack * histosStackMultiPntProjClos[kuNbDim];
   THStack * histosStackMultiPntProjFurt[kuNbDim];
   THStack * histosStackMultiPntProjMean[kuNbDim];
   THStack * histosStackMultiTrkProjClos[kuNbDim];
   THStack * histosStackMultiTrkProjFurt[kuNbDim];
   THStack * histosStackMultiTrkProjMean[kuNbDim];
      
   TLegend * legMultiPntProjClos[kuNbDim];
   TLegend * legMultiPntProjFurt[kuNbDim];
   TLegend * legMultiPntProjMean[kuNbDim];
   TLegend * legMultiTrkProjClos[kuNbDim];
   TLegend * legMultiTrkProjFurt[kuNbDim];
   TLegend * legMultiTrkProjMean[kuNbDim];
               
   TCanvas *canvMultiProj[kuNbDim];

   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {
      histosStackMultiPntProjClos[uDim] = new THStack(Form("histosStackMultiPntProjClos%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Pnt, Hits from multi Pnt, Closest Pnt; Pull %s(Hit -> Point) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackMultiPntProjFurt[uDim] = new THStack(Form("histosStackMultiPntProjFurt%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Pnt, Hits from multi Pnt, Furthest Pnt; Pull %s(Hit -> Point) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackMultiPntProjMean[uDim] = new THStack(Form("histosStackMultiPntProjMean%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Pnt, Hits from multi Pnt, Mean Pnt; Pull %s(Hit -> Point) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Trk, Hits from multi Trk, Closest Trk; Pull %s(Hit -> Track) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Trk, Hits from multi Trk, Furthest Trk; Pull %s(Hit -> Track) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackMultiTrkProjMean[uDim] = new THStack(Form("histosStackMultiTrkProjMean%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Trk, Hits from multi Trk, Mean Trk; Pull %s(Hit -> Track) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );

      legMultiPntProjClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiPntProjFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiPntProjMean[uDim] = new TLegend(0.45,0.55,0.9,0.9); 
      legMultiTrkProjClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjMean[uDim] = new TLegend(0.45,0.55,0.9,0.9);     
      legMultiPntProjClos[uDim]->SetHeader( Form("Pull in %s, Closest",  ksDimName[uDim].Data() ) );
      legMultiPntProjFurt[uDim]->SetHeader( Form("Pull in %s, Furthest", ksDimName[uDim].Data() ) );
      legMultiPntProjMean[uDim]->SetHeader( Form("Pull in %s, Mean",     ksDimName[uDim].Data() ) );
      legMultiTrkProjClos[uDim]->SetHeader( Form("Pull in %s, Closest",  ksDimName[uDim].Data() ) );
      legMultiTrkProjFurt[uDim]->SetHeader( Form("Pull in %s, Furthest", ksDimName[uDim].Data() ) );
      legMultiTrkProjMean[uDim]->SetHeader( Form("Pull in %s, Mean",     ksDimName[uDim].Data() ) );
      legMultiPntProjClos[uDim]->SetTextSize( kdLegTextSize );
      legMultiPntProjFurt[uDim]->SetTextSize( kdLegTextSize );
      legMultiPntProjMean[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjClos[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjFurt[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjMean[uDim]->SetTextSize( kdLegTextSize );
                            
      canvMultiProj[uDim] = new TCanvas( Form("canvMultiProj%s",ksDimName[uDim].Data() ), 
            Form("Pull in %s, hits from mixed MC Points/tracks",ksDimName[uDim].Data() ),
            1920, 986 );
      canvMultiProj[uDim]->cd();
      canvMultiProj[uDim]->Divide(3,2);
   
      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {
         hMultiPntHitClosestPull_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiPntHitClosestPull_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiPntProjClos[uDim]->Add( hMultiPntHitClosestPull_proj[uFile][uDim] );
         legMultiPntProjClos[uDim]->AddEntry( hMultiPntHitClosestPull_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiPntHitFurthestPull_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiPntHitFurthestPull_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiPntProjFurt[uDim]->Add( hMultiPntHitFurthestPull_proj[uFile][uDim] );
         legMultiPntProjFurt[uDim]->AddEntry(hMultiPntHitFurthestPull_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiPntHitMeanPull_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiPntHitMeanPull_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiPntProjMean[uDim]->Add( hMultiPntHitMeanPull_proj[uFile][uDim] );
         legMultiPntProjMean[uDim]->AddEntry( hMultiPntHitMeanPull_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiTrkHitClosestPull_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiTrkHitClosestPull_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjClos[uDim]->Add( hMultiTrkHitClosestPull_proj[uFile][uDim] );
         legMultiTrkProjClos[uDim]->AddEntry( hMultiTrkHitClosestPull_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiTrkHitFurthestPull_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiTrkHitFurthestPull_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjFurt[uDim]->Add( hMultiTrkHitFurthestPull_proj[uFile][uDim] );
         legMultiTrkProjFurt[uDim]->AddEntry( hMultiTrkHitFurthestPull_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiTrkHitMeanPull_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiTrkHitMeanPull_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjMean[uDim]->Add( hMultiTrkHitMeanPull_proj[uFile][uDim] );
         legMultiTrkProjMean[uDim]->AddEntry( hMultiTrkHitMeanPull_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
      } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      
      canvMultiProj[uDim]->cd(1);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiPntProjClos[uDim]->Draw("nostack");
//      legMultiPntProjClos[uDim]->Draw();
      
      canvMultiProj[uDim]->cd(2);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiPntProjFurt[uDim]->Draw("nostack");
//      legMultiPntProjFurt[uDim]->Draw();
      
      canvMultiProj[uDim]->cd(3);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiPntProjMean[uDim]->Draw("nostack");
//      legMultiPntProjMean[uDim]->Draw();
      
      canvMultiProj[uDim]->cd(4);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiTrkProjClos[uDim]->Draw("nostack");
//      legMultiTrkProjClos[uDim]->Draw();
      
      canvMultiProj[uDim]->cd(5);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiTrkProjFurt[uDim]->Draw("nostack");
//      legMultiTrkProjFurt[uDim]->Draw();
      
      canvMultiProj[uDim]->cd(6);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiTrkProjMean[uDim]->Draw("nostack");
//      legMultiTrkProjMean[uDim]->Draw();
      
      canvMultiProj[uDim]->SaveAs( Form( "Pull_MultiProj%s%s.png", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
      canvMultiProj[uDim]->SaveAs( Form( "Pull_MultiProj%s%s.pdf", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)

   // Display Single Trk and Multi Pnt Single Trk histos
   THStack * histosStackSingTrk[kuNbDim];
   THStack * histosStackSingTrkMultiPnt[kuNbDim];
      
   TLegend * legSingTrk[kuNbDim];
   TLegend * legSingTrkMultiPnt[kuNbDim];
               
   TCanvas *canvSingTrk = new TCanvas( "canvSingTrk", 
            "Pull between Hit and Track, single tracks",
            1920, 986 );
   canvSingTrk->cd();
   canvSingTrk->Divide(3,2);
      
   TCanvas *canvSingTrkMultiPnt = new TCanvas( "canvSingTrkMultiPnt", 
            "Pull between Hit and Track, single tracks with multiple points",
            1920, 986 );
   canvSingTrkMultiPnt->cd();
   canvSingTrkMultiPnt->Divide(3,2);

   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {
      histosStackSingTrk[uDim] = new THStack(Form("histosStackSingTrk%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Pnt, Hits from single Trk; Pull %s(Hit -> Point) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackSingTrkMultiPnt[uDim] = new THStack(Form("histosStackSingTrkMultiPnt%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Trk, Hits from single Trk but multi Pnt; Pull %s(Hit -> Track) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );

      legSingTrk[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legSingTrkMultiPnt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legSingTrk[uDim]->SetHeader( Form("Pull in %s",  ksDimName[uDim].Data() ) );
      legSingTrkMultiPnt[uDim]->SetHeader( Form("Pull in %s",  ksDimName[uDim].Data() ) );
      legSingTrk[uDim]->SetTextSize( kdLegTextSize );
      legSingTrkMultiPnt[uDim]->SetTextSize( kdLegTextSize );
      
      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {
         hSingleTrackHitPull[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hSingleTrackHitPull[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackSingTrk[uDim]->Add( hSingleTrackHitPull[uFile][uDim] );
         legSingTrk[uDim]->AddEntry( hSingleTrackHitPull[uFile][uDim] , sTagFiles[uFile],"l");
         
         hSingTrkMultiPntHitPull[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hSingTrkMultiPntHitPull[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackSingTrkMultiPnt[uDim]->Add( hSingTrkMultiPntHitPull[uFile][uDim] );
         legSingTrkMultiPnt[uDim]->AddEntry( hSingTrkMultiPntHitPull[uFile][uDim] , sTagFiles[uFile],"l");
      } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      
      canvSingTrk->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackSingTrk[uDim]->Draw("nostack");
//      legSingTrk[uDim]->Draw();
      
      canvSingTrkMultiPnt->cd( 1 + uDim );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackSingTrkMultiPnt[uDim]->Draw("nostack");
//      legSingTrkMultiPnt[uDim]->Draw();
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
      
   canvSingTrk->SaveAs( Form( "Pull_SingTrk%s.png", sOutFileTag.Data() ) );
   canvSingTrk->SaveAs( Form( "Pull_SingTrk%s.pdf", sOutFileTag.Data() ) );
   
   canvSingTrkMultiPnt->SaveAs( Form( "Pull_SingTrkMultiPnt%s.png", sOutFileTag.Data() ) );
   canvSingTrkMultiPnt->SaveAs( Form( "Pull_SingTrkMultiPnt%s.pdf", sOutFileTag.Data() ) );
   
   // Display all Hits distrib
   THStack * histosStackAllPntHitClos[kuNbDim];
   THStack * histosStackAllPntHitFurt[kuNbDim];
   THStack * histosStackAllPntHitMean[kuNbDim];
   THStack * histosStackMultiTrkProjClos[kuNbDim];
   THStack * histosStackMultiTrkProjFurt[kuNbDim];
   THStack * histosStackMultiTrkProjMean[kuNbDim];
      
   TLegend * legAllPntHitClos[kuNbDim];
   TLegend * legAllPntHitFurt[kuNbDim];
   TLegend * legAllPntHitMean[kuNbDim];
   TLegend * legMultiTrkProjClos[kuNbDim];
   TLegend * legMultiTrkProjFurt[kuNbDim];
   TLegend * legMultiTrkProjMean[kuNbDim];
               
   TCanvas *canvAllPntHit[kuNbDim];

   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {
      histosStackAllPntHitClos[uDim] = new THStack(Form("histosStackAllPntHitClos%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Pnt, all Hits, Closest Pnt if Multi; Pull %s(Hit -> Point) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackAllPntHitFurt[uDim] = new THStack(Form("histosStackAllPntHitFurt%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Pnt, all Hits, Furthest Pnt if Multi; Pull %s(Hit -> Point) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackAllPntHitMean[uDim] = new THStack(Form("histosStackAllPntHitMean%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Pnt, all Hits, Mean Pnt if Multi; Pull %s(Hit -> Point) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Trk, all Hits, Closest Trk if Multi; Pull %s(Hit -> Track) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Trk, all Hits, Furthest Trk if Multi; Pull %s(Hit -> Track) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      histosStackMultiTrkProjMean[uDim] = new THStack(Form("histosStackMultiTrkProjMean%s",  ksDimName[uDim].Data()),
            Form("Pull in %s from Trk, all Hits, Mean Trk if Multi; Pull %s(Hit -> Track) []; Counts [Hits]",
                        ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );

      legAllPntHitClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legAllPntHitFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legAllPntHitMean[uDim] = new TLegend(0.45,0.55,0.9,0.9); 
      legMultiTrkProjClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjMean[uDim] = new TLegend(0.45,0.55,0.9,0.9);     
      legAllPntHitClos[uDim]->SetHeader( Form("Pull in %s, Closest",  ksDimName[uDim].Data() ) );
      legAllPntHitFurt[uDim]->SetHeader( Form("Pull in %s, Furthest", ksDimName[uDim].Data() ) );
      legAllPntHitMean[uDim]->SetHeader( Form("Pull in %s, Mean",     ksDimName[uDim].Data() ) );
      legMultiTrkProjClos[uDim]->SetHeader( Form("Pull in %s, Closest",  ksDimName[uDim].Data() ) );
      legMultiTrkProjFurt[uDim]->SetHeader( Form("Pull in %s, Furthest", ksDimName[uDim].Data() ) );
      legMultiTrkProjMean[uDim]->SetHeader( Form("Pull in %s, Mean",     ksDimName[uDim].Data() ) );
      legAllPntHitClos[uDim]->SetTextSize( kdLegTextSize );
      legAllPntHitFurt[uDim]->SetTextSize( kdLegTextSize );
      legAllPntHitMean[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjClos[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjFurt[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjMean[uDim]->SetTextSize( kdLegTextSize );
                            
      canvAllPntHit[uDim] = new TCanvas( Form("canvAllPntHit%s",ksDimName[uDim].Data() ), 
            Form("Pull in %s, all hits",ksDimName[uDim].Data() ),
            1920, 986 );
      canvAllPntHit[uDim]->cd();
      canvAllPntHit[uDim]->Divide(3,2);
   
      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {
         hAllPntHitClosestPull[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllPntHitClosestPull[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackAllPntHitClos[uDim]->Add( hAllPntHitClosestPull[uFile][uDim] );
         legAllPntHitClos[uDim]->AddEntry( hAllPntHitClosestPull[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllPntHitFurthestPull[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllPntHitFurthestPull[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackAllPntHitFurt[uDim]->Add( hAllPntHitFurthestPull[uFile][uDim] );
         legAllPntHitFurt[uDim]->AddEntry(hAllPntHitFurthestPull[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllPntHitMeanPull[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllPntHitMeanPull[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackAllPntHitMean[uDim]->Add( hAllPntHitMeanPull[uFile][uDim] );
         legAllPntHitMean[uDim]->AddEntry( hAllPntHitMeanPull[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllTrkHitClosestPull[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllTrkHitClosestPull[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjClos[uDim]->Add( hAllTrkHitClosestPull[uFile][uDim] );
         legMultiTrkProjClos[uDim]->AddEntry( hAllTrkHitClosestPull[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllTrkHitFurthestPull[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllTrkHitFurthestPull[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjFurt[uDim]->Add( hAllTrkHitFurthestPull[uFile][uDim] );
         legMultiTrkProjFurt[uDim]->AddEntry( hAllTrkHitFurthestPull[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllTrkHitMeanPull[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllTrkHitMeanPull[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjMean[uDim]->Add( hAllTrkHitMeanPull[uFile][uDim] );
         legMultiTrkProjMean[uDim]->AddEntry( hAllTrkHitMeanPull[uFile][uDim] , sTagFiles[uFile],"l");
         
      } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      
      canvAllPntHit[uDim]->cd(1);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackAllPntHitClos[uDim]->Draw("nostack");
//      legAllPntHitClos[uDim]->Draw();
      
      canvAllPntHit[uDim]->cd(2);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackAllPntHitFurt[uDim]->Draw("nostack");
//      legAllPntHitFurt[uDim]->Draw();
      
      canvAllPntHit[uDim]->cd(3);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackAllPntHitMean[uDim]->Draw("nostack");
//      legAllPntHitMean[uDim]->Draw();
      
      canvAllPntHit[uDim]->cd(4);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiTrkProjClos[uDim]->Draw("nostack");
//      legMultiTrkProjClos[uDim]->Draw();
      
      canvAllPntHit[uDim]->cd(5);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiTrkProjFurt[uDim]->Draw("nostack");
//      legMultiTrkProjFurt[uDim]->Draw();
      
      canvAllPntHit[uDim]->cd(6);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiTrkProjMean[uDim]->Draw("nostack");
//      legMultiTrkProjMean[uDim]->Draw();
      
      canvAllPntHit[uDim]->SaveAs( Form( "Pull_AllPntHit%s%s.png", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
      canvAllPntHit[uDim]->SaveAs( Form( "Pull_AllPntHit%s%s.pdf", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   
   return kTRUE;
}             
