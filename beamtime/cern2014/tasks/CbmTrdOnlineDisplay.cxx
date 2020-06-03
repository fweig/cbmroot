#include "CbmTrdOnlineDisplay.h"

#include "CbmHistManager.h"
#include "CbmTrdRawBeamProfile.h"

#include "FairLogger.h"

#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TROOT.h"
#include "TSeqCollection.h"
#include "TTask.h"
#include "TClass.h"
#include "TDataMember.h"
#include "TMethodCall.h"

// ---- Default constructor -------------------------------------------
CbmTrdOnlineDisplay::CbmTrdOnlineDisplay(TString inFile)
  :FairTask("CbmTrdOnlineDisplay"),
   fSpadic1(),
   fSpadic1a(),
   fSpadicTime(),
   finFile(inFile),
   fUpdateInterval(10),
   fEventCounter(0)
{
}

// ---- Destructor ----------------------------------------------------
CbmTrdOnlineDisplay::~CbmTrdOnlineDisplay()
{
//  delete[] fSpadic1;
//  delete[] fSpadic1a;
}

// ----  Initialisation  ----------------------------------------------
void CbmTrdOnlineDisplay::SetParContainers()
{
}

// ---- Init ----------------------------------------------------------
InitStatus CbmTrdOnlineDisplay::Init()
{

  TH1* h1 = NULL;
  TH1* h2 = NULL;
  //  TGraph* g1 = NULL;
  gROOT->cd();
  Float_t lsize=0.07;
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  fSpadicTime = new TCanvas("fSpadicTime","fSpadicTime",0,0,1600,1200);
  fSpadicTime->Divide(4,3);
  fSpadicTime->cd(1)->SetLogy(1);
  fSpadicTime->cd(1)->SetLogx(1);
  h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("DeltaClusterTime2015CernSPS")));
  if (h1!=NULL) {
    h1->Draw("");
  }
  fSpadicTime->cd(2);
  h2=static_cast<TH2*>(gROOT->FindObjectAny(TString("DeltaClusterTimeCorrelation2015CernSPS")));
  if (h2!=NULL) {
    h2->Draw("colz");
  }
  fSpadicTime->cd(3);
  h2=static_cast<TH2*>(gROOT->FindObjectAny(TString("DeltaClusterTimeCorrelationFullCluster2015CernSPS")));
  if (h2!=NULL) {
    h2->Draw("colz");
  }
  fSpadicTime->cd(4);
  h2=static_cast<TH2*>(gROOT->FindObjectAny(TString("DeltaClusterTimeCorrelationAllEvents2015CernSPS")));
  if (h2!=NULL) {
    h2->Draw("colz");
  }
  h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("DeltaTime_Cluster_SysCore0_Spadic0")));
  fSpadicTime->cd(5)->SetLogy(1);
  if (h1!=NULL) {
    h1->Draw("");
  }
  h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("DeltaTime_Cluster_SysCore0_Spadic1")));
  h1->SetLineColor(2);
  if (h1!=NULL) {
    h1->Draw("same");
  }
  h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("DeltaTime_Cluster_SysCore1_Spadic1")));
  h1->SetLineColor(3);
  if (h1!=NULL) {
    h1->Draw("same");
  }
  h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("DeltaTime_Cluster_SysCore1_Spadic1")));
  h1->SetLineColor(4);
  if (h1!=NULL) {
    h1->Draw("same");
  }
 for (Int_t sys = 0; sys < 2; sys++){
    for (Int_t spa = 0; spa < 2; spa++){
      fSpadic1[spa][sys] = new TCanvas(TString("fSysCore" + std::to_string(sys) + "Spadic" + std::to_string(spa)), TString("fSysCore" + std::to_string(sys) + "Spadic" + std::to_string(spa)), 0, 0, 2000, 1200);
      fSpadic1[spa][sys]->Divide(5,3);

      // Should be set for each pad of the Canvas
      gPad->SetFillColor(0);

      fSpadic1[spa][sys]->cd(1);
      h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("CountRate_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h1!=NULL) {
	h1->Draw("");
      }

      fSpadic1[spa][sys]->cd(2)->SetLogz(1);
      h2=static_cast<TH2*>(gROOT->FindObjectAny(TString("BaseLine_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h2!=NULL) {
	h2->Draw("COLZ");
      }

      fSpadic1[spa][sys]->cd(3)->SetLogy(1);
      h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("GroupId_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h1!=NULL) {
	h1->Draw("");
      }

      fSpadic1[spa][sys]->cd(4)->SetLogz(1);
      h2=static_cast<TH2*>(gROOT->FindObjectAny(TString("Trigger_Heatmap_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h2!=NULL) {
	h2->Draw("COLZ");
      }
      fSpadic1[spa][sys]->cd(5)->SetLogy(1);
      /*
      g1=static_cast<TGraph*>(gROOT->FindObjectAny(TString("TriggerCounterGraph_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (g1!=NULL) {
	g1->Draw("AL");
      }
      */
      h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("TriggerCounter_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h1!=NULL) {
	h1->Draw("");
      }
      h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("OverFlowCounter_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      h1->SetLineColor(2);
      if (h1!=NULL) {
	h1->Draw("same");
      }
      h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("ErrorCounter_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      h1->SetLineColor(3);
      if (h1!=NULL) {
	h1->Draw("same");
      }

      fSpadic1[spa][sys]->cd(6)->SetLogy(1);
      h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("TriggerSum")));
      if (h1!=NULL) {
	h1->Draw("");
      }

      fSpadic1[spa][sys]->cd(7)->SetLogy(0);
      h1=static_cast<TH2*>(gROOT->FindObjectAny(TString("StopTypes_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h1!=NULL) {
	h1->Draw("colz");
      }
      fSpadic1[spa][sys]->cd(8)->SetLogy(0);
      h1=static_cast<TH2*>(gROOT->FindObjectAny(TString("InfoTypes_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h1!=NULL) {
	h1->Draw("colz");
      }
      fSpadic1[spa][sys]->cd(9)->SetLogy(0);
      h1=static_cast<TH2*>(gROOT->FindObjectAny(TString("TriggerTypes_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h1!=NULL) {
	h1->Draw("colz");
      }
      fSpadic1[spa][sys]->cd(10)->SetLogy(1);
      h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("ClusterSize_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h1!=NULL) {
	h1->Draw("");
      }
      fSpadic1[spa][sys]->cd(11)->SetLogy(0);
      h1=static_cast<TH2*>(gROOT->FindObjectAny(TString("Message_Length_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h1!=NULL) {
	h1->Draw("colz");
      }
      fSpadic1[spa][sys]->cd(12)->SetLogy(0);
      h1=static_cast<TH2*>(gROOT->FindObjectAny(TString("TriggerType_ClusterSize_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
      if (h1!=NULL) {
	h1->Draw("colz");
      }
    }
  }

  for (Int_t sys = 0; sys < 2; sys++){
    for (Int_t spa = 0; spa < 2; spa++) {
      fSpadic1a[spa][sys] = new TCanvas(TString("fSysCore" + std::to_string(sys) + "Spadic" + std::to_string(spa) +"_Signal_Shape"), TString("fSysCore" + std::to_string(sys) + "Spadic" + std::to_string(spa) +"_Signal_Shape"), 50, 50, 1200, 700);
      fSpadic1a[spa][sys]->Divide(8,4);
      gPad->SetFillColor(0);

      for (Int_t i=0; i<32; i++) {
	fSpadic1a[spa][sys]->cd(i+1)->SetLogz(1);

	if (true){
	  TString temphistname = "Signal_Shape_SysCore"+std::to_string(sys)+"_Spadic"+std::to_string(spa)+"_Ch";
	  if(i<10) {
	    temphistname = temphistname + std::to_string(0) + std::to_string(i);
	    h2=static_cast<TH2*>(gROOT->FindObjectAny(temphistname.Data()));
	    if (h2!=NULL) {
	      h2->Draw("COLZ");
	    }
	  } else {
	    temphistname = temphistname + std::to_string(i);
	    h2=static_cast<TH2*>(gROOT->FindObjectAny(temphistname.Data()));
	    if (h2!=NULL) {
	      h2->Draw("COLZ");
	    }
	  }
	} else {
	  TString temphistname = "Signal_Shape_SysCore"+std::to_string(sys)+"_Spadic"+std::to_string(spa)+"_Ch" + std::to_string(0) + std::to_string(i);
	  h1=static_cast<TH1*>(gROOT->FindObjectAny(temphistname.Data()));
	  if (h1!=NULL) {
	    h1->SetLineColor(sys);
	    h1->SetLineStyle(spa);
	    if (sys == 0 && spa == 0)
	      h1->Draw("C");
	    else
	      h1->Draw("C,same");
	  }
	}
      }
    }
  }
  return kSUCCESS;

}

// ---- ReInit  -------------------------------------------------------
InitStatus CbmTrdOnlineDisplay::ReInit()
{
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void CbmTrdOnlineDisplay::Exec(Option_t*)
{
  Int_t maxTrigger = 0;
  fEventCounter++;
  TH1* h1=NULL;

  if ( 0 == fEventCounter%(fUpdateInterval/10)) {
    LOG(info)<<"Update Canvas for Event "<< fEventCounter;
    for (Int_t sys = 0; sys < 2; sys++){
      for (Int_t spa = 0; spa < 2; spa++){
	h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("TriggerCounter_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
	if (h1->GetBinContent(h1->GetMaximumBin()) > maxTrigger)
	  maxTrigger = h1->GetBinContent(h1->GetMaximumBin());
      }
    }
    for (Int_t sys = 0; sys < 2; sys++){
      for (Int_t spa = 0; spa < 2; spa++){
	h1=static_cast<TH1*>(gROOT->FindObjectAny(TString("TriggerCounter_SysCore" + std::to_string(sys) + "_Spadic" + std::to_string(spa))));
	//h1->GetYaxis()->SetRangeUser(-10,10+1.125*maxTrigger);
	for(Int_t iCh=0; iCh<9; iCh++){
	  fSpadic1[spa][sys]->cd(iCh+1);
	  gPad->Modified();
	  //gPad->Update();
	}
	fSpadic1[spa][sys]->Update();
      }
    }
  }
}

  // ---- Finish --------------------------------------------------------
void CbmTrdOnlineDisplay::Finish()
{
fSpadicTime->Update();
 finFile.ReplaceAll("data/","");
finFile.ReplaceAll(".tsa","_");
 fSpadicTime->SaveAs(TString("pics/" + finFile + "fSpadicTime.png"));
fSpadicTime->Write("",TObject::kOverwrite);
  for (Int_t sys = 0; sys < 2; sys++){
    for (Int_t spa = 0; spa < 2; spa++){
      fSpadic1[spa][sys]->Update();
      fSpadic1[spa][sys]->SaveAs(TString("pics/") + finFile + TString(fSpadic1[spa][sys]->GetTitle())+TString(".png"));
      fSpadic1[spa][sys]->Write("",TObject::kOverwrite);
      for(Int_t iCh=0; iCh<32; iCh++){
	fSpadic1a[spa][sys]->cd(iCh+1);
	gPad->Modified();
	//gPad->Update();
      }
      fSpadic1a[spa][sys]->ResizeOpaque(0);
      //fSpadic1a[spa][sys]->Resize();
      fSpadic1a[spa][sys]->Update();
      fSpadic1a[spa][sys]->SaveAs(TString("pics/") + finFile + TString(fSpadic1a[spa][sys]->GetTitle())+TString(".png"));
      fSpadic1a[spa][sys]->Write("",TObject::kOverwrite);
    }
  }
}
  ClassImp(CbmTrdOnlineDisplay)
