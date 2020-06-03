#include "StsClusterSet.h"

#include <FairTask.h>           // for InitStatus, kSUCCESS, FairTask, kERROR
#include <TH1.h>                // for TH1F
#include <TTree.h>              // for TTree
#include <FairLogger.h>         // for LOG
#include <stdio.h>              // for sprintf, NULL
#include <iostream>             // for operator<<, basic_ostream, endl, cout
#include "CbmStsAddress.h"      // for GetElementId, kStsSide
#include "CbmStsDigi.h"         // for CbmStsDigi
#include "CbmStsSetup.h"        // for CbmStsSetup
#include <FairRootManager.h>    // for FairRootManager
#include <TClonesArray.h>       // for TClonesArray
#include <TF1.h>                // for TF1
#include "CbmDigiManager.h"

using std::map;
using std::set;
using namespace std;

// ---- Default constructor -------------------------------------------
StsClusterSet::StsClusterSet()
  :FairTask("StsClusterSet",1),
   time_diff_strips(),
   cluster_adc(),
   fChargeMinStrip(),
   fChargeMaxStrip(),
   fDigiMap(),
   fEvent(0)
{  
  for(int i=0; i<3; i++)
  {
    fChargeMinStrip[i]= 50.;
    fChargeMaxStrip[i]= 500.;
  }
}
// --------------------------------------------------------------------

// ---- Destructor ----------------------------------------------------
StsClusterSet::~StsClusterSet()
{

}

// ----  Initialisation  ----------------------------------------------
void StsClusterSet::SetParContainers()
{

  // Get Base Container
/*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
*/  
}
// --------------------------------------------------------------------

// ---- ReInit  -------------------------------------------------------
InitStatus StsClusterSet::ReInit(){
  
/*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
*/
  
  return kSUCCESS;
}
// --------------------------------------------------------------------

// ---- Init ----------------------------------------------------------
InitStatus StsClusterSet::Init()
{

//  FairRootManager *ioman = FairRootManager::Instance();
  
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if ( ! fDigiMan->IsPresent(ECbmModuleId::kSts) ) {
    LOG(error) << "No StsDigi array!";
    LOG(error) << "Task will be inactive";
    return kERROR;
  }
  
  // Time differences between strips - time cut cluster building
  
   for(int i =0; i<3;i++)
    {
	  Char_t inName[200];
	  sprintf(inName,"strips_time_diff_STS%i",i);	  
	  time_diff_strips[i] = new TH1F(inName, inName, 1000, -1000, 1000);
	  for(int ii =0; ii<2;ii++)
	  {
	    sprintf(inName,"cluster_charge_STS%i_side%i",i,ii);
	    cluster_adc[i][ii] = new TH1F(inName, inName, 1000, 0, 1000);
	  }
    }  

  fEvent = 0;
   
  return kSUCCESS;
  
} 
// --------------------------------------------------------------------

