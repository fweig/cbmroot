
{
   TCanvas *cFeeRms = new TCanvas("cFeeRms", "Fee RMS");
   cFeeRms->cd( );
   hTimeRmsAllFee->SetStats(0);
   hTimeRmsAllFee->Draw("colz");

   TCanvas *cFeeRes = new TCanvas("cFeeRes", "Fee Resolution after fit (for single channel)");
   cFeeRes->cd( );
   hTimeResAllFee->SetStats(0);
   hTimeResAllFee->Draw("colz");

   const UInt_t kuNbChanMax  = 32*4 + 32*4 +32*4; // 4 FEE/ROC in hardware + 4 FEE/ROC only in software, 2 ROCS
   const UInt_t kuNbChanTest = 16;
   TCanvas *cTimeResPairs = new TCanvas("cTimeResPairs", "Time Res. of pairs of selected chan");
   cTimeResPairs->Divide(4, 4);
   Int_t iPadIndexPairs = 1;
//   TPaveStats *st;
   gStyle->SetOptStat(1110);
   gStyle->SetOptFit(2);
   for(UInt_t uChanA = 0; uChanA < kuNbChanMax && iPadIndexPairs < kuNbChanTest; uChanA++)
      for(UInt_t uChanB = 0; uChanB < kuNbChanMax && iPadIndexPairs < kuNbChanTest; uChanB++)
   {
      cTimeResPairs->cd( iPadIndexPairs );
      TH1* hHistoPtr = (TH1*) (gDirectory->FindObjectAny( Form("hTimeResPairs_%03u_%03u", uChanA, uChanB) ) );
      if( NULL != hHistoPtr )
      {
         gPad->SetLogy();
         hHistoPtr->Draw();
//         st = (TPaveStats*)hHistoPtr->FindObject("stats");
//         st->SetOptStat(1110);
//         st->SetOptFit(0001);
         iPadIndexPairs++;
      } // if( NULL != hHistoPtr )

      // Jump the channels existing only in software
      if( 32*4 -1 == uChanA )
         uChanA += 32*4;
      if( 32*4-1 == uChanB )
         uChanB += 32*4;
   } // Loop on all possible channel combinations

   TCanvas *cTimeResComb = new TCanvas("cTimeResComb", "Time Res. of all Comb. for 4 first selected chan (for single channel)");
   cTimeResComb->Divide(3, 2);
   Int_t iPadIndexComb = 1;
   for(UInt_t uChanA = 0; uChanA < kuNbChanMax && iPadIndexComb < 7; uChanA++)
      for(UInt_t uChanB = 0; uChanB < kuNbChanMax && iPadIndexComb < 7; uChanB++)
   {
      cTimeResComb->cd( iPadIndexComb);
      TH1* hHistoPtr = (TH1*) (gDirectory->FindObjectAny( Form("hTimeResCombi_%03u_%03u", uChanA, uChanB) ) );
      if( NULL != hHistoPtr )
      {
         gPad->SetLogy();
         hHistoPtr->Draw();
//         st = (TPaveStats*)hHistoPtr->FindObject("stats");
//         st->SetOptStat(1110);
//         st->SetOptFit(0001);
         iPadIndexComb++;
      } // if( NULL != hHistoPtr )

      // Jump the channels existing only in software
      if( 32*4-1 == uChanA )
         uChanA += 32*4;
      if( 32*4-1 == uChanB )
         uChanB += 32*4;
   } // Loop on all possible channel combinations

   TCanvas *cChosenRmsRes = new TCanvas("cChosenRmsRes", "RMS and time res for chosen pairs and combi");
   cChosenRmsRes->Divide(2, 2);
   cChosenRmsRes->cd(1);
   hTimeRmsAllPairs->SetStats(0);
   hTimeRmsAllPairs->Draw();
   cChosenRmsRes->cd(2);
   hTimeResAllPairs->SetStats(0);
   hTimeResAllPairs->Draw();
   cChosenRmsRes->cd(3);
   hTimeRmsAllCombi->SetStats(0);
   hTimeRmsAllCombi->Draw("colztext");
   cChosenRmsRes->cd(4);
   hTimeResAllCombi->SetStats(0);
   hTimeResAllCombi->Draw("colztext");

   if( 0 < hTimeRmsAllFeeB->GetEntries() )
   {
      TCanvas *cFeeRmsB = new TCanvas("cFeeRmsB", "Fee RMS for FEE B");
      cFeeRmsB->cd( );
      hTimeRmsAllFeeB->SetStats(0);
      hTimeRmsAllFeeB->Draw("colz");
   } //if( 0 < hTimeRmsAllFee->GetEntries() )

   if( 0 < hTimeResAllFeeB->GetEntries() )
   {
      TCanvas *cFeeResB = new TCanvas("cFeeResB", "Fee Resolution for FEE B after fit (for single channel)");
      cFeeResB->cd( );
      hTimeResAllFeeB->SetStats(0);
      hTimeResAllFeeB->Draw("colz");
   }

   if( 0 < hTimeRmsAllFeeAB->GetEntries() )
   {
      TCanvas *cFeeRmsAB = new TCanvas("cFeeRmsAB", "Fee A vs Fee B RMS ");
      cFeeRmsAB->cd( );
      hTimeRmsAllFeeAB->SetStats(0);
      hTimeRmsAllFeeAB->Draw("colz");
   } //if( 0 < hTimeRmsAllFee->GetEntries() )

   if( 0 < hTimeResAllFeeAB->GetEntries() )
   {
      TCanvas *cFeeResAB = new TCanvas("cFeeResAB", "Fee A vs Fee B Resolution after fit (for single channel)");
      cFeeResAB->cd( );
      hTimeResAllFeeAB->SetStats(0);
      hTimeResAllFeeAB->Draw("colz");
   }
}
