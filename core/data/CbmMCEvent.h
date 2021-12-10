/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#/** CbmMCEvent.h
 *@author V.Friese <v.friese@gsi.de>
 ** Data class (level MC) containing information about the input event.
 ** 15.05.2008 change the event time to ns (M. Al-Turany)
 ** 11.05.2009 New CBM class derived from FairMCEventHeader
 **/


#ifndef CBMMCEVENT_H
#define CBMMCEVENT_H 1

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TNamed.h>
#include <TVector3.h>

#include <cstdint>


class CbmMCEvent : public TNamed {

public:
  /** Default constructor **/
  CbmMCEvent();


  /** Constructor with all members
   **
   *@param runId    run identifier
   *@param iEvent   event identifier
   *@param x,y,z    vertex oordinates [cm]
   *@param t        event time [ns]
   *@param b        impact parameter [fm] (if relevant)
   *@param phi      event plane angle [rad]
   *@param nPrim    number of input tracks
   **/
  CbmMCEvent(uint32_t runId, int32_t iEvent, double x, double y, double z, double t, double b, double phi,
             int32_t nPrim);


  /** Standard constructor with run identifier **/
  CbmMCEvent(uint32_t runId);


  /** Destructor **/
  virtual ~CbmMCEvent();


  /** Accessors **/
  uint32_t GetRunID() const { return fRunId; }     // run identifier
  int32_t GetEventID() const { return fEventId; }  // event identifier
  double GetX() const { return fX; }               // vertex x [cm]
  double GetY() const { return fY; }               // vertex y [cm]
  double GetZ() const { return fZ; }               // vertex z [cm]
  double GetT() const { return fT; }               // event time [ns]
  double GetB() const { return fB; }               // impact parameter [fm]
  double GetPhi() const { return fPhi; }           // event plane angle [rad]
  int32_t GetNPrim() const { return fNPrim; }      // number of input tracks
  bool IsSet() const { return fIsSet; }            // Flag
  void GetVertex(TVector3& vertex) { vertex.SetXYZ(fX, fY, fZ); }


  /** Modifiers **/
  void SetEventID(int32_t eventId) { fEventId = eventId; }
  void SetTime(double t) { fT = t; }
  void SetB(double b) { fB = b; }
  void SetPhi(double phi) { fPhi = phi; }
  void SetNPrim(int32_t nPrim) { fNPrim = nPrim; }
  void MarkSet(bool isSet) { fIsSet = isSet; }
  void SetVertex(double x, double y, double z);
  void SetVertex(const TVector3& vertex);


  /** Reset all members **/
  void Reset();


private:
  uint32_t fRunId;    //  Run identifier
  uint32_t fEventId;  //  Event identifier
  Double32_t fX;      //  Primary vertex x [cm]
  Double32_t fY;      //  Primary vertex y [cm]
  Double32_t fZ;      //  Primary vertex z [cm]
  Double32_t fT;      //  Event time [s]
  Double32_t fB;      //  Impact parameter [fm] (if relevant)
  Double32_t fPhi;    //  Event plane angle [rad] (if relevant)
  int32_t fNPrim;     //  Number of input tracks
  bool fIsSet;        //  Flag whether variables are filled


  ClassDef(CbmMCEvent, 1);
};


inline void CbmMCEvent::SetVertex(double x, double y, double z)
{
  fX = x;
  fY = y;
  fZ = z;
}


inline void CbmMCEvent::SetVertex(const TVector3& vertex)
{
  fX = vertex.X();
  fY = vertex.Y();
  fZ = vertex.Z();
}


#endif
