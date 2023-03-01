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


#include "CbmL1DetectorID.h"
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

#include <boost/functional/hash.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string_view>
#include <unordered_map>
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
class TNtuple;


/// Internal structure to handle link keys
struct CbmL1LinkKey {
  /// Constructor from links
  CbmL1LinkKey(int32_t index, int32_t entry, int32_t file) : fIndex(index), fEntry(entry), fFile(file) {}
  bool operator==(const CbmL1LinkKey& other) const
  {
    return fFile == other.fFile && fEntry == other.fEntry && fIndex == other.fIndex;
  }

  int32_t fIndex = -1;  ///< index of point/track, saved to link
  int32_t fEntry = -1;  ///< index of link entry
  int32_t fFile  = -1;  ///< index of link file
};

/// Hash for CbmL1LinkKey
namespace std
{
  template<>
  struct hash<CbmL1LinkKey> {
    std::size_t operator()(const CbmL1LinkKey& key) const
    {
      std::size_t res = 0;
      boost::hash_combine(res, key.fFile);
      boost::hash_combine(res, key.fEntry);
      boost::hash_combine(res, key.fIndex);
      return res;
    }
  };
}  // namespace std



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

  using DFSET = std::set<std::pair<int, int>>;

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
  ///                           - #0 run without performance measurement
  ///                           - #1 standard efficiency definition
  ///                           - #2 QA efficiency definition
  /// \param  dataMode          Option to work with files for the standalone mode
  ///                           - #0 standalone mode is not used
  ///                           - #1 data for standalone mode is written to configuration file (currently does not work)
  ///                           - #2 tracking runs in standalone mode using configuration file (currently does not work)
  ///                           - #3 data is written and read (currently does not work)
  ///                           - #4 parameters file is saved, but the data does not
  /// \param  dataDir           Name of directory for configuration file
  /// \param  findParticleMode  Find particle utility mode
  ///                           - #0 FindParticles is not used
  ///                           - #1 All MC particles are reconstructable
  ///                           - #2 MC particles are reconstructable if created from reconstructable tracks
  ///                           - #3: MC particles are reconstructable if created from reconstructed tracks
  CbmL1(const char* name, Int_t verbose = 1, Int_t performance = 0, int dataMode = 0, const char* dataDir = "",
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
  void SetInputConfigName(const char* filename);

  /// \brief Sets a name for the input search window file
  /// If the filename is not defined, a default track finder with Kalman filter is used. Otherwise, an experimental
  /// version of track finder with estimated hit search windows is utilised
  /// \param filename Name of the input search window file
  void SetInputSearchWindowFilename(const char* filename) { fsInputSearchWindowsFilename = filename; }

  /// \brief Sets a name for the output configuration file
  /// \param filename  Name of the input tracking configuration file
  void SetOutputConfigName(const char* filename) { fInitManager.SetOutputConfigName(std::string(filename)); }

  /// \brief Sets output file for MC triplets tree
  /// If the filename is empty string, tree is not filled
  /// \param filename Name of the output file name
  void SetOutputMcTripletsTreeFilename(const char* filename) { fsMcTripletsOutputFilename = std::string(filename); }

  /// Sets flag: to correct input hits on MC or not
  /// \param flag: true - hits will be corrected on MC information
  void SetCorrectHitsOnMC(bool flag) { fIfCorrectHitsOnMC = flag; }

  /// Gets flag: to correct input hits on MC or not
  bool GetCorrectHitsOnMC() const { return fIfCorrectHitsOnMC; }

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
  void SetMissingHits(bool value) { fMissingHits = value; }
  void SetStsOnlyMode() { fTrackingMode = L1Algo::TrackingMode::kSts; }
  void SetMcbmMode() { fTrackingMode = L1Algo::TrackingMode::kMcbm; }
  void SetGlobalMode() { fTrackingMode = L1Algo::TrackingMode::kGlobal; }


  //   void SetTrackingLevel( Int_t iLevel ){ fTrackingLevel = iLevel; }
  //   void MomentumCutOff( Double_t cut ){ fMomentumCutOff = cut; }
  //   void SetGhostSuppression( Bool_t b ){ fGhostSuppression= b; }
  //   void SetDetectorEfficiency( Double_t eff ){ fDetectorEfficiency = eff; }

  /// Reconstructs tracks in a given event
  /// \param event  Pointer to current CbmEvent object
  void Reconstruct(CbmEvent* event = nullptr);


  //   static bool compareZ(const int &a, const int &b );
  inline double Get_Z_vMCPoint(int a) const { return fvMCPoints[a].z; }

  const L1Vector<CbmL1MCPoint>& GetMcPoints() const { return fvMCPoints; }

  const L1Vector<CbmL1MCTrack>& GetMcTracks() const { return fvMCTracks; }

  const L1Vector<int>& GetHitMCRefs() const { return fvHitPointIndexes; }

  void SetUseMcHit(int MvdUseMcHit = 0, int StsUseMcHit = 0, int MuchUseMcHit = 0, int TrdUseMcHit = 0,
                   int TofUseMcHit = 0)
  {
    fMvdUseMcHit  = MvdUseMcHit;
    fStsUseMcHit  = StsUseMcHit;
    fMuchUseMcHit = MuchUseMcHit;
    fTrdUseMcHit  = TrdUseMcHit;
    fTofUseMcHit  = TofUseMcHit;
  }

  static double boundedGaus(double sigma);

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
  /// \param event        Pointer to the current CbmEvent object
  void ReadEvent(CbmEvent* event = NULL);

  /// Converts data from generic FairMCPoint based class to the CbmL1MCPoint
  /// \param   MC       Pointer to a target CbmL1MCPoint object
  /// \param   iPoint   Index of the point into the input MC points CbmMCDataArray object for the particular detector
  /// \param   file     Index of the input file
  /// \param   event    Index of the input event
  /// \param   iDet     Index of the detector subsystem
  /// \return  flag: false - success, true - some errors occurred
  // TODO: Probably, we should replace input parameter MVD with the template  (S.Zharko)
  bool ReadMCPoint(CbmL1MCPoint* MC, int iPoint, int file, int event, int iDet);

  //   static bool compareZ(const int &a, const int &b );
  //   bool compareZ(const int &a, const int &b );

  /// Fills the fvMCTracks vector and the fmMCTracksMap
  void Fill_vMCTracks();

  /*
   * Input Performance
   */

  /// Matches hit with MC point
  /// \tparam  DetId Detector ID
  /// \param   iHit  External index of hit
  /// \return        MC-point index in fvMCPoints array
  template<L1DetectorID DetId>
  int MatchHitWithMc(int iHit) const;

  /// Procedure for match hits and MCPoints.
  /// Reads information about correspondence between hits and MC-points and fill CbmL1MCPoint::hitIds and CbmL1Hit::mcPointIds arrays
  /// should be called after reading the event
  void HitMatch();

  void FieldApproxCheck();    // Build histograms with difference between Field map and approximated field
  void FieldIntegralCheck();  // Build 2D histogram: dependence of the field integral on phi and theta
  void InputPerformance();    // Build histograms about input data, like hit pulls, etc.
  void TimeHist();

  // ********************************
  // ** Reconstruction Performance **
  // ********************************


  /// Matches reconstructed and MC tracks
  /// \note Should be called before Performances
  void TrackMatch();

  /// Calculates tracking efficiencies (counters)
  void EfficienciesPerformance();

  /// Builds pulls and residuals
  /// \note Should be called only after CbmL1::Performance()
  void TrackFitPerformance();

  void FillFitHistos(L1TrackPar& tr, const CbmL1MCPoint& mc, bool isTimeFitted, TH1F* h[]);

  /// Fills performance histograms
  void HistoPerformance();

  /// Writes MC triplets to tree
  /// \note Executed only if the filename for MC tracks ntuple output is defined
  void DumpMCTripletsToTree();


  // ** STandAlone Package service-functions **

  /// @brief Defines names for output in STAP mode
  /// @param dirName  Name of output directory for STAP data
  ///
  /// Defines the name of input/output directory [dir] and prefix of the files [pref], which is used to define
  /// input and output data trees in the reconstruction macro. If the output TTree file has name
  /// /path/to/[pref].reco.root, the data files will be:
  ///   [dir]/input_hits/[pref].job[No].L1InputData.dat - hits input files, containing serialized L1InputData objects,
  ///     stored for each job (each call of CbmL1::ReadEvent function)
  ///   [dir]/[pref].L1Parameters.dat - parameters input files, containing serialized L1Parameters object
  void DefineSTAPNames(const char* dirName);

  /// Writes initialized L1Parameters object to file ""
  void WriteSTAPParamObject();

  /// Writes a sample of an L1InputData object to defined directory fSTAPDataDir
  /// \param iJob  Number of job, usually is defined by the nCalls of executing function
  /// \note  Creates a file fSTAPDataDir + "/" + fSTAPDataPrefix + "." + TString::Format(kSTAPAlgoIDataSuffix, iJob)
  void WriteSTAPAlgoInputData(int iJob = 0);

  void WriteSTAPPerfInputData();

  /// Reads a sample of an L1InputData object from defined directory fSTAPDataDir
  /// \param iJob  Number of job, usually is defined by the nCalls of executing function
  /// \note  Reads from a file fSTAPDataDir + "/" + fSTAPDataPrefix + "." + TString::Format(kSTAPAlgoIDataSuffix, iJob)
  void ReadSTAPParamObject();

  void ReadSTAPAlgoInputData(int iJob = 0);

  void ReadSTAPPerfInputData();

  /// SIMD KF Banchmark service-functions
  void WriteSIMDKFData();

  /// Gets a pointer to L1InitManager (for an access in run_reco.C)
  L1InitManager* GetInitManager() { return &fInitManager; }

  void WriteHistosCurFile(TObject* obj);

  static std::istream& eatwhite(std::istream& is);  // skip spaces
  static void writedir2current(TObject* obj);       // help procedure

private:
  std::string fInputDataFilename           = "";  ///< File name to read/write input hits
  std::string fsInputSearchWindowsFilename = "";  ///< File name to read search windows

  // ***************************
  // ** Member variables list **
  // ***************************


  L1InitManager fInitManager;                                  ///< Tracking parameters data manager
  std::shared_ptr<L1IODataManager> fpIODataManager = nullptr;  ///< Input-output data manager

  //std::unique_ptr<CbmCaMCModule> fpMCModule = nullptr;  ///< MC-module for tracking


public:
  // ** Basic data members **

  L1Algo* fpAlgo = nullptr;  ///< Pointer to the L1 track finder algorithm

  bool fMissingHits = false;  ///< Turns on several ad-hock settings for "mcbm_beam_2021_07_surveyed.100ev" setup

  L1Algo::TrackingMode fTrackingMode = L1Algo::TrackingMode::kSts;  ///< Tracking mode: kSts, kMcbm or kGlobal

  DFSET fvSelectedMcEvents {};  ///< Set of selected MC events with fileID and eventId


  L1Vector<CbmL1Track> fvRecoTracks = {"CbmL1::fvRecoTracks"};  ///< Reconstructed tracks container

private:
  static CbmL1* fpInstance;  ///< Instance of CbmL1

  int fNpointsMvd  = 0;  ///< Number of MC points for MVD
  int fNpointsSts  = 0;  ///< Number of MC points for STS
  int fNpointsMuch = 0;  ///< Number of MC points for MuCh
  int fNpointsTrd  = 0;  ///< Number of MC points for TRD
  int fNpointsTof  = 0;  ///< Number of MC points for TOF

  int fNpointsMvdAll  = 0;  ///< Number of MC points for MVD
  int fNpointsStsAll  = 0;  ///< Number of MC points for STS
  int fNpointsMuchAll = 0;  ///< Number of MC points for MuCh
  int fNpointsTrdAll  = 0;  ///< Number of MC points for TRD
  int fNpointsTofAll  = 0;  ///< Number of MC points for TOF

  L1Vector<CbmL1MCPoint> fvMCPoints = {"CbmL1::fvMCPoints"};          ///< Container of MC points

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

  TString fSTAPDataDir    = ".";     ///< Name of input/output directory for running in a STAP mode
  TString fSTAPDataPrefix = "test";  ///< Name of input/output file prefix. The prefix is defined by output TTree file

  /// Extension for IO of the L1Parameters object
  static constexpr std::string_view kSTAPParamSuffix = "L1Parameters.dat";

  /// Extension for IO of the L1InputData object
  /// \note IO of the L1InputData object is called inside every launch of CbmL1::ReadEvent function. Inside the function
  ///       there is a static counter, which calculates the job (function call) number. One have to define the name of
  ///       the kSTAPAlgoIDataSuffix containing '%d' control symbol, which is replaced with the current job number.
  ///       \example The file name with [pref] = auau.mbias.eb.100ev and [suff] = "job%d.L1InputData.dat" for the job
  ///       number 10 is auau.mbias.eb.100ev.job10.L1InputData.dat
  static constexpr std::string_view kSTAPAlgoIDataSuffix = "job%d.L1InputData.dat";

  /// Name of subdirectory for handling L1InputData objects
  static constexpr std::string_view kSTAPAlgoIDataDir = "input_hits";


  Int_t fTrackingLevel     = 2;     // currently not used
  Double_t fMomentumCutOff = 0.1;   // currently not used
  Bool_t fGhostSuppression = true;  // currently not used

  /// Flags to adjust hits with MC information
  /// 1: Position of a reconstructed hit is taken from matched MC point and smeared (optionally) with the position
  ///    resolution
  /// 2: A set of hits is created from the set of MC points
  Int_t fMvdUseMcHit  = -1;  ///< MC info flag for MVD hits
  Int_t fStsUseMcHit  = -1;  ///< MC info flag for STS hits
  Int_t fMuchUseMcHit = -1;  ///< MC info flag for MuCh hits
  Int_t fTrdUseMcHit  = -1;  ///< MC info flag for TRD hits
  Int_t fTofUseMcHit  = -1;  ///< MC info flag for TOF hits

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
  CbmMCEventList* fpMcEventList    = nullptr;  ///< MC event list (all)
  CbmMCDataArray* fpMcTracks       = nullptr;  ///< MC tracks list
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


  L1Vector<CbmL1MCTrack> fvMCTracks = {"CbmL1::fvMCTracks"};         ///< Array of MC tracks
  L1Vector<int> fvHitPointIndexes   = {"CbmL1::fvHitPointIndexes"};  ///< Indexes of MC points vs. hit index

public:
  // ** Repacked input data **

  L1Vector<CbmL1HitId> fvExternalHits = {"CbmL1::fvExternalHits"};  ///< Array of hits

private:
  L1Vector<CbmL1HitDebugInfo> fvHitDebugInfo = {
    "CbmL1::fvHitDebugInfo"};  ///< Container of hits with extended information
  // indices of MCPoints in fvMCPoints, indexed by index of hit in algo->vHits array. According to StsMatch. Used for IdealResponce
  //    L1Vector<int>          vHitMCRef1;
  //   CbmMatch HitMatch;
private:
  std::unordered_map<CbmL1LinkKey, int> fmMCPointsLinksMap = {};  /// Internal MC point index vs. link
  std::unordered_map<CbmL1LinkKey, int> fmMCTracksLinksMap = {};  /// Internal MC track index vs. link


  // *****************************
  // ** Tracking performance QA **
  // *****************************
  // TODO: move to a separate class (S.Zharko)
  TFile* fPerfFile {nullptr};
  TDirectory* fHistoDir {nullptr};
  TDirectory* fTableDir {nullptr};

  static const int fNTimeHistos = 22;
  TH1F* fTimeHisto[fNTimeHistos] {nullptr};

  static const int fNGhostHistos = 9;
  TH1F* fGhostHisto[fNGhostHistos] {nullptr};

  TFile* fpMcTripletsOutFile             = nullptr;  ///< File to save MC-triplets tree
  TTree* fpMcTripletsTree                = nullptr;  ///< Tree to save MC-triplets
  std::string fsMcTripletsOutputFilename = "";       ///< Name of file to save MC-triplets tree

  int fFindParticlesMode {0};  // 0 - don't run FindParticles
                               // 1 - run, all MC particle is reco-able
                               // 2 - run, MC particle is reco-able if created from reco-able tracks
                               // 3 - run, MC particle is reco-able if created from reconstructed tracks

  std::unordered_map<L1DetectorID, TString>
    fMatBudgetFileName {};  ///< Map for material budget file names vs. detectorID

  bool fExtrapolateToTheEndOfSTS {false};

  KFTopoPerformance* fTopoPerformance {nullptr};
  L1EventEfficiencies fEventEfficiency {};  // average efficiencies

  ClassDef(CbmL1, 0);
};


#endif  //_CbmL1_h_
