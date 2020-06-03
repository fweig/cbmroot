{
   TCanvas *cRoc0Fee01DistD = new TCanvas("cRoc0Fee01DistD", "cRoc0Fee01 Double hits distance");
   cRoc0Fee01DistD->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc0Fee01DistD->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistDoubleHits_%03u", uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc0Fee23DistD = new TCanvas("cRoc0Fee23DistD", "cRoc0Fee23 Double hits distance");
   cRoc0Fee23DistD->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc0Fee23DistD->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistDoubleHits_%03u", 16 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc1Fee01DistD = new TCanvas("cRoc1Fee01DistD", "cRoc1Fee01 Double hits distance");
   cRoc1Fee01DistD->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc1Fee01DistD->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistDoubleHits_%03u", 64 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc1Fee23DistD = new TCanvas("cRoc1Fee23DistD", "cRoc1Fee23 Double hits distance");
   cRoc1Fee23DistD->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc1Fee23DistD->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistDoubleHits_%03u", 80 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc0Fee01DistT = new TCanvas("cRoc0Fee01DistT", "cRoc0Fee01 Triple hits distance");
   cRoc0Fee01DistT->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc0Fee01DistT->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistTripleHits_%03u", uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc0Fee23DistT = new TCanvas("cRoc0Fee23DistT", "cRoc0Fee23 Triple hits distance");
   cRoc0Fee23DistT->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc0Fee23DistT->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistTripleHits_%03u", 16 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc1Fee01DistT = new TCanvas("cRoc1Fee01DistT", "cRoc1Fee01 Triple hits distance");
   cRoc1Fee01DistT->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc1Fee01DistT->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistTripleHits_%03u", 64 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc1Fee23DistT = new TCanvas("cRoc1Fee23DistT", "cRoc1Fee23 Triple hits distance");
   cRoc1Fee23DistT->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc1Fee23DistT->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistTripleHits_%03u", 80 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   UInt_t uNbRocs     =  2;
   UInt_t uNbFeeRoc   =  4;
   UInt_t uNbTotFeeRoc=  8;
   UInt_t uNbChipFee  =  8;
   Int_t iNbTdcs      = uNbRocs*uNbFeeRoc*uNbChipFee;
   Int_t iNbChChip    =  4;
   Int_t iNbChFee     = uNbChipFee*uNbChipFee;
   TProfile * profDistDouble[iNbTdcs];
   TProfile * profDistTriple[iNbTdcs];

   UInt_t uTdc = 0;
   for( UInt_t uRoc = 0; uRoc < uNbRocs; uRoc++)
      for( UInt_t uFee = 0; uFee < uNbFeeRoc; uFee++)
         for( UInt_t uGet4 = 0; uGet4 < uNbChipFee; uGet4 ++)
         {
            TH2* hHistoPtrD = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistDoubleHits_%03u", 
                                          uNbChipFee*uNbTotFeeRoc*uRoc + uNbChipFee*uFee + uGet4) ) );
            if( NULL != hHistoPtrD )
               profDistDouble[uTdc] = hHistoPtrD->ProfileX( Form("ProfDouble_%03u", uTdc) );
               else profDistDouble[uTdc] = NULL;

            TH2* hHistoPtrT = (TH2*) (gDirectory->FindObjectAny( Form("hGet4DistTripleHits_%03u", 
                                          uNbChipFee*uNbTotFeeRoc*uRoc + uNbChipFee*uFee + uGet4) ) );
            if( NULL != hHistoPtrT )
               profDistTriple[uTdc] = hHistoPtrT->ProfileX( Form("ProfTriple_%03u", uTdc) );
               else profDistTriple[uTdc] = NULL;

            uTdc++;
         } // loop over GET4s

   TCanvas *cRocS[uNbRocs];
   TH1D* hMeanDoubleHitsDist[uNbRocs][uNbFeeRoc];
   TH1D* hMeanTripleHitsDist[uNbRocs][uNbFeeRoc];

   uTdc = 0;
   for( UInt_t uRoc = 0; uRoc < uNbRocs; uRoc++)
   {
      cRocS[uRoc] = new TCanvas(Form("cRocS_%03u", uRoc), Form("Roc %03u summary mean double dist (blue) and mean triple dist (red)", uRoc) );
      cRocS[uRoc]->Divide(2, 2);

      for( UInt_t uFee = 0; uFee < uNbFeeRoc; uFee++)
      {
         hMeanDoubleHitsDist[uRoc][uFee] = new TH1D( Form("hMeanDoubleHitsDist_%03u_%03u", uRoc, uFee),
               Form("2nd hit distance per channel for Roc %03u Fee %03u; Channel; Mean deltaT 1st-2nd [ns]", uRoc, uFee),
               iNbChFee, -0.5, iNbChFee -0.5 );

         hMeanTripleHitsDist[uRoc][uFee] = new TH1D(Form("hMeanTripleHitsDist_%03u_%03u", uRoc, uFee),
               Form("3rd hit distance per channel for Roc %03u Fee %03u; Channel; Mean deltaT 3rd-2nd [ns]",uRoc, uFee),
               iNbChFee, -0.5, iNbChFee -0.5 );

         for( UInt_t uGet4 = 0; uGet4 < uNbChipFee; uGet4 ++)
         {
            if( NULL != profDistDouble[uTdc] && NULL != profDistTriple[uTdc] )
            for(Int_t iChan = 0; iChan < iNbChChip; iChan++)
            {
               // Double
               hMeanDoubleHitsDist[uRoc][uFee]->SetBinContent( uGet4*iNbChChip + iChan + 1,
                     profDistDouble[     uTdc]->GetBinContent(iChan + 1) );
               hMeanDoubleHitsDist[uRoc][uFee]->SetBinError(   uGet4*iNbChChip + iChan + 1,
                     profDistDouble[     uTdc]->GetBinError(iChan + 1) );

               // Triple
               hMeanTripleHitsDist[uRoc][uFee]->SetBinContent( uGet4*iNbChChip + iChan + 1,
                     profDistTriple[     uTdc]->GetBinContent(iChan + 1) );
               hMeanTripleHitsDist[uRoc][uFee]->SetBinError(   uGet4*iNbChChip + iChan + 1,
                     profDistTriple[     uTdc]->GetBinError(iChan + 1) );
            } // for(Int_t iChan = 0; iChan < iNbChChip; iChan++)
            uTdc++;
         } // for( UInt_t uGet4 = 0; uGet4 < uNbChipFee; uGet4 ++)

         cRocS[uRoc]->cd( 1 + uFee );

         hMeanDoubleHitsDist[uRoc][uFee]->SetStats(kFALSE);
         hMeanDoubleHitsDist[uRoc][uFee]->SetLineColor( kBlue );
         hMeanDoubleHitsDist[uRoc][uFee]->Draw("E");

         hMeanTripleHitsDist[uRoc][uFee]->SetStats(kFALSE);
         hMeanTripleHitsDist[uRoc][uFee]->SetLineColor( kRed );
         hMeanTripleHitsDist[uRoc][uFee]->Draw("ESame");
      } // for( UInt_t uFee = 0; uFee < uNbFeeRoc; uFee++)
   } // for( UInt_t uRoc = 0; uRoc < uNbRocs; uRoc++)
}
