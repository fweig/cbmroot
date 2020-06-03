
Bool_t PlotLongRangeDate( UInt_t uGdpbSel = 0, UInt_t uFeetSel = 0,
                           Int_t dateIn = 20170510, Int_t timeIn = 1648,
                           TString sInFileName = "data/test.root",
                           Int_t iBinSizeMin = 1,
                           UInt_t uNbGdpb = 4, UInt_t uNbFeeGdpb = 3 )
{
   TDatime * fRunStartDateTime = new TDatime( dateIn, timeIn);
   Int_t fiRunStartDateTimeSec = fRunStartDateTime->Convert();

   std::cout << "Assigned new TOF Run Start Date-Time: " << fRunStartDateTime->AsString()
             << std::endl;

   /// Open Histo file
   TFile * pSourceFile = new TFile( sInFileName, "READ");
   if( NULL == pSourceFile )
   {
      std::cerr << "Failed to open the file with histos!"
                << std::endl;
      return kFALSE;
   } // if( NULL == pSourceFile )

   /// Go back to session root folder (out of file)
   gROOT->cd();

   /// Get Input histos
   std::vector< std::vector< TH1 * > > pHitsMin( uNbGdpb, std::vector< TH1 * >(uNbFeeGdpb, NULL) );
   std::vector< std::vector< TH1 * > > pErrorsMin( uNbGdpb, std::vector< TH1 * >(uNbFeeGdpb, NULL) );
   TH1  * pTemp;
   for( UInt_t uGdpb = 0; uGdpb < uNbGdpb; uGdpb ++)
      for( UInt_t uFeet = 0; uFeet < uNbFeeGdpb; uFeet ++)
      {
            // Recover Hits histo and clone it
         pTemp = NULL;
         pTemp = (TH1F*) (pSourceFile->FindObjectAny( Form("FeetRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet) ) );
         if( NULL != pTemp )
         {
            pHitsMin[uGdpb][uFeet] = (TH1F*)(pTemp->Clone( Form("HitsMin_gDPB_g%02u_f%1u", uGdpb, uFeet) ) );
         } // if( NULL != pTemp )
            else
            {
               std::cerr << "ERROR: Failed to recover the hits rate histogram for "
                         << Form("FeetRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet)
                         << ", stopping there."
                         << std::endl;
               pSourceFile->Close();
               return kFALSE;
            } // else of if( NULL != pTemp )
            // Recover Errors histo and clone it
         pTemp = NULL;
         pTemp = (TH1F*) (pSourceFile->FindObjectAny( Form("FeetErrorRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet) ) );
         if( NULL != pTemp )
         {
            pErrorsMin[uGdpb][uFeet] = (TH1F*)(pTemp->Clone( Form("ErrorsMin_gDPB_g%02u_f%1u", uGdpb, uFeet) ) );
         } // if( NULL != pTemp )
            else
            {
               std::cerr << "ERROR: Failed to recover the errors rate histogram for "
                         << Form("FeetErrorRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet)
                         << ", stopping there."
                         << std::endl;
               pSourceFile->Close();
               return kFALSE;
            } // else of if( NULL != pTemp )

         if( pHitsMin[uGdpb][uFeet]->GetNbinsX() != pErrorsMin[uGdpb][uFeet]->GetNbinsX() ||
             pHitsMin[uGdpb][uFeet]->GetXaxis()->GetXmin() != pErrorsMin[uGdpb][uFeet]->GetXaxis()->GetXmin() ||
             pHitsMin[uGdpb][uFeet]->GetXaxis()->GetXmax() != pErrorsMin[uGdpb][uFeet]->GetXaxis()->GetXmax() )
         {
            std::cerr << "X axis of hits and errors input histos are not the same, do nothing!"
                      << std::endl;
            return kFALSE;
         } // X axis not the same for hits and errors
      } // Loop on gDPB and FEE

   /// Close input file
   pSourceFile->Close();

   /// Create output histos
   std::vector< std::vector< TH1 * > > hHitsDate( uNbGdpb, std::vector< TH1 * >(uNbFeeGdpb, NULL) );
   std::vector< std::vector< TH1 * > > hErrsDate( uNbGdpb, std::vector< TH1 * >(uNbFeeGdpb, NULL) );
   for( UInt_t uGdpb = 0; uGdpb < uNbGdpb; uGdpb ++)
      for( UInt_t uFeet = 0; uFeet < uNbFeeGdpb; uFeet ++)
      {
         Double_t dStartTime = pHitsMin[uGdpb][uFeet]->GetXaxis()->GetXmin() * 60.0 ; // Convert Minutes to Seconds
         Double_t dStopTime  = pHitsMin[uGdpb][uFeet]->GetXaxis()->GetXmax() * 60.0 ; // Convert Minutes to Seconds
         hHitsDate[uGdpb][uFeet] = new TH1D( Form("FeetRateLongDate_gDPB_g%02u_f%1u", uGdpb, uFeet),
                                Form("Counts per second in Feet %1u of gDPB %02u; Time[s] ; Counts", uFeet, uGdpb),
                                ( (dStopTime - dStartTime) / (iBinSizeMin * 60.0) ), dStartTime, dStopTime );
         hHitsDate[uGdpb][uFeet]->GetXaxis()->SetTimeDisplay(1);
         hHitsDate[uGdpb][uFeet]->GetXaxis()->SetTimeOffset( fiRunStartDateTimeSec );
         hErrsDate[uGdpb][uFeet] = new TH1D( Form("FeetErrorRateLongDate_gDPB_g%02u_f%1u", uGdpb, uFeet),
                                Form("Error Counts per second in Feet %1u of gDPB %02u; Time[s] ; Error Counts", uFeet, uGdpb),
                                ( (dStopTime - dStartTime) / (iBinSizeMin * 60.0) ), dStartTime, dStopTime );
         hErrsDate[uGdpb][uFeet]->GetXaxis()->SetTimeDisplay(1);
         hErrsDate[uGdpb][uFeet]->GetXaxis()->SetTimeOffset( fiRunStartDateTimeSec );

         /// Fill output histos
         for( Int_t iBin = 1; iBin <= pHitsMin[uGdpb][uFeet]->GetNbinsX(); iBin++ )
         {
            hHitsDate[uGdpb][uFeet]->Fill( 60.0 * pHitsMin[uGdpb][uFeet]->GetBinCenter( iBin ),
                                                  pHitsMin[uGdpb][uFeet]->GetBinContent( iBin ) );
            hErrsDate[uGdpb][uFeet]->Fill( 60.0 * pErrorsMin[uGdpb][uFeet]->GetBinCenter( iBin ),
                                                  pErrorsMin[uGdpb][uFeet]->GetBinContent( iBin ) );
         } // for( Int_t iBin = 1; iBin <= pHitsMin->GetNbinsX() )
      } // Loop on gDPB and FEE

   /// Create FEET rates long duration Canvas
   TCanvas* cFeeRatesLong = new TCanvas("cFeeRatesLong", "gDPB FEET rates date");
   cFeeRatesLong->Divide(uNbFeeGdpb, uNbGdpb );

   /// Create Hits + Errors stack and draw hit
   UInt_t uPadIdx = 0;
   THStack * stackLong[uNbGdpb][uNbFeeGdpb];
   for( UInt_t uGdpb = 0; uGdpb < uNbGdpb; uGdpb ++)
      for( UInt_t uFeet = 0; uFeet < uNbFeeGdpb; uFeet ++)
      {
         uPadIdx ++;
         cFeeRatesLong->cd( uPadIdx );

         stackLong[uGdpb][uFeet] = new THStack(
                  Form("hsRateLong_g%02u_f%1u", uGdpb, uFeet),
                  Form("Rate of hits and errors (red), long duration, for g%02u f%1u", uGdpb, uFeet) );

         stackLong[uGdpb][uFeet]->Add(hHitsDate[uGdpb][uFeet], "hist");
         hErrsDate[uGdpb][uFeet]->SetLineColor( kRed );
         stackLong[uGdpb][uFeet]->Add(hErrsDate[uGdpb][uFeet], "hist");
         stackLong[uGdpb][uFeet]->Draw("nostack");
            /// Change Axis display only after first draw, otherwise axis not existing yet!
         stackLong[uGdpb][uFeet]->GetXaxis()->SetTimeDisplay(1);
         stackLong[uGdpb][uFeet]->GetXaxis()->SetTimeOffset( fiRunStartDateTimeSec );
            /// Pad cosmetics
         gPad->SetLogy();
         gPad->SetGridx();
         gPad->SetGridy();
      } // Loop on gDPB and FEE

   /// Create selected plot canvas
   TCanvas* cFeeRatesSel = new TCanvas("cFeeRatesSel", "gDPB FEET rate selected");
   cFeeRatesSel->cd();
   gPad->SetLogy();
   gPad->SetGridx();
   gPad->SetGridy();
   stackLong[uGdpbSel][uFeetSel]->Draw("nostack");

   return kTRUE;
}
