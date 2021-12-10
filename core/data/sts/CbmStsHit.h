/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig, Denis Bertini [committer] */

/**
 ** \file CbmStsHit.h
 ** \author Volker Friese <v.friese@gsi.de>
 ** \since 30.08.06
 ** \brief Data class for a reconstructed hit in the STS
 **
 ** Updated 14/03/2014 by Andrey Lebedev <andrey.lebedev@gsi.de>.
 ** Updated 15/08/2015 by Volker Friese <v.friese@gsi.de>.
 **/

#ifndef CBMSTSSHIT_H
#define CBMSTSSHIT_H 1

#include "CbmPixelHit.h"  // for CbmPixelHit

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <TVector3.h>    // for TVector3

#include <cstdint>
#include <string>  // for string


/** @class CbmStsHit
 ** @brief  data class for a reconstructed 3-d hit in the STS
 **
 ** A hit in the STS is a position measurement constructed from two clusters
 ** on the front and back side of the sensors, respectively, which have
 ** a geometric intersection. In addition to the base class, it provides
 ** indices of the contributing clusters and the measurement time.
 **/
class CbmStsHit : public CbmPixelHit {

public:
  /** Default constructor **/
  CbmStsHit();


  /** Constructor with all parameters
     ** @param address  Unique detector address (see CbmStsAddress)
     ** @param pos      Hit coordinate vector [cm]
     ** @param dpos     Hit coordinate error vector [cm]
     ** @param dxy      x-y covariance [cm**2]
     ** @param frontClusterId  Index of front-side cluster
     ** @param backClusterId   Index of back-side cluster
     ** @param time            Hit time [ns]
     ** @param timeError       Hit time error [ns]
     ** @param du       Coordinate error across front-side strips [cm]
     ** @param dv       Coordinate error across back-side strips [cm]
     **/
  CbmStsHit(int32_t address, const TVector3& pos, const TVector3& dpos, double dxy, int32_t frontClusterId,
            int32_t backClusterId, double time = 0., double timeError = 0., double du = 0., double dv = 0.);


  /** Destructor **/
  virtual ~CbmStsHit();


  /** Index of cluster at the back side
     ** @value  Back-side cluster index
     **/
  int32_t GetBackClusterId() const { return fBackClusterId; }


  /** @brief Error of coordinate across front-side strips
     ** @value Coordinate error [cm]
     **
     ** Note that this error is defined only in the
     ** local coordinate system of the sensor.
     **/
  double GetDu() const { return fDu; }


  /** @brief Error of coordinate across front-side strips
     ** @value Coordinate error [cm]
     **
     ** Note that this error is defined only in the
     ** local coordinate system of the sensor.
     **/
  double GetDv() const { return fDv; }


  /** Index of cluster at the front side
     ** @value  Front-side cluster index
     **/
  int32_t GetFrontClusterId() const { return fFrontClusterId; }

  /** @brief Set the index of the frontside cluster 
     ** To keep track of the input during matching
     ** @param index  Index of cluster in cluster array
     **/
  void SetFrontClusterId(int32_t index) { fFrontClusterId = index; }

  /** @brief Set the index of the backside cluster
     ** To keep track of the input during matching
     ** @param index  Index of cluster in cluster array
     **/
  void SetBackClusterId(int32_t index) { fBackClusterId = index; }

  /** Info to string **/
  virtual std::string ToString() const;


private:
  int32_t fFrontClusterId;  ///< Cluster index front side
  int32_t fBackClusterId;   ///< Cluster index back side
  double fDu;               ///< Error of coordinate across front-side strips [cm]
  double fDv;               ///< Error of coordinate across back-side strips [cm]

  ClassDef(CbmStsHit, 7);
};

#endif
