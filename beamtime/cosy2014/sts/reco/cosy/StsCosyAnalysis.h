/** class StsCosyAnalysis.h
 *@author Olga Bertini @ Anna Senger <a.senger@gsi.de>
 **
 ** 
 **/


#ifndef STSCOSYANALYSIS_H
#define STSCOSYANALYSIS_H 1

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t, Option_t
#include <TChain.h>      // for TChain
#include <TString.h>     // for TString
#include <map>           // for map
#include <vector>        // for vector
#include <FairTask.h>    // for FairTask, InitStatus

#include "TH1.h"          // for RootCling
#include "TH2.h"          // for RootCling

class CbmStsHit;
class TClonesArray;

class StsCosyAnalysis : public FairTask
{

 public:

  StsCosyAnalysis();
  ~StsCosyAnalysis();
  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  Int_t GetEntries () { return fChain->GetEntries();}

  void SetChi2Cut(Double_t chi2x, Double_t chi2y){fChi2X=chi2x;fChi2Y=chi2y;}
  
  void SetAlignFileName(TString name){fAlignName = name;}
  void SetCutFileName(TString name){fCutName = name;}  

  void SetNofDetectors(Int_t NofH){fNofDet=NofH;}
  void SetTimeLimit(Double_t timeArr[3]) { for(int i=0; i<3; i++)fTimeLimit[i]=timeArr[i]; }
  void SetTimeShift(Double_t timeArr[3]) { for(int i=0; i<3; i++)fTimeShift[i]=timeArr[i]; }
  
  void SetTrackSelectType(Int_t type) {fTrackSelectType = type;}
  void SetCoordPrecision(Double_t x, Double_t y){fXbin = x; fYbin = y;}
  void SetEventSelectType(Int_t type) {fEventSelectType = type;}
  
 private:

  TClonesArray*     fHits;   
  TChain *fChain;           
     

  StsCosyAnalysis(const StsCosyAnalysis&);
  StsCosyAnalysis operator=(const StsCosyAnalysis&);

  void Reset();
  void Finish();

  Bool_t FindTracks(); 

 // Map for layered hits
  std::map< Int_t , std::
  vector<CbmStsHit*> > fMapPts; //!
  
  std::vector <Int_t> XY_events, proj_events;
  std::vector <Double_t> X_STS1, Y_STS1, X_proj, Y_proj;
  
  TH1F* resX[3]; 
  TH1F* resY[3];
  TH2F* X_ResX[3]; 
  TH2F* Y_ResY[3];
  TH1F* chi2X; 
  TH1F* chi2Y;
  TH2F* chi2XY;
  
  TH2F * XY[3];  
  TH2F * XY_eff;  
  TH2F * XY_woSTS1[2];
  TH2F * XY_STS1projection;  
  TH2F * XY_STS1projection_select;  

  TH2F *XX_STS_0_1;
  TH2F *XX_STS_0_2;
  TH2F *XX_STS_0_2woSTS1;
  TH2F *XX_STS_2_1;
  
  TH2F *YY_STS_0_1;
  TH2F *YY_STS_0_2;
  TH2F *YY_STS_0_2woSTS1;
  TH2F *YY_STS_2_1;

  TH1F* NofTracks;  
  
  Double_t fChi2X, fChi2Y;
  Double_t fX[3], fY[3]; // alignment
  Double_t fXbin, fYbin; // in Xm
  
  TString fAlignName;
  TString fCutName;
  
  Int_t fNofDet;
  Double_t fTimeLimit[3];
  Double_t fTimeShift[3];
  Int_t fTrackSelectType, fEventSelectType, fEvent;
  ClassDef(StsCosyAnalysis,1);

};

#endif
