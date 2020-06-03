
const UInt_t   kuNbFiles = 4;
const UInt_t   kuNbDim   = 5;
const TString  ksDimName[kuNbDim] = { "X", "Y", "Z", "R", "T" }; 
const Color_t	kcFileColor[kuNbFiles] = { kBlue, kRed, kGreen, kOrange };
const Int_t    kiLineWidth    = 2;
const Double_t kdLegTextSize  = 0.04;
const Double_t kdPadTitleSize = 0.1;

Bool_t valHpDigiComp( TString sFilenameA = "data/tofqaHP.cbm100_pbar_auau.25gev.centr._qa.hst.all.root",
                      TString sFilenameB = "data/tofqa.cbm100_pbar_auau.25gev.centr._qa.hst.all.root",
                      TString sFilenameC = "data/tofqaHP.cbm100_pbar_auau.25gev.centr.noTRD_qa.hst.all.root",
                      TString sFilenameD = "data/tofqa.cbm100_pbar_auau.25gev.centr.noTRD_qa.hst.all.root",
                      TString sTagFilesA = "HitProd",       TString sTagFilesB = "DigiClust",
                      TString sTagFilesC = "HitProd_noTrd", TString sTagFilesD = "DigiClust_noTrd",
                      TString sOutFileTag = "_25gev" 
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
      } // Loop on files 
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
      hIntegratedPntHitEffPrim[uFile]->SetLineColor( kcFileColor[uFile] );
      hIntegratedPntHitEffPrim[uFile]->SetLineWidth( kiLineWidth );
      histosStackAllEff->Add( hIntegratedPntHitEffPrim[uFile] );
      legAllEff->AddEntry( hIntegratedPntHitEffPrim[uFile] , 
                              Form("%s", sTagFiles[uFile].Data()), "l");
   } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++)
   
   canvAll->cd(1 + kuNbDim);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   histosStackAllEff->Draw("nostack");
   
//   canvAll->SaveAs( Form( "HProdDigi_All_%s.png", sOutFileTag.Data() ) );
//   canvAll->SaveAs( Form( "HProdDigi_All_%s.pdf", sOutFileTag.Data() ) );
   
   return kTRUE;
}
                       
