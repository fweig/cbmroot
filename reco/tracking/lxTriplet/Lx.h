#ifndef LX_INCLUDED
#define LX_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "FairTask.h"
#include "CbmMuchPixelHit.h"
#include "TClonesArray.h"
#include "LxMC.h"
#include "LxCATriplets.h"
#include "LxEff.h"
#include "CbmTrackMatch.h"
#include "TH1.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmVertex.h"
#include <fstream>

#ifdef FAST_CODE
#define LX_DYNAMIC_CAST static_cast
#else// FAST_CODE
#define LX_DYNAMIC_CAST dynamic_cast
#endif// FAST_CODE

extern TString lxFinderParticleType;

class LxFinderTriplet : public FairTask
{
  friend struct LxEff;
  friend class LxDraw;

public:
  static LxFinderTriplet* Instance();
  explicit LxFinderTriplet();
  ~LxFinderTriplet();
  InitStatus Init();// Inherited virtual.
  void Exec(Option_t* opt);// Inherited virtual.

  void SetGenInvMass(bool value)
  {
    generateInvMass = value;
  }

  void SetGenBackground(bool value)
  {
    generateBackground = value;
  }

  void SetGenChi2(bool value)
  {
    generateChi2 = value;
  }

  void SetLinkWithSts(bool value)
  {
    linkWithSts = value;
  }

  void SetUseMCPInsteadOfHits(bool value)
  {
    useMCPInsteadOfHits = value;
  }

  void SetCalcMiddlePoints(bool value)
  {
    calcMiddlePoints = value;
  }

  void SetCutCoeff(Double_t value)
  {
    cutCoeff = value;
  }

  void SetParticleType(TString v)
  {
    //particleType = v;
    lxFinderParticleType = v;

    if (v == "omega")
    {
      caSpace.stationsInAlgo = 5;
#ifdef MAKE_EFF_CALC
      pPtCut = false;
#endif//MAKE_EFF_CALC
    }
  }

private:
  void SaveRecoTracks();
  void CalcInvMass();
  void FinishTask();// Inherited virtual.
  void SaveEventTracks();
  void SaveInvMass();
  void SaveBackground();
  void MatchRecoToMC();
  static LxFinderTriplet* fInstance;
  TClonesArray* muchPixelHits;
  TClonesArray* listMCTracks;
  TClonesArray* listMuchPts;
  TClonesArray* listMuchClusters;
  TClonesArray* listMuchPixelDigiMatches;
  TClonesArray* listStsTracks;
  TClonesArray* listStsMatches;
  TClonesArray* listStsPts;// STS MC-points array. Used for experiments with STS matching.
  TClonesArray* listRecoTracks;// It is an output array.
  LxEff effCounter;
  static TH1F* massHisto;
  //static TH1F* backgroundMassHisto;
  static CbmStsTrack* superEventData;
  static TTree* superEventTracks;
  static TH1F* signalChi2Histo;
  static TH1F* backgroundChi2Histo;
  static TH1F* unknownChi2Histo;
  static TProfile* effByMomentumProfile;
  static TH1F* signalInterTracksDistance;
  static TH1F* bgrInterTracksDistance;
  static TH1F* signalSignDefect;
  static TH1F* bgrSignDefect;
  static TH1F* signalYAtZ0;
  static TH1F* bgrYAtZ0;
  static TH1F* signalYAtZ0_2;
  static TH1F* bgrYAtZ0_2;
  CbmStsKFTrackFitter extFitter;
  CbmVertex* fPrimVtx;
  // These two containers below accumulates respectively all positive and all negative reconstructed tracks for all events.
  std::list<CbmStsTrack> positiveTracks;
  std::list<CbmStsTrack> negativeTracks;
  bool generateInvMass;
  bool generateBackground;
  bool generateChi2;
  bool linkWithSts;
  bool useMCPInsteadOfHits;
  bool calcMiddlePoints;
  Double_t cutCoeff;
  bool pPtCut;
  std::vector<LxMCPoint> MCPoints;// Points should lay here in the same order as in listMuchPts.
  std::vector<LxMCTrack> MCTracks;// Tracks should lay here in the same order as in listMCTracks.
  std::list<LxStsMCPoint> MCStsPoints;
  std::list<LxStsMCPoint*> MCStsPointsByStations[8];
#ifdef MAKE_DISPERSE_2D_HISTOS
  std::list<LxMCPoint*> MCPointsByStations[LXSTATIONS][LXLAYERS];
  Double_t zCoordsByStations[LXSTATIONS][LXLAYERS];
#endif//MAKE_DISPERSE_2D_HISTOS
  LxSpace caSpace;
  std::map<Int_t, std::map<Int_t, int> > particleCounts;
#ifdef MAKE_EFF_CALC
  std::ofstream incomplete_events;// Events where not all tracks are reconstructed.
#endif//MAKE_EFF_CALC
  Int_t eventNumber;
#ifdef CALC_MUCH_DETECTORS_EFF
  Int_t mcPointsCount;
  Int_t mcPointsTriggered;
#endif//CALC_MUCH_DETECTORS_EFF

  ClassDef(LxFinderTriplet, 1);
};

#endif//LX_INCLUDED
