
void DtStackS1()
{
  TFile* _file0 = TFile::Open("data/EfficiencyHistos.root");

  TH1* phDtPlotRaw;
  TH1* phDtPlotSel;
  if (NULL != _file0) {

    gDirectory->cd("Sts_Raw");

    phDtPlotRaw = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhSetupSortedDtX1Y1X2Y2N1P1"));
    if (NULL == phDtPlotRaw) return;

    gDirectory->cd("../Sts_GeoCuts");
    phDtPlotSel = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhDtBestPairsHodoS1GeoCut"));
    if (NULL == phDtPlotRaw) return;

  }  // if( NULL != _file0 )
  else
    return;

  THStack* stackDt = new THStack("hsDt", "Raw Hodo-S1 dt and selected one");

  phDtPlotRaw->SetLineColor(kBlue);
  stackDt->Add(phDtPlotRaw);

  phDtPlotSel->SetLineColor(kRed);
  stackDt->Add(phDtPlotSel);

  stackDt->Draw("nostack hist");
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
}
