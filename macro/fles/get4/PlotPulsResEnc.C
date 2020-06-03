
{
   const UInt_t kuNbChanMax  = 32*4 + 32*4 +32*4; // 4 FEE/ROC in hardware + 4 FEE/ROC only in software, 2 ROCS
   const UInt_t kuNbChanTest = 16;

   TCanvas *cTimeResCombEncA = new TCanvas("cTimeResCombEncA", "Time Res. of all Comb. for 4 first selected chan (Encoder A)");
   cTimeResCombEncA->Divide(3, 2);
   Int_t iPadIndexCombEncA = 1;
   for(UInt_t uChanA = 0; uChanA < kuNbChanMax && iPadIndexCombEncA < 7; uChanA++)
      for(UInt_t uChanB = 0; uChanB < kuNbChanMax && iPadIndexCombEncA < 7; uChanB++)
   {
      cTimeResCombEncA->cd( iPadIndexCombEncA);
      TH1* hHistoPtr = (TH1*) (gDirectory->FindObjectAny( Form("hTimeResCombiEncA_%03u_%03u", uChanA, uChanB) ) );
      if( NULL != hHistoPtr )
      {
         gPad->SetLogy();
         hHistoPtr->Draw();
//         st = (TPaveStats*)hHistoPtr->FindObject("stats");
//         st->SetOptStat(1110);
//         st->SetOptFit(0001);
         iPadIndexCombEncA++;
      } // if( NULL != hHistoPtr )

      // Jump the channels existing only in software
      if( 32*4-1 == uChanA )
         uChanA += 32*4;
      if( 32*4-1 == uChanB )
         uChanB += 32*4;
   } // Loop on all possible channel combinations

   TCanvas *cTimeResCombEncB = new TCanvas("cTimeResCombEncB", "Time Res. of all Comb. for 4 first selected chan (Encoder B)");
   cTimeResCombEncB->Divide(3, 2);
   Int_t iPadIndexCombEncB = 1;
   for(UInt_t uChanA = 0; uChanA < kuNbChanMax && iPadIndexCombEncB < 7; uChanA++)
      for(UInt_t uChanB = 0; uChanB < kuNbChanMax && iPadIndexCombEncB < 7; uChanB++)
   {
      cTimeResCombEncB->cd( iPadIndexCombEncB);
      TH1* hHistoPtr = (TH1*) (gDirectory->FindObjectAny( Form("hTimeResCombiEncB_%03u_%03u", uChanA, uChanB) ) );
      if( NULL != hHistoPtr )
      {
         gPad->SetLogy();
         hHistoPtr->Draw();
//         st = (TPaveStats*)hHistoPtr->FindObject("stats");
//         st->SetOptStat(1110);
//         st->SetOptFit(0001);
         iPadIndexCombEncB++;
      } // if( NULL != hHistoPtr )

      // Jump the channels existing only in software
      if( 32*4-1 == uChanA )
         uChanA += 32*4;
      if( 32*4-1 == uChanB )
         uChanB += 32*4;
   } // Loop on all possible channel combinations

   TCanvas *cChosenRmsRes = new TCanvas("cChosenRmsRes", "RMS and time res for chosen combi, Encoder A only or Encoder B only");
   cChosenRmsRes->Divide(2, 2);
   cChosenRmsRes->cd(1);
   hTimeRmsAllCombiEncA->SetStats(0);
   hTimeRmsAllCombiEncA->Draw("colztext");
   cChosenRmsRes->cd(2);
   hTimeResAllCombiEncA->SetStats(0);
   hTimeResAllCombiEncA->Draw("colztext");
   cChosenRmsRes->cd(3);
   hTimeRmsAllCombiEncB->SetStats(0);
   hTimeRmsAllCombiEncB->Draw("colztext");
   cChosenRmsRes->cd(4);
   hTimeResAllCombiEncB->SetStats(0);
   hTimeResAllCombiEncB->Draw("colztext");
}
