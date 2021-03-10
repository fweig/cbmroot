// -------------------------------------------------------------------------
// -----                  CbmStsFindTracksQa header file               -----
// -----                  Created 11/01/06  by V. Friese               -----
// -------------------------------------------------------------------------


/** CbmStsFindTracksQa.h
 *@author V.Friese <v.friese@gsi.de>
 **
 ** Quality check task for CbmStsFindTracks
 **/

#ifndef CBMSTSTRACKFINDERQA_H
#define CBMSTSTRACKFINDERQA_H 1

#include "CbmLink.h"

#include "FairTask.h"

#include "TStopwatch.h"
#include "TVector3.h"

class TH1F;
class CbmMCDataArray;
class CbmStsSetup;
class CbmMCDataManager;
class CbmTimeSlice;

class CbmStsFindTracksQa : public FairTask {

public:
  /** Default constructor **/
  CbmStsFindTracksQa(Int_t iVerbose = 1);

  /** Standard constructor 
  *@param minHits     Minimal number of StsHits for considered MCTracks
  *@param quota       True/all hits for track to be considered reconstructed
  *@param iVerbose    Verbosity level
  **/
  CbmStsFindTracksQa(Int_t minHits, Double_t quota, Int_t iVerbose);

  /** Destructor **/
  virtual ~CbmStsFindTracksQa();

  /** Set parameter containers **/
  virtual void SetParContainers();

  /** Initialisation **/
  virtual InitStatus Init();

  /** Reinitialisation **/
  virtual InitStatus ReInit();

  /** Execution **/
  virtual void Exec(Option_t* opt);

private:
  /** Finish **/
  virtual void Finish();

  /** Read the geometry parameters **/
  InitStatus GetGeometry();

  /** Get the target node from the geometry **/
  void GetTargetPosition();

  /** Create histograms **/
  void CreateHistos();

  /** Reset histograms and counters **/
  void Reset();

  // collect id's of MC events participating to the data
  void CollectMcEvents(TClonesArray* Matches);

  /** Fill a map from MCTrack index to number of corresponding StsHits **/
  void FillHitMap();

  /** Fill a map from MCTrack index to matched StsTrack index
   *@param nRec  Number of reconstructed tracks (return)
   *@param nGhosts  Number of ghost tracks (return)
   *@param nClones  Number of clone tracks (return)
   **/
  void FillMatchMap(Int_t& nRec, Int_t& nGhosts, Int_t& nClones);

  /** Divide histograms (reco/all) with correct error for the efficiency
   *@param histo1  reconstructed tracks
   *@param histo2  all tracks (normalisation)
   *@param histo3  efficiency
   **/
  void DivideHistos(TH1* histo1, TH1* histo2, TH1* histo3);

  struct McTrackInfo {
    std::map<Int_t, Int_t> fHitMap = {};  /// Mvd / Sts station -> number of attached hits
    Int_t fStsTrackMatch           = -1;  /// matched StsTrack index
    Double_t fQuali                = 0.;  /// percentage of matched hits
    Int_t fMatchedNHitsAll         = 0;   /// number of matched hits
    Int_t fMatchedNHitsTrue        = 0;   /// number of matched hits
  };

  McTrackInfo& getMcTrackInfo(const CbmLink& link)
  {
    assert(fMcTrackInfoMap.find(link) != fMcTrackInfoMap.end());
    return fMcTrackInfoMap[link];
  }

  std::map<CbmLink, McTrackInfo> fMcTrackInfoMap = {};  //! map track link -> track info

  /// Setup
  FairRootManager* fManager    = nullptr;  //!
  CbmMCDataManager* fMcManager = nullptr;  //!
  CbmTimeSlice* fTimeSlice     = nullptr;  //!

  /// MC tracks
  CbmMCDataArray* fMCTracks = nullptr;  //! MCtrack

