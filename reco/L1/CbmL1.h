/* Copyright (C) 2006-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel,  Sergey Gorbunov, Maksym Zyzak, Valentina Akishina, Igor Kulakov, Denis Bertini [committer], Sergei Zharko */

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
#include "L1IODataManager.h"
#include "L1InitManager.h"

class L1Algo;
class L1Event;
class CbmL1ParticlesFinder;
class L1FieldSlice;
class CbmL1Track;
class CbmL1MCTrack;
class KFTopoPerformance;
class CbmMCDataObject;

class CbmEvent;
class TProfile2D;

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
public:
  // **********************
  // ** Types definition **
  // **********************

  using DFSET  = std::set<std::pair<int, int>>;  // Why std::set<std::pair<>> instead of std::map<>?
  using DFEI2I = std::map<Double_t, Int_t>;

  // **************************
  // ** Friends classes list **
  // **************************

  friend class L1AlgoDraw;
  friend class L1AlgoPulls;

  template<int NHits>
  friend class L1AlgoEfficiencyPerformance;

  friend class CbmL1MCTrack;
  friend class CbmL1PFFitter;


  // **********************************
  // ** Member functions declaration **
  // **********************************

  // ** Constructors and destructor **

  /// Default constructor
  CbmL1();

  /// Constructor from parameters
  /// \param  name              Name of the task
  /// \param  verbose           Verbosity level
  /// \param  performance       Performance run flag:
  ///                           0: run without performance measurement
  ///                           1: standard efficiency definition
  ///                           2: QA efficiency definition
  /// \param  dataMode          Option to work with files for the standalone mode
  ///                           0: standalone mode is not used
  ///                           1: data for standalone mode is written to configuration file (currently does not work)
  ///                           2: tracking runs in standalone mode using configuration file (currently does not work)
  ///                           3: data is written and read (currently does not work)
  /// \param  dataDir           Name of directory for configuration file
  /// \param  findParticleMode  Find particle utility mode
  ///                           0: FindParticles is not used
  ///                           1: All MC particles are reconstructable
  ///                           2: MC particles are reconstructable if created from reconstructable tracks
  ///                           3: MC particles are reconstructable if created from reconstructed tracks
  CbmL1(const char* name, Int_t verbose = 1, Int_t performance = 0, int dataMode = 0, const TString& dataDir = "./",
        int findParticleMode = 0);

  /// Copy constructor
  CbmL1(const CbmL1&) = delete;

  /// Move constructor
  CbmL1(CbmL1&&) = delete;

  /// Copy assignment operator
  CbmL1& operator=(const CbmL1&) = delete;

  /// Move assignment operator
  CbmL1& operator=(CbmL1&&) = delete;

  /// Destructor
  ~CbmL1();

  /// Pointer to CbmL1 instance
  static CbmL1* Instance() { return fpInstance; }


  // ** Member functions override from FairTask **

  /// Defines action in the beginning of the run (initialization)
  virtual InitStatus Init();

  /// Reruns the initialization
  virtual InitStatus ReInit();

  /// Defines action in the end of the run (saves results)
  void Finish();


  // ** Specific member functions **

  /// Checks, if detector subsystems are present in the setup
  /// If a particular subsystem is absent, it is not used in tracking.
  void CheckDetectorPresence();


  /// Sets material budget file name for MVD
  void SetMvdMaterialBudgetFileName(const TString& fileName)
  {
    if (fileName != "") fMatBudgetFileName[L1DetectorID::kMvd] = fileName;
  }

  /// Sets material budget file name for STS
  void SetStsMaterialBudgetFileName(const TString& fileName)
  {
    if (fileName != "") fMatBudgetFileName[L1DetectorID::kSts] = fileName;
  }

  /// Sets material budget file name for MuCh
  void SetMuchMaterialBudgetFileName(const TString& fileName)
  {
    if (fileName != "") fMatBudgetFileName[L1DetectorID::kMuch] = fileName;
  }

  /// Sets material budget file name for TRD
  void SetTrdMaterialBudgetFileName(const TString& fileName)
  {
    if (fileName != "") fMatBudgetFileName[L1DetectorID::kTrd] = fileName;
  }

  /// Sets material budget file name for TOF
  void SetTofMaterialBudgetFileName(const TString& fileName)
  {
    if (fileName != "") fMatBudgetFileName[L1DetectorID::kTof] = fileName;
  }

  /// \brief Sets a name for the input configuration file
  /// If the file is undefined, default tracking parameters will be used. Otherwise, the default parameters will be
  /// overridden with ones from the configuration file
  /// \param filename  Name of the input tracking configuration file
  void SetInputConfigName(const char* filename) { fInitManager.SetInputConfigName(std::string(filename)); }

  /// \brief Sets a name for the output configuration file
  /// \param filename  Name of the input tracking configuration file
  void SetOutputConfigName(const char* filename) { fInitManager.SetOutputConfigName(std::string(filename)); }

  /// Sets flag: to correct input hits on MC or not
  /// \param flag: true - hits will be corrected on MC information
  void SetCorrectHitsOnMC(bool flag) { fIfCorrectHitsOnMC = flag; }

  /// Gets flag: to correct input hits on MC or not
  bool GetCorrectHitsOnMC() const { return fIfCorrectHitsOnMC; }

  /// Correction function for the material budget map
  /// It fills bins with no statistics
  template<L1DetectorID detID>
  void ApplyCorrectionToMaterialMap(L1Material& material, const L1MaterialInfo& homogenious);

  /// Utility to map the L1DetectorID items into detector names
  static constexpr const char* GetDetectorName(L1DetectorID detectorID)
  {
    switch (detectorID) {
      case L1DetectorID::kMvd: return "MVD";
      case L1DetectorID::kSts: return "STS";
      case L1DetectorID::kMuch: return "MuCh";
      case L1DetectorID::kTrd: return "TRD";
      case L1DetectorID::kTof: return "TOF";
    }
    // TODO: Probably, we should provide default with throwing exception here... (S.Zharko)
    return "";
  }

  /// Reads material budget information: station thickness in units of radiation length vs. point at the XY plane
  /// \param detectorID  ID of a detector subsystem
  std::vector<L1Material> ReadMaterialBudget(L1DetectorID detectorID);


  void SetExtrapolateToTheEndOfSTS(bool b) { fExtrapolateToTheEndOfSTS = b; }
  void SetLegacyEventMode(bool b) { fLegacyEventMode = b; }
  void SetUseHitErrors(bool value) { fUseHitErrors = value; }
  void SetMissingHits(bool value) { fMissingHits = value; }
  void SetStsOnlyMode() { fTrackingMode = L1Algo::TrackingMode::kSts; }
  void SetMcbmMode() { fTrackingMode = L1Algo::TrackingMode::kMcbm; }
  void SetGlobalMode() { fTrackingMode = L1Algo::TrackingMode::kGlobal; }


  //   void SetTrackingLevel( Int_t iLevel ){ fTrackingLevel = iLevel; }
  //   void MomentumCutOff( Double_t cut ){ fMomentumCutOff = cut; }
  //   void SetGhostSuppression( Bool_t b ){ fGhostSuppression= b; }
  //   void SetDetectorEfficiency( Double_t eff ){ fDetectorEfficiency = eff; }

  /// Reconstructs an event
  /// \param event  Pointer to current CbmEvent object
  void Reconstruct(CbmEvent* event = nullptr);

  //  bool ReadMCDataFromFile(const char work_dir[100], const int maxNEvent, const int iVerbose);
  //   vector<CbmL1MCPoint> fvMCPoints;

  //   static bool compareZ(const int &a, const int &b );
  inline double Get_Z_vMCPoint(int a) const { return fvMCPoints[a].z; }

  const L1Vector<CbmL1MCPoint>& GetMcPoints() const { return fvMCPoints; }

  const L1Vector<int>& GetHitMCRefs() const { return fvHitPointIndexes; }


