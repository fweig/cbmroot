{
   TCanvas *cRoc0Fee01Tot = new TCanvas("cRoc0Fee01Tot", "cRoc0Fee01 Multiple hits as function of TOT");
   cRoc0Fee01Tot->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc0Fee01Tot->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHitsVsTot_%03u", uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc0Fee23Tot = new TCanvas("cRoc0Fee23Tot", "cRoc0Fee23 Double hits distance");
   cRoc0Fee23Tot->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc0Fee23Tot->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHitsVsTot_%03u", 16 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc1Fee01Tot = new TCanvas("cRoc1Fee01Tot", "cRoc1Fee01 Double hits distance");
   cRoc1Fee01Tot->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc1Fee01Tot->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHitsVsTot_%03u", 64 + uGet4) ) );
      if( NULL != hHistoPtr )
      {
         hHistoPtr->Draw("colz");
      } // if( NULL != hHistoPtr )
   } // for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)

   TCanvas *cRoc1Fee23Tot = new TCanvas("cRoc1Fee23Tot", "cRoc1Fee23 Double hits distance");
   cRoc1Fee23Tot->Divide(4, 4);

   for( UInt_t uGet4 = 0; uGet4 < 16; uGet4++)
   {
      cRoc1Fee23Tot->cd( 1 + uGet4);

      TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHitsVsTot_%03u", 80 + uGet4) ) );
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
   Int_t iNbTotBins   = 26;

   TCanvas *cRocS[uNbRocs];
   TH2D* hDoubleHitsRatioTot[uNbRocs];
   TH2D* hTripleHitsRatioTot[uNbRocs];

   for( UInt_t uRoc = 0; uRoc < uNbRocs; uRoc++)
   {
      cRocS[uRoc] = new TCanvas(Form("cRocS_%03u", uRoc), Form("Roc %03u summary Multiple hist ratio as function of Tot", uRoc) );
      cRocS[uRoc]->Divide(2);

      hDoubleHitsRatioTot[uRoc] = new TH2D( Form("hDoubleHitsRatioTot_%03u", uRoc),
            Form("Ratio of double hits per chip Vs Tot for Roc %03u; Chip; ToT [ns]; Ratio [\%]", uRoc),
            uNbChipFee*uNbFeeRoc, -0.5, uNbChipFee*uNbFeeRoc - 0.5,
            iNbTotBins,  0.0, iNbTotBins );

      hTripleHitsRatioTot[uRoc] = new TH2D(Form("hTripleHitsRatioTot_%03u", uRoc),
            Form("Ratio of triple hits per chip Vs Tot for Roc %03u; Chip; ToT [ns]; Ratio [\%]",uRoc),
            uNbChipFee*uNbFeeRoc, -0.5, uNbChipFee*uNbFeeRoc - 0.5,
            iNbTotBins,  0.0, iNbTotBins );

      for( UInt_t uFee = 0; uFee < uNbFeeRoc; uFee++)
      {

         for( UInt_t uGet4 = 0; uGet4 < uNbChipFee; uGet4 ++)
         {
            TH2* hHistoPtr = (TH2*) (gDirectory->FindObjectAny( Form("hGet4MultipleHitsVsTot_%03u", 
                                       uNbChipFee*uNbTotFeeRoc*uRoc + uNbChipFee*uFee + uGet4) ) );
            if( NULL != hHistoPtr )
               for(Int_t iBin = 0; iBin < iNbTotBins; iBin++)
               {
                  // Double
                  if( 0 < hHistoPtr->GetBinContent( 1 + iBin, 1) )
                     hDoubleHitsRatioTot[uRoc]->Fill( uFee*uNbChipFee + uGet4, 0.5 + iBin,
                        100.0 *
                         hHistoPtr->GetBinContent( 1 + iBin, 2)
                       / hHistoPtr->GetBinContent( 1 + iBin, 1) );

                  // Triple
                  if( 0 < hHistoPtr->GetBinContent( 1 + iBin, 1) )
                     hTripleHitsRatioTot[uRoc]->Fill( uFee*uNbChipFee + uGet4, 0.5 + iBin,
                        100.0 *
                         hHistoPtr->GetBinContent( 1 + iBin, 3)
                       / hHistoPtr->GetBinContent( 1 + iBin, 1) );
               } // for(Int_t iChan = 0; iChan < iNbChChip; iChan++)
         } // for( UInt_t uGet4 = 0; uGet4 < uNbChipFee; uGet4 ++)
      } // for( UInt_t uFee = 0; uFee < uNbFeeRoc; uFee++)

         cRocS[uRoc]->cd(1);
//         hDoubleHitsRatioTot[uRoc]->SetStats(kFALSE);
//         hDoubleHitsRatioTot[uRoc]->SetLineColor( kBlue );
         hDoubleHitsRatioTot[uRoc]->Draw("colz");

         cRocS[uRoc]->cd(2);
//         hTripleHitsRatioTot[uRoc]->SetStats(kFALSE);
//         hTripleHitsRatioTot[uRoc]->SetLineColor( kRed );
         hTripleHitsRatioTot[uRoc]->Draw("colz");
   } // for( UInt_t uRoc = 0; uRoc < uNbRocs; uRoc++)

}
