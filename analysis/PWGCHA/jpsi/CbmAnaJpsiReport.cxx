
#include "CbmReportElement.h"
#include "CbmHistManager.h"
#include "CbmDrawHist.h"
#include "CbmUtils.h"
#include "TH1.h"
#include "TF1.h"
#include "TPad.h"
#include "TDirectory.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TMarker.h"
#include <boost/assign/list_of.hpp>
#include <map>
#include <iostream>
#include "CbmAnaJpsiReport.h"
#include "CbmAnaJpsiHist.h"
#include "CbmAnaJpsiCuts.h"
#include "TLine.h"
#include "TStyle.h"
#include "cbm/elid/CbmLitGlobalElectronId.h"

using boost::assign::list_of;
using Cbm::NumberToString;
using Cbm::Split;
using std::map;
using std::endl;
using std::cout;
using std::string;
using std::vector;

CbmAnaJpsiReport::CbmAnaJpsiReport():
		CbmSimulationReport()
{
	SetReportName("jpsi_qa");
}

CbmAnaJpsiReport::~CbmAnaJpsiReport()
{
}

void CbmAnaJpsiReport::Create()
{
   //Out().precision(3);
   Out() << R()->DocumentBegin();
   Out() << R()->Title(0, GetTitle());

   PrintCanvases();

   Out() << R()->DocumentEnd();
}

