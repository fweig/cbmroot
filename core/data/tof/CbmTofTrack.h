/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Andrey Lebedev, Florian Uhlig */

/** @file CbmTofTrack.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.10.2013
 ** Updated 28/11/2021 by V.Akishina <v.akishina@gsi.de>
 **/


#ifndef CBMTOFTRACK_H
#define CBMTOFTRACK_H 1

#include "CbmHit.h"    // for kTOFHIT
#include "CbmTrack.h"  // for CbmTrack

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double32_t

#include <cassert>  // for assert
#include <cstdint>
#include <string>  // for string
#include <vector>  // for vector

/** @class CbmTofTrack.h
 ** @brief Data class with information on a TOF local track
 ** @author V.Friese <v.friese@gsi.de>
 **
 ** The TOF track is a collection of TOF hits, together
 ** with the track parameters obtained by the track fit.
 **/
class CbmTofTrack : public CbmTrack {
public:
  /** Default constructor **/
  CbmTofTrack();


  /** Destructor **/
  virtual ~CbmTofTrack();

  /** Associate a TofHit to the track
    ** @param hitIndex  Index of the TOF hit in TClonesArray
    **/
  void AddTofHit(int32_t hitIndex) { AddHit(hitIndex, kTOFHIT); }


  /**  PID hypothesis for track extrapolation to TOF **/
  int32_t GetPidHypo() const { return fPidHypo; }

  /**  mass hypothesis from TOF **/
  float GetMass() const { return fMass; }

  /**  Index of TOF hit **/
  int32_t GetTofHitIndex() const { return GetHitIndex(0); }

  /**  Error of track x coordinate at TOF  **/
  double GetTrackDx() const { return sqrt(fTrackPar.GetCovariance(1, 1)); }

  /**  Error of track x coordinate at TOF  **/
  double GetTrackDy() const { return sqrt(fTrackPar.GetCovariance(2, 2)); }

  /**  Index of global track **/
  int32_t GetTrackIndex() const { return fGlbTrack; }

  /**  Track length from primary vertex to TOF **/
  double GetTrackLength() const { return fTrackLength; }

  /**  Track parameters at TOF **/
  const FairTrackParam* GetTrackParameter() const { return &fTrackPar; }

  /**  Track x position at TOF  **/
  double GetTrackX() const { return fTrackPar.GetX(); }

  /**  Track y position at TOF  **/
  double GetTrackY() const { return fTrackPar.GetY(); }

  /** Normalized distance from hit to track **/
  double GetDistance() const { return fDistance; }

  /** Set track index **/
  void SetTrackIndex(int32_t trackIndex) { fGlbTrack = trackIndex; }

  /** Set TOF hit index **/
  void SetTofHitIndex(int32_t tofHitIndex)
  {
    fHitIndex.clear();
    fHitType.clear();
    AddTofHit(tofHitIndex);
  }

  /** Set track parameter **/
  void SetTrackParameter(const FairTrackParam* par) { fTrackPar = *par; }

  /** Set track length **/
  void SetTrackLength(double trackLength) { fTrackLength = trackLength; }

  /** Set PID hypothesis for track extrapolation to TOF **/
  void SetPidHypo(int32_t pid) { fPidHypo = pid; }

  /** Set mass from TOF **/
  void SetMass(float mass) { fMass = mass; }

  /** Set normalized distance from hit to track **/
  void SetDistance(double distance) { fDistance = distance; }


  /** Number of TOF hits
    ** @return  Number of TOF hits associated to the track
    **/
  int32_t GetNofTofHits() const { return CbmTrack::GetNofHits(); }


  /** Index of a TOF hit
    ** @value Array index of the ith TOF hit of the track
    **
    ** Throws std::vector exception if out of bounds.
    **/
  int32_t GetTofHitIndex(int32_t iHit) const
  {
    assert(iHit < GetNofTofHits());
    return GetHitIndex(iHit);
  }

  /** Debug output **/
  virtual std::string ToString() const;


private:
  int32_t fGlbTrack;         ///< Index of global track
  double fTrackLength;       ///< Track length from primary vertex to TOF [cm]
  FairTrackParam fTrackPar;  ///< Track parameters at z of TofHit
  int32_t fPidHypo;          ///< PID hypothesis used for track extrapolation
  float fMass;               ///< Mass from Tof
  double fDistance = 0.;     ///< Normalized distance from hit to track


  ClassDef(CbmTofTrack, 3);
};

#endif
