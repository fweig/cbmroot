const UInt_t   kuNbHistos= 5;
const UInt_t   kuNbDim   = 5;
const TString  ksDimName[kuNbDim] = { "X", "Y", "Z", "R", "T" }; 
const Color_t	kcFileColor[kuNbHistos] = { kBlue, kRed, kGreen, kOrange, kMagenta };
const Int_t    kiLineWidth    = 2;
const Double_t kdLegTextSize  = 0.04;
const Double_t kdPadTitleSize = 0.1;


const Int_t   kiNbPart = 6;
const TString ksPartTag[kiNbPart] = 
   { "pip", "pim", "kp", "km", "p",  "pbar" };

Bool_t hitProdDigiComp( TString sFilenameA = "data/tofqa.sis300_electron_auau.25gev.centr._HitProd.hst.all.root",
                    TString sFilenameB = "data/tofqa.sis300_electron_auau.25gev.centr._qa.hst.all.root",
                    TString sTagFilesA = "HitProd", 
                    TString sTagFilesB = "DigiClust",
                    TString sOutFileTag = "_25gev",
                    Bool_t  bSanityCheck = kFALSE 
                    )
{
   
   // Open the input files
   TFile * filePntA = new TFile( sFilenameA, "READ" );
   TFile * filePntB = new TFile( sFilenameB, "READ" );
   
   if( NULL == filePntA || NULL == filePntB )
   {
      cout<<"One of the input files could not be opened: "
          <<sFilenameA<<" -> "<<filePntA<<" "
          <<sFilenameB<<" -> "<<filePntB<<" "
          <<endl;
      return kFALSE;    
   } // if( NULL == filePntA || NULL == filePntB )
   
   // First recover all input hitograms from files
   gROOT->cd();
   TH1 * hSinglePointHitHpDelta[kuNbDim];
   TH1 * hDiffPointHitLeftDelta[kuNbDim];
   TH1 * hDiffPointHitRightDelta[kuNbDim];
   
   TH1 * hSinglePointHitDelta[kuNbDim];
   TH2 * hMultiPntHitClosestDelta[kuNbDim];
   TH2 * hMultiPntHitFurthestDelta[kuNbDim];
   TH2 * hMultiPntHitMeanDelta[kuNbDim];
   
   TH1 * hMultiPntHitClosestDelta_proj[kuNbDim];
   TH1 * hMultiPntHitFurthestDelta_proj[kuNbDim];
   TH1 * hMultiPntHitMeanDelta_proj[kuNbDim];
   
   TH1 * hAllPntHitHpLeftDelta[kuNbDim];
   TH1 * hAllPntHitHpRightDelta[kuNbDim];
   TH1 * hAllPntHitClosestDelta[kuNbDim];
   TH1 * hAllPntHitFurthestDelta[kuNbDim];
   TH1 * hAllPntHitMeanDelta[kuNbDim];
   
   // Particles distributions (objects)
   TH1 * fvhHpPlabGenTrk[kiNbPart];
   TH1 * fvhHpPlabStsPnt[kiNbPart];
   TH1 * fvhHpPlabTofPnt[kiNbPart];
   TH1 * fvhHpPlabTofHit[kiNbPart];
   TH1 * fvhDcPlabGenTrk[kiNbPart];
   TH1 * fvhDcPlabStsPnt[kiNbPart];
   TH1 * fvhDcPlabTofPnt[kiNbPart];
   TH1 * fvhDcPlabTofHit[kiNbPart];
   
   // Tracks matching specific stages
   TH1 * fvhHpPlabGenTrkTofPnt[kiNbPart];
   TH1 * fvhHpPlabGenTrkTofHit[kiNbPart];
   TH1 * fvhHpPlabStsTrkTofPnt[kiNbPart];
   TH1 * fvhHpPlabStsTrkTofHit[kiNbPart];
   TH1 * fvhDcPlabGenTrkTofPnt[kiNbPart];
   TH1 * fvhDcPlabGenTrkTofHit[kiNbPart];
   TH1 * fvhDcPlabStsTrkTofPnt[kiNbPart];
   TH1 * fvhDcPlabStsTrkTofHit[kiNbPart];
   
   // Ratios
   TH1 * fvhRatioPlabTofHit[kiNbPart];
   TH1 * fvhRatioPlabGenTrkTofHit[kiNbPart];
   TH1 * fvhRatioPlabStsTrkTofHit[kiNbPart];
   
   // Hit position comparison
      // XZ
   TH2 * fhHpHitMapXZ;
   TH2 * fhDcHitMapXZ;
   TH2 * fhRatioHitMapXZ;
      // YZ
   TH2 * fhHpHitMapYZ;
   TH2 * fhDcHitMapYZ;
   TH2 * fhRatioHitMapYZ;
      // Angular
   TH2 * fhHpHitMapAng;
   TH2 * fhDcHitMapAng;
   TH2 * fhRatioHitMapAng;
   
   TH1 * tempOneDimHist = NULL;
   TH2 * tempTwoDimHist = NULL;
   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {
/*
      tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
            Form("TofTests_SinglePointHitDelta%s",  ksDimName[uDim].Data()) ) );
      if( NULL != tempOneDimHist )
         hSinglePointHitHpDelta[uDim] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_SinglePointHitHpDelta%s", sTagFilesA.Data(), ksDimName[uDim].Data()) ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
            Form("HitProd_DiffPointHitLeftDelta%s",  ksDimName[uDim].Data()) ) );
      if( NULL != tempOneDimHist )
         hDiffPointHitLeftDelta[uDim] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_DiffPointHitLeftDelta%s", sTagFilesA.Data(), ksDimName[uDim].Data()) ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
            Form("HitProd_DiffPointHitRightDelta%s",  ksDimName[uDim].Data()) ) );
      if( NULL != tempOneDimHist )
         hDiffPointHitRightDelta[uDim] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_DiffPointHitRightDelta%s", sTagFilesA.Data(), ksDimName[uDim].Data()) ) );
         else return kFALSE;
      
      hAllPntHitHpLeftDelta[uDim]  = (TH1*)(hDiffPointHitLeftDelta[uDim]->Clone( 
               Form("%s_AllPntHitHpLeftDelta%s", sTagFilesA.Data(), ksDimName[uDim].Data()) ) );
      hAllPntHitHpRightDelta[uDim]  = (TH1*)(hDiffPointHitRightDelta[uDim]->Clone( 
               Form("%s_AllPntHitHpRightDelta%s", sTagFilesA.Data(), ksDimName[uDim].Data()) ) );

      hAllPntHitHpLeftDelta[uDim] ->Add( hSinglePointHitHpDelta[uDim] );
      hAllPntHitHpRightDelta[uDim]->Add( hSinglePointHitHpDelta[uDim] );
*/
      tempTwoDimHist = (TH2*) (filePntA->FindObjectAny( 
            Form("TofTests_MultiPntHitMeanDelta%s",  ksDimName[uDim].Data()) ) );
      if( NULL == tempTwoDimHist )
         return kFALSE;
      
      hAllPntHitHpLeftDelta[uDim]  = (TH1*)(tempTwoDimHist->ProjectionX( 
               Form("%s_AllPntHitHpLeftDelta%s", sTagFilesA.Data(), ksDimName[uDim].Data()) ) );
      hAllPntHitHpRightDelta[uDim]  = (TH1*)(tempTwoDimHist->ProjectionX( 
               Form("%s_AllPntHitHpRightDelta%s", sTagFilesA.Data(), ksDimName[uDim].Data()) ) );

      tempOneDimHist = (TH1*) (filePntB->FindObjectAny( 
            Form("TofTests_SinglePointHitDelta%s",  ksDimName[uDim].Data()) ) );
      if( NULL != tempOneDimHist )
         hSinglePointHitDelta[uDim] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_SinglePointHitDelta%s", sTagFilesB.Data(), ksDimName[uDim].Data()) ) );
         else return kFALSE;

      tempTwoDimHist = (TH2*) (filePntB->FindObjectAny( 
            Form("TofTests_MultiPntHitClosestDelta%s",  ksDimName[uDim].Data()) ) );
      if( NULL != tempTwoDimHist )
         hMultiPntHitClosestDelta[uDim] = (TH2*)(tempTwoDimHist->Clone( 
               Form("%s_MultiPntHitClosestDelta%s", sTagFilesB.Data(), ksDimName[uDim].Data()) ) );
         else return kFALSE;

      tempTwoDimHist = (TH2*) (filePntB->FindObjectAny( 
            Form("TofTests_MultiPntHitFurthestDelta%s",  ksDimName[uDim].Data()) ) );
      if( NULL != tempTwoDimHist )
         hMultiPntHitFurthestDelta[uDim] = (TH2*)(tempTwoDimHist->Clone( 
               Form("%s_MultiPntHitFurthestDelta%s", sTagFilesB.Data(), ksDimName[uDim].Data()) ) );
         else return kFALSE;

      tempTwoDimHist = (TH2*) (filePntB->FindObjectAny( 
            Form("TofTests_MultiPntHitMeanDelta%s",  ksDimName[uDim].Data()) ) );
      if( NULL != tempTwoDimHist )
         hMultiPntHitMeanDelta[uDim] = (TH2*)(tempTwoDimHist->Clone( 
               Form("%s_MultiPntHitMeanDelta%s", sTagFilesB.Data(), ksDimName[uDim].Data()) ) );
         else return kFALSE;
         
      hMultiPntHitClosestDelta_proj[uDim]  = hMultiPntHitClosestDelta[uDim]->ProjectionX(
            Form("%s_MultiPntHitClosestDelta%s_proj", sTagFilesB.Data(), ksDimName[uDim].Data()) );
      hMultiPntHitFurthestDelta_proj[uDim] = hMultiPntHitFurthestDelta[uDim]->ProjectionX(
            Form("%s_MultiPntHitFurthestDelta%s_proj", sTagFilesB.Data(), ksDimName[uDim].Data()) );
      hMultiPntHitMeanDelta_proj[uDim]     = hMultiPntHitMeanDelta[uDim]->ProjectionX(
            Form("%s_MultiPntHitMeanDelta%s_proj", sTagFilesB.Data(), ksDimName[uDim].Data()) );
            
      hAllPntHitClosestDelta[uDim]  = (TH1*)(hMultiPntHitClosestDelta_proj[uDim]->Clone( 
               Form("%s_AllPntHitClosestDelta%s", sTagFilesB.Data(), ksDimName[uDim].Data()) ) );
      hAllPntHitFurthestDelta[uDim] = (TH1*)(hMultiPntHitFurthestDelta_proj[uDim]->Clone( 
               Form("%s_AllPntHitFurthestDelta%s", sTagFilesB.Data(), ksDimName[uDim].Data()) ) );
      hAllPntHitMeanDelta[uDim]     = (TH1*)(hMultiPntHitMeanDelta_proj[uDim]->Clone( 
               Form("%s_AllPntHitMeanDelta%s", sTagFilesB.Data(), ksDimName[uDim].Data()) ) );
               
      hAllPntHitClosestDelta[uDim] ->Add( hSinglePointHitDelta[uDim] );
      hAllPntHitFurthestDelta[uDim]->Add( hSinglePointHitDelta[uDim] );
      hAllPntHitMeanDelta[uDim]    ->Add( hSinglePointHitDelta[uDim] );
   } // Loop on dimensions
   
   
   // Now loop on species and get the distribution of particles at different stages
   for( Int_t iPart = 0; iPart < kiNbPart; iPart++ )
   {
      // Should be no difference between HitProd and Digi+Clust
      // at MC level
      // => These plots are optional and only for testing nothing 
      //    went completely wrong
      if( kTRUE == bSanityCheck )
      {
         // HP
         tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
               Form("TofTests_PlabGenTrk_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhHpPlabGenTrk[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabGenTrk_%s", sTagFilesA.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
         tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
               Form("TofTests_PlabStsPnt_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhHpPlabStsPnt[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabStsPnt_%s", sTagFilesA.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
         tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
               Form("TofTests_PlabTofPnt_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhHpPlabTofPnt[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabTofPnt_%s", sTagFilesA.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
         // D+C
         tempOneDimHist = (TH1*) (filePntB->FindObjectAny( 
               Form("TofTests_PlabGenTrk_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhDcPlabGenTrk[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabGenTrk_%s", sTagFilesB.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
         tempOneDimHist = (TH1*) (filePntB->FindObjectAny( 
               Form("TofTests_PlabStsPnt_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhDcPlabStsPnt[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabStsPnt_%s", sTagFilesB.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
         tempOneDimHist = (TH1*) (filePntB->FindObjectAny( 
               Form("TofTests_PlabTofPnt_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhDcPlabTofPnt[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabTofPnt_%s", sTagFilesB.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
         // HP
         tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
               Form("TofTests_PlabGenTrkTofPnt_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhHpPlabGenTrkTofPnt[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabGenTrkTofPnt_%s", sTagFilesA.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
         tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
               Form("TofTests_PlabStsTrkTofPnt_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhHpPlabStsTrkTofPnt[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabStsTrkTofPnt_%s", sTagFilesA.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
         // D+C
         tempOneDimHist = (TH1*) (filePntB->FindObjectAny( 
               Form("TofTests_PlabGenTrkTofPnt_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhDcPlabGenTrkTofPnt[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabGenTrkTofPnt_%s", sTagFilesB.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
         tempOneDimHist = (TH1*) (filePntB->FindObjectAny( 
               Form("TofTests_PlabStsTrkTofPnt_%s",  ksPartTag[iPart].Data()) ) );
         if( NULL != tempOneDimHist )
            fvhDcPlabStsTrkTofPnt[iPart] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabStsTrkTofPnt_%s", sTagFilesB.Data(), ksPartTag[iPart].Data()) ) );
            else return kFALSE;
         
      } // if( kTRUE == bSanityCheck )
      
      // HP
      tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
            Form("TofTests_PlabTofHit_%s",  ksPartTag[iPart].Data()) ) );
      if( NULL != tempOneDimHist )
         fvhHpPlabTofHit[iPart] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_PlabTofHit_%s", sTagFilesA.Data(), ksPartTag[iPart].Data()) ) );
         else return kFALSE;

      // D+C
      tempOneDimHist = (TH1*) (filePntB->FindObjectAny( 
            Form("TofTests_PlabTofHit_%s",  ksPartTag[iPart].Data()) ) );
      if( NULL != tempOneDimHist )
         fvhDcPlabTofHit[iPart] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_PlabTofHit_%s", sTagFilesB.Data(), ksPartTag[iPart].Data()) ) );
         else return kFALSE;

      // HP
      tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
            Form("TofTests_PlabGenTrkTofHit_%s",  ksPartTag[iPart].Data()) ) );
      if( NULL != tempOneDimHist )
         fvhHpPlabGenTrkTofHit[iPart] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_PlabGenTrkTofHit_%s", sTagFilesA.Data(), ksPartTag[iPart].Data()) ) );
         else return kFALSE;
      
      tempOneDimHist = (TH1*) (filePntA->FindObjectAny( 
            Form("TofTests_PlabStsTrkTofHit_%s",  ksPartTag[iPart].Data()) ) );
      if( NULL != tempOneDimHist )
         fvhHpPlabStsTrkTofHit[iPart] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_PlabStsTrkTofHit_%s", sTagFilesA.Data(), ksPartTag[iPart].Data()) ) );
         else return kFALSE;
      
      // D+C
      tempOneDimHist = (TH1*) (filePntB->FindObjectAny( 
            Form("TofTests_PlabGenTrkTofHit_%s",  ksPartTag[iPart].Data()) ) );
      if( NULL != tempOneDimHist )
         fvhDcPlabGenTrkTofHit[iPart] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_PlabGenTrkTofHit_%s", sTagFilesB.Data(), ksPartTag[iPart].Data()) ) );
         else return kFALSE;
      
      tempOneDimHist = (TH1*) (filePntB->FindObjectAny( 
            Form("TofTests_PlabStsTrkTofHit_%s",  ksPartTag[iPart].Data()) ) );
      if( NULL != tempOneDimHist )
         fvhDcPlabStsTrkTofHit[iPart] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_PlabStsTrkTofHit_%s", sTagFilesB.Data(), ksPartTag[iPart].Data()) ) );
         else return kFALSE;
         
         
      fvhRatioPlabTofHit[iPart]  = (TH1*)(fvhHpPlabTofHit[iPart]->Clone( 
               Form("RatioPlabTofHit_%s", ksPartTag[iPart].Data()) ) );
      fvhRatioPlabGenTrkTofHit[iPart]  = (TH1*)(fvhHpPlabGenTrkTofHit[iPart]->Clone( 
               Form("RatioPlabGenTrkTofHit_%s", ksPartTag[iPart].Data()) ) );
      fvhRatioPlabStsTrkTofHit[iPart]  = (TH1*)(fvhHpPlabStsTrkTofHit[iPart]->Clone( 
               Form("RatioPlabStsTrkTofHit_%s", ksPartTag[iPart].Data()) ) );
               
      fvhRatioPlabTofHit[iPart]->Divide( 
         fvhDcPlabTofHit[iPart], 
         fvhHpPlabTofHit[iPart], 100.0);
      fvhRatioPlabGenTrkTofHit[iPart]->Divide( 
         fvhDcPlabGenTrkTofHit[iPart], 
         fvhHpPlabGenTrkTofHit[iPart], 100.0);
      fvhRatioPlabStsTrkTofHit[iPart]->Divide( 
         fvhDcPlabStsTrkTofHit[iPart], 
         fvhHpPlabStsTrkTofHit[iPart], 100.0);
      
      fvhRatioPlabTofHit[iPart]->GetZaxis()->SetTitle( "Ratio [\%]" );
      fvhRatioPlabGenTrkTofHit[iPart]->GetZaxis()->SetTitle( "Ratio [\%]" );
      fvhRatioPlabStsTrkTofHit[iPart]->GetZaxis()->SetTitle( "Ratio [\%]" );
   } // for( Int_t iPart = 0; iPart < kiNbPart; iPart++ )
   
   // Compare the position of hits in XZ coordinates
   tempTwoDimHist = (TH2*) (filePntA->FindObjectAny( "TofTests_HitsMapXZ" ) );
   if( NULL != tempTwoDimHist )
      fhHpHitMapXZ = (TH2*)(tempTwoDimHist->Clone( 
            Form("%s_HitsMapXZ", sTagFilesA.Data()) ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (filePntB->FindObjectAny( "TofTests_HitsMapXZ" ) );
   if( NULL != tempTwoDimHist )
      fhDcHitMapXZ = (TH2*)(tempTwoDimHist->Clone( 
            Form("%s_HitsMapXZ", sTagFilesB.Data()) ) );
      else return kFALSE;
   
   fhRatioHitMapXZ = (TH2*)(fhHpHitMapXZ->Clone( "RatioHitMapXZ" ) );
   fhRatioHitMapXZ->Divide( fhDcHitMapXZ, fhHpHitMapXZ, 100.0);
   fhRatioHitMapXZ->GetZaxis()->SetTitle( "Ratio [\%]" );
   
   // Compare the position of hits in YZ coordinates
   tempTwoDimHist = (TH2*) (filePntA->FindObjectAny( "TofTests_HitsMapYZ" ) );
   if( NULL != tempTwoDimHist )
      fhHpHitMapYZ = (TH2*)(tempTwoDimHist->Clone( 
            Form("%s_HitsMapYZ", sTagFilesA.Data()) ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (filePntB->FindObjectAny( "TofTests_HitsMapYZ" ) );
   if( NULL != tempTwoDimHist )
      fhDcHitMapYZ = (TH2*)(tempTwoDimHist->Clone( 
            Form("%s_HitsMapYZ", sTagFilesB.Data()) ) );
      else return kFALSE;
   
   fhRatioHitMapYZ = (TH2*)(fhHpHitMapYZ->Clone( "RatioHitMapYZ" ) );
   fhRatioHitMapYZ->Divide( fhDcHitMapYZ, fhHpHitMapYZ, 100.0);
   fhRatioHitMapYZ->GetZaxis()->SetTitle( "Ratio [\%]" );
   
   // Compare the position of hits in angular coordinates
   tempTwoDimHist = (TH2*) (filePntA->FindObjectAny( "TofTests_HitsMapAng" ) );
   if( NULL != tempTwoDimHist )
      fhHpHitMapAng = (TH2*)(tempTwoDimHist->Clone( 
            Form("%s_HitsMapAng", sTagFilesA.Data()) ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (filePntB->FindObjectAny( "TofTests_HitsMapAng" ) );
   if( NULL != tempTwoDimHist )
      fhDcHitMapAng = (TH2*)(tempTwoDimHist->Clone( 
            Form("%s_HitsMapAng", sTagFilesB.Data()) ) );
      else return kFALSE;
   
   fhRatioHitMapAng = (TH2*)(fhHpHitMapAng->Clone( "RatioHitMapAng" ) );
   fhRatioHitMapAng->Divide( fhDcHitMapAng, fhHpHitMapAng, 100.0);
   fhRatioHitMapAng->GetZaxis()->SetTitle( "Ratio [\%]" );
   
   // Display SinglePnt, Multi Pnts, Single Trk and Multi Trk histos
   THStack * histosStackAll[kuNbDim];
   TLegend * legAll[kuNbDim];
   
   TCanvas *canvAll = new TCanvas( "canvAll", 
         "Distance between Hit and Point, all hits, comparison of HitProducer and Digitizer+Clusterizer",
         1920, 986 );
   canvAll->cd();
   canvAll->Divide(3,2);
   
   for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   {   
      if( "T" == ksDimName[uDim] )
      {
         histosStackAll[uDim] = new THStack(Form("histosStackAll%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits; %s Hit - %s Trk [ps]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // if( "T" == ksDimName[uDim] )
      else
      {
         histosStackAll[uDim] = new THStack(Form("histosStackAll%s",  ksDimName[uDim].Data()),
               Form("Distance in %s from Pnt, all Hits; %s Hit - %s Trk [cm]; Counts [Hits]",
                           ksDimName[uDim].Data(), ksDimName[uDim].Data(), ksDimName[uDim].Data() ) );
      } // else of if( "T" == ksDimName[uDim] )

      legAll[uDim] = new TLegend(0.55,0.55,0.9,0.9);  
      legAll[uDim]->SetHeader( Form("Dist. in %s",  ksDimName[uDim].Data() ) );
      legAll[uDim]->SetTextSize( kdLegTextSize );

      // HitProd With Left point if multiple MC      
      hAllPntHitHpLeftDelta[uDim]->SetLineColor( kcFileColor[0] );
      hAllPntHitHpLeftDelta[uDim]->SetLineWidth( kiLineWidth );
      histosStackAll[uDim]->Add( hAllPntHitHpLeftDelta[uDim] );
      legAll[uDim]->AddEntry( hAllPntHitHpLeftDelta[uDim] , 
                              Form("%s, Left", sTagFilesA.Data()), "l");
      
      // HitProd With Right point if multiple MC
      hAllPntHitHpRightDelta[uDim]->SetLineColor( kcFileColor[1] );
      hAllPntHitHpRightDelta[uDim]->SetLineWidth( kiLineWidth );
      histosStackAll[uDim]->Add( hAllPntHitHpRightDelta[uDim] );
      legAll[uDim]->AddEntry( hAllPntHitHpRightDelta[uDim] , 
                              Form("%s, Right", sTagFilesA.Data()), "l");
      
      // DigiClust With Closest point if multiple MC
      hAllPntHitClosestDelta[uDim]->SetLineColor( kcFileColor[2] );
      hAllPntHitClosestDelta[uDim]->SetLineWidth( kiLineWidth );
      histosStackAll[uDim]->Add( hAllPntHitClosestDelta[uDim] );
      legAll[uDim]->AddEntry( hAllPntHitClosestDelta[uDim] , 
                              Form("%s, Closest", sTagFilesB.Data()), "l");

      // DigiClust With Furthest point if multiple MC
      hAllPntHitFurthestDelta[uDim]->SetLineColor( kcFileColor[3] );
      hAllPntHitFurthestDelta[uDim]->SetLineWidth( kiLineWidth );
      histosStackAll[uDim]->Add( hAllPntHitFurthestDelta[uDim] );
      legAll[uDim]->AddEntry( hAllPntHitFurthestDelta[uDim] , 
                              Form("%s, Furthest", sTagFilesB.Data()), "l");

      // DigiClust With Mean point if multiple MC
      hAllPntHitMeanDelta[uDim]->SetLineColor( kcFileColor[4] );
      hAllPntHitMeanDelta[uDim]->SetLineWidth( kiLineWidth );
      histosStackAll[uDim]->Add( hAllPntHitMeanDelta[uDim] );
      legAll[uDim]->AddEntry( hAllPntHitMeanDelta[uDim] , 
                              Form("%s, Mean", sTagFilesB.Data()), "l");
      
      canvAll->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackAll[uDim]->Draw("nostack");
      if( 2 == uDim )
         legAll[uDim]->Draw();
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   
   canvAll->SaveAs( Form( "HProdDigi_All_%s.png", sOutFileTag.Data() ) );
   canvAll->SaveAs( Form( "HProdDigi_All_%s.pdf", sOutFileTag.Data() ) );
   
   
   TCanvas *canvPlabTofHit = new TCanvas( "canvPlabTofHit", 
         "Distribution of TofHits as function of Plab, per species, comparison of HitProducer and Digitizer+Clusterizer",
         1920, 986 );
   canvPlabTofHit->cd();
   canvPlabTofHit->Divide(3,2);
   
   TCanvas *canvPlabGenTrkTofHit = new TCanvas( "canvPlabGenTrkTofHit", 
         "Distribution of MC Tracks with TofHits as function of Plab, per species, comparison of HitProducer and Digitizer+Clusterizer",
         1920, 986 );
   canvPlabGenTrkTofHit->cd();
   canvPlabGenTrkTofHit->Divide(3,2);
   
   TCanvas *canvPlabStsTrkTofHit = new TCanvas( "canvPlabStsTrkTofHit", 
         "Distribution of MC Tracks with STS Points and TofHits as function of Plab, per species, comparison of HitProducer and Digitizer+Clusterizer",
         1920, 986 );
   canvPlabStsTrkTofHit->cd();
   canvPlabStsTrkTofHit->Divide(3,2);
   
   TCanvas *canvPlabGenTrk;
   TCanvas *canvPlabStsPnt;
   TCanvas *canvPlabTofPnt;
   TCanvas *canvPlabGenTrkTofPnt;
   TCanvas *canvPlabStsTrkTofPnt;
   
   THStack * histosStackPlabTofHit[kiNbPart];
   THStack * histosStackPlabGenTrkTofHit[kiNbPart];
   THStack * histosStackPlabStsTrkTofHit[kiNbPart];
   
   THStack * histosStackPlabGenTrk[kiNbPart];
   THStack * histosStackPlabStsPnt[kiNbPart];
   THStack * histosStackPlabTofPnt[kiNbPart];
   THStack * histosStackPlabGenTrkTofPnt[kiNbPart];
   THStack * histosStackPlabStsTrkTofPnt[kiNbPart];
   
   if( kTRUE == bSanityCheck )
   {
      canvPlabGenTrk = new TCanvas( "canvPlabGenTrk", 
         "Distribution of MC Trk as function of Plab, per species, comparison of HitProducer and Digitizer+Clusterizer",
         1920, 986 );
      canvPlabGenTrk->cd();
      canvPlabGenTrk->Divide(3,2);
   
      canvPlabStsPnt = new TCanvas( "canvPlabStsPnt", 
         "Distribution of Sts Pnt as function of Plab, per species, comparison of HitProducer and Digitizer+Clusterizer",
         1920, 986 );
      canvPlabStsPnt->cd();
      canvPlabStsPnt->Divide(3,2);
   
      canvPlabTofPnt = new TCanvas( "canvPlabTofPnt", 
         "Distribution of Tof Pnt as function of Plab, per species, comparison of HitProducer and Digitizer+Clusterizer",
         1920, 986 );
      canvPlabTofPnt->cd();
      canvPlabTofPnt->Divide(3,2);
   
      canvPlabGenTrkTofPnt = new TCanvas( "canvPlabGenTrkTofPnt", 
         "Distribution of MC Tracks with Tof Pnt as function of Plab, per species, comparison of HitProducer and Digitizer+Clusterizer",
         1920, 986 );
      canvPlabGenTrkTofPnt->cd();
      canvPlabGenTrkTofPnt->Divide(3,2);
   
      canvPlabStsTrkTofPnt = new TCanvas( "canvPlabStsTrkTofPnt", 
         "Distribution of MC Tracks with STS Points and Tof Pnt as function of Plab, per species, comparison of HitProducer and Digitizer+Clusterizer",
         1920, 986 );
      canvPlabStsTrkTofPnt->cd();
      canvPlabStsTrkTofPnt->Divide(3,2);
   } // if( kTRUE == bSanityCheck )
      
   for( Int_t iPart = 0; iPart < kiNbPart; iPart++ )
   {
      if( kTRUE == bSanityCheck )
      {   
         // MC Trk
         histosStackPlabGenTrk[iPart] = new THStack(Form("histosStackPlabGenTrk%s",  ksPartTag[iPart].Data()),
               Form("Distribution of MC Trk for %s, all Hits; Plab [GeV]; Counts [Hits]",
                           ksPartTag[iPart].Data() ) );
                              
         canvPlabGenTrk->cd( 1 +iPart );
         fvhHpPlabGenTrk[iPart]->SetLineColor( kcFileColor[0] );
         fvhHpPlabGenTrk[iPart]->SetLineWidth( kiLineWidth );
         fvhDcPlabGenTrk[iPart]->SetLineColor( kcFileColor[1] );
         fvhDcPlabGenTrk[iPart]->SetLineWidth( kiLineWidth );
         
         histosStackPlabGenTrk[iPart]->Add( fvhHpPlabGenTrk[iPart] );
         histosStackPlabGenTrk[iPart]->Add( fvhDcPlabGenTrk[iPart] );
         
         histosStackPlabGenTrk[iPart]->Draw("nostack");
   
         // STS Pnt
         histosStackPlabStsPnt[iPart] = new THStack(Form("histosStackPlabStsPnt%s",  ksPartTag[iPart].Data()),
               Form("Distribution of STS Pnt for %s, all Hits; Plab [GeV]; Counts [Hits]",
                           ksPartTag[iPart].Data() ) );
                              
         canvPlabStsPnt->cd( 1 +iPart );
         fvhHpPlabStsPnt[iPart]->SetLineColor( kcFileColor[0] );
         fvhHpPlabStsPnt[iPart]->SetLineWidth( kiLineWidth );
         fvhDcPlabStsPnt[iPart]->SetLineColor( kcFileColor[1] );
         fvhDcPlabStsPnt[iPart]->SetLineWidth( kiLineWidth );
         
         histosStackPlabStsPnt[iPart]->Add( fvhHpPlabStsPnt[iPart] );
         histosStackPlabStsPnt[iPart]->Add( fvhDcPlabStsPnt[iPart] );
         
         histosStackPlabStsPnt[iPart]->Draw("nostack");
   
         // TOF Pnt
         histosStackPlabTofPnt[iPart] = new THStack(Form("histosStackPlabTofPnt%s",  ksPartTag[iPart].Data()),
               Form("Distribution of MC Trk for %s, all Hits; Plab [GeV]; Counts [Hits]",
                           ksPartTag[iPart].Data() ) );
                              
         canvPlabTofPnt->cd( 1 +iPart );
         fvhHpPlabTofPnt[iPart]->SetLineColor( kcFileColor[0] );
         fvhHpPlabTofPnt[iPart]->SetLineWidth( kiLineWidth );
         fvhDcPlabTofPnt[iPart]->SetLineColor( kcFileColor[1] );
         fvhDcPlabTofPnt[iPart]->SetLineWidth( kiLineWidth );
         
         histosStackPlabTofPnt[iPart]->Add( fvhHpPlabTofPnt[iPart] );
         histosStackPlabTofPnt[iPart]->Add( fvhDcPlabTofPnt[iPart] );
         
         histosStackPlabTofPnt[iPart]->Draw("nostack");
         
         // MC Tracks with Tof Pnt
         histosStackPlabGenTrkTofPnt[iPart] = new THStack(Form("histosStackPlabGenTrkTofPnt%s",  ksPartTag[iPart].Data()),
               Form("Distribution of MC Tracks with TofPnts for %s, all Pnts; Plab [GeV]; Counts [Tracks]",
                           ksPartTag[iPart].Data() ) );
                              
         canvPlabGenTrkTofPnt->cd( 1 +iPart );
         fvhHpPlabGenTrkTofPnt[iPart]->SetLineColor( kcFileColor[0] );
         fvhHpPlabGenTrkTofPnt[iPart]->SetLineWidth( kiLineWidth );
         fvhDcPlabGenTrkTofPnt[iPart]->SetLineColor( kcFileColor[1] );
         fvhDcPlabGenTrkTofPnt[iPart]->SetLineWidth( kiLineWidth );
         
         histosStackPlabGenTrkTofPnt[iPart]->Add( fvhHpPlabGenTrkTofPnt[iPart] );
         histosStackPlabGenTrkTofPnt[iPart]->Add( fvhDcPlabGenTrkTofPnt[iPart] );
         
         histosStackPlabGenTrkTofPnt[iPart]->Draw("nostack");
         
         // MC Tracks with enough STS Points and Tof Pnt
         histosStackPlabStsTrkTofPnt[iPart] = new THStack(Form("histosStackPlabStsTrkTofPnt%s",  ksPartTag[iPart].Data()),
               Form("Distribution of MC Tracks with STS Points and TofPnts for %s, all Pnts; Plab [GeV]; Counts [Tracks]",
                           ksPartTag[iPart].Data() ) );
                              
         canvPlabStsTrkTofPnt->cd( 1 +iPart );
         fvhHpPlabStsTrkTofPnt[iPart]->SetLineColor( kcFileColor[0] );
         fvhHpPlabStsTrkTofPnt[iPart]->SetLineWidth( kiLineWidth );
         fvhDcPlabStsTrkTofPnt[iPart]->SetLineColor( kcFileColor[1] );
         fvhDcPlabStsTrkTofPnt[iPart]->SetLineWidth( kiLineWidth );
         
         histosStackPlabStsTrkTofPnt[iPart]->Add( fvhHpPlabStsTrkTofPnt[iPart] );
         histosStackPlabStsTrkTofPnt[iPart]->Add( fvhDcPlabStsTrkTofPnt[iPart] );
         
         histosStackPlabStsTrkTofPnt[iPart]->Draw("nostack");
         
         // Tof Hits
         histosStackPlabTofHit[iPart] = new THStack(Form("histosStackPlabTofHit%s",  ksPartTag[iPart].Data()),
               Form("Distribution of Tof hits for %s, all Hits; Plab [GeV]; Counts [Hits]",
                           ksPartTag[iPart].Data() ) );
                              
         canvPlabTofHit->cd( 1 +iPart );
         fvhHpPlabTofHit[iPart]->SetLineColor( kcFileColor[0] );
         fvhHpPlabTofHit[iPart]->SetLineWidth( kiLineWidth );
         fvhDcPlabTofHit[iPart]->SetLineColor( kcFileColor[1] );
         fvhDcPlabTofHit[iPart]->SetLineWidth( kiLineWidth );
         
         histosStackPlabTofHit[iPart]->Add( fvhHpPlabTofHit[iPart] );
         histosStackPlabTofHit[iPart]->Add( fvhDcPlabTofHit[iPart] );
         
         histosStackPlabTofHit[iPart]->Draw("nostack");
         
         // MC Tracks with Tof Hit
         histosStackPlabGenTrkTofHit[iPart] = new THStack(Form("histosStackPlabGenTrkTofHit%s",  ksPartTag[iPart].Data()),
               Form("Distribution of MC Tracks with TofHits for %s, all Hits; Plab [GeV]; Counts [Tracks]",
                           ksPartTag[iPart].Data() ) );
                              
         canvPlabGenTrkTofHit->cd( 1 +iPart );
         fvhHpPlabGenTrkTofHit[iPart]->SetLineColor( kcFileColor[0] );
         fvhHpPlabGenTrkTofHit[iPart]->SetLineWidth( kiLineWidth );
         fvhDcPlabGenTrkTofHit[iPart]->SetLineColor( kcFileColor[1] );
         fvhDcPlabGenTrkTofHit[iPart]->SetLineWidth( kiLineWidth );
         
         histosStackPlabGenTrkTofHit[iPart]->Add( fvhHpPlabGenTrkTofHit[iPart] );
         histosStackPlabGenTrkTofHit[iPart]->Add( fvhDcPlabGenTrkTofHit[iPart] );
         
         histosStackPlabGenTrkTofHit[iPart]->Draw("nostack");
         
         // MC Tracks with enough STS Points and Tof Hit
         histosStackPlabStsTrkTofHit[iPart] = new THStack(Form("histosStackPlabStsTrkTofHit%s",  ksPartTag[iPart].Data()),
               Form("Distribution of MC Tracks with STS Points and TofHits for %s, all Hits; Plab [GeV]; Counts [Tracks]",
                           ksPartTag[iPart].Data() ) );
                              
         canvPlabStsTrkTofHit->cd( 1 +iPart );
         fvhHpPlabStsTrkTofHit[iPart]->SetLineColor( kcFileColor[0] );
         fvhHpPlabStsTrkTofHit[iPart]->SetLineWidth( kiLineWidth );
         fvhDcPlabStsTrkTofHit[iPart]->SetLineColor( kcFileColor[1] );
         fvhDcPlabStsTrkTofHit[iPart]->SetLineWidth( kiLineWidth );
         
         histosStackPlabStsTrkTofHit[iPart]->Add( fvhHpPlabStsTrkTofHit[iPart] );
         histosStackPlabStsTrkTofHit[iPart]->Add( fvhDcPlabStsTrkTofHit[iPart] );
         
         histosStackPlabStsTrkTofHit[iPart]->Draw("nostack");
      } // if( kTRUE == bSanityCheck )
         else
         {
            // Tof Hits
            canvPlabTofHit->cd( 1 +iPart );
            fvhRatioPlabTofHit[iPart]->SetLineColor( kcFileColor[0] );
            fvhRatioPlabTofHit[iPart]->SetLineWidth( kiLineWidth );
            fvhRatioPlabTofHit[iPart]->Draw("");
            
            // MC Tracks with Tof Hit
                                 
            canvPlabGenTrkTofHit->cd( 1 +iPart );
            fvhRatioPlabGenTrkTofHit[iPart]->SetLineColor( kcFileColor[0] );
            fvhRatioPlabGenTrkTofHit[iPart]->SetLineWidth( kiLineWidth );
            fvhRatioPlabGenTrkTofHit[iPart]->Draw("");
            
            // MC Tracks with enough STS Points and Tof Hit
            canvPlabStsTrkTofHit->cd( 1 +iPart );
            fvhRatioPlabStsTrkTofHit[iPart]->SetLineColor( kcFileColor[0] );
            fvhRatioPlabStsTrkTofHit[iPart]->SetLineWidth( kiLineWidth );
            fvhRatioPlabStsTrkTofHit[iPart]->Draw("");
         } // else of if( kTRUE == bSanityCheck )
   } // for( Int_t iPart = 0; iPart < kiNbPart; iPart++ )
      
   // XZ
   TCanvas *canvRatioHitsMapXZ = new TCanvas( "canvRatioHitsMapXZ", 
         "Ratio of TofHits counts of Digitizer+Clusterizer over HitProducer, in XZ coordinates",
         1920, 986 );
   canvRatioHitsMapXZ->cd();
   canvRatioHitsMapXZ->Divide(2,2);
   
   canvRatioHitsMapXZ->cd(1);
   fhRatioHitMapXZ->GetZaxis()->SetRangeUser( 0.0, 200.0);
   fhRatioHitMapXZ->SetStats(kFALSE);
   fhRatioHitMapXZ->Draw("colz");

   canvRatioHitsMapXZ->cd(2);
   gPad->SetLogz();
   fhHpHitMapXZ->SetStats(kFALSE);
   fhHpHitMapXZ->Draw("colz");
   
   canvRatioHitsMapXZ->cd(3);
   gPad->SetLogz();
   fhDcHitMapXZ->SetStats(kFALSE);
   fhDcHitMapXZ->Draw("colz");
   
   // YZ
   TCanvas *canvRatioHitsMapYZ = new TCanvas( "canvRatioHitsMapYZ", 
         "Ratio of TofHits counts of Digitizer+Clusterizer over HitProducer, in YZ coordinates",
         1920, 986 );
   canvRatioHitsMapYZ->cd();
   canvRatioHitsMapYZ->Divide(2,2);
   
   canvRatioHitsMapYZ->cd(1);
   fhRatioHitMapYZ->GetZaxis()->SetRangeUser( 0.0, 200.0);
   fhRatioHitMapYZ->SetStats(kFALSE);
   fhRatioHitMapYZ->Draw("colz");

   canvRatioHitsMapYZ->cd(2);
   gPad->SetLogz();
   fhHpHitMapYZ->SetStats(kFALSE);
   fhHpHitMapYZ->Draw("colz");
   
   canvRatioHitsMapYZ->cd(3);
   gPad->SetLogz();
   fhDcHitMapYZ->SetStats(kFALSE);
   fhDcHitMapYZ->Draw("colz");
   
   // Angular
   TCanvas *canvRatioHitsMapAng = new TCanvas( "canvRatioHitsMapAng", 
         "Ratio of TofHits counts of Digitizer+Clusterizer over HitProducer, in angular coordinates",
         1920, 986 );
   canvRatioHitsMapAng->cd();
   canvRatioHitsMapAng->Divide(2,2);
   
   canvRatioHitsMapAng->cd(1);
   fhRatioHitMapAng->GetZaxis()->SetRangeUser( 0.0, 200.0);
   fhRatioHitMapAng->SetStats(kFALSE);
   fhRatioHitMapAng->Draw("colz");

   canvRatioHitsMapAng->cd(2);
   gPad->SetLogz();
   fhHpHitMapAng->SetStats(kFALSE);
   fhHpHitMapAng->Draw("colz");
   
   canvRatioHitsMapAng->cd(3);
   gPad->SetLogz();
   fhDcHitMapAng->SetStats(kFALSE);
   fhDcHitMapAng->Draw("colz");

   return kTRUE;
}
                       
