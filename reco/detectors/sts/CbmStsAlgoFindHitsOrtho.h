/** @file CbmStsAlgoFindHitsOrtho.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsAlgoFindHitsOrtho.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.03.2020
 **/

#ifndef CBMSTSALGOFINDHITSORTHO_H
#define CBMSTSALGOFINDHITSORTHO_H 1


#include <Rtypes.h>

#include <cassert>

class TGeoHMatrix;
class CbmStsCluster;
class CbmStsHit;
class CbmStsParSensor;


/** @class CbmStsAlgoFindHitsOrtho
 ** @brief Algorithm for hit finding in sensors with orthogonal strips
 ** @author V.Friese <v.friese@gsi.de>
 ** @author F. Boeck (FIAS)
 ** @since 23.03.2020
 **
 ** A hit is constructed from two clusters, one from the front side,
 ** the other one from the back side of the sensor, if they two clusters
 ** have a geometrical intersection in the active sensor area, and
 ** if they are compatible in time. The algorithm combines front-side
 ** with back-side clusters and checks for these conditions. The hit
 ** coordinates are transformed into the global coordinate system.
 **
 ** The loop over the clusters was optimised by F. Boeck for application
 ** to free-streaming data (not sorted into events).
 **/
class CbmStsAlgoFindHitsOrtho {

public:
  /** @brief Constructor **/
  CbmStsAlgoFindHitsOrtho();


  /** @brief Copy constructor (disabled) **/
  CbmStsAlgoFindHitsOrtho(const CbmStsAlgoFindHitsOrtho&) = delete;


  /** @brief Assignment operator (disabled) **/
  CbmStsAlgoFindHitsOrtho& operator=(const CbmStsAlgoFindHitsOrtho&) = delete;


  /** @brief Destructor  **/
  virtual ~CbmStsAlgoFindHitsOrtho() {};


  /** @brief Execute algorithm
     ** @param clustersF  Input clusters front side
     ** @param clusterB   Input clusters back side
     ** @param hits  Output hits
     ** @param Hit address ( = module address)
     ** @param timeCutSig  Time cut in units of cluster time error
     ** @param timeCutAbs  Time cut in ns
     ** @param dY  Vertical size of the active sensor area [cm]
     ** @param nStripsF  Number of strips on front side
     ** @param nStripsB  Number of strips on back side
     ** @param pitchF  Strip pitch on front side [cm]
     ** @param pitchB  Strip pitch on back side [cm]
     ** @param lorentzF  Mean Lorentz shift front side (electrons) [cm]
     ** @param lorentzB  Mean Lorentz shift back side (holes) [cm]
     ** @param matrix   Transformation matrix from local to global C.S.
     ** @return Number of created hits
     **/
  Long64_t Exec(const std::vector<CbmStsCluster>& clustersF, const std::vector<CbmStsCluster>& clustersB,
                std::vector<CbmStsHit>& hits, UInt_t address, Double_t timeCutSig, Double_t timeCutAbs, UInt_t nStripsF,
                UInt_t nStripsB, Double_t pitchF, Double_t pitchB, Double_t lorentzF, Double_t lorentzB,
                TGeoHMatrix* matrix);


private:
  /** @brief Create a new hit in the output array
     ** @param xLocal   hit x coordinate in sensor system [cm]
     ** @param yLocal   hit y coordinate in sensor system [cm]
     ** @param varX     Variance in x [cm^2]
     ** @param varY     Variance in y [cm^2]
     ** @param varXY    Covariance of x and y [cm^2]
     ** @param clusterF Front-side cluster
     ** @param clusterB Back-side cluster
     ** @param indexF   Index of front-side cluster
     ** @param indexB   Index of back-side cluster
     ** @param du       Error in u coordinate (across strips front side) [cm]
     ** @param dv       Error in v coordinate (across strips back side) [cm]
     **/
  void CreateHit(Double_t xLocal, Double_t yLocal, Double_t varX, Double_t varY, Double_t varXY,
                 const CbmStsCluster& clusterF, const CbmStsCluster& clusterB, UInt_t indexF, UInt_t indexB,
                 Double_t du = 0., Double_t dv = 0.);


  /** Get the cluster position at the top edge of the sensor.
     ** @param[in]  centre    Cluster centre in (module) channel units
     ** @param[out] xCluster  Cluster position at readout edge
     ** @param[out] side      Sensor side [0 = front, 1 = back]
     **
     ** A correction for the Lorentz shift is applied.
     **/
  void GetClusterPosition(Double_t ClusterCentre, Double_t& xCluster, Int_t& side);


  /** Get the side of the sensor from the module channel number
     ** The channel number can also be the cluster position, so it needs
     ** not be integer.
     ** @param channel  Channel number
     ** @return Sensor side ( 0 = front, 1 = back)
     **/
  Int_t GetSide(Double_t channel) const
  {
    assert(channel < fNofStripsF + fNofStripsB);
    return (channel < Double_t(fNofStripsF) ? 0 : 1);
  }


  /** @brief Get strip and side from module channel.
     ** @param[in] channel   Channel number in module
     ** @param[in] sensorId  Sensor index in module
     ** @value     Pair of strip number and side
     **
     ** Note: This must be the inverse of GetModuleChannel.
     **/
  std::pair<Int_t, Int_t> GetStrip(UInt_t channel) const;


  /** @brief Find the intersection points of two clusters.
     ** @param clusterF    Cluster on front side
     ** @param clusterB    Cluster on back side
     ** @param indexF      Index of cluster on front side
     ** @param indexB      Index of cluster on back side
     ** @return Number of intersection points inside active area
     **
     ** For each intersection point, a hit is created.
     **/
  Int_t IntersectClusters(const CbmStsCluster& clusterF, const CbmStsCluster& clusterB, UInt_t indexF, UInt_t indexB);


  /** @brief Check whether a point (x,y) is inside the active area.
     **
     ** @param x  x coordinate in the local c.s. [cm]
     ** @param y  y coordinate in the local c.s. [cm]
     ** @return  kTRUE if inside active area.
     **
     ** The coordinates have to be given in the local
     ** coordinate system (origin in the sensor centre).
     **/
  Bool_t IsInside(Double_t x, Double_t y);


private:
  UInt_t fAddress      = 0;        ///< Unique address for hits (sensor)
  Double_t fTimeCutSig = 0.;       ///< Time cut on clusters in error units
  Double_t fTimeCutAbs = 0.;       ///< Time cut on clusters in ns
  UInt_t fNofStripsF   = 0;        ///< Number of strips front side
  UInt_t fNofStripsB   = 0;        ///< Number of strips backs side
  Double_t fDx         = 0.;       ///< Active size in x [cm]
  Double_t fDy         = 0.;       ///< Active size in y [cm]
  Double_t fPitchF     = 0.;       ///< Strip pitch front side [cm]
  Double_t fPitchB     = 0.;       ///< Strip pitch back side [cm]
  Double_t fLorentzF   = 0.;       ///< Lorentz shift correction front side [cm]
  Double_t fLorentzB   = 0.;       ///< Lorentz shift correction back side [cm]
  TGeoHMatrix* fMatrix = nullptr;  //! ///< Transformation matrix to global C.S.

  /** @brief Output vector of hits **/
  std::vector<CbmStsHit>* fHits = nullptr;  //!


  ClassDef(CbmStsAlgoFindHitsOrtho, 1);
};


#endif
