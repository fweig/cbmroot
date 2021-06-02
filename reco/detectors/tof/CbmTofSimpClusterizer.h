/** @file CbmTofSimpClusterizer.h
  * @copyright Copyright (C) 2013-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer], Norbert Herrmann **/

/** @file CbmTofSimpClusterizer.h
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 23.08.2013
 **/

/** @class CbmTofSimpClusterizer
 ** @brief Simple Cluster building and hit producing for CBM ToF using Digis as input
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @version 1.0
 **/
#ifndef CBMTOFSIMPCLUSTERIZER_H
#define CBMTOFSIMPCLUSTERIZER_H 1

// TOF Classes and includes
// Input/Output
//class CbmTofPoint;
class CbmTofDigi;
class CbmMatch;
// Geometry
class CbmTofGeoHandler;
class CbmTofDetectorId;
class CbmTofDigiPar;
class CbmTofDigiBdfPar;
class CbmTofCell;
class CbmDigiManager;
class CbmEvent;

// FAIR classes and includes
#include "FairTask.h"

// ROOT Classes and includes
class TClonesArray;
class TH1;
class TH2;
class TString;
#include "TStopwatch.h"
#include "TTimeStamp.h"

// C++ Classes and includes
#include <vector>

class CbmTofSimpClusterizer : public FairTask {
public:
  /**
       ** @brief Constructor.
       **/
  CbmTofSimpClusterizer();

  /**
       ** @brief Constructor.
       **/
  CbmTofSimpClusterizer(const char* name, Int_t verbose = 1);
  /**
       ** @brief Destructor.
       **/
  virtual ~CbmTofSimpClusterizer();

  /**
       ** @brief Inherited from FairTask.
       **/
  virtual InitStatus Init();

  /**
       ** @brief Inherited from FairTask.
       **/
  virtual void SetParContainers();

  /**
       ** @brief Inherited from FairTask.
       **/
  virtual void Exec(Option_t* option);

  /**
       ** @brief Inherited from FairTask.
       **/
  virtual void Finish();

  inline void SetCalMode(Int_t iMode) { fCalMode = iMode; }
  inline void SetCalTrg(Int_t iTrg) { fCalTrg = iTrg; }
  inline void SetCalSmType(Int_t iCalSmType) { fCalSmType = iCalSmType; }
  inline void SetCaldXdYMax(Double_t dCaldXdYMax) { fdCaldXdYMax = dCaldXdYMax; }
  inline void SetTRefId(Int_t Id) { fTRefMode = Id; }
  inline void SetTRefDifMax(Double_t TRefMax) { fTRefDifMax = TRefMax; }
  inline void SetdTRefMax(Double_t dTRefMax) { fdTRefMax = dTRefMax; }
  inline void PosYMaxScal(Double_t PosYmaxScal) { fPosYMaxScal = PosYmaxScal; }
  inline void SetTotMax(Double_t TOTMax) { fTotMax = TOTMax; }
  inline void SetTotMin(Double_t TOTMin) { fTotMin = TOTMin; }
  inline void SetOutTimeFactor(Double_t val) { fOutTimeFactor = val; }

  inline void SetCalParFileName(TString CalParFileName) { fCalParFileName = CalParFileName; }
  Bool_t SetHistoFileName(TString sFilenameIn = "./tofSimpClust.hst.root");

  void UseMcTrackMonitoring(Bool_t bMcTrkMonitor = kTRUE) { fbMcTrkMonitor = bMcTrkMonitor; }

protected:
private:
  /**
       ** @brief Copy constructor.
       **/
  CbmTofSimpClusterizer(const CbmTofSimpClusterizer&);
  /**
       ** @brief Copy operator.
       **/
  CbmTofSimpClusterizer& operator=(const CbmTofSimpClusterizer&);

