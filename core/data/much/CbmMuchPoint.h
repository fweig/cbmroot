/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Denis Bertini [committer], Mikhail Ryzhinskiy */

/** CbmMuchPoint.h
 *
 * @author  M.Ryzhinskiy <m.ryzhinskiy@gsi.de>
 * @version 1.0
 * @since   21.03.07
 *
 *  Class for Monte Carlo points in MUon CHambers detector
 *
 */


#ifndef CBMMUCHPOINT_H
#define CBMMUCHPOINT_H

#include <FairMCPoint.h>  // for FairMCPoint

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double32_t
#include <TVector3.h>    // for TVector3

#include <cstdint>

class CbmMuchPoint : public FairMCPoint {

public:
  /** Default constructor **/
  CbmMuchPoint();


  /** Constructor with arguments
   *@param trackID  Index of MCTrack
   *@param detID    Detector ID (including module number)
   *@param posIn    Coordinates at entrance to active volume [cm]
   *@param posOut   Coordinates at exit of active volume [cm]
   *@param momIn    Momentum of track at entrance [GeV]
   *@param momOut   Momentum of track at exit [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [GeV]
   *@param eventId  MC event identifier
   **/
  CbmMuchPoint(int32_t trackID, int32_t detID, TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
               double tof, double length, double eLoss, int32_t eventId = 0);


  /** Copy constructor with event and epoch time 
   ** Re-calculates time w.r.t. epoch time start
   *@param eventId     MC event identifier (negative values keep original event ID)
   *@param eventTime   MC event time [ns]
   *@param epochTime   epoch start time [ns]
   **/
  CbmMuchPoint(const CbmMuchPoint& point, int32_t eventId = -1, double eventTime = 0., double epochTime = 0.);


  /** Destructor **/
  virtual ~CbmMuchPoint();


  /** Accessors **/
  int32_t GetDetectorId() const { return fDetectorID; }
  double GetXIn() const { return fX; }
  double GetYIn() const { return fY; }
  double GetZIn() const { return fZ; }
  double GetXOut() const { return fX_out; }
  double GetYOut() const { return fY_out; }
  double GetZOut() const { return fZ_out; }
  double GetPxOut() const { return fPx_out; }
  double GetPyOut() const { return fPy_out; }
  double GetPzOut() const { return fPz_out; }
  void PositionIn(TVector3& pos) const { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) const { pos.SetXYZ(fX_out, fY_out, fZ_out); }
  void MomentumOut(TVector3& mom) const { mom.SetXYZ(fPx_out, fPy_out, fPz_out); }

  /** Point coordinates at given z from linear extrapolation **/
  double GetX(double z) const;
  double GetY(double z) const;

  /** Check for distance between in and out **/
  bool IsUsable() const;

  /** Modifiers **/
  void SetPositionOut(TVector3 pos);
  void SetMomentumOut(TVector3 mom);

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

protected:
  Double32_t fX_out, fY_out, fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;

  ClassDef(CbmMuchPoint, 1)
};


inline void CbmMuchPoint::SetPositionOut(TVector3 pos)
{
  fX_out = pos.X();
  fY_out = pos.Y();
  fZ_out = pos.Z();
}


inline void CbmMuchPoint::SetMomentumOut(TVector3 mom)
{
  fPx_out = mom.Px();
  fPy_out = mom.Py();
  fPz_out = mom.Pz();
}
#endif