  /// Mvd
  Bool_t fIsMvdActive        = kTRUE;  // is the Mvd module active
  Int_t fMvdNstations        = 0;
  CbmMCDataArray* fMvdPoints = nullptr;  //!
  TClonesArray* fMvdCluster  = nullptr;  //!
  TClonesArray* fMvdHits     = nullptr;  //!
  TClonesArray* fMvdHitMatch = nullptr;  //!

  /// STS
  CbmStsSetup* fStsSetup         = nullptr;  // STS setup interface
  Int_t fStsNstations            = 0;        // Number of STS stations
  CbmMCDataArray* fStsPoints     = nullptr;  //! StsPoints
  TClonesArray* fStsHits         = nullptr;  //! StsHits
  TClonesArray* fStsHitMatch     = nullptr;  //! StsHitMatch
  TClonesArray* fStsClusterMatch = nullptr;  //!StsClusterMatch
  TClonesArray* fStsTracks       = nullptr;  //! StsTrack
  TClonesArray* fStsTrackMatches = nullptr;  //! StsTrackMatch

  /** Geometry parameters **/
  TVector3 fTargetPos = {0., 0., 0.};  // Target centre position

  /** Task parameters **/

  Int_t fMinStations = 4;  // Minimal number of stations with hits for considered MCTrack

  Double_t fQuota = 0.7;  // True/all hits for track to be considered reconstructed

  TFolder fOutFolder = {"CbmStsFindTracksQa", "CbmStsFindTracksQa"};  /// output folder with histos and canvases

  /** Histograms **/

  // eff. vs. p, all
  TH1F* fhMomAccAll = nullptr;
  TH1F* fhMomRecAll = nullptr;
  TH1F* fhMomEffAll = nullptr;

  // eff. vs. p, vertex
  TH1F* fhMomAccPrim = nullptr;
  TH1F* fhMomRecPrim = nullptr;
  TH1F* fhMomEffPrim = nullptr;

  // eff. vs. p, non-vertex
  TH1F* fhMomAccSec = nullptr;
  TH1F* fhMomRecSec = nullptr;
  TH1F* fhMomEffSec = nullptr;

  // eff. vs. np, all
  TH1F* fhNpAccAll = nullptr;
  TH1F* fhNpRecAll = nullptr;
  TH1F* fhNpEffAll = nullptr;

  // eff. vs. np, vertex
  TH1F* fhNpAccPrim = nullptr;
  TH1F* fhNpRecPrim = nullptr;
  TH1F* fhNpEffPrim = nullptr;

  // eff. vs. np, non-vertex
  TH1F* fhNpAccSec = nullptr;
  TH1F* fhNpRecSec = nullptr;
  TH1F* fhNpEffSec = nullptr;

  // eff. vs. z, non-vertex
  TH1F* fhZAccSec = nullptr;
  TH1F* fhZRecSec = nullptr;
  TH1F* fhZEffSec = nullptr;

  // # hits of clones and ghosts
  TH1F* fhNhClones = nullptr;
  TH1F* fhNhGhosts = nullptr;


  /** List of histograms **/
  TList* fHistoList = nullptr;


  /** Counters **/
  Int_t fNAll          = 0;
  Int_t fNAccAll       = 0;
  Int_t fNAccPrim      = 0;
  Int_t fNAccRef       = 0;
  Int_t fNAccRefLong   = 0;
  Int_t fNAccSec       = 0;
  Int_t fNRecAll       = 0;
  Int_t fNRecPrim      = 0;
  Int_t fNRecRef       = 0;
  Int_t fNRecRefLong   = 0;
  Int_t fNRecSec       = 0;
  Int_t fNGhosts       = 0;
  Int_t fNClones       = 0;
  Int_t fNEvents       = 0;  /** Number of events with success **/
  Int_t fNEventsFailed = 0;  /** Number of events with failure **/
  Double_t fTime       = 0.; /** Total real time used for good events **/

  /** Timer **/
  TStopwatch fTimer = {};

  CbmStsFindTracksQa(const CbmStsFindTracksQa&);
  CbmStsFindTracksQa operator=(const CbmStsFindTracksQa&);

  ClassDef(CbmStsFindTracksQa, 0);
};


#endif
