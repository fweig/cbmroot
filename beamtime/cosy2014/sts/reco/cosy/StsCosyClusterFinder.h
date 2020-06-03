/** StsCosyClusterFinder.h
 *@author Florian Uhlig <f.uhlig@gsi.de>
 **
 ** Task to find neighboring fibres which are above
 ** the base line.
 ** Create as an output an array of
 ** the digis belonging to the cluster.
 **
 ** 2015 Anna Senger <a.senger@gsi.de>
 **/

#ifndef STSCOSYCLUSTERFINDER_H
#define STSCOSYCLUSTERFINDER_H 

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t, Option_t
#include <TString.h>     // for TString
#include <map>           // for map
#include <set>           // for set
#include <vector>
#include "CbmStsDigi.h"  // for CbmStsDigi
#include <FairTask.h>    // for InitStatus, FairTask


class TH1F;


struct classcomp1 {
  bool operator() (const CbmStsDigi* lhs, const CbmStsDigi* rhs) const
    {return lhs->GetChannel() < rhs->GetChannel();}
};
    
class StsCosyClusterFinder : public FairTask
{
  
 public:
  
  /**
   * Default constructor.
   */
  StsCosyClusterFinder();
  
  /**
   * Default destructor.
   */
  ~StsCosyClusterFinder();
 
  /** Initialisation **/
  virtual InitStatus ReInit();
  virtual InitStatus Init();
  virtual void SetParContainers();
  
  /** Executed task **/
  virtual void Exec(Option_t * option);
  
  void SetTriggeredMode(Bool_t mode) { fTriggeredMode = mode; }
  void SetTriggeredStation(Int_t station) { fTriggeredStation = station ; }
  
  void SetChargeLimitsStrip(Double_t min[3], Double_t max[3]) { for(int i=0; i<3; i++){fChargeMinStrip[i] = min[i]; fChargeMaxStrip[i] = max[i];} }
  void SetChargeMinCluster(Double_t min[3]) { for(int i=0; i<3; i++)fChargeMinCluster[i]=min[i]; }
  
  void SetTimeLimit(Double_t time[3]) { for(int i=0; i<3; i++)fTimeLimit[i]=time[i]; }
  void SetTimeShift(Double_t time[3]) { for(int i=0; i<3; i++)fTimeShift[i]=time[i]; }
  
  void SetCutFileName(TString name){fCutName = name;}  

/** Finish task **/
  virtual void Finish();

 private:

  const std::vector<CbmStsDigi>*  fDigis; /** Input array of CbmStsDigi **/
  TClonesArray*     fClusters;      /** candidates array of CbmStsCluster **/
  TClonesArray*     finalClusters;  /** Output array of CbmStsCluster **/

  Bool_t fTriggeredMode; ///< Flag if data is taken in triggered mode
  Int_t  fTriggeredStation;
  
  Double_t fChargeMinStrip[3];
  Double_t fChargeMaxStrip[3];
  Double_t fChargeMinCluster[3];
  
  Double_t fTimeLimit[3];
  Double_t fTimeShift[3];

  TString fCutName;

  TH1F* cluster_size[3][2]; //!
  
  Int_t fEvent;

  std::map<Int_t, std::set<const CbmStsDigi*, classcomp1> > fDigiMap;   /** digis per hodo layer **/ 

  StsCosyClusterFinder(const StsCosyClusterFinder&);
  StsCosyClusterFinder& operator=(const StsCosyClusterFinder&);

  ClassDef(StsCosyClusterFinder,1);
  
};
#endif