void CbmAnaJpsiReport::Draw()
{
	  SetDefaultDrawStyle();

	  int nofEvents = H1("fh_event_number")->GetEntries();
	  cout << "Number of events = " << nofEvents << endl;
	  HM()->ScaleByPattern(".*", 1./nofEvents);

	 //Rebin minv histograms
	  Int_t nRebins = 20;
	  //Double_t nRebinScale = 1/((Double_t) nRebins);
	  HM()->RebinByPattern("fh_signal_minv.+", nRebins);  //Rebin scales with 1/(binWith)
	  HM()->RebinByPattern("fh_bg_minv.+", nRebins);
	  HM()->RebinByPattern("fh_pi0_minv.+", nRebins);
	  HM()->RebinByPattern("fh_gamma_minv.+", nRebins);
	  HM()->RebinByPattern("fh_bg_truematch_minv.+", nRebins);
	  HM()->RebinByPattern("fh_bg_truematch_el_minv.+", nRebins);
	  HM()->RebinByPattern("fh_bg_truematch_notel_minv.+", nRebins);
	  HM()->RebinByPattern("fh_bg_mismatch_minv.+", nRebins);
	  HM()->RebinByPattern("fh_bg_participants_minv.+", nRebins);

	  HM()->ScaleByPattern("fh_signal_minv.+", nRebins);
	  HM()->ScaleByPattern("fh_bg_minv.+", nRebins);
	  HM()->ScaleByPattern("fh_pi0_minv.+", nRebins);
	  HM()->ScaleByPattern("fh_gamma_minv.+", nRebins);
	  HM()->ScaleByPattern("fh_bg_truematch_minv.+", nRebins);
	  HM()->ScaleByPattern("fh_bg_truematch_el_minv.+", nRebins);
	  HM()->ScaleByPattern("fh_bg_truematch_notel_minv.+", nRebins);
	  HM()->ScaleByPattern("fh_bg_mismatch_minv.+", nRebins);
	  HM()->ScaleByPattern("fh_bg_participants_minv.+", nRebins);

	  Draw2DCut("fh_rich_pmt_xy");
	  DrawCutDistributions();

	  DrawMinvMismatchesAll();

	  DrawSignalMinv();
	  DrawAnalysisStepsH1("fh_bg_minv",false, 5e-5, 2);
	  DrawAnalysisStepsH1("fh_pi0_minv",false);
	  DrawAnalysisStepsH1("fh_gamma_minv",false);

	  DrawAnalysisStepsH2("fh_signal_minv_pt",false);
	  DrawAnalysisStepsH1("fh_signal_mom",false);
	  DrawAnalysisStepsH2("fh_signal_pty",true);

	  {
		  TCanvas* c = CreateCanvas("jpsi_fh_vertex_el_gamma","jpsi_fh_vertex_el_gamma",1000,1000);
		  c->Divide(2,2);
		  c->cd(1);
	  	  DrawH2(H2("fh_vertex_el_gamma_xz"));
	  	  c->cd(2);
		  DrawH2(H2("fh_vertex_el_gamma_yz"));
		  c->cd(3);
		  DrawH2(H2("fh_vertex_el_gamma_xy"));
	      c->cd(4);
		  DrawH2(H2("fh_vertex_el_gamma_rz"));
	  }

	  { // Number of BG and signal tracks after each cut
	   	  TCanvas* c = CreateCanvas("jpsi_fh_nof_tracks","jpsi_fh_nof_tracks",1000, 500);
	   	  c->Divide(2, 1);
	   	  c->cd(1);
	   	  gPad->SetLogy();
	      DrawH1(H1("fh_nof_bg_tracks"));
	      //H1("fh_nof_bg_tracks")->SetMinimum(0.0);
	      SetAnalysisStepLabels(H1("fh_nof_bg_tracks"));
	      c->cd(2);
	  	  DrawH1(H1("fh_nof_el_tracks"));
	  	  H1("fh_nof_el_tracks")->SetMinimum(0.0);
	  	  SetAnalysisStepLabels(H1("fh_nof_el_tracks"));
	  }

	  { //number of mismatches
	  	  TCanvas* c = CreateCanvas("jpsi_fh_nof_mismatches","jpsi_fh_nof_mismatches",1000,1000);
	  	  c->Divide(2,2);
	  	  c->cd(1);
	  	  DrawH1(H1("fh_nof_mismatches"));
	  	  DrawTextOnPad("All mismatches", 0.15, 0.9, 0.85, 0.99);
	  	  SetAnalysisStepLabels(H1("fh_nof_mismatches"));
	  	  c->cd(2);
	  	  DrawH1(H1("fh_nof_mismatches_rich"));
	  	  DrawTextOnPad("Mismatches in RICH", 0.15, 0.9, 0.85, 0.99);
	  	  SetAnalysisStepLabels(H1("fh_nof_mismatches_rich"));
	  	  c->cd(3);
	  	  DrawH1(H1("fh_nof_mismatches_trd"));
	  	  DrawTextOnPad("mismatches in TRD", 0.15, 0.9, 0.85, 0.99);
	  	  SetAnalysisStepLabels(H1("fh_nof_mismatches_trd"));
	  	  c->cd(4);
	  	  DrawH1(H1("fh_nof_mismatches_tof"));
	  	  DrawTextOnPad("mismatches in TOF", 0.15, 0.9, 0.85, 0.99);
	  	  SetAnalysisStepLabels(H1("fh_nof_mismatches_tof"));
	  }

	   DrawBgSource2D("fh_source_tracks", list_of("#gamma")("#pi^{0}")("#pi^{#pm}")("p")("K")("e^{#pm}_{sec}")("oth."), "Tracks per event");

		  DrawPtYEfficiencyAll();
		  SignalOverBgAllSteps();
		  DrawMinvSAndBgAllSteps();
		  DrawMomEffAllSteps();
		  DrawMomMcVsRec();
		  DrawPairSource();

	 { TCanvas* c = CreateCanvas("jpsi_fh_PdgCode_of Others_BG","jpsi_fh_PdgCode_of Others_BG",1000,1000);
	 	 	 	 c->Divide(2,2);
		 	  	  for (int i=0;i<CbmAnaJpsiHist::fNofAnaSteps-2;i++){
		 	  		  c->cd(i+1);
		 	  		  DrawH1(H1("fh_PdgCode_of Others_BG_" + CbmAnaJpsiHist::fAnaSteps[i+2]));
		 	  		  DrawTextOnPad(CbmAnaJpsiHist::fAnaStepsLatex[i+2], 0.6, 0.92, 0.7, 0.99);
		 	  	  }
	 }
}

void CbmAnaJpsiReport::DrawAnalysisStepsH2(
      const string& hName, bool DoDrawEfficiency)
{
	TCanvas* c = CreateCanvas(("jpsi_"+hName).c_str(), ("jpsi_"+hName).c_str(), 1200, 800);
	c->Divide(3,2);
	for (int i = 0;i< CbmAnaJpsiHist::fNofAnaSteps; i++){
		c->cd(i+1);
		string h = hName+"_"+CbmAnaJpsiHist::fAnaSteps[i];

		DrawH2(H2(h));
		DrawTextOnPad(CbmAnaJpsiHist::fAnaStepsLatex[i], 0.6, 0.89, 0.7, 0.99);

		if (DoDrawEfficiency) DrawEfficiency(h, hName+"_"+CbmAnaJpsiHist::fAnaSteps[kJpsiMc]);
	}
}

