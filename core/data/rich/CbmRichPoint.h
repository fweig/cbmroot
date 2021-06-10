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
#include <RtypesCore.h>  // for Double_t, Int_t, Option_t

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
  CbmRichPoint(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom, Double_t tof, Double_t length, Double_t eLoss);


  /** Copy constructor **/
  CbmRichPoint(const CbmRichPoint& point) : FairMCPoint(point) {};


  /** Destructor **/
  virtual ~CbmRichPoint();


  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;


  ClassDef(CbmRichPoint, 1)
};


#endif
