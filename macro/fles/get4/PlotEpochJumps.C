{
   TCanvas *cEpJumpChips = new TCanvas("cEpJumpChips", "Epoch Jumps CT chips");
   cEpJumpChips->Divide(2, 4);

   cEpJumpChips->cd( 1);
   hFullCtEpJumpFeeA_000->Draw("colz");

   cEpJumpChips->cd( 2);
   hFullCtEpJumpFeeA_001->Draw("colz");

   cEpJumpChips->cd( 3);
   hFullCtEpJumpFeeA_002->Draw("colz");

   cEpJumpChips->cd( 4);
   hFullCtEpJumpFeeA_003->Draw("colz");

   cEpJumpChips->cd( 5);
   hFullCtEpJumpFeeA_004->Draw("colz");

   cEpJumpChips->cd( 6);
   hFullCtEpJumpFeeA_005->Draw("colz");

   cEpJumpChips->cd( 7);
   hFullCtEpJumpFeeA_006->Draw("colz");

   cEpJumpChips->cd( 8);
   hFullCtEpJumpFeeA_007->Draw("colz");

   TCanvas *cEpJumpChans = new TCanvas("cEpJumpChans", "Epoch Jumps CT chans");
   cEpJumpChans->Divide(2, 2);

   cEpJumpChans->cd( 1);
   hFullCtEpJumpFeeACh_000->Draw("colz");

   cEpJumpChans->cd( 2);
   hFullCtEpJumpFeeACh_001->Draw("colz");

   cEpJumpChans->cd( 3);
   hFullCtEpJumpFeeACh_002->Draw("colz");

   cEpJumpChans->cd( 4);
   hFullCtEpJumpFeeACh_003->Draw("colz");

   TCanvas *cEpJumpChansSortCt = new TCanvas("cEpJumpChansSortCt", "Epoch Jumps CT chans sorted");
   cEpJumpChansSortCt->Divide(2, 2);

   cEpJumpChansSortCt->cd( 1);
   hFullCtEpJumpFeeAChSort_000->Draw("colz");

   cEpJumpChansSortCt->cd( 2);
   hFullCtEpJumpFeeAChSort_001->Draw("colz");

   cEpJumpChansSortCt->cd( 3);
   hFullCtEpJumpFeeAChSort_002->Draw("colz");

   cEpJumpChansSortCt->cd( 4);
   hFullCtEpJumpFeeAChSort_003->Draw("colz");

   TCanvas *cEpJumpChansSort = new TCanvas("cEpJumpChansSort", "Epoch Jumps chan sort");
   cEpJumpChansSort->Divide(2, 2);

   cEpJumpChansSort->cd( 1);
   hFullCtEpJumpFeeAChOrder->Draw("colz");

   cEpJumpChansSort->cd( 2);
   gPad->SetLogy();
   hGet4BadEpochRatio->Draw("colz");

   cEpJumpChansSort->cd( 3);
   TProfile * pGet4BadEpRatioChip_prof = hGet4BadEpRatioChip->ProfileX( "hGet4BadEpRatioChip_prof" );
//   hGet4BadEpRatioChip->Draw("colz");
   pGet4BadEpRatioChip_prof->Draw("SAME");

   TCanvas *cEpQualityFirstCh = new TCanvas("cEpQualityFirstCh", "Epoch quality as function of first channel readout for each chip in FEE A");
   cEpQualityFirstCh->Divide(2, 4);

   cEpQualityFirstCh->cd( 1);
   TProfile * pEpQualityFirstChFeeA_prof000 = hEpQualityFirstChFeeA_000->ProfileX( "pEpQualityFirstChFeeA_prof000" );
//   hEpQualityFirstChFeeA_000->Draw("colz");
   pEpQualityFirstChFeeA_prof000->Draw("SAME");

   cEpQualityFirstCh->cd( 2);
   TProfile * pEpQualityFirstChFeeA_prof001 = hEpQualityFirstChFeeA_001->ProfileX( "pEpQualityFirstChFeeA_prof001" );
//   hEpQualityFirstChFeeA_001->Draw("colz");
   pEpQualityFirstChFeeA_prof001->Draw("SAME");

   cEpQualityFirstCh->cd( 3);
   TProfile * pEpQualityFirstChFeeA_prof002 = hEpQualityFirstChFeeA_002->ProfileX( "pEpQualityFirstChFeeA_prof002" );
//   hEpQualityFirstChFeeA_002->Draw("colz");
   pEpQualityFirstChFeeA_prof002->Draw("SAME");

   cEpQualityFirstCh->cd( 4);
   TProfile * pEpQualityFirstChFeeA_prof003 = hEpQualityFirstChFeeA_003->ProfileX( "pEpQualityFirstChFeeA_prof003" );
//   hEpQualityFirstChFeeA_003->Draw("colz");
   pEpQualityFirstChFeeA_prof003->Draw("SAME");

   cEpQualityFirstCh->cd( 5);
   TProfile * pEpQualityFirstChFeeA_prof004 = hEpQualityFirstChFeeA_004->ProfileX( "pEpQualityFirstChFeeA_prof004" );
//   hEpQualityFirstChFeeA_004->Draw("colz");
   pEpQualityFirstChFeeA_prof004->Draw("SAME");

   cEpQualityFirstCh->cd( 6);
   TProfile * pEpQualityFirstChFeeA_prof005 = hEpQualityFirstChFeeA_005->ProfileX( "pEpQualityFirstChFeeA_prof005" );
//   hEpQualityFirstChFeeA_005->Draw("colz");
   pEpQualityFirstChFeeA_prof005->Draw("SAME");

   cEpQualityFirstCh->cd( 7);
   TProfile * pEpQualityFirstChFeeA_prof006 = hEpQualityFirstChFeeA_006->ProfileX( "pEpQualityFirstChFeeA_prof006" );
//   hEpQualityFirstChFeeA_006->Draw("colz");
   pEpQualityFirstChFeeA_prof006->Draw("SAME");

   cEpQualityFirstCh->cd( 8);
   TProfile * pEpQualityFirstChFeeA_prof007 = hEpQualityFirstChFeeA_007->ProfileX( "pEpQualityFirstChFeeA_prof007" );
//   hEpQualityFirstChFeeA_007->Draw("colz");
   pEpQualityFirstChFeeA_prof007->Draw("SAME");

   TCanvas *chFullCtChOrderCh = new TCanvas("chFullCtChOrderCh", "CT and readout order for each channel when epoch jump, chip 0 FEE A");
   chFullCtChOrderCh->Divide(2, 2);

   chFullCtChOrderCh->cd( 1);
   hFullCtChOrderCh_000->Draw("colz");

   chFullCtChOrderCh->cd( 2);
   hFullCtChOrderCh_001->Draw("colz");

   chFullCtChOrderCh->cd( 3);
   hFullCtChOrderCh_002->Draw("colz");

   chFullCtChOrderCh->cd( 4);
   hFullCtChOrderCh_003->Draw("colz");

   TCanvas *chFullCtEpQualityCh = new TCanvas("chFullCtEpQualityCh", "Epoch Quality as function of CT (Full range) for each channel, chip 0 FEE A");
   chFullCtEpQualityCh->Divide(2, 2);

   chFullCtEpQualityCh->cd( 1);
   TProfile * pFullCtEpQualityCh_prof000 = hFullCtEpQualityCh_000->ProfileX( "pFullCtEpQualityCh_prof000" );
//   hFullCtEpQualityCh_000->Draw("colz");
   pFullCtEpQualityCh_prof000->Draw("SAME");

   chFullCtEpQualityCh->cd( 2);
   TProfile * pFullCtEpQualityCh_prof001 = hFullCtEpQualityCh_001->ProfileX( "pFullCtEpQualityCh_prof001" );
//   hFullCtEpQualityCh_001->Draw("colz");
   pFullCtEpQualityCh_prof001->Draw("SAME");

   chFullCtEpQualityCh->cd( 3);
   TProfile * pFullCtEpQualityCh_prof002 = hFullCtEpQualityCh_002->ProfileX( "pFullCtEpQualityCh_prof002" );
//   hFullCtEpQualityCh_002->Draw("colz");
   pFullCtEpQualityCh_prof002->Draw("SAME");

   chFullCtEpQualityCh->cd( 4);
   TProfile * pFullCtEpQualityCh_prof003 = hFullCtEpQualityCh_003->ProfileX( "pFullCtEpQualityCh_prof003" );
//   hFullCtEpQualityCh_003->Draw("colz");
   pFullCtEpQualityCh_prof003->Draw("SAME");

   TCanvas *chFullCtEpQualityChZoom = new TCanvas("chFullCtEpQualityChZoom", "Epoch Quality as function of CT (Zoomed) for each channel, chip 0 FEE A");
   chFullCtEpQualityChZoom->Divide(2, 2);

   chFullCtEpQualityChZoom->cd( 1);
   TProfile * pFullCtEpQualityChZoom_prof000 = hFullCtEpQualityChZoom_000->ProfileX( "pFullCtEpQualityChZoom_prof000" );
//   hFullCtEpQualityChZoom_000->Draw("colz");
   pFullCtEpQualityChZoom_prof000->Draw("SAME");

   chFullCtEpQualityChZoom->cd( 2);
   TProfile * pFullCtEpQualityChZoom_prof001 = hFullCtEpQualityChZoom_001->ProfileX( "pFullCtEpQualityChZoom_prof001" );
//   hFullCtEpQualityChZoom_001->Draw("colz");
   pFullCtEpQualityChZoom_prof001->Draw("SAME");

   chFullCtEpQualityChZoom->cd( 3);
   TProfile * pFullCtEpQualityChZoom_prof002 = hFullCtEpQualityChZoom_002->ProfileX( "pFullCtEpQualityChZoom_prof002" );
//   hFullCtEpQualityChZoom_002->Draw("colz");
   pFullCtEpQualityChZoom_prof002->Draw("SAME");

   chFullCtEpQualityChZoom->cd( 4);
   TProfile * pFullCtEpQualityChZoom_prof003 = hFullCtEpQualityChZoom_003->ProfileX( "pFullCtEpQualityChZoom_prof003" );
//   hFullCtEpQualityChZoom_003->Draw("colz");
   pFullCtEpQualityChZoom_prof003->Draw("SAME");
}