void CbmAnaJpsiReport::DrawAnalysisStepsH1(
      const string& hName,
      bool doScale,
	  double min,
	  double max)
{
	CreateCanvas( ("jpsi_" + hName).c_str(), ("jpsi_" + hName).c_str(), 600, 600);
	vector<TH1*> h;
	vector<string> hLegend;
	for (int i = 0; i < CbmAnaJpsiHist::fNofAnaSteps; i++){
		string fullName = hName+"_"+CbmAnaJpsiHist::fAnaSteps[i];
		h.push_back( H1(fullName) );
		h[i]->SetLineWidth(2);
		h[i]->SetLineColor(CbmAnaJpsiHist::fAnaStepsColor[i]);
		if (doScale) h[i]->Scale(1. / h[i]->Integral());
		hLegend.push_back( CbmAnaJpsiHist::fAnaStepsLatex[i] );
		if (min != max) {
			h[i]->SetMinimum(min);
			h[i]->SetMaximum(max);
		}
	}
	DrawH1(h, hLegend, kLinear, kLog, true, 0.90, 0.7, 0.99, 0.99);


}

void CbmAnaJpsiReport::DrawSourceTypesH1(
      const string& hName,
      bool doScale)
{
   vector<TH1*> h;
   vector<string> hLegend;
   for (int i = 0; i < CbmAnaJpsiHist::fNofSourceTypes; i++){
      string fullName = hName+"_"+CbmAnaJpsiHist::fSourceTypes[i];
      h.push_back( H1(fullName) );
      h[i]->SetLineWidth(2);
      h[i]->SetLineColor(CbmAnaJpsiHist::fSourceTypesColor[i]);
      if (doScale){
    	  Double_t integral = h[i]->Integral();
    	  if (integral != 0.0) {
    		  h[i]->Scale(1. / h[i]->Integral());
    	  }
      }
      hLegend.push_back( CbmAnaJpsiHist::fSourceTypesLatex[i] );
   }
   DrawH1(h, hLegend, kLinear, kLog, true, 0.90, 0.7, 0.99, 0.99);
}

void CbmAnaJpsiReport::DrawCutH1(
      const string& hName,
      double cutValue,
	  bool doScale)
{
   CreateCanvas( ("jpsi_" + hName).c_str(), ("jpsi_" + hName).c_str(), 600, 600);
   DrawSourceTypesH1(hName, doScale);
   if (cutValue != -999999.){
      TLine* cutLine = new TLine(cutValue, 0.0, cutValue, 1.);
      cutLine->SetLineWidth(2);
      cutLine->Draw();
   }
}

void CbmAnaJpsiReport::Draw2DCut(
      const string& hist)
{
   TCanvas *c = CreateCanvas(("jpsi_" + hist).c_str(), ("jpsi_" + hist).c_str(), 900, 900);
   c->Divide(2,2);
   for (int i = 0; i < CbmAnaJpsiHist::fNofSourceTypes; i++){
      c->cd(i+1);
      DrawH2(H2( hist + "_"+ CbmAnaJpsiHist::fSourceTypes[i]));
      DrawTextOnPad(CbmAnaJpsiHist::fSourceTypesLatex[i], 0.6, 0.89, 0.7, 0.99);
   }
}

void CbmAnaJpsiReport::DrawCutDistributions()
{
	CbmAnaJpsiCuts cuts;
	cuts.SetDefaultCuts();
	DrawCutH1("fh_track_chi2prim", cuts.fChiPrimCut, false);
	DrawCutH1("fh_track_mom", -999999., false);
	DrawCutH1("fh_track_chi2sts", -999999., false);
	DrawCutH1("fh_track_rapidity", -999999., false);
	DrawCutH1("fh_track_pt", cuts.fPtCut, false);
	DrawCutH1("fh_track_rich_ann", CbmLitGlobalElectronId::GetInstance().GetRichAnnCut(), false);
	DrawCutH1("fh_track_trd_ann", CbmLitGlobalElectronId::GetInstance().GetTrdAnnCut(), false);
	Draw2DCut("fh_track_tof_m2");
}

