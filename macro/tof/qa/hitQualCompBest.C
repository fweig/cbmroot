
const UInt_t   kuNbFiles = 4;
const UInt_t   kuNbDim   = 5;
const TString  ksDimName[kuNbDim] = { "X", "Y", "Z", "R", "T" }; 
const Color_t	kcFileColor[kuNbFiles] = { kBlue, kRed, kGreen, kOrange };
const Int_t    kiLineWidth    = 2;
const Double_t kdLegTextSize  = 0.04;
const Double_t kdPadTitleSize = 0.1;

const UInt_t   kuNbStepsDist = 30;
const Double_t kdDistStep    =  1; //cm
const UInt_t   kuNbStepsTime = 10;
const Double_t kdTimeStep    = 100.0; //ps

Bool_t hitQualCompBest( TString sFilenameA = "data/tofqa.sis300_electron_auau.25gev.centr._qa.hst.all.root",
                     TString sFilenameB = "data/tofqa.sis300_electron_auau.25gev.centr.noTRD_qa.hst.all.root",
                     TString sFilenameC = "data/tofqa.cbm100_pbar_auau.25gev.centr._qa.hst.all.root",
                     TString sFilenameD = "data/tofqa.cbm100_pbar_auau.25gev.centr.noTRD_qa.hst.all.root",
                     TString sTagA = "sis300e_Trd_25gev", TString sTagB = "sis300e_NoTrd_25gev",
                     TString sTagC = "cbm100p_Trd_25gev", TString sTagD = "cbm100p_NoTrd_25gev",
                     TString sOutFileTag = "_25gev" )
{
   sOutFileTag += "_Best";
   
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
   TH2 * hMultiPntHitClosestDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiPntHitFurthestDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiPntHitBestDelta[kuNbFiles][kuNbDim];
   TH1 * hSingleTrackHitDelta[kuNbFiles][kuNbDim];
   TH1 * hSingTrkMultiPntHitDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiTrkHitClosestDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiTrkHitFurthestDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiTrkHitBestDelta[kuNbFiles][kuNbDim];
   
   TH1 * hMultiPntHitClosestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiPntHitFurthestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiPntHitBestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiTrkHitClosestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiTrkHitFurthestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiTrkHitBestDelta_proj[kuNbFiles][kuNbDim];
   
   TH1 * hAllPntHitClosestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitFurthestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitBestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllTrkHitClosestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllTrkHitFurthestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllTrkHitBestDelta[kuNbFiles][kuNbDim];
   
   TH1 * hRatioPntHitClosestDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioPntHitFurthestDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioPntHitBestDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioTrkHitClosestDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioTrkHitFurthestDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioTrkHitBestDelta[kuNbFiles][kuNbDim];
   
   TH1 * tempOneDimHist = NULL;
   TH2 * tempTwoDimHist = NULL;
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
      {
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_SinglePointHitDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hSinglePointHitDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_SinglePointHitDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiPntHitClosestDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiPntHitClosestDelta[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiPntHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiPntHitFurthestDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiPntHitFurthestDelta[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiPntHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiPntHitBestDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiPntHitBestDelta[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiPntHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_SingleTrackHitDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hSingleTrackHitDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_SingleTrackHitDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_SingTrkMultiPntHitDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempOneDimHist )
            hSingTrkMultiPntHitDelta[uFile][uDim] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_SingTrkMultiPntHitDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiTrkHitClosestDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiTrkHitClosestDelta[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiTrkHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiTrkHitFurthestDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiTrkHitFurthestDelta[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiTrkHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiTrkHitBestDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiTrkHitBestDelta[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiTrkHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         hMultiPntHitClosestDelta_proj[uFile][uDim]  = hMultiPntHitClosestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitClosestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiPntHitFurthestDelta_proj[uFile][uDim] = hMultiPntHitFurthestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitFurthestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiPntHitBestDelta_proj[uFile][uDim]     = hMultiPntHitBestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitBestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiTrkHitClosestDelta_proj[uFile][uDim]  = hMultiTrkHitClosestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiTrkHitClosestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiTrkHitFurthestDelta_proj[uFile][uDim] = hMultiTrkHitFurthestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiTrkHitFurthestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiTrkHitBestDelta_proj[uFile][uDim]     = hMultiTrkHitBestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiTrkHitBestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );

         hAllPntHitClosestDelta[uFile][uDim]  = (TH1*)(hMultiPntHitClosestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitFurthestDelta[uFile][uDim] = (TH1*)(hMultiPntHitFurthestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitBestDelta[uFile][uDim]     = (TH1*)(hMultiPntHitBestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllTrkHitClosestDelta[uFile][uDim]  = (TH1*)(hMultiTrkHitClosestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllTrkHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllTrkHitFurthestDelta[uFile][uDim] = (TH1*)(hMultiTrkHitFurthestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllTrkHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllTrkHitBestDelta[uFile][uDim]     = (TH1*)(hMultiTrkHitBestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllTrkHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) ); 
                  
         hAllPntHitClosestDelta[uFile][uDim] ->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllPntHitFurthestDelta[uFile][uDim]->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllPntHitBestDelta[uFile][uDim]    ->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllTrkHitClosestDelta[uFile][uDim] ->Add( hSingleTrackHitDelta[uFile][uDim] );
         hAllTrkHitFurthestDelta[uFile][uDim]->Add( hSingleTrackHitDelta[uFile][uDim] );
         hAllTrkHitBestDelta[uFile][uDim]    ->Add( hSingleTrackHitDelta[uFile][uDim] );
         
         if( "T" == ksDimName[uDim] )
         {
            hRatioPntHitClosestDelta[uFile][uDim]  = new TH1D(
                  Form("%s_RatioPntHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Pnt, Closest Pnt if multi, %s; %s Hit - %s Pnt [ps]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsTime, -0.5, kuNbStepsTime*kdTimeStep -0.5 );
            hRatioPntHitFurthestDelta[uFile][uDim] = new TH1D(
                  Form("%s_RatioPntHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Pnt, Furthest Pnt if multi, %s; %s Hit - %s Pnt [ps]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsTime, -0.5, kuNbStepsTime*kdTimeStep -0.5 );
            hRatioPntHitBestDelta[uFile][uDim]     = new TH1D(
                  Form("%s_RatioPntHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Pnt, Best Pnt Pos. if multi, %s; %s Hit - %s Pnt [ps]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsTime, -0.5, kuNbStepsTime*kdTimeStep -0.5 );
            hRatioTrkHitClosestDelta[uFile][uDim]  = new TH1D(
                  Form("%s_RatioTrkHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Trk, Closest Trk if multi, %s; %s Hit - %s Trk [ps]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsTime, -0.5, kuNbStepsTime*kdTimeStep -0.5 );
            hRatioTrkHitFurthestDelta[uFile][uDim] = new TH1D(
                  Form("%s_RatioTrkHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Trk, Furthest Trk if multi, %s; %s Hit - %s Trk [ps]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsTime, -0.5, kuNbStepsTime*kdTimeStep -0.5 );
            hRatioTrkHitBestDelta[uFile][uDim]     = new TH1D(
                  Form("%s_RatioTrkHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Trk, Best Trk Pos if multi, %s; %s Hit - %s Trk [ps]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsTime, -0.5, kuNbStepsTime*kdTimeStep -0.5 );
         } // if( "T" == ksDimName[uDim] )
         else
         {
            hRatioPntHitClosestDelta[uFile][uDim]  = new TH1D(
                  Form("%s_RatioPntHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Pnt, Closest Pnt if multi, %s; %s Hit - %s Pnt [cm]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsDist, -0.01, kuNbStepsDist*kdDistStep -0.01 );
            hRatioPntHitFurthestDelta[uFile][uDim] = new TH1D(
                  Form("%s_RatioPntHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Pnt, Furthest Pnt if multi, %s; %s Hit - %s Pnt [cm]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsDist, -0.01, kuNbStepsDist*kdDistStep -0.01 );
            hRatioPntHitBestDelta[uFile][uDim]     = new TH1D(
                  Form("%s_RatioPntHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Pnt, Best Pnt Pos. if multi, %s; %s Hit - %s Pnt [cm]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsDist, -0.01, kuNbStepsDist*kdDistStep -0.01 );
            hRatioTrkHitClosestDelta[uFile][uDim]  = new TH1D(
                  Form("%s_RatioTrkHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Trk, Closest Trk if multi, %s; %s Hit - %s Trk [cm]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsDist, -0.01, kuNbStepsDist*kdDistStep -0.01 );
            hRatioTrkHitFurthestDelta[uFile][uDim] = new TH1D(
                  Form("%s_RatioTrkHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Trk, Furthest Trk if multi, %s; %s Hit - %s Trk [cm]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsDist, -0.01, kuNbStepsDist*kdDistStep -0.01 );
            hRatioTrkHitBestDelta[uFile][uDim]     = new TH1D(
                  Form("%s_RatioTrkHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Trk, Best Trk Pos if multi, %s; %s Hit - %s Trk [cm]; Fraction[\%]",
                        ksDimName[uDim].Data(), sTagFiles[uFile].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ),
                  kuNbStepsDist, -0.01, kuNbStepsDist*kdDistStep -0.01 );
         } // else of if( "T" == ksDimName[uDim] )
      } // Loop on files and dimensions

   // Close files now that we have all histos
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      filePnt[uFile]->Close();
   
   // Extract interesting numbers from hit quality plots
   UInt_t   uNbTotHits[kuNbFiles];
   Double_t dRatioHitsMultiPnt[kuNbFiles];
   Double_t dRatioHitsMultiPntSingTrk[kuNbFiles];
   Double_t dRatioHitsMultiTrk[kuNbFiles];
   
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   {
      uNbTotHits[uFile] = hSinglePointHitDelta[uFile][0]->GetEntries() +
                          hMultiPntHitClosestDelta[uFile][0]->GetEntries();
      dRatioHitsMultiPnt[uFile] = (100.0 * hMultiPntHitClosestDelta[uFile][0]->GetEntries() ) 
                                  /uNbTotHits[uFile];
      dRatioHitsMultiPntSingTrk[uFile] = (100.0 * hSingTrkMultiPntHitDelta[uFile][0]->GetEntries() ) 
                                  /uNbTotHits[uFile];
      dRatioHitsMultiTrk[uFile] = (100.0 * hMultiTrkHitClosestDelta[uFile][0]->GetEntries() ) 
                                  /uNbTotHits[uFile];
      for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
      {
         if( "T" == ksDimName[uDim] )
            for( UInt_t uStep = 0; uStep < kuNbStepsTime; uStep++)
            {
               Int_t iBinMinNeg = hAllPntHitClosestDelta[uFile][uDim]->FindBin( -1.0*(uStep + 1)*kdTimeStep );
               Int_t iBinMaxNeg = hAllPntHitClosestDelta[uFile][uDim]->FindBin( -1.0*uStep*kdTimeStep) -1;
               Int_t iBinMinPos = hAllPntHitClosestDelta[uFile][uDim]->FindBin( uStep*kdTimeStep );
               Int_t iBinMaxPos = hAllPntHitClosestDelta[uFile][uDim]->FindBin( (uStep + 1)*kdTimeStep ) - 1;
                        
               hRatioPntHitClosestDelta[uFile][uDim] ->Fill( uStep*kdTimeStep, 
                     ( hAllPntHitClosestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllPntHitClosestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                     / uNbTotHits[uFile] );
               hRatioPntHitFurthestDelta[uFile][uDim] ->Fill( uStep*kdTimeStep, 
                     ( hAllPntHitFurthestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllPntHitFurthestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                     / uNbTotHits[uFile] );
               hRatioPntHitBestDelta[uFile][uDim]     ->Fill( uStep*kdTimeStep, 
                     ( hAllPntHitBestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllPntHitBestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                     / uNbTotHits[uFile] );
               hRatioTrkHitClosestDelta[uFile][uDim]  ->Fill( uStep*kdTimeStep, 
                     ( hAllTrkHitClosestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllTrkHitClosestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                     / uNbTotHits[uFile] );
               hRatioTrkHitFurthestDelta[uFile][uDim] ->Fill( uStep*kdTimeStep, 
                     ( hAllTrkHitFurthestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllTrkHitFurthestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                     / uNbTotHits[uFile] );
               hRatioTrkHitBestDelta[uFile][uDim]     ->Fill( uStep*kdTimeStep, 
                     ( hAllTrkHitBestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllTrkHitBestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                     / uNbTotHits[uFile] );
            } // for( UInt_t uStep = 0; uStep < kuNbStepsTime; uStep++)
            else for( UInt_t uStep = 0; uStep < kuNbStepsDist; uStep++)
               {
                  Int_t iBinMinNeg = hAllPntHitClosestDelta[uFile][uDim]->FindBin( -1.0*(uStep + 1)*kdDistStep );
                  Int_t iBinMaxNeg = hAllPntHitClosestDelta[uFile][uDim]->FindBin( -1.0*uStep*kdDistStep ) - 1;
                  Int_t iBinMinPos = hAllPntHitClosestDelta[uFile][uDim]->FindBin( uStep*kdDistStep );
                  Int_t iBinMaxPos = hAllPntHitClosestDelta[uFile][uDim]->FindBin( (uStep + 1)*kdDistStep ) - 1;
                           
                  hRatioPntHitClosestDelta[uFile][uDim] ->Fill( uStep*kdDistStep, 
                        ( hAllPntHitClosestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllPntHitClosestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                        / uNbTotHits[uFile] );
                  hRatioPntHitFurthestDelta[uFile][uDim] ->Fill( uStep*kdDistStep, 
                        ( hAllPntHitFurthestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllPntHitFurthestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                        / uNbTotHits[uFile] );
                  hRatioPntHitBestDelta[uFile][uDim]     ->Fill( uStep*kdDistStep, 
                        ( hAllPntHitBestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllPntHitBestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                        / uNbTotHits[uFile] );
                  hRatioTrkHitClosestDelta[uFile][uDim]  ->Fill( uStep*kdDistStep, 
                        ( hAllTrkHitClosestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllTrkHitClosestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                        / uNbTotHits[uFile] );
                  hRatioTrkHitFurthestDelta[uFile][uDim] ->Fill( uStep*kdDistStep, 
                        ( hAllTrkHitFurthestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllTrkHitFurthestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                        / uNbTotHits[uFile] );
                  hRatioTrkHitBestDelta[uFile][uDim]     ->Fill( uStep*kdDistStep, 
                        ( hAllTrkHitBestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllTrkHitBestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                        / uNbTotHits[uFile] );
               } // for( UInt_t uStep = 0; uStep < kuNbStepsDist; uStep++)
               
      } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
      
      cout<<"------------------------------------------------"<<endl;
      cout<<"Hit quality numbers for "<<sTagFiles[uFile]<<":"<<endl;
      cout<<"  Total number of hits:                           "<<uNbTotHits[uFile]<<endl; 
      cout<<"  Fraction of hits from multiple MC points:       "<<dRatioHitsMultiPnt[uFile]<<endl;
      cout<<"  Fraction of hits from multi pnt but single trk: "<<dRatioHitsMultiPntSingTrk[uFile]<<endl;
      cout<<"  Fraction of hits from multiple MC tracks:       "<<dRatioHitsMultiTrk[uFile]<<endl;
   } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   
   // Display resulting graphs
   THStack * histosStackPntClos[kuNbDim];
   THStack * histosStackPntFurt[kuNbDim];
   THStack * histosStackPntBest[kuNbDim];
   THStack * histosStackTrkClos[kuNbDim];
   THStack * histosStackTrkFurt[kuNbDim];
   THStack * histosStackTrkBest[kuNbDim];
      
   TLegend * legPntClos[kuNbDim];
   TLegend * legPntFurt[kuNbDim];
   TLegend * legPntBest[kuNbDim];
   TLegend * legTrkClos[kuNbDim];
   TLegend * legTrkFurt[kuNbDim];
   TLegend * legTrkBest[kuNbDim];
               
   TCanvas *canvRatio[kuNbDim];
      
   // Set size of PAD title for all pads
   gStyle->SetTitleW(0.9); //title width
   gStyle->SetTitleH(0.1); //title height 

   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {
      if( "T" == ksDimName[uDim] )
      {
         histosStackPntClos[uDim] = new THStack(Form("histosStackPntClos%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Pnt, Closest Pnt if multi; %s Hit - %s Pnt [ps]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackPntFurt[uDim] = new THStack(Form("histosStackPntFurt%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Pnt, Furthest Pnt if multi; %s Hit - %s Pnt [ps]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackPntBest[uDim] = new THStack(Form("histosStackPntBest%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Pnt, Best Pnt if multi; %s Hit - %s Pnt [ps]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkClos[uDim] = new THStack(Form("histosStackTrkClos%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Closest Trk if multi; %s Hit - %s Trk [ps]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkFurt[uDim] = new THStack(Form("histosStackTrkFurt%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Furthest Trk if multi; %s Hit - %s Trk [ps]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkBest[uDim] = new THStack(Form("histosStackTrkBest%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Best Trk if multi; %s Hit - %s Trk [ps]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // if( "T" == ksDimName[uDim] )
      else
      {
         histosStackPntClos[uDim] = new THStack(Form("histosStackPntClos%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Pnt, Closest Pnt if multi; %s Hit - %s Pnt [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackPntFurt[uDim] = new THStack(Form("histosStackPntFurt%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Pnt, Furthest Pnt if multi; %s Hit - %s Pnt [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackPntBest[uDim] = new THStack(Form("histosStackPntBest%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Pnt, Best Pnt if multi; %s Hit - %s Pnt [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkClos[uDim] = new THStack(Form("histosStackTrkClos%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Closest Trk if multi; %s Hit - %s Trk [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkFurt[uDim] = new THStack(Form("histosStackTrkFurt%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Furthest Trk if multi; %s Hit - %s Trk [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkBest[uDim] = new THStack(Form("histosStackTrkBest%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Best Trk if multi; %s Hit - %s Trk [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // else of if( "T" == ksDimName[uDim] )

      legPntClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legPntFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legPntBest[uDim] = new TLegend(0.45,0.55,0.9,0.9); 
      legTrkClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legTrkFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legTrkBest[uDim] = new TLegend(0.45,0.55,0.9,0.9);     
      legPntClos[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legPntFurt[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legPntBest[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Best",     ksDimName[uDim].Data() ) );
      legTrkClos[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legTrkFurt[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legTrkBest[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Best",     ksDimName[uDim].Data() ) );
      legPntClos[uDim]->SetTextSize( kdLegTextSize );
      legPntFurt[uDim]->SetTextSize( kdLegTextSize );
      legPntBest[uDim]->SetTextSize( kdLegTextSize );
      legTrkClos[uDim]->SetTextSize( kdLegTextSize );
      legTrkFurt[uDim]->SetTextSize( kdLegTextSize );
      legTrkBest[uDim]->SetTextSize( kdLegTextSize );
                            
      canvRatio[uDim] = new TCanvas( Form("canvRatio%s",ksDimName[uDim].Data() ), 
            Form("Fraction of hits vs Distance in %s",ksDimName[uDim].Data() ),
            1920, 986 );
      canvRatio[uDim]->cd();
      canvRatio[uDim]->Divide(3,2);
   
      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {         
         hRatioPntHitClosestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioPntHitClosestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackPntClos[uDim]->Add( hRatioPntHitClosestDelta[uFile][uDim] );
         legPntClos[uDim]->AddEntry( hRatioPntHitClosestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hRatioPntHitFurthestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioPntHitFurthestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackPntFurt[uDim]->Add( hRatioPntHitFurthestDelta[uFile][uDim] );
         legPntFurt[uDim]->AddEntry(hRatioPntHitFurthestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hRatioPntHitBestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioPntHitBestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackPntBest[uDim]->Add( hRatioPntHitBestDelta[uFile][uDim] );
         legPntBest[uDim]->AddEntry( hRatioPntHitBestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hRatioTrkHitClosestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioTrkHitClosestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackTrkClos[uDim]->Add( hRatioTrkHitClosestDelta[uFile][uDim] );
         legTrkClos[uDim]->AddEntry( hRatioTrkHitClosestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hRatioTrkHitFurthestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioTrkHitFurthestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackTrkFurt[uDim]->Add( hRatioTrkHitFurthestDelta[uFile][uDim] );
         legTrkFurt[uDim]->AddEntry( hRatioTrkHitFurthestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hRatioTrkHitBestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioTrkHitBestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackTrkBest[uDim]->Add( hRatioTrkHitBestDelta[uFile][uDim] );
         legTrkBest[uDim]->AddEntry( hRatioTrkHitBestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
      } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      
      canvRatio[uDim]->cd(1);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackPntClos[uDim]->Draw("nostack");
      legPntClos[uDim]->Draw();
      
      canvRatio[uDim]->cd(2);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackPntFurt[uDim]->Draw("nostack");
      legPntFurt[uDim]->Draw();
      
      canvRatio[uDim]->cd(3);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackPntBest[uDim]->Draw("nostack");
      legPntBest[uDim]->Draw();
      
      canvRatio[uDim]->cd(4);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackTrkClos[uDim]->Draw("nostack");
      legTrkClos[uDim]->Draw();
      
      canvRatio[uDim]->cd(5);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackTrkFurt[uDim]->Draw("nostack");
      legTrkFurt[uDim]->Draw();
      
      canvRatio[uDim]->cd(6);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackTrkBest[uDim]->Draw("nostack");
      legTrkBest[uDim]->Draw();
      
      canvRatio[uDim]->SaveAs( Form( "FracComp%s%s.png", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
      canvRatio[uDim]->SaveAs( Form( "FracComp%s%s.pdf", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   
   // Display MultiPnt/MultiTrk Projections
   THStack * histosStackMultiPntProjClos[kuNbDim];
   THStack * histosStackMultiPntProjFurt[kuNbDim];
   THStack * histosStackMultiPntProjBest[kuNbDim];
   THStack * histosStackMultiTrkProjClos[kuNbDim];
   THStack * histosStackMultiTrkProjFurt[kuNbDim];
   THStack * histosStackMultiTrkProjBest[kuNbDim];
      
   TLegend * legMultiPntProjClos[kuNbDim];
   TLegend * legMultiPntProjFurt[kuNbDim];
   TLegend * legMultiPntProjBest[kuNbDim];
   TLegend * legMultiTrkProjClos[kuNbDim];
   TLegend * legMultiTrkProjFurt[kuNbDim];
   TLegend * legMultiTrkProjBest[kuNbDim];
               
   TCanvas *canvMultiProj[kuNbDim];

   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {
      if( "T" == ksDimName[uDim] )
      {
         histosStackMultiPntProjClos[uDim] = new THStack(Form("histosStackMultiPntProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Closest Pnt; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntProjFurt[uDim] = new THStack(Form("histosStackMultiPntProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Furthest Pnt; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntProjBest[uDim] = new THStack(Form("histosStackMultiPntProjBest%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Best Pnt; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Closest Trk; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Furthest Trk; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjBest[uDim] = new THStack(Form("histosStackMultiTrkProjBest%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Best Trk; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // if( "T" == ksDimName[uDim] )
      else
      {
         histosStackMultiPntProjClos[uDim] = new THStack(Form("histosStackMultiPntProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Closest Pnt; %s Hit - %s Pnt [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntProjFurt[uDim] = new THStack(Form("histosStackMultiPntProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Furthest Pnt; %s Hit - %s Pnt [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntProjBest[uDim] = new THStack(Form("histosStackMultiPntProjBest%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Best Pnt; %s Hit - %s Pnt [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Closest Trk; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Furthest Trk; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjBest[uDim] = new THStack(Form("histosStackMultiTrkProjBest%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Best Trk; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // else of if( "T" == ksDimName[uDim] )

      legMultiPntProjClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiPntProjFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiPntProjBest[uDim] = new TLegend(0.45,0.55,0.9,0.9); 
      legMultiTrkProjClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjBest[uDim] = new TLegend(0.45,0.55,0.9,0.9);     
      legMultiPntProjClos[uDim]->SetHeader( Form("Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legMultiPntProjFurt[uDim]->SetHeader( Form("Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legMultiPntProjBest[uDim]->SetHeader( Form("Dist. in %s, Best",     ksDimName[uDim].Data() ) );
      legMultiTrkProjClos[uDim]->SetHeader( Form("Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legMultiTrkProjFurt[uDim]->SetHeader( Form("Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legMultiTrkProjBest[uDim]->SetHeader( Form("Dist. in %s, Best",     ksDimName[uDim].Data() ) );
      legMultiPntProjClos[uDim]->SetTextSize( kdLegTextSize );
      legMultiPntProjFurt[uDim]->SetTextSize( kdLegTextSize );
      legMultiPntProjBest[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjClos[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjFurt[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjBest[uDim]->SetTextSize( kdLegTextSize );
                            
      canvMultiProj[uDim] = new TCanvas( Form("canvMultiProj%s",ksDimName[uDim].Data() ), 
            Form("Distance in %s, hits from mixed MC Points/tracks",ksDimName[uDim].Data() ),
            1920, 986 );
      canvMultiProj[uDim]->cd();
      canvMultiProj[uDim]->Divide(3,2);
   
      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {
         hMultiPntHitClosestDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiPntHitClosestDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiPntProjClos[uDim]->Add( hMultiPntHitClosestDelta_proj[uFile][uDim] );
         legMultiPntProjClos[uDim]->AddEntry( hMultiPntHitClosestDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiPntHitFurthestDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiPntHitFurthestDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiPntProjFurt[uDim]->Add( hMultiPntHitFurthestDelta_proj[uFile][uDim] );
         legMultiPntProjFurt[uDim]->AddEntry(hMultiPntHitFurthestDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiPntHitBestDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiPntHitBestDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiPntProjBest[uDim]->Add( hMultiPntHitBestDelta_proj[uFile][uDim] );
         legMultiPntProjBest[uDim]->AddEntry( hMultiPntHitBestDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiTrkHitClosestDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiTrkHitClosestDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjClos[uDim]->Add( hMultiTrkHitClosestDelta_proj[uFile][uDim] );
         legMultiTrkProjClos[uDim]->AddEntry( hMultiTrkHitClosestDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiTrkHitFurthestDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiTrkHitFurthestDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjFurt[uDim]->Add( hMultiTrkHitFurthestDelta_proj[uFile][uDim] );
         legMultiTrkProjFurt[uDim]->AddEntry( hMultiTrkHitFurthestDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiTrkHitBestDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiTrkHitBestDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjBest[uDim]->Add( hMultiTrkHitBestDelta_proj[uFile][uDim] );
         legMultiTrkProjBest[uDim]->AddEntry( hMultiTrkHitBestDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
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
      histosStackMultiPntProjBest[uDim]->Draw("nostack");
//      legMultiPntProjBest[uDim]->Draw();
      
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
      histosStackMultiTrkProjBest[uDim]->Draw("nostack");
//      legMultiTrkProjBest[uDim]->Draw();
      
      canvMultiProj[uDim]->SaveAs( Form( "MultiProj%s%s.png", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
      canvMultiProj[uDim]->SaveAs( Form( "MultiProj%s%s.pdf", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)

   // Display Single Trk and Multi Pnt Single Trk histos
   THStack * histosStackSingTrk[kuNbDim];
   THStack * histosStackSingTrkMultiPnt[kuNbDim];
      
   TLegend * legSingTrk[kuNbDim];
   TLegend * legSingTrkMultiPnt[kuNbDim];
               
   TCanvas *canvSingTrk = new TCanvas( "canvSingTrk", 
            "Distance between Hit and Track, single tracks",
            1920, 986 );
   canvSingTrk->cd();
   canvSingTrk->Divide(3,2);
      
   TCanvas *canvSingTrkMultiPnt = new TCanvas( "canvSingTrkMultiPnt", 
            "Distance between Hit and Track, single tracks with multiple points",
            1920, 986 );
   canvSingTrkMultiPnt->cd();
   canvSingTrkMultiPnt->Divide(3,2);

   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {
      if( "T" == ksDimName[uDim] )
      {
         histosStackSingTrk[uDim] = new THStack(Form("histosStackSingTrk%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from single Trk; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackSingTrkMultiPnt[uDim] = new THStack(Form("histosStackSingTrkMultiPnt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from single Trk but multi Pnt; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // if( "T" == ksDimName[uDim] )
      else
      {
         histosStackSingTrk[uDim] = new THStack(Form("histosStackSingTrk%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from single Trk; %s Hit - %s Pnt [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackSingTrkMultiPnt[uDim] = new THStack(Form("histosStackSingTrkMultiPnt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from single Trk but multi Pnt; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // else of if( "T" == ksDimName[uDim] )

      legSingTrk[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legSingTrkMultiPnt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legSingTrk[uDim]->SetHeader( Form("Dist. in %s",  ksDimName[uDim].Data() ) );
      legSingTrkMultiPnt[uDim]->SetHeader( Form("Dist. in %s",  ksDimName[uDim].Data() ) );
      legSingTrk[uDim]->SetTextSize( kdLegTextSize );
      legSingTrkMultiPnt[uDim]->SetTextSize( kdLegTextSize );
      
      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {
         hSingleTrackHitDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hSingleTrackHitDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackSingTrk[uDim]->Add( hSingleTrackHitDelta[uFile][uDim] );
         legSingTrk[uDim]->AddEntry( hSingleTrackHitDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hSingTrkMultiPntHitDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hSingTrkMultiPntHitDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackSingTrkMultiPnt[uDim]->Add( hSingTrkMultiPntHitDelta[uFile][uDim] );
         legSingTrkMultiPnt[uDim]->AddEntry( hSingTrkMultiPntHitDelta[uFile][uDim] , sTagFiles[uFile],"l");
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
      
   canvSingTrk->SaveAs( Form( "SingTrk%s.png", sOutFileTag.Data() ) );
   canvSingTrk->SaveAs( Form( "SingTrk%s.pdf", sOutFileTag.Data() ) );
   
   canvSingTrkMultiPnt->SaveAs( Form( "SingTrkMultiPnt%s.png", sOutFileTag.Data() ) );
   canvSingTrkMultiPnt->SaveAs( Form( "SingTrkMultiPnt%s.pdf", sOutFileTag.Data() ) );
   
   // Display all Hits distrib
   THStack * histosStackAllPntHitClos[kuNbDim];
   THStack * histosStackAllPntHitFurt[kuNbDim];
   THStack * histosStackAllPntHitBest[kuNbDim];
   THStack * histosStackMultiTrkProjClos[kuNbDim];
   THStack * histosStackMultiTrkProjFurt[kuNbDim];
   THStack * histosStackMultiTrkProjBest[kuNbDim];
      
   TLegend * legAllPntHitClos[kuNbDim];
   TLegend * legAllPntHitFurt[kuNbDim];
   TLegend * legAllPntHitBest[kuNbDim];
   TLegend * legMultiTrkProjClos[kuNbDim];
   TLegend * legMultiTrkProjFurt[kuNbDim];
   TLegend * legMultiTrkProjBest[kuNbDim];
               
   TCanvas *canvAllPntHit[kuNbDim];

   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {
      if( "T" == ksDimName[uDim] )
      {
         histosStackAllPntHitClos[uDim] = new THStack(Form("histosStackAllPntHitClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Closest Pnt if Multi; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackAllPntHitFurt[uDim] = new THStack(Form("histosStackAllPntHitFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Furthest Pnt if Multi; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackAllPntHitBest[uDim] = new THStack(Form("histosStackAllPntHitBest%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Best Pnt if Multi; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Closest Trk if Multi; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Furthest Trk if Multi; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjBest[uDim] = new THStack(Form("histosStackMultiTrkProjBest%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Best Trk if Multi; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // if( "T" == ksDimName[uDim] )
      else
      {
         histosStackAllPntHitClos[uDim] = new THStack(Form("histosStackAllPntHitClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Closest Pnt if Multi; %s Hit - %s Pnt [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackAllPntHitFurt[uDim] = new THStack(Form("histosStackAllPntHitFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Furthest Pnt if Multi; %s Hit - %s Pnt [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackAllPntHitBest[uDim] = new THStack(Form("histosStackAllPntHitBest%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Best Pnt if Multi; %s Hit - %s Pnt [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Closest Trk if Multi; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Furthest Trk if Multi; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjBest[uDim] = new THStack(Form("histosStackMultiTrkProjBest%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Best Trk if Multi; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // else of if( "T" == ksDimName[uDim] )

      legAllPntHitClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legAllPntHitFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legAllPntHitBest[uDim] = new TLegend(0.45,0.55,0.9,0.9); 
      legMultiTrkProjClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjBest[uDim] = new TLegend(0.45,0.55,0.9,0.9);     
      legAllPntHitClos[uDim]->SetHeader( Form("Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legAllPntHitFurt[uDim]->SetHeader( Form("Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legAllPntHitBest[uDim]->SetHeader( Form("Dist. in %s, Best",     ksDimName[uDim].Data() ) );
      legMultiTrkProjClos[uDim]->SetHeader( Form("Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legMultiTrkProjFurt[uDim]->SetHeader( Form("Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legMultiTrkProjBest[uDim]->SetHeader( Form("Dist. in %s, Best",     ksDimName[uDim].Data() ) );
      legAllPntHitClos[uDim]->SetTextSize( kdLegTextSize );
      legAllPntHitFurt[uDim]->SetTextSize( kdLegTextSize );
      legAllPntHitBest[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjClos[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjFurt[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjBest[uDim]->SetTextSize( kdLegTextSize );
                            
      canvAllPntHit[uDim] = new TCanvas( Form("canvAllPntHit%s",ksDimName[uDim].Data() ), 
            Form("Distance in %s, all hits",ksDimName[uDim].Data() ),
            1920, 986 );
      canvAllPntHit[uDim]->cd();
      canvAllPntHit[uDim]->Divide(3,2);
   
      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {
         hAllPntHitClosestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllPntHitClosestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackAllPntHitClos[uDim]->Add( hAllPntHitClosestDelta[uFile][uDim] );
         legAllPntHitClos[uDim]->AddEntry( hAllPntHitClosestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllPntHitFurthestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllPntHitFurthestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackAllPntHitFurt[uDim]->Add( hAllPntHitFurthestDelta[uFile][uDim] );
         legAllPntHitFurt[uDim]->AddEntry(hAllPntHitFurthestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllPntHitBestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllPntHitBestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackAllPntHitBest[uDim]->Add( hAllPntHitBestDelta[uFile][uDim] );
         legAllPntHitBest[uDim]->AddEntry( hAllPntHitBestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllTrkHitClosestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllTrkHitClosestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjClos[uDim]->Add( hAllTrkHitClosestDelta[uFile][uDim] );
         legMultiTrkProjClos[uDim]->AddEntry( hAllTrkHitClosestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllTrkHitFurthestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllTrkHitFurthestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjFurt[uDim]->Add( hAllTrkHitFurthestDelta[uFile][uDim] );
         legMultiTrkProjFurt[uDim]->AddEntry( hAllTrkHitFurthestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllTrkHitBestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllTrkHitBestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjBest[uDim]->Add( hAllTrkHitBestDelta[uFile][uDim] );
         legMultiTrkProjBest[uDim]->AddEntry( hAllTrkHitBestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
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
      histosStackAllPntHitBest[uDim]->Draw("nostack");
//      legAllPntHitBest[uDim]->Draw();
      
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
      histosStackMultiTrkProjBest[uDim]->Draw("nostack");
//      legMultiTrkProjBest[uDim]->Draw();
      
      canvAllPntHit[uDim]->SaveAs( Form( "AllPntHit%s%s.png", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
      canvAllPntHit[uDim]->SaveAs( Form( "AllPntHit%s%s.pdf", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   
   // Save constructed histos
   TFile * fileTest = new TFile("test.root", "RECREATE");
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
      {
         hRatioPntHitClosestDelta[uFile][uDim] ->Write();
         hRatioPntHitFurthestDelta[uFile][uDim]->Write();
         hRatioPntHitBestDelta[uFile][uDim]    ->Write();
         hRatioTrkHitClosestDelta[uFile][uDim] ->Write();
         hRatioTrkHitFurthestDelta[uFile][uDim]->Write();
         hRatioTrkHitBestDelta[uFile][uDim]    ->Write();
      } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   fileTest->Close();   
   
   return kTRUE;
}             
