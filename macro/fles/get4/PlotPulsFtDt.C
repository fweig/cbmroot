{
/*
   TCanvas *cFtDtTime= new TCanvas("cFtDtTime", "Time edge FT for big and small dt");
   cFtDtTime->Divide(2, 2);

   cFtDtTime->cd( 1);
   hFtSmallDtFeeA->Draw("colz");

   cFtDtTime->cd( 2);
   hFtSmallDtFeeB->Draw("colz");

   cFtDtTime->cd( 3);
   hFtBigDtFeeA->Draw("colz");

   cFtDtTime->cd( 4);
   hFtBigDtFeeB->Draw("colz");

   TCanvas *cFtDtTot= new TCanvas("cFtDtTot", "Tot edge FT for big and small dt");
   cFtDtTot->Divide(2, 2);

   cFtDtTot->cd( 1);
   hFtTotSmallDtFeeA->Draw("colz");

   cFtDtTot->cd( 2);
   hFtTotSmallDtFeeB->Draw("colz");

   cFtDtTot->cd( 3);
   hFtTotBigDtFeeA->Draw("colz");

   cFtDtTot->cd( 4);
   hFtTotBigDtFeeB->Draw("colz");

   TCanvas *cFtDtPrevTime= new TCanvas("cFtDtPrevTime", "Previous Time edge FT for big and small dt");
   cFtDtPrevTime->Divide(2, 2);

   cFtDtPrevTime->cd( 1);
   hFtPrevSmallDtFeeA->Draw("colz");

   cFtDtPrevTime->cd( 2);
   hFtPrevSmallDtFeeB->Draw("colz");

   cFtDtPrevTime->cd( 3);
   hFtPrevBigDtFeeA->Draw("colz");

   cFtDtPrevTime->cd( 4);
   hFtPrevBigDtFeeB->Draw("colz");

   TCanvas *cFtDtPrevTot= new TCanvas("cFtDtPrevTot", "Previous Tot edge FT for big and small dt");
   cFtDtPrevTot->Divide(2, 2);

   cFtDtPrevTot->cd( 1);
   hFtPrevTotSmallDtFeeA->Draw("colz");

   cFtDtPrevTot->cd( 2);
   hFtPrevTotSmallDtFeeB->Draw("colz");

   cFtDtPrevTot->cd( 3);
   hFtPrevTotBigDtFeeA->Draw("colz");

   cFtDtPrevTot->cd( 4);
   hFtPrevTotBigDtFeeB->Draw("colz");
*/
   TCanvas *cFtDt= new TCanvas("cFtDt", "FT for big and small dt");
   cFtDt->Divide(4, 4);

   cFtDt->cd( 1);
   hFtPrevSmallDtFeeA->Draw("colz");

   cFtDt->cd( 2);
   hFtPrevSmallDtFeeB->Draw("colz");

   cFtDt->cd( 3);
   hFtPrevTotSmallDtFeeA->Draw("colz");

   cFtDt->cd( 4);
   hFtPrevTotSmallDtFeeB->Draw("colz");

   cFtDt->cd( 5);
   hFtPrevBigDtFeeA->Draw("colz");

   cFtDt->cd( 6);
   hFtPrevBigDtFeeB->Draw("colz");

   cFtDt->cd( 7);
   hFtPrevTotBigDtFeeA->Draw("colz");

   cFtDt->cd( 8);
   hFtPrevTotBigDtFeeB->Draw("colz");

   cFtDt->cd( 9);
   hFtSmallDtFeeA->Draw("colz");

   cFtDt->cd(10);
   hFtSmallDtFeeB->Draw("colz");

   cFtDt->cd(11);
   hFtTotSmallDtFeeA->Draw("colz");

   cFtDt->cd(12);
   hFtTotSmallDtFeeB->Draw("colz");

   cFtDt->cd(13);
   hFtBigDtFeeA->Draw("colz");

   cFtDt->cd(14);
   hFtBigDtFeeB->Draw("colz");

   cFtDt->cd(15);
   hFtTotBigDtFeeA->Draw("colz");

   cFtDt->cd(16);
   hFtTotBigDtFeeB->Draw("colz");
}
