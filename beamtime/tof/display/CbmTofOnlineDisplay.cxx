#include "CbmTofOnlineDisplay.h"

#include "FairLogger.h"

#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TF1.h"
#include "THStack.h"
#include "TLegend.h"
#include "TString.h"
#include "TROOT.h"
#include "TPaveStats.h"
#include "TGraph.h"
#include <TLegendEntry.h>

#include "TofDef.h"
#include "TMbsUnpackTofPar.h"
#include "TMbsCalibTofPar.h"

#include "FairRun.h"
#include "FairRuntimeDb.h"

// ---- Default constructor -------------------------------------------
CbmTofOnlineDisplay::CbmTofOnlineDisplay()
  :FairTask("CbmTofOnlineDisplay"),
   fMbsUnpackPar(NULL),
   fMbsCalibPar(NULL),
   fbMonitorTdcOcc(kTRUE),
   fTdcChannelOccupancy(NULL),
   fNumberOfTDC(30),
   fNumberOfSEB(8),
   fUpdateInterval(100),
   fEventCounter(0),
   fbMonitorCts(kFALSE),
   fCtsTriggerMonitor(NULL),
   fbMonitorSebStatus(kFALSE),
   fSebSizeMonitor(NULL),
   fSebStatusMonitor(NULL),
   fSebDataRateMonitor(NULL),
   fbMonitorTdcStatus(kFALSE),
   fTdcSizeMonitor(NULL),
   fTdcStatusMonitor(NULL),
   fbMonitorFSMockup(kFALSE),
   fFSMockupMonitor(NULL),
   fbMonitorRes(kFALSE),
   fOverviewRes(NULL),
   fhResolutionSummary(NULL),
   fhResolutionRmsSummary(NULL),
   fbMonitorDigiStatus(kFALSE),
   fDigiSizeMonitor(NULL),
   fDigiStatusMonitor(NULL),
   fbMonitorRates(kFALSE),
   fCanvRatesMonitor(NULL),
   fStackMbsTrloA(NULL),
   fStackMbsTrloB(NULL),
   fStackFreeTrloA(NULL),
   fStackFreeTrloB(NULL),
   fStackFreeTrloOutA(NULL),
   fStackFreeTrloOutB(NULL),
   fLegStackMbsTrloA(NULL),
   fLegStackMbsTrloB(NULL),
   fLegStackFreeTrloA(NULL),
   fLegStackFreeTrloB(NULL),
   fLegStackFreeTrloOutA(NULL),
   fLegStackFreeTrloOutB(NULL),
   fbRatesSlidingScale(kFALSE),
   fbMonitorInspection(kFALSE),
   fLeadingEdgeOnly(NULL),
   fTrailingEdgeOnly(NULL),
   fUnequalEdgeCounts(NULL),
   fTimeOverThreshold(NULL),
   fLeadingPosition(NULL),
   fTrailingPosition(NULL),
   fBoardFineTime(NULL),
   fRefChFineTime(NULL),
   fbMonitorCalibration(kFALSE),
   fBoardOffsetGraphs(NULL),
   fBoardOffsetLinear(NULL),
   fBoardOffsetBinCenter(NULL),
   fBoardOffsetBinEdge(NULL)

{
  LOG(debug) << "Default Constructor of CbmTofOnlineDisplay";
  for( Int_t iCh = 0; iCh < 16; iCh++)
   fsFreeTrloNames[iCh] = Form("Ch %02d", iCh);
}

// ---- Destructor ----------------------------------------------------
CbmTofOnlineDisplay::~CbmTofOnlineDisplay()
{
  LOG(debug) << "Destructor of CbmTofOnlineDisplay";
}

// ----  Initialisation  ----------------------------------------------
void CbmTofOnlineDisplay::SetParContainers()
{
  LOG(debug) << "SetParContainers of CbmTofOnlineDisplay";

   InitParameters();
}

void CbmTofOnlineDisplay::SetFreeTrloNames( 
         TString sChA, TString sChB, TString sChC, TString sChD, 
         TString sChE, TString sChF, TString sChG, TString sChH, 
         TString sChI, TString sChJ, TString sChK, TString sChL, 
         TString sChM, TString sChN, TString sChO, TString sChP )
{
   fsFreeTrloNames[ 0] = sChA;
   fsFreeTrloNames[ 1] = sChB;
   fsFreeTrloNames[ 2] = sChC;
   fsFreeTrloNames[ 3] = sChD;
   fsFreeTrloNames[ 4] = sChE;
   fsFreeTrloNames[ 5] = sChF;
   fsFreeTrloNames[ 6] = sChG;
   fsFreeTrloNames[ 7] = sChH;
   fsFreeTrloNames[ 8] = sChI;
   fsFreeTrloNames[ 9] = sChJ;
   fsFreeTrloNames[10] = sChK;
   fsFreeTrloNames[11] = sChL;
   fsFreeTrloNames[12] = sChM;
   fsFreeTrloNames[13] = sChN;
   fsFreeTrloNames[14] = sChO;
   fsFreeTrloNames[15] = sChP;
}

