#include "StsHitSet.h"

#include <FairTask.h>           // for InitStatus, kSUCCESS, FairTask, kERROR
#include <TH1.h>                // for TH1F
#include <TTree.h>              // for TTree
#include <FairLogger.h>         // for LOG
#include <stdio.h>              // for sprintf, NULL
#include <iostream>             // for operator<<, basic_ostream, endl, cout
#include <map>                  // for map
#include <set>                  // for set
#include <vector>               // for vector
#include "CbmStsAddress.h"      // for GetElementId, kStsSide
#include "CbmStsCluster.h"      // for CbmStsCluster
#include "CbmStsSetup.h"        // for CbmStsSetup
#include <FairRootManager.h>    // for FairRootManager
#include <TClonesArray.h>       // for TClonesArray
#include <TF1.h>                // for TF1

using std::map;
using std::set;
using namespace std;

// ---- Default constructor -------------------------------------------
StsHitSet::StsHitSet()
  :FairTask("StsHitSet",1),
   stsClusters(NULL),
   fChain(new TChain("cbmsim")),
   sts_time_diff(),
   cluster_time_diff(),
   fEvent(0)
{
//  fChain = new TChain("cbmsim");
  //  outFile=NULL;
}
// --------------------------------------------------------------------

// ---- Destructor ----------------------------------------------------
StsHitSet::~StsHitSet()
{
  if(stsClusters){
    stsClusters->Clear("C");
    stsClusters->Delete();
    delete stsClusters;
        
  }
}

// ----  Initialisation  ----------------------------------------------
void StsHitSet::SetParContainers()
{

  // Get Base Container
/*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
*/  
}
// --------------------------------------------------------------------

// ---- ReInit  -------------------------------------------------------
InitStatus StsHitSet::ReInit(){
  
/*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
*/  
  return kSUCCESS;
}
// --------------------------------------------------------------------

// ---- Init ----------------------------------------------------------
InitStatus StsHitSet::Init()
{

  FairRootManager *ioman = FairRootManager::Instance();

  stsClusters =(TClonesArray *)  ioman->GetObject("StsCluster");
  if ( !stsClusters  ) {
    LOG(error) << "No StsCluster array!";
    LOG(error) << "Task will be inactive";
    return kERROR;
  }
       
  // Time differences between p and n side - time cut setting for hit building

for (Int_t i=0;i<3;i++)
     {
       Char_t buf[25]; 
       sprintf(buf,"cluster_time_diff_STS%i",i); 
       cluster_time_diff[i]= new TH1F(buf, buf, 400, -1000, 1000); 
     }
     
  // Time differences between hits in different sensors - for hit selection from the same event
     
    sts_time_diff[0] = (TH1F*)cluster_time_diff[0]->Clone("time_diff_between_STS01");
    sts_time_diff[0]->SetTitle("time_diff_between_STS01");
    
    sts_time_diff[1] = (TH1F*)cluster_time_diff[0]->Clone("time_diff_between_STS02");
    sts_time_diff[1]->SetTitle("time_diff_between_STS02");
    
    sts_time_diff[2] = (TH1F*)cluster_time_diff[0]->Clone("time_diff_between_STS21");
    sts_time_diff[2]->SetTitle("time_diff_between_STS21");
          
    fEvent = 0;

     return kSUCCESS;
  
} 
// --------------------------------------------------------------------

