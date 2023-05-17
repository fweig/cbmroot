/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmRecoSts.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 22.03.2020
 **/


#ifndef CBMRECOSTS_H
#define CBMRECOSTS_H 1

#include <FairTask.h>

#include <TClonesArray.h>
#include <TStopwatch.h>

#include "StsHitfinderChain.h"

class CbmDigiManager;
class CbmEvent;
class CbmStsElement;
class CbmStsParAsic;
class CbmStsParModule;
class CbmStsParSensor;
class CbmStsParSensorCond;
class CbmStsParSetModule;
class CbmStsParSetSensor;
class CbmStsParSetSensorCond;
class CbmStsParSim;
class CbmStsRecoModule;
class CbmStsSensor;
class CbmStsSetup;


/** @enum ECbmMode
 ** @brief Time-slice or event-by-event mode
 **/
enum ECbmRecoMode
{
  kCbmRecoTimeslice,
  kCbmRecoEvent
};


/** @class CbmRecoSts
 ** @brief Task class for local reconstruction in the STS
 ** @author Volker Friese <v.friese@gsi.de>
 ** @author Florian Boeck (FIAS)
 ** @since 16.06.2014
 ** @date 22.03.2019
 **
 ** Local reconstruction in the STS comprises cluster finding,
 ** cluster analysis, and hit finding. All these tasks are
 ** performed separately on each module, which can be parallelised.
 **
 ** In the mode kCbmRecoTimeslice, the complete input array of digis
 ** is processed. In the mode kCbmRecoEvent, only digis belonging to
 ** the respective event are processed.
 **
 ** Parallelisation using OpenMP was introduced by F. Boek from FIAS.
 **/
class CbmRecoSts : public FairTask {

public:
  /** @brief Constructor **/
  CbmRecoSts(ECbmRecoMode mode = kCbmRecoTimeslice, Bool_t writeClusters = kFALSE);


  /** @brief Copy constructor (disabled) **/
  CbmRecoSts(const CbmRecoSts&) = delete;


  /** @brief Assignment operator (disabled) **/
  CbmRecoSts operator=(const CbmRecoSts&) = delete;


