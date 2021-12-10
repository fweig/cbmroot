/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Andrey Lebedev, Florian Uhlig */

/** @file CbmTofTrack.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.10.2013
 **/

#ifndef CBMTOFTRACK_H
#define CBMTOFTRACK_H 1

#include <FairTrackParam.h>  // for FairTrackParam

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double32_t
#include <TObject.h>     // for TObject

#include <cstdint>

#include <cmath>

/** @class CbmTofTrack
 ** @brief Provides information on attaching a TofHit to a GlobalTrack
 ** @author V.Friese <v.friese@gsi.de>
 **/
class CbmTofTrack : public TObject {

public:
  /**   Constructor   **/
  CbmTofTrack();


  /** Constructor with parameters
	 ** @param trackIndex   Array index of global track
	 ** @param hitIndex     Array index of TOF hit
	 ** @param trackLength  Track length from primary vertex to TOF
	 ** @param trackPar     Parameters of track at TOF
	 ** @param pidHypo      PID hypothesis for track extrapolation
	 **/

  CbmTofTrack(int32_t trackIndex, int32_t hitIndex, double trackLength, float mass, const FairTrackParam& trackPar,
              int32_t pidHypo)
    : TObject()
    , fGlbTrack(trackIndex)
    , fTofHit(hitIndex)
    , fTrackLength(trackLength)
    , fTrackPar(trackPar)
    , fPidHypo(pidHypo)
    , fMass(mass)
    , fDistance(0.)
  {
  }


  /**   Destructor   **/
  virtual ~CbmTofTrack();

  /**  PID hypothesis for track extrapolation to TOF **/
  int32_t GetPidHypo() const { return fPidHypo; }

  /**  mass hypothesis from TOF **/
  float GetMass() const { return fMass; }

  /**  Index of TOF hit **/
  int32_t GetTofHitIndex() const { return fTofHit; }

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
  void SetTofHitIndex(int32_t tofHitIndex) { fTofHit = tofHitIndex; }

  /** Set track parameter **/
  void SetTrackParameter(const FairTrackParam* par) { fTrackPar = *par; }

  /** Set track length **/
  void SetTrackLength(Double32_t trackLength) { fTrackLength = trackLength; }

  /** Set PID hypothesis for track extrapolation to TOF **/
  void SetPidHypo(int32_t pid) { fPidHypo = pid; }

  /** Set mass from TOF **/
  void SetMass(float mass) { fMass = mass; }

  /** Set normalized distance from hit to track **/
  void SetDistance(double distance) { fDistance = distance; }

private:
  int32_t fGlbTrack;         ///< Index of global track
  int32_t fTofHit;           ///< Index of TofHit
  Double32_t fTrackLength;   ///< Track length from primary vertex to TOF [cm]
  FairTrackParam fTrackPar;  ///< Track parameters at z of TofHit
  int32_t fPidHypo;          ///< PID hypothesis used for track extrapolation
  float fMass;               ///< Mass from Tof
  Double32_t fDistance;      ///< Normalized distance from hit to track


  ClassDef(CbmTofTrack, 1);
};

#endif /* CBMTOFTRACK_H */
