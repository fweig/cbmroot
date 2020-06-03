void save_canvas_to_file(TFile *outf, TCanvas *canv, TString runCase) {
   outf->cd();
   gDirectory->mkdir(runCase);
   gDirectory->cd(runCase);
   canv->Write();
   gDirectory->cd("..");

}

void save_canvas_to_pdf(TCanvas *canv,
                        TString runCase){
   canv->SaveAs(TString("canvasesLimits2elinks/") + runCase + TString(canv->GetName()) + TString(".pdf") );
}

UInt_t uScanCaseOffset = 9;
const UInt_t kuMaxNbFiredChan = 15 + 1;
const UInt_t kuMaxNbTotChan   = 16 + 1;
Double_t dDummyStart =  1e5;
Double_t dDummyStop  = -1e5;
Double_t dNoiseBlockStart[kuMaxNbFiredChan][kuMaxNbTotChan];
Double_t dNoiseBlockStop [kuMaxNbFiredChan][kuMaxNbTotChan];
Double_t dMainBlockStart [kuMaxNbFiredChan][kuMaxNbTotChan];
Double_t dMainBlockStop  [kuMaxNbFiredChan][kuMaxNbTotChan];

Bool_t analyseTimeCoincidenceBugCase(const UInt_t kuNbFiles,
                                    const UInt_t kuNbStats,
                                    TString *sFilename,
                                    UInt_t *uNbChanFixed,
                                    UInt_t *uNbChanScan,
                                    TString *sStats,
                                    TString runCase,
                                    TFile *outf) {
   TFile *pFile[ kuNbFiles ];

   /// Scaling function, the constant should lead to a ratio of ~1 for the 1st bins
   Double_t hitsExpected = 1/500.0; // per 1/2s bin

   /// Declaration of plotting variable
   TH2D * phNbFiredChanPerMs[ kuNbFiles ];

   /// Delay scan parameters
   Double_t dDistMin = -1000.0;
   Double_t dDistMax =  1000.0;
   Double_t dDistStep =    4.0;
   UInt_t   uNbBinsDist = (dDistMax - dDistMin) / dDistStep;
   Double_t dDistStepDuration = 2.0; /// second
   Double_t dDistStepPulses = dDistStepDuration * 1000.0; /// 1 kHz pulser


   /// canvas division
   Int_t canvCols = 4;
   Int_t canvRows = 2;

   /// Loop on scan cases
   TH2 * tempTwoDimHist = NULL;
   for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++ )
   {
      pFile[uFile] = TFile::Open( sFilename[uFile] );
      gROOT->cd();

      UInt_t uNbChanTot = uNbChanFixed[uFile] + uNbChanScan[uFile];

      TH1D * phNbFiredChanProj[ uNbChanTot - uScanCaseOffset + 1 ];
      TH1D * phNbFiredChanScan[ uNbChanTot - uScanCaseOffset + 1 ];
      TCanvas * cNbFiredProj = new TCanvas( Form("cNbFiredProj_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
                                      "Numbers of fired Channels projections for each number of channels" );
      cNbFiredProj->Divide( canvCols, canvRows );

      TCanvas * cNbFiredProjStack = new TCanvas( Form("cNbFiredProjStack_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
                                      "Numbers of fired Channels projections for each number of channels" );

      THStack * stackFiredProj = new THStack( "stackFiredProj",
                                              "Stack of the projections" );

      /// Recover the NbFiredChan and NbMultiChan 2D plots
      tempTwoDimHist = (TH2*) (pFile[uFile]->FindObjectAny("hNbFiredChanPerMs00"));
      if (NULL != tempTwoDimHist) {
         phNbFiredChanPerMs[uFile] = (TH2D*) (tempTwoDimHist->Clone(
                     Form("phNbFiredChanPerMs_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]) ) );
      } else {
         return kFALSE;
      }

      for (UInt_t uNbChan = uScanCaseOffset; uNbChan <= uNbChanTot; uNbChan++){
         phNbFiredChanProj[uNbChan - uScanCaseOffset] = phNbFiredChanPerMs[uFile]->ProjectionX(
                                          Form( "hNbFiredChanProj_%u_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan ),
                                          uNbChan, uNbChan );
         phNbFiredChanScan[uNbChan - uScanCaseOffset] = new TH1D( Form( "hNbFiredChanScan_%u_%u_%u",
                                                          uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan ),
                                                    Form( "Number of pulses with %u fired channels instead of %u for the %u-%u case",
                                                          uNbChan, uNbChanTot, uNbChanFixed[uFile], uNbChanScan[uFile]),
                                                    uNbBinsDist, dDistMin, dDistMax );

         /// Loop on the time in run bins (1/2s stepping) with check on the corresponding delay stepping
         for( UInt_t uBin = 1;
              uBin <= phNbFiredChanProj[uNbChan - uScanCaseOffset]->GetNbinsX() &&
              dDistMin + dDistStep * phNbFiredChanProj[uNbChan - uScanCaseOffset]->GetBinCenter( uBin ) / dDistStepDuration
                   <= dDistMax;
              ++ uBin )
         {
            /// Delay corresponding to the bin
            Double_t dDist = dDistMin + dDistStep * phNbFiredChanProj[uNbChan - uScanCaseOffset]->GetBinCenter( uBin )
                                                  / dDistStepDuration;

            phNbFiredChanScan[uNbChan - uScanCaseOffset]->Fill( dDist,
                                                  phNbFiredChanProj[uNbChan - uScanCaseOffset]->GetBinContent( uBin )
                                                  / dDistStepPulses );
         } // loop on bins until end or out of coincidence window

         /// Loop on the time delay bins and check if we are starting to see the wrong number of channels
         if( uNbChan < uNbChanTot )
         {
            std::vector< Double_t > dDelayStartNoise;
            std::vector< Double_t > dDelayStopNoise;
            std::vector< Double_t > dDelayStartMain;
            std::vector< Double_t > dDelayStopMain;
            Bool_t   bStartFlagNoise = kFALSE;
            Bool_t   bStartFlagMain  = kFALSE;
            Bool_t   bStopFlagNoise = kTRUE;
            Bool_t   bStopFlagMain  = kTRUE;
            Double_t dCountRatioPreviousDelay = 0.0;
            for( UInt_t uBin = 1; uBin <= phNbFiredChanScan[uNbChan - uScanCaseOffset]->GetNbinsX(); ++ uBin )
            {
               /// Delay corresponding to the bin
               Double_t dDist = phNbFiredChanScan[uNbChan - uScanCaseOffset]->GetBinCenter( uBin );

               /// Fraction of pulses having this number of fired channels
               Double_t dCountRatio = phNbFiredChanScan[uNbChan - uScanCaseOffset]->GetBinContent( uBin );

               if( 1e-3 < dCountRatio && dCountRatioPreviousDelay <= 1e-3 && bStopFlagNoise )
               {
                  bStartFlagNoise = kTRUE;
                  bStopFlagNoise  = kFALSE;
                  dDelayStartNoise.push_back( dDist );
               } // if( 1e-3 < dCountRatio && dCountRatioPreviousDelay <= 1e-3 )
               else if( 1e-3 > dCountRatio && dCountRatioPreviousDelay >= 1e-3  && bStartFlagNoise )
               {
                  bStartFlagNoise = kFALSE;
                  bStopFlagNoise  = kTRUE;
                  dDelayStopNoise.push_back( dDist );
               } // if( 1e-3 > dCountRatio && dCountRatioPreviousDelay >= 1e-3 )

               if( 1e-2 < dCountRatio && dCountRatioPreviousDelay <= 1e-2 && bStopFlagMain )
               {
                  bStartFlagMain = kTRUE;
                  bStopFlagMain  = kFALSE;
                  dDelayStartMain.push_back( dDist );
               } // if( 1e-2 < dCountRatio && dCountRatioPreviousDelay <= 1e-2 )
               else if( 1e-2 > dCountRatio && dCountRatioPreviousDelay >= 1e-2  && bStartFlagMain )
               {
                  bStartFlagMain = kFALSE;
                  bStopFlagMain  = kTRUE;
                  dDelayStopMain.push_back( dDist );
               } // if( 1e-2 > dCountRatio && dCountRatioPreviousDelay >= 1e-2 )
/*
               if( 0.0 == dCountRatio )
               {
                  bStartFlagNoise = kFALSE;
                  bStartFlagMain  = kFALSE;
                  bStopFlagNoise  = kTRUE;
                  bStopFlagMain   = kTRUE;
               } // if( 0.0 == dCountRatio )
*/
               dCountRatioPreviousDelay = dCountRatio;
            } // loop on bins until end

            if( dDelayStartNoise.size() != dDelayStopNoise.size() )
            {
               std::cout << Form( "Case %2u-%2u fired %2u ", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan )
                         << " Unmatched Start-Stop for noise! "
                         << dDelayStartNoise.size() << " " << dDelayStopNoise.size()
                         << std::endl;
            } // if( dDelayStartNoise.size() != dDelayStopNoise.size() )

            std::cout << Form( "Case %2u-%2u fired %2u noise blocks: ", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan );
            for( UInt_t uNoiseBlock = 0;
                 uNoiseBlock < dDelayStartNoise.size() && uNoiseBlock < dDelayStopNoise.size();
                 ++ uNoiseBlock)
            {
               std::cout << Form( "%4.0f to % 4.0f, ", dDelayStartNoise[uNoiseBlock], dDelayStopNoise[uNoiseBlock] );

               if( dDelayStartNoise[uNoiseBlock] < dNoiseBlockStart[uNbChan][uNbChanTot] )
                  dNoiseBlockStart[uNbChan][uNbChanTot] = dDelayStartNoise[uNoiseBlock];
               if( dNoiseBlockStop [uNbChan][uNbChanTot] < dDelayStopNoise[uNoiseBlock] )
                  dNoiseBlockStop [uNbChan][uNbChanTot] = dDelayStopNoise[uNoiseBlock];
            } // Loop on noise blocks
            std::cout << std::endl;

            if( dDelayStartMain.size() != dDelayStopMain.size() )
            {
               std::cout << Form( "Case %2u-%2u fired %2u ", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan )
                         << " Unmatched Start-Stop for main! "
                         << dDelayStartMain.size() << " " << dDelayStopMain.size()
                         << std::endl;
            } // if( dDelayStartMain.size() != dDelayStopMain.size() )

            std::cout << Form( "Case %2u-%2u fired %2u main blocks:  ", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan );
            for( UInt_t uMainBlock = 0;
                 uMainBlock < dDelayStartMain.size() && uMainBlock < dDelayStopMain.size();
                 ++ uMainBlock)
            {
               std::cout << Form( "%4.0f to % 4.0f, ", dDelayStartMain[uMainBlock], dDelayStopMain[uMainBlock] );

               if( dDelayStartMain[uMainBlock] < dMainBlockStart[uNbChan][uNbChanTot] )
                  dMainBlockStart[uNbChan][uNbChanTot] = dDelayStartMain[uMainBlock];
               if( dMainBlockStop [uNbChan][uNbChanTot] < dDelayStopMain[uMainBlock] )
                  dMainBlockStop [uNbChan][uNbChanTot] = dDelayStopMain[uMainBlock];
            } // Loop on noise blocks
            std::cout << std::endl;

         } // if( uNbChan < uNbChanTot )

         cNbFiredProj->cd(uNbChan -uScanCaseOffset +1);
         gPad->SetGridx();
         gPad->SetGridy();
//         phNbFiredChanProj[uNbChan - uScanCaseOffset]->Draw();
         phNbFiredChanScan[uNbChan - uScanCaseOffset]->Draw( "hist" );

//         stackFiredProj->Add( phNbFiredChanProj[uNbChan - uScanCaseOffset] );
         stackFiredProj->Add( phNbFiredChanScan[uNbChan - uScanCaseOffset] );
      } // Loop on missing channels cases

      cNbFiredProjStack->cd();
      gPad->SetGridx();
      gPad->SetGridy();
      stackFiredProj->Draw( "nostack,hist" );

      save_canvas_to_file(outf, cNbFiredProj, runCase);
      save_canvas_to_file(outf, cNbFiredProjStack, runCase);
      delete cNbFiredProj;
      delete cNbFiredProjStack;

      for (UInt_t uNbChan = uScanCaseOffset; uNbChan <= uNbChanTot; uNbChan++)
      {
         delete phNbFiredChanProj[uNbChan - uScanCaseOffset];
         delete phNbFiredChanScan[uNbChan - uScanCaseOffset];
      } // for (UInt_t uNbChan = uScanCaseOffset; uNbChan <= uNbChanTot; uNbChan++)
   } // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++ )

   return kTRUE;

}

// n + m = 9 cases
Bool_t AnalyseTimeCoincidenceBug09(TFile* outf)
{
   /// Data source parameters
   const UInt_t kuNbFiles = 8;
   const UInt_t kuNbStats = 3;
   TString sFilename[ kuNbFiles ] = {  "data/PulserHistos_r0052_20181009_1619.root",
                                       "data/PulserHistos_r0053_20181009_1636.root",
                                       "data/PulserHistos_r0054_20181009_1653.root",
                                       "data/PulserHistos_r0055_20181009_1710.root",
                                       "data/PulserHistos_r0056_20181009_1727.root",
                                       "data/PulserHistos_r0057_20181009_1744.root",
                                       "data/PulserHistos_r0058_20181009_1801.root",
                                       "data/PulserHistos_r0059_20181009_1818.root"
                                     };
   UInt_t uNbChanFixed[ kuNbFiles ] = { 8, 7, 6, 5, 4, 3, 2, 1 };
   UInt_t uNbChanScan[ kuNbFiles ]  = { 1, 2, 3, 4, 5, 6, 7, 8 };

   TString sStats[ kuNbStats ] = { "min", "mean", "max" };
   TString runCase = "9tot";
   return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase, outf);
}


// n + m = 10 cases
Bool_t AnalyseTimeCoincidenceBug10(TFile* outf)
{
   /// Data source parameters
   const UInt_t kuNbFiles = 7;
   const UInt_t kuNbStats = 3;
   TString sFilename[ kuNbFiles ] = {  "data/PulserHistos_r0060_20181009_1835.root",
                                       "data/PulserHistos_r0061_20181009_1852.root",
                                       "data/PulserHistos_r0062_20181009_1909.root",
                                       "data/PulserHistos_r0063_20181009_1926.root",
                                       "data/PulserHistos_r0064_20181009_1943.root",
                                       "data/PulserHistos_r0065_20181009_2000.root",
                                       "data/PulserHistos_r0066_20181009_2017.root"
                                     };
   UInt_t uNbChanFixed[ kuNbFiles ] = { 8, 7, 6, 5, 4, 3, 2 };
   UInt_t uNbChanScan[ kuNbFiles ]  = { 2, 3, 4, 5, 6, 7, 8 };

   TString sStats[ kuNbStats ] = { "min", "mean", "max" };
   TString runCase = "10tot";
   return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase, outf);
}

// n + m = 11 cases
Bool_t AnalyseTimeCoincidenceBug11(TFile* outf)
{
   /// Data source parameters
   const UInt_t kuNbFiles = 6;
   const UInt_t kuNbStats = 3;
   TString sFilename[ kuNbFiles ] = {  "data/PulserHistos_r0067_20181009_2034.root",
                                       "data/PulserHistos_r0068_20181009_2051.root",
                                       "data/PulserHistos_r0069_20181009_2108.root",
                                       "data/PulserHistos_r0070_20181009_2125.root",
                                       "data/PulserHistos_r0071_20181009_2142.root",
                                       "data/PulserHistos_r0072_20181009_2159.root"
                                     };
   UInt_t uNbChanFixed[ kuNbFiles ] = { 8, 7, 6, 5, 4, 3 };
   UInt_t uNbChanScan[ kuNbFiles ]  = { 3, 4, 5, 6, 7, 8 };

   TString sStats[ kuNbStats ] = { "min", "mean", "max" };
   TString runCase = "11tot";
   return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase, outf);
}

// n + m = 12 cases
Bool_t AnalyseTimeCoincidenceBug12(TFile* outf)
{
   /// Data source parameters
   const UInt_t kuNbFiles = 5;
   const UInt_t kuNbStats = 3;
   TString sFilename[ kuNbFiles ] = {  "data/PulserHistos_r0073_20181009_2216.root",
                                       "data/PulserHistos_r0074_20181009_2233.root",
                                       "data/PulserHistos_r0075_20181009_2250.root",
                                       "data/PulserHistos_r0076_20181009_2307.root",
                                       "data/PulserHistos_r0077_20181009_2324.root"
                                     };
   UInt_t uNbChanFixed[ kuNbFiles ] = { 8, 7, 6, 5, 4 };
   UInt_t uNbChanScan[ kuNbFiles ]  = { 4, 5, 6, 7, 8 };

   TString sStats[ kuNbStats ] = { "min", "mean", "max" };
   TString runCase = "12tot";
   return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase, outf);
}

// n + m = 13 cases
Bool_t AnalyseTimeCoincidenceBug13(TFile* outf)
{
   /// Data source parameters
   const UInt_t kuNbFiles = 4;
   const UInt_t kuNbStats = 3;
   TString sFilename[ kuNbFiles ] = {  "data/PulserHistos_r0078_20181009_2341.root",
                                       "data/PulserHistos_r0079_20181009_2358.root",
                                       "data/PulserHistos_r0080_20181010_0015.root",
                                       "data/PulserHistos_r0081_20181010_0032.root"
                                     };
   UInt_t uNbChanFixed[ kuNbFiles ] = { 8, 7, 6, 5 };
   UInt_t uNbChanScan[ kuNbFiles ]  = { 5, 6, 7, 8 };

   TString sStats[ kuNbStats ] = { "min", "mean", "max" };
   TString runCase = "13tot";
   return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase, outf);
}