private:

  struct TH1FParameters {
    TString name, title;
    int nbins;
    float xMin, xMax;
  };

  /// Runs ideal track finder: copies all MC-tracks into reconstructed tracks
  void IdealTrackFinder();

  /// Read information about hits, mcPoints and mcTracks into L1 classes

  /// Repacks data from the external TClonesArray objects to the internal L1 arrays
  /// \param TsStart      Reference to the time slice start time
  /// \param TsLength     Reference to the time slice length
  /// \param TsOverlap    Reference to the time slice overlap length (does not used at the moment)
  /// \param FstHitinTs   Index of the first hit in the time-slice
  /// \param areDataLeft  Flag: true - data were left after reading the sub-timeslice
  /// \param event        Pointer to the current CbmEvent object
  void ReadEvent(float& TsStart, float& TsLength, float& TsOverlap, int& FstHitinTs, bool& areDataLeft,
                 CbmEvent* event = NULL);

  /// Converts data from generic FairMCPoint based class to the CbmL1MCPoint (dummy method)
  /// \param   MC       Pointer to a target CbmL1MCPoint object
  /// \param   iPoint   Index of the point into the input MC points CbmMCDataArray object for the particular detector
  /// \param   MVD      Index of the detector subsystem
  /// \return  flag: false - success, true - some errors occurred
  bool ReadMCPoint(CbmL1MCPoint* MC, int iPoint, int MVD);  // help procedure

  /// Converts data from generic FairMCPoint based class to the CbmL1MCPoint
  /// \param   MC       Pointer to a target CbmL1MCPoint object
  /// \param   iPoint   Index of the point into the input MC points CbmMCDataArray object for the particular detector
  /// \param   file     Index of the input file
  /// \param   event    Index of the input event
  /// \param   MVD      Index of the detector subsystem
  /// \return  flag: false - success, true - some errors occurred
  // TODO: Probably, we should replace input parameter MVD with the template  (S.Zharko)
  bool ReadMCPoint(CbmL1MCPoint* MC, int iPoint, int file, int event, int MVD);

  //   static bool compareZ(const int &a, const int &b );
  //   bool compareZ(const int &a, const int &b );

  /// Fills the fvMCTracks vector and the dFEI2vMCTracks set
  void Fill_vMCTracks();

  /*
   * Input Performance
   */

  /// Procedure for match hits and MCPoints.
  /// Reads information about correspondence between hits and MC-points and fill CbmL1MCPoint::hitIds and CbmL1Hit::mcPointIds arrays
  /// should be called after fill of algo
  void HitMatch();

  void FieldApproxCheck();    // Build histograms with difference between Field map and approximated field
  void FieldIntegralCheck();  // Build 2D histogram: dependence of the field integral on phi and theta
  void InputPerformance();    // Build histograms about input data, like hit pulls, etc.
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

  /// Gets a pointer to L1InitManager (for an access in run_reco.C)
  L1InitManager* GetInitManager() { return &fInitManager; }

  void SetUseMcHit(int StsUseMcHit = 0, int MuchUseMcHit = 0, int TrdUseMcHit = 0, int TofUseMcHit = 0)
  {
    fStsUseMcHit  = StsUseMcHit;
    fMuchUseMcHit = MuchUseMcHit;
    fTrdUseMcHit  = TrdUseMcHit;
    fTofUseMcHit  = TofUseMcHit;
  }

  void WriteHistosCurFile(TObject* obj);

  static std::istream& eatwhite(std::istream& is);  // skip spaces
  static void writedir2current(TObject* obj);       // help procedure

  inline Double_t dFEI(int file, int event, int idx) { return (1000 * idx) + file + (0.0001 * event); }


  // ***************************
  // ** Member variables list **
  // ***************************

