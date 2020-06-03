{
   TCanvas *cRoc0Fee01 = new TCanvas("cRoc0Fee01", "cRoc0Fee01");
   cRoc0Fee01->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc0Fee01->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHits_%03u", uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc0Fee23 = new TCanvas("cRoc0Fee23", "cRoc0Fee23");
   cRoc0Fee23->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc0Fee23->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHits_%03u", 16 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc1Fee01 = new TCanvas("cRoc1Fee01", "cRoc1Fee01");
   cRoc1Fee01->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc1Fee01->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHits_%03u", 64 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc1Fee23 = new TCanvas("cRoc1Fee23", "cRoc1Fee23");
   cRoc1Fee23->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc1Fee23->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHits_%03u", 80 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)


   UInt_t uNbFeeRoc   = 4;
   UInt_t uNbTotFeeRoc= 8;
   UInt_t uNbChipFee  = 8;
   Int_t  iNbChanChip = 4;

   TCanvas *cRoc0S = new TCanvas("cRoc0S", "Roc0 summary");
   cRoc0S->Divide(2);

   TH2* hDoubleHitsRatioRoc0 = new TH2D("hDoubleHitsRatioRoc0",
         "Ratio of double hits per channel for Roc 0; Channel; FEE; Ratio [\%]",
         uNbChipFee*iNbChanChip, -0.5, uNbChipFee*iNbChanChip - 0.5,
          uNbFeeRoc, -0.5,  uNbFeeRoc - 0.5 );
   TH2* hTripleHitsRatioRoc0 = new TH2D("hTripleHitsRatioRoc0",
         "Ratio of triple hits per channel for Roc 0; Channel; FEE; Ratio [\%]",
         uNbChipFee*iNbChanChip, -0.5, uNbChipFee*iNbChanChip - 0.5,
          uNbFeeRoc, -0.5,  uNbFeeRoc - 0.5 );
   for( UInt_t uFee = 0; uFee < uNbFeeRoc; uFee++)
      for( UInt_t uGet4 = 0; uGet4 < uNbChipFee; uGet4 ++)
      {
         TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHits_%03u",
               uNbChipFee*uFee + uGet4) ) );
         if( NULL != hHistoPtr )
            for( Int_t iChan = 0; iChan < iNbChanChip; iChan ++)
            {
               // Double hits
               if( 0 < hHistoPtr->GetBinContent( 1 + iChan, 1) )
                  hDoubleHitsRatioRoc0->Fill( iNbChanChip*uGet4 + iChan, uFee,
                     100.0 *
                      hHistoPtr->GetBinContent( 1 + iChan, 2)
                    / hHistoPtr->GetBinContent( 1 + iChan, 1) );
               // Triple hits
               if( 0 < hHistoPtr->GetBinContent( 1 + iChan, 1) )
                  hTripleHitsRatioRoc0->Fill( iNbChanChip*uGet4 + iChan, uFee,
                     100.0 *
                      hHistoPtr->GetBinContent( 1 + iChan, 3)
                    / hHistoPtr->GetBinContent( 1 + iChan, 1) );
            } // for( Int_t iChan = 0; iChan < 4; iChan ++)
      } // Loop over Get4 and Fee

   cRoc0S->cd(1);
   hDoubleHitsRatioRoc0->SetStats(kFALSE);
   hDoubleHitsRatioRoc0->Draw("colz");
   cRoc0S->cd(2);
   hTripleHitsRatioRoc0->SetStats(kFALSE);
   hTripleHitsRatioRoc0->Draw("colz");


   TCanvas *cRoc1S = new TCanvas("cRoc1S", "Roc1 summary");
   cRoc1S->Divide(2);

   TH2* hDoubleHitsRatioRoc1 = new TH2D("hDoubleHitsRatioRoc1",
         "Ratio of double hits per channel for Roc 1; Channel; FEE; Ratio [\%]",
         uNbChipFee*iNbChanChip, -0.5, uNbChipFee*iNbChanChip - 0.5,
          uNbFeeRoc, -0.5,  uNbFeeRoc - 0.5 );
   TH2* hTripleHitsRatioRoc1 = new TH2D("hTripleHitsRatioRoc1",
         "Ratio of triple hits per channel for Roc 1; Channel; FEE; Ratio [\%]",
         uNbChipFee*iNbChanChip, -0.5, uNbChipFee*iNbChanChip - 0.5,
          uNbFeeRoc, -0.5,  uNbFeeRoc - 0.5 );
   for( UInt_t uFee = 0; uFee < uNbFeeRoc; uFee++)
      for( UInt_t uGet4 = 0; uGet4 < uNbChipFee; uGet4 ++)
      {
         TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHits_%03u",
               uNbTotFeeRoc*uNbChipFee + uNbChipFee*uFee + uGet4) ) );
         if( NULL != hHistoPtr )
            for( Int_t iChan = 0; iChan < iNbChanChip; iChan ++)
            {
               // Double hits
               if( 0 < hHistoPtr->GetBinContent( 1 + iChan, 1) )
                  hDoubleHitsRatioRoc1->Fill( iNbChanChip*uGet4 + iChan, uFee,
                     100.0 *
                      hHistoPtr->GetBinContent( 1 + iChan, 2)
                    / hHistoPtr->GetBinContent( 1 + iChan, 1) );
               // Triple hits
               if( 0 < hHistoPtr->GetBinContent( 1 + iChan, 1) )
                  hTripleHitsRatioRoc1->Fill( iNbChanChip*uGet4 + iChan, uFee,
                     100.0 *
                      hHistoPtr->GetBinContent( 1 + iChan, 3)
                    / hHistoPtr->GetBinContent( 1 + iChan, 1) );
            } // for( Int_t iChan = 0; iChan < 4; iChan ++)
      } // Loop over Get4 and Fee

   cRoc1S->cd(1);
   hDoubleHitsRatioRoc1->SetStats(kFALSE);
   hDoubleHitsRatioRoc1->Draw("colz");
   cRoc1S->cd(2);
   hTripleHitsRatioRoc1->SetStats(kFALSE);
   hTripleHitsRatioRoc1->Draw("colz");
}
