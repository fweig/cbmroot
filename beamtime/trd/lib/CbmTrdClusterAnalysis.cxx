#include "CbmTrdClusterAnalysis.h"
#include "CbmTrdDigi.h"
#include "CbmTrdCluster.h"
#include "CbmTrdHit.h"
#include "CbmHistManager.h"
//#include "CbmBeamDefaults.h"

#include "FairLogger.h"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"

#include "TString.h"
#include "TStyle.h"
#include "TMath.h"
#include "TVector3.h"

#include <cmath>
#include <map>
#include <vector>
#include <math.h>

CbmTrdClusterAnalysis::CbmTrdClusterAnalysis()
  :FairTask("CbmTrdClusterAnalysis"),
   fDigis(NULL),
   fClusters(NULL),  
   fHits(NULL),
   fHM(new CbmHistManager())
{
 LOG(debug) << "Default Constructor of CbmTrdClusterAnalysis";
}
CbmTrdClusterAnalysis::~CbmTrdClusterAnalysis()
{
  fDigis->Delete();
  delete fDigis;
  fClusters->Delete();
  delete fClusters;
  fHits->Delete();
  delete fHits;
  LOG(debug) << "Destructor of CbmTrdClusterAnalysis";
}
void CbmTrdClusterAnalysis::SetParContainers()
{
 LOG(debug) << "SetParContainers of CbmTrdClusterAnalysis";
}
InitStatus CbmTrdClusterAnalysis::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  fDigis = static_cast<TClonesArray*>(ioman->GetObject("TrdDigi"));
  if ( ! fDigis) {
    LOG(fatal) << "No CbmTrdDigi TClonesArray!\n CbmTrdClusterAnalysis will be inactive";
    return kERROR;
  }
  fClusters = static_cast<TClonesArray*>(ioman->GetObject("TrdCluster"));
  if ( ! fClusters) {
    LOG(fatal) << "No CbmTrdCluster TClonesArray!\n CbmTrdClusterAnalysis will be inactive";
    return kERROR;
  }
  fHits = new TClonesArray("CbmTrdHit", 100);
  ioman->Register("TrdHit", "TRD Hits", fDigis, IsOutputBranchPersistent("TrdHit"));

  CbmTrdClusterAnalysis::CreateHistograms();

  return kSUCCESS;
}
InitStatus CbmTrdClusterAnalysis::ReInit()
{
  LOG(debug) << "Initilization of CbmTrdClusterAnalysis";
  return kSUCCESS;
}
void CbmTrdClusterAnalysis::Exec(Option_t*)
{
  // Exec runs on the TimeSliceContainer level. The TClonesArrys are not available at this time. Therefore we do the analysis in the finish function.
}
void CbmTrdClusterAnalysis::Finish()
{
  TCanvas* c = new TCanvas("pulseshape","pulseshape",800,600);
  TH1F* pulse = new TH1F("pulse","pulse",32,-0.5,31.5);
  pulse->GetYaxis()->SetRangeUser(-255,256);
  TString histName = "";
  //  Int_t nDigis = fDigis->GetEntriesFast();
  Int_t nClusters = fClusters->GetEntriesFast();
  Int_t DigiCounter(0), ClusterCounter(0);
  for (Int_t iCluster=0; iCluster < nClusters; iCluster++) {
    ClusterCounter++;
    CbmTrdCluster *cluster = (CbmTrdCluster*) fClusters->At(iCluster);
    Int_t nofDigis = cluster->GetNofDigis();
    Int_t moduleAddress = 0;
    Int_t /*secId(-1),*/ colId(-1), /*rowId(-1),*/ layerId(-1);  // (VF) not used.
    TVector3 hit_posV; // hit position in global chamber coordinates
    TVector3 local_pad_dposV; // Error in local pad coordinates
    Double_t totalCharge = 0;
    std::vector<Float_t> charge;
    std::vector<Float_t> amplitude;
    Int_t last_colId = -1;
    //printf("|>------------------Cluster %i started\n", iCluster);
    c->Clear();
    for (Int_t iDigi = 0; iDigi < nofDigis; iDigi++) {      
      DigiCounter++;  
      pulse->SetLineColor(iDigi+1); 
      Int_t digiId = cluster->GetDigi(iDigi);
      Float_t digiCharge = 0.;
      CbmTrdDigi* digi = (CbmTrdDigi*) fDigis->At(digiId);
      //if (digi->GetStopType() > 0) continue;
      Int_t nrSamples = digi->GetNrSamples();
      //printf("%02i: (%5.1f) ",nrSamples,digi->GetSamples()[nrSamples-1]);
      Float_t maxAmpl = -255.0;
      for (Int_t iBin = 0; iBin < nrSamples; iBin++){
	pulse->SetBinContent(iBin+1,digi->GetSamples()[iBin]);
	digiCharge += digi->GetSamples()[iBin];
	if (digi->GetSamples()[iBin] > maxAmpl)
	  maxAmpl = digi->GetSamples()[iBin];
	//printf("%5.0f ",digi->GetSamples()[iBin]);
      }
      c->cd();
      if(iDigi==0)
	pulse->DrawCopy();
      else
	pulse->DrawCopy("same");
      pulse->Reset();
      //printf("\n");      
      Int_t digiAddress = digi->GetAddress();
      // secId = CbmTrdAddress::GetSectorId(digiAddress);  (VF) not used
      colId = CbmTrdAddress::GetColumnId(digiAddress);
      // rowId = CbmTrdAddress::GetRowId(digiAddress);   (VF) not used
      layerId = CbmTrdAddress::GetLayerId(digiAddress);
      //printf("time %10.2f Layer%02i Sec%i row%02i col%02i\n",digi->GetTime(),layerId,secId,rowId,colId);
      histName.Form("_Layer%02i",layerId);
      //moduleInfo = fDigiPar->GetModule(CbmTrdAddress::GetModuleAddress(digiAddress));
      moduleAddress = CbmTrdAddress::GetModuleAddress(digi->GetAddress());
      if (nofDigis >= 3)
	if (digi->GetStopType() == 0)
	  fHM->H1(TString("DigiSpectrum"+histName).Data())->Fill(digiCharge);
      if (iDigi > 0 && last_colId != -1)
	fHM->H1(TString("Delta_Channel_in_Cluster"+histName).Data())->Fill(colId - last_colId);
      // --- Here we have to test if the channel with maximum charge is primary triggerd (or at the same time FNR) and is centered (use colId) between two FNR (or at the same time primary) channels with smaller charge---
      // left FNR [0]; max Prim.[1]; right FNR [2];
      // Due to the clusterization the input Digi TClonesArray should be sorted in time and space (combiId). Therefore the colId should increase from digi 0 to digi 'n'. But this has to be tested here!!!
      charge.push_back(digiCharge);
      amplitude.push_back(maxAmpl);
      // ---
      if (nofDigis >= 3)
	if (digi->GetStopType() == 0)
	  totalCharge += digiCharge;  
      last_colId = colId;
    }  
    TString pulseId;
    pulseId.Form("pics/%08iCluster.png",iCluster);
    c->Update();
    if (iCluster % 100 == 0){
      //c->SaveAs(pulseId);
    }
    //printf("------------------>|Cluster %i finished\n", iCluster);
    if (nofDigis >= 3)
      fHM->H1(TString("ClusterSpectrum"+histName).Data())->Fill(totalCharge);
    if (nofDigis == 3){ // Here we need a better condition to be able to use also clusters larger than 3 channels calculating the PRF for the channel with the highest charge and the left and right neighboring channels
      if (charge[0] > 0 && charge[1] > 0 && charge[2] > 0){
	if (charge[0] <= charge[1] && charge[2] <= charge[1]){
	  Float_t d = 0.5 * W * (TMath::Log(charge[2]/charge[0])) / (TMath::Log((charge[1]*charge[1]) / (charge[0]*charge[2])));
	  fHM->H2(TString("PRF_Integral"+histName).Data())->Fill(-d,     charge[1] / totalCharge);
	  fHM->H2(TString("PRF_Integral"+histName).Data())->Fill(-d - W, charge[0] / totalCharge);
	  fHM->H2(TString("PRF_Integral"+histName).Data())->Fill(-d + W, charge[2] / totalCharge);
	  fHM->H1(TString("PRF_Integral_Profile"+histName).Data())->Fill(-d,     charge[1] / totalCharge);
	  fHM->H1(TString("PRF_Integral_Profile"+histName).Data())->Fill(-d - W, charge[0] / totalCharge);
	  fHM->H1(TString("PRF_Integral_Profile"+histName).Data())->Fill(-d + W, charge[2] / totalCharge);

	}
	if (amplitude[0] > 0 && amplitude[1] > 0 && amplitude[2] > 0){
	  if (amplitude[0] <= amplitude[1] && amplitude[2] <= amplitude[1]){
	    Float_t d = 0.5 * W * (TMath::Log(amplitude[2]/amplitude[0])) / (TMath::Log((amplitude[1]*amplitude[1]) / (amplitude[0]*amplitude[2])));
	    fHM->H2(TString("PRF_maxAmpl"+histName).Data())->Fill(-d,     amplitude[1] / (amplitude[0] + amplitude[1] + amplitude[2]));
	    fHM->H2(TString("PRF_maxAmpl"+histName).Data())->Fill(-d - W, amplitude[0] / (amplitude[0] + amplitude[1] + amplitude[2]));
	    fHM->H2(TString("PRF_maxAmpl"+histName).Data())->Fill(-d + W, amplitude[2] / (amplitude[0] + amplitude[1] + amplitude[2]));
	    fHM->H1(TString("PRF_maxAmpl_Profile"+histName).Data())->Fill(-d,     amplitude[1] / (amplitude[0] + amplitude[1] + amplitude[2]));
	    fHM->H1(TString("PRF_maxAmpl_Profile"+histName).Data())->Fill(-d - W, amplitude[0] / (amplitude[0] + amplitude[1] + amplitude[2]));
	    fHM->H1(TString("PRF_maxAmpl_Profile"+histName).Data())->Fill(-d + W, amplitude[2] / (amplitude[0] + amplitude[1] + amplitude[2]));
	  }
	}
      }
    }
    Int_t nofHits = fHits->GetEntriesFast();
    new ((*fHits)[nofHits]) CbmTrdHit(moduleAddress, hit_posV, local_pad_dposV, 0, iCluster, 0, 0, totalCharge);

    /**
     * \brief Standard constructor CbmTrdHit.
     *\param address Unique detector ID
     *\param pos Position in global c.s. [cm]
     *\param dpos Errors of position in global c.s. [cm]
     *\param dxy XY correlation of the hit
     *\param refId Index of digi or cluster
     *\param eLossTR Energy losses of electron via TR
     *\param eLossdEdx Energy deposition in the detector without TR
     *\param eLoss TR + dEdx
     **/
  }


  LOG(debug) << "Finish of CbmTrdRawBeamProfile";
  // Write to file
  fHM->WriteToFile();
  // Update Histos and Canvases
  LOG(info) << "CbmTrdClusterAnalysis::Finish Digis:               " << fDigis->GetEntriesFast()    << " Digis in Clusters: " << DigiCounter;
  LOG(info) << "CbmTrdClusterAnalysis::Finish Clusters:            " << fClusters->GetEntriesFast() << " Clusters found:    " << ClusterCounter;
  LOG(info) << "CbmTrdClusterAnalysis::Finish Hits:                " << fHits->GetEntriesFast();
}
  void CbmTrdClusterAnalysis::CreateHistograms()
  {
    gStyle->SetNumberContours(99);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    TString histName;
    for (Int_t layerId = 0; layerId < 6; layerId++){
      histName.Form("_Layer%02i",layerId);
      fHM->Add(TString("ClusterSpectrum"+histName).Data(), new TH1I(TString("ClusterSpectrum"+histName), TString("ClusterSpectrum"+histName), 2*32*255*5+1,-32*5*255.5,32*5*255.5));
      fHM->Add(TString("DigiSpectrum"+histName).Data(), new TH1I(TString("DigiSpectrum"+histName), TString("DigiSpectrum"+histName), 2*32*255+1,-32*255.5,32*255.5));
      fHM->Add(TString("PRF_maxAmpl"+histName).Data(),  new TH2I(TString("PRF_maxAmpl"+histName),  TString("PRF_maxAmpl"+histName),  300, -1.5*W, 1.5*W, 100, 0, 1));
      fHM->Add(TString("PRF_maxAmpl_Profile"+histName).Data(),  new TProfile(TString("PRF_maxAmpl_Profile"+histName),  TString("PRF_maxAmpl_Profile"+histName),  300, -1.5*W, 1.5*W, 0, 1));
      fHM->Add(TString("PRF_Integral"+histName).Data(), new TH2I(TString("PRF_Integral"+histName), TString("PRF_Integral"+histName), 300, -1.5*W, 1.5*W, 100, 0, 1));
      fHM->Add(TString("PRF_Integral_Profile"+histName).Data(),  new TProfile(TString("PRF_Integral_Profile"+histName),  TString("PRF_Integral_Profile"+histName),  300, -1.5*W, 1.5*W, 0, 1));
      fHM->Add(TString("Delta_Channel_in_Cluster"+histName).Data(), new TH1I(TString("Delta_Channel_in_Cluster"+histName), TString("Delta_Channel_in_Cluster"+histName),9,-4.5,4.5));
    }
  }
void CbmTrdClusterAnalysis::DrawHistograms(){

}
  ClassImp(CbmTrdClusterAnalysis)
