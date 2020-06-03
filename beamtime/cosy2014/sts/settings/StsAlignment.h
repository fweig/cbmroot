/** StsAlignment.h
 *@author Anna Senger <a.senger@gsi.de>
 **
 ** Task to calculate alignment parameters.
 **/

#ifndef STSALIGNMENT_H
#define STSALIGNMENT_H 1

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t, Option_t
#include <TChain.h>      // for TChain
#include <TString.h>     // for TString
#include <map>           // for map
#include <vector>        // for vector
#include <FairTask.h>    // for FairTask, InitStatus

#include "TH1.h"         // for RootCling

class CbmStsHit;
class TClonesArray;


class StsAlignment : public FairTask
{

 public:

  StsAlignment();
  ~StsAlignment();
  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  Int_t GetEntries () { return fChain->GetEntries();}
  
  void SetCutFileName(TString name){fCutName = name;}  

  void SetChi2Cut(Double_t chi2x, Double_t chi2y){fChi2X=chi2x;fChi2Y=chi2y;}
  void SetNofDetectors(Int_t NofD){fNofDet=NofD;}
  void SetTimeLimit(Double_t time[3]) { for(int i=0; i<3; i++)fTimeLimits[i]=time[i]; }
  void SetTimeShift(Double_t time[3]) { for(int i=0; i<3; i++)fTimeShifts[i]=time[i]; }  

 private:

  TClonesArray*     fHits;   
  TChain *fChain;           
     

  StsAlignment(const StsAlignment&);
  StsAlignment operator=(const StsAlignment&);

  void Reset();
  void Finish();

  Bool_t FindTracks(); 

 // Map for layered hits
  std::map< Int_t , std::vector<CbmStsHit*> > fMapPts; //!

  Double_t fTimeLimits[3];
  Double_t fTimeShifts[3];
  
  TH1F* resX[3]; 
  TH1F* resY[3]; 

  Double_t fChi2X, fChi2Y;
  Double_t fZ[3]; 
  Double_t fdZ[3]; 

  TString fCutName;

  Int_t fNofDet;
  Int_t fEvent;
  
  ClassDef(StsAlignment,1);

};

#endif
