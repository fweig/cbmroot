/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel,  Sergey Gorbunov, Maksym Zyzak, Valentina Akishina, Igor Kulakov, Denis Bertini [committer] */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  CbmL1 header file
 *
 *====================================================================
 */

#ifndef _CbmL1_h_
#define _CbmL1_h_


#include "CbmL1Hit.h"
#include "CbmL1MCPoint.h"
#include "CbmL1MCTrack.h"
#include "CbmL1Track.h"
#include "CbmL1Vtx.h"
#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMvdHit.h"
#include "CbmMvdPoint.h"
#include "CbmTimeSlice.h"

#include "FairDetector.h"
#include "FairRootManager.h"
#include "FairTask.h"

#include "TClonesArray.h"
#include "TH1.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <utility>

#include "L1Algo/L1Algo.h"
#include "L1Algo/L1Vector.h"
#include "L1EventEfficiencies.h"

class L1AlgoInputData;
class L1Algo;
class L1Event;
class CbmL1ParticlesFinder;
class L1FieldSlice;
class CbmL1Track;
class CbmL1MCTrack;
class KFTopoPerformance;
class CbmMCDataObject;

class CbmEvent;
class CbmStsParSetSensor;
class CbmStsParSetSensorCond;
class CbmStsParSetModule;
class CbmTofDigiBdfPar;

class CbmTrdParSetDigi;
class CbmTrdParModDigi;
class CbmTofDigiPar;

class CbmL1HitStore {
public:
  int ExtIndex;
  int iStation;
  double x;
  double y;
  double time;
  double dx;
  double dy;
  double dt;
  double dxy;
  int Det;
};


/// Enumeration for the detector subsystems used in L1 tracking
/// It is important for the subsystems to be specified in the actual order. The order is used
/// for the L1Station array filling.
/// Note: L1DetectorID has a forward declaration in L1InitManager.h and L1BaseStationInfo.h
enum class L1DetectorID
{
  kMvd,
  kSts,
  kMuch,
  kTrd,
  kTof
};

// TODO: insert documentation! (S.Zh.)
//
/// L1Algo runtime constants modification can be performed in run_reco.C. Example:
///
///   l1->GetInitManager()->GetParameters()->SetMaxDoubletsPerSinglet(149);
/// TODO: L1InitManager - main interface of communication between cbmroot/bmnroot and L1Algo (S.Zharko)
///
class CbmL1 : public FairTask {
private:
  CbmL1(const CbmL1&);
  CbmL1 operator=(const CbmL1&);

public:
  typedef std::set<std::pair<int, int>> DFSET;

  friend class L1AlgoDraw;
  friend class L1AlgoPulls;

  template<int NHits>
  friend class L1AlgoEfficiencyPerformance;

  friend class CbmL1MCTrack;
  friend class CbmL1PFFitter;

  static CbmL1* Instance() { return fInstance; }

  void SetParContainers();
  void CheckDetectorPresence();
  virtual InitStatus ReInit();
  virtual InitStatus Init();
  void Exec(Option_t* option);

  CbmL1();

  /**                                  Constructor
      * @param _fPerformance - type of Efficiency output: 0 - w\o efficiencies, doesn't use MC data; 1 - L1 standard efficiency definition; 2 - QA efficiency definition
      * @param fSTAPDataMode_ - way to work with files for the standalone package: 0 - off , 1 - write, 2  - read data and work only with it, 3 - write and read (debug)
      * @param findParticleMode_ : 0 - don't run FindParticles; 1 - run, all MC particle is reco-able; 2 - run, MC particle is reco-able if created from reco-able tracks; 3 - run, MC particle is reco-able if created from reconstructed tracks
      */
  CbmL1(const char* name, Int_t iVerbose = 1, Int_t _fPerformance = 0, int fSTAPDataMode_ = 0,
        TString fSTAPDataDir_ = "./", int findParticleMode_ = 0);

  ~CbmL1(/*if (targetFieldSlice) delete;*/);

  /// Gets a pointer to L1InitManager (for an access in run_reco.C)
  L1InitManager* GetInitManager() { return fpInitManager; }
  /// Gets a set of active detectors used in tracking
  // TODO: think about return (value, reference or const reference?) (S.Zh.)
  std::set<L1DetectorID> GetActiveTrackingDetectorIDs() const { return fActiveTrackingDetectorIDs; }
  /// Sets a vector of detectors used in tracking
  void SetActiveTrackingDetectorIDs(const std::set<L1DetectorID>& init) { fActiveTrackingDetectorIDs = init; }