// n + m = 14 cases
Bool_t AnalyseTimeCoincidenceBug14(TFile* outf)
{
   /// Data source parameters
   const UInt_t kuNbFiles = 3;
   const UInt_t kuNbStats = 3;
   TString sFilename[ kuNbFiles ] = {  "data/PulserHistos_r0082_20181010_0049.root",
                                       "data/PulserHistos_r0083_20181010_0106.root",
                                       "data/PulserHistos_r0084_20181010_0123.root"
                                     };
   UInt_t uNbChanFixed[ kuNbFiles ] = { 8, 7, 6 };
   UInt_t uNbChanScan[ kuNbFiles ]  = { 6, 7, 8 };

   TString sStats[ kuNbStats ] = { "min", "mean", "max" };
   TString runCase = "14tot";
   return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase, outf);
}

// n + m = 15 cases
Bool_t AnalyseTimeCoincidenceBug15(TFile* outf)
{
   /// Data source parameters
   const UInt_t kuNbFiles = 2;
   const UInt_t kuNbStats = 3;
   TString sFilename[ kuNbFiles ] = {  "data/PulserHistos_r0085_20181010_0140.root",
                                       "data/PulserHistos_r0086_20181010_0157.root"
                                     };
   UInt_t uNbChanFixed[ kuNbFiles ] = { 8, 7 };
   UInt_t uNbChanScan[ kuNbFiles ]  = { 7, 8 };

   TString sStats[ kuNbStats ] = { "min", "mean", "max" };
   TString runCase = "15tot";
   return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase, outf);
}

