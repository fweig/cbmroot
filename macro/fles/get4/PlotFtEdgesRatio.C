

//   hPulserFeeDnl->Draw("colz");

//   hPulserFeeTotDnl->Draw("colz");

Bool_t PlotFtEdgesRatio( UInt_t uChannel = 0 )
{
/*
   TH2* hInputRis = hPulserFeeDnl;
   TH2* hInputFal = hPulserFeeTotDnl;
*/
   TH2* hInputRis = hFtDistribFeeRis;
   TH2* hInputFal = hFtDistribFeeFal;

   TH1* projRisingEdgeAll  = hInputRis->ProjectionX( "projRisingEdgeAll" );

   TH1* projFallingEdgeAll = hInputFal->ProjectionX( "projFallingEdgeAll" );

   TH1* projRisingEdge  = hInputRis->ProjectionY( Form("projRisingEdge_ch%02u", uChannel),  1+uChannel, 1+uChannel);

   TH1* projFallingEdge = hInputFal->ProjectionY( Form("projFallingEdge_ch%02u", uChannel), 1+uChannel, 1+uChannel);

   // Ratios of the total counts of rising/falling edges per chanl
   TH1* hEdgesRatioAll = projRisingEdgeAll->Clone( "hEdgesRatio_all" );
   hEdgesRatioAll->SetTitle( "Ratio of Falling over Rising edges totals for each channel" );
   (hEdgesRatioAll->GetYaxis())->SetTitle( "Ratio F/R [\%]");

   hEdgesRatioAll->Reset();
   hEdgesRatioAll->Divide( projFallingEdgeAll, projRisingEdgeAll, 100.0);

   // Ratio of the counts of rising/falling edges for each FT bin of the selected channel
   TH1* hEdgesRatio = projRisingEdge->Clone( Form("hEdgesRatio_ch%02u", uChannel) );
   hEdgesRatio->SetTitle( Form("Ratio of Falling over Rising edges per FT bin for channel %02u", uChannel) );
   (hEdgesRatio->GetYaxis())->SetTitle( "Ratio F/R [\%]");

   hEdgesRatio->Reset();
   hEdgesRatio->Divide( projFallingEdge, projRisingEdge, 100.0);

   // Ratios of the total counts of rising/falling edges per chanl
   TH1* hEdgesDiffAll = projRisingEdgeAll->Clone( "hEdgesRatio_all_diff" );
   hEdgesDiffAll->SetTitle( "Difference of Falling minus Rising edges totals for each channel" );
   (hEdgesDiffAll->GetYaxis())->SetTitle( "Diff F/R []");

   hEdgesDiffAll->Reset();
   hEdgesDiffAll->Add( projFallingEdgeAll, projRisingEdgeAll, -1.0);

   // Ratio of the counts of rising/falling edges for each FT bin of the selected channel
   TH1* hEdgesDiff = projRisingEdge->Clone( Form("hEdgesRatio_ch%02u_diff", uChannel) );
   hEdgesDiff->SetTitle( Form("Difference of Falling minus Rising edges per FT bin for channel %02u", uChannel) );
   (hEdgesDiff->GetYaxis())->SetTitle( "Diff F/R []");

   hEdgesDiff->Reset();
   hEdgesDiff->Add( projFallingEdge, projRisingEdge, -1.0);

   TCanvas *cFtEdgesRatio= new TCanvas("cFtEdgesRatio", "Ratio of Falling over Rising edges per FT bin");
   cFtEdgesRatio->Divide(2, 2);

   cFtEdgesRatio->cd(1);
   hEdgesRatioAll->Draw("htext");

   cFtEdgesRatio->cd(2);
   hEdgesRatio->Draw();

   cFtEdgesRatio->cd(3);
   hEdgesDiffAll->Draw("htext");

   cFtEdgesRatio->cd(4);
//   hEdgesDiff->Draw();
   THStack * stackFt = new THStack();
   hEdgesDiff->SetLineColor(kViolet);
   projRisingEdge->SetLineColor( kBlack);
   projFallingEdge->SetLineColor( kRed);
   stackFt->Add(hEdgesDiff);
   stackFt->Add(projRisingEdge);
   stackFt->Add(projFallingEdge);
   stackFt->Draw("nostack,h");

   TLegend * legFt = new TLegend(0.6,0.7,0.98,0.9);
//   legFt->SetHeader("title");
   legFt->AddEntry(hEdgesDiff,      "Diff F - R","l");
   legFt->AddEntry(projRisingEdge,  "R per FT bin","l");
   legFt->AddEntry(projFallingEdge, "F per FT bin","l");
   legFt->Draw();

   /* counts per CT bin Rising/Falling comparison */
   TH2* hInputRisCt = hPulserFeeRisCtWideBins;
   TH2* hInputFalCt = hPulserFeeFalCtWideBins;

   TH1* projRisingEdgeAllCt  = hInputRisCt->ProjectionX( "projRisingEdgeAllCt" );

   TH1* projFallingEdgeAllCt = hInputFalCt->ProjectionX( "projFallingEdgeAllCt" );

   TH1* projRisingEdgeCt  = hInputRisCt->ProjectionY( Form("projRisingEdgeCt_ch%02u", uChannel),  1+uChannel, 1+uChannel);

   TH1* projFallingEdgeCt = hInputFalCt->ProjectionY( Form("projFallingEdgeCt_ch%02u", uChannel), 1+uChannel, 1+uChannel);

   // Ratios of the total counts of rising/falling edges per chanl
   TH1* hEdgesRatioAllCt = projRisingEdgeAllCt->Clone( "hEdgesRatio_allCt" );
   hEdgesRatioAllCt->SetTitle( "Ratio of Falling over Rising edges totals for each channel" );
   (hEdgesRatioAllCt->GetYaxis())->SetTitle( "Ratio F/R [\%]");

   hEdgesRatioAllCt->Reset();
   hEdgesRatioAllCt->Divide( projFallingEdgeAllCt, projRisingEdgeAllCt, 100.0);

   // Ratio of the counts of rising/falling edges for each FT bin of the selected channel
   TH1* hEdgesRatioCt = projRisingEdgeCt->Clone( Form("hEdgesRatioCt_ch%02u", uChannel) );
   hEdgesRatioCt->SetTitle( Form("Ratio of Falling over Rising edges per FT bin for channel %02u", uChannel) );
   (hEdgesRatioCt->GetYaxis())->SetTitle( "Ratio F/R [\%]");

   hEdgesRatioCt->Reset();
   hEdgesRatioCt->Divide( projFallingEdgeCt, projRisingEdgeCt, 100.0);

   // Ratios of the total counts of rising/falling edges per chanl
   TH1* hEdgesDiffAllCt = projRisingEdgeAllCt->Clone( "hEdgesRatio_allCt_diff" );
   hEdgesDiffAllCt->SetTitle( "Difference of Falling minus Rising edges totals for each channel" );
   (hEdgesDiffAllCt->GetYaxis())->SetTitle( "Diff F/R []");

   hEdgesDiffAllCt->Reset();
   hEdgesDiffAllCt->Add( projFallingEdgeAllCt, projRisingEdgeAllCt, -1.0);

   // Ratio of the counts of rising/falling edges for each FT bin of the selected channel
   TH1* hEdgesDiffCt = projRisingEdgeCt->Clone( Form("hEdgesRatioCt_ch%02u_diff", uChannel) );
   hEdgesDiffCt->SetTitle( Form("Difference of Falling minus Rising edges per FT bin for channel %02u", uChannel) );
   (hEdgesDiffCt->GetYaxis())->SetTitle( "Diff F/R []");

   hEdgesDiffCt->Reset();
   hEdgesDiffCt->Add( projFallingEdgeCt, projRisingEdgeCt, -1.0);

   TCanvas *cFtEdgesRatioCt= new TCanvas("cFtEdgesRatioCt", "Ratio of Falling over Rising edges per Ct bin");
   cFtEdgesRatioCt->Divide(2, 2);

   cFtEdgesRatioCt->cd(1);
   hEdgesRatioAllCt->Draw("htext");

   cFtEdgesRatioCt->cd(2);
   hEdgesRatioCt->Draw();

   cFtEdgesRatioCt->cd(3);
   hEdgesDiffAllCt->Draw("htext");

   cFtEdgesRatioCt->cd(4);
  // hEdgesDiffCt->Draw();
   THStack * stackCt = new THStack();
   hEdgesDiffCt->SetLineColor(kViolet);
   projRisingEdgeCt->SetLineColor( kBlack);
   projFallingEdgeCt->SetLineColor( kRed);
   stackCt->Add(hEdgesDiffCt);
   stackCt->Add(projFallingEdgeCt);
   stackCt->Add(projRisingEdgeCt);
   stackCt->Draw("nostack,h");

   TLegend * legCt = new TLegend(0.6,0.7,0.98,0.9);
//   legCt->SetHeader("title");
   legCt->AddEntry(hEdgesDiffCt,      "Diff F - R","l");
   legCt->AddEntry(projFallingEdgeCt,  "R per FT bin","l");
   legCt->AddEntry(projRisingEdgeCt, "F per FT bin","l");
   legCt->Draw();

   // Extra edges
   TH1* projExtraEdgeRisA  = hPulserFeeFtExtraEdgeRisA->ProjectionY(
                                 Form("projExtraEdgeRisA_ch%02u", uChannel),
                                 1+uChannel, 1+uChannel);
   TH1* projExtraEdgeFalA  = hPulserFeeFtExtraEdgeFalA->ProjectionY(
                                 Form("projExtraEdgeFalA_ch%02u", uChannel),
                                 1+uChannel, 1+uChannel);
   TH1* projExtraEdgeRisB  = hPulserFeeFtExtraEdgeRisB->ProjectionY(
                                 Form("projExtraEdgeRisB_ch%02u", uChannel),
                                 1+uChannel, 1+uChannel);
   TH1* projExtraEdgeFalB  = hPulserFeeFtExtraEdgeFalB->ProjectionY(
                                 Form("projExtraEdgeFalB_ch%02u", uChannel),
                                 1+uChannel, 1+uChannel);


   TCanvas *cExtraEdges= new TCanvas("cExtraEdges", "Monitoring extra edges (assume 1 pulse per epoch)");
   cExtraEdges->Divide(2);

   cExtraEdges->cd(1);
   gPad->SetLogz();
   hPulserFeeExtraEdgesEp->Draw("colz");

   cExtraEdges->cd(2);
   THStack * stackExtraFt = new THStack();
   projExtraEdgeRisA->SetLineColor( kBlack  );
   projExtraEdgeFalA->SetLineColor( kGreen  );
   projExtraEdgeRisB->SetLineColor( kRed    );
   projExtraEdgeFalB->SetLineColor( kViolet );
   stackExtraFt->Add(projExtraEdgeRisA);
   stackExtraFt->Add(projExtraEdgeFalA);
   stackExtraFt->Add(projExtraEdgeRisB);
   stackExtraFt->Add(projExtraEdgeFalB);
   stackExtraFt->Draw("nostack,h");

   TLegend * legExtraFt = new TLegend(0.6,0.7,0.98,0.9);
//   legExtraFt->SetHeader("title");
   legExtraFt->AddEntry(projExtraEdgeRisA, "FT first R","l");
   legExtraFt->AddEntry(projExtraEdgeFalA, "FT first F","l");
   legExtraFt->AddEntry(projExtraEdgeRisB, "FT Sec R","l");
   legExtraFt->AddEntry(projExtraEdgeFalB, "FT Sec F","l");
   legExtraFt->Draw();

   TCanvas *cExtraEdgesB= new TCanvas("cExtraEdgesB", "Monitoring extra edges (assume 1 pulse per epoch)");

   TH1* projExtraEdgeTotA  = hPulserFeeGoodTot->ProjectionY(
                                 Form("projExtraEdgeTotA_ch%02u", uChannel),
                                 1+uChannel, 1+uChannel);
   TH1* projExtraEdgeTotB  = hPulserFeeExtraRecoTot->ProjectionY(
                                 Form("projExtraEdgeTotB_ch%02u", uChannel),
                                 1+uChannel, 1+uChannel);

   cExtraEdgesB->cd();
   gPad->SetLogy();
   THStack * stackExtraTot = new THStack();
   projExtraEdgeTotA->SetLineColor( kBlack  );
   projExtraEdgeTotB->SetLineColor( kRed    );
   stackExtraTot->Add(projExtraEdgeTotA);
   stackExtraTot->Add(projExtraEdgeTotB);
   stackExtraTot->Draw("nostack,h");

   TLegend * legExtraTot = new TLegend(0.6,0.7,0.98,0.9);
//   legExtraFt->SetHeader("title");
   legExtraTot->AddEntry(projExtraEdgeTotA, "Tot good pulse","l");
   legExtraTot->AddEntry(projExtraEdgeTotB, "Tot reco extra","l");
   legExtraTot->Draw();

   return kTRUE;
}
