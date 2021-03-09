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

//#define GLOBAL

/// temporary TEST !!!!
///#define HAVE_SSE

#include "CbmL1Track.h"
#include "CbmL1Vtx.h"

//#include "L1Algo/L1Algo.h"
#include "CbmEvent.h"
#include "CbmL1MCPoint.h"
#include "CbmL1MCTrack.h"
#include "CbmL1StsHit.h"
#include "CbmMCTrack.h"
#include "CbmMvdHit.h"
#include "CbmMvdPoint.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"

#include "FairDetector.h"
#include "FairRootManager.h"
#include "FairTask.h"

//#include "CbmMCEventHeader.h"
//#include "L1AlgoInputData.h"
#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"
#include "CbmTimeSlice.h"

#include "TClonesArray.h"
#include "TGeoBBox.h"
#include "TGeoCone.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoPcon.h"
#include "TGeoTube.h"
#include "TH1.h"
#include "TH2.h"
#include "TParticle.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TRandom.h"
#include "TRefArray.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "L1EventEfficiencies.h"

using std::vector;

class L1AlgoInputData;
class L1Algo;
class L1Event;
class CbmL1ParticlesFinder;
class L1FieldSlice;
class CbmL1Track;
class CbmL1MCTrack;
class KFTopoPerformance;
//class CbmEvBasedArray;
class CbmStsParSetSensor;
class CbmStsParSetSensorCond;
class CbmStsParSetModule;
class CbmTofDigiBdfPar;

class CbmL1HitStore {
public:
  int ExtIndex;
  int iStation;
  double x, y, time, dx, dy, dt, dxy;
  int Det;
};

//class CbmL1FileEvent{
//public:
//  int File;
//  int Event;
//}

class CbmTrdParSetDigi;
class CbmTrdParModDigi;
class CbmTofDigiPar;


class CbmL1 : public FairTask {
private:
  CbmL1(const CbmL1&);
  CbmL1 operator=(const CbmL1&);

public:
  L1Algo* algo;  // for access to L1 Algorithm from L1::Instance

  TString fDigiFile;  // Digitization file
  bool fUseHitErrors;
  bool fmCBMmode;
  bool fGlobalMode;
  vector<CbmL1Track> vRTracks;  // reconstructed tracks
  typedef std::set<std::pair<int, int>> DFSET;
  DFSET vFileEvent;
  // set<pair<int, int> > vFileEvent;
  static CbmL1* Instance() { return fInstance; }

  void SetParContainers();
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

  void SetStsMaterialBudgetFileName(TString fileName) { fStsMatBudgetFileName = fileName; }
  void SetMvdMaterialBudgetFileName(TString fileName) { fMvdMatBudgetFileName = fileName; }
  void SetMuchMaterialBudgetFileName(TString fileName) { fMuchMatBudgetFileName = fileName; }
  void SetTrdMaterialBudgetFileName(TString fileName) { fTrdMatBudgetFileName = fileName; }
  void SetTofMaterialBudgetFileName(TString fileName) { fTofMatBudgetFileName = fileName; }
  void SetExtrapolateToTheEndOfSTS(bool b) { fExtrapolateToTheEndOfSTS = b; }
  void SetDataMode(int TimesliceMode) { fTimesliceMode = TimesliceMode; }
  void SetMuchPar(TString fileName) { fDigiFile = fileName; }
  void SetUseHitErrors(bool value) { fUseHitErrors = value; }
  void SetmCBMmode(bool value) { fmCBMmode = value; }

  void Finish();

  //   void SetTrackingLevel( Int_t iLevel ){ fTrackingLevel = iLevel; }
  //   void MomentumCutOff( Double_t cut ){ fMomentumCutOff = cut; }
  //   void SetGhostSuppression( Bool_t b ){ fGhostSuppression= b; }
  //   void SetDetectorEfficiency( Double_t eff ){ fDetectorEfficiency = eff; }

