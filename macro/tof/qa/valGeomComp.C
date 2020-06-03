
const UInt_t   kuNbFiles = 4;
const UInt_t   kuNbDim   = 5;
const TString  ksDimName[kuNbDim] = { "X", "Y", "Z", "R", "T" }; 
const Color_t	kcFileColor[kuNbFiles] = { kBlue, kRed, kGreen, kOrange };
const Int_t    kiLineWidth    = 2;
const Double_t kdLegTextSize  = 0.04;
const Double_t kdPadTitleSize = 0.1;

//___________________________________________________________________
// Constants definitions: Particles list
const Int_t   kiNbPart =  9;
const TString ksPartTag[kiNbPart] = 
   { "others",
     "ep", "em",   "pip", "pim", "kp", "km", 
     "p",  "pbar" };
const TString ksPartName[kiNbPart] = 
   { "any other part.", 
     "e-", "e+",   "#pi+", "#pi-", "k+", "k-", 
     "p",  "anti-p"};
const Int_t   kiMinNbStsPntAcc = 3; // Number of STS Pnt for Trk to be reconstructable
//___________________________________________________________________

Bool_t valGeomComp( Bool_t bSavePlots = kFALSE,
                    TString sFilenameA = "data/tofqaHP_sis100_electron_auau_10gev_centr__qa_hst_all.root",
                    TString sFilenameB = "data/tofqaHP_new_sis100_electron_auau_10gev_centr__qa_hst_all.root",
                    TString sFilenameC = "data/tofqaHP_sis100_electron_auau_10gev_centr_noTRD_qa_hst_all.root",
                    TString sFilenameD = "data/tofqaHP_new_sis100_electron_auau_10gev_centr_noTRD_qa_hst_all.root",
                    TString sTagFilesA = "v13-5a",       TString sTagFilesB = "v16a_1h",
                    TString sTagFilesC = "v13-5a_noTrd", TString sTagFilesD = "v16a_1h_noTrd",
                    TString sOutFileTag = "_10gev" 
                    )
{
   
   // Open the input files
   TFile * filePnt[kuNbFiles];
   filePnt[0] = new TFile( sFilenameA, "READ" );
   filePnt[1] = new TFile( sFilenameB, "READ" );
   filePnt[2] = new TFile( sFilenameC, "READ" );
   filePnt[3] = new TFile( sFilenameD, "READ" );
   
   TString sTagFiles[kuNbFiles];
   sTagFiles[0] = sTagFilesA;
   sTagFiles[1] = sTagFilesB;
   sTagFiles[2] = sTagFilesC;
   sTagFiles[3] = sTagFilesD;
   
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
   TH2 * hMultiPntHitBestDelta[kuNbFiles][kuNbDim];
   
   TH1 * hMultiPntHitClosestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiPntHitFurthestDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiPntHitMeanDelta_proj[kuNbFiles][kuNbDim];
   TH1 * hMultiPntHitBestDelta_proj[kuNbFiles][kuNbDim];
   
   TH1 * hAllPntHitClosestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitFurthestDelta[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitMeanDelta[kuNbFiles][kuNbDim];
   TH1 * hAllPntHitBestDelta[kuNbFiles][kuNbDim];
   
   TH1 * hIntegratedPntHitEff[kuNbFiles];
   TH1 * hIntegratedPntHitEffPrim[kuNbFiles];
   TH1 * hIntegratedPntHitEffSec[kuNbFiles];
   
   TH1 * hPlabGenTrk[kuNbFiles][kiNbPart];
   TH1 * hPlabStsPnt[kuNbFiles][kiNbPart];
   TH1 * hPlabGenTrkTofPnt[kuNbFiles][kiNbPart];
   TH1 * hPlabGenTrkTofHit[kuNbFiles][kiNbPart];
   TH1 * hPlabStsTrkTofPnt[kuNbFiles][kiNbPart];
   TH1 * hPlabStsTrkTofHit[kuNbFiles][kiNbPart];
   TH1 * hPlabTofPntEff[kuNbFiles][kiNbPart];
   TH1 * hPlabTofHitEff[kuNbFiles][kiNbPart];
   TH1 * hPlabTofHitGenEff[kuNbFiles][kiNbPart];
   TH1 * hPlabTofPntEffSts[kuNbFiles][kiNbPart];
   TH1 * hPlabTofHitEffSts[kuNbFiles][kiNbPart];
   
   TH1 * hPlabSecGenTrk[kuNbFiles][kiNbPart];
   TH1 * hPlabSecStsPnt[kuNbFiles][kiNbPart];
   TH1 * hPlabSecGenTrkTofPnt[kuNbFiles][kiNbPart];
   TH1 * hPlabSecGenTrkTofHit[kuNbFiles][kiNbPart];
   TH1 * hPlabSecStsTrkTofPnt[kuNbFiles][kiNbPart];
   TH1 * hPlabSecStsTrkTofHit[kuNbFiles][kiNbPart];
   TH1 * hPlabSecTofPntEff[kuNbFiles][kiNbPart];
   TH1 * hPlabSecTofHitEff[kuNbFiles][kiNbPart];
   TH1 * hPlabSecTofHitGenEff[kuNbFiles][kiNbPart];
   TH1 * hPlabSecTofPntEffSts[kuNbFiles][kiNbPart];
   TH1 * hPlabSecTofHitEffSts[kuNbFiles][kiNbPart];
      // Physics coord mapping, 1 per particle type
         // PLab
   Int_t    iNbBinsPlab = 100;
   Double_t dMinPlab    =   0.0;
   Double_t dMaxPlab    =  10.0;    
   
   TH1 * tempOneDimHist = NULL;
   TH2 * tempTwoDimHist = NULL;
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   {
      tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
            Form("TofTests_IntegratedHitPntEff") ) );
      if( NULL != tempOneDimHist )
         hIntegratedPntHitEff[uFile] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_IntegratedHitPntEff", sTagFiles[uFile].Data()) ) );
         else return kFALSE;
    
      tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
            Form("TofTests_IntegratedHitPntEffPrim") ) );
      if( NULL != tempOneDimHist )
         hIntegratedPntHitEffPrim[uFile] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_IntegratedHitPntEffPrim", sTagFiles[uFile].Data()) ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
            Form("TofTests_IntegratedHitPntEffSec") ) );
      if( NULL != tempOneDimHist )
         hIntegratedPntHitEffSec[uFile] = (TH1*)(tempOneDimHist->Clone( 
               Form("%s_IntegratedHitPntEffSec", sTagFiles[uFile].Data()) ) );
         else return kFALSE;
              
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

         tempTwoDimHist = (TH2*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_MultiPntHitBestDelta%s",  ksDimName[uDim].Data()) ) );
         if( NULL != tempTwoDimHist )
            hMultiPntHitBestDelta[uFile][uDim] = (TH2*)(tempTwoDimHist->Clone( 
                  Form("%s_MultiPntHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
            else return kFALSE;
            
         hMultiPntHitClosestDelta_proj[uFile][uDim]  = hMultiPntHitClosestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitClosestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiPntHitFurthestDelta_proj[uFile][uDim] = hMultiPntHitFurthestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitFurthestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiPntHitMeanDelta_proj[uFile][uDim]     = hMultiPntHitMeanDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitMeanDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
         hMultiPntHitBestDelta_proj[uFile][uDim]     = hMultiPntHitBestDelta[uFile][uDim]->ProjectionX(
               Form("%s_MultiPntHitBestDelta%s_proj", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) );
               
         hAllPntHitClosestDelta[uFile][uDim]  = (TH1*)(hMultiPntHitClosestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitClosestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitFurthestDelta[uFile][uDim] = (TH1*)(hMultiPntHitFurthestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitFurthestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitMeanDelta[uFile][uDim]     = (TH1*)(hMultiPntHitMeanDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitMeanDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
         hAllPntHitBestDelta[uFile][uDim]     = (TH1*)(hMultiPntHitBestDelta_proj[uFile][uDim]->Clone( 
                  Form("%s_AllPntHitBestDelta%s", sTagFiles[uFile].Data(), ksDimName[uDim].Data()) ) );
               
         hAllPntHitClosestDelta[uFile][uDim] ->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllPntHitFurthestDelta[uFile][uDim]->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllPntHitMeanDelta[uFile][uDim]    ->Add( hSinglePointHitDelta[uFile][uDim] );
         hAllPntHitBestDelta[uFile][uDim]    ->Add( hSinglePointHitDelta[uFile][uDim] );
      } // Loop on dimensions
      
      for(UInt_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
      {
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabGenTrk_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabGenTrk[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabGenTrk_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
   
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabStsPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabStsPnt[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabStsPnt_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
   
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabGenTrkTofPnt[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabGenTrkTofPnt_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
   
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabGenTrkTofHit_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabGenTrkTofHit[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabGenTrkTofHit_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
            
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabStsTrkTofPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabStsTrkTofPnt[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabStsTrkTofPnt_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
   
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabStsTrkTofHit_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabStsTrkTofHit[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabStsTrkTofHit_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
   
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabSecGenTrk_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabSecGenTrk[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabSecGenTrk_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
   
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabSecStsPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabSecStsPnt[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabSecStsPnt_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
   
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabSecGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabSecGenTrkTofPnt[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabSecGenTrkTofPnt_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
   
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabSecGenTrkTofHit_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabSecGenTrkTofHit[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabSecGenTrkTofHit_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;  
         
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabSecStsTrkTofPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabSecStsTrkTofPnt[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabSecStsTrkTofPnt_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
   
         tempOneDimHist = (TH1*) (filePnt[uFile]->FindObjectAny( 
               Form("TofTests_PlabSecStsTrkTofHit_%s", ksPartTag[iPartIdx].Data() ) ) );
         if( NULL != tempOneDimHist )
            hPlabSecStsTrkTofHit[uFile][iPartIdx] = (TH1*)(tempOneDimHist->Clone( 
                  Form("%s_PlabSecStsTrkTofHit_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data()) ) );
            else return kFALSE;
            
         hPlabTofPntEff[uFile][iPartIdx] = new TH1D( Form("%s_PlabTofPntEff_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency VS Generator for MC Primary Track with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF/GEN) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         hPlabTofHitEff[uFile][iPartIdx] = new TH1D( Form("%s_PlabTofHitEff_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency VS Generator for MC Primary Track with TOF hits, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/GEN) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         hPlabTofHitGenEff[uFile][iPartIdx] = new TH1D( Form("%s_PlabTofHitGenEff_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency for MC Primary Track with TOF hits VS same with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/TOF Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         hPlabTofPntEffSts[uFile][iPartIdx] = new TH1D( Form("%s_PlabTofPntEffSts_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency VS Trk w/ 4+ STS Pnt for MC Primary Track with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF/STS Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         hPlabTofHitEffSts[uFile][iPartIdx] = new TH1D( Form("%s_PlabTofHitEffSts_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency VS Trk w/ 4+ STS Pnt for MC Primary Track with TOF hits, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/STS Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         hPlabSecTofPntEff[uFile][iPartIdx] = new TH1D( Form("%s_PlabSecTofPntEff_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency VS Generator for MC Secondary Track with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF/GEN) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         hPlabSecTofHitEff[uFile][iPartIdx] = new TH1D( Form("%s_PlabSecTofHitEff_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency VS Generator for MC Secondary Track with TOF hits, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/GEN) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         hPlabSecTofHitGenEff[uFile][iPartIdx] = new TH1D( Form("%s_PlabSecTofHitGenEff_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency for MC Secondary Track with TOF hits VS same with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/TOF Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         hPlabSecTofPntEffSts[uFile][iPartIdx] = new TH1D( Form("%s_PlabSecTofPntEffSts_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency VS Trk w/ 4+ STS Pnt for MC Secondary Track with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF/STS Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         hPlabSecTofHitEffSts[uFile][iPartIdx] = new TH1D( Form("%s_PlabSecTofHitEffSts_%s", sTagFiles[uFile].Data(), ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency VS Trk w/ 4+ STS Pnt for MC Secondary Track with TOF hits, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/STS Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
                              
         hPlabTofPntEff[uFile][iPartIdx]      ->Divide( hPlabGenTrkTofPnt[uFile][iPartIdx],
                                                        hPlabGenTrk[uFile][iPartIdx],
                                                        100.0 );
         hPlabTofHitEff[uFile][iPartIdx]      ->Divide( hPlabGenTrkTofHit[uFile][iPartIdx],
                                                        hPlabGenTrk[uFile][iPartIdx],
                                                        100.0 );
         hPlabTofHitGenEff[uFile][iPartIdx]   ->Divide( hPlabGenTrkTofHit[uFile][iPartIdx],
                                                        hPlabGenTrkTofPnt[uFile][iPartIdx],
                                                        100.0 );
         hPlabTofPntEffSts[uFile][iPartIdx]   ->Divide( hPlabStsTrkTofPnt[uFile][iPartIdx],
                                                        hPlabStsPnt[uFile][iPartIdx],
                                                        100.0 );
         hPlabTofHitEffSts[uFile][iPartIdx]   ->Divide( hPlabStsTrkTofHit[uFile][iPartIdx],
                                                        hPlabStsPnt[uFile][iPartIdx],
                                                        100.0 );
         hPlabSecTofPntEff[uFile][iPartIdx]   ->Divide( hPlabSecGenTrkTofPnt[uFile][iPartIdx],
                                                        hPlabSecGenTrk[uFile][iPartIdx],
                                                        100.0 );
         hPlabSecTofHitEff[uFile][iPartIdx]   ->Divide( hPlabSecGenTrkTofHit[uFile][iPartIdx],
                                                        hPlabSecGenTrk[uFile][iPartIdx],
                                                        100.0 );
         hPlabSecTofHitGenEff[uFile][iPartIdx]->Divide( hPlabSecGenTrkTofHit[uFile][iPartIdx],
                                                        hPlabSecGenTrkTofPnt[uFile][iPartIdx],
                                                        100.0 );
         hPlabSecTofPntEffSts[uFile][iPartIdx]->Divide( hPlabSecStsTrkTofPnt[uFile][iPartIdx],
                                                        hPlabSecStsPnt[uFile][iPartIdx],
                                                        100.0 );
         hPlabSecTofHitEffSts[uFile][iPartIdx]->Divide( hPlabSecStsTrkTofHit[uFile][iPartIdx],
                                                        hPlabSecStsPnt[uFile][iPartIdx],
                                                        100.0 );
      } // Loop on particle species
       
   } // Loop on files
   
   // Display SinglePnt, Multi Pnts, Single Trk and Multi Trk histos
   THStack * histosStackAll[kuNbDim];
   THStack * histosStackAllEff;
   TLegend * legAll[kuNbDim];
   TLegend * legAllEff;
   
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

      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {          
 //        if( kTRUE == sTagFiles[uFile].Contains( "HitProd" ) )
         {
            hAllPntHitMeanDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
            hAllPntHitMeanDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
            histosStackAll[uDim]->Add( hAllPntHitMeanDelta[uFile][uDim] );
            legAll[uDim]->AddEntry( hAllPntHitMeanDelta[uFile][uDim] , 
                                    Form("%s, Mean", sTagFiles[uFile].Data()), "l");
         } // if( kTRUE == sTagFiles[uFile].Contains( "HitProd" ) )
         /*
            else
            {
               hAllPntHitBestDelta[uFile][uDim]->SetLineColor( kcFileColor[uFile] );
               hAllPntHitBestDelta[uFile][uDim]->SetLineWidth( kiLineWidth );
               histosStackAll[uDim]->Add( hAllPntHitBestDelta[uFile][uDim] );
               legAll[uDim]->AddEntry( hAllPntHitBestDelta[uFile][uDim] , 
                                       Form("%s, Best", sTagFiles[uFile].Data()), "l");
            } // else of if( kTRUE == sTagFiles[uFile].Contains( "HitProd" ) )
            */
      } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      
      canvAll->cd(1 + uDim);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      histosStackAll[uDim]->Draw("nostack");
      if( 2 == uDim )
         legAll[uDim]->Draw();
   } // for(UInt_t uDim = 0; uDim < kuNbDim; uDim++)
   
   // Efficiency
   
   histosStackAllEff = new THStack( "histosStackAllEff",
         "Efficiency of TOF hit generation, all tracks; Eff = Nb_{Trk w/ Hit}/Nb_{Trk w/ Pnt} [\%]; # [Events]");
   legAllEff = new TLegend(0.55,0.55,0.9,0.9);  
   legAllEff->SetHeader( Form("Pnt->Hit eff., all tracks" ) );
   legAllEff->SetTextSize( kdLegTextSize );

   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   {          
      hIntegratedPntHitEff[uFile]->SetLineColor( kcFileColor[uFile] );
      hIntegratedPntHitEff[uFile]->SetLineWidth( kiLineWidth );
      histosStackAllEff->Add( hIntegratedPntHitEff[uFile] );
      legAllEff->AddEntry( hIntegratedPntHitEff[uFile] , 
                              Form("%s", sTagFiles[uFile].Data()), "l");
   } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   
   canvAll->cd(1 + kuNbDim);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   histosStackAllEff->Draw("nostack");
   
   if( kTRUE == bSavePlots )
   {
      canvAll->SaveAs( Form( "plots/tofGeoComp_ResidualsAll_%s.png", sOutFileTag.Data() ) );
      canvAll->SaveAs( Form( "plots/tofGeoComp_ResidualsAll_%s.pdf", sOutFileTag.Data() ) );
   } // if( kTRUE == bSavePlots )

   TCanvas *canvEffPntPrim = new TCanvas( "canvEffPntPrim", 
         "Efficiency up to TOF Points, for different species, geometries comparison",
         1920, 986 );
   canvEffPntPrim->cd();
   canvEffPntPrim->Divide(3,3);
   
   TCanvas *canvEffHitPrim = new TCanvas( "canvEffHitPrim", 
         "Efficiency up to TOF Hits, for different species, geometries comparison",
         1920, 986 );
   canvEffHitPrim->cd();
   canvEffHitPrim->Divide(3,3);
   
   TCanvas *canvEffHitGenPrim = new TCanvas( "canvEffHitGenPrim", 
         "Efficiency up to TOF Hits generation, for different species, geometries comparison",
         1920, 986 );
   canvEffHitGenPrim->cd();
   canvEffHitGenPrim->Divide(3,3);

   TCanvas *canvEffPntPrimSts = new TCanvas( "canvEffPntPrimSts", 
         "Efficiency from STS point to TOF Points, for different species, geometries comparison",
         1920, 986 );
   canvEffPntPrimSts->cd();
   canvEffPntPrimSts->Divide(3,3);
   
   TCanvas *canvEffHitPrimSts = new TCanvas( "canvEffHitPrimSts", 
         "Efficiency from STS point to TOF Hits, for different species, geometries comparison",
         1920, 986 );
   canvEffHitPrimSts->cd();
   canvEffHitPrimSts->Divide(3,3);

   TCanvas *canvEffPntSec = new TCanvas( "canvEffPntSec", 
         "Efficiency up to TOF Points, for different species, geometries comparison",
         1920, 986 );
   canvEffPntSec->cd();
   canvEffPntSec->Divide(3,3);
   
   TCanvas *canvEffHitSec = new TCanvas( "canvEffHitSec", 
         "Efficiency up to TOF Hits, for different species, geometries comparison",
         1920, 986 );
   canvEffHitSec->cd();
   canvEffHitSec->Divide(3,3);
   
   TCanvas *canvEffHitGenSec = new TCanvas( "canvEffHitGenSec", 
         "Efficiency up to TOF Hits generation, for different species, geometries comparison",
         1920, 986 );
   canvEffHitGenSec->cd();
   canvEffHitGenSec->Divide(3,3);

   TCanvas *canvEffPntSecSts = new TCanvas( "canvEffPntSecSts", 
         "Efficiency from STS point to TOF Points, for different species, geometries comparison",
         1920, 986 );
   canvEffPntSecSts->cd();
   canvEffPntSecSts->Divide(3,3);
   
   TCanvas *canvEffHitSecSts = new TCanvas( "canvEffHitSecSts", 
         "Efficiency from STS point to TOF Hits, for different species, geometries comparison",
         1920, 986 );
   canvEffHitSecSts->cd();
   canvEffHitSecSts->Divide(3,3);
   
   THStack * histosStackEffPntPrim[kiNbPart];
   THStack * histosStackEffHitPrim[kiNbPart];
   THStack * histosStackEffHitGenPrim[kiNbPart];
   THStack * histosStackEffPntPrimSts[kiNbPart];
   THStack * histosStackEffHitPrimSts[kiNbPart];
   
   THStack * histosStackEffPntSec[kiNbPart];
   THStack * histosStackEffHitSec[kiNbPart];
   THStack * histosStackEffHitGenSec[kiNbPart];
   THStack * histosStackEffPntSecSts[kiNbPart];
   THStack * histosStackEffHitSecSts[kiNbPart];
   
   for(UInt_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
   {
      histosStackEffPntPrim[iPartIdx] = new THStack( Form("histosStackEffPntPrim_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency VS Generator for MC Primary Track with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF/GEN) [\%]", 
                                   ksPartName[iPartIdx].Data()  ) );
      histosStackEffHitPrim[iPartIdx] = new THStack( Form("histosStackEffHitPrim_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency VS Generator for MC Primary Track with TOF hits, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/GEN) [\%]",
                                   ksPartName[iPartIdx].Data()  ) );
      histosStackEffHitGenPrim[iPartIdx] = new THStack( Form("histosStackEffHitGenPrim_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency for MC Primary Track with TOF hits VS same with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/TOF Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ) );
      histosStackEffPntPrimSts[iPartIdx] = new THStack( Form("histosStackEffPntPrimSts_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency VS Trk w/ 4+ STS Pnt for MC Primary Track with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF/STS Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ) );
      histosStackEffHitPrimSts[iPartIdx] = new THStack( Form("histosStackEffHitPrimSts_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency VS Trk w/ 4+ STS Pnt for MC Primary Track with TOF hits, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/STS Pnt) [\%]",
                                   ksPartName[iPartIdx].Data()  ) );
                                   
      histosStackEffPntSec[iPartIdx] = new THStack( Form("histosStackEffPntSec_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency VS Generator for MC Secondary Track with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF/GEN) [\%]", 
                                   ksPartName[iPartIdx].Data()  ) );
      histosStackEffHitSec[iPartIdx] = new THStack( Form("histosStackEffHitSec_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency VS Generator for MC Secondary Track with TOF hits, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/GEN) [\%]",
                                   ksPartName[iPartIdx].Data()  ) );
      histosStackEffHitGenSec[iPartIdx] = new THStack( Form("histosStackEffHitGenSec_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency for MC Secondary Track with TOF hits VS same with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/TOF Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ) );
      histosStackEffPntSecSts[iPartIdx] = new THStack( Form("histosStackEffPntSecSts_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency VS Trk w/ 4+ STS Pnt for MC Secondary Track with TOF points, %s; P_{lab} [GeV/c]; Eff. (TOF/STS Pnt) [\%]", 
                                   ksPartName[iPartIdx].Data()  ) );
      histosStackEffHitSecSts[iPartIdx] = new THStack( Form("histosStackEffHitSecSts_%s", ksPartTag[iPartIdx].Data() ),
          Form("P_{lab} efficiency VS Trk w/ 4+ STS Pnt for MC Secondary Track with TOF hits, %s; P_{lab} [GeV/c]; Eff. (TOF Hit/STS Pnt) [\%]",
                                   ksPartName[iPartIdx].Data()  ) );
                                   
      for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      {
         hPlabTofPntEff[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabTofPntEff[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffPntPrim[iPartIdx]->Add( hPlabTofPntEff[uFile][iPartIdx] );
         
         hPlabTofHitEff[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabTofHitEff[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffHitPrim[iPartIdx]->Add( hPlabTofHitEff[uFile][iPartIdx] );
         
         hPlabTofHitGenEff[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabTofHitGenEff[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffHitGenPrim[iPartIdx]->Add( hPlabTofHitGenEff[uFile][iPartIdx] );
         
         hPlabTofPntEffSts[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabTofPntEffSts[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffPntPrimSts[iPartIdx]->Add( hPlabTofPntEffSts[uFile][iPartIdx] );
         
         hPlabTofHitEffSts[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabTofHitEffSts[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffHitPrimSts[iPartIdx]->Add( hPlabTofHitEffSts[uFile][iPartIdx] );
         
         hPlabSecTofPntEff[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabSecTofPntEff[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffPntSec[iPartIdx]->Add( hPlabSecTofPntEff[uFile][iPartIdx] );
         
         hPlabSecTofHitEff[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabSecTofHitEff[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffHitSec[iPartIdx]->Add( hPlabSecTofHitEff[uFile][iPartIdx] );
         
         hPlabSecTofHitGenEff[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabSecTofHitGenEff[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffHitGenSec[iPartIdx]->Add( hPlabSecTofHitGenEff[uFile][iPartIdx] );
         
         hPlabSecTofPntEffSts[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabSecTofPntEffSts[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffPntSecSts[iPartIdx]->Add( hPlabSecTofPntEffSts[uFile][iPartIdx] );
         
         hPlabSecTofHitEffSts[uFile][iPartIdx]->SetLineColor( kcFileColor[uFile] );
         hPlabSecTofHitEffSts[uFile][iPartIdx]->SetLineWidth( kiLineWidth );
         histosStackEffHitSecSts[iPartIdx]->Add( hPlabSecTofHitEffSts[uFile][iPartIdx] );
      } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
      
      canvEffPntPrim->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffPntPrim[iPartIdx]->Draw("nostack");
      
      canvEffHitPrim->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffHitPrim[iPartIdx]->Draw("nostack");
      
      canvEffHitGenPrim->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffHitGenPrim[iPartIdx]->Draw("nostack");
      
      canvEffPntPrimSts->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffPntPrimSts[iPartIdx]->Draw("nostack");
      
      canvEffHitPrimSts->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffHitPrimSts[iPartIdx]->Draw("nostack");
      
      canvEffPntSec->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffPntSec[iPartIdx]->Draw("nostack");
      
      canvEffHitSec->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffHitSec[iPartIdx]->Draw("nostack");
      
      canvEffHitGenSec->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffHitGenSec[iPartIdx]->Draw("nostack");
      
      canvEffPntSecSts->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffPntSecSts[iPartIdx]->Draw("nostack");
      
      canvEffHitSecSts->cd(1 + iPartIdx);
      gPad->SetGridx();
      gPad->SetGridy();
      histosStackEffHitSecSts[iPartIdx]->Draw("nostack");
   } // for(UInt_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
   
   if( kTRUE == bSavePlots )
   {
      canvEffPntPrim   ->SaveAs( Form( "plots/tofGeoComp_EffPntPrim_%s.png", sOutFileTag.Data() ) );
      canvEffHitPrim   ->SaveAs( Form( "plots/tofGeoComp_EffHitPrim_%s.png", sOutFileTag.Data() ) );
      canvEffHitGenPrim->SaveAs( Form( "plots/tofGeoComp_EffHitGenPrim_%s.png", sOutFileTag.Data() ) );
      canvEffPntPrimSts->SaveAs( Form( "plots/tofGeoComp_EffPntPrimSts_%s.png", sOutFileTag.Data() ) );
      canvEffHitPrimSts->SaveAs( Form( "plots/tofGeoComp_EffHitPrimSts_%s.png", sOutFileTag.Data() ) );
      canvEffPntSec    ->SaveAs( Form( "plots/tofGeoComp_EffPntSec_%s.png", sOutFileTag.Data() ) );
      canvEffHitSec    ->SaveAs( Form( "plots/tofGeoComp_EffHitSec_%s.png", sOutFileTag.Data() ) );
      canvEffHitGenSec ->SaveAs( Form( "plots/tofGeoComp_EffHitGenSec_%s.png", sOutFileTag.Data() ) );
      canvEffPntSecSts ->SaveAs( Form( "plots/tofGeoComp_EffPntSecSts_%s.png", sOutFileTag.Data() ) );
      canvEffHitSecSts ->SaveAs( Form( "plots/tofGeoComp_EffHitSecSts_%s.png", sOutFileTag.Data() ) );
      
      canvEffPntPrim   ->SaveAs( Form( "plots/tofGeoComp_EffPntPrim_%s.pdf", sOutFileTag.Data() ) );
      canvEffHitPrim   ->SaveAs( Form( "plots/tofGeoComp_EffHitPrim_%s.pdf", sOutFileTag.Data() ) );
      canvEffHitGenPrim->SaveAs( Form( "plots/tofGeoComp_EffHitGenPrim_%s.pdf", sOutFileTag.Data() ) );
      canvEffPntPrimSts->SaveAs( Form( "plots/tofGeoComp_EffPntPrimSts_%s.pdf", sOutFileTag.Data() ) );
      canvEffHitPrimSts->SaveAs( Form( "plots/tofGeoComp_EffHitPrimSts_%s.pdf", sOutFileTag.Data() ) );
      canvEffPntSec    ->SaveAs( Form( "plots/tofGeoComp_EffPntSec_%s.pdf", sOutFileTag.Data() ) );
      canvEffHitSec    ->SaveAs( Form( "plots/tofGeoComp_EffHitSec_%s.pdf", sOutFileTag.Data() ) );
      canvEffHitGenSec ->SaveAs( Form( "plots/tofGeoComp_EffHitGenSec_%s.pdf", sOutFileTag.Data() ) );
      canvEffPntSecSts ->SaveAs( Form( "plots/tofGeoComp_EffPntSecSts_%s.pdf", sOutFileTag.Data() ) );
      canvEffHitSecSts ->SaveAs( Form( "plots/tofGeoComp_EffHitSecSts_%s.pdf", sOutFileTag.Data() ) );
   } // if( kTRUE == bSavePlots )

   return kTRUE;
}
                       