// ---- Init ----------------------------------------------------------
InitStatus CbmTofOnlineDisplay::Init()
{
  LOG(debug) << "Initilization of CbmTofOnlineDisplay";

  fNumberOfTDC = fMbsUnpackPar->GetNbActiveBoards( tofMbs::trbtdc );
  fNumberOfSEB = fMbsUnpackPar->GetActiveTrbSebNb();

  TH1 *h1;
  TH2 *h2;
  Float_t lsize=0.07;

  if( fbMonitorTdcOcc )
  {
    fTdcChannelOccupancy = new TCanvas("tCanvasOccupancy","TRB TDC channel occupancy",8,6,900,600);
    fTdcChannelOccupancy->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
      fTdcChannelOccupancy->cd(iCh+1);
      gROOT->cd();
      TString hname=Form("tof_trb_ch_occ_%03d",iCh);
      h1=(TH1 *)gROOT->FindObjectAny(hname);
      if (h1!=NULL) {
        h1->Draw("");
        gPad->SetFillColor(0);
        gStyle->SetPalette(1);
        gStyle->SetLabelSize(lsize);
        gPad->SetLogy();
      } else {
        LOG(info)<<"Histogram "<<hname<<" not existing. ";
      }
    }
  }

  if( fbMonitorCalibration )
  {
    fBoardOffsetGraphs = new TCanvas("tBoardOffsetGraphs",Form("Reference hit run time difference precision with respect to reference TDC"),0,0,700,700);
    fBoardOffsetGraphs->Divide(2,2);

    fBoardOffsetLinear = new TCanvas("tBoardOffsetLinear",Form("Reference hit run time differences (calibrated linearly) with respect to reference TDC"),0,0,700,700);
    fBoardOffsetLinear->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    fBoardOffsetBinCenter = new TCanvas("tBoardOffsetBinCenter",Form("Reference hit run time differences (calibrated bin-by-bin center) with respect to reference TDC"),0,0,700,700);
    fBoardOffsetBinCenter->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    fBoardOffsetBinEdge = new TCanvas("tBoardOffsetBinEdge",Form("Reference hit run time differences (calibrated bin-by-bin edge) with respect to reference TDC"),0,0,700,700);
    fBoardOffsetBinEdge->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

  }

  if( fbMonitorInspection )
  {
    fLeadingEdgeOnly = new TCanvas("tCanvasLeadingEdgeOnly","Shares of leading edge only TDC channel buffers",8,6,900,600);
    fLeadingEdgeOnly->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++)
    {
      fLeadingEdgeOnly->cd(iCh+1);
      gROOT->cd();
      TString hname=Form("tof_trb_ch_leadonly_%03d",iCh);
      h1=(TH1 *)gROOT->FindObjectAny(hname);
      if (h1!=NULL)
      {
        h1->Draw("");
        gPad->SetFillColor(0);
        gStyle->SetPalette(1);
        gStyle->SetLabelSize(lsize);
      }
      else
      {
        LOG(info)<<"Histogram "<<hname<<" not existing. ";
      }
    }

    fTrailingEdgeOnly = new TCanvas("tCanvasTrailingEdgeOnly","Shares of trailing edge only TDC channel buffers",8,6,900,600);
    fTrailingEdgeOnly->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++)
    {
      fTrailingEdgeOnly->cd(iCh+1);
      gROOT->cd();
      TString hname=Form("tof_trb_ch_trailonly_%03d",iCh);
      h1=(TH1 *)gROOT->FindObjectAny(hname);
      if (h1!=NULL)
      {
        h1->Draw("");
        gPad->SetFillColor(0);
        gStyle->SetPalette(1);
        gStyle->SetLabelSize(lsize);
      }
      else
      {
        LOG(info)<<"Histogram "<<hname<<" not existing. ";
      }
    }

    fUnequalEdgeCounts = new TCanvas("tCanvasUnequalEdgeCounts","Shares of unequal edge TDC channel buffers",8,6,900,600);
    fUnequalEdgeCounts->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++)
    {
      fUnequalEdgeCounts->cd(iCh+1);
      gROOT->cd();
      TString hname=Form("tof_trb_ch_unequal_%03d",iCh);
      h1=(TH1 *)gROOT->FindObjectAny(hname);
      if (h1!=NULL)
      {
        h1->Draw("");
        gPad->SetFillColor(0);
        gStyle->SetPalette(1);
        gStyle->SetLabelSize(lsize);
      }
      else
      {
        LOG(info)<<"Histogram "<<hname<<" not existing. ";
      }
    }

    fTimeOverThreshold = new TCanvas("tCanvasTimeOverThreshold","RAW(!!!) Time over threshold - If negative, change the inversion flag!",8,6,900,600);
    fTimeOverThreshold->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++)
    {
      fTimeOverThreshold->cd(iCh+1);
      gROOT->cd();
      TString hname=Form("tof_trb_board_tot_%03d",iCh);
      h2=(TH2 *)gROOT->FindObjectAny(hname);
      if (h2!=NULL)
      {
        gPad->SetFillColor(0);
        gStyle->SetPalette(1);
        gStyle->SetLabelSize(lsize);
        gPad->SetLogz();
        h2->Draw("colz");

      }
      else
      {
        LOG(info)<<"Histogram "<<hname<<" not existing. ";
      }
    }

    fLeadingPosition = new TCanvas("tCanvasLeadingPosition","Leading edge time to trigger",8,6,900,600);
    fLeadingPosition->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++)
    {
      fLeadingPosition->cd(iCh+1);
      gROOT->cd();
      TString hname=Form("tof_trb_board_lead_pos_%03d",iCh);
      h2=(TH2 *)gROOT->FindObjectAny(hname);
      if (h2!=NULL)
      {
        gPad->SetFillColor(0);
        gStyle->SetPalette(1);
        gStyle->SetLabelSize(lsize);
        gPad->SetLogz();
        h2->Draw("colz");
      }
      else
      {
        LOG(info)<<"Histogram "<<hname<<" not existing. ";
      }
    }

    fTrailingPosition = new TCanvas("tCanvasTrailingPosition","Trailing edge time to trigger",8,6,900,600);
    fTrailingPosition->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++)
    {
      fTrailingPosition->cd(iCh+1);
      gROOT->cd();
      TString hname=Form("tof_trb_board_trail_pos_%03d",iCh);
      h2=(TH2 *)gROOT->FindObjectAny(hname);
      if (h2!=NULL)
      {
        gPad->SetFillColor(0);
        gStyle->SetPalette(1);
        gStyle->SetLabelSize(lsize);
        gPad->SetLogz();
        h2->Draw("colz");
      }
      else
      {
        LOG(info)<<"Histogram "<<hname<<" not existing. ";
      }
    }

    fBoardFineTime = new TCanvas("tCanvasBoardFineTime","TDC channel fine times",8,6,900,600);
    fBoardFineTime->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

    for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++)
    {
      fBoardFineTime->cd(iCh+1);
      gROOT->cd();
      TString hname=Form("tof_trb_board_ft_%03d",iCh);
      h2=(TH2 *)gROOT->FindObjectAny(hname);
      if (h2!=NULL)
      {
        gPad->SetFillColor(0);
        gStyle->SetPalette(1);
        gStyle->SetLabelSize(lsize);
        gPad->SetLogz();
        h2->Draw("colz");
      }
      else
      {
        LOG(info)<<"Histogram "<<hname<<" not existing. ";
      }
    }

    fRefChFineTime = new TCanvas("tCanvasRefChFineTime","TDC reference channel fine times",8,6,400,300);

    gROOT->cd();
    TString hname=Form("tof_trb_all_ref_ft");
    h2=(TH2 *)gROOT->FindObjectAny(hname);
    if (h2!=NULL)
    {
      gPad->SetFillColor(0);
      gStyle->SetPalette(1);
      gStyle->SetLabelSize(lsize);
      gPad->SetLogz();
      h2->Draw("colz");
    }
    else
    {
      LOG(info)<<"Histogram "<<hname<<" not existing. ";
    }

  }


  if( fbMonitorCts )
  {
    fCtsTriggerMonitor = new TCanvas("tCanvasTrigger","TRB CTS trigger monitor",1080,0,1000,750);
    fCtsTriggerMonitor->Divide(4,3);

    fCtsTriggerMonitor->cd(1);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_trigger_pattern"));
    if( h1 )
    {
      h1->Draw("");
    }

    fCtsTriggerMonitor->cd(2);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_trigger_types"));
    if( h1 )
    {
      h1->Draw("");
    }

    fCtsTriggerMonitor->cd(3);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_cts_busy"));
    if( h1 )
    {
      h1->Draw("");
    }

    fCtsTriggerMonitor->cd(4);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_cts_idle"));
    if( h1 )
    {
      h1->Draw("");
    }

    fCtsTriggerMonitor->cd(5);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_cts_time_spill"));
    if( h1 )
    {
      h1->Draw("");
    }

    fCtsTriggerMonitor->cd(6);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_itc_assert"));
    if( h1 )
    {
      h1->Draw("");
    }

    fCtsTriggerMonitor->cd(7);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_cts_events"));
    if( h1 )
    {
      h1->Draw("");
    }

    fCtsTriggerMonitor->cd(8);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_cts_cycle"));
    if( h1 )
    {
      h1->Draw("");
    }

    fCtsTriggerMonitor->cd(9);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_trigger_rate"));
    if( h1 )
    {
      h1->Draw("LP");
    }
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_event_rate"));
    if( h1 )
    {
      h1->Draw("LPSAME");
    }

    fCtsTriggerMonitor->cd(10);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_hadaq_rate"));
    if( h1 )
    {
      h1->Draw("LP");
    }
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_event_rate"));
    if( h1 )
    {
      h1->Draw("LPSAME");
    }

    fCtsTriggerMonitor->cd(11);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_cts_idle_spill"));
    if( h1 )
    {
      h1->Draw("");
    }

    fCtsTriggerMonitor->cd(12);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_cts_spill_length"));
    if( h1 )
    {
      h1->Draw("");
    }

  }

  if( fbMonitorSebStatus )
  {
	fSebSizeMonitor = new TCanvas("tCanvasSebSize","TRB subevent sizes",1080,36,500,100);
	fSebSizeMonitor->Divide(8,(fNumberOfSEB % 8 ? fNumberOfSEB/8+1 : fNumberOfSEB/8));

	for(Int_t iCh=0; iCh<fNumberOfSEB; iCh++){
      fSebSizeMonitor->cd(iCh+1);
	  gROOT->cd();
	  TString hname=Form("tof_trb_size_subevent_%03d",iCh);
	  h1=(TH1 *)gROOT->FindObjectAny(hname);
	  if (h1!=NULL) {
	    h1->Draw("");
	    gPad->SetFillColor(0);
	    gStyle->SetPalette(1);
	    gStyle->SetLabelSize(lsize);
	    gPad->SetLogy();
	  } else {
	    LOG(info)<<"Histogram "<<hname<<" not existing. ";
	  }
    }

	fSebStatusMonitor = new TCanvas("tCanvasSebStatus","TrbNet subevent status bits",1080,56,500,100);
	fSebStatusMonitor->Divide(8,(fNumberOfSEB % 8 ? fNumberOfSEB/8+1 : fNumberOfSEB/8));

	for(Int_t iCh=0; iCh<fNumberOfSEB; iCh++){
      fSebStatusMonitor->cd(iCh+1);
	  gROOT->cd();
	  TString hname=Form("tof_trb_status_subevent_%03d",iCh);
	  h1=(TH1 *)gROOT->FindObjectAny(hname);
	  if (h1!=NULL) {
	    h1->Draw("");
	    gPad->SetFillColor(0);
	    gStyle->SetPalette(1);
	    gStyle->SetLabelSize(lsize);
	    gPad->SetLogy();
	  } else {
	    LOG(info)<<"Histogram "<<hname<<" not existing. ";
	  }
    }

	fSebDataRateMonitor = new TCanvas("tCanvasSebDataRate","TRB subevent data rate",1080,36,500,100);
	fSebDataRateMonitor->Divide(8,(fNumberOfSEB % 8 ? fNumberOfSEB/8+1 : fNumberOfSEB/8));

	for(Int_t iCh=0; iCh<fNumberOfSEB; iCh++){
      fSebDataRateMonitor->cd(iCh+1);
	  gROOT->cd();
	  TString hname=Form("tof_trb_rate_subevent_%03d",iCh);
	  h2=(TH2 *)gROOT->FindObjectAny(hname);
	  if (h2!=NULL) {
            TProfile* tProfile = h2->ProfileX();
	    tProfile->Draw("");
	    gPad->SetFillColor(0);
	    gStyle->SetLabelSize(lsize);
	  } else {
	    LOG(info)<<"Histogram "<<hname<<" not existing. ";
	  }
    }
  }

  if( fbMonitorTdcStatus )
  {
	fTdcSizeMonitor = new TCanvas("tCanvasTdcSize","TDC data payload",980,76,500,300);
	fTdcSizeMonitor->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

	for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
      fTdcSizeMonitor->cd(iCh+1);
	  gROOT->cd();
	  TString hname=Form("tof_trb_words_tdc_%03d",iCh);
	  h1=(TH1 *)gROOT->FindObjectAny(hname);
	  if (h1!=NULL) {
	    h1->Draw("");
	    gPad->SetFillColor(0);
	    gStyle->SetPalette(1);
	    gStyle->SetLabelSize(lsize);
	    gPad->SetLogy();
	  } else {
	    LOG(info)<<"Histogram "<<hname<<" not existing. ";
	  }
    }

	fTdcStatusMonitor = new TCanvas("tCanvasTdcStatus","TDC processing status",48,56,1000,700);
	fTdcStatusMonitor->Divide(8,(fNumberOfTDC % 8 ? fNumberOfTDC/8+1 : fNumberOfTDC/8));

	for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
      fTdcStatusMonitor->cd(iCh+1);
	  gROOT->cd();
	  TString hname=Form("tof_trb_process_status_tdc_%03d",iCh);
	  h1=(TH1 *)gROOT->FindObjectAny(hname);
	  if (h1!=NULL) {
	    h1->Draw("");
	    gPad->SetFillColor(0);
	    gStyle->SetPalette(1);
	    gStyle->SetLabelSize(lsize);
	    gPad->SetLogy();
	  } else {
	    LOG(info)<<"Histogram "<<hname<<" not existing. ";
	  }
    }
  }

  if( fbMonitorFSMockup )
  {
	fFSMockupMonitor = new TCanvas("tCanvasMockup","TRB free-running mock-up monitor",1080,0,500,400);
	fFSMockupMonitor->Divide(2,2);

	fFSMockupMonitor->cd(1);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_event_jump"));
    if( h1 )
    {
      h1->Draw("");
      gPad->Update();
      TPaveStats* tStats = (TPaveStats*)h1->FindObject("stats");
      tStats->SetOptStat(111111);
      gPad->Update();
    }

    fFSMockupMonitor->cd(2);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    gPad->SetLogy();
    h1 = ((TH1 *)gROOT->FindObjectAny("tof_trb_buffer_overflow"));
    if( h1 )
    {
      h1->Draw("");
    }

    fFSMockupMonitor->cd(3);
    gROOT->cd();
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetLabelSize(lsize);
    h1 = ((TH2 *)gROOT->FindObjectAny("tof_trb_skips_spill"));
    if( h1 )
    {
      h1->Draw("colz");
    }

  }

