const UInt_t   kuNbFiles = 4;
const UInt_t   kuNbDim   = 5;
const TString  ksDimName[kuNbDim] = { "X", "Y", "Z", "R", "T" }; 
const Color_t	kcFileColor[kuNbFiles] = { kBlue, kRed, kGreen, kOrange };
const Int_t    kiLineWidth    = 2;
const Double_t kdLegTextSize  = 0.04;
const Double_t kdPadTitleSize = 0.1;

const UInt_t   kuNbStepsDist = 12;
const Double_t kdDistStep    = 2.5; //cm
const UInt_t   kuNbStepsTime = 10;
const Double_t kdTimeStep    = 100.0; //ps

Bool_t hitProdComp( TString sFilenameA = "data/tofqa.sis300_electron_auau.25gev.centr._HitProd.hst.all.root",
                     TString sFilenameB = "data/tofqa.sis300_electron_auau.25gev.centr.noTRD_HitProd.hst.all.root",
                     TString sFilenameC = "data/tofqa.cbm100_pbar_auau.25gev.centr._HitProd.hst.all.root",
                     TString sFilenameD = "data/tofqa.cbm100_pbar_auau.25gev.centr.noTRD_HitProd.hst.all.root",
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
   TH1 * hSinglePointHitDelta[kuNbFiles][kuNbDim];
   TH1 * hDiffPointHitLeftDelta[kuNbFiles][kuNbDim];
   TH1 * hDiffPointHitRightDelta[kuNbFiles][kuNbDim];
   TH1 * hSingleTrackHitLeftDelta[kuNbFiles][kuNbDim];
   TH1 * hSingleTrackHitRightDelta[kuNbFiles][kuNbDim];
   TH1 * hDiffTrackHitLeftDelta[kuNbFiles][kuNbDim];
   TH1 * hDiffTrackHitRightDelta[kuNbFiles][kuNbDim];

   TH1 * hAllPntHitHpLeftDelta[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitHpRightDelta[kuNbFiles][kuNbDim];
   
   TH1 * tempOneDimHist = NULL;
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
      {
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("HitProd_SinglePointHitDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hSinglePointHitDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_SinglePointHitDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("HitProd_DiffPointHitLeftDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hDiffPointHitLeftDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_DiffPointHitLeftDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("HitProd_DiffPointHitRightDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hDiffPointHitRightDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_DiffPointHitRightDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("HitProd_SingleTrackHitLeftDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hSingleTrackHitLeftDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_SingleTrackHitLeftDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("HitProd_SingleTrackHitRightDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hSingleTrackHitRightDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_SingleTrackHitRightDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("HitProd_DiffTrackHitLeftDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hDiffTrackHitLeftDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_DiffTrackHitLeftDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("HitProd_DiffTrackHitRightDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hDiffTrackHitRightDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_DiffTrackHitRightDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
         
         
         hAllPntHitHpLeftDelta[uFile][uDim]  = (TH1*)(hDiffPointHitLeftDelta[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitHpLeftDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitHpRightDelta[uFile][uDim]  = (TH1*)(hDiffPointHitRightDelta[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitHpRightDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
                  
         hAllPntHitHpLeftDelta[uFile][uDim] ->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllPntHitHpRightDelta[uFile][uDim]->Add( hSinglePointHitDelta[uFile][uDim] );
      } // Loop on files and dimensions

   // Display SinglePnt, Multi Pnts, Single Trk and Multi Trk histos
   THStack * histosStackSingPnt[kuNbDim];
   THStack * histosStackMultiPntL[kuNbDim];
   THStack * histosStackMultiPntR[kuNbDim];
   THStack * histosStackSingTrkL[kuNbDim];
   THStack * histosStackSingTrkR[kuNbDim];
   THStack * histosStackMultiTrkL[kuNbDim];
   THStack * histosStackMultiTrkR[kuNbDim];
   THStack * histosStackAllPntL[kuNbDim];
   THStack * histosStackAllPntR[kuNbDim];
      
   TLegend * legSingPnt[kuNbDim];
   TLegend * legMultiPntL[kuNbDim];
   TLegend * legMultiPntR[kuNbDim];
   TLegend * legSingTrkL[kuNbDim];
   TLegend * legSingTrkR[kuNbDim];
   TLegend * legMultiTrkL[kuNbDim];
   TLegend * legMultiTrkR[kuNbDim];
   TLegend * legAllPntL[kuNbDim];
   TLegend * legAllPntR[kuNbDim];
               
   TCanvas *canvSingPnt = new TCanvas( "canvSingPnt", 
            "Distance between Hit and Point, single point",
            1920, 986 );
   TCanvas *canvMultiPntL = new TCanvas( "canvMultiPntL", 
            "Distance between Hit and Left Point, multiple points",
            1920, 986 );
   TCanvas *canvMultiPntR = new TCanvas( "canvMultiPntR", 
            "Distance between Hit and Right Point, multiple points",
            1920, 986 );
   TCanvas *canvSingTrkL = new TCanvas( "canvSingTrkL", 
            "Distance between Hit and Left Point, single trk but Multi Pnt",
            1920, 986 );
   TCanvas *canvSingTrkR = new TCanvas( "canvSingTrkR", 
            "Distance between Hit and Right Point, single trk but Multi Pnt",
            1920, 986 );
   TCanvas *canvMultiTrkL = new TCanvas( "canvMultiTrkL", 
            "Distance between Hit and Left Point, multiple tracks",
            1920, 986 );
   TCanvas *canvMultiTrkR = new TCanvas( "canvMultiTrkR", 
            "Distance between Hit and Right Point, multiple tracks",
            1920, 986 );
   TCanvas *canvAllPntL = new TCanvas( "canvAllPntL", 
            "Distance between Hit and Point, Left Point if multiple points",
            1920, 986 );
   TCanvas *canvAllPntR = new TCanvas( "canvAllPntR", 
            "Distance between Hit and Point, Right Point if multiple points",
            1920, 986 );
            
   canvSingPnt->cd();
   canvSingPnt->Divide(3,2);
   canvMultiPntL->cd();
   canvMultiPntL->Divide(3,2);
   canvMultiPntR->cd();
   canvMultiPntR->Divide(3,2);
   canvSingTrkL->cd();
   canvSingTrkL->Divide(3,2);
   canvSingTrkR->cd();
   canvSingTrkR->Divide(3,2);
   canvMultiTrkL->cd();
   canvMultiTrkL->Divide(3,2);
   canvMultiTrkR->cd();
   canvMultiTrkR->Divide(3,2);
   canvAllPntL->cd();
   canvAllPntL->Divide(3,2);
   canvAllPntR->cd();
   canvAllPntR->Divide(3,2);

   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {   
      if( "T" == ksDimName[uDim] )
      {
         histosStackSingPnt[uDim] = new THStack(Form("histosStackSingPnt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from single Pnt; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntL[uDim] = new THStack(Form("histosStackMultiPntL_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Left Pnt, Hits from multi Pnt; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntR[uDim] = new THStack(Form("histosStackMultiPntR_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Right Pnt, Hits from multi Pnt; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackSingTrkL[uDim] = new THStack(Form("histosStackSingTrkL_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Left Pnt, Hits from single Trk but multi Pnt; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackSingTrkR[uDim] = new THStack(Form("histosStackSingTrkR_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Right Pnt, Hits from single Trk but multi Pnt; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkL[uDim] = new THStack(Form("histosStackMultiTrkL_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Left Pnt, Hits from multi Trk; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkR[uDim] = new THStack(Form("histosStackMultiTrkR_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Right Pnt, Hits from multi Trk; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
                           
         histosStackAllPntL[uDim] = new THStack(Form("histosStackAllPntL_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Left Pnt if multi Pnt; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackAllPntR[uDim] = new THStack(Form("histosStackAllPntR_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Right Pnt if multi Pnt; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // if( "T" == ksDimName[uDim] )
      else
      {
         histosStackSingPnt[uDim] = new THStack(Form("histosStackSingPnt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from single Pnt; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntL[uDim] = new THStack(Form("histosStackMultiPntL_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Left Pnt, Hits from multi Pnt; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntR[uDim] = new THStack(Form("histosStackMultiPntR_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Right Pnt, Hits from multi Pnt; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackSingTrkL[uDim] = new THStack(Form("histosStackSingTrkL_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Left Pnt, Hits from single Trk but multi Pnt; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackSingTrkR[uDim] = new THStack(Form("histosStackSingTrkR_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Right Pnt, Hits from single Trk but multi Pnt; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkL[uDim] = new THStack(Form("histosStackMultiTrkL_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Left Pnt, Hits from multi Trk; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkR[uDim] = new THStack(Form("histosStackMultiTrkR_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Right Pnt, Hits from multi Trk; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
                           
         histosStackAllPntL[uDim] = new THStack(Form("histosStackAllPntL_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Left Pnt if multi Pnt; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackAllPntR[uDim] = new THStack(Form("histosStackAllPntR_%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Right Pnt if multi Pnt; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // else of if( "T" == ksDimName[uDim] )

      legSingPnt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legSingPnt[uDim]->SetHeader( Form("Dist. in %s",  ksDimName[uDim].Data() ) );
      legSingPnt[uDim]->SetTextSize( kdLegTextSize );
      legMultiPntL[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiPntL[uDim]->SetHeader( Form("Dist. in %s to left Pnt",  ksDimName[uDim].Data() ) );
      legMultiPntL[uDim]->SetTextSize( kdLegTextSize );
      legMultiPntR[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiPntR[uDim]->SetHeader( Form("Dist. in %s to right Pnt",  ksDimName[uDim].Data() ) );
      legMultiPntR[uDim]->SetTextSize( kdLegTextSize );
      legSingTrkL[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legSingTrkL[uDim]->SetHeader( Form("Dist. in %s to left Pnt",  ksDimName[uDim].Data() ) );
      legSingTrkL[uDim]->SetTextSize( kdLegTextSize );
      legSingTrkR[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legSingTrkR[uDim]->SetHeader( Form("Dist. in %s to right Pnt",  ksDimName[uDim].Data() ) );
      legSingTrkR[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkL[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkL[uDim]->SetHeader( Form("Dist. in %s to left Pnt",  ksDimName[uDim].Data() ) );
      legMultiTrkL[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkR[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkR[uDim]->SetHeader( Form("Dist. in %s to right Pnt",  ksDimName[uDim].Data() ) );
      legMultiTrkR[uDim]->SetTextSize( kdLegTextSize );
      
      legAllPntL[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legAllPntL[uDim]->SetHeader( Form("Dist. in %s to left Pnt",  ksDimName[uDim].Data() ) );
      legAllPntL[uDim]->SetTextSize( kdLegTextSize );
      legAllPntR[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legAllPntR[uDim]->SetHeader( Form("Dist. in %s to right Pnt",  ksDimName[uDim].Data() ) );
      legAllPntR[uDim]->SetTextSize( kdLegTextSize );
      
      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {
         hSinglePointHitDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hSinglePointHitDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackSingPnt[uDim]->Add( hSinglePointHitDelta[uFile][uDim] );
         legSingPnt[uDim]->AddEntry( hSinglePointHitDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hDiffPointHitLeftDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hDiffPointHitLeftDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiPntL[uDim]->Add( hDiffPointHitLeftDelta[uFile][uDim] );
         legMultiPntL[uDim]->AddEntry( hDiffPointHitLeftDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hDiffPointHitRightDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hDiffPointHitRightDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiPntR[uDim]->Add( hDiffPointHitRightDelta[uFile][uDim] );
         legMultiPntR[uDim]->AddEntry( hDiffPointHitRightDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hSingleTrackHitLeftDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hSingleTrackHitLeftDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackSingTrkL[uDim]->Add( hSingleTrackHitLeftDelta[uFile][uDim] );
         legSingTrkL[uDim]->AddEntry( hSingleTrackHitLeftDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hSingleTrackHitRightDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hSingleTrackHitRightDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackSingTrkR[uDim]->Add( hSingleTrackHitRightDelta[uFile][uDim] );
         legSingTrkR[uDim]->AddEntry( hSingleTrackHitRightDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hDiffTrackHitLeftDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hDiffTrackHitLeftDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkL[uDim]->Add( hDiffTrackHitLeftDelta[uFile][uDim] );
         legMultiTrkL[uDim]->AddEntry( hDiffTrackHitLeftDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hDiffTrackHitRightDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hDiffTrackHitRightDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkR[uDim]->Add( hDiffTrackHitRightDelta[uFile][uDim] );
         legMultiTrkR[uDim]->AddEntry( hDiffTrackHitRightDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllPntHitHpLeftDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllPntHitHpLeftDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackAllPntL[uDim]->Add( hAllPntHitHpLeftDelta[uFile][uDim] );
         legAllPntL[uDim]->AddEntry( hAllPntHitHpLeftDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllPntHitHpRightDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllPntHitHpRightDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackAllPntR[uDim]->Add( hAllPntHitHpRightDelta[uFile][uDim] );
         legAllPntR[uDim]->AddEntry( hAllPntHitHpRightDelta[uFile][uDim] , sTagFiles[uFile],"l");
      } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      
      canvSingPnt->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackSingPnt[uDim]->Draw("nostack");
//      legSingPnt[uDim]->Draw();
      
      canvMultiPntL->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiPntL[uDim]->Draw("nostack");
//      legMultiPntL[uDim]->Draw();
      
      canvMultiPntR->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiPntR[uDim]->Draw("nostack");
//      legMultiPntR[uDim]->Draw();
      
      canvSingTrkL->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackSingTrkL[uDim]->Draw("nostack");
//      legSingTrkL[uDim]->Draw();
      
      canvSingTrkR->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackSingTrkR[uDim]->Draw("nostack");
//      legSingTrkR[uDim]->Draw();
      
      canvMultiTrkL->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiTrkL[uDim]->Draw("nostack");
//      legMultiTrkL[uDim]->Draw();
      
      canvMultiTrkR->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackMultiTrkR[uDim]->Draw("nostack");
//      legMultiTrkR[uDim]->Draw();
      
      canvAllPntL->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackAllPntL[uDim]->Draw("nostack");
//      legAllPntL[uDim]->Draw();
      
      canvAllPntR->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackAllPntR[uDim]->Draw("nostack");
//      legAllPntR[uDim]->Draw();
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   
   canvSingPnt->SaveAs( Form( "HProd_SingPnt_%s.png", sOutFileTag.Data() ) );
   canvSingPnt->SaveAs( Form( "HProd_SingPnt_%s.pdf", sOutFileTag.Data() ) );
   
   canvMultiPntL->SaveAs( Form( "HProd_MultiPntL_%s.png", sOutFileTag.Data() ) );
   canvMultiPntL->SaveAs( Form( "HProd_MultiPntL_%s.pdf", sOutFileTag.Data() ) );
   
   canvMultiPntR->SaveAs( Form( "HProd_MultiPntR_%s.png", sOutFileTag.Data() ) );
   canvMultiPntR->SaveAs( Form( "HProd_MultiPntR_%s.pdf", sOutFileTag.Data() ) );
   
   canvSingTrkL->SaveAs( Form( "HProd_SingTrkL_%s.png", sOutFileTag.Data() ) );
   canvSingTrkL->SaveAs( Form( "HProd_SingTrkL_%s.pdf", sOutFileTag.Data() ) );
   
   canvSingTrkR->SaveAs( Form( "HProd_SingTrkR_%s.png", sOutFileTag.Data() ) );
   canvSingTrkR->SaveAs( Form( "HProd_SingTrkR_%s.pdf", sOutFileTag.Data() ) );
   
   canvMultiTrkL->SaveAs( Form( "HProd_MultiTrkL_%s.png", sOutFileTag.Data() ) );
   canvMultiTrkL->SaveAs( Form( "HProd_MultiTrkL_%s.pdf", sOutFileTag.Data() ) );
   
   canvMultiTrkR->SaveAs( Form( "HProd_MultiTrkR_%s.png", sOutFileTag.Data() ) );
   canvMultiTrkR->SaveAs( Form( "HProd_MultiTrkR_%s.pdf", sOutFileTag.Data() ) );
   
   canvAllPntL->SaveAs( Form( "HProd_AllPntL_%s.png", sOutFileTag.Data() ) );
   canvAllPntL->SaveAs( Form( "HProd_AllPntL_%s.pdf", sOutFileTag.Data() ) );
   
   canvAllPntR->SaveAs( Form( "HProd_AllPntR_%s.png", sOutFileTag.Data() ) );
   canvAllPntR->SaveAs( Form( "HProd_AllPntR_%s.pdf", sOutFileTag.Data() ) );


   return kTRUE;
}
