/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsRecoModule.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 21.03.2020
 **/

#ifndef CBMSTSRECOMODULE_H
#define CBMSTSRECOMODULE_H 1


#include "CbmStsCluster.h"
#include "CbmStsHit.h"

#include <TNamed.h>

#include <mutex>

class TGeoHMatrix;
class CbmStsAlgoAnaCluster;
class CbmStsAlgoFindClusters;
class CbmStsAlgoFindHits;
class CbmStsAlgoFindHitsOrtho;
class CbmStsHit;
class CbmStsDigi;
class CbmStsModule;
class CbmStsParModule;
class CbmStsParSensor;
class CbmStsParSensorCond;


/** @class CbmStsRecoModule
 ** @brief Class for reconstruction in one STS module
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 05.04.2017
 ** @date 21.03.2020
 **
 ** The module represents one module/sensor in the STS setup.
 ** Reconstruction in the modules is independent from other
 ** modules and can thus pe performed in parallel. The
 ** reconstruction module performs cluster finding, cluster
 ** analysis and hit finding.
 **
 ** The implementation assumes the module to be connected to
 ** exactly one double-sided strip sensor. The digi channel number
 ** must equal to the strip number on the front side of the sensor,
 ** and to strip number + number of strips on the back side.
 **/
class CbmStsRecoModule : public TNamed {

public:
  struct Timings {
    double timeSortDigi    = 0;
    double timeCluster     = 0;
    double timeSortCluster = 0;
    double timeHits        = 0;
  };

  /** @brief Default constructor **/
  CbmStsRecoModule();


  /** @brief Standard constructor
     ** @param setupModule  Corresponding setup element
     ** @param parModule    Module parameters
     ** @param parSensor    Sensor parameters
     ** @param lorentzShiftF  Average Lorentz shift sensor front side [cm]
     ** @param lorentzShiftB  Average Lorentz shift sensor back side [cm]
     **
     ** The Lorentz shift will be used to correct the hit position in
     ** hit finding.
     **/
  CbmStsRecoModule(CbmStsModule* setupModule, const CbmStsParModule& parModule, const CbmStsParSensor& parSensor,
                   Double_t lorentzShiftF, Double_t lorentzShiftB);


  /** @brief Copy constructor (disabled) **/
  CbmStsRecoModule(const CbmStsRecoModule&) = delete;


  /** @brief Assignment operator (disabled) **/
  CbmStsRecoModule& operator=(const CbmStsRecoModule&) = delete;


  /** @brief Destructor **/
  virtual ~CbmStsRecoModule();


  /** @brief Add a digi to the processing queue
     ** @param digi Pointer to digi object
     ** @param digiIndex  Index of digi in input array
     **/
  void AddDigiToQueue(const CbmStsDigi* digi, Int_t digiIndex);


  /** @brief Output front-side clusters
     ** @return Vector of front-side clusters
     **/
  const std::vector<CbmStsCluster>& GetClustersF() const { return fClustersF; }


  /** @brief Output back-side clusters
     ** @return Vector of back-side clusters
     **/
  const std::vector<CbmStsCluster>& GetClustersB() const { return fClustersB; }


  /** @brief Output hits
     ** @return Vector of hits
     **/
  const std::vector<CbmStsHit>& GetHits() const { return fHits; }

  /** @brief Time measurements
   **/
  Timings GetTimings() const { return fTimings; }


  /** @brief Perform reconstruction **/
  void Reconstruct();

  void SortDigis();

  void FindClusters();

  void SortClusters();

  void FindHits();


  /** @brief Clear input queue **/
  void Reset();


  TGeoHMatrix* getMatrix() { return fMatrix; }


  /** @brief Info to string **/
  std::string ToString() const;

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


private:
  /** @brief Set and check the needed parameters **/
  void Init();


private:
  // --- Algorithms
  CbmStsAlgoAnaCluster* fClusterAna        = nullptr;  //! ///< Algo
  CbmStsAlgoFindClusters* fClusterFinder   = nullptr;  //! ///< Algo
  CbmStsAlgoFindHits* fHitFinder           = nullptr;  //! ///< Algo
  CbmStsAlgoFindHitsOrtho* fHitFinderOrtho = nullptr;  //! ///< Algo
  std::mutex fLock {};

  // --- Parameters
  CbmStsModule* fSetupModule        = nullptr;  //!
  const CbmStsParModule* fParModule = nullptr;  //!
  const CbmStsParSensor* fParSensor = nullptr;  //!
  Double_t fDyActive                = 0.;       ///< Active sensor size in y
  UInt_t fNofStripsF                = 0;        ///< Number of sensor strips front side
  UInt_t fNofStripsB                = 0;        ///< Number of sensor strips back side
  Double_t fStripPitchF             = 0.;       ///< Sensor strip pitch front side [cm]
  Double_t fStripPitchB             = 0.;       ///< Sensor strip pitch back side [cm]
  Double_t fStereoFront             = 0.;       ///< Strip stereo angle front side [deg]
  Double_t fStereoBack              = 0.;       ///< Strip stereo angle back side [deg]
  TGeoHMatrix* fMatrix              = nullptr;  ///< Sensor position in global C.S. [cm]
  Double_t fLorentzShiftF           = 0.;       ///< Average Lorentz shift front side [cm|
  Double_t fLorentzShiftB           = 0.;       ///< Average Lorentz shift back side [cm|

  // --- Data
  std::vector<std::pair<const CbmStsDigi*, Long64_t>> fDigisF {};  //!
  std::vector<std::pair<const CbmStsDigi*, Long64_t>> fDigisB {};  //!
  std::vector<CbmStsCluster> fClustersF {};                        //!
  std::vector<CbmStsCluster> fClustersB {};                        //!
  std::vector<CbmStsHit> fHits {};                                 //!

  // --- Settings
  Double_t fTimeCutDigisSig    = 3.;      ///< Time cut for cluster finding (in sigma)
  Double_t fTimeCutDigisAbs    = -1.;     ///< Time cut for cluster finding (in ns)
  Double_t fTimeCutClustersSig = 4.;      ///< Time cut for hit finding (in ns)
  Double_t fTimeCutClustersAbs = -1.;     ///< Time cut for hit finding (in sigma)
  Bool_t fConnectEdgeFront    = kFALSE;  ///< Round-the edge clustering front side
  Bool_t fConnectEdgeBack     = kFALSE;  ///< Round-the edge clustering back side

  // --- Time measurement
  Timings fTimings;


  ClassDef(CbmStsRecoModule, 1);
};

#endif /* CBMSTSRECOMODULE_H */
