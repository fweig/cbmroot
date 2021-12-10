/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                    CbmPsdPoint header file                   -----
// -----                 Created 28/07/04  by V. Friese                -----
// -------------------------------------------------------------------------

/**  CbmPsdPoint.h
 *@author V. Friese
 **
 ** Interception of MC track with the plane representing the ECAL.
 **/


#ifndef CBMPSDPOINT_H
#define CBMPSDPOINT_H 1

#include <FairMCPoint.h>  // for FairMCPoint

#include <Rtypes.h>      // for ClassDef
#include <TVector3.h>    // for TVector3

#include <cstdint>
#include <string>  // for string

class CbmPsdPoint : public FairMCPoint {

public:
  /** Default constructor **/
  CbmPsdPoint();


  /** Constructor with arguments
   *@param trackID  Index of MCTrack
   *@param detID    Detector ID
   *@param pos      Ccoordinates at entrance to active volume [cm]
   *@param mom      Momentum of track at entrance [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [GeV]
   **/
  CbmPsdPoint(int32_t trackID, int32_t detID, TVector3 pos, TVector3 mom, double tof, double length, double eLoss);


  /** Copy constructor **/
  //  CbmPsdPoint(const CbmPsdPoint& point) { *this = point; };


  /** Destructor **/
  virtual ~CbmPsdPoint();


  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

  /** Modifiers **/
  void SetModuleID(int32_t mod) { fModuleID = mod; }
  /** Accessors **/
  int32_t GetModuleID() const { return fModuleID; }

  std::string ToString() const;

private:
  int32_t fModuleID;  //number of module


  ClassDef(CbmPsdPoint, 2)
};


#endif
