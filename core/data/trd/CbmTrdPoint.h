/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Denis Bertini [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                     CbmTrdPoint header file                   -----
// -----                 Created 27/07/04  by V. Friese                -----
// -------------------------------------------------------------------------

/**  CbmTrdPoint.h
 *@author V. Friese
 **
 ** Interception of MC track with a TR detector.
 **/


#ifndef CBMTRDPOINT_H
#define CBMTRDPOINT_H 1


#include <FairMCPoint.h>  // for FairMCPoint

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double32_t
#include <TVector3.h>    // for TVector3

#include <cstdint>
#include <string>

class CbmTrdPoint : public FairMCPoint {

public:
  /** Default constructor **/
  CbmTrdPoint();


  /** Constructor with arguments
     *@param trackID  Index of MCTrack
     *@param detID    Detector ID
     *@param posIn    Coordinates at entrance to active volume [cm]
     *@param posOut   Coordinates at exit of active volume [cm]
     *@param momIn    Momentum of track at entrance [GeV]
     *@param momOut   Momentum of track at exit [GeV]
     *@param tof      Time since event start [ns]
     *@param length   Track length since creation [cm]
     *@param eLoss    Energy deposit [GeV]
     **/
  CbmTrdPoint(int32_t trackID, int32_t detID, const TVector3& posIn, const TVector3& momIn, const TVector3& posOut,
              const TVector3& momOut, double tof, double length, double eLoss);


  /** Copy constructor **/
  //    CbmTrdPoint(const CbmTrdPoint& point) { *this = point; };


  /** Destructor **/
  virtual ~CbmTrdPoint();

  /** \brief Output to screen **/
  virtual void Print(const Option_t* opt) const;
  /** \brief Output to string.**/
  virtual std::string ToString() const;

  /** Accessors **/
  double GetXIn() const { return fX; }
  double GetYIn() const { return fY; }
  double GetZIn() const { return fZ; }
  double GetXOut() const { return fX_out; }
  double GetYOut() const { return fY_out; }
  double GetZOut() const { return fZ_out; }
  double GetPxOut() const { return fPx_out; }
  double GetPyOut() const { return fPy_out; }
  double GetPzOut() const { return fPz_out; }
  double GetPxIn() const { return fPx; }
  double GetPyIn() const { return fPy; }
  double GetPzIn() const { return fPz; }

  int32_t GetModuleAddress() const { return GetDetectorID(); }

  void PositionOut(TVector3& pos) const { pos.SetXYZ(fX_out, fY_out, fZ_out); }
  void MomentumOut(TVector3& mom) const { mom.SetXYZ(fPx_out, fPy_out, fPz_out); }

private:
  Double32_t fX_out, fY_out, fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;


  ClassDef(CbmTrdPoint, 3)
};


#endif
