/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

// -------------------------------------------------------------------------
// -----                  CbmTrackingTrdQa header file                 -----
// -----                  Created 15/08/22                             -----
// -------------------------------------------------------------------------


/** CbmTrackingTrdQa.h
 *@author S. Gorbunov
 **
 ** Quality check task for Trd tracks
 **/

#ifndef CbmTrackingTrdQa_H
#define CbmTrackingTrdQa_H 1

#include "CbmLink.h"
#include "CbmQaHist.h"

#include <FairTask.h>

#include "TH2F.h"
#include "TProfile2D.h"
#include <TFolder.h>
#include <TStopwatch.h>
#include <TVector3.h>

#include "Riostream.h"

class TH1F;
class CbmMCDataArray;
class CbmMCDataManager;
class CbmTimeSlice;

class CbmTrackingTrdQa : public FairTask {

public:
  /** Default constructor **/
  CbmTrackingTrdQa(Int_t iVerbose = 1);

  /** Standard constructor 
  *@param minHits     Minimal number of TrdHits for considered MCTracks
  *@param quota       True/all hits for track to be considered reconstructed
  *@param iVerbose    Verbosity level
  **/
  CbmTrackingTrdQa(Int_t minHits, Double_t quota, Int_t iVerbose);

  /** Destructor **/
  virtual ~CbmTrackingTrdQa();

  /** Set rapidity of CM for the collision such that tracks are represented in CM **/
  void SetYcm(double ycm) { fYCM = ycm; }

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

  /** convert Pdg code <-> index **/
  static const int fgkNpdg = 6;
  static const char* fgkIdxName[fgkNpdg];
  static const char* fgkIdxSymb[fgkNpdg];
  static int Pdg2Idx(int pdg);
  static int Idx2Pdg(int idx);
  static const char* Idx2Name(int idx);
  static const char* Idx2Symb(int idx);

  /** Create histograms **/
  void CreateHistos();

  /** Reset histograms and counters **/
  void Reset();

  // collect id's of MC events participating to the data
  void CollectMcEvents(TClonesArray* Matches);

  /** Fill a map from MCTrack index to number of corresponding TrdHits **/
  void FillHitMap();

  /** Fill a map from MCTrack index to matched GlobalTrack index
   *@param nRec  Number of reconstructed tracks (return)
   *@param nGhosts  Number of ghost tracks (return)
   *@param nClones  Number of clone tracks (return)
   **/
  void FillTrackMatchMap(Int_t& nRec, Int_t& nGhosts, Int_t& nClones);

  /** Divide histograms (reco/all) with correct error for the efficiency
   *@param histo1  reconstructed tracks
   *@param histo2  all tracks (normalisation)
   *@param histo3  efficiency
   *@param opt  histogram dimension, for 2D use opt = "2D"
   **/
  void DivideHistos(TH1* histo1, TH1* histo2, TH1* histo3, Option_t* opt = "");

  struct McTrackInfo {
    std::map<Int_t, Int_t> fHitMap = {};  /// Trd station -> number of attached hits
    Int_t fGlobalTrackMatch        = -1;  /// matched GlobalTrack index
    Int_t fTrdTrackMatch           = -1;  /// matched TrdTrack index
    Double_t fQuali                = 0.;  /// percentage of matched hits
    Int_t fMatchedNHitsAll         = 0;   /// number of matched hits
    Int_t fMatchedNHitsTrue        = 0;   /// number of matched hits
    Bool_t fIsAccepted {0};
    Int_t fNtimesReconstructed {0};
    Bool_t fIsPrimary {0};
    Bool_t fIsFast {0};
    Bool_t fIsLong {0};
    Double_t fPt {0.};
    Int_t fPdg  = 0;
    Double_t fY = 0.;
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

  /// Trd
  Int_t fTrdNstations               = 0;        // Number of Trd stations
  CbmMCDataArray* fTrdPoints        = nullptr;  //! TrdPoints
  TClonesArray* fTrdHits            = nullptr;  //! TrdHits
  TClonesArray* fTrdHitMatch        = nullptr;  //! TrdHitMatch
  TClonesArray* fGlobalTracks       = nullptr;  //! GlobalTrack
  //TClonesArray* fGlobalTrackMatches = nullptr;  //! GlobalTrackMatch
  TClonesArray* fTrdTracks          = nullptr;  //! TrdTrack
  TClonesArray* fTrdTrackMatches    = nullptr;  //! TrdTrackMatch

  /** Geometry parameters **/
  TVector3 fTargetPos = {0., 0., 0.};  // Target centre position

  /** Task parameters **/

  Int_t fMinStations = 4;  // Minimal number of stations with hits for considered MCTrack

  Double_t fQuota = 0.7;  // True/all hits for track to be considered reconstructed
  float fYCM      = 0.;   // rapidity of CM

  TFolder fOutFolder = {"TrackingTrdQa", "TrackingTrdQa"};  /// output folder with histos and canvases

  /** Histograms **/

  // eff. vs. XY
  std::vector<CbmQaHist<TProfile2D>> fhStationEffXY;
  std::array<TH2F*, fgkNpdg> fhPidXY;
  // eff. vs. pT - Y
  std::map<const char*, std::array<TH2F*, fgkNpdg>> fhPidPtY;

  // eff. vs. pt, all
  TH1F* fhPtAccAll = nullptr;
  TH1F* fhPtRecAll = nullptr;
  TH1F* fhPtEffAll = nullptr;

  // eff. vs. pt, vertex
  TH1F* fhPtAccPrim = nullptr;
  TH1F* fhPtRecPrim = nullptr;
  TH1F* fhPtEffPrim = nullptr;

  // eff. vs. pt, non-vertex
  TH1F* fhPtAccSec = nullptr;
  TH1F* fhPtRecSec = nullptr;
  TH1F* fhPtEffSec = nullptr;

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

  // Pt resolution
  TH1F* fhPtResPrim = nullptr;

  /** List of histograms **/
  TList* fHistoList = nullptr;


  /** Counters **/
  Int_t fNAll          = 0;
  Int_t fNAccAll       = 0;
  Int_t fNAccPrim      = 0;
  Int_t fNAccFast      = 0;
  Int_t fNAccFastLong  = 0;
  Int_t fNAccSec       = 0;
  Int_t fNRecAll       = 0;
  Int_t fNRecPrim      = 0;
  Int_t fNRecFast      = 0;
  Int_t fNRecFastLong  = 0;
  Int_t fNRecSec       = 0;
  Int_t fNGhosts       = 0;
  Int_t fNClones       = 0;
  Int_t fNEvents       = 0;  /** Number of events with success **/
  Int_t fNEventsFailed = 0;  /** Number of events with failure **/
  Double_t fTime       = 0.; /** Total real time used for good events **/

  /** Timer **/
  TStopwatch fTimer = {};

  CbmTrackingTrdQa(const CbmTrackingTrdQa&);
  CbmTrackingTrdQa operator=(const CbmTrackingTrdQa&);

  ClassDef(CbmTrackingTrdQa, 0);
};


#endif
