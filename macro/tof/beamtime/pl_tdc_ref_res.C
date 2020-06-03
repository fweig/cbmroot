
void pl_tdc_ref_res(Int_t fNumberOfTDC=34)
{
   // Monitoring of the TDc resolution stability using reference channel
   TCanvas* fOverviewRes; //!
   TH2*     fhResolutionSummary;

   delete gROOT->FindObjectAny( "OverviewRes" );
   delete gROOT->FindObjectAny( "fhResolutionSummary" );
   fOverviewRes        = new TCanvas("OverviewRes","Overview TDc Resolution",48,56,1000,700);
   fhResolutionSummary = new TH2D("fhResolutionSummary", "TDC Resolution summary; TDC #1 []; TDC #2 [] ",
                                   fNumberOfTDC - 1, 0, fNumberOfTDC - 1,
                                   fNumberOfTDC - 1, 1, fNumberOfTDC );
   fhResolutionSummary->SetStats(kFALSE);


   gROOT->cd();
   gPad->SetFillColor(0);

   TH2 *h2;
   TF1 *fitFunc = new TF1( "fitFunc", "gaus", -1000, 1000 );
   for(Int_t iTdc1 = 0; iTdc1 < fNumberOfTDC - 1; iTdc1++)
   {
      // First get the reference comp histogram
      TString hname=Form("tof_trb_RefChComp_b%03d", iTdc1);
      h2=(TH2 *)gROOT->FindObjectAny(hname);
      if (h2!=NULL)
      {
	h2->Draw("colz");
         // Fit a gaussian for each pair
         TObjArray aSlices;
         h2->FitSlicesY( fitFunc, 0, -1, 0, "QNR", &aSlices);

         // Read the obtained sigma and fill it in the 2D summary histogram
         for(Int_t iTdc2 = iTdc1 + 1; iTdc2 < fNumberOfTDC; iTdc2++)
            fhResolutionSummary->Fill(iTdc1, iTdc2, ((TH2*)aSlices[2])->GetBinContent( iTdc2 - iTdc1 ) );
      } // if (h2!=NULL)
      else cout<<"Histogram "<<hname<<" not existing. "<<endl;
   } // for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++)

   fOverviewRes->cd();
   fhResolutionSummary->Draw("colz");
   ( fhResolutionSummary->GetZaxis() )->SetRangeUser(0.0, 100.0);
}
