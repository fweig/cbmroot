/** StsCosyHitFinder.h
 *@author Florian Uhlig <f.uhlig@gsi.de>
 **
 ** Task to find neighboring fibres which are above
 ** the base line.
 ** Create as an output an array of
 ** the digis belonging to the cluster.
 **
 ** 2015 Anna Senger <a.senger@gsi.de>
 **/

#ifndef STSCOSYHITFINDER_H
#define STSCOSYHITFINDER_H 

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Option_t
#include <TChain.h>      // for TChain
#include <TString.h>     // for TString
#include <FairTask.h>    // for InitStatus, FairTask

class TClonesArray;
class TH1F;

class StsCosyHitFinder : public FairTask
{
  
 public:
  
  /**
   * Default constructor.
   */
  StsCosyHitFinder();
  
  /**
   * Default destructor.
   */
  ~StsCosyHitFinder();
 
  /** Initialisation **/
  virtual InitStatus ReInit();
  virtual InitStatus Init();
  virtual void SetParContainers();
  
  /** Executed task **/
  virtual void Exec(Option_t * option);
  Int_t GetEntries () { return fChain->GetEntries();}  
  
  void SetZ(Double_t z0, Double_t z1, Double_t z2) { fZ0 =  z0; fZ1 =  z1; fZ2 =  z2; }
  
  void SetTimeLimit(Double_t time[3]) { for(int i=0; i<3; i++)fTimeLimit[i]=time[i]; }
  void SetTimeShift(Double_t time[3]) { for(int i=0; i<3; i++)fTimeShift[i]=time[i]; }
  
  void SetSensorId(Int_t sensor) { fSensorId=sensor; } //13 for W13, 10 for W10, 18 for W18
  
  void SetCutFileName(TString name){fCutName = name;}  

  /** Finish task **/
  virtual void Finish();
 
 private:

  TClonesArray*     stsClusters;    /** Input array of CbmStsCluster **/

  TClonesArray*     fHits;    /** Output array of CbmHits **/
  
  TChain *fChain;           
  
  Int_t    fSensorId;
  Double_t fZ0, fZ1, fZ2;
  Double_t fTimeLimit[3];
  Double_t fTimeShift[3];
  
  TH1F* cluster_size[3][2]; //!
  
  TString fCutName;

  Int_t fEvent;

  StsCosyHitFinder(const StsCosyHitFinder&);
  StsCosyHitFinder& operator=(const StsCosyHitFinder&);

  ClassDef(StsCosyHitFinder,1);
  
};
#endif
