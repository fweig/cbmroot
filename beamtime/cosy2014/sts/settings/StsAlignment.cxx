#include "StsAlignment.h"

#include <FairTask.h>           // for kSUCCESS, FairTask, InitStatus, kERROR
#include <TFile.h>              // for TFile
#include <TH1.h>                // for TH1F
#include <TMathBase.h>          // for Abs
#include <FairLogger.h>         // for LOG, Logger
#include <stdio.h>              // for sprintf, NULL
#include <iostream>             // for operator<<, basic_ostream, endl, cout
#include <utility>              // for pair, __unwrap_reference<>::type, mak...
#include <vector>               // for vector
#include "CbmStsHit.h"          // for CbmStsHit
#include "CbmStsSetup.h"        // for CbmStsSetup
#include <FairRootManager.h>    // for FairRootManager
#include <TClonesArray.h>       // for TClonesArray
#include <TF1.h>                // for TF1
#include <TGraphErrors.h>       // for TGraphErrors
#include <TTree.h>              // for TTree
#include <TVector3.h>           // for TVector3

using std::cout;
using std::endl;
using std::vector;

StsAlignment::StsAlignment() :
  FairTask("StsAlignment",1),
  fHits(NULL),
  fChain(new TChain("cbmsim")),
  fMapPts(),
  fTimeLimits(),
  fTimeShifts(),
  resX(),
  resY(),  
  fChi2X(1e6),
  fChi2Y(1e6),
  fZ(),
  fdZ(),
  fCutName(""),
  fNofDet(3),
  fEvent(0)
{ 
//  fChain = new TChain("cbmsim");
  fTimeLimits[0] = 16;
  fTimeLimits[1] = 16;
  fTimeLimits[2] = 16;
  
  fTimeShifts[0] = 0;
  fTimeShifts[1] = -16;
  fTimeShifts[2] = -16;
  
/*
  fEvent = 0;
  
  fCutName = "";
*/
}

StsAlignment::~StsAlignment() 
{
  if ( fHits ) {
    fHits->Delete();
    delete fHits;
  }
}


InitStatus StsAlignment::Init() {

  // Get RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman )
    {
      LOG(fatal) << "StsAlignment::Init: noRootManager !"; 
      return kFATAL;
    }
  // Get input arrays
  
  fHits =(TClonesArray *)  ioman->GetObject("StsHit");
  if ( ! fHits ) {
    LOG(error) << "No StsHits array!";
    LOG(error) << "Task will be inactive";
    return kERROR;
  }

  for(int i=0;i<fNofDet;i++)
    fMapPts.insert(make_pair(i,vector<CbmStsHit*>()));   
  
   for (Int_t i=0;i<3;i++)
     {
       Char_t buf[25]; 
       sprintf(buf,"ResX_STS%i",i); 
       resX[i] = new TH1F(buf,buf,400, -1,1);
       
       sprintf(buf,"ResY_STS%i",i); 
       resY[i] = new TH1F(buf,buf,400, -1,1); 
     }	
    
    if(fCutName != "" ){
    LOG(info) << "Use inpute file for time cuts " << fCutName;
    TFile *file_cuts = new TFile(fCutName);
   
   
   TTree *timeHit0 = (TTree*)file_cuts ->Get("STS0-STS1");
   timeHit0->SetBranchAddress("TimeLimit",&fTimeLimits[0]);   
   timeHit0->SetBranchAddress("TimeShift",&fTimeShifts[0]);   
   timeHit0->GetEntry(0);

   TTree *timeHit1 = (TTree*)file_cuts ->Get("STS0-STS2");
   timeHit1->SetBranchAddress("TimeLimit",&fTimeLimits[1]);   
   timeHit1->SetBranchAddress("TimeShift",&fTimeShifts[1]);   
   timeHit1->GetEntry(0);

   TTree *timeHit2 = (TTree*)file_cuts ->Get("STS2-STS1");
   timeHit2->SetBranchAddress("TimeLimit",&fTimeLimits[2]);   
   timeHit2->SetBranchAddress("TimeShift",&fTimeShifts[2]);   
   timeHit2->GetEntry(0);
  } 
  
  
  cout << "-I- StsAlignment: Intialisation successfull " << kSUCCESS<< endl;
   return kSUCCESS;

}