// ---- Exec ----------------------------------------------------------
void StsHitSet::Exec(Option_t*)
{
 
  if(fEvent%100000 == 0)cout << "----- " << fEvent << endl;

  Int_t nofSTSClusters = stsClusters->GetEntries();
 
  vector<double>  sts_0, sts_1, sts_2; 
    
  for (Int_t i = 0; i < nofSTSClusters; i++)
   {
//      int layer =-1;
      const CbmStsCluster* cluster = static_cast<const CbmStsCluster*>(stsClusters->At(i));
      int station = CbmStsSetup::Instance()->GetStationNumber(cluster->GetAddress());
      int side = CbmStsAddress::GetElementId(cluster->GetAddress(),kStsSide);
      if(side==1)continue;
      
      Double_t front_time = cluster->GetTime();
    for (Int_t j = 0; j < nofSTSClusters; j++)
    {
      if(i==j)continue;
      
      const CbmStsCluster* cluster2 = static_cast<const CbmStsCluster*>(stsClusters->At(j));
      int station2 = CbmStsSetup::Instance()->GetStationNumber(cluster2->GetAddress());
      if(station != station2)continue;
      
      int side2 = CbmStsAddress::GetElementId(cluster2->GetAddress(),kStsSide);
      if(side2 == side)continue;
      
      Double_t back_time = cluster2->GetTime();
      cluster_time_diff[station]->Fill(back_time-front_time);
      Double_t Cluster_time = (back_time + front_time)/2.;
      if(station==0)sts_0.push_back(Cluster_time);
      else if(station==1)sts_1.push_back(Cluster_time);
      else if(station==2)sts_2.push_back(Cluster_time);
    }
   }
  for(UInt_t i=0; i<sts_0.size(); i++)
  {
    Double_t time0 = sts_0[i];
    for(UInt_t j=0; j<sts_1.size(); j++)
    {
      Double_t time1 = sts_1[j];
      sts_time_diff[0]->Fill(time0-time1);
    }
    for(UInt_t j=0; j<sts_2.size(); j++)
    {
      Double_t time1 = sts_2[j];
      sts_time_diff[1]->Fill(time0-time1);
    }
  }
  for(UInt_t i=0; i<sts_2.size(); i++)
  {
    Double_t time0 = sts_2[i];
    for(UInt_t j=0; j<sts_1.size(); j++)
    {
      Double_t time1 = sts_1[j];
      sts_time_diff[2]->Fill(time0-time1);
    }
  }
  
  stsClusters->Clear();
  
  fEvent++; 

}
// --------------------------------------------------------------------

  // ---- Finish --------------------------------------------------------
  void StsHitSet::Finish()
  {
    
  Double_t dT[3], dT0, dT1, dT2;
  Double_t T[3], T0, T1, T2;
  
for(Int_t i=0;i<3;i++)
  {    
    TF1 *fit = new TF1("fit", "gaus");
    cluster_time_diff[i]->Fit("fit","QME");
    T[i] = fit->GetParameter(1);
    dT[i] = fit->GetParameter(2)*2.;
      
    fit->Delete();
  }
  
  TTree * timeCluster = new TTree("Cluster cuts","Cluster cuts");

	 timeCluster->Branch("TimeLimit", dT, "dT[3]/D");
	 timeCluster->Branch("TimeShift", T, "T[3]/D");

	 timeCluster->Fill();
	 timeCluster->Write();    
	 
for(Int_t i=0;i<3;i++)
  {    
    TF1 *fit = new TF1("fit", "gaus");
    sts_time_diff[i]->Fit("fit","QME");
    
    if(i==0)
    {
    T0 = fit->GetParameter(1);
    dT0 = fit->GetParameter(2)*2.;}
    else if(i==1)
    {
    T1 = fit->GetParameter(1);
    dT1 = fit->GetParameter(2)*2.;}
    else 
    {
    T2 = fit->GetParameter(1);
    dT2 = fit->GetParameter(2)*2.;}
      
    fit->Delete();
  }
  
  TTree * timeHit0 = new TTree("STS0-STS1","STS0-STS1");

	 timeHit0->Branch("TimeLimit", &dT0, "dT0/D");
	 timeHit0->Branch("TimeShift", &T0, "T0/D");

	 timeHit0->Fill();
	 timeHit0->Write();    
	 
  TTree * timeHit1 = new TTree("STS0-STS2","STS0-STS2");

	 timeHit1->Branch("TimeLimit", &dT1, "dT1/D");
	 timeHit1->Branch("TimeShift", &T1, "T1/D");

	 timeHit1->Fill();
	 timeHit1->Write();    
	 
  TTree * timeHit2 = new TTree("STS2-STS1","STS2-STS1");

	 timeHit2->Branch("TimeLimit", &dT2, "dT2/D");
	 timeHit2->Branch("TimeShift", &T2, "T2/D");

	 timeHit2->Fill();
	 timeHit2->Write();    
	 
	 
	 
    for(int i=0; i<3; i++)cluster_time_diff[i]->Write();    
    for(int i=0; i<3; i++)sts_time_diff[i]->Write();
   
  }





  ClassImp(StsHitSet)