  void SetUseMcHit(int StsUseMcHit = 0, int MuchUseMcHit = 0, int TrdUseMcHit = 0, int TofUseMcHit = 0)
  {
    fStsUseMcHit  = StsUseMcHit;
    fMuchUseMcHit = MuchUseMcHit;
    fTrdUseMcHit  = TrdUseMcHit;
    fTofUseMcHit  = TofUseMcHit;
  }


  void SetStsMaterialBudgetFileName(TString fileName) { fStsMatBudgetFileName = fileName; }
  void SetMvdMaterialBudgetFileName(TString fileName) { fMvdMatBudgetFileName = fileName; }
  void SetMuchMaterialBudgetFileName(TString fileName) { fMuchMatBudgetFileName = fileName; }
  void SetTrdMaterialBudgetFileName(TString fileName) { fTrdMatBudgetFileName = fileName; }
  void SetTofMaterialBudgetFileName(TString fileName) { fTofMatBudgetFileName = fileName; }
  void SetExtrapolateToTheEndOfSTS(bool b) { fExtrapolateToTheEndOfSTS = b; }
  void SetLegacyEventMode(bool b) { fLegacyEventMode = b; }
  void SetMuchPar(TString fileName) { fMuchDigiFile = fileName; }
  void SetUseHitErrors(bool value) { fUseHitErrors = value; }
  void SetMissingHits(bool value) { fMissingHits = value; }
  void SetStsOnlyMode() { fTrackingMode = L1Algo::TrackingMode::kSts; }
  void SetMcbmMode() { fTrackingMode = L1Algo::TrackingMode::kMcbm; }
  void SetGlobalMode() { fTrackingMode = L1Algo::TrackingMode::kGlobal; }

  void Finish();

  //   void SetTrackingLevel( Int_t iLevel ){ fTrackingLevel = iLevel; }
  //   void MomentumCutOff( Double_t cut ){ fMomentumCutOff = cut; }
  //   void SetGhostSuppression( Bool_t b ){ fGhostSuppression= b; }
  //   void SetDetectorEfficiency( Double_t eff ){ fDetectorEfficiency = eff; }

  /// Reconstructs an event
  /// \param event  Pointer to current CbmEvent object
  void Reconstruct(CbmEvent* event = NULL);

  //  bool ReadMCDataFromFile(const char work_dir[100], const int maxNEvent, const int iVerbose);
  //   vector<CbmL1MCPoint> vMCPoints;

  //   static bool compareZ(const int &a, const int &b );
  inline double Get_Z_vMCPoint(int a) const { return vMCPoints[a].z; }

private:
  typedef std::map<Double_t, Int_t> DFEI2I;

  struct TH1FParameters {
    TString name, title;
    int nbins;
    float xMin, xMax;
  };

  void IdealTrackFinder();  // just copy all reconstructable MCTracks into RecoTracks.

  /// Read information about hits, mcPoints and mcTracks into L1 classes

  /// Repacks data from the external TClonesArray objects to the internal L1 arrays
  /// \param fData_       Pointer to the target object containig L1Algo internal arrays of hits
  /// \param TsStart      Reference to the timeslice start time
  /// \param TsLength     Reference to the timeslice length
  /// \param TsOverlap    Reference to the timeslice overlap length (does not used at the moment)
  /// \param FstHitinTs   Index of the first hit in the time-lice
  /// \param areDataLeft  Flag: true - data were left after reading the sub-timeslice
  /// \param event        Pointer to the current CbmEvent object
  void ReadEvent(L1AlgoInputData* fData_, float& TsStart, float& TsLength, float& TsOverlap, int& FstHitinTs,
                 bool& areDataLeft, CbmEvent* event = NULL);

  /// Converts data from generic FairMCPoint based class to the CbmL1MCPoint (dummy method)
  /// \param   MC       Pointer to a target CbmL1MCPoint object
  /// \param   iPoint   Index of the point into the input MC points CbmMCDataArray object for the particular detector
  /// \param   MVD      Index of the detector subsystem
  /// \return  flag: false - success, true - some errors occured
  bool ReadMCPoint(CbmL1MCPoint* MC, int iPoint, int MVD);  // help procedure

  /// Converts data from generic FairMCPoint based class to the CbmL1MCPoint
  /// \param   MC       Pointer to a target CbmL1MCPoint object
  /// \param   iPoint   Index of the point into the input MC points CbmMCDataArray object for the particular detector
  /// \param   file     Index of the input file
  /// \param   event    Index of the input event
  /// \param   MVD      Index of the detector subsystem
  /// \return  flag: false - success, true - some errors occured
  // TODO: Probably, we should replace input parameter MVD with the template  (S.Zharko)
  bool ReadMCPoint(CbmL1MCPoint* MC, int iPoint, int file, int event, int MVD);