// n + m = 16 cases
Bool_t AnalyseTimeCoincidenceBug16(TFile* outf)
{
   /// Data source parameters
   const UInt_t kuNbFiles = 1;
   const UInt_t kuNbStats = 3;
   TString sFilename[ kuNbFiles ] = {  "data/PulserHistos_r0087_20181010_0214.root"
                                     };
   UInt_t uNbChanFixed[ kuNbFiles ] = { 8 };
   UInt_t uNbChanScan[ kuNbFiles ]  = { 8 };

   TString sStats[ kuNbStats ] = { "min", "mean", "max" };
   TString runCase = "16tot";
   return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase, outf);
}

Bool_t AnalyseTimeCoincidenceBugLimits2elinks( TString sFilename = "STSXyterPulserLimits_2Elinks.root" )
{
   /// Open output root file
   TDirectory* oldDir = gDirectory;
   TFile *  histoFile = new TFile( sFilename , "RECREATE");
   if( NULL == histoFile )
      return kFALSE;

   /// Prepare the arrays with the limits
   for( UInt_t uNbFiredChan = 0; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan )
   {
      for( UInt_t uNbTotChan = 0; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan )
      {
         dNoiseBlockStart[uNbFiredChan][uNbTotChan] = dDummyStart;
         dNoiseBlockStop [uNbFiredChan][uNbTotChan] = dDummyStop;
         dMainBlockStart [uNbFiredChan][uNbTotChan] = dDummyStart;
         dMainBlockStop  [uNbFiredChan][uNbTotChan] = dDummyStop;
      } // for( UInt_t uNbTotChan = 0; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan )
   } // for( UInt_t uNbFiredChan = 0; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan )

   /// Run all cases
   AnalyseTimeCoincidenceBug09(histoFile);
   AnalyseTimeCoincidenceBug10(histoFile);
   AnalyseTimeCoincidenceBug11(histoFile);
   AnalyseTimeCoincidenceBug12(histoFile);
   AnalyseTimeCoincidenceBug13(histoFile);
   AnalyseTimeCoincidenceBug14(histoFile);
   AnalyseTimeCoincidenceBug15(histoFile);
   AnalyseTimeCoincidenceBug16(histoFile);

   /// Printout limits for Main blocks
   std::cout << "Main blocks limits" << std::endl;
   std::cout << "          ";
   for( UInt_t uNbTotChan = uScanCaseOffset + 1; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan )
      std::cout <<  Form( "%2u channels ", uNbTotChan);
   std::cout << std::endl;
   for( UInt_t uNbFiredChan = uScanCaseOffset; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan )
   {
      std::cout << Form( "%2u fired: ", uNbFiredChan );
      for( UInt_t uNbTotChan = uScanCaseOffset + 1; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan )
      {
         if( dMainBlockStart[uNbFiredChan][uNbTotChan] < dDummyStart &&
             dDummyStop < dMainBlockStop [uNbFiredChan][uNbTotChan] )
             std::cout << Form("%4.0f - %4.0f ", dMainBlockStart[uNbFiredChan][uNbTotChan],
                                                 dMainBlockStop [uNbFiredChan][uNbTotChan] );
            else std::cout <<  "----------- ";
      } // for( UInt_t uNbTotChan = uScanCaseOffset + 1; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan )
      std::cout << std::endl;
   } // for( UInt_t uNbFiredChan = uScanCaseOffset; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan )

   /// Close the file
   histoFile->Close();
   oldDir->cd();

   return kTRUE;
}