// -----   Public method Exec   --------------------------------------------
void StsAlignment::Exec(Option_t*) {
    
  Reset(); 
  
  if(fEvent%100000 == 0)cout << "----- " << fEvent << endl;
  
  
  CbmStsHit* hit = NULL;

  Int_t nofHits = fHits->GetEntries();
 
  // Loop over hits
  for (Int_t iHit=0; iHit<nofHits; iHit++) 
    {
      hit = (CbmStsHit*) fHits->At(iHit);      
      
      Int_t detId = CbmStsSetup::Instance()->GetStationNumber(hit->GetAddress());
      vector<CbmStsHit*>& vlist =  fMapPts[detId];
      vlist.push_back((CbmStsHit*) hit); 
    }   

    // Find the tracks 
   FindTracks();
   
   fEvent++; 
}


Bool_t StsAlignment::FindTracks(){
  
  // Check the content per detector   
  for (int idet=0; idet<fNofDet;idet++)
    {
      // get the vector     
      vector<CbmStsHit*>& v =  fMapPts[idet];
      // select at least 1 points per detector
      if (v.size()==0) return kFALSE;
    }  
  
  vector<CbmStsHit*>& v_sts0 =  fMapPts[0];
  vector<CbmStsHit*>& v_sts1 =  fMapPts[1];
  vector<CbmStsHit*>& v_sts2 =  fMapPts[2];
  
  CbmStsHit* pt0=NULL; 
  CbmStsHit* pt1=NULL; 
  CbmStsHit* pt2=NULL; 
  
  
  TVector3 sts0_pos, sts1_pos, sts2_pos;
  int Npoints = fNofDet;
  
  double x[Npoints];
  double ex[Npoints];
  double z[Npoints];
  double ez[Npoints];
  double y[Npoints];
  double ey[Npoints];


    for(UInt_t i=0; i< v_sts0.size() ; i++) // STS1 @ Cosy2014
    {
      pt0 = (CbmStsHit*) v_sts0[i];
      pt0->Position(sts0_pos);       
      x[0]= sts0_pos.X();
      y[0]= sts0_pos.Y();
      z[0]= sts0_pos.Z();
      ex[0]= pt0->GetDx();
      ey[0]= pt0->GetDy();
      ez[0]= 0.5;
      fZ[0] = z[0];
      fdZ[0] = ez[0];
	  
	for(UInt_t j=0; j< v_sts1.size() ; j++) // STS3 @ Cosy2014
	{
	  pt1 = (CbmStsHit*) v_sts1[j];
	  if(TMath::Abs(pt0->GetTime()-pt1->GetTime() - fTimeShifts[0])>fTimeLimits[0])continue;
	      
	  pt1->Position(sts1_pos); 
	  x[1]= sts1_pos.X();
	  y[1]= sts1_pos.Y();	    
	  z[1]= sts1_pos.Z();
	  ex[1]= pt1->GetDx();
	  ey[1]= pt1->GetDy();
	  ez[1]= 0.5;
	  fZ[1] = z[1];
	  fdZ[1] = ez[1];

	    for(UInt_t k=0; k< v_sts2.size() ; k++) // STS0 @ Cosy2014
	    {
	      pt2 = (CbmStsHit*) v_sts2[k];
	      if(TMath::Abs(pt0->GetTime()-pt2->GetTime()- fTimeShifts[1])>fTimeLimits[1])continue;
	      if(TMath::Abs(pt2->GetTime()-pt1->GetTime()- fTimeShifts[2])>fTimeLimits[2])continue;
	      
	      pt2->Position(sts2_pos); 
	      x[2]= sts2_pos.X();
	      y[2]= sts2_pos.Y();	
	      z[2]= sts2_pos.Z();
	      ex[2]= pt2->GetDx();
	      ey[2]= pt2->GetDy();
	      ez[2]= 0.5;
	      fZ[2] = z[2];
	      fdZ[2] = ez[2];
	      	   	      
	      // Fit data numerically:
	      // ---------------------
	      TGraphErrors* graph;
	      TGraphErrors* graph1;
	      graph = new TGraphErrors(Npoints,z,x,ez,ex);

	      TF1 *fit = new TF1("fit", "pol1");	      
	      graph->Fit("fit","QME");
	      
	      double xChi2 = fit->GetChisquare();
	      double xNdof = fit->GetNDF();
	      double p0 = fit->GetParameter(0);
	      double p1 = fit->GetParameter(1);
	      
	      graph1 = new TGraphErrors(Npoints,z,y,ez,ey);
	      
	      TF1 *fit1 = new TF1("fit1", "pol1");
	      graph1->Fit("fit1","QME");
	      
	      double yChi2 = fit1->GetChisquare();
	      double yNdof = fit1->GetNDF();
	      double p2 = fit1->GetParameter(0);
	      double p3 = fit1->GetParameter(1);
	      
	      graph1->Delete();
	      graph->Delete();
	      fit->Delete();
 	      fit1->Delete();
	      
	      if(xChi2/xNdof > fChi2X || yChi2/yNdof > fChi2Y)continue; 
	      for(int m=0; m<Npoints; m++)
	      {
		resX[m]->Fill(x[m]-p0-p1*z[m]);
		resY[m]->Fill(y[m]-p2-p3*z[m]);
	      }	           	    
	  }//!sts1 //STS0 @ Cosy2014
	}//!sts2 //STS3 @ Cosy2014
    }//!sts0 //STS1 @ Cosy2014
	 	      
  return kTRUE;
}