  /** @brief Destructor  **/
  virtual ~CbmRecoSts();


  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);


  /** @brief End-of-run action **/
  virtual void Finish();


  /** @brief Access to output array of clusters **/
  TClonesArray* GetClusters() { return fClusters; }


  /** @brief Initialisation **/
  virtual InitStatus Init();


  /** @brief Set event-by-event mode
     ** @param choice  If true, event-by-event mode is used
     **
     ** In the event-by-event mode, the event objects in the input tree
     ** are used, and events are processed one after the other.
     ** An event builder has to be run before, creating the event objects.
     ** By default, time-slice mode is applied.
     **
     ** Alternative to using SetMode.
     **/
  void SetEventMode(Bool_t choice = kTRUE) { fMode = (choice ? kCbmRecoEvent : kCbmRecoTimeslice); }


  /** @brief Set execution mode
     ** @param mode  Time-slice or event
     **
     ** In the time-slice mode, the entire time-slice (input arrays)
     ** will be processed. In the event mode, events read from the event
     ** branch are processed one after the other.
     **/
  void SetMode(ECbmRecoMode mode) { fMode = mode; }

  void SetUseGpuReco(bool useGPU);

  /** @brief Define the needed parameter containers **/
  virtual void SetParContainers();


  /** @brief Time cut on clusters for hit finding
     ** @param value  Maximal time difference between two clusters in a hit [ns]
     **
     ** Two clusters are considered compatible if their time difference
     ** is below value.
     ** Setting this cut parameter to a positive value will override
     ** the time cut defined by SetTimeCutClustersSig.
     **/
  void SetTimeCutClustersAbs(Double_t value) { fTimeCutClustersAbs = value; }


  /** @brief Time cut on clusters for hit finding
     ** @param value  Maximal time difference in units of error
     **
     ** Two clusters are considered compatible if their time difference
     ** is below value * sqrt(terr1**2 + terr2*+2).
     **/
  void SetTimeCutClustersSig(Double_t value) { fTimeCutClustersSig = value; }


  /** @brief Time cut on digis for cluster finding
     ** @param value  Maximal time difference between two digis in a cluster [ns]
     **
     ** Two digis are considered compatible if their time difference
     ** is below value.
     ** Setting this cut parameter to a positive value will override
     ** the time cut defined by SetTimeCutDigisSig.
     **/
  void SetTimeCutDigisAbs(Double_t value) { fTimeCutDigisAbs = value; }


  /** @brief Time cut on digis for hit finding
     ** @param value  Maximal time difference in units of error
     **
     ** Two digis are considered compatible if their time difference
     ** is below value * sqrt2 * sigma(t), where the time error of
     ** the digis is assumed to be the same.
     **/
  void SetTimeCutDigisSig(Double_t value) { fTimeCutDigisSig = value; }


  /** @brief User-defined module parameters
     ** @param parModule Module parameter object
     **
     ** If defined, these parameters will be used for all modules instead
     ** of those found in the runtimeDb.
     */
  void UseModulePar(CbmStsParModule* modulePar) { fUserParModule = modulePar; }


  /** @brief User-defined module parameter set
     ** @param parModule Module parameter set object
     **
     ** If defined, this parameter set will be used instead of that found in the runtimeDb.
     */
  void UseModuleParSet(CbmStsParSetModule* moduleParSet) { fUserParSetModule = moduleParSet; }


  /** @brief User-defined sensor condition parameters
      ** @param parModule Sensor condition parameter object
      **
      ** If defined, these condition parameters will be used for all sensors instead
      ** of those found in the runtimeDb.
      */
  void UseSensorCond(CbmStsParSensorCond* sensorCond) { fUserParCond = sensorCond; }

  /** @brief User-defined module parameter set
     ** @param parModule Module parameter set object
     **
     ** If defined, this parameter set will be used instead of that found in the runtimeDb.
     */
  void UseSensorCondSet(CbmStsParSetSensorCond* sensorCondSet) { fUserParSetCond = sensorCondSet; }


  /** @brief User-defined sensor parameters
     ** @param parModule Sensor parameter object
     **
     ** If defined, these parameters will be used for all sensors instead
     ** of those found in the runtimeDb.
     */
  void UseSensorPar(CbmStsParSensor* sensorPar) { fUserParSensor = sensorPar; }


  /** @brief User-defined module parameter set
     ** @param parModule Module parameter set object
     **
     ** If defined, this parameter set will be used instead of that found in the runtimeDb.
     */
  void UseSensorParSet(CbmStsParSetSensor* sensorParSet) { fUserParSetSensor = sensorParSet; }

  void DumpNewHits();
  void DumpOldHits();

private:
  /** @brief Average Lorentz Shift in a sensor
     ** @param conditions  Sensor operating conditions
     ** @param dZ  Sensor thickness [cm]
     ** @param bY  y component of magnetic field in sensor centre
     ** @return Mean Lorentz shift front side and back side [cm]
     **
     ** The Lorentz shift will be corrected for in hit finding.
     **/
  std::pair<Double_t, Double_t> LorentzShift(const CbmStsParSensorCond& conditions, Double_t dZ, Double_t bY);


  /** @brief Instantiate reconstruction modules
     ** @value Number of modules created
     **/
  UInt_t CreateModules();


  /** @brief Get the sensor parameters
     ** @param geoSensor Pointer to setup sensor
     **/
  void GetSensorParameters(CbmStsElement* geoSensor);


  /** @brief Initialise parameters
   **
   ** For simulated data, the parameters for modules and sensors are retrieved from the
   ** runtimeDb. They can be overridden by user-specified parameter sets using the respective
   ** setters. This is necessary when processing experiment data without a prior simulation step.
   **
   **/
  void InitParams();


  /** @brief Process one time slice or event
     ** @param event  Pointer to CbmEvent object
     **
     ** If a null event pointer is given, the entire input array is processed.
     **/
  void ProcessData(CbmEvent* event = nullptr);

  void ProcessDataGpu();

