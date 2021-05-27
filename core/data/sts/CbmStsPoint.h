// -------------------------------------------------------------------------
// -----                      CbmStsPoint header file                  -----
// -----                  Created 26/07/04  by V. Friese               -----
// -------------------------------------------------------------------------


/**  CbmStsPoint.h
 *@author V.Friese <v.friese@gsi.de>
 *
 * Interception of MC track with a STS detetcor. Holds in addition
 * to the base class the coordinates and momentum at the exit from
 * the active volume.
 **/


#ifndef CBMSTSPOINT_H
#define CBMSTSPOINT_H

#include <FairMCPoint.h>  // for FairMCPoint

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Double32_t, Bool_t, Short_t
#include <TVector3.h>    // for TVector3

#include <string>  // for string

class CbmStsPoint : public FairMCPoint {

public:
  /** Default constructor **/
  CbmStsPoint();


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
   *@param pid      Particle ID (PDG code)
   *@param eventId  MC event identifier
   *@param index    Index of point in TClonesArray
   *@param flag     +1 if entering, +2 if leaving the sensor
   **/
  CbmStsPoint(Int_t trackID, Int_t detID, TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
              Double_t tof, Double_t length, Double_t eLoss, Int_t pid = 0, Int_t eventId = 0, Int_t index = 0,
              Short_t flag = 3);


  /** Copy constructor with event and epoch time 
   ** Re-calculates time w.r.t. epoch time start
   *@param eventId     MC event identifier (negative values keep original event ID)
   *@param eventTime   MC event time [ns]
   *@param epochTime   epoch start time [ns]
   **/
  CbmStsPoint(const CbmStsPoint& point, Int_t eventId = -1, Double_t eventTime = 0., Double_t epochTime = 0.);


  /** Destructor **/
  virtual ~CbmStsPoint();


  /** Accessors **/
  Double_t GetXIn() const { return fX; }
  Double_t GetYIn() const { return fY; }
  Double_t GetZIn() const { return fZ; }
  Double_t GetXOut() const { return fX_out; }
  Double_t GetYOut() const { return fY_out; }
  Double_t GetZOut() const { return fZ_out; }
  Double_t GetPxOut() const { return fPx_out; }
  Double_t GetPyOut() const { return fPy_out; }
  Double_t GetPzOut() const { return fPz_out; }
  Int_t GetPid() const { return fPid; }
  Int_t GetIndex() const { return fIndex; }
  Bool_t IsEntry() const { return (fFlag == 1 || fFlag == 3); }
  Bool_t IsExit() const { return (fFlag == 2 || fFlag == 3); }

  void PositionIn(TVector3& pos) { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out, fY_out, fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out, fPy_out, fPz_out); }


  /** Point coordinates at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;


  /** Check for distance between in and out **/
  Bool_t IsUsable() const;


  /** Modifiers **/
  void SetPositionOut(TVector3 pos);
  void SetMomentumOut(TVector3 mom);
  virtual void SetTrackID(Int_t id)
  {
    //SetLink(kMCTrack, id);
    FairMCPoint::SetTrackID(id);
  };


  /** String output **/
  std::string ToString() const;


protected:
  Double32_t fX_out, fY_out, fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;
  Int_t fPid;     ///> Particle ID [PDG code]
  Int_t fIndex;   ///> Index of point in its TClonesArray
  Short_t fFlag;  ///> 1 or 3: track entry step; 2 or 3: track exit step


  ClassDef(CbmStsPoint, 3)
};


inline void CbmStsPoint::SetPositionOut(TVector3 pos)
{
  fX_out = pos.X();
  fY_out = pos.Y();
  fZ_out = pos.Z();
}


inline void CbmStsPoint::SetMomentumOut(TVector3 mom)
{
  fPx_out = mom.Px();
  fPy_out = mom.Py();
  fPz_out = mom.Pz();
}


#endif
