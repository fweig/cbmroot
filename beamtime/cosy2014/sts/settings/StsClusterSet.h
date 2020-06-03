/** StsClusterSet.h
 *@author Anna Senger <a.senger@gsi.de>
 **
 ** Task to calculate time differences for the cluster building.
 **/

#ifndef STSCLUSTERSET_H
#define STSCLUSTERSET_H 

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Option_t
#include <map>           // for map
#include <set>           // for set
#include "CbmStsDigi.h"  // for CbmStsDigi
#include <FairTask.h>    // for InitStatus, FairTask

#include "TH1.h"         // for RootCling
#include "TClonesArray.h"//for RootCling

class CbmDigiManager;

struct classcomp2 {
  bool operator() (const CbmStsDigi* lhs, const CbmStsDigi* rhs) const
    {return lhs->GetChannel() < rhs->GetChannel();}
};
    
class StsClusterSet : public FairTask
{
  
 public:
  
  /**
   * Default constructor.
   */
  StsClusterSet();
  
  /**
   * Default destructor.
   */
  ~StsClusterSet();
 
  /** Initialisation **/
  virtual InitStatus ReInit();
  virtual InitStatus Init();
  virtual void SetParContainers();
  
  /** Executed task **/
  virtual void Exec(Option_t * option);
  
  /** Selection of the strips with signal charge **/  
  void SetChargeLimitsStrip(Double_t min[3], Double_t max[3]) { for(int i=0; i<3; i++){fChargeMinStrip[i] = min[i]; fChargeMaxStrip[i] = max[i];} }

  /** Finish task **/
  virtual void Finish();

 private:

  CbmDigiManager* fDigiMan = nullptr; //!

  TH1F *time_diff_strips[3];
  TH1F* cluster_adc[3][2]; //!
  
  
  Double_t fChargeMinStrip[3];
  Double_t fChargeMaxStrip[3];

  std::map<Int_t, std::set<const CbmStsDigi*, classcomp2> > fDigiMap; //!  /** digis per hodo layer **/

  Int_t fEvent;

  StsClusterSet(const StsClusterSet&);
  StsClusterSet& operator=(const StsClusterSet&);

  ClassDef(StsClusterSet,1);
  
};
#endif
