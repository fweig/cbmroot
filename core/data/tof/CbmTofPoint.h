/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Denis Bertini [committer] */

/** @file CbmTofPoint.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @author Christian Simon <c.simon@physi.uni-heidelberg.de>
 ** @since 16.06.2014
 ** @date 11.04.2017
 **/


#ifndef CBMTOFPOINT_H
#define CBMTOFPOINT_H 1

#include <FairMCPoint.h>  // for FairMCPoint

#include <Rtypes.h>      // for ClassDef

#include <cstdint>
#include <string>  // for string

class TVector3;


/** @class CbmTofPoint
 ** @brief Geometric intersection of a MC track with a TOFb detector
 ** @author Volker Friese <v.friese@gsi.de>
 ** @author Christian Simon <c.simon@physi.uni-heidelberg.de>
 ** @since 16.06.2014
 ** @date 11.04.2017
 ** @version 3.0
 **
 ** This data class holds the geometric properties of the intersection
 ** of a Monte-Carlo track with an active TOF detector element.
 ** The TOF detector class produces CbmTofPoints in two modes. In the
 ** first one, a TofPoint is created for each gap the track enters.
 ** In the second mode, only one TofPoint per track and module is created.
 ** The space and momentum coordinates then correspond to the respective
 ** averages over all traversed gaps. In this case, the variables fNofCells
 ** gives the number of transversal segments the track has touched, and
 ** fGapMask gives the pattern of traversed gaps.
 **/
class CbmTofPoint : public FairMCPoint {

public:
  /** @brief Default constructor **/
  CbmTofPoint();


  /** @brief Constructor with arguments
   ** @param trackID  Index of MCTrack
   ** @param detID    Detector ID
   ** @param pos      Coordinates at entrance to active volume [cm]
   ** @param mom      Momentum of track at entrance [GeV]
   ** @param tof      Time since event start [ns]
   ** @param length   Track length since creation [cm]
   ** @param eLoss    Energy deposit [GeV]
   **/
  CbmTofPoint(int32_t trackID, int32_t detID, TVector3 pos, TVector3 mom, double tof, double length, double eLoss);


  /** @brief Destructor **/
  virtual ~CbmTofPoint();


  /** @brief Number of traversed cells
   ** @value Number of traversed cells
   **/
  int32_t GetNCells() const { return fNofCells; }


  /** @brief Number of traversed gaps
   ** @value Number of traversed gaps
   **/
  int32_t GetNGaps() const;


  /** @brief Index of first traversed gap
   ** @value First gap index
   **/
  int32_t GetFirstGap() const;


  /** @brief Index of last traversed gap
   ** @value Last gap index
   **/
  int32_t GetLastGap() const;


  /** @brief Set x component of momentum
   ** @param pX  x component of momentum [GeV]
   **/
  void SetPx(double pX) { fPx = pX; }


  /** @brief Set y component of momentum
   ** @param pY  y component of momentum [GeV]
   **/
  void SetPy(double pY) { fPy = pY; }


  /** @brief Set z component of momentum
   ** @param pZ  z component of momentum [GeV]
   **/
  void SetPz(double pZ) { fPz = pZ; }


  /** @brief Set the number of traversed cells
   ** @param nCells  Number of traversed cells
   **/
  void SetNCells(int32_t nCells) { fNofCells = nCells; }


  /** @brief Set a gap in the gap mask
   ** @param iGap  Gap number
   **/
  void SetGap(int32_t iGap);


  /** @brief String representation of the object.
   ** @value String representation of the object.
   **/
  virtual std::string ToString() const;


private:
  int32_t fNofCells;  /// Number of cells traversed
  uint16_t fGapMask;  /// Set of gaps traversed


  ClassDef(CbmTofPoint, 4)
};


#endif
