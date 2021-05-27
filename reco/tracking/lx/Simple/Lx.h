#ifndef LX_INCLUDED
#define LX_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "CbmMuchPixelHit.h"

#include "FairTask.h"

#include "TClonesArray.h"

#include <unistd.h>

#include "LxCA.h"
#include "LxMC.h"
//#include "LxEff.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmTrackMatch.h"
#include "CbmVertex.h"

#include "FairFileHeader.h"
#include "FairRunAna.h"

#include "TH1.h"
#include "TProfile.h"
#include "TProfile2D.h"

#include <fstream>

#ifdef FAST_CODE
#define LX_DYNAMIC_CAST static_cast
#else  // FAST_CODE
#define LX_DYNAMIC_CAST dynamic_cast
#endif  // FAST_CODE

class LxHitFile {
public:
  LxHitFile();
  ~LxHitFile();
  bool Open(TString fileName, bool forWrite);
  bool StartEvent(Int_t nEnt);
  bool EndEvent();
  bool WriteHit(Int_t stationNumber, Int_t layerNumber, Double_t x, Double_t y, Double_t z, Double_t xErr,
                Double_t yErr, Double_t zErr);
  bool ReadEvent(Int_t eventNumber);
  bool ReadHit(Int_t& stationNumber, Int_t& layerNumber, Double_t& x, Double_t& y, Double_t& z, Double_t& xErr,
               Double_t& yErr, Double_t& zErr);
  bool Close();

private:
  bool useForWrite;
  int fd;
  std::list<off_t> eventOffs;
  char* evBuf;
  int bufSize;
  char* evPtr;
  char* evEnd;
};

extern TString lxFinderParticleType;

struct LxStsMCPoint {
  scaltype p, q, x, y, z, px, py, pz;
  Int_t stationNumber;
  LxMCTrack* mcTrack;
};

class LxFinder : public FairTask {
  //friend struct LxEff;
  friend class LxDraw;
  friend class LxParallFinder;
  friend class FinderCapsule;

public:
  static LxFinder* Instance();
  explicit LxFinder();
  ~LxFinder();
  InitStatus Init();         // Inherited virtual.
  void Exec(Option_t* opt);  // Inherited virtual.

  void SetGenInvMass(bool value) { generateInvMass = value; }

  void SetGenBackground(bool value) { generateBackground = value; }

  void SetGenChi2(bool value) { generateChi2 = value; }

  void SetLinkWithSts(bool value) { linkWithSts = value; }

  void SetUseMCPInsteadOfHits(bool value) { useMCPInsteadOfHits = value; }

  void SetCalcMiddlePoints(bool value) { calcMiddlePoints = value; }

  void SetCutCoeff(scaltype value) { cutCoeff = value; }

  void SetSaveOnlyTriggeringTracks(bool value) { saveOnlyTriggeringTracks = value; }

  void SetVerbosity(Int_t v) { verbosity = v; }

  void SetParallMode(bool v) { parallMode = v; }

  void SetHitFileName(TString v) { hitFileName = v; }

  void SetFileSaveSuffix(TString v) { fileSaveSuffix = v; }