public:
  // ** Basic data members **

  L1Algo* fpAlgo = nullptr;  ///< Pointer to the L1 track finder algorithm

  L1InitManager fInitManager;      ///< Tracking parameters data manager
  L1IODataManager fIODataManager;  ///< Input-output data manager

  bool fUseHitErrors = true;   ///<
  bool fMissingHits  = false;  ///< Turns on several ad-hock settings for "mcbm_beam_2021_07_surveyed.100ev" setup

  L1Algo::TrackingMode fTrackingMode = L1Algo::TrackingMode::kSts;  ///< Tracking mode: kSts, kMcbm or kGlobal

  DFSET fvFileEvent {};  ///< Map of fileID to eventId


  L1Vector<CbmL1Track> fvRecoTracks = {"CbmL1::fvRecoTracks"};  ///< Reconstructed tracks container

private:
  static CbmL1* fpInstance;  ///< Instance of CbmL1

  int nMvdPoints = 0;  // TODO: Should be removed (S.Zharko)

  L1Vector<CbmL1MCPoint> fvMCPoints = {"CbmL1::fvMCPoints"};          ///< Container of MC points
  L1Vector<int> fvMCPointIndexesTs  = {"CbmL1::fvMCPointIndexesTs"};  ///< Indexes of MC points in TS

  int fNStations     = 0;  ///< number of total active detector stations
  int fNMvdStations  = 0;  ///< number of active MVD stations
  int fNStsStations  = 0;  ///< number of active STS stations
  int fNMuchStations = 0;  ///< number of active MuCh stations
  int fNTrdStations  = 0;  ///< number of active TRD stations
  int fNTofStations  = 0;  ///< number of active TOF stations

  int fNStationsGeom     = 0;  ///< number of total detector stations
  int fNMvdStationsGeom  = 0;  ///< number of MVD stations
  int fNStsStationsGeom  = 0;  ///< number of STS stations;
  int fNMuchStationsGeom = 0;  ///< number of MuCh stations;
  int fNTrdStationsGeom  = 0;  ///< number of TRD stations;
  int fNTofStationsGeom  = 0;  ///< number of TOF stations;


  Int_t fPerformance   = 0;   ///< performance mode: 0 - w\o perf. 1 - L1-Efficiency definition. 2 - QA-Eff.definition
  double fTrackingTime = 0.;  ///< time of track finding procedure

  /// Option to work with file for standalone package (currently does not work)
  /// 0 (off) , 1 (write), 2 (read data and work only with it), 3 (debug - write and read)
  int fSTAPDataMode = 0;

  TString fSTAPDataDir {};

  Int_t fTrackingLevel     = 2;     // currently not used
  Double_t fMomentumCutOff = 0.1;   // currently not used
  Bool_t fGhostSuppression = true;  // currently not used

  /// TODO: change to bool
  Int_t fStsUseMcHit  = -1;  ///< if STS data should be processed
  Int_t fMuchUseMcHit = -1;  ///< if Much data should be processed
  Int_t fTrdUseMcHit  = -1;  ///< if Trd data should be processed
  Int_t fTofUseMcHit  = -1;  ///< if Tof data should be processed

  bool fUseMVD  = false;  ///< if Mvd data should be processed
  bool fUseSTS  = false;  ///< if Mvd data should be processed
  bool fUseMUCH = false;  ///< if Much data should be processed
  bool fUseTRD  = false;  ///< if Trd data should be processed
  bool fUseTOF  = false;  ///< if Tof data should be processed

  bool fIfCorrectHitsOnMC = false;  ///< if correct input hits on MC information (debug)

  // ** Raw input data **

  CbmTimeSlice* fTimeSlice = nullptr;  ///< Pointer to the TS object

  // Reconstructed hits input
  TClonesArray* fpMvdHits       = nullptr;  ///< Array of MVD hits ("MvdHit")
  TClonesArray* fpStsHits       = nullptr;  ///< Array of STS hits ("StsHit")
  TClonesArray* fpMuchPixelHits = nullptr;  ///< Array of MuCh hits ("MuchPixelHit")
  TClonesArray* fpTrdHits       = nullptr;  ///< Array of TRD hits ("TrdHit")
  TClonesArray* fpTofHits       = nullptr;  ///< Array of TOF hits ("TofHit")

  // ** MC input **

  // General
  CbmMCEventList* fpEventList      = nullptr;  ///< MC event list (all)
  CbmMCDataArray* fpMCTracks       = nullptr;  ///< MC tracks list
  CbmMCDataObject* fpMcEventHeader = nullptr;  ///< MC event header

  // MC-point arrays
  CbmMCDataArray* fpMvdPoints  = nullptr;  ///< MVD MC-points array
  CbmMCDataArray* fpStsPoints  = nullptr;  ///< STS MC-points array
  CbmMCDataArray* fpMuchPoints = nullptr;  ///< MuCh MC-points array
  CbmMCDataArray* fpTrdPoints  = nullptr;  ///< TRD MC-points array
  CbmMCDataArray* fpTofPoints  = nullptr;  ///< TOF MC-points array


  // ** MC-reconstruction matching input **

  // Measured digis
  TClonesArray* fpMuchDigis = nullptr;  ///< Array of MuCh digis

  // Reconstructed clusters
  TClonesArray* fpStsClusters  = nullptr;  ///< Array of STS clusters
  TClonesArray* fpMuchClusters = nullptr;  ///< Array of MuCh clusters

  // Hit matches
  TClonesArray* fpStsHitMatches  = nullptr;  ///< Array of STS hit matches (NOTE: currently not used)
  TClonesArray* fpMvdHitMatches  = nullptr;  ///< Array of MVD hit matches
  TClonesArray* fpMuchHitMatches = nullptr;  ///< Array of MuCh hit matches
  TClonesArray* fpTrdHitMatches  = nullptr;  ///< Array of TOF hit matches
  TClonesArray* fpTofHitMatches  = nullptr;  ///< Array of TRD hit matches

  // Digi and cluster matches
  TClonesArray* fpMvdDigiMatches    = nullptr;  ///< Array of MVD digi matches (NOTE: currently unused)
  TClonesArray* fpStsDigiMatches    = nullptr;  ///< Array of STS digi matches (NOTE: currently unused)
  TClonesArray* fpStsClusterMatches = nullptr;  ///< Array of STS cluster matches
  TClonesArray* fpMuchDigiMatches   = nullptr;  ///< Array of MuCh digi matches (NOTE: currently unsused)

  vector<vector<int>> fTofPointToTrack;  ///<

  // ** Repacked input data **

  L1Vector<CbmL1Hit> fvExternalHits = {"CbmL1::fvExternalHits"};  ///< Array of hits with MC information

  /// Indexes of hits after hits sorting (used only with fLegacyEventMode = true)
  L1Vector<int> fvSortedHitsIndexes = {"CbmL1::fvSortedHitsIndexes"};

  /// Indexes of STS hits in fpStsHits array after hits sorting (used only with fLegacyEventMode = true)
  L1Vector<int> fvSortedStsHitsIndexes = {"CbmL1::fvSortedStsHitsIndexes"};
  L1Vector<CbmL1MCTrack> fvMCTracks    = {"CbmL1::fvMCTracks"};         ///< Array of MC tracks
  L1Vector<int> fvHitPointIndexes      = {"CbmL1::fvHitPointIndexes"};  ///< Indexes of MC points vs. hit index

