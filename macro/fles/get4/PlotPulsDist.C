{
   TCanvas *cPulserDistNs = new TCanvas("cPulserDistNs", "Pulses time interval, ns range");
   cPulserDistNs->cd( );
   hPulserHitDistNs->Draw("colz");

   TCanvas *cPulserDistUs = new TCanvas("cPulserDistUs", "Pulses time interval, us range");
   cPulserDistUs->cd( );
   hPulserHitDistUs->Draw("colz");

   TCanvas *cPulserDistMs = new TCanvas("cPulserDistMs", "Pulses time interval, ms range");
   cPulserDistMs->cd( );
   hPulserHitDistMs->Draw("colz");

}