//  fbMonitorDigiStatus=kTRUE;
  if( fbMonitorDigiStatus )
  {
	fDigiStatusMonitor = new TCanvas("tCanvasDigiSize","Digi status",1000,500,700,700);
	fDigiStatusMonitor->Divide(4,5);
	Int_t iType[9]={1,3,4,6,7,9,2,5,8};
	Int_t iNumSm[9]={1,1,1,1,1,1,4,3,1};
	Int_t iNumRpc[9]={1,1,1,1,1,1,1,1,6};
	Int_t iCanv=0;
	Int_t iNDet=9;
	for(Int_t iCh=0; iCh<iNDet; iCh++){
	  for(Int_t iSm=0; iSm<iNumSm[iCh];iSm++){
	    for(Int_t iRpc=0; iRpc<iNumRpc[iCh];iRpc++){
	      fDigiStatusMonitor->cd(iCanv+1); iCanv++;
	      gROOT->cd();
	      TString hname=Form("cl_SmT%01d_sm%03d_rpc%03d_DigiCor",iType[iCh],iSm,iRpc);
	      h2=(TH2 *)gROOT->FindObjectAny(hname);
	      if (h2!=NULL) {
            h2->Draw("colz");
            //gPad->SetLogz();
	      } else {
            LOG(info)<<"Histogram "<<hname<<" not existing. ";
            fbMonitorDigiStatus=kFALSE;
	      }
	    }
	  }
	}
  }

  if( kTRUE == fbMonitorRes )
  {
     fOverviewRes        = new TCanvas("OverviewRes","Overview TDC Resolution (RMS and Fit)",48,56,1000,700);
     fOverviewRes->Divide(2);
     fhResolutionSummary = new TH2D("fhResolutionSummary", "TDC Resolution summary (Fit); TDC #1 []; TDC #2 [] ",
                                      fNumberOfTDC - 1, 0, fNumberOfTDC - 1,
                                      fNumberOfTDC - 1, 1, fNumberOfTDC );
     fhResolutionSummary->SetStats(kFALSE);

     fhResolutionRmsSummary = new TH2D("fhResolutionRmsSummary", "TDC Resolution summary (RMS); TDC #1 []; TDC #2 [] ",
                                      fNumberOfTDC - 1, 0, fNumberOfTDC - 1,
                                      fNumberOfTDC - 1, 1, fNumberOfTDC );
     fhResolutionRmsSummary->SetStats(kFALSE);
  } // if( kTRUE == fbMonitorRes )

  if( kTRUE == fbMonitorRates )
  {
     fCanvRatesMonitor        = new TCanvas("OverviewRates","Overview TrigLog scaler rates (MBS and free)",48,56,1000,700);
     fCanvRatesMonitor->Divide(3, 2);
     fStackMbsTrloA     = new THStack("fStackMbsTrloA", 
                                           "Evolution of MBS scalers 1- 8 rates; Time since start [s]; Rate [Hz] ");
     fStackMbsTrloB     = new THStack("fStackMbsTrloB", 
                                           "Evolution of MBS scalers 9-16 rates; Time since start [s]; Rate [Hz] ");
     fStackFreeTrloA    = new THStack("fStackFreeTrloA", 
                                           "Evolution of Free scalers 1- 8 rates; Time since start [s]; Rate [Hz] ");
     fStackFreeTrloB    = new THStack("fStackFreeTrloB", 
                                           "Evolution of Free scalers 9-16 rates; Time since start [s]; Rate [Hz] ");
     fStackFreeTrloOutA = new THStack("fStackFreeTrloOutA", 
                                           "Evolution of Free output scalers 1- 8 counts; Time since start [s]; Counts [] ");
     fStackFreeTrloOutB = new THStack("fStackFreeTrloOutB", 
                                           "Evolution of Free output scalers 9-16 counts; Time since start [s]; Counts [] ");
     fLegStackMbsTrloA     = new TLegend(0.80,0.5,0.99,0.9);
     fLegStackMbsTrloB     = new TLegend(0.80,0.5,0.99,0.9);
     fLegStackFreeTrloA    = new TLegend(0.80,0.5,0.99,0.9);
     fLegStackFreeTrloB    = new TLegend(0.80,0.5,0.99,0.9);
     fLegStackFreeTrloOutA = new TLegend(0.80,0.5,0.99,0.9);
     fLegStackFreeTrloOutB = new TLegend(0.80,0.5,0.99,0.9);

     TProfile * pTemp = NULL;
     Int_t      iColorsArr[8] = { kRed,       kBlue,  kGreen+3,  kOrange+1, 
                                  kMagenta+1, kBlack, kViolet-6, kCyan+2};
     gROOT->cd();
     for( Int_t iCh = 0; iCh < 16; iCh++)
     {
	     TString hname;
        if( iCh < 8 )
        {
           hname=Form("tof_cal_triglog_00_scal_evo_00_%02d",iCh);
           pTemp=(TProfile *)gROOT->FindObjectAny(hname);
           if( NULL != pTemp) 
           {
               pTemp->SetLineColor( iColorsArr[iCh] );
               fStackMbsTrloA->Add(pTemp);
               fLegStackMbsTrloA->AddEntry(pTemp,Form("Ch %02d", iCh),"l");
           } // if( NULL != pTemp) 

           hname=Form("tof_cal_trigscal_07_scal_evo_00_%02d",iCh);
           pTemp=(TProfile *)gROOT->FindObjectAny(hname);
           if( NULL != pTemp) 
           {
               pTemp->SetLineColor( iColorsArr[iCh] );
               fStackFreeTrloA->Add(pTemp);
//               fLegStackFreeTrloA->AddEntry(pTemp,Form("Ch %02d", iCh),"l");
               fLegStackFreeTrloA->AddEntry(pTemp,fsFreeTrloNames[iCh],"l");
           } // if( NULL != pTemp)

           hname=Form("tof_trigscal_00_scalers_evo_01_%02d",iCh);
           h1=(TH1 *)gROOT->FindObjectAny(hname);
           if( NULL != h1) 
           {
               h1->SetLineColor( iColorsArr[iCh] );
               fStackFreeTrloOutA->Add(h1);
               fLegStackFreeTrloOutA->AddEntry(h1,Form("LmuO %01d", iCh),"l");
//               fLegStackFreeTrloOutA->AddEntry(pTemp,fsFreeTrloNames[iCh],"l");
           } // if( NULL != pTemp)
        } // if( iCh < 8 )
        else
        {
           hname=Form("tof_cal_triglog_00_scal_evo_00_%02d",iCh);
           pTemp=(TProfile *)gROOT->FindObjectAny(hname);
           if( NULL != pTemp) 
           {
               pTemp->SetLineColor( iColorsArr[iCh - 8] );
               fStackMbsTrloB->Add(pTemp);
               fLegStackMbsTrloB->AddEntry(pTemp,Form("Ch %02d", iCh),"l");
           } // if( NULL != pTemp) 

           hname=Form("tof_cal_trigscal_07_scal_evo_00_%02d",iCh);
           pTemp=(TProfile *)gROOT->FindObjectAny(hname);
           if( NULL != pTemp) 
           {
               pTemp->SetLineColor( iColorsArr[iCh - 8] );
               fStackFreeTrloB->Add(pTemp);
//               fLegStackFreeTrloB->AddEntry(pTemp,Form("Ch %02d", iCh),"l");
               fLegStackFreeTrloB->AddEntry(pTemp,fsFreeTrloNames[iCh],"l");
           } // if( NULL != pTemp)

           hname=Form("tof_trigscal_00_scalers_evo_01_%02d",iCh);
           h1=(TH1 *)gROOT->FindObjectAny(hname);
           if( NULL != h1) 
           {
               h1->SetLineColor( iColorsArr[iCh - 8] );
               fStackFreeTrloOutB->Add(h1);
               fLegStackFreeTrloOutB->AddEntry(h1,Form("LmuO %02d", iCh),"l");
//               fLegStackFreeTrloOutB->AddEntry(pTemp,fsFreeTrloNames[iCh],"l");
           } // if( NULL != pTemp)

        } // else of if( iCh < 8 )
     } // for( Int_t iCh = 0; iCh < 16; iCh++)

     fCanvRatesMonitor->cd(1);
     fStackMbsTrloA->Draw("nostack");
     fLegStackMbsTrloA->Draw();

     fCanvRatesMonitor->cd(2);
     fStackFreeTrloA->Draw("nostack");
     fLegStackFreeTrloA->Draw();

     fCanvRatesMonitor->cd(3);
     fStackFreeTrloOutA->Draw("nostack");
     fLegStackFreeTrloOutB->Draw();

     fCanvRatesMonitor->cd(4);
     fStackMbsTrloB->Draw("nostack");
     fLegStackMbsTrloB->Draw();

     fCanvRatesMonitor->cd(5);
     fStackFreeTrloB->Draw("nostack");
     fLegStackFreeTrloB->Draw();

     fCanvRatesMonitor->cd(6);
     fStackFreeTrloOutB->Draw("nostack");
     fLegStackFreeTrloOutB->Draw();
  } // if( kTRUE == fbMonitorRes )


  return kSUCCESS;

}