void CbmAnaJpsiReport::DrawSignalMinv()
{
	double min = 1e-9;
	double max = 3e-8;
	const string hName = "fh_signal_minv";

	CreateCanvas( ("jpsi_" + hName).c_str(), ("jpsi_" + hName).c_str(), 600, 600);
	vector<TH1*> h;
	vector<string> hLegend;
	for (int i = 0; i < CbmAnaJpsiHist::fNofAnaSteps-2; i++){
		string fullName = hName+"_"+CbmAnaJpsiHist::fAnaSteps[i+2];
		h.push_back( (TH1*)H1(fullName)->Clone() );
		cout<<i<< " Integral "<<h[i]->Integral()<<endl;
		cout<<i<< " Entries "<<h[i]->GetEntries()<<endl;
		h[i]->SetAxisRange(2., 4.,"X");
		h[i]->SetLineWidth(2);
		h[i]->SetLineColor(CbmAnaJpsiHist::fAnaStepsColor[i+2]);
		hLegend.push_back( CbmAnaJpsiHist::fAnaStepsLatex[i+2] );
		if (min != max) {
			h[i]->SetMinimum(min);
			h[i]->SetMaximum(max);
		}
	}
	DrawH1(h, hLegend, kLinear, kLog, true, 0.90, 0.7, 0.99, 0.99);

}

