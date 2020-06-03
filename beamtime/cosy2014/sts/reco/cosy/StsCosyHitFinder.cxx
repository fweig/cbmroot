#include "StsCosyHitFinder.h"

#include <FairTask.h>           // for kSUCCESS, InitStatus, FairTask, kERROR
#include <TFile.h>              // for TFile
#include <TH1.h>                // for TH1F
#include <TMathBase.h>          // for Abs
#include <TTree.h>              // for TTree
#include <FairLogger.h>         // for LOG, Logger
#include <stdio.h>              // for sprintf, NULL
#include <iostream>             // for operator<<, basic_ostream, endl, cout
#include <map>                  // for map
#include <set>                  // for set
#include <vector>               // for vector
#include "CbmStsAddress.h"      // for GetElementId, kStsSide
#include "CbmStsCluster.h"      // for CbmStsCluster
#include "CbmStsHit.h"          // for CbmStsHit
#include "CbmStsSetup.h"        // for CbmStsSetup
#include <FairRootManager.h>    // for FairRootManager
#include <TClonesArray.h>       // for TClonesArray
#include <TMath.h>              // for DegToRad, Tan
#include <TVector3.h>           // for TVector3

using std::map;
using std::set;
using namespace std;

// ---- Default constructor -------------------------------------------
StsCosyHitFinder::StsCosyHitFinder()
  :FairTask("StsCosyHitFinder",1),
   stsClusters(NULL),
   fHits(NULL),
   fChain(new TChain("cbmsim")),
   fSensorId(13),
   fZ0(25.3),
   fZ1(99.3),
   fZ2(140.7),
   fTimeLimit(),
   fTimeShift(),
   cluster_size(),
   fCutName(""),
   fEvent(0)
{
//  fChain = new TChain("cbmsim");
  
  //--------------- Cosy 2013
  //fZ0 = 25.4;    // STS1
  //fZ1 = 70.9;    // STS2
  //fZ2 = 123.4;   // STS0 

  //--------------- Cosy 2014
//  fZ0 = 25.3;    // STS1
//  fZ1 = 99.3;    // STS3
//  fZ2 = 140.7;   // STS0 
  
  //--------------- Cosy 2014  
   fTimeLimit[0] = 16.;
   fTimeLimit[1] = 14.;
   fTimeLimit[2] = 22.;
   
   fTimeShift[0] = -3.;
   fTimeShift[1] = 3.;
   fTimeShift[2] = -6.;
   
   fCutName = "";
}
// --------------------------------------------------------------------

// ---- Destructor ----------------------------------------------------
StsCosyHitFinder::~StsCosyHitFinder()
{
  if (fHits){
    fHits->Delete();
    delete fHits;
  }
  if(stsClusters){
    stsClusters->Clear("C");
    stsClusters->Delete();
    delete stsClusters;
  }

}

// ----  Initialisation  ----------------------------------------------
void StsCosyHitFinder::SetParContainers()
{

  // Get Base Container
/*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
*/  
}
// --------------------------------------------------------------------

// ---- ReInit  -------------------------------------------------------
InitStatus StsCosyHitFinder::ReInit(){
  
/*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
*/
  
  return kSUCCESS;
}
// --------------------------------------------------------------------

// ---- Init ----------------------------------------------------------
InitStatus StsCosyHitFinder::Init()
{

  FairRootManager *ioman = FairRootManager::Instance();

  fHits = new TClonesArray("CbmStsHit", 1000);
  ioman->Register("StsHit", "Hit in STS", fHits, IsOutputBranchPersistent("StsHit"));
    
  stsClusters =(TClonesArray *)  ioman->GetObject("StsCluster");
  if ( !stsClusters  ) {
    LOG(error) << "No StsCluster array!";
    LOG(error) << "Task will be inactive";
    return kERROR;
  }
     
  if(fCutName != ""){
    LOG(info) << "Use inpute file for time cuts " << fCutName;
    TFile *file = new TFile(fCutName);
   
   TTree *cluster = (TTree*)file->Get("Cluster cuts");
   cluster->SetBranchAddress("TimeLimit",fTimeLimit);   
   cluster->SetBranchAddress("TimeShift",fTimeShift);   
   cluster->GetEntry(0);

  }  
    fEvent = 0;
    
   for(int i =0; i<3;i++)for(int ii =0; ii<2;ii++)
	  {
	    Char_t inName[25]; 
	    sprintf(inName,"cluster_size_STS%i_side%i",i,ii);
	    cluster_size[i][ii] = new TH1F(inName, inName, 100, -0.5, 99.5);
	  }
     
    
  cout << "-I- StsCosyHitFinder: Intialisation successfull " << kSUCCESS<< endl;
     
  return kSUCCESS;
  
} 
// --------------------------------------------------------------------

