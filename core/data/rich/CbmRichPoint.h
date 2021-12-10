/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Boris Polichtchouk, Denis Bertini [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                     CbmRichPoint header file                  -----
// -----               Created 28/04/04  by B. Polichtchouk            -----
// -------------------------------------------------------------------------


/**  CbmRichPoint.h
 *@author B. Polichtchouk
 **
 ** Interception of MC track with an RICH photodetector.
 **/


#ifndef CBMRICHPOINT_H
#define CBMRICHPOINT_H 1

#include <FairMCPoint.h>  // for FairMCPoint

#include <Rtypes.h>      // for ClassDef

#include <cstdint>

class TVector3;

class CbmRichPoint : public FairMCPoint {

public:
  /** Default constructor **/
  CbmRichPoint();


  /** Constructor with arguments
   *@param trackID  Index of MCTrack
   *@param detID    Detector ID
   *@param pos      Coordinates at entrance to active volume [cm]
   *@param mom      Momentum of track at entrance [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [GeV]
   **/
  CbmRichPoint(int32_t trackID, int32_t detID, TVector3 pos, TVector3 mom, double tof, double length, double eLoss);


  /** Copy constructor **/
  CbmRichPoint(const CbmRichPoint& point) : FairMCPoint(point) {};


  /** Destructor **/
  virtual ~CbmRichPoint();


  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;


  ClassDef(CbmRichPoint, 1)
};


#endif