// ---- ReInit  -------------------------------------------------------
InitStatus CbmTofOnlineDisplay::ReInit()
{
  LOG(debug) << "Initilization of CbmTofOnlineDisplay";
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void CbmTofOnlineDisplay::Exec(Option_t* /*option*/)
{

  fEventCounter++;

  if ( 0 == fEventCounter%fUpdateInterval ) {
     LOG(debug)<<"Update Canvas for Event "<< fEventCounter;

     if( fbMonitorTdcOcc )
     {
        for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
          fTdcChannelOccupancy->cd(iCh+1);
          gPad->Modified();
          gPad->Update();
        }
        fTdcChannelOccupancy->Modified();
        fTdcChannelOccupancy->Update();
     }

     if( fbMonitorInspection )
     {
       for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
    	 fLeadingEdgeOnly->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fLeadingEdgeOnly->Modified();
       fLeadingEdgeOnly->Update();

       for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
    	 fTrailingEdgeOnly->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fTrailingEdgeOnly->Modified();
       fTrailingEdgeOnly->Update();

       for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
    	 fUnequalEdgeCounts->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fUnequalEdgeCounts->Modified();
       fUnequalEdgeCounts->Update();

       for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
    	 fTimeOverThreshold->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fTimeOverThreshold->Modified();
       fTimeOverThreshold->Update();

       for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
    	 fLeadingPosition->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fLeadingPosition->Modified();
       fLeadingPosition->Update();

       for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
    	 fTrailingPosition->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fTrailingPosition->Modified();
       fTrailingPosition->Update();

       for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
    	 fBoardFineTime->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fBoardFineTime->Modified();
       fBoardFineTime->Update();

       fRefChFineTime->Modified();
       fRefChFineTime->Update();

     }

     if( fbMonitorCts )
     {
       fCtsTriggerMonitor->cd(1);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(2);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(3);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(4);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(5);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(6);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(7);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(8);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(9);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(10);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(11);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->cd(12);
       gPad->Modified();
       gPad->Update();

       fCtsTriggerMonitor->Modified();
       fCtsTriggerMonitor->Update();
     }

     if( fbMonitorSebStatus )
     {
       for(Int_t iCh=0; iCh<fNumberOfSEB; iCh++){
    	 fSebSizeMonitor->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fSebSizeMonitor->Modified();
       fSebSizeMonitor->Update();

       for(Int_t iCh=0; iCh<fNumberOfSEB; iCh++){
    	 fSebStatusMonitor->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fSebStatusMonitor->Modified();
       fSebStatusMonitor->Update();

       for(Int_t iCh=0; iCh<fNumberOfSEB; iCh++){
    	 fSebDataRateMonitor->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fSebDataRateMonitor->Modified();
       fSebDataRateMonitor->Update();
     }

     if( fbMonitorTdcStatus )
     {
       for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
    	 fTdcSizeMonitor->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fTdcSizeMonitor->Modified();
       fTdcSizeMonitor->Update();

       for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++){
    	 fTdcStatusMonitor->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }

       fTdcStatusMonitor->Modified();
       fTdcStatusMonitor->Update();
     }

     if( fbMonitorFSMockup )
     {
       fFSMockupMonitor->cd(1);
       gPad->Modified();
       gPad->Update();

       fFSMockupMonitor->cd(2);
       gPad->Modified();
       gPad->Update();

       fFSMockupMonitor->cd(3);
       gPad->Modified();
       gPad->Update();

       fFSMockupMonitor->Modified();
       fFSMockupMonitor->Update();
     }

     if( kTRUE == fbMonitorRes )
     {
        fhResolutionSummary->Reset();
        fhResolutionRmsSummary->Reset();

        TH2 *h2;
//        TProfile *pTemp;
        TH1 *hTemp;
        TF1 *fitFunc;
        for(Int_t iTdc1 = 0; iTdc1 < fNumberOfTDC - 1; iTdc1++)
        {
           // First get the reference comp histogram
           TString hname=Form("tof_trb_RefChComp_b%03d", iTdc1);
           h2=(TH2 *)gROOT->FindObjectAny(hname);
           if (h2!=NULL)
           {
              if( 0 < h2->GetEntries() )
              {
                 for(Int_t iTdc2 = iTdc1 + 1; iTdc2 < fNumberOfTDC; iTdc2++)
                 {
                     hTemp = h2->ProjectionY("hTemp", iTdc2 - iTdc1, iTdc2 - iTdc1 ); // s to get RMS instead of standard error

                     if( 0 < hTemp->GetEntries() && 0 < hTemp->Integral() )
                     {
                        // Fill the RMS plot
                        fhResolutionRmsSummary->Fill(iTdc1, iTdc2, hTemp->GetRMS() );

                        // Fit and fill the Sigma plot
                        Int_t iBinPeak = hTemp->GetMaximumBin();
                        fitFunc = new TF1( Form("fitFunc_%03d_%03d",iTdc1, iTdc2), "gaus",
                                       hTemp->GetBinCenter(iBinPeak) - 1000 ,
                                       hTemp->GetBinCenter(iBinPeak) + 1000);
                        hTemp->Fit( Form("fitFunc_%03d_%03d",iTdc1, iTdc2), "QRM0");
                        fhResolutionSummary->Fill(iTdc1, iTdc2, fitFunc->GetParameter(2) );

                        delete hTemp;
                        if( NULL != gROOT->FindObjectAny( Form("fitFunc_%03d_%03d",iTdc1, iTdc2) ) )
                           delete gROOT->FindObjectAny( Form("fitFunc_%03d_%03d",iTdc1, iTdc2) ) ;
                     } // if( 0 < hTemp->GetEntries() )
                 } // for(Int_t iTdc2 = iTdc1 + 1; iTdc2 < fNumberOfTDC; iTdc2++)
               } // if( 0 < h2->GetEntries() )
           } // if (h2!=NULL)
               else LOG(info)<<"Histogram "<<hname<<" not existing. ";
        } // for(Int_t iCh=0; iCh<fNumberOfTDC; iCh++)
        fOverviewRes->cd(1);
        fhResolutionRmsSummary->Draw("colz");
        ( fhResolutionRmsSummary->GetZaxis() )->SetRangeUser(10.0, 50.0);

        fOverviewRes->cd(2);
        fhResolutionSummary->Draw("colz");
        ( fhResolutionSummary->GetZaxis() )->SetRangeUser(10.0, 50.0);

        fOverviewRes->Modified();
        fOverviewRes->Update();
     } // if( kTRUE == fbMonitorRes )

     if( fbMonitorDigiStatus )
     {
       for(Int_t iCh=0; iCh<20; iCh++){
    	 fDigiStatusMonitor->cd(iCh+1);
         gPad->Modified();
         gPad->Update();
       }
     }
     if( kTRUE == fbMonitorRates )
     {
        if( kTRUE == fbRatesSlidingScale )
        {
           TProfile *pTemp = (TProfile *)gROOT->FindObjectAny("tof_cal_triglog_00_scal_evo_00_15");
           Double_t dTimeEnd = pTemp->GetBinCenter( pTemp->FindLastBinAbove( 0.0 ) );
           if( 100.0 < dTimeEnd )
           {
              (fStackMbsTrloA->GetXaxis() )->SetRangeUser( dTimeEnd - 100.0, dTimeEnd + 10 );
              (fStackMbsTrloB->GetXaxis() )->SetRangeUser( dTimeEnd - 100.0, dTimeEnd + 10 );
              (fStackFreeTrloA->GetXaxis() )->SetRangeUser( dTimeEnd - 100.0, dTimeEnd + 10 );
              (fStackFreeTrloB->GetXaxis() )->SetRangeUser( dTimeEnd - 100.0, dTimeEnd + 10 );
              (fStackFreeTrloOutA->GetXaxis() )->SetRangeUser( dTimeEnd - 100.0, dTimeEnd + 10 );
              (fStackFreeTrloOutB->GetXaxis() )->SetRangeUser( dTimeEnd - 100.0, dTimeEnd + 10 );
           } // if( 100.0 < dTimeEnd )
              else
              {
                 (fStackMbsTrloA->GetXaxis() )->SetRangeUser( 0.0, dTimeEnd + 10 );
                 (fStackMbsTrloB->GetXaxis() )->SetRangeUser( 0.0, dTimeEnd + 10 );
                 (fStackFreeTrloA->GetXaxis() )->SetRangeUser( 0.0, dTimeEnd + 10 );
                 (fStackFreeTrloB->GetXaxis() )->SetRangeUser( 0.0, dTimeEnd + 10 );
                 (fStackFreeTrloOutA->GetXaxis() )->SetRangeUser( 0.0, dTimeEnd + 10 );
                 (fStackFreeTrloOutB->GetXaxis() )->SetRangeUser( 0.0, dTimeEnd + 10 );
              } // else of if( 100.0 < dTimeEnd )
        } // if( kTRUE == fbRatesSlidingScale )

        fCanvRatesMonitor->cd(1);
        gPad->SetLogy();
        gPad->SetGridx();
        gPad->SetGridy();
        gPad->Modified();
        gPad->Update();

        fCanvRatesMonitor->cd(2);
        gPad->SetLogy();
        gPad->SetGridx();
        gPad->SetGridy();
        gPad->Modified();
        gPad->Update();

        fCanvRatesMonitor->cd(3);
        gPad->SetLogy();
        gPad->SetGridx();
        gPad->SetGridy();
        gPad->Modified();
        gPad->Update();

        fCanvRatesMonitor->cd(4);
        gPad->SetLogy();
        gPad->SetGridx();
        gPad->SetGridy();
        gPad->Modified();
        gPad->Update();

        fCanvRatesMonitor->cd(5);
        gPad->SetLogy();
        gPad->SetGridx();
        gPad->SetGridy();
        gPad->Modified();
        gPad->Update();

        fCanvRatesMonitor->cd(6);
//        gPad->SetLogy();
        gPad->SetGridx();
        gPad->SetGridy();
        gPad->Modified();
        gPad->Update();

     } // if( kTRUE == fbMonitorRes )
  } 
}

// ---- Finish --------------------------------------------------------
void CbmTofOnlineDisplay::Finish()
{
  LOG(info)<<"Finish of CbmTofOnlineDisplay";

  if( fbMonitorTdcOcc )
   fTdcChannelOccupancy->Update();

  if( fbMonitorCalibration )
  {
    TLegendEntry* tEntry;

    TGraph* tGraphRmsLinear = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphRmsLinear"));
    TGraph* tGraphSigmaLinear = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphSigmaLinear"));
    TGraph* tGraphRmsBinCenter = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphRmsBinCenter"));
    TGraph* tGraphSigmaBinCenter = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphSigmaBinCenter"));
    TGraph* tGraphRmsBinEdge = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphRmsBinEdge"));
    TGraph* tGraphSigmaBinEdge = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphSigmaBinEdge"));

    TGraph* tGraphHistoMeanLinear = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphHistoMeanLinear"));
    TGraph* tGraphGausMeanLinear = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphGausMeanLinear"));
    TGraph* tGraphHistoMeanBinCenter = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphHistoMeanBinCenter"));
    TGraph* tGraphGausMeanBinCenter = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphGausMeanBinCenter"));
    TGraph* tGraphHistoMeanBinEdge = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphHistoMeanBinEdge"));
    TGraph* tGraphGausMeanBinEdge = dynamic_cast<TGraph*>(gROOT->FindObjectAny("tGraphGausMeanBinEdge"));

    // Workaround to prevent (not understood) warning messages like
    //   Warning in <TROOT::Append>: Replacing existing TH1: tGraphRmsBinCenter (Potential memory leak).
    // from popping up when multiple named TGraph objects added to the list of gROOT memory objects are drawn
    // onto the same pad.
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    gROOT->GetList()->Remove(tGraphRmsLinear);
    gROOT->GetList()->Remove(tGraphSigmaLinear);
    gROOT->GetList()->Remove(tGraphRmsBinCenter);
    gROOT->GetList()->Remove(tGraphSigmaBinCenter);
    gROOT->GetList()->Remove(tGraphRmsBinEdge);
    gROOT->GetList()->Remove(tGraphSigmaBinEdge);

    gROOT->GetList()->Remove(tGraphHistoMeanLinear);
    gROOT->GetList()->Remove(tGraphGausMeanLinear);
    gROOT->GetList()->Remove(tGraphHistoMeanBinCenter);
    gROOT->GetList()->Remove(tGraphGausMeanBinCenter);
    gROOT->GetList()->Remove(tGraphHistoMeanBinEdge);
    gROOT->GetList()->Remove(tGraphGausMeanBinEdge);
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    if(tGraphRmsLinear && tGraphRmsBinCenter && tGraphRmsBinEdge)
    {
      fBoardOffsetGraphs->cd(1);
      gPad->SetGridx();
      gPad->SetGridy();
      tGraphRmsLinear->SetMarkerColor(1);
      tGraphRmsBinCenter->SetMarkerColor(4);
      tGraphRmsBinEdge->SetMarkerColor(2);
      tGraphRmsLinear->SetMarkerStyle(20);
      tGraphRmsBinCenter->SetMarkerStyle(21);
      tGraphRmsBinEdge->SetMarkerStyle(22);
      tGraphRmsLinear->GetXaxis()->SetTitle("TDC index []");
      tGraphRmsLinear->GetYaxis()->SetTitle("RMS [ps]");
      tGraphRmsLinear->Draw("AP");
      tGraphRmsBinCenter->Draw("P");
      tGraphRmsBinEdge->Draw("P");

      TLegend* tLegend41 = new TLegend(gPad->GetLeftMargin()+0.05,gPad->GetBottomMargin()+0.05,1.-gPad->GetRightMargin()-0.05,0.5);
      tLegend41->SetName("tLegendRms");
      tLegend41->SetHeader(Form("%d events",fEventCounter));
      tEntry = tLegend41->AddEntry("tGraphRmsLinear","linear calibration","P");
      tEntry->SetMarkerSize(1.5);
      tEntry = tLegend41->AddEntry("tGraphRmsBinEdge","bin-by-bin edge","P");
      tEntry->SetMarkerSize(1.5);
      tEntry = tLegend41->AddEntry("tGraphRmsBinCenter","bin-by-bin center","P");
      tEntry->SetMarkerSize(1.5);
      tLegend41->Draw();

    }

    if(tGraphSigmaLinear && tGraphSigmaBinCenter && tGraphSigmaBinEdge)
    {
      fBoardOffsetGraphs->cd(2);
      gPad->SetGridx();
      gPad->SetGridy();
      tGraphSigmaLinear->SetMarkerColor(1);
      tGraphSigmaBinCenter->SetMarkerColor(4);
      tGraphSigmaBinEdge->SetMarkerColor(2);
      tGraphSigmaLinear->SetMarkerStyle(20);
      tGraphSigmaBinCenter->SetMarkerStyle(21);
      tGraphSigmaBinEdge->SetMarkerStyle(22);
      tGraphSigmaLinear->GetXaxis()->SetTitle("TDC index []");
      tGraphSigmaLinear->GetYaxis()->SetTitle("Sigma [ps]");
      tGraphSigmaLinear->Draw("AP");
      tGraphSigmaBinCenter->Draw("P");
      tGraphSigmaBinEdge->Draw("P");

      TLegend* tLegend42 = new TLegend(gPad->GetLeftMargin()+0.05,gPad->GetBottomMargin()+0.05,1.-gPad->GetRightMargin()-0.05,0.5);
      tLegend42->SetName("tLegendSigma");
      tLegend42->SetHeader(Form("%d events",fEventCounter));
      tEntry = tLegend42->AddEntry("tGraphSigmaLinear","linear calibration","P");
      tEntry->SetMarkerSize(1.5);
      tEntry = tLegend42->AddEntry("tGraphSigmaBinEdge","bin-by-bin edge","P");
      tEntry->SetMarkerSize(1.5);
      tEntry = tLegend42->AddEntry("tGraphSigmaBinCenter","bin-by-bin center","P");
      tEntry->SetMarkerSize(1.5);
      tLegend42->Draw();

    }

    if(tGraphHistoMeanLinear && tGraphHistoMeanBinCenter && tGraphHistoMeanBinEdge)
    {
      fBoardOffsetGraphs->cd(3);
      gPad->SetLogy();
      gPad->SetGridx();
      gPad->SetGridy();
      tGraphHistoMeanLinear->SetMarkerColor(1);
      tGraphHistoMeanBinCenter->SetMarkerColor(4);
      tGraphHistoMeanBinEdge->SetMarkerColor(2);
      tGraphHistoMeanLinear->SetMarkerStyle(20);
      tGraphHistoMeanBinCenter->SetMarkerStyle(21);
      tGraphHistoMeanBinEdge->SetMarkerStyle(22);
      tGraphHistoMeanLinear->GetXaxis()->SetTitle("TDC index []");
      tGraphHistoMeanLinear->GetYaxis()->SetTitle("h-Mean [clock cycles]");
      tGraphHistoMeanLinear->Draw("AP");
      tGraphHistoMeanBinCenter->Draw("P");
      tGraphHistoMeanBinEdge->Draw("P");
      tGraphHistoMeanLinear->GetYaxis()->SetRangeUser(1.,1000000000000.);

    }

    if(tGraphGausMeanLinear && tGraphGausMeanBinCenter && tGraphGausMeanBinEdge)
    {
      fBoardOffsetGraphs->cd(4);
      gPad->SetLogy();
      gPad->SetGridx();
      gPad->SetGridy();
      tGraphGausMeanLinear->SetMarkerColor(1);
      tGraphGausMeanBinCenter->SetMarkerColor(4);
      tGraphGausMeanBinEdge->SetMarkerColor(2);
      tGraphGausMeanLinear->SetMarkerStyle(20);
      tGraphGausMeanBinCenter->SetMarkerStyle(21);
      tGraphGausMeanBinEdge->SetMarkerStyle(22);
      tGraphGausMeanLinear->GetXaxis()->SetTitle("TDC index []");
      tGraphGausMeanLinear->GetYaxis()->SetTitle("g-Mean [clock cycles]");
      tGraphGausMeanLinear->Draw("AP");
      tGraphGausMeanBinCenter->Draw("P");
      tGraphGausMeanBinEdge->Draw("P");
      tGraphGausMeanLinear->GetYaxis()->SetRangeUser(1.,1000000000000.);

    }


    for(Int_t iBoardIndex = 0; iBoardIndex < fNumberOfTDC; iBoardIndex++)
    {
      TH1D* tOffsetsLinear = dynamic_cast<TH1D*>(gROOT->FindObjectAny(Form("tOffsetsLinear%d",iBoardIndex)));
      TH1D* tOffsetsBinCenter = dynamic_cast<TH1D*>(gROOT->FindObjectAny(Form("tOffsetsBinCenter%d",iBoardIndex)));
      TH1D* tOffsetsBinEdge = dynamic_cast<TH1D*>(gROOT->FindObjectAny(Form("tOffsetsBinEdge%d",iBoardIndex)));

      if( tOffsetsLinear )
      {
        fBoardOffsetLinear->cd(iBoardIndex+1);
        gPad->SetLogy();
        tOffsetsLinear->GetXaxis()->SetRangeUser(tOffsetsLinear->GetMean()-0.3,tOffsetsLinear->GetMean()+0.3);
        tOffsetsLinear->Draw();
        gPad->Update();
        (tOffsetsLinear->GetFunction("gaus"))->SetLineWidth(3);
        (tOffsetsLinear->GetFunction("gaus"))->Draw("same");
        gPad->Update();
        gPad->Modified();
      }

      if( tOffsetsBinCenter )
      {
        fBoardOffsetBinCenter->cd(iBoardIndex+1);
        gPad->SetLogy();
        tOffsetsBinCenter->GetXaxis()->SetRangeUser(tOffsetsBinCenter->GetMean()-0.3,tOffsetsBinCenter->GetMean()+0.3);
        tOffsetsBinCenter->Draw();
        gPad->Update();
        (tOffsetsBinCenter->GetFunction("gaus"))->SetLineWidth(3);
        (tOffsetsBinCenter->GetFunction("gaus"))->Draw("same");
        gPad->Update();
        gPad->Modified();
      }

      if( tOffsetsBinEdge )
      {
        fBoardOffsetBinEdge->cd(iBoardIndex+1);
        gPad->SetLogy();
        tOffsetsBinEdge->GetXaxis()->SetRangeUser(tOffsetsBinEdge->GetMean()-0.3,tOffsetsBinEdge->GetMean()+0.3);
        tOffsetsBinEdge->Draw();
        gPad->Update();
        (tOffsetsBinEdge->GetFunction("gaus"))->SetLineWidth(3);
        (tOffsetsBinEdge->GetFunction("gaus"))->Draw("same");
        gPad->Update();
        gPad->Modified();
      }
    }

  }

  if( fbMonitorInspection )
  {
    fLeadingEdgeOnly->Update();
    fTrailingEdgeOnly->Update();
    fUnequalEdgeCounts->Update();
    fTimeOverThreshold->Update();
    fLeadingPosition->Update();
    fTrailingPosition->Update();
    fBoardFineTime->Update();
    fRefChFineTime->Update();
  }

  if( fbMonitorCts )
  {
    fCtsTriggerMonitor->Update();
  }
  if( fbMonitorSebStatus )
  {
    fSebSizeMonitor->Update();
    fSebStatusMonitor->Update();
    fSebDataRateMonitor->Update();
  }
  if( fbMonitorTdcStatus )
  {
    fTdcSizeMonitor->Update();
    fTdcStatusMonitor->Update();
  }
  if( fbMonitorFSMockup )
  {
    fFSMockupMonitor->Update();
  }
  if( kTRUE == fbMonitorRes )
     fOverviewRes->Update();

  if( fbMonitorDigiStatus )
  {
    fDigiStatusMonitor->Update();
  }

}

Bool_t CbmTofOnlineDisplay::InitParameters()
{
   // Get Base Container
   FairRun* ana = FairRun::Instance();
   if( 0 == ana )
      return kFALSE;
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();
   if( 0 == rtdb )
      return kFALSE;

   // Unpacker parameter
   fMbsUnpackPar = (TMbsUnpackTofPar*) (rtdb->getContainer("TMbsUnpackTofPar"));
   if( 0 == fMbsUnpackPar )
      return kFALSE;

   // Call needed here to make available calibration parameters
   // in the TRB unpack stage
   fMbsCalibPar = (TMbsCalibTofPar*) (rtdb->getContainer("TMbsCalibTofPar"));
   if( 0 == fMbsCalibPar )
      return kFALSE;

   return kTRUE;
}

ClassImp(CbmTofOnlineDisplay)
