{
   Double_t dStartTime =    0.0;
   Double_t dStopTime  =  400.0;
   Double_t dMinY = 1;
   Double_t dMaxY = 2e8;

   TCanvas *cUnpTimeEvo = new TCanvas("cUnpTimeEvo", "UnpTimeEvo");
   cUnpTimeEvo->Divide(4, 3);

   cUnpTimeEvo->cd( 1);
   // Find max value and min value of all 4 plotted histos and use for Y scale
   /*
   if( 0 < hRocDataCntEvo_000->GetEntries() )
   {
      if( 0 < hRocDataCntEvo_000->GetMinimum() && hRocDataCntEvo_000->GetMinimum() < dMinY)
         dMinY = hRocDataCntEvo_000->GetMinimum();
      if( 0 < hRocDataCntEvo_000->GetMaximum() && hRocDataCntEvo_000->GetMaximum() > dMaxY)
         dMaxY = hRocDataCntEvo_000->GetMaximum();
   }
   */
   TH1 *frameA = gPad->DrawFrame(dStartTime, dMinY, dStopTime, dMaxY);
   frameA->SetTitle("ROC #0");
   frameA->GetXaxis()->SetTitle("Time in run [s]");
//   frameA->GetYaxis()->SetTitle("Y title");
//   frameA->GetXaxis()->SetTimeDisplay(1);
   frameA->GetXaxis()->SetTitleSize( 0.05 );
   frameA->GetXaxis()->SetLabelSize( 0.05 );
   frameA->GetYaxis()->SetLabelSize( 0.06 );
   gPad->Update();
   gPad->SetLogy();
   hRocDataCntEvo_000->SetLineColor(kBlack);
   hRocDataCntEvo_000->SetLineWidth( 2 );
   hRocDataCntEvo_000->Draw("same");
   hRocEpochCntEvo_000->SetLineColor(kMagenta + 2);
   hRocEpochCntEvo_000->SetLineWidth( 2 );
   hRocEpochCntEvo_000->Draw("same");
   hRocErrorCntEvo_000->SetLineColor(kRed);
   hRocErrorCntEvo_000->SetLineWidth( 2 );
   hRocErrorCntEvo_000->Draw("same");
   if( 0 < hRocDataLossCntEvo_000->GetEntries() )
   {
      hRocDataLossCntEvo_000->SetLineColor(kBlue);
      hRocDataLossCntEvo_000->SetLineWidth( 2 );
      hRocDataLossCntEvo_000->Draw("same");
   } // if( 0 < hRocDataLossCntEvo_000->GetEntries() )
   if( 0 < hRocEpochLossCntEvo_000->GetEntries() )
   {
      hRocEpochLossCntEvo_000->SetLineColor(kGreen);
      hRocEpochLossCntEvo_000->SetLineWidth( 2 );
      hRocEpochLossCntEvo_000->Draw("same");
   } // if( 0 < hRocEpochLossCntEvo_000->GetEntries() )

   cUnpTimeEvo->cd( 2);
   TH1 *frameB = gPad->DrawFrame(dStartTime, dMinY, dStopTime, dMaxY);
   frameB->SetTitle("ROC #1");
   frameB->GetXaxis()->SetTitle("Time in run [s]");
//   frameB->GetYaxis()->SetTitle("Y title");
//   frameB->GetXaxis()->SetTimeDisplay(1);
   frameB->GetXaxis()->SetTitleSize( 0.05 );
   frameB->GetXaxis()->SetLabelSize( 0.05 );
   frameB->GetYaxis()->SetLabelSize( 0.06 );
   gPad->Update();
   gPad->SetLogy();
   hRocDataCntEvo_001->SetLineColor(kBlack);
   hRocDataCntEvo_001->SetLineWidth( 2 );
   hRocDataCntEvo_001->Draw("same");
   hRocEpochCntEvo_001->SetLineColor(kMagenta + 2);
   hRocEpochCntEvo_001->SetLineWidth( 2 );
   hRocEpochCntEvo_001->Draw("same");
   hRocErrorCntEvo_001->SetLineColor(kRed);
   hRocErrorCntEvo_001->SetLineWidth( 2 );
   hRocErrorCntEvo_001->Draw("same");
   if( 0 < hRocDataLossCntEvo_001->GetEntries() )
   {
      hRocDataLossCntEvo_001->SetLineColor(kBlue);
      hRocDataLossCntEvo_001->SetLineWidth( 2 );
      hRocDataLossCntEvo_001->Draw("same");
   } // if( 0 < hRocDataLossCntEvo_001->GetEntries() )
   if( 0 < hRocEpochLossCntEvo_001->GetEntries() )
   {
      hRocEpochLossCntEvo_001->SetLineColor(kGreen);
      hRocEpochLossCntEvo_001->SetLineWidth( 2 );
      hRocEpochLossCntEvo_001->Draw("same");
   } // if( 0 < hRocEpochLossCntEvo_001->GetEntries() )

   cUnpTimeEvo->cd( 3);
   gPad->SetLogy();
   (hMsSizeEvo->GetXaxis())->SetRangeUser(dStartTime, dStopTime);
   hMsSizeEvo->Draw("");

   cUnpTimeEvo->cd( 4);
   gPad->SetLogy();
   (hClosedEpSizeEvo->GetXaxis())->SetRangeUser(dStartTime, dStopTime);
   hClosedEpSizeEvo->Draw("");

   cUnpTimeEvo->cd( 5);
   TH1 *frameC = gPad->DrawFrame(dStartTime, dMinY, dStopTime, dMaxY);
   frameC->SetTitle("HDP2");
   frameC->GetXaxis()->SetTitle("Time in run [s]");
//   frameC->GetYaxis()->SetTitle("Y title");
//   frameC->GetXaxis()->SetTimeDisplay(1);
   frameC->GetXaxis()->SetTitleSize( 0.05 );
   frameC->GetXaxis()->SetLabelSize( 0.05 );
   frameC->GetYaxis()->SetLabelSize( 0.06 );
   gPad->Update();
   gPad->SetLogy();
   hDetDataCntEvo_HDP2->SetLineColor(kBlack);
   hDetDataCntEvo_HDP2->SetLineWidth( 2 );
   hDetDataCntEvo_HDP2->Draw("same");
   hDetErrorCntEvo_HDP2->SetLineColor(kRed);
   hDetErrorCntEvo_HDP2->SetLineWidth( 2 );
   hDetErrorCntEvo_HDP2->Draw("same");
   if( 0 < hDetDataLossCntEvo_HDP2->GetEntries() )
   {
      hDetDataLossCntEvo_HDP2->SetLineColor(kBlue);
      hDetDataLossCntEvo_HDP2->SetLineWidth( 2 );
      hDetDataLossCntEvo_HDP2->Draw("same");
   } // if( 0 < hDetDataLossCntEvo_HDP2->GetEntries() )
   if( 0 < hDetEpochLossCntEvo_HDP2->GetEntries() )
   {
      hDetEpochLossCntEvo_HDP2->SetLineColor(kGreen);
      hDetEpochLossCntEvo_HDP2->SetLineWidth( 2 );
      hDetEpochLossCntEvo_HDP2->Draw("same");
   } // if( 0 < hDetEpochLossCntEvo_HDP2->GetEntries() )

   cUnpTimeEvo->cd( 6);
   TH1 *frameD = gPad->DrawFrame(dStartTime, dMinY, dStopTime, dMaxY);
   frameD->SetTitle("TSU Strip");
   frameD->GetXaxis()->SetTitle("Time in run [s]");
//   frameD->GetYaxis()->SetTitle("Y title");
//   frameD->GetXaxis()->SetTimeDisplay(1);
   frameD->GetXaxis()->SetTitleSize( 0.05 );
   frameD->GetXaxis()->SetLabelSize( 0.05 );
   frameD->GetYaxis()->SetLabelSize( 0.06 );
   gPad->Update();
   gPad->SetLogy();
   hDetDataCntEvo_TSU->SetLineColor(kBlack);
   hDetDataCntEvo_TSU->SetLineWidth( 2 );
   hDetDataCntEvo_TSU->Draw("same");
   hDetErrorCntEvo_TSU->SetLineColor(kRed);
   hDetErrorCntEvo_TSU->SetLineWidth( 2 );
   hDetErrorCntEvo_TSU->Draw("same");
   if( 0 < hDetDataLossCntEvo_TSU->GetEntries() )
   {
      hDetDataLossCntEvo_TSU->SetLineColor(kBlue);
      hDetDataLossCntEvo_TSU->SetLineWidth( 2 );
      hDetDataLossCntEvo_TSU->Draw("same");
   } // if( 0 < hDetDataLossCntEvo_TSU->GetEntries() )
   if( 0 < hDetEpochLossCntEvo_TSU->GetEntries() )
   {
      hDetEpochLossCntEvo_TSU->SetLineColor(kGreen);
      hDetEpochLossCntEvo_TSU->SetLineWidth( 2 );
      hDetEpochLossCntEvo_TSU->Draw("same");
   } // if( 0 < hDetEpochLossCntEvo_TSU->GetEntries() )

   cUnpTimeEvo->cd( 7);
   TH1 *frameE = gPad->DrawFrame(dStartTime, dMinY, dStopTime, dMaxY);
   frameE->SetTitle("HDP5");
   frameE->GetXaxis()->SetTitle("Time in run [s]");
//   frameE->GetYaxis()->SetTitle("Y title");
//   frameE->GetXaxis()->SetTimeDisplay(1);
   frameE->GetXaxis()->SetTitleSize( 0.05 );
   frameE->GetXaxis()->SetLabelSize( 0.05 );
   frameE->GetYaxis()->SetLabelSize( 0.06 );
   gPad->Update();
   gPad->SetLogy();
   hDetDataCntEvo_HDP5->SetLineColor(kBlack);
   hDetDataCntEvo_HDP5->SetLineWidth( 2 );
   hDetDataCntEvo_HDP5->Draw("same");
   hDetErrorCntEvo_HDP5->SetLineColor(kRed);
   hDetErrorCntEvo_HDP5->SetLineWidth( 2 );
   hDetErrorCntEvo_HDP5->Draw("same");
   if( 0 < hDetDataLossCntEvo_HDP5->GetEntries() )
   {
      hDetDataLossCntEvo_HDP5->SetLineColor(kBlue);
      hDetDataLossCntEvo_HDP5->SetLineWidth( 2 );
      hDetDataLossCntEvo_HDP5->Draw("same");
   } // if( 0 < hDetDataLossCntEvo_HDP5->GetEntries() )
   if( 0 < hDetEpochLossCntEvo_HDP5->GetEntries() )
   {
      hDetEpochLossCntEvo_HDP5->SetLineColor(kGreen);
      hDetEpochLossCntEvo_HDP5->SetLineWidth( 2 );
      hDetEpochLossCntEvo_HDP5->Draw("same");
   } // if( 0 < hDetEpochLossCntEvo_HDP5->GetEntries() )

   cUnpTimeEvo->cd( 8);
   TH1 *frameF = gPad->DrawFrame(dStartTime, dMinY, dStopTime, dMaxY);
   frameF->SetTitle("USTC");
   frameF->GetXaxis()->SetTitle("Time in run [s]");
//   frameF->GetYaxis()->SetTitle("Y title");
//   frameF->GetXaxis()->SetTimeDisplay(1);
   frameF->GetXaxis()->SetTitleSize( 0.05 );
   frameF->GetXaxis()->SetLabelSize( 0.05 );
   frameF->GetYaxis()->SetLabelSize( 0.06 );
   gPad->Update();
   gPad->SetLogy();
   hDetDataCntEvo_USTC->SetLineColor(kBlack);
   hDetDataCntEvo_USTC->SetLineWidth( 2 );
   hDetDataCntEvo_USTC->Draw("same");
   hDetErrorCntEvo_USTC->SetLineColor(kRed);
   hDetErrorCntEvo_USTC->SetLineWidth( 2 );
   hDetErrorCntEvo_USTC->Draw("same");
   if( 0 < hDetDataLossCntEvo_USTC->GetEntries() )
   {
      hDetDataLossCntEvo_USTC->SetLineColor(kBlue);
      hDetDataLossCntEvo_USTC->SetLineWidth( 2 );
      hDetDataLossCntEvo_USTC->Draw("same");
   } // if( 0 < hDetDataLossCntEvo_USTC->GetEntries() )
   if( 0 < hDetEpochLossCntEvo_USTC->GetEntries() )
   {
      hDetEpochLossCntEvo_USTC->SetLineColor(kGreen);
      hDetEpochLossCntEvo_USTC->SetLineWidth( 2 );
      hDetEpochLossCntEvo_USTC->Draw("same");
   } // if( 0 < hDetEpochLossCntEvo_USTC->GetEntries() )

   cUnpTimeEvo->cd( 9);
   gPad->SetLogy();
   hGet4ChanDataCount->Draw("");

   cUnpTimeEvo->cd(10);
   hMessTypePerGet4->Draw("colz");

   cUnpTimeEvo->cd(11);
   hGet4EpochJumps->Draw("colz");

   cUnpTimeEvo->cd(12);

   cUnpTimeEvo->cd(13);

   cUnpTimeEvo->cd(14);

   cUnpTimeEvo->cd(15);

   cUnpTimeEvo->cd(16);
}
