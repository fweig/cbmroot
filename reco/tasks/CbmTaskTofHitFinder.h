/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer], Pierre-Alain Loizeau, Norbert Herrmann */

/** @class CbmTaskTofHitFinder
 ** @brief Simple Cluster building and hit producing for CBM ToF using Digis as input
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @version 1.0
 **/
#ifndef CBMTOFHITFINDER_H
#define CBMTOFHITFINDER_H 1

// TOF Classes and includes
// Input/Output
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

#include "HitFinderTof.h"

// FAIR classes and includes
#include "FairTask.h"

// ROOT Classes and includes
class TClonesArray;
#include "TStopwatch.h"
#include "TTimeStamp.h"

// C++ Classes and includes
#include <vector>

class CbmTaskTofHitFinder : public FairTask {
public:
  /**
       ** @brief Constructor.
       **/
  CbmTaskTofHitFinder();

  /**
       ** @brief Constructor.
       **/
  CbmTaskTofHitFinder(const char* name, int32_t verbose = 1);
  /**
       ** @brief Destructor.
       **/
  virtual ~CbmTaskTofHitFinder();

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

protected:
private:
  /**
       ** @brief Copy constructor.
       **/
  CbmTaskTofHitFinder(const CbmTaskTofHitFinder&);
  /**
       ** @brief Copy operator.
       **/
  CbmTaskTofHitFinder& operator=(const CbmTaskTofHitFinder&);

  // Functions common for all clusters approximations
  /**
       ** @brief Recover pointer on input TClonesArray: TofPoints, TofDigis...
       **/
  bool RegisterInputs();
  /**
       ** @brief Create and register output TClonesArray of Tof Hits.
       **/
  bool RegisterOutputs();
  /**
       ** @brief Initialize other parameters not included in parameter classes.
       **/
  bool InitParameters();
  /**
       ** @brief Initialize other parameters not included in parameter classes.
       **/
  bool InitCalibParameter();

  /**
       ** @brief Create one algo object for each RPC
       **/
  bool InitAlgos();

  /**
       ** @brief Build clusters out of ToF Digis and store the resulting info in a TofHit.
       **/
  std::pair<int32_t, int32_t> BuildClusters(CbmEvent* event);

  /**
       ** @brief Retrieve event info from run manager to properly fill the CbmLink objects.
       **/
  void GetEventInfo(int32_t& inputNr, int32_t& eventNr, double& eventTime);

  // ToF geometry variables
  CbmTofGeoHandler* fGeoHandler;
  CbmTofDetectorId* fTofId      = nullptr;
  CbmTofDigiPar* fDigiPar       = nullptr;
  CbmTofDigiBdfPar* fDigiBdfPar = nullptr;

  // Input variables
  CbmDigiManager* fDigiMan = nullptr;
  TClonesArray* fEvents    = nullptr;

  // Output variables
  TClonesArray* fTofHitsColl      = nullptr;  // TOF hits
  TClonesArray* fTofDigiMatchColl = nullptr;  // TOF Digis

  // Hit finder algo
  std::map<uint32_t, std::map<uint32_t, cbm::algo::HitFinderTof>> fAlgo = {};  //[nbType][nbSm*nbRpc]

  // Intermediate storage variables
  std::vector<std::vector<std::vector<CbmTofDigi>>> fStorDigiExp;  //[nbType][nbSm*nbRpc][nbCh][nDigis]
  std::vector<std::vector<std::vector<int32_t>>> fStorDigiInd;     //[nbType][nbSm*nbRpc][nbCh][nDigis]

  //Calibration parameters
  std::vector<std::vector<double>> fvCPSigPropSpeed;                       //[nSMT][nRpc]
  std::vector<std::vector<std::vector<std::vector<double>>>> fvCPTOff;     //[nSMT][nRpc][nCh][nbSide]
  std::vector<std::vector<std::vector<std::vector<double>>>> fvCPTotGain;  //[nSMT][nRpc][nCh][nbSide]
  std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>>
    fvCPWalk;  //[nSMT][nRpc][nCh][nbSide][nbWalkBins]

  // Control
  TTimeStamp fStart;
  TTimeStamp fStop;

  // --- Run counters
  TStopwatch fTimer;          ///< ROOT timer
  int32_t fiNofTs      = 0;   ///< Number of processed timeslices
  int32_t fiNofEvents  = 0;   ///< Total number of events processed
  double fNofDigisAll  = 0.;  ///< Total number of TOF digis in input
  double fNofDigisUsed = 0.;  ///< Total number of Tof Digis processed
  double fdNofHitsTot  = 0.;  ///< Total number of hits produced
  double fdTimeTot     = 0.;  ///< Total execution time

  ClassDef(CbmTaskTofHitFinder, 1);
};

#endif  // CBMTOFHITFINDER_H