private:
  // --- I/O
  TClonesArray* fEvents        = nullptr;  //! Input array of events
  CbmDigiManager* fDigiManager = nullptr;  //! Interface to digi branch
  TClonesArray* fClusters      = nullptr;  //! Output cluster array
  TClonesArray* fHits          = nullptr;  //! Output hit array

  // --- Setup and parameters
  CbmStsSetup* fSetup                 = nullptr;  //! Instance of STS setup
  CbmStsParSim* fParSim               = nullptr;  ///< Simulation settings
  CbmStsParSetModule* fParSetModule   = nullptr;  ///< Module parameters
  CbmStsParSetSensor* fParSetSensor   = nullptr;  ///< Sensor parameters
  CbmStsParSetSensorCond* fParSetCond = nullptr;  ///< Sensor conditions

  // --- User-defined parameters, not from database
  CbmStsParAsic* fUserParAsic       = nullptr;
  CbmStsParModule* fUserParModule   = nullptr;
  CbmStsParSensor* fUserParSensor   = nullptr;
  CbmStsParSensorCond* fUserParCond = nullptr;

  // --- User-defined parameter sets, not from database
  CbmStsParSetModule* fUserParSetModule   = nullptr;
  CbmStsParSetSensor* fUserParSetSensor   = nullptr;
  CbmStsParSetSensorCond* fUserParSetCond = nullptr;

  // --- Settings
  ECbmRecoMode fMode           = kCbmRecoEvent;  ///< Time-slice or event
  Double_t fTimeCutDigisSig    = 3.;             ///< Time cut for cluster finding
  Double_t fTimeCutDigisAbs    = -1.;            ///< Time cut for cluster finding [ns]
  Double_t fTimeCutClustersSig = 4.;             ///< Time cut for hit finding
  Double_t fTimeCutClustersAbs = -1.;            ///< Time cut for hit finding [ns]
  Bool_t fWriteClusters        = kFALSE;         ///< Write clusters to tree

  // --- Timeslice counters
  Long64_t fNofDigis        = 0;   ///< Total number of digis processed
  Long64_t fNofDigisUsed    = 0;   ///< Total number of used digis
  Long64_t fNofDigisIgnored = 0;   ///< Total number of ignored digis
  Long64_t fNofClusters     = 0;   ///< Total number of clusters produced
  Long64_t fNofHits         = 0;   ///< Total number of clusters produced
  Double_t fTimeTot         = 0.;  ///< Total execution time
  Double_t fTime1           = 0.;  ///< Time for resetting modules
  Double_t fTime2           = 0.;  ///< Time for distributing data
  Double_t fTime3           = 0.;  ///< Time for reconstruction
  Double_t fTime4           = 0.;  ///< Time for output results
  double fTimeSortDigis     = 0.;
  double fTimeFindClusters  = 0.;
  double fTimeSortClusters  = 0.;
  double fTimeFindHits      = 0.;

  // --- Run counters
  TStopwatch fTimer {};               //! ROOT timer
  Int_t fNofTs                 = 0;   ///< Number of time slices processed
  Int_t fNofEvents             = 0;   ///< Number of events processed
  Double_t fNofDigisRun        = 0;   ///< Total number of digis processed
  Double_t fNofDigisUsedRun    = 0;   ///< Total number of used digis
  Double_t fNofDigisIgnoredRun = 0;   ///< Total number of ignored digis
  Double_t fNofClustersRun     = 0;   ///< Total number of clusters produced
  Double_t fNofHitsRun         = 0;   ///< Total number of clusters produced
  Double_t fTimeRun            = 0.;  ///< Total execution time
  Double_t fTime1Run           = 0.;  ///< Time for resetting modules
  Double_t fTime2Run           = 0.;  ///< Time for distributing data
  Double_t fTime3Run           = 0.;  ///< Time for reconstruction
  Double_t fTime4Run           = 0.;  ///< Time for output results


  // --- Reconstruction modules
  std::map<UInt_t, CbmStsRecoModule*> fModules {};  //!
  std::vector<CbmStsRecoModule*> fModuleIndex {};   //!

  bool fUseGpuReco = false;
  cbm::algo::sts::HitfinderChain fGpuReco;

  std::pair<size_t, size_t> ForwardGpuClusterAndHits();

  ClassDef(CbmRecoSts, 1);
};

#endif