  vector<CbmL1HitStore> vHitStore;  // diff hit information

  void Reconstruct(CbmEvent* event = NULL);

  friend class L1AlgoDraw;
  friend class L1AlgoPulls;
  template<int NHits>
  friend class L1AlgoEfficiencyPerformance;
  friend class CbmL1MCTrack;
  friend class CbmL1PFFitter;
  //  bool ReadMCDataFromFile(const char work_dir[100], const int maxNEvent, const int iVerbose);
  //   vector<CbmL1MCPoint> vMCPoints;

  //   static bool compareZ(const int &a, const int &b );
  inline double Get_Z_vMCPoint(int a) const { return vMCPoints[a].z; }

private:
  vector<CbmL1MCPoint> vMCPoints;
  int nMvdPoints;
  vector<int> vMCPoints_in_Time_Slice;
  void IdealTrackFinder();  // just copy all reconstructable MCTracks into RecoTracks.

  /// Read information about hits, mcPoints and mcTracks into L1 classes

  void ReadEvent(L1AlgoInputData* fData_, float& fTsStart, float& fTsLength, float& fTsOverlap, int& fFstHitinTs,
                 bool& fnewTS, CbmEvent* event = NULL);

  bool ReadMCPoint(CbmL1MCPoint* MC, int iPoint, int MVD);  // help procedure
  bool ReadMCPoint(CbmL1MCPoint* MC, int iPoint, int file, int event, int MVD);
  //   static bool compareZ(const int &a, const int &b );
  //   bool compareZ(const int &a, const int &b );
  void Fill_vMCTracks();
  /// Input Performance
  void HitMatch();            // Procedure for match hits and MCPoints.
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
  void WriteSTAPGeoData(const vector<float>& geo);  // create geo_algo.dat
  void WriteSTAPAlgoData();                         // create data_algo.dat
  void WriteSTAPPerfData();                         // create data_perfo.dat
  //void ReadSTAPGeoData(vector<float> geo, int &size);
  void ReadSTAPGeoData(vector<float>& geo, int& size);
  void ReadSTAPAlgoData();
  void ReadSTAPPerfData();
  /// SIMD KF Banchmark service-functions
  void WriteSIMDKFData();

  void WriteHistosCurFile(TObject* obj);

  static std::istream& eatwhite(std::istream& is);  // skip spaces
  static void writedir2current(TObject* obj);       // help procedure

  int NStation, NMvdStations, NStsStations, NMuchStations, NTrdStations,
    NTOFStation;       // number of detector stations (all\sts\mvd)
  Int_t fPerformance;  // 0 - w\o perf. 1 - L1-Efficiency definition. 2 - QA-Eff.definition
  int
    fSTAPDataMode;  // way to work with file for standalone package. 0 (off) , 1 (write), 2 (read data and work only with it), 3 (debug - write and read)
  TString fSTAPDataDir;

  Int_t fTrackingLevel;                        // really doesn't used
  Double_t fMomentumCutOff;                    // really doesn't used
  Bool_t fGhostSuppression;                    // really doesn't used
  Bool_t fUseMVD, fUseMUCH, fUseTRD, fUseTOF;  //
  //   Double_t fDetectorEfficiency;  // really doesn't used

  CbmL1Vtx PrimVtx;
  //    L1FieldSlice *targetFieldSlice  _fvecalignment;

  /// Input data
  CbmTimeSlice* fTimeSlice;
  CbmMCEventList* fEventList;  //!  MC event list (all)
  //vector<CbmStsDigi> *listStsDigi;
  vector<CbmStsDigi> listStsDigi;
  CbmMCDataArray* fStsPoints;
  CbmMCDataArray* fMCTracks;
  CbmMCDataArray* fMvdPoints;


  //TClonesArray *listMCTracks ;
  TClonesArray* listStsPts;  // Sts MC points
  //TClonesArray *listStsDigi;
  TClonesArray* listStsDigiMatch;
  TClonesArray* listStsClusters;
  TClonesArray* listStsHits;
  TClonesArray* listStsHitMatch;
  TClonesArray* listStsClusterMatch;

