/** @file PlotPulsDist.C
  * @copyright Copyright (C) 2015 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [orginator] **/

{
  TCanvas* cPulserDistNs = new TCanvas("cPulserDistNs", "Pulses time interval, ns range");
  cPulserDistNs->cd();
  hPulserHitDistNs->Draw("colz");

  TCanvas* cPulserDistUs = new TCanvas("cPulserDistUs", "Pulses time interval, us range");
  cPulserDistUs->cd();
  hPulserHitDistUs->Draw("colz");

  TCanvas* cPulserDistMs = new TCanvas("cPulserDistMs", "Pulses time interval, ms range");
  cPulserDistMs->cd();
  hPulserHitDistMs->Draw("colz");
}
