#include "StsCosyClusterFinder.h"

#include <FairTask.h>           // for kSUCCESS, InitStatus, FairTask, kERROR
#include <TFile.h>              // for TFile
#include <TH1.h>                // for TH1F
#include <TMathBase.h>          // for Abs
#include <TTree.h>              // for TTree
#include <FairLogger.h>         // for LOG, Logger
#include <stdio.h>              // for NULL, sprintf
#include <iostream>             // for operator<<, basic_ostream, endl, cout
#include "CbmStsAddress.h"      // for GetElementId, kStsSide
#include "CbmStsCluster.h"      // for CbmStsCluster
#include "CbmStsDigi.h"         // for CbmStsDigi
#include "CbmStsSetup.h"        // for CbmStsSetup
#include <FairRootManager.h>    // for FairRootManager
#include <TClonesArray.h>       // for TClonesArray
#include <TMath.h>              // for Sqrt

using std::map;
using std::set;
using namespace std;

// ---- Default constructor -------------------------------------------
StsCosyClusterFinder::StsCosyClusterFinder()
  :FairTask("StsCosyClusterFinder",1),
   fDigis(NULL),
   fClusters(NULL),
   finalClusters(NULL),
   fTriggeredMode(kFALSE),
   fTriggeredStation(1),
   fChargeMinStrip(),
   fChargeMaxStrip(),
   fChargeMinCluster(),
   fTimeLimit(),
   fTimeShift(),
   fCutName(""),
   cluster_size(),
   fEvent(0),
   fDigiMap()
{
  for(int i=0; i<3; i++)
  {
    fTimeLimit[i]= 40.;
    fTimeShift[i]= 0.;
    
    fChargeMinStrip[i]= 50.;
    fChargeMaxStrip[i]= 500.;
    
    fChargeMinCluster[i]= 100.;
    fCutName = "";
  }
}
// --------------------------------------------------------------------

// ---- Destructor ----------------------------------------------------
StsCosyClusterFinder::~StsCosyClusterFinder()
{

  if(fClusters){
    fClusters->Clear("C");
    fClusters->Delete();
    delete fClusters;
  }
  if(finalClusters){
    finalClusters->Clear("C");
    finalClusters->Delete();
    delete finalClusters;
  }

}

// ----  Initialisation  ----------------------------------------------
void StsCosyClusterFinder::SetParContainers()
{

  // Get Base Container
/*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
*/  
}
// --------------------------------------------------------------------

// ---- ReInit  -------------------------------------------------------
InitStatus StsCosyClusterFinder::ReInit(){
  
/*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
*/
  
  return kSUCCESS;
}
// --------------------------------------------------------------------

// ---- Init ----------------------------------------------------------
InitStatus StsCosyClusterFinder::Init()
{

  FairRootManager *ioman = FairRootManager::Instance();
  

  //  fDigis =(TClonesArray *)  ioman->GetObject("StsCalibDigi");
  fDigis = ioman->InitObjectAs<std::vector<CbmStsDigi> const *>("StsCalibDigi");

  if ( ! fDigis ) {
    LOG(error) << "No StsDigi array!";
    LOG(error) << "Task will be inactive";
    return kERROR;
  }
    
  finalClusters = new TClonesArray("CbmStsCluster", 100);
  ioman->Register("StsCluster","STS",finalClusters,IsOutputBranchPersistent("StsCluster"));

  fClusters = new TClonesArray("CbmStsCluster", 100);
  ioman->Register("StsClusterCandidate","STS",fClusters,IsOutputBranchPersistent("StsClusterCandidate"));
    
  if(fCutName != ""){
    
  TFile *file = new TFile(fCutName);
    
    LOG(info) << "Use inpute file for time cuts " << fCutName;
   
   TTree *sts0 = (TTree*)file->Get("STS0");
   sts0->SetBranchAddress("TimeLimit",&fTimeLimit[0]);   
   sts0->SetBranchAddress("TimeShift",&fTimeShift[0]);   
   sts0->GetEntry(0);
 
  TTree * sts1 = (TTree*)file->Get("STS1");
   sts1->SetBranchAddress("TimeLimit",&fTimeLimit[1]);   
   sts1->SetBranchAddress("TimeShift",&fTimeShift[1]);   
   sts1->GetEntry(0);
 
  TTree * sts2 = (TTree*)file->Get("STS2");
   sts2->SetBranchAddress("TimeLimit",&fTimeLimit[2]);   
   sts2->SetBranchAddress("TimeShift",&fTimeShift[2]);   
   sts2->GetEntry(0);
  }
  
   for(int i =0; i<3;i++)for(int ii =0; ii<2;ii++)
	  {
	    Char_t inName[25]; 
	    sprintf(inName,"cluster_size_STS%i_side%i",i,ii);
	    cluster_size[i][ii] = new TH1F(inName, inName, 100, -0.5, 99.5);
	  }
 
  fEvent = 0;
  
  cout << "-I- StsCosyClusterFinder: Intialisation successfull " << kSUCCESS<< endl;
 
  return kSUCCESS;
  
} 
// --------------------------------------------------------------------

