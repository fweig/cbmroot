/* Copyright (C) 2008-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Anna Kotynia [committer] */

/**
 ** \file CbmStsCluster.h
 ** \author V.Friese <v.friese@gsi.de>
 ** \since 28.08.06
 ** \brief Data class for STS clusters.
 **
 ** Updated 25/06/2008 by R. Karabowicz.
 ** Updated 04/03/2014 by A. Lebedev <andrey.lebedev@gsi.de>
 ** Updated 10/06/2014 by V.Friese <v.friese@gsi.de>
 **/

#ifndef CBMSTSCLUSTER_H
#define CBMSTSCLUSTER_H 1

#include "CbmCluster.h"  // for CbmCluster

#include <Rtypes.h>      // for ClassDef

#include <cstdint>
#include <string>  // for string

/**
 ** \class CbmStsCluster
 ** \brief Data class for STS clusters.
 **
 ** The CbmStsCluster is a collection of CbmStsDigis in neighbouring
 ** module channels. Apart from the indices of the contributing digis,
 ** it provides address, time, total charge, mean position in channel units
 ** and the error of the latter.
 **/
class CbmStsCluster : public CbmCluster {

public:
  /**
     * \brief Default constructor
     **/
  CbmStsCluster();


  /** @brief Copy constructor **/
  CbmStsCluster(const CbmStsCluster&) = default;


  /**
     * \brief Destructor
     **/
  virtual ~CbmStsCluster();


  /** @brief Get cluster charge
     ** @value  Total cluster charge [e]
     **
     ** This is the sum of the charges of the contributing digis.
     **/
  double GetCharge() const { return fCharge; }


  /** @brief Get cluster index
     ** @return Index of cluster in cluster array
     **/
  int32_t GetIndex() const { return fIndex; }


  /** @brief Cluster position
     ** @value Cluster position in channel number units
     **/
  double GetPosition() const { return fPosition; }


  /** @brief Cluster position error
     ** @value Error (r.m.s.) of cluster position in channel number units
     **/
  double GetPositionError() const { return fPositionError; }


  /** @brief Set size of the cluster (number of channels)
     ** @value size  Number of channels from first to last
     **
     ** Note that this can be different from the number of digis in the
     ** cluster in case there are gaps e.g. due to dead channels.
     **/
  int32_t GetSize() const { return fSize; }


  /** @brief Get cluster time
     ** @return Time of cluster [ns]
     **
     ** This is the average time of the contributing digis.
     **/
  double GetTime() const { return fTime; }


  /** @brief Get error of cluster time
     ** @return Time error [ns]
     **/
  double GetTimeError() const { return fTimeError; }


  /** @brief Set cluster index
     ** To keep track of the input during hit finding
     ** @param index  Index of cluster in cluster array
     **/
  void SetIndex(int32_t index) { fIndex = index; }


  /** @brief Set the position error
     ** @param error  Position error (r.m.s.) in channel units
     **/
  void SetPositionError(double error) { fPositionError = error; }


  /** Set cluster properties (time, charge, mean)
     ** @param charge         Total charge in cluster
     ** @param position       Cluster centre in channel units
     ** @param positionError  Error of cluster centre in channel units
     ** @param time           Cluster time [ns]
     ** @param timeError      Error of cluster time [ns]
     **/
  void SetProperties(double charge, double position, double positionError, double time = 0., double timeError = 0.)
  {
    fCharge        = charge;
    fPosition      = position;
    fPositionError = positionError;
    fTime          = time;
    fTimeError     = timeError;
  }


  /** @brief Set size of the cluster (number of channels)
     ** @param size  Number of channels from first to last
     **
     ** Note that this can be different from the number of digis in the
     ** cluster in case there are gaps e.g. due to dead channels.
     **/
  void SetSize(int32_t size) { fSize = size; }


  /** String output **/
  virtual std::string ToString() const;


private:
  double fCharge;         ///< Total charge
  int32_t fSize;          ///< Difference between first and last channel
  double fPosition;       ///< Cluster centre in channel number units
  double fPositionError;  ///< Cluster centre error (r.m.s.) in channel number units
  double fTime;           ///< Cluster time (average of digi times) [ns]
  double fTimeError;      ///< Error of cluster time [ns]
  int32_t fIndex;         ///< Index of cluster in input array


  ClassDef(CbmStsCluster, 7);
};

#endif
