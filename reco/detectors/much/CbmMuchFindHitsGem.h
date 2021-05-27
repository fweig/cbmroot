/* CbmMuchFindHitsGem.h
 *
 *@author  V.Singhal <vikas@vecc.gov.in>
 *@version 2.0 (removeing fDaq Time stream mode as per CbmDaqNew Scheme)
 *@define  Clusterization and Hit creation is not dependend of EBE or Time Based mode.
 *         fClusterSeprationTime taken as 100 ns which will work with Time Based mode.
 *@since   22.08.17
 *
 *@author  E.Kryshen <e.kryshen@gsi.de>
 *@version 2.0
 *@since   31.07.12
 *
 *@author  M.Ryzhinskiy <m.ryzhinskiy@gsi.de>
 *@version 1.0
 *@since   11.02.08
 *
 *  Produces hits in GEM-like modules using different
 *  clustering algorithms. Clustering algorithms can be chosen
 *  by the switch SetAlgorithm(Int_t iAlgorithm)
 *  Several clustering algorithms are supported
 *  0 - one hit per fired pad
 *  1 - one hit per cluster
 *  2 - simple cluster deconvolution with charge threshold
 *  3 - local maxima finder
 */

#ifndef CBMMUCHFINDHITSGEM_H
#define CBMMUCHFINDHITSGEM_H 1
#include "CbmEvent.h"
#include "CbmMuchDigi.h"

#include "FairTask.h"

#include "TString.h"

#include <vector>
//#include "CbmStsReco.h"

class CbmDigiManager;
class CbmMuchPad;
class CbmMuchCluster;
class TClonesArray;
class CbmMuchGeoScheme;

// Root file for both the Event by Event Mode and Time Stream mode is same therefore
// Time based data can be read directly from the Root Tree ( One Tree entry = One Time Slice)
// class CbmTimeSlice;

class CbmMuchFindHitsGem : public FairTask {
public:
  CbmMuchFindHitsGem(const char* digiFileName, Int_t flag);
  virtual ~CbmMuchFindHitsGem() {}
  virtual void Exec(Option_t* opt);
  void ProcessData(CbmEvent*);
  void SetAlgorithm(Int_t iAlgorithm) { fAlgorithm = iAlgorithm; }
  void SetThresholdRatio(Double_t thresholdRatio) { fThresholdRatio = thresholdRatio; }
  void SetClusterSeparationTime(Double_t time) { fClusterSeparationTime = time; }
  // Removing SetDaq functionality as Cluster and Hit Finder algorithm is same for both the Time Based and Event Based mode.
  // void SetDaq(Bool_t daq)                         { fDaq = daq; }
  // Set functionality for accepting CbmMuchBeamTimeDigi generated from mCBM data.
  void SetBeamTimeDigi(Bool_t beam) { bBeamTimeDigi = beam; }

private:
  virtual InitStatus Init();
  void FindClusters(CbmEvent*);
  void CreateCluster(CbmMuchPad* pad);
  void ExecClusteringSimple(CbmMuchCluster* cluster, Int_t iCluster, CbmEvent* event);
  void ExecClusteringPeaks(CbmMuchCluster* cluster, Int_t iCluster, CbmEvent* event);
  void CreateHits(CbmMuchCluster* cluster, Int_t iCluster, CbmEvent* event);
  TString fDigiFile;  // Digitization file
  Int_t fFlag;
  Int_t fAlgorithm;                 // Defines which algorithm to use
  Double_t fClusterSeparationTime;  // Minimum required time between two clusters
  Double_t fThresholdRatio;         // Charge threshold ratio relative to max cluster charge
  Int_t fEvent;                     // Event counter
  Int_t fNofTimeslices;             // TimeSlice Counter
  //TClonesArray* fDigis;                   		// Input array of CbmMuchDigi
  CbmDigiManager* fDigiManager;                 //! Interface to digi branch
  TClonesArray* fEvents;                        // Input array of CbmEvent after Event Building
  std::vector<Int_t> fClusterCharges;           //!
  std::vector<Bool_t> fLocalMax;                //!
  std::vector<CbmMuchPad*> fClusterPads;        //!
  std::vector<std::vector<Int_t>> fNeighbours;  //!

  TClonesArray* fClusters;       // Output array of CbmMuchCluster objects
  TClonesArray* fHits;           // Output array of CbmMuchHit
  CbmMuchGeoScheme* fGeoScheme;  // Geometry scheme
  // auxiliary maps and vectors
  std::vector<Int_t> fDigiIndices;      //!
  std::vector<CbmMuchPad*> fFiredPads;  //!
  Bool_t fEventMode = kFALSE;
  //ECbmMode fMode = kCbmTimeslice;                 // To Select CbmEvent Branch in Time Based Event building mode
  // Removing SetDaq functionality as Cluster and Hit Finder algorithm is same for both the Time Based and Event Based mode.
  //Bool_t fDaq;                            // To daq or not to daq // TODO
  //CbmTimeSlice* fTimeSlice;               // Time slice object in the DAQ approach
  // according to the input branch , below need to be used so using auto, such that runtime it will decide
  //std::vector<CbmMuchDigi> fDigiData;          // Vector of digis

  // No need of fDigiDtata
  //std::vector< auto > fDigiData;          // Vector of digis
  UInt_t fuClusters    = 0;       //Number of Clusters.
  Bool_t bBeamTimeDigi = kFALSE;  // Boolean for Using Beam Time Digi

  CbmMuchFindHitsGem(const CbmMuchFindHitsGem&);
  CbmMuchFindHitsGem operator=(const CbmMuchFindHitsGem&);

  ClassDef(CbmMuchFindHitsGem, 1);
};

#endif
