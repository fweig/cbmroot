
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

Bool_t hitQualComp( TString sFilenameA = "data/tofqa.sis300_electron_auau.25gev.centr._qa.hst.all.root",
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
   TH1 * hSinglePointHitDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiPntHitClosestDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiPntHitFurthestDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiPntHitMeanDelta[kuNbFiles][kuNbDim];
   TH1 * hSingleTrackHitDelta[kuNbFiles][kuNbDim];
   TH1 * hSingTrkMultiPntHitDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiTrkHitClosestDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiTrkHitFurthestDelta[kuNbFiles][kuNbDim];
   TH2 * hMultiTrkHitMeanDelta[kuNbFiles][kuNbDim];
   
   TH1 * hMultiPntHitClosestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiPntHitFurthestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiPntHitMeanDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiTrkHitClosestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiTrkHitFurthestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiTrkHitMeanDelta_proj[kuNbFiles][kuNbDim];
   
   TH1 * hAllPntHitClosestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitFurthestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitMeanDelta[kuNbFiles][kuNbDim];
   TH1 * hAllTrkHitClosestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllTrkHitFurthestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllTrkHitMeanDelta[kuNbFiles][kuNbDim];
   
   TH1 * hRatioPntHitClosestDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioPntHitFurthestDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioPntHitMeanDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioTrkHitClosestDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioTrkHitFurthestDelta[kuNbFiles][kuNbDim];
   TH1 * hRatioTrkHitMeanDelta[kuNbFiles][kuNbDim];
   
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
               Form("TofTests_MultiPntHitMeanDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiPntHitMeanDelta[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiPntHitMeanDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
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
               Form("TofTests_MultiTrkHitMeanDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiTrkHitMeanDelta[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiTrkHitMeanDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         hMultiPntHitClosestDelta_proj[uFile][uDim]  = hMultiPntHitClosestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitClosestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiPntHitFurthestDelta_proj[uFile][uDim] = hMultiPntHitFurthestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitFurthestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiPntHitMeanDelta_proj[uFile][uDim]     = hMultiPntHitMeanDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitMeanDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiTrkHitClosestDelta_proj[uFile][uDim]  = hMultiTrkHitClosestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiTrkHitClosestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiTrkHitFurthestDelta_proj[uFile][uDim] = hMultiTrkHitFurthestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiTrkHitFurthestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiTrkHitMeanDelta_proj[uFile][uDim]     = hMultiTrkHitMeanDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiTrkHitMeanDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );

         hAllPntHitClosestDelta[uFile][uDim]  = (TH1*)(hMultiPntHitClosestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitFurthestDelta[uFile][uDim] = (TH1*)(hMultiPntHitFurthestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitMeanDelta[uFile][uDim]     = (TH1*)(hMultiPntHitMeanDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitMeanDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllTrkHitClosestDelta[uFile][uDim]  = (TH1*)(hMultiTrkHitClosestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllTrkHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllTrkHitFurthestDelta[uFile][uDim] = (TH1*)(hMultiTrkHitFurthestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllTrkHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllTrkHitMeanDelta[uFile][uDim]     = (TH1*)(hMultiTrkHitMeanDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllTrkHitMeanDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) ); 
                  
         hAllPntHitClosestDelta[uFile][uDim] ->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllPntHitFurthestDelta[uFile][uDim]->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllPntHitMeanDelta[uFile][uDim]    ->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllTrkHitClosestDelta[uFile][uDim] ->Add( hSingleTrackHitDelta[uFile][uDim] );
         hAllTrkHitFurthestDelta[uFile][uDim]->Add( hSingleTrackHitDelta[uFile][uDim] );
         hAllTrkHitMeanDelta[uFile][uDim]    ->Add( hSingleTrackHitDelta[uFile][uDim] );
         
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
            hRatioPntHitMeanDelta[uFile][uDim]     = new TH1D(
                  Form("%s_RatioPntHitMeanDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Pnt, Mean Pnt Pos. if multi, %s; %s Hit - %s Pnt [ps]; Fraction[\%]",
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
            hRatioTrkHitMeanDelta[uFile][uDim]     = new TH1D(
                  Form("%s_RatioTrkHitMeanDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Trk, Mean Trk Pos if multi, %s; %s Hit - %s Trk [ps]; Fraction[\%]",
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
            hRatioPntHitMeanDelta[uFile][uDim]     = new TH1D(
                  Form("%s_RatioPntHitMeanDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Pnt, Mean Pnt Pos. if multi, %s; %s Hit - %s Pnt [cm]; Fraction[\%]",
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
            hRatioTrkHitMeanDelta[uFile][uDim]     = new TH1D(
                  Form("%s_RatioTrkHitMeanDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()),
                  Form("Fraction of hits vs Distance in %s from Trk, Mean Trk Pos if multi, %s; %s Hit - %s Trk [cm]; Fraction[\%]",
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
               hRatioPntHitMeanDelta[uFile][uDim]     ->Fill( uStep*kdTimeStep, 
                     ( hAllPntHitMeanDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllPntHitMeanDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                     / uNbTotHits[uFile] );
               hRatioTrkHitClosestDelta[uFile][uDim]  ->Fill( uStep*kdTimeStep, 
                     ( hAllTrkHitClosestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllTrkHitClosestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                     / uNbTotHits[uFile] );
               hRatioTrkHitFurthestDelta[uFile][uDim] ->Fill( uStep*kdTimeStep, 
                     ( hAllTrkHitFurthestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllTrkHitFurthestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                     / uNbTotHits[uFile] );
               hRatioTrkHitMeanDelta[uFile][uDim]     ->Fill( uStep*kdTimeStep, 
                     ( hAllTrkHitMeanDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                     + hAllTrkHitMeanDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
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
                  hRatioPntHitMeanDelta[uFile][uDim]     ->Fill( uStep*kdDistStep, 
                        ( hAllPntHitMeanDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllPntHitMeanDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                        / uNbTotHits[uFile] );
                  hRatioTrkHitClosestDelta[uFile][uDim]  ->Fill( uStep*kdDistStep, 
                        ( hAllTrkHitClosestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllTrkHitClosestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                        / uNbTotHits[uFile] );
                  hRatioTrkHitFurthestDelta[uFile][uDim] ->Fill( uStep*kdDistStep, 
                        ( hAllTrkHitFurthestDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllTrkHitFurthestDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
                        / uNbTotHits[uFile] );
                  hRatioTrkHitMeanDelta[uFile][uDim]     ->Fill( uStep*kdDistStep, 
                        ( hAllTrkHitMeanDelta[uFile][uDim]->Integral(iBinMinNeg, iBinMaxNeg)
                        + hAllTrkHitMeanDelta[uFile][uDim]->Integral(iBinMinPos, iBinMaxPos) ) *100.0
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
   THStack * histosStackPntMean[kuNbDim];
   THStack * histosStackTrkClos[kuNbDim];
   THStack * histosStackTrkFurt[kuNbDim];
   THStack * histosStackTrkMean[kuNbDim];
      
   TLegend * legPntClos[kuNbDim];
   TLegend * legPntFurt[kuNbDim];
   TLegend * legPntMean[kuNbDim];
   TLegend * legTrkClos[kuNbDim];
   TLegend * legTrkFurt[kuNbDim];
   TLegend * legTrkMean[kuNbDim];
               
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
         histosStackPntMean[uDim] = new THStack(Form("histosStackPntMean%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Pnt, Mean Pnt if multi; %s Hit - %s Pnt [ps]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkClos[uDim] = new THStack(Form("histosStackTrkClos%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Closest Trk if multi; %s Hit - %s Trk [ps]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkFurt[uDim] = new THStack(Form("histosStackTrkFurt%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Furthest Trk if multi; %s Hit - %s Trk [ps]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkMean[uDim] = new THStack(Form("histosStackTrkMean%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Mean Trk if multi; %s Hit - %s Trk [ps]; Fraction[\%]",
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
         histosStackPntMean[uDim] = new THStack(Form("histosStackPntMean%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Pnt, Mean Pnt if multi; %s Hit - %s Pnt [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkClos[uDim] = new THStack(Form("histosStackTrkClos%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Closest Trk if multi; %s Hit - %s Trk [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkFurt[uDim] = new THStack(Form("histosStackTrkFurt%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Furthest Trk if multi; %s Hit - %s Trk [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackTrkMean[uDim] = new THStack(Form("histosStackTrkMean%s",  ksDimName[uDim].Data()),
               Form("Fraction of hits vs Distance in %s from Trk, Mean Trk if multi; %s Hit - %s Trk [cm]; Fraction[\%]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // else of if( "T" == ksDimName[uDim] )

      legPntClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legPntFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legPntMean[uDim] = new TLegend(0.45,0.55,0.9,0.9); 
      legTrkClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legTrkFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legTrkMean[uDim] = new TLegend(0.45,0.55,0.9,0.9);     
      legPntClos[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legPntFurt[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legPntMean[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Mean",     ksDimName[uDim].Data() ) );
      legTrkClos[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legTrkFurt[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legTrkMean[uDim]->SetHeader( Form("Fract. of hits vs Dist. in %s, Mean",     ksDimName[uDim].Data() ) );
      legPntClos[uDim]->SetTextSize( kdLegTextSize );
      legPntFurt[uDim]->SetTextSize( kdLegTextSize );
      legPntMean[uDim]->SetTextSize( kdLegTextSize );
      legTrkClos[uDim]->SetTextSize( kdLegTextSize );
      legTrkFurt[uDim]->SetTextSize( kdLegTextSize );
      legTrkMean[uDim]->SetTextSize( kdLegTextSize );
                            
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
         
         hRatioPntHitMeanDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioPntHitMeanDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackPntMean[uDim]->Add( hRatioPntHitMeanDelta[uFile][uDim] );
         legPntMean[uDim]->AddEntry( hRatioPntHitMeanDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hRatioTrkHitClosestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioTrkHitClosestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackTrkClos[uDim]->Add( hRatioTrkHitClosestDelta[uFile][uDim] );
         legTrkClos[uDim]->AddEntry( hRatioTrkHitClosestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hRatioTrkHitFurthestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioTrkHitFurthestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackTrkFurt[uDim]->Add( hRatioTrkHitFurthestDelta[uFile][uDim] );
         legTrkFurt[uDim]->AddEntry( hRatioTrkHitFurthestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hRatioTrkHitMeanDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hRatioTrkHitMeanDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackTrkMean[uDim]->Add( hRatioTrkHitMeanDelta[uFile][uDim] );
         legTrkMean[uDim]->AddEntry( hRatioTrkHitMeanDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
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
      histosStackPntMean[uDim]->Draw("nostack");
      legPntMean[uDim]->Draw();
      
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
      histosStackTrkMean[uDim]->Draw("nostack");
      legTrkMean[uDim]->Draw();
      
      canvRatio[uDim]->SaveAs( Form( "FracComp%s%s.png", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
      canvRatio[uDim]->SaveAs( Form( "FracComp%s%s.pdf", ksDimName[uDim].Data(), sOutFileTag.Data() ) );
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   
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
      if( "T" == ksDimName[uDim] )
      {
         histosStackMultiPntProjClos[uDim] = new THStack(Form("histosStackMultiPntProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Closest Pnt; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntProjFurt[uDim] = new THStack(Form("histosStackMultiPntProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Furthest Pnt; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiPntProjMean[uDim] = new THStack(Form("histosStackMultiPntProjMean%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Mean Pnt; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Closest Trk; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Furthest Trk; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjMean[uDim] = new THStack(Form("histosStackMultiTrkProjMean%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Mean Trk; %s Hit - %s Trk [ps]; Counts [Hits]",
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
         histosStackMultiPntProjMean[uDim] = new THStack(Form("histosStackMultiPntProjMean%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, Hits from multi Pnt, Mean Pnt; %s Hit - %s Pnt [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Closest Trk; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Furthest Trk; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjMean[uDim] = new THStack(Form("histosStackMultiTrkProjMean%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, Hits from multi Trk, Mean Trk; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // else of if( "T" == ksDimName[uDim] )

      legMultiPntProjClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiPntProjFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiPntProjMean[uDim] = new TLegend(0.45,0.55,0.9,0.9); 
      legMultiTrkProjClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjMean[uDim] = new TLegend(0.45,0.55,0.9,0.9);     
      legMultiPntProjClos[uDim]->SetHeader( Form("Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legMultiPntProjFurt[uDim]->SetHeader( Form("Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legMultiPntProjMean[uDim]->SetHeader( Form("Dist. in %s, Mean",     ksDimName[uDim].Data() ) );
      legMultiTrkProjClos[uDim]->SetHeader( Form("Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legMultiTrkProjFurt[uDim]->SetHeader( Form("Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legMultiTrkProjMean[uDim]->SetHeader( Form("Dist. in %s, Mean",     ksDimName[uDim].Data() ) );
      legMultiPntProjClos[uDim]->SetTextSize( kdLegTextSize );
      legMultiPntProjFurt[uDim]->SetTextSize( kdLegTextSize );
      legMultiPntProjMean[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjClos[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjFurt[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjMean[uDim]->SetTextSize( kdLegTextSize );
                            
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
         
         hMultiPntHitMeanDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiPntHitMeanDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiPntProjMean[uDim]->Add( hMultiPntHitMeanDelta_proj[uFile][uDim] );
         legMultiPntProjMean[uDim]->AddEntry( hMultiPntHitMeanDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiTrkHitClosestDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiTrkHitClosestDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjClos[uDim]->Add( hMultiTrkHitClosestDelta_proj[uFile][uDim] );
         legMultiTrkProjClos[uDim]->AddEntry( hMultiTrkHitClosestDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiTrkHitFurthestDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiTrkHitFurthestDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjFurt[uDim]->Add( hMultiTrkHitFurthestDelta_proj[uFile][uDim] );
         legMultiTrkProjFurt[uDim]->AddEntry( hMultiTrkHitFurthestDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
         hMultiTrkHitMeanDelta_proj[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hMultiTrkHitMeanDelta_proj[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjMean[uDim]->Add( hMultiTrkHitMeanDelta_proj[uFile][uDim] );
         legMultiTrkProjMean[uDim]->AddEntry( hMultiTrkHitMeanDelta_proj[uFile][uDim] , sTagFiles[uFile],"l");
         
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
      if( "T" == ksDimName[uDim] )
      {
         histosStackAllPntHitClos[uDim] = new THStack(Form("histosStackAllPntHitClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Closest Pnt if Multi; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackAllPntHitFurt[uDim] = new THStack(Form("histosStackAllPntHitFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Furthest Pnt if Multi; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackAllPntHitMean[uDim] = new THStack(Form("histosStackAllPntHitMean%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Mean Pnt if Multi; %s Hit - %s Pnt [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Closest Trk if Multi; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Furthest Trk if Multi; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjMean[uDim] = new THStack(Form("histosStackMultiTrkProjMean%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Mean Trk if Multi; %s Hit - %s Trk [ps]; Counts [Hits]",
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
         histosStackAllPntHitMean[uDim] = new THStack(Form("histosStackAllPntHitMean%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits, Mean Pnt if Multi; %s Hit - %s Pnt [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjClos[uDim] = new THStack(Form("histosStackMultiTrkProjClos%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Closest Trk if Multi; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjFurt[uDim] = new THStack(Form("histosStackMultiTrkProjFurt%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Furthest Trk if Multi; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
         histosStackMultiTrkProjMean[uDim] = new THStack(Form("histosStackMultiTrkProjMean%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Trk, all Hits, Mean Trk if Multi; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // else of if( "T" == ksDimName[uDim] )

      legAllPntHitClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legAllPntHitFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legAllPntHitMean[uDim] = new TLegend(0.45,0.55,0.9,0.9); 
      legMultiTrkProjClos[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjFurt[uDim] = new TLegend(0.45,0.55,0.9,0.9);  
      legMultiTrkProjMean[uDim] = new TLegend(0.45,0.55,0.9,0.9);     
      legAllPntHitClos[uDim]->SetHeader( Form("Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legAllPntHitFurt[uDim]->SetHeader( Form("Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legAllPntHitMean[uDim]->SetHeader( Form("Dist. in %s, Mean",     ksDimName[uDim].Data() ) );
      legMultiTrkProjClos[uDim]->SetHeader( Form("Dist. in %s, Closest",  ksDimName[uDim].Data() ) );
      legMultiTrkProjFurt[uDim]->SetHeader( Form("Dist. in %s, Furthest", ksDimName[uDim].Data() ) );
      legMultiTrkProjMean[uDim]->SetHeader( Form("Dist. in %s, Mean",     ksDimName[uDim].Data() ) );
      legAllPntHitClos[uDim]->SetTextSize( kdLegTextSize );
      legAllPntHitFurt[uDim]->SetTextSize( kdLegTextSize );
      legAllPntHitMean[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjClos[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjFurt[uDim]->SetTextSize( kdLegTextSize );
      legMultiTrkProjMean[uDim]->SetTextSize( kdLegTextSize );
                            
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
         
         hAllPntHitMeanDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllPntHitMeanDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackAllPntHitMean[uDim]->Add( hAllPntHitMeanDelta[uFile][uDim] );
         legAllPntHitMean[uDim]->AddEntry( hAllPntHitMeanDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllTrkHitClosestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllTrkHitClosestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjClos[uDim]->Add( hAllTrkHitClosestDelta[uFile][uDim] );
         legMultiTrkProjClos[uDim]->AddEntry( hAllTrkHitClosestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllTrkHitFurthestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllTrkHitFurthestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjFurt[uDim]->Add( hAllTrkHitFurthestDelta[uFile][uDim] );
         legMultiTrkProjFurt[uDim]->AddEntry( hAllTrkHitFurthestDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
         hAllTrkHitMeanDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
         hAllTrkHitMeanDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
         histosStackMultiTrkProjMean[uDim]->Add( hAllTrkHitMeanDelta[uFile][uDim] );
         legMultiTrkProjMean[uDim]->AddEntry( hAllTrkHitMeanDelta[uFile][uDim] , sTagFiles[uFile],"l");
         
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
         hRatioPntHitMeanDelta[uFile][uDim]    ->Write();
         hRatioTrkHitClosestDelta[uFile][uDim] ->Write();
         hRatioTrkHitFurthestDelta[uFile][uDim]->Write();
         hRatioTrkHitMeanDelta[uFile][uDim]    ->Write();
      } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   fileTest->Close();   
   
   return kTRUE;
}             
