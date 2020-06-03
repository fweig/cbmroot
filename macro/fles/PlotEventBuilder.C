{
   TCanvas *cEventBuilder = new TCanvas("cEventBuilder", "EventBuilder");
   cEventBuilder->Divide(2);

   cEventBuilder->cd( 1);
   gPad->SetLogy();
   hMaxMulPerEpochDist->SetLineColor(kBlack);
   hMaxMulPerEpochDist->Draw("");

   cEventBuilder->cd( 2);
   gPad->SetLogy();
   hEventSizeDist->SetLineColor(kBlack);
   hEventSizeDist->Draw("");


   TCanvas *canvasDeb = new TCanvas("canvasDeb", "Multiplicity trigger", 1200, 600);
   canvasDeb->Divide(2, 2);

   canvasDeb->cd( 1);
   gPad->SetLogz();
   hMaxMulEpoch->Draw("colz");

   canvasDeb->cd( 2);
   gPad->SetLogz();
   hBinThrEpoch->Draw("colz");

   canvasDeb->cd( 3);
   gPad->SetLogz();
   hMaxEvtSizeEpoch->Draw("colz");

   canvasDeb->cd( 4);
   gPad->SetLogz();
   hEvtSizeEvo->Draw("colz");

   // Canvas with ProfileY for each of the 4, all plotted in same
   TCanvas *canvasDebProfY = new TCanvas("canvasDebProfY", "Multiplicity trigger", 1200, 600);

   TH1D* projMaxEvtSizeEpoch = hMaxEvtSizeEpoch->ProjectionY();
   projMaxEvtSizeEpoch->Scale( 1.0/1000.0 ); // Weight factor to get the mean value
   projMaxEvtSizeEpoch->SetLineColor(kBlue);
   projMaxEvtSizeEpoch->Draw("");

   TH1D* projMaxMulEpoch = hMaxMulEpoch->ProjectionY();
   projMaxMulEpoch->Scale( 1.0/1000.0 ); // Weight factor to get the mean value
   projMaxMulEpoch->SetLineColor(kBlack);
   projMaxMulEpoch->Draw("same");

   TH1D* projBinThrEpoch = hBinThrEpoch->ProjectionY();
   projBinThrEpoch->Scale( 1.0/1000.0 ); // Weight factor to get the mean value
   projBinThrEpoch->SetLineColor(kRed);
   projBinThrEpoch->Draw("same");

   TProfile* projEvtSizeEvo = hEvtSizeEvo->ProfileY();
//   projEvtSizeEvo->Scale( 1.0/1000.0 ); // Weight factor to get the mean value
   projEvtSizeEvo->SetLineColor(kGreen);
   projEvtSizeEvo->Draw("sameL");
}