  //   static bool compareZ(const int &a, const int &b );
  //   bool compareZ(const int &a, const int &b );

  /// Fills the vMCTracks vector and the dFEI2vMCTracks set
  void Fill_vMCTracks();

  /*
   * Input Performance
   */

  /// Procedure for match hits and MCPoints.
  /// Reads information about correspondence between hits and mcpoints and fill CbmL1MCPoint::hitIds and CbmL1Hit::mcPointIds arrays
  /// should be called after fill of algo
  void HitMatch();

  void FieldApproxCheck();    // Build histos with difference between Field map and approximated field
  void FieldIntegralCheck();  // Build 2D histo: dependence of the field integral on phi and theta
  void InputPerformance();    // Build histos about input data, like hit pulls, etc.
  void TimeHist();

  /// Reconstruction Performance
  void TrackMatch();  // Procedure for match Reconstructed and MC Tracks. Should be called before Performances
  void EfficienciesPerformance();  // calculate efficiencies
  void TrackFitPerformance();      // pulls & residuals. Can be called only after Performance()
  void HistoPerformance();         // fill some histograms and calculate efficiencies

  /// STandAlone Package service-functions
  void WriteSTAPGeoData(const L1Vector<float>& geo);  // create geo_algo.dat
  void WriteSTAPAlgoData();                           // create data_algo.dat
  void WriteSTAPPerfData();                           // create data_perfo.dat
  //void ReadSTAPGeoData(L1Vector<float> geo, int &size);
  void ReadSTAPGeoData(L1Vector<float>& geo, int& size);
  void ReadSTAPAlgoData();
  void ReadSTAPPerfData();
  /// SIMD KF Banchmark service-functions
  void WriteSIMDKFData();

  void WriteHistosCurFile(TObject* obj);

  static std::istream& eatwhite(std::istream& is);  // skip spaces
  static void writedir2current(TObject* obj);       // help procedure


  inline Double_t dFEI(int file, int event, int idx) { return (1000 * idx) + file + (0.0001 * event); }

public:
  L1Algo* algo {nullptr};  // for access to L1 Algorithm from L1::Instance

  TString fMuchDigiFile {};  // Much digitization file name
  bool fUseHitErrors {true};
  bool fMissingHits {false};
  L1Algo::TrackingMode fTrackingMode {L1Algo::TrackingMode::kSts};

  L1Vector<CbmL1Track> vRTracks {"CbmL1::vRTracks"};  ///> reconstructed tracks
  DFSET vFileEvent {};

  L1Vector<CbmL1HitStore> vHitStore {"CbmL1::vHitStore"};  // diff hit information

private:
  static CbmL1* fInstance;

  L1InitManager* fpInitManager {nullptr};  ///< Pointer to L1InitManager object of L1 algorithm core

  std::set<L1DetectorID> fActiveTrackingDetectorIDs {};  ///< Set of detectors active in tracking

  L1AlgoInputData* fData {nullptr};

  L1Vector<CbmL1MCPoint> vMCPoints {"CbmL1::vMCPoints"};
  int nMvdPoints {0};
  L1Vector<int> vMCPoints_in_Time_Slice {"CbmL1::vMCPoints_in_Time_Slice"};

  int NStation {0};       ///< number of all detector stations
  int NMvdStations {0};   ///< number of mvd stations
  int NStsStations {0};   ///< number of sts stations
  int NMuchStations {0};  ///< number of much stations
  int NTrdStations {0};   ///< number of trd stations
  int NTOFStation {0};    ///< number of tof stations
  ///
  int NMvdStationsGeom;
  int NStsStationsGeom;
  int NTrdStationsGeom;
  int NMuchStationsGeom;
  int NTOFStationGeom;
  int NStationGeom;

  vector<int> StationIdxReverseConverter;

  Int_t fPerformance {0};     // 0 - w\o perf. 1 - L1-Efficiency definition. 2 - QA-Eff.definition
  double fTrackingTime {0.};  // time of track finding

  int fSTAPDataMode {0};  // way to work with file for standalone package.
                          // 0 (off) , 1 (write), 2 (read data and work only with it), 3 (debug - write and read)

  TString fSTAPDataDir {};

  Int_t fTrackingLevel {2};         // currently not used
  Double_t fMomentumCutOff {0.1};   // currently not used
  Bool_t fGhostSuppression {true};  // currently not used

  Int_t fStsUseMcHit {-1};   // if STS data should be processed
  Int_t fMuchUseMcHit {-1};  // if Much data should be processed
  Int_t fTrdUseMcHit {-1};   // if Trd data should be processed
  Int_t fTofUseMcHit {-1};   // if Tof data should be processed