  void SetParticleType(TString v)
  {
    particleType         = v;
    lxFinderParticleType = v;

    if (v == "omega") {
      caSpace.stationsInAlgo = 5;
#ifdef MAKE_EFF_CALC
      pPtCut = false;
#endif  //MAKE_EFF_CALC
    }
  }

private:
  void SaveRecoTracks();
  void CalcInvMass();
  void FinishTask();  // Inherited virtual.
  void SaveEventTracks();
  void SaveInvMass();
  void SaveBackground();
  void SaveSignalChi2();
  void SaveBackgroundChi2();
  bool Trigger();
  bool ReadEvent(Int_t evNum);
  static LxFinder* fInstance;
  TClonesArray* muchPixelHits;
  TClonesArray* listMCTracks;
  TClonesArray* listMuchPts;
  TClonesArray* listMuchClusters;
  TClonesArray* listMuchPixelDigiMatches;
  TClonesArray* listStsTracks;
  TClonesArray* listStsMatches;
  TClonesArray* listStsPts;      // STS MC-points array. Used for experiments with STS matching.
  TClonesArray* listRecoTracks;  // It is an output array.
#ifdef MAKE_EFF_CALC
  //LxEff effCounter;
  void MatchMCToReco();
  void MatchRecoToMC();
#endif  //MAKE_EFF_CALC
  CbmStsTrack* superEventData;
#ifdef MAKE_HISTOS
  static TH1F* massHisto;
  //static TH1F* backgroundMassHisto;
  static TTree* superEventTracks;
  static TH1F* signalChi2Histo;
  static TH1F* backgroundChi2Histo;
  static TProfile* effByMomentumProfile;
  static TH1F* muPlusDtxHisto;
  static TH1F* muMinusDtxHisto;
  static TH1F* signalXAtZ0;
  static TH1F* signalYAtZ0;
  static TH1F* bgrXAtZ0;
  static TH1F* bgrYAtZ0;
  static TH1F* signalInterTracksDistance;
  static TH1F* bgrInterTracksDistance;
  static TH1F* signalTanSigns;
  static TH1F* signalCoordSigns;
  static TH1F* bgrTanSigns;
  static TH1F* bgrCoordSigns;
  static TH1F* numberOfTracks;
  static TH1F* signalInterTracksDistanceOn1st;
  static TH1F* signalInterTracksAngle;
  static TH2F* signalInterTrackCorrDA;
  static TH1F* bgrInterTracksDistanceOn1st;
  static TH1F* bgrInterTracksAngle;
  static TH2F* bgrInterTrackCorrDA;
  static TH1F* bgrInterTracksDistanceOn1stSigns;
  static TH1F* muchMomErrSig;
  static TH1F* muchMomErrBgr;
#endif  //  static CbmStsTrack* superEventData;

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
  scaltype cutCoeff;
  Int_t verbosity;
  bool parallMode;
  TString hitFileName;
  TString fileSaveSuffix;
  TString particleType;
  bool pPtCut;
  std::vector<LxMCPoint> MCPoints;  // Points should lay here in the same order as in listMuchPts.
  std::vector<LxMCTrack> MCTracks;  // Tracks should lay here in the same order as in listMCTracks.
  std::list<LxStsMCPoint> MCStsPoints;
  std::list<LxStsMCPoint*> MCStsPointsByStations[8];
#ifdef MAKE_DISPERSE_2D_HISTOS
  std::list<LxMCPoint*> MCPointsByStations[LXSTATIONS][LXLAYERS];
  scaltype zCoordsByStations[LXSTATIONS][LXLAYERS];
#endif  //MAKE_DISPERSE_2D_HISTOS
  LxSpace caSpace;
  //std::map<Int_t, std::map<Int_t, int> > particleCounts;
#ifdef MAKE_EFF_CALC
  std::ofstream incomplete_events;  // Events where not all tracks are reconstructed.
  Int_t falseSignalTriggerings;
  Int_t trueSignalTriggerings;
  bool hasSignalInEvent;  //hasJPsiInEvent;
  Int_t signalCounter;    //jpsiCounter;
#endif  //MAKE_EFF_CALC
  Int_t eventNumber;
#ifdef CALC_MUCH_DETECTORS_EFF
  Int_t mcPointsCount;
  Int_t mcPointsTriggered;
#endif  //CALC_MUCH_DETECTORS_EFF
  bool saveOnlyTriggeringTracks;
#ifdef MAKE_TRIGGERING_HISTOS
  static TH1F* triggeringAllTracksVertices;
  static TH1F* triggeringDistTracksVertices;
  static TH1F* triggeringSignTracksVertices;
  static TH1F* triggeringTrigTracksVertices;
#endif  //MAKE_TRIGGERING_HISTOS
  std::map<string, unsigned int> particlesCountAll;
  std::map<string, unsigned int> particlesCountSign;
  std::map<string, unsigned int> particlesCountDist;
  std::map<string, unsigned int> particlesCountTrig;
  LxHitFile hitFile;

  ClassDef(LxFinder, 1);
};

#pragma GCC diagnostic warning "-Weffc++"

#endif  //LX_INCLUDED