  // Functions common for all clusters approximations
  /**
       ** @brief Recover pointer on input TClonesArray: TofPoints, TofDigis...
       **/
  Bool_t RegisterInputs();
  /**
       ** @brief Create and register output TClonesArray of Tof Hits.
       **/
  Bool_t RegisterOutputs();
  /**
       ** @brief Initialize other parameters not included in parameter classes.
       **/
  Bool_t InitParameters();
  /**
       ** @brief Initialize other parameters not included in parameter classes.
       **/
  Bool_t InitCalibParameter();
  /**
       ** @brief Load the geometry: for now just resizing the Digis temporary vectors
       **/
  Bool_t LoadGeometry();
  /**
       ** @brief Delete the geometry related arrays: for now just clearing the Digis temporary vectors
       **/
  Bool_t DeleteGeometry();

  // Histogramming functions
  Bool_t CreateHistos();
  Bool_t FillHistos();
  Bool_t WriteHistos();
  Bool_t DeleteHistos();

  /**
       ** @brief Build clusters out of ToF Digis and store the resulting info in a TofHit.
       **/
  std::pair<Int_t, Int_t> BuildClusters(CbmEvent* event);

  /**
       ** @brief Retrieve event info from run manager to properly fill the CbmLink objects.
       **/
  void GetEventInfo(Int_t& inputNr, Int_t& eventNr, Double_t& eventTime);

  // ToF geometry variables
  CbmTofGeoHandler* fGeoHandler;
  CbmTofDetectorId* fTofId;
  CbmTofDigiPar* fDigiPar;
  CbmTofCell* fChannelInfo;
  CbmTofDigiBdfPar* fDigiBdfPar;
  Double_t fdParFeeTimeRes;
  Double_t fdParSystTimeRes;

  // Input variables
  TClonesArray* fTofPointsColl;  // TOF MC points
  TClonesArray* fMcTracksColl;   // MC tracks
  CbmDigiManager* fDigiMan;
  TClonesArray* fEvents = nullptr;

  // Output variables
  TClonesArray* fTofHitsColl;       // TOF hits
  TClonesArray* fTofDigiMatchColl;  // TOF Digis
  Int_t fiNbHits;                   // Index of the CbmTofHit TClonesArray

  // Generic
  Int_t fVerbose;

  // Intermediate storage variables
  std::vector<std::vector<std::vector<std::vector<CbmTofDigi*>>>> fStorDigiExp;  //[nbType][nbSm*nbRpc][nbCh][nDigis]
  std::vector<std::vector<std::vector<std::vector<Int_t>>>> fStorDigiInd;        //[nbType][nbSm*nbRpc][nbCh][nDigis]
  /*
      std::vector< std::vector< std::vector< std::vector< std::vector< CbmTofDigi* > > > > >
               fStorDigi; //[nbType][nbSm][nbRpc][nbCh][nDigis]
      std::vector< std::vector< std::vector< std::vector< std::vector< CbmTofDigiExp* > > > > >
               fStorDigiExp; //[nbType][nbSm][nbRpc][nbCh][nDigis]
      */
  std::vector<std::vector<std::vector<Int_t>>> fviClusterMul;   //[nbType][nbSm][nbRpc]
  std::vector<std::vector<std::vector<Int_t>>> fviClusterSize;  //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Int_t>>> fviTrkMul;       //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdX;         //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdY;         //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdDifX;      //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdDifY;      //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdDifCh;     //[nbType][nbRpc][nClusters]

  // Output file name and path
  TString fsHistoOutFilename;
  // Histograms
  TH1* fhClustBuildTime;
  TH1* fhHitsPerTracks;
  TH1* fhPtsPerHit;
  TH1* fhTimeResSingHits;
  TH2* fhTimeResSingHitsB;
  TH2* fhTimePtVsHits;
  TH1* fhClusterSize;
  TH2* fhClusterSizeType;
  TH1* fhTrackMul;
  TH2* fhClusterSizeMulti;
  TH2* fhTrk1MulPos;
  TH2* fhHiTrkMulPos;
  TH2* fhAllTrkMulPos;
  TH2* fhMultiTrkProbPos;
  TH1* fhDigSpacDifClust;
  TH1* fhDigTimeDifClust;
  TH2* fhDigDistClust;
  TH2* fhClustSizeDifX;
  TH2* fhClustSizeDifY;
  TH2* fhChDifDifX;
  TH2* fhChDifDifY;