void CbmAnaJpsiReport::DrawMinvMismatches(
		int step)
{
	double trueMatch = H1("fh_bg_truematch_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->GetEntries();
	double trueMatchEl = H1("fh_bg_truematch_el_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->GetEntries();
	double trueMatchNotEl = H1("fh_bg_truematch_notel_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->GetEntries();
	double misMatch = H1("fh_bg_mismatch_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->GetEntries();
	double nofBg = H1("fh_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->GetEntries();

	DrawH1(list_of( H1("fh_bg_truematch_minv_" + CbmAnaJpsiHist::fAnaSteps[step]) )
		( H1("fh_bg_truematch_el_minv_" + CbmAnaJpsiHist::fAnaSteps[step]) )
		( H1("fh_bg_truematch_notel_minv_" + CbmAnaJpsiHist::fAnaSteps[step]) )
		( H1("fh_bg_mismatch_minv_" + CbmAnaJpsiHist::fAnaSteps[step]) ),
		list_of("true match (" + Cbm::NumberToString(100. * trueMatch / nofBg, 1) + "%)")
		("true match (e^{#pm}) (" + Cbm::NumberToString(100. * trueMatchEl / nofBg, 1)+ "%)")
		("true match (not e^{#pm}) (" + Cbm::NumberToString(100. * trueMatchNotEl / nofBg, 1)+ "%)")
		("mismatch (" + Cbm::NumberToString(100. * misMatch / nofBg)+ "%)"),
		kLinear, kLog, true, 0.63, 0.8, 0.99, 0.99);

    DrawTextOnPad(CbmAnaJpsiHist::fAnaStepsLatex[step], 0.15, 0.9, 0.35, 0.99);
}

void CbmAnaJpsiReport::DrawMinvMismatchesAll()
{
   Int_t hi = 1;
   TCanvas *c = CreateCanvas("jpsi_fh_minv_mismatches", "jpsi_fh_minv_mismatches", 1000, 1000);
   c->Divide(2,2);
   for (int step = kJpsiReco; step < CbmAnaJpsiHist::fNofAnaSteps; step++){
      c->cd(hi++);
      DrawMinvMismatches(step);
   }

   CreateCanvas("jpsi_fh_minv_mismatches_ptcut", "jpsi_fh_minv_mismatches_ptcut", 600, 600);
   DrawMinvMismatches(kJpsiPtCut);
}


void CbmAnaJpsiReport::DrawEfficiency(
		const string& histName,
		const string& McHistName)
{
	Double_t nofMCEntries =H2(McHistName)->GetEntries();
	if (nofMCEntries!=0){
 	DrawTextOnPad(Cbm::NumberToString((Double_t) H2(histName)->GetEntries() / nofMCEntries*100.)+"%",0.2,0.9,0.35,0.99);
	}
}

void CbmAnaJpsiReport::DrawPtYEfficiency(
		int step)
{
	   TH2D* Efficiency = Cbm::DivideH2(H2("fh_signal_pty_" + CbmAnaJpsiHist::fAnaSteps[step]), H2("fh_signal_pty_" + CbmAnaJpsiHist::fAnaSteps[kJpsiMc]));
	   DrawH2(Efficiency);
	   DrawTextOnPad(CbmAnaJpsiHist::fAnaStepsLatex[step], 0.6, 0.89, 0.7, 0.99);
	   DrawEfficiency("fh_signal_pty_" + CbmAnaJpsiHist::fAnaSteps[step], "fh_signal_pty_" + CbmAnaJpsiHist::fAnaSteps[kJpsiMc]);
}

void CbmAnaJpsiReport::DrawPtYEfficiencyAll()
{

	TCanvas *c = CreateCanvas("jpsi_fh_pty_efficiency", "jpsi_fh_pty_efficiency", 1200, 800);
	   c->Divide(3,2);
	   for (int i = 1; i < CbmAnaJpsiHist::fNofAnaSteps; i++){
	      c->cd(i);
	      DrawPtYEfficiency(i);
	   }

}

void CbmAnaJpsiReport::SetAnalysisStepLabels(
      TH1* h)
{
   h->GetXaxis()->SetLabelSize(0.06);
   int x = 1;
   for (Int_t step = 0; step < CbmAnaJpsiHist::fNofAnaSteps; step++){
      h->GetXaxis()->SetBinLabel(x, CbmAnaJpsiHist::fAnaStepsLatex[step].c_str());
      x++;
   }
}

double CbmAnaJpsiReport::SignalOverBg(
		int step)
{
  TH1D* signal = (TH1D*) H1("fh_signal_minv_"+CbmAnaJpsiHist::fAnaSteps[step]);
  TH1D* bg = (TH1D*) H1("fh_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[step]);

  //Create Histogram for the Gaus-Fit
  TH1D* signalFit = (TH1D*)signal->Clone();
  signalFit->Fit("gaus");

  //Calculate sigma and Mean
  Double_t sigmaSignal = signalFit->GetFunction("gaus")->GetParameter("Sigma");
  Double_t meanSignal = signalFit->GetFunction("gaus")->GetParameter("Mean");

  //Get the number of the Bins of Min and Max
  int signalMin = signal->FindBin(meanSignal - 2.*sigmaSignal);
  int signalMax = signal->FindBin(meanSignal + 2.*sigmaSignal);

  double NOfSignalEntries = 0.;
  double NOfBgEntries = 0.;

  //sum up all the bins
  for (int i=signalMin; i<=signalMax; i++)
  {
	  NOfSignalEntries += signal->GetBinContent(i);
	  NOfBgEntries += bg->GetBinContent(i);
  }

  //Calculate Signal/Background
  if (NOfBgEntries <= 0.)	  {
	  return 0.;
  } else
  {
  double sOverBg = NOfSignalEntries / NOfBgEntries;
  return sOverBg;
  }

}

void CbmAnaJpsiReport::SignalOverBgAllSteps()
{
	CreateCanvas("jpsi_fh_SignalOverBg_allAnaSteps","jpsi_fh_SignalOverBg_allAnaSteps",600,600);
	HM()->Create1<TH1D>("fh_SignalOverBg_allAnaSteps","fh_SignalOverBg_allAnaSteps;AnaSteps;S/Bg",6,0,6);

	for (int i=0; i<CbmAnaJpsiHist::fNofAnaSteps;i++)
	{
		double nstep = i + 0.5;
		double SOverBg = SignalOverBg(i);
		HM()->H1("fh_SignalOverBg_allAnaSteps")->Fill(nstep,SOverBg);
		gPad->SetLogy();
	}

	SetAnalysisStepLabels(H1("fh_SignalOverBg_allAnaSteps"));
	DrawH1(H1("fh_SignalOverBg_allAnaSteps"));
}

void CbmAnaJpsiReport::DrawMinvSAndBg(
		int step)
{
	TH1D* signal = (TH1D*) H1("fh_signal_minv_"+CbmAnaJpsiHist::fAnaSteps[step]);
	TH1D* background = (TH1D*) H1("fh_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[step]);

	//Create new histograms for further steps
	TH1D* s = (TH1D*)signal->Clone();
	TH1D* bg = (TH1D*)background ->Clone();
	TH1D* sbg = (TH1D*) background ->Clone();

	//s->Scale(0.0596);

	sbg->Add(s);
	sbg->SetMinimum(1e-12);

	DrawH1(list_of(sbg)(bg)(s),list_of("")("")(""),kLinear, kLog, false, 0,0,0,0);
	//DrawH1(list_of(sbg)(bg)(s),list_of("Signal And Bg")("Background")("Signal"),kLinear, kLog, true, 0.9, 0.7, 0.99, 0.9);
	s->SetFillColor(kRed);
	s->SetLineColor(kBlack);
	s->SetLineWidth(1);
	s->SetLineStyle(CbmDrawingOptions::MarkerStyle(1));
	bg->SetFillColor(kYellow - 10);
	bg->SetLineColor(kBlack);
	bg->SetLineWidth(2);
	bg->SetLineStyle(CbmDrawingOptions::MarkerStyle(1));
	sbg->SetFillColor(kBlue);
	sbg->SetLineColor(kBlack);
	sbg->SetLineWidth(1);
	sbg->SetLineStyle(CbmDrawingOptions::MarkerStyle(1));
	s->SetMarkerStyle(1);
	bg->SetMarkerStyle(1);
	sbg->SetMarkerStyle(1);


	DrawTextOnPad(CbmAnaJpsiHist::fAnaStepsLatex[step], 0.65, 0.9, 0.85, 1.);
}

void CbmAnaJpsiReport::DrawMinvSAndBgAllSteps()
{
	TCanvas *c1 = CreateCanvas("jpsi_fh_Minv_Signal_and_Bg","jpsi_fh_Minv_Signal_and_Bg",1200,800);

	c1->Divide(3,2);

	for (int i=0; i<CbmAnaJpsiHist::fNofAnaSteps;i++)
	{	c1->cd(i+1);
		DrawMinvSAndBg(i);
	}

	CreateCanvas("jpsi_fh_Minv_Signal_and_Bg_ptCut","jpsi_fh_Minv_Signal_and_Bg_ptCut",600,600);
	DrawMinvSAndBg(kJpsiPtCut);

}

void CbmAnaJpsiReport::DrawMomEffAllSteps()
{
	TH1D* Mc = (TH1D*) H1("fh_track_el_mom_"+CbmAnaJpsiHist::fAnaSteps[kJpsiMc])->Clone();
	TH1D* McEff = Cbm::DivideH1((TH1D*)H1("fh_track_el_mom_"+CbmAnaJpsiHist::fAnaSteps[kJpsiMc])->Clone(), (TH1D*)Mc->Clone());
	McEff->SetMinimum(0.);
	McEff->SetMaximum(115.);
	TH1D* AccEff = Cbm::DivideH1((TH1D*)H1("fh_track_el_mom_"+CbmAnaJpsiHist::fAnaSteps[kJpsiAcc])->Clone(), (TH1D*)Mc->Clone());
	TH1D* RecEff = Cbm::DivideH1((TH1D*)H1("fh_track_el_mom_"+CbmAnaJpsiHist::fAnaSteps[kJpsiReco])->Clone(), (TH1D*)Mc->Clone());
	TH1D* Chi2PrimEff = Cbm::DivideH1((TH1D*)H1("fh_track_el_mom_"+CbmAnaJpsiHist::fAnaSteps[kJpsiChi2Prim])->Clone(), (TH1D*)Mc->Clone());
	TH1D* ElIdEff = Cbm::DivideH1((TH1D*)H1("fh_track_el_mom_"+CbmAnaJpsiHist::fAnaSteps[kJpsiElId])->Clone(), (TH1D*)Mc->Clone());
	TH1D* PtEff = Cbm::DivideH1((TH1D*)H1("fh_track_el_mom_"+CbmAnaJpsiHist::fAnaSteps[kJpsiPtCut])->Clone(), (TH1D*)Mc->Clone());

	CreateCanvas("jpsi_fh_Momentum_Efficiency_AllSteps","jpsi_fh_Momentum_Efficiency_AllSteps",800,800);

	DrawH1(list_of(McEff)(AccEff)(RecEff)(Chi2PrimEff)(ElIdEff)(PtEff),list_of(CbmAnaJpsiHist::fAnaStepsLatex[kJpsiMc])(CbmAnaJpsiHist::fAnaStepsLatex[kJpsiAcc])
			(CbmAnaJpsiHist::fAnaStepsLatex[kJpsiReco])(CbmAnaJpsiHist::fAnaStepsLatex[kJpsiChi2Prim])(CbmAnaJpsiHist::fAnaStepsLatex[kJpsiElId])
			(CbmAnaJpsiHist::fAnaStepsLatex[kJpsiPtCut]), kLinear, kLinear, true, 0.9, 0.6, 0.99, 0.99,"HIST");


	DrawAnalysisStepsH1("fh_track_el_mom",false);
	gPad->SetLogy(false);
}

void CbmAnaJpsiReport::DrawMomMcVsRec()
{
	CreateCanvas("jpsi_fh_Momentum_Mc_Reco","jpsi_fh_Momentum_Mc_Reco",800,800);
	DrawH2(H2("fh_track_el_mom_mc_rec"));
}

void CbmAnaJpsiReport::DrawPairSourceAnaSteps(
		int step)
{	H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_gg")->SetMaximum(1e-2);
	H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_gg")->SetMinimum(1e-6);
	DrawH1(list_of(H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_gg"))
		((TH1*) H1("fh_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Clone())
		(H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_gp"))
		(H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_go"))
		//(H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_pg"))
		(H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_pp"))
		(H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_po"))
		//(H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_og"))
		//(H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_op"))
		(H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_oo")),
		list_of("#gamma + #gamma")("whole BG")("#gamma + #pi^{0}")("#gamma + others")
		/*("#pi^{0} + #gamma")*/("#pi^{0} + #pi^{0}")("#pi^{0} + others")
		/*("others + #gamma")("others + #pi^{0}")*/("others + others"),
		kLinear, kLog, true, 0.85, 0.6, 0.99, 0.99);
}

void CbmAnaJpsiReport::DrawPairSource()
{
	TCanvas* c = CreateCanvas("jpsi_fh_bg_pair_source", "jpsi_fh_bg_pair_source" , 1200, 800);
	c->Divide(2,2);
	for (int i = 0;i< CbmAnaJpsiHist::fNofAnaSteps-2; i++){
		c->cd(i+1);
		DrawPairSourceAnaSteps(i+2);
		DrawTextOnPad(CbmAnaJpsiHist::fAnaStepsLatex[i+2], 0.6, 0.89, 0.7, 0.99);
	}
}

void CbmAnaJpsiReport::DrawBgSource2D(
      const string& histName,
      const vector<string>& yLabels,
      const string& zTitle)
{
	gStyle->SetPaintTextFormat("4.2f");
	CreateCanvas(string("jpsi_" + histName+"_abs").c_str(), string("jpsi_"+histName+"_abs").c_str(), 900, 600);
	TH2D* habs = (TH2D*)H2(histName)->Clone();
	habs->SetStats(false);
	habs->GetZaxis()->SetTitle(zTitle.c_str());
	habs->GetXaxis()->SetRange(kJpsiReco + 1, CbmAnaJpsiHist::fNofAnaSteps);
	habs->SetMarkerSize(1.8);
	DrawH2(habs, kLinear, kLinear, kLog, "text COLZ");

	CreateCanvas(string("jpsi_" + histName+"_percent").c_str(), string("jpsi_" + histName+"_percent").c_str(), 900, 600);
	TH2D* hperc = (TH2D*)H2(histName)->Clone();
	hperc->SetStats(false);
	Int_t nBinsX = hperc->GetNbinsX();
	Int_t nBinsY = hperc->GetNbinsY();
	for (Int_t x = 1; x <= nBinsX; x++){
		// calculate total number of BG tracks (pairs) for a current step
		double nbg = 0.;
		for (Int_t y = 1; y <= nBinsY; y++){
			nbg += habs->GetBinContent(x, y);
		}
		Double_t sc =100. / (nbg);
		for (Int_t y = 1; y <= nBinsY; y++){
			Double_t val = sc * hperc->GetBinContent(x, y);
			hperc->SetBinContent(x, y, val);
		}
	}
	hperc->GetZaxis()->SetTitle("[%]");
	hperc->GetXaxis()->SetLabelSize(0.06);
	hperc->GetYaxis()->SetLabelSize(0.06);
	hperc->SetMarkerColor(kBlack);
	hperc->SetMarkerSize(1.8);
	hperc->GetXaxis()->SetRange(kJpsiReco + 1, CbmAnaJpsiHist::fNofAnaSteps);
	DrawH2(hperc, kLinear, kLinear, kLinear, "text COLZ");

	for (UInt_t y = 1; y <= yLabels.size(); y++){
		hperc->GetYaxis()->SetBinLabel(y, yLabels[y-1].c_str());
		habs->GetYaxis()->SetBinLabel(y, yLabels[y-1].c_str());
	}

	SetAnalysisStepLabels(hperc);
	SetAnalysisStepLabels(habs);
}

ClassImp(CbmAnaJpsiReport)