// ---- Exec ----------------------------------------------------------
void StsCosyHitFinder::Exec(Option_t*)
{
  fHits->Clear();  
//  if(fEvent%100000 == 0)cout << "-I- StsCosyHitFinder: ----- " << fEvent << endl; 

  Int_t nofSTSClusters = stsClusters->GetEntries();
  
  if(nofSTSClusters<1)
    return;
  
  vector<int>  sts_0p, sts_0n, sts_1p, sts_1n, sts_2p, sts_2n; 
  
//  double x=-100;
//  double y=-100;
  int clust[6]={0,0,0,0,0,0}; 
  
  for (Int_t iclus = 0; iclus < nofSTSClusters; iclus++)
    {
      int layer =-1;
      const CbmStsCluster* cluster = static_cast<const CbmStsCluster*>(stsClusters->At(iclus));
//      Int_t nofStrips = cluster->GetNofDigis();
      
//      double adc=-100;
      
      int station = CbmStsSetup::Instance()->GetStationNumber(cluster->GetAddress());
      int side = CbmStsAddress::GetElementId(cluster->GetAddress(),kStsSide);
//      int ch = CbmStsAddress::GetElementId(cluster->GetAddress(),kStsChannel);

      layer = 2*station+side;

      clust[layer]++;
      
      if(layer ==0)
	sts_0n.push_back(iclus);
      if(layer ==1)
	sts_0p.push_back(iclus);
      if(layer ==2)
	sts_1n.push_back(iclus);
      if(layer ==3)
	sts_1p.push_back(iclus);
      if(layer ==4)
	sts_2n.push_back(iclus);
      if(layer ==5)
	sts_2p.push_back(iclus);
    }
  
  for(UInt_t i=0; i< sts_0n.size(); i++)
    {
      Int_t frontClusterId = sts_0n[i];
      const CbmStsCluster* frontCluster = static_cast<const CbmStsCluster*>(stsClusters->At(sts_0n[i]));
      //Double_t frontChannel = frontCluster->GetMean();
      Double_t frontChannel = frontCluster->GetPosition();
      Double_t front_time = frontCluster->GetTime();            
      for(UInt_t k=0; k< sts_0p.size(); k++)
	{
	  Int_t backClusterId = sts_0p[k];
	  const CbmStsCluster* backCluster = static_cast<const CbmStsCluster*>(stsClusters->At(sts_0p[k]));
	  //Double_t backChannel = backCluster->GetMean();
	  Double_t backChannel = backCluster->GetPosition();
	  Double_t back_time = backCluster->GetTime();
	  if(TMath::Abs(back_time - front_time - fTimeShift[0]) < fTimeLimit[0])
	  {
	    
	      Double_t xHit = (frontChannel - 127)*0.005; 
	      Double_t yHit = (backChannel-127)*0.005; 

	      Double_t zHit = fZ0; 
	      TVector3 pos(xHit, yHit, zHit);
	      TVector3 dpos;
	      //dpos.SetXYZ(frontCluster->GetMeanError()*0.005,backCluster->GetMeanError()*0.005, 0.);
	      
	      Double_t global[] = {xHit, yHit, zHit};
	      Double_t error[] = { 0.0017, 0.0017, 0.};
	      
	      Int_t size = fHits->GetEntriesFast();
	     /*
		new ((*fHits)[size]) CbmStsHit(frontCluster->GetAddress(), 
					     pos, dpos, 0, frontClusterId, backClusterId, 
					     0, 0, 1,(back_time + front_time)/2.);
			*/
	     
		Double_t hitTimeError = 0.5 * TMath::Abs(back_time - front_time);
			
		new ((*fHits)[size]) CbmStsHit(frontCluster->GetAddress(), 
					     global, error, 0, frontClusterId, backClusterId, 
					     (back_time + front_time)/2., hitTimeError);
					     
					     
	      cluster_size[0][0]->Fill(frontCluster->GetNofDigis());
	      cluster_size[0][1]->Fill(backCluster->GetNofDigis());
	    }
	}
    }
  
  for(UInt_t i=0; i< sts_1n.size(); i++)
    {
      Int_t frontClusterId = sts_1n[i];
      const CbmStsCluster* frontCluster = static_cast<const CbmStsCluster*>(stsClusters->At(sts_1n[i]));
      //Double_t frontChannel = frontCluster->GetMean();
      Double_t frontChannel = frontCluster->GetPosition();
      Double_t front_time = frontCluster->GetTime();
      for(UInt_t k=0; k< sts_1p.size(); k++)
	{
	  Int_t backClusterId = sts_1p[k];
	  const CbmStsCluster* backCluster = static_cast<const CbmStsCluster*>(stsClusters->At(sts_1p[k]));
	  //Double_t backChannel = backCluster->GetMean();
	  Double_t backChannel = backCluster->GetPosition();
	  if(fSensorId==19)backChannel-=63;
	  Double_t back_time = backCluster->GetTime();	  
	  //if ((!fTriggeredMode && TMath::Abs(back_time -front_time - fTimeShift[1]) < fTimeLimit[1]) || fTriggeredMode)
	  if (TMath::Abs(back_time -front_time - fTimeShift[1]) < fTimeLimit[1])
	    {
	    
	      Double_t zHit = fZ1;
	    
	      Double_t xHit = (frontChannel - 34)*0.0058;
	      Double_t yHit = (frontChannel - backChannel)*0.0058/TMath::Tan(7.5*TMath::DegToRad());
	     		      
	      TVector3 pos(xHit, yHit, zHit);
	      TVector3 dpos;
	      //dpos.SetXYZ(frontCluster->GetMeanError()*0.0058,backCluster->GetMeanError()*0.0058, 0.);
	      dpos.SetXYZ(0.0017, 0.0017, 0.);
	      
	      Double_t global[] = {xHit, yHit, zHit};
	      Double_t error[] = { 0.0017, 0.0017, 0.};
	      
	      Int_t size = fHits->GetEntriesFast();
	      /*
		new ((*fHits)[size]) CbmStsHit(frontCluster->GetAddress(), 
					     pos, dpos, 0, frontClusterId, backClusterId, 
					     0, 0, 2,(back_time + front_time)/2.);
					     */
		Double_t hitTimeError = 0.5 * TMath::Abs(back_time - front_time);
			
		new ((*fHits)[size]) CbmStsHit(frontCluster->GetAddress(), 
					     global, error, 0, frontClusterId, backClusterId, 
					     (back_time + front_time)/2., hitTimeError);
			     
	      cluster_size[1][0]->Fill(frontCluster->GetNofDigis());
	      cluster_size[1][1]->Fill(backCluster->GetNofDigis());
	    }
	}
    }

  for(UInt_t i=0; i< sts_2n.size(); i++)
    {
      Int_t frontClusterId = sts_2n[i];
      const CbmStsCluster* frontCluster = static_cast<const CbmStsCluster*>(stsClusters->At(sts_2n[i]));
      //Double_t frontChannel = frontCluster->GetMean();
      Double_t frontChannel = frontCluster->GetPosition();
      Double_t front_time = frontCluster->GetTime();
      for(UInt_t k=0; k< sts_2p.size(); k++)
	{
	  Int_t backClusterId = sts_2p[k];
	  const CbmStsCluster* backCluster = static_cast<const CbmStsCluster*>(stsClusters->At(sts_2p[k]));
	  //Double_t backChannel = backCluster->GetMean();
	  Double_t backChannel = backCluster->GetPosition();
	  Double_t back_time = backCluster->GetTime();
	  if(TMath::Abs(back_time -front_time - fTimeShift[2]) < fTimeLimit[2])
	    {
	      Double_t zHit = fZ2; 
	      
	      Double_t xHit = (frontChannel-127)*0.005;
	      Double_t yHit = (128-backChannel)*0.005;
	      TVector3 pos(xHit, yHit, zHit);
	      TVector3 dpos;
	      //dpos.SetXYZ(frontCluster->GetMeanError()*0.005,backCluster->GetMeanError()*0.005, 0.);
	      dpos.SetXYZ(0.0017, 0.0017, 0.);
	      
	      Double_t global[] = {xHit, yHit, zHit};
	      Double_t error[] = { 0.0017, 0.0017, 0.};
	      
	      Int_t size = fHits->GetEntriesFast();
	      /*
		new ((*fHits)[size]) CbmStsHit(frontCluster->GetAddress(), 
					     pos, dpos, 0, frontClusterId, backClusterId, 
					     0, 0, 3,(back_time + front_time)/2.);
					     */
		Double_t hitTimeError = 0.5 * TMath::Abs(back_time - front_time);
			
		new ((*fHits)[size]) CbmStsHit(frontCluster->GetAddress(), 
					     global, error, 0, frontClusterId, backClusterId, 
					     (back_time + front_time)/2., hitTimeError);

	      cluster_size[2][0]->Fill(frontCluster->GetNofDigis());
	      cluster_size[2][1]->Fill(backCluster->GetNofDigis());
	    }
	}
    }
    
  fEvent++;
   
  stsClusters->Clear();

}
// --------------------------------------------------------------------

  // ---- Finish --------------------------------------------------------
  void StsCosyHitFinder::Finish()
  {
    for(int i=0; i<3; i++)for(int j=0; j<2; j++)cluster_size[i][j]->Write();
    
  LOG(info);
  LOG(info) << "=====================================================";
  LOG(info) << GetName() << ": Finish";
  LOG(info) << "=====================================================";
  LOG(info);
  }





  ClassImp(StsCosyHitFinder)