public:
  L1Vector<CbmL1HitStore> fvHitStore = {"CbmL1::fvHitStore"};  ///< Container of hits with extended information
  // indices of MCPoints in fvMCPoints, indexed by index of hit in algo->vHits array. According to StsMatch. Used for IdealResponce
  //    L1Vector<int>          vHitMCRef1;
  //   CbmMatch HitMatch;
private:
  DFEI2I dFEI2vMCPoints = {};
  DFEI2I dFEI2vMCTracks = {};

  // *****************************
  // ** Tracking performance QA **
  // *****************************
  // TODO: move to a separate class (S.Zharko)
  TFile* fPerfFile {nullptr};
  TDirectory* fHistoDir {nullptr};

  static const int fNTimeHistos = 22;
  TH1F* fTimeHisto[fNTimeHistos] {nullptr};

  static const int fNGhostHistos = 9;
  TH1F* fGhostHisto[fNGhostHistos] {nullptr};


  int fFindParticlesMode {0};  // 0 - don't run FindParticles
                               // 1 - run, all MC particle is reco-able
                               // 2 - run, MC particle is reco-able if created from reco-able tracks
                               // 3 - run, MC particle is reco-able if created from reconstructed tracks


  std::unordered_map<L1DetectorID, TString>
    fMatBudgetFileName {};  ///< Map for material budget file names vs. detectorID

  bool fExtrapolateToTheEndOfSTS {false};
  bool fLegacyEventMode {false};

  KFTopoPerformance* fTopoPerformance {nullptr};
  L1EventEfficiencies fEventEfficiency {};  // average efficiencies

  ClassDef(CbmL1, 0);
};


