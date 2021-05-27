/** @file CbmStsAlgoFindHits.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.03.2020
 **/

#ifndef CBMSTSALGOFINDHITS_H
#define CBMSTSALGOFINDHITS_H 1


#include <Rtypes.h>

class TGeoHMatrix;
class CbmStsCluster;
class CbmStsHit;
class CbmStsParSensor;


/** @class CbmStsAlgoFindHits
 ** @brief Algorithm for hit finding in the sensors of the CBM-STS
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
class CbmStsAlgoFindHits {

public:
  /** @brief Constructor **/
  CbmStsAlgoFindHits();


  /** @brief Copy constructor (disabled) **/
  CbmStsAlgoFindHits(const CbmStsAlgoFindHits&) = delete;


  /** @brief Assignment operator (disabled) **/
  CbmStsAlgoFindHits& operator=(const CbmStsAlgoFindHits&) = delete;


  /** @brief Destructor  **/
  virtual ~CbmStsAlgoFindHits() {};


  /** @brief Execute algorithm
     ** @param clustersF  Input clusters front side
     ** @param clusterB   Input clusters back side
     ** @param hits  Output hits
     ** @param Hit address ( = module address)
     ** @param timeCutSig  Time cut in units of cluster time error
     ** @param timeCutAbs  Time cut in ns
     ** @param dY  Vertical size of the active sensor area [cm]
     ** @param nStrips  Number of strips (same for both sensor sides)
     ** @param pitch  Strip ptch [cm] (same for both sensor sides)
     ** @param stereoF  Strip stereo angle sensor front side [deg]
     ** @param stereoB  Strip stereo angle sensor back side [deg]
     ** @param lorentzF  Mean Lorentz shift front side (electrons) [cm]
     ** @param lorentzB  Mean Lorentz shift back side (holes) [cm]
     ** @param matrix   Transformation matrix from local to global C.S.
     ** @return Number of created hits
     **/
  Long64_t Exec(const std::vector<CbmStsCluster>& clustersF, const std::vector<CbmStsCluster>& clustersB,
                std::vector<CbmStsHit>& hits, UInt_t address, Double_t timeCutSig, Double_t timeCutAbs, Double_t dY,
                UInt_t nStrips, Double_t pitch, Double_t stereoF, Double_t stereoB, Double_t lorentzF,
                Double_t lorentzB, TGeoHMatrix* matrix);


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
  Int_t GetSide(Double_t channel) const { return (channel < Double_t(fNofStrips) ? 0 : 1); }


  /** @brief Get strip and side from module channel.
     ** @param[in] channel   Channel number in module
     ** @param[in] sensorId  Sensor index in module
     ** @value     Pair of strip number and side
     **
     ** Note: This must be the inverse of GetModuleChannel.
     **/
  std::pair<Int_t, Int_t> GetStrip(UInt_t channel) const;


  /** @brief Intersection point of two strips / cluster centres
     ** @param[in] xF  x coordinate on read-out edge, front side [cm]
     ** @param[in] exF  uncertainty on xF [cm]
     ** @param[in] xB  x coordinate on read-out edge, back side  [cm]
     ** @param[in] eBF  uncertainty on xB [cm]
     ** @param[out] x x coordinate of crossing [cm]
     ** @param[out] y y coordinate of crossing [cm]
     ** @param[out] varX  Variance in x [cm^2]
     ** @param[out] varY  Variance in y [cm^2]
     ** @param[out] varXY Covariance of x and y [cm^2]
     ** @return kTRUE if intersection is inside active area.
     **
     ** This function calculates the intersection point of two
     ** lines starting at xF and xB at the top edge with slopes
     ** corresponding to the respective stereo angle.
     **
     ** All coordinates are in the sensor frame with the origin in the
     ** bottom left corner of the active area.
     **/
  Bool_t Intersect(Double_t xF, Double_t exF, Double_t xB, Double_t exB, Double_t& x, Double_t& y, Double_t& varX,
                   Double_t& varY, Double_t& varXY);


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
  UInt_t fNofStrips    = 0;        ///< Number of strips
  Double_t fDx         = 0.;       ///< Active size in x [cm]
  Double_t fDy         = 0.;       ///< Active size in y [cm]
  Double_t fPitch      = 0.;       ///< Strip pitch [cm]
  Double_t fStereoF    = 0.;       ///< Stereo angle front side [deg]
  Double_t fStereoB    = 0.;       ///< Stereo angle back side [deg]
  Double_t fLorentzF   = 0.;       ///< Lorentz shift correction front side [cm]
  Double_t fLorentzB   = 0.;       ///< Lorentz shift correction back side [cm]
  TGeoHMatrix* fMatrix = nullptr;  //! ///< Transformation matrix to global C.S.
  Double_t fTanStereoF = 0.;       ///< Tangent of stereo angle front side
  Double_t fTanStereoB = 0.;       ///< Tangent of stereo angle back side
  Double_t fErrorFac   = 0.;       ///< For calculation of the hit error

  /** @brief Output vector of hits **/
  std::vector<CbmStsHit>* fHits = nullptr;  //!


  ClassDef(CbmStsAlgoFindHits, 1);
};


#endif