  // Pointers to data arrays
  //   CbmEvBasedArray* fMCTracks;
  // STS
  //   CbmEvBasedArray* fStsPts; // CbmStsPoint array
  // MVD
  //    CbmEvBasedArray* fMvdPts; //CbmMvdPoint array

  TClonesArray* listMvdPts;  // Mvd MC points
  TClonesArray* listMvdHits;
  TClonesArray* listMvdDigiMatches;
  TClonesArray* listMvdHitMatches;

  //MuCh
  int nMuchPoints;
  CbmMCDataArray* fMuchPoints;
  TClonesArray* listMuchHitMatches;  // Output CbmMatch array
  TClonesArray* fDigiMatchesMuch;
  TClonesArray* fClustersMuch;

  TClonesArray* fMuchPixelHits;  // CbmMuchPixelHit array
  TClonesArray* fDigisMuch;

  //TRD

  CbmTrdParSetDigi* fTrdDigiPar;     //!
  CbmTrdParModDigi* fTrdModuleInfo;  //!

  CbmMCDataArray* fTrdPoints;
  TClonesArray* listTrdHits;
  TClonesArray* fTrdHitMatches;

  //ToF
  CbmMCDataArray* fTofPoints;
  TClonesArray* fTofHitDigiMatches;  // CbmMatches array
  TClonesArray* fTofHits;            // CbmMatches array
  CbmTofDigiPar* fDigiPar;
  CbmTofDigiBdfPar* fTofDigiBdfPar;


  struct TH1FParameters {
    TString name, title;
    int nbins;
    float xMin, xMax;
  };

  TFile* fPerfFile;
  TDirectory* fHistoDir;

  static const int fNTimeHistos = 22;
  TH1F* fTimeHisto[fNTimeHistos];

  static const int fNGhostHistos = 9;
  TH1F* fGhostHisto[fNGhostHistos];

  //CbmMCEventHeader* fEvent;
  /// Used data = Repacked input data
  vector<CbmL1StsHit> vStsHits;  // hits with hit-mcpoint match information
  //   vector<CbmL1MCPoint> vMCPoints;
  vector<int> SortedIndex;
  vector<int> StsIndex;
  vector<CbmL1MCTrack> vMCTracks;
  vector<int>
    vHitMCRef;  // indices of MCPoints in vMCPoints, indexed by index of hit in algo->vStsHits array. According to StsMatch. Used for IdealResponce
                //    vector<int>          vHitMCRef1;
                //   CbmMatch stsHitMatch;
  //std::map<Double_t, Int_t> FEI2vMCPoints;
  typedef std::map<Double_t, Int_t> DFEI2I;
  DFEI2I dFEI2vMCPoints;
  DFEI2I dFEI2vMCTracks;
  inline Double_t dFEI(int file, int event, int idx) { return (1000 * idx) + file + (0.0001 * event); }
  // DFEI2I::iterator map_it;
  L1AlgoInputData* fData;

  static CbmL1* fInstance;

private:
  int fFindParticlesMode;

  TString fStsMatBudgetFileName;
  TString fMvdMatBudgetFileName;
  TString fMuchMatBudgetFileName;
  TString fTrdMatBudgetFileName;
  TString fTofMatBudgetFileName;
  bool fExtrapolateToTheEndOfSTS;
  int fTimesliceMode;

  KFTopoPerformance* fTopoPerformance;
  L1EventEfficiencies fEventEfficiency;  // average efficiencies

  CbmStsParSetSensor* fStsParSetSensor {nullptr};
  CbmStsParSetSensorCond* fStsParSetSensorCond {nullptr};
  CbmStsParSetModule* fStsParSetModule {nullptr};

  ClassDef(CbmL1, 1);
};

#endif  //_CbmL1_h_
