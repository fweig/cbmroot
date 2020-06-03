/** CbmFiberHodoClusterFinder.h
 **
 ** Task to find neighboring fibres which are above
 ** the base line.
 ** Create as an output an array of
 ** the digis belonging to the cluster.
 **/

#ifndef CBMFIBERHODOCLUSTERFINDER_H
#define CBMFIBERHODOCLUSTERFINDER_H 

#include <Rtypes.h>               // for ClassDef
#include <RtypesCore.h>           // for Int_t, Option_t
#include <map>                    // for map
#include <set>                    // for set
#include "CbmFiberHodoAddress.h"  // for CbmFiberHodoAddress
#include "CbmFiberHodoDigi.h"     // for CbmFiberHodoDigi
#include <FairTask.h>             // for InitStatus, FairTask
#include <TString.h>              // for TString
#include <vector>

class TH1F;
class TH2F;
class TMemberInspector;

struct classcomp {
  bool operator() (const CbmFiberHodoDigi* lhs, const CbmFiberHodoDigi* rhs) const
    {return CbmFiberHodoAddress::GetStripId(lhs->GetAddress()) < CbmFiberHodoAddress::GetStripId(rhs->GetAddress());}
};
    
class CbmFiberHodoClusterFinder : public FairTask
{
  
 public:
  
  /**
   * Default constructor.
   */
  CbmFiberHodoClusterFinder();
  
  /**
   * Default destructor.
   */
  ~CbmFiberHodoClusterFinder();
 
  /** Initialisation **/
  virtual InitStatus ReInit();
  virtual InitStatus Init();
  virtual void SetParContainers();
  
  /** Executed task **/
  virtual void Exec(Option_t * option);
  
  /** Finish task **/
  virtual void Finish();

  void SetInputDataLevelName(TString name) { fInputLevelName = name; }

 private:

  const std::vector<CbmFiberHodoDigi>*     fDigis;       /** Input array of CbmFiberHodoDigi **/
  TClonesArray*     fClusters;    /** candidates array of CbmFiberHodoCluster **/
  TClonesArray*     finalClusters;    /** Output array of CbmFiberHodoCluster **/

  std::map<Int_t, std::set<const CbmFiberHodoDigi*, classcomp> > fDigiMap;   /** digis per hodo layer **/ 

  TString fInputLevelName;

  TH2F* hodo1_pos;
  TH2F* hodo2_pos;

  TH2F* hodo1_xcor;
  TH2F* hodo2_xcor;

  TH2F* hodo1_ycor;
  TH2F* hodo2_ycor;
  TH1F* hodo1x;
  TH1F* hodo1y;
  TH1F* hodo2x;
  TH1F* hodo2y;
  TH2F* hodo_xx;
  TH2F* hodo_yy;
  TH2F* hodo1_pos_nocut;
  TH1F* hodo1x_nocut;
  TH1F* hodo1y_nocut;

  CbmFiberHodoClusterFinder(const CbmFiberHodoClusterFinder&);
  CbmFiberHodoClusterFinder& operator=(const CbmFiberHodoClusterFinder&);
  
  ClassDef(CbmFiberHodoClusterFinder,2);
  
};
#endif