void StsAlignment::Reset() {
  fMapPts.clear();
}


void StsAlignment::Finish(){
  
  Double_t X[3], Y[3];
  Double_t dX[3], dY[3];
  
  // Fit residuals
  
for(Int_t i=0;i<3;i++)
  {
    TF1 *fitX = new TF1("fitX", "gaus");
    resX[i]->Fit("fitX","QME");
    X[i] = fitX->GetParameter(1);
    dX[i] = fitX->GetParameter(2);
    fitX->Delete();
    
    TF1 *fitY = new TF1("fitY", "gaus");
    resY[i]->Fit("fitY","QME");
    Y[i] = fitY->GetParameter(1);
    dY[i] = fitY->GetParameter(2);    
    fitY->Delete();
  }
  
  // Calculate alignment parameters
  LOG(info);
  LOG(info) << "=====================================================";
  LOG(info) << GetName() << ": Finish. Calculate alignment parameters";
  LOG(info) << "=====================================================";
  LOG(info);  
  
TF1 *fitX = new TF1("fitX", "pol1");
TF1 *fitY = new TF1("fitY", "pol1");
TGraphErrors* graphX = new TGraphErrors(3,fZ, X, fdZ, dX);
TGraphErrors* graphY = new TGraphErrors(3,fZ, Y, fdZ, dY);
graphX->Fit("fitX", "QEM");
graphY->Fit("fitY", "QEM");


Double_t X0=X[0]-fitX->GetParameter(0)-fitX->GetParameter(1)*fZ[0];
Double_t X1=X[1]-fitX->GetParameter(0)-fitX->GetParameter(1)*fZ[1];
Double_t X2=X[2]-fitX->GetParameter(0)-fitX->GetParameter(1)*fZ[2];

Double_t Y0=Y[0]-fitY->GetParameter(0)-fitY->GetParameter(1)*fZ[0];
Double_t Y1=Y[1]-fitY->GetParameter(0)-fitY->GetParameter(1)*fZ[1];
Double_t Y2=Y[2]-fitY->GetParameter(0)-fitY->GetParameter(1)*fZ[2];

TTree * data = new TTree("Alignment",	"Alignment");
	 data->Branch("dX0", &X0, "X0/D");
	 data->Branch("dX1", &X1, "X1/D");
	 data->Branch("dX2", &X2, "X2/D");

	 data->Branch("dY0", &Y0, "Y0/D");
	 data->Branch("dY1", &Y1, "Y1/D");
	 data->Branch("dY2", &Y2, "Y2/D");

	 data->Fill();
	 data->Write();     

  for(Int_t i=0;i<3;i++){resX[i]->Write(); resY[i]->Write();}
  
  LOG(info);
  LOG(info) << "=====================================================";
  LOG(info) << GetName() << ": Finish";
  LOG(info) << "=====================================================";
  LOG(info);  
}




ClassImp(StsAlignment)