// ---- Exec ----------------------------------------------------------
void StsCosyClusterFinder::Exec(Option_t*)
{
  if(fEvent%100000 == 0)cout << "-I- StsCosyClusterFinder: ----- " << fEvent << endl;

  fClusters->Delete();
  finalClusters->Delete();

  map<Int_t, set<const CbmStsDigi*, classcomp1> >::iterator mapIt;
  for (mapIt=fDigiMap.begin(); mapIt!=fDigiMap.end(); mapIt++) 
    {
      ((*mapIt).second).clear();
    }
  fDigiMap.clear();
  
  Int_t nofDigis = fDigis->size();

  fEvent++;
  
  if (0 == nofDigis) return;
    
  set<Int_t> layerSet;
  
  const CbmStsDigi* digi = NULL;
  
  map<const CbmStsDigi*, Int_t> fIndices;
  

  for (Int_t iDigi=0; iDigi < nofDigis; iDigi++ ) 
    {
      digi = &(fDigis->at(iDigi));
      int station = CbmStsSetup::Instance()->GetStationNumber(digi->GetAddress());
      int side = CbmStsAddress::GetElementId(digi->GetAddress(),kStsSide);
      Int_t layer= 2*station+side;

      if(digi->GetCharge() > fChargeMinStrip[station] && digi->GetCharge() < fChargeMaxStrip[station])
      {
	layerSet.insert(layer);
	fDigiMap[layer].insert(digi);
	fIndices.at(digi)=iDigi;          
      }
    }
  
  set <const CbmStsDigi*, classcomp1> digiSet;

  for (set<Int_t>::iterator i = layerSet.begin(); i != layerSet.end(); i++) 
    {
      digiSet = fDigiMap.at(*i);
      Bool_t newCluster = kTRUE;
      Int_t stripNr = -1;
      Int_t stripNrPrev = -1;
      CbmStsCluster* cluster=NULL;
      Double_t time = -1;
      Double_t timePrev = -1;

      for (set<const CbmStsDigi*, classcomp1>::iterator j = digiSet.begin(); j != digiSet.end(); j++) 
	{
	  if (newCluster) 
	    {
	      Int_t size = fClusters->GetEntriesFast();
	      cluster = new ((*fClusters)[size]) CbmStsCluster();
	      Int_t index = fIndices[(*j)];
	      //----time----
	      digi = &(fDigis->at(index));
	      // digi = static_cast<CbmStsDigi*>(fDigis->At(index));
	      timePrev = digi->GetTime();

	      cluster->AddDigi(index);  
	      stripNrPrev = (*j)->GetChannel();
	      newCluster = kFALSE;
	    } 
	  else 
	    {
	      Int_t index = fIndices[(*j)];
	      digi = &(fDigis->at(index));
	      //digi = static_cast<CbmStsDigi*>(fDigis->At(index));
	      time = digi->GetTime();
	      stripNr = (*j)->GetChannel();
	      
	      int station = CbmStsSetup::Instance()->GetStationNumber(digi->GetAddress());
//	      int side = CbmStsAddress::GetElementId(digi->GetAddress(),kStsSide);
	      
	      Bool_t TrCl = kTRUE;
	      
	      if(fTriggeredMode && station==fTriggeredStation)
	      {
		if(TMath::Abs(time - timePrev) > 1)TrCl=kFALSE;
	      }
	      if (1 == stripNr-stripNrPrev && TMath::Abs(time - timePrev- fTimeShift[station])< fTimeLimit[station] && TrCl) 
		{
		  stripNrPrev = stripNr;
		  timePrev = time;
		  cluster = (CbmStsCluster*) fClusters->Last();
		  cluster->AddDigi(index);  
		} 
	      else if(TrCl)
		{
		  Int_t size = fClusters->GetEntriesFast();
		  cluster = new ((*fClusters)[size]) CbmStsCluster();
		  index = fIndices[(*j)];
                  cluster->AddDigi(index);  
		  stripNrPrev = (*j)->GetChannel();
		  newCluster = kFALSE;
		}
 	    }
	}
    }
  
  if(nofDigis>0)
    {
      Int_t nofClusterCandidates = fClusters->GetEntriesFast();
//      int clust[6]={0,0,0,0,0,0}; 
//      Int_t layer=0;
      for (Int_t iclus = 0; iclus < nofClusterCandidates; iclus++)
	{
	  Double_t chanNr = 0;
	  Double_t chanADC = 0.;
//	  Double_t adc = 100.;
	  Double_t sumW = 0;
	  Double_t sumWX = 0;
	  Double_t error = 0;
//	  layer=0;
	  Double_t chanNrMean = 0;
	  const CbmStsCluster* cluster = static_cast<const CbmStsCluster*>(fClusters->At(iclus));
	  Int_t nofStrips = cluster->GetNofDigis();
	  
	  //if(nofStrips > 1)continue; //produce only 1 strip cluster
	  
//	  Double_t time = -999.;
	  double mean_time = 0.;
	  
	  int station = 0;
	  int side = 0;
	  for(int st=0; st<nofStrips; st++)
	    {
	      const CbmStsDigi* temp_digi = &(fDigis->at(cluster->GetDigi(st)));
	      station = CbmStsSetup::Instance()->GetStationNumber(temp_digi->GetAddress());
	      side = CbmStsAddress::GetElementId(temp_digi->GetAddress(),kStsSide);
	      int ch = temp_digi->GetChannel();
//	      layer = 2*station+side;
	      chanNr += ch;
	      chanADC = temp_digi->GetCharge();
	      sumW += chanADC;
	      sumWX += ch * chanADC;
	      error += (chanADC * chanADC);
	      
//	      time = temp_digi->GetTime();
	      mean_time+=temp_digi->GetTime();
	      
	      //if(chanADC > adc){adc=chanADC; mean_time=time;}
	    }
          chanNrMean = chanNr/(Double_t)nofStrips;
          if(sumW < fChargeMinCluster[station]){continue;}

	  Int_t size = finalClusters->GetEntriesFast();
	  CbmStsCluster* new_cluster = new ((*finalClusters)[size]) CbmStsCluster();
	  
	  for (Int_t i = 0; i < nofStrips; ++i)
	    {
	      Int_t digi_index=cluster->GetDigi(i);
	      const CbmStsDigi* temp_digi = &(fDigis->at(digi_index));
	      if (i == 0)
		{
		  new_cluster->SetAddress(temp_digi->GetAddress());
		}
	      new_cluster->AddDigi(digi_index);
	 
	    }
	  //new_cluster->SetCentre(sumWX / sumW);
	  //new_cluster->SetProperties(sumW,chanNrMean,0.,mean_time/nofStrips);
	  //new_cluster->SetProperties(0.,0.,0.,mean_time);
	  //new_cluster->SetCentreError((1. / (sumW)) * TMath::Sqrt(error));
	  
	  new_cluster->SetProperties(sumW, chanNrMean, (1./(sumW))*TMath::Sqrt(error), mean_time/nofStrips);
	  
	 cluster_size[station][side]->Fill(new_cluster->GetNofDigis());
	}
    }
  fIndices.clear();
  digiSet.clear();
  layerSet.clear();
  
}
// --------------------------------------------------------------------

  // ---- Finish --------------------------------------------------------
  void StsCosyClusterFinder::Finish()
  {
        for(int i=0; i<3; i++)for(int j=0; j<2; j++)cluster_size[i][j]->Write();

  LOG(info);
  LOG(info) << "=====================================================";
  LOG(info) << GetName() << ": Finish";
  LOG(info) << "=====================================================";
  LOG(info);
  
  }





  ClassImp(StsCosyClusterFinder)