// ---- Exec ----------------------------------------------------------
void StsClusterSet::Exec(Option_t*)
{
  
  if(fEvent%100000 == 0)cout << "----- " << fEvent << endl;
  
  for (auto mapIt=fDigiMap.begin(); mapIt!=fDigiMap.end(); mapIt++)
    {
      ((*mapIt).second).clear();
    }
  fDigiMap.clear();
  
  set<Int_t> layerSet;
  
  const CbmStsDigi* digi = NULL;
//  CbmStsDigi* digi2 = NULL;
  
  map<const CbmStsDigi*, Int_t> fIndices;
  
  Int_t nofDigis = fDigiMan->GetNofDigis(ECbmModuleId::kSts);

  for (Int_t iDigi=0; iDigi < nofDigis; iDigi++ ) 
    {
      digi = fDigiMan->Get<CbmStsDigi>(iDigi);
	int station = CbmStsSetup::Instance()->GetStationNumber(digi->GetAddress());
      int side = CbmStsAddress::GetElementId(digi->GetAddress(),kStsSide);
      Int_t layer= 2*station+side;
      
      if(digi->GetCharge() > fChargeMinStrip[station] && digi->GetCharge() < fChargeMaxStrip[station])
      {
	layerSet.insert(layer);
	fDigiMap[layer].insert(digi);
	fIndices[digi]=iDigi;          
      }
    }
  
  set <const CbmStsDigi*, classcomp2> digiSet;

  for (set<Int_t>::iterator i = layerSet.begin(); i != layerSet.end(); i++) 
    {
      digiSet = fDigiMap[*i];
      Bool_t newCluster = kTRUE;
      Bool_t Cluster = kFALSE;
      Int_t stripNr = -1;
      Int_t stripNrPrev = -1;
//      Int_t cluster_size =0;
      Double_t time = -1;
      Double_t timePrev = -1;
      Double_t sumW = 0;
      int station = -1;
      int side = -1;
      for (set<const CbmStsDigi*, classcomp2>::iterator j = digiSet.begin(); j != digiSet.end(); j++)
	{
	  if (newCluster) 
	    {
	      Int_t index = fIndices[(*j)];
	      //----time----
	      digi = fDigiMan->Get<CbmStsDigi>(index);
	      timePrev = digi->GetTime();
	      stripNrPrev = (*j)->GetChannel();
	      sumW = digi->GetCharge();
	      newCluster = kFALSE;
	    } 
	  else 
	    {
	      Int_t index = fIndices[(*j)];
        digi = fDigiMan->Get<CbmStsDigi>(index);
	      time = digi->GetTime();
	      stripNr = (*j)->GetChannel();
	      station = CbmStsSetup::Instance()->GetStationNumber(digi->GetAddress());
	      side = CbmStsAddress::GetElementId(digi->GetAddress(),kStsSide);
	      
	      if(1 == stripNr-stripNrPrev)
	      {
		time_diff_strips[station]->Fill(time - timePrev);	      
		sumW += digi->GetCharge();
		Cluster = kTRUE;
	      }	      
	    }
	}
	if(station < 0 || side < 0)continue;
	if(Cluster || (!newCluster && !Cluster))cluster_adc[station][side]->Fill(sumW);	
    }   

    fEvent++; 
    

}
// --------------------------------------------------------------------

  // ---- Finish --------------------------------------------------------
  void StsClusterSet::Finish()
  {
    
  Double_t dT0, dT1, dT2;
  Double_t T0, T1, T2;
  
  // Fit residuals
  
for(Int_t i=0;i<3;i++)
  {
    TF1 *fit = new TF1("fit", "gaus");
    time_diff_strips[i]->Fit("fit","QME");
            
    if(i==0){
      T0 = fit->GetParameter(1);
      dT0 = fit->GetParameter(2)*2.;}
      else if(i==1){
      T1 = fit->GetParameter(1);
      dT1 = fit->GetParameter(2)*2.;}
      else {
      T2 = fit->GetParameter(1);
      dT2 = fit->GetParameter(2)*2.;}
      
    fit->Delete();
 }
  
  TTree * sts0 = new TTree("STS0","STS0");

	 sts0->Branch("TimeLimit", &dT0, "dT0/D");
	 sts0->Branch("TimeShift", &T0, "T0/D");

	 sts0->Fill();
	 sts0->Write();      
	 
  TTree * sts1 = new TTree("STS1","STS1");

	 sts1->Branch("TimeLimit", &dT1, "dT1/D");
	 sts1->Branch("TimeShift", &T1, "T1/D");

	 sts1->Fill();
	 sts1->Write(); 
	 
  TTree * sts2 = new TTree("STS2","STS2");

	 sts2->Branch("TimeLimit", &dT2, "dT2/D");
	 sts2->Branch("TimeShift", &T2, "T2/D");

	 sts2->Fill();
	 sts2->Write();      
	 
    for(int i =0; i<3;i++)
    time_diff_strips[i]->Write();
    for(int i=0; i<3; i++)for(int j=0; j<2; j++)cluster_adc[i][j]->Write();
    

  }

  ClassImp(StsClusterSet)