  Bool_t fUseMVD {false};   // if Mvd data should be processed
  Bool_t fUseMUCH {false};  // if Much data should be processed
  Bool_t fUseTRD {false};   // if Trd data should be processed
  Bool_t fUseTOF {false};   // if Tof data should be processed

  /// Input data
  CbmTimeSlice* fTimeSlice {nullptr};
  CbmMCEventList* fEventList {nullptr};  //!  MC event list (all)

  CbmMCDataArray* fStsPoints {nullptr};
  CbmMCDataArray* fMvdPoints {nullptr};
  CbmMCDataArray* fMCTracks {nullptr};
  CbmMCDataObject* fMcEventHeader {nullptr};

  TClonesArray* listStsDigiMatch {nullptr};
  TClonesArray* listStsClusters {nullptr};
  TClonesArray* listStsHits {nullptr};
  TClonesArray* listStsHitMatch {nullptr};
  TClonesArray* listStsClusterMatch {nullptr};

  TClonesArray* listMvdHits {nullptr};
  TClonesArray* listMvdDigiMatches {nullptr};
  TClonesArray* listMvdHitMatches {nullptr};

  //MuCh

  CbmMCDataArray* fMuchPoints {nullptr};
  TClonesArray* listMuchHitMatches {nullptr};  // Output CbmMatch array
  TClonesArray* fDigiMatchesMuch {nullptr};
  TClonesArray* fClustersMuch {nullptr};
  TClonesArray* fMuchPixelHits {nullptr};  // CbmMuchPixelHit array
  TClonesArray* fDigisMuch {nullptr};

  //TRD

  CbmTrdParSetDigi* fTrdDigiPar {nullptr};     //!
  CbmTrdParModDigi* fTrdModuleInfo {nullptr};  //!

  CbmMCDataArray* fTrdPoints {nullptr};
  TClonesArray* listTrdHits {nullptr};
  TClonesArray* fTrdHitMatches {nullptr};

  //ToF
  CbmMCDataArray* fTofPoints {nullptr};
  TClonesArray* fTofHitDigiMatches {nullptr};  // CbmMatches array
  TClonesArray* fTofHits {nullptr};            // CbmMatches array
  CbmTofDigiPar* fDigiPar {nullptr};
  CbmTofDigiBdfPar* fTofDigiBdfPar {nullptr};
  vector<vector<int>> TofPointToTrack;

  TFile* fPerfFile {nullptr};
  TDirectory* fHistoDir {nullptr};

  static const int fNTimeHistos = 22;
  TH1F* fTimeHisto[fNTimeHistos] {nullptr};

  static const int fNGhostHistos = 9;
  TH1F* fGhostHisto[fNGhostHistos] {nullptr};

  /// Used data = Repacked input data
  L1Vector<CbmL1Hit> vStsHits {"CbmL1::vStsHits"};  // hits with hit-mcpoint match information
  L1Vector<int> SortedIndex {"CbmL1::SortedIndex"};
  L1Vector<int> StsIndex {"CbmL1::StsIndex"};
  L1Vector<CbmL1MCTrack> vMCTracks {"CbmL1::vMCTracks"};
  L1Vector<int> vHitMCRef {
    "CbmL1::vHitMCRef"};  // indices of MCPoints in vMCPoints, indexed by index of hit in algo->vStsHits array. According to StsMatch. Used for IdealResponce
                          //    L1Vector<int>          vHitMCRef1;
                          //   CbmMatch stsHitMatch;

  DFEI2I dFEI2vMCPoints {};
  DFEI2I dFEI2vMCTracks {};

  int fFindParticlesMode {0};  // 0 - don't run FindParticles
                               // 1 - run, all MC particle is reco-able
                               // 2 - run, MC particle is reco-able if created from reco-able tracks
                               // 3 - run, MC particle is reco-able if created from reconstructed tracks

  TString fStsMatBudgetFileName {};
  TString fMvdMatBudgetFileName {};
  TString fMuchMatBudgetFileName {};
  TString fTrdMatBudgetFileName {};
  TString fTofMatBudgetFileName {};
  bool fExtrapolateToTheEndOfSTS {false};
  bool fLegacyEventMode {false};

  KFTopoPerformance* fTopoPerformance {nullptr};
  L1EventEfficiencies fEventEfficiency {};  // average efficiencies

  CbmStsParSetSensor* fStsParSetSensor {nullptr};
  CbmStsParSetSensorCond* fStsParSetSensorCond {nullptr};
  CbmStsParSetModule* fStsParSetModule {nullptr};

  ClassDef(CbmL1, 0);
};

#endif  //_CbmL1_h_
