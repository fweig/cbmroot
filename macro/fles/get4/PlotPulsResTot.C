
{
   const UInt_t kuNbChanMax  = 32*4 + 32*4 +32*4; // 4 FEE/ROC in hardware + 4 FEE/ROC only in software, 2 ROCS
   const UInt_t kuNbChanTest = 16;
   TCanvas *cTimeResPairsTot = new TCanvas("cTimeResPairsTot", "Time Res. vs Tot of pairs of selected chan");
   cTimeResPairsTot->Divide(8, 4);
   Int_t iPadIndexPairs = 1;
   for(UInt_t uChanA = 0; uChanA < kuNbChanMax && iPadIndexPairs < 2*kuNbChanTest; uChanA++)
      for(UInt_t uChanB = 0; uChanB < kuNbChanMax && iPadIndexPairs < 2*kuNbChanTest; uChanB++)
   {
      cTimeResPairsTot->cd( iPadIndexPairs );
      TH1* hHistoPtr = (TH1*) (gDirectory->FindObjectAny( Form("hTimeResPairsTot_%03u_%03u_A", uChanA, uChanB) ) );
      if( NULL != hHistoPtr )
      {
         gPad->SetLogy();
         hHistoPtr->Draw("colz");
         iPadIndexPairs++;
      } // if( NULL != hHistoPtr )

      cTimeResPairsTot->cd( iPadIndexPairs );
      TH1* hHistoPtr = (TH1*) (gDirectory->FindObjectAny( Form("hTimeResPairsTot_%03u_%03u_B", uChanA, uChanB) ) );
      if( NULL != hHistoPtr )
      {
         gPad->SetLogy();
         hHistoPtr->Draw("colz");
         iPadIndexPairs++;
      } // if( NULL != hHistoPtr )

      // Jump the channels existing only in software
      if( 32*4 -1 == uChanA )
         uChanA += 32*4;
      if( 32*4-1 == uChanB )
         uChanB += 32*4;
   } // Loop on all possible channel combinations

   TCanvas *cTimeResCombTot = new TCanvas("cTimeResCombTot", "Time Res. vs TOT of all Comb. for 4 first selected chan");
   cTimeResCombTot->Divide(4, 3);
   Int_t iPadIndexComb = 1;
   for(UInt_t uChanA = 0; uChanA < kuNbChanMax && iPadIndexComb < 2*7; uChanA++)
      for(UInt_t uChanB = 0; uChanB < kuNbChanMax && iPadIndexComb < 2*7; uChanB++)
   {
      cTimeResCombTot->cd( iPadIndexComb);
      TH1* hHistoPtr = (TH1*) (gDirectory->FindObjectAny( Form("hTimeResCombiTot_%03u_%03u_A", uChanA, uChanB) ) );
      if( NULL != hHistoPtr )
      {
         gPad->SetLogy();
         hHistoPtr->Draw("colz");
         iPadIndexComb++;
      } // if( NULL != hHistoPtr )

      cTimeResCombTot->cd( iPadIndexComb);
      TH1* hHistoPtr = (TH1*) (gDirectory->FindObjectAny( Form("hTimeResCombiTot_%03u_%03u_B", uChanA, uChanB) ) );
      if( NULL != hHistoPtr )
      {
         gPad->SetLogy();
         hHistoPtr->Draw("colz");
         iPadIndexComb++;
      } // if( NULL != hHistoPtr )

      // Jump the channels existing only in software
      if( 32*4-1 == uChanA )
         uChanA += 32*4;
      if( 32*4-1 == uChanB )
         uChanB += 32*4;
   } // Loop on all possible channel combinations
}
