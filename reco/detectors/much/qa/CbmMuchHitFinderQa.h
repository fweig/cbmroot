// -------------------------------------------------------------------------
// -----                  CbmMuchHitFinderQa header file               -----
// -----                  Modified 01/18  by Vikas Singhal             -----
// -----                  Created 16/11/07  by E. Kryshen              -----
// -------------------------------------------------------------------------

#ifndef CbmMuchHitFinderQa_H
#define CbmMuchHitFinderQa_H

#include "FairTask.h"

#include "TParameter.h"
#include "TString.h"
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TFolder.h>

class CbmDigiManager;
class CbmMuchGeoScheme;
class CbmQaCanvas;
class TBuffer;
class TClass;
class TClonesArray;
class TH1D;
class TH1I;
class TMemberInspector;

class CbmMuchHitFinderQa : public FairTask {

public:
  CbmMuchHitFinderQa(const char* name = "MuchHitFinderQa", Int_t verbose = 1);
  virtual ~CbmMuchHitFinderQa();
  virtual InitStatus Init();
  virtual void Exec(Option_t* option);
  virtual void FinishTask();
  virtual void SetParContainers();
  void SetGeoFileName(TString fileName) { fGeoFileName = fileName; }
  void SetPerformanceFileName(TString fileName) { fFileName = fileName; }
  void SetGeometryID(Int_t flag) { fFlag = flag; }

protected:
  /* Analysis of hit uncertainty (pull) distributions
   * as function of pad size and cluster shape
   */
  void PullsQa();

  /* Information on clusters - number of pads in a cluster, number of points, contributed to
   * a cluster, number of hits, created from a cluster
   */
  void StatisticsQa();

  /* Number of points and hits in a cluster for signal muons (MotherId = 0) */
  void ClusterDeconvQa();

private:
  void DeInit();
  void DrawCanvases();

  CbmMuchGeoScheme* fGeoScheme;
  TString fGeoFileName;
  TString fFileName;
  Int_t fVerbose               = 0;
  Int_t fFlag                  = 0;
  TClonesArray* fPoints        = nullptr;
  CbmDigiManager* fDigiManager = nullptr;

  TFolder fOutFolder;   /// output folder with histos and canvases
  TFolder* histFolder;  /// subfolder for histograms

  TClonesArray* fClusters = nullptr;
  TClonesArray* fHits     = nullptr;
  TClonesArray* fMCTracks = nullptr;

  Int_t fNstations = 0;

  //1D Histogram for PULL Distribution
  TH1D* fhPullX = nullptr;
  TH1D* fhPullY = nullptr;
  TH1D* fhPullT = nullptr;

  //1D Histogram for Residual Distribution
  TH1D* fhResidualX = nullptr;
  TH1D* fhResidualY = nullptr;
  TH1D* fhResidualT = nullptr;

  std::vector<TH1I*> fhPointsInCluster;
  std::vector<TH1I*> fhDigisInCluster;
  std::vector<TH1I*> fhHitsPerCluster;

  CbmQaCanvas* fCanvPointsInCluster = nullptr;
  CbmQaCanvas* fCanvDigisInCluster  = nullptr;
  CbmQaCanvas* fCanvHitsPerCluster  = nullptr;
  CbmQaCanvas* fCanvPull            = nullptr;
  CbmQaCanvas* fCanvResidual        = nullptr;

  TParameter<int> fNevents;       /// number of processed events
  TParameter<int> fSignalPoints;  // Number of signal MC points
  TParameter<int> fSignalHits;    // Number of signal hits
  TParameter<int> fPointsTotal;
  TParameter<int> fPointsUnderCounted;
  TParameter<int> fPointsOverCounted;

  /** Defines whether the point with the given index is signal point. **/
  Bool_t IsSignalPoint(Int_t iPoint);

  CbmMuchHitFinderQa(const CbmMuchHitFinderQa&);
  CbmMuchHitFinderQa& operator=(const CbmMuchHitFinderQa&);

  ClassDef(CbmMuchHitFinderQa, 1)
};

#endif