// ---------------------------------------------------------------------------------------------------------------------
//
template<L1DetectorID detID>
void CbmL1::ApplyCorrectionToMaterialMap(L1Material& material, const L1MaterialInfo& homogenious)
{
  // TODO: unify the correction function for all detectors
  [[maybe_unused]] float minVal = 0.;
  if constexpr (detID == L1DetectorID::kMuch) { minVal = 0.15f; }
  else if constexpr (detID == L1DetectorID::kTof || detID == L1DetectorID::kTrd) {
    minVal = 0.0015f;
  }

  // A bit ugly solution, but so can we avoid dependency on input maps file
  std::vector<float> keepRow {};
  if constexpr (detID != L1DetectorID::kSts) { keepRow.resize(material.GetNbins()); }

  for (int iBinX = 0; iBinX < material.GetNbins(); ++iBinX) {
    if constexpr (detID == L1DetectorID::kTof) { minVal = 0.0015f; }
    if constexpr (detID != L1DetectorID::kSts) {
      for (int iBinY = 0; iBinY < material.GetNbins(); ++iBinY) {
        keepRow[iBinY] = material.GetRadThick(iBinX, iBinY);
      }
    }
    for (int iBinY = 0; iBinY < material.GetNbins(); ++iBinY) {
      if constexpr (detID == L1DetectorID::kMvd) {
        // Correction for holes in the material map
        if (material.GetRadThick(iBinX, iBinY) < homogenious.RadThick[0]) {
          if (iBinY > 0 && iBinY < material.GetNbins() - 1) {
            material.SetRadThick(iBinX, iBinY, TMath::Min(keepRow[iBinY - 1], keepRow[iBinY + 1]));
          }
        }

        // Correction for the hard-coded value of RadThick of MVD stations
        if (material.GetRadThick(iBinX, iBinY) < 0.0015) { material.SetRadThick(iBinX, iBinY, 0.0015); }
      }
      else if constexpr (detID == L1DetectorID::kSts) {
        if (material.GetRadThick(iBinX, iBinY) < homogenious.RadThick[0]) {
          material.SetRadThick(iBinX, iBinY, homogenious.RadThick[0]);
        }
      }
      else if constexpr (detID == L1DetectorID::kMuch || detID == L1DetectorID::kTrd || detID == L1DetectorID::kTof) {
        // Correction for holes in the material map
        if (L1Algo::TrackingMode::kGlobal != fTrackingMode) {
          if ((iBinY > 0) && (iBinY < material.GetNbins() - 1)) {
            material.SetRadThick(iBinX, iBinY, TMath::Min(keepRow[iBinY - 1], keepRow[iBinY + 1]));
          }
        }
        float val = material.GetRadThick(iBinX, iBinY);
        if (val > 0.0015) {  // remember last non-zero value
          minVal = val;
        }
        else {  // empty bin with no statistics, fill it with the neighbours value
          material.SetRadThick(iBinX, iBinY, minVal);
        }
      }
    }
  }
}


#endif  //_CbmL1_h_