  std::vector<TH2*> fhRpcDigiCor;                            //[nbDet]
  std::vector<TH1*> fhRpcCluMul;                             //[nbDet]
  std::vector<TH1*> fhRpcSigPropSpeed;                       //[nbDet]
  std::vector<TH2*> fhRpcCluPosition;                        //[nbDet]
  std::vector<TH2*> fhRpcCluTOff;                            //[nbDet]
  std::vector<TH2*> fhRpcCluTrms;                            //[nbDet]
  std::vector<TH2*> fhRpcCluTot;                             // [nbDet]
  std::vector<TH2*> fhRpcCluSize;                            // [nbDet]
  std::vector<TH2*> fhRpcCluAvWalk;                          // [nbDet]
  std::vector<std::vector<std::vector<TH2*>>> fhRpcCluWalk;  // [nbDet][nbCh][nSide]

  std::vector<std::vector<TH1*>> fhTRpcCluMul;                             // [nbDet][nbTrg]
  std::vector<std::vector<TH2*>> fhTRpcCluPosition;                        // [nbDet][nbTrg]
  std::vector<std::vector<TH2*>> fhTRpcCluTOff;                            // [nbDet][nbTrg]
  std::vector<std::vector<TH2*>> fhTRpcCluTot;                             // [nbDet][nbTrg]
  std::vector<std::vector<TH2*>> fhTRpcCluSize;                            // [nbDet][nbTrg]
  std::vector<std::vector<TH2*>> fhTRpcCluAvWalk;                          // [nbDet][nbTrg]
  std::vector<std::vector<TH2*>> fhTRpcCluDelTof;                          // [nbDet][nbTrg]
  std::vector<std::vector<TH2*>> fhTRpcCludXdY;                            // [nbDet][nbTrg]
  std::vector<std::vector<std::vector<std::vector<TH2*>>>> fhTRpcCluWalk;  // [nbDet][nbTrg][nbCh][nSide]

  std::vector<TH1*> fhTrgdT;  //[nbTrg]

  std::vector<std::vector<Double_t>> fvCPSigPropSpeed;                       //[nSMT][nRpc]
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPDelTof;   //[nSMT][nRpc][nbClDelTofBinX][nbTrg]
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPTOff;     //[nSMT][nRpc][nCh][nbSide]
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPTotGain;  //[nSMT][nRpc][nCh][nbSide]
  std::vector<std::vector<std::vector<std::vector<std::vector<Double_t>>>>>
    fvCPWalk;  //[nSMT][nRpc][nCh][nbSide][nbWalkBins]

  // Digis quality
  Int_t fiNbSameSide;
  TH1* fhNbSameSide;
  TH1* fhNbDigiPerChan;

  // Control
  TTimeStamp fStart;
  TTimeStamp fStop;

  // --- Run counters
  TStopwatch fTimer;           ///< ROOT timer
  Int_t fiNofTs = 0;           ///< Number of processed timeslices
  Int_t fiNofEvents;           ///< Total number of events processed
  Double_t fNofDigisAll  = 0;  ///< Total number of TOF digis in input
  Double_t fNofDigisUsed = 0;  ///< Total number of Tof Digis processed
  Double_t fdNofHitsTot;       ///< Total number of hits produced
  Double_t fdTimeTot;          ///< Total execution time

  // Calib
  Double_t dTRef;
  Double_t fdTRefMax;
  Int_t fCalMode;
  Int_t fCalTrg;
  Int_t fCalSmType;
  Double_t fdCaldXdYMax;
  Int_t fTRefMode;
  Int_t fTRefHits;
  Double_t fPosYMaxScal;
  Double_t fTRefDifMax;
  Double_t fTotMax;
  Double_t fTotMin;
  Double_t fOutTimeFactor;

  TString fCalParFileName;  // name of the file name with Calibration Parameters
  TFile* fCalParFile;       // pointer to Calibration Parameter file

  Bool_t fbMcTrkMonitor;

  ClassDef(CbmTofSimpClusterizer, 2);
};

#endif  // CBMTOFSIMPCLUSTERIZER_H
