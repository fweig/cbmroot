/* Copyright (C) 2013-2017 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/*
 * CbmLitTofTrack.h
 *
 *  Created on: Nov 4, 2013
 *      Author: andrey
 */

#ifndef CBMLITTOFTRACK_H_
#define CBMLITTOFTRACK_H_

#include "data/CbmLitTrackParam.h"
class CbmLitHit;
class CbmLitTrack;

/**
 * \class CbmLitFitNode
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 * \brief Data class for storage of fitted track parameters, transport matrix and chi-square on each detector station.
 */
class CbmLitTofTrack {
public:
  /**
    * \brief Constructor.
    */
  CbmLitTofTrack() : fTrack(NULL), fHit(NULL), fTrackParam(), fDistance(0.) {}

  CbmLitTofTrack(const CbmLitTofTrack&) = delete;
  CbmLitTofTrack& operator=(const CbmLitTofTrack&) = delete;
  /**
    * \brief Destructor.
    */
  virtual ~CbmLitTofTrack() {}

  /* Getters */
  const CbmLitTrack* GetTrack() const { return fTrack; }
  const CbmLitHit* GetHit() const { return fHit; }
  const CbmLitTrackParam* GetTrackParam() const { return &fTrackParam; }
  litfloat GetDistance() const { return fDistance; }

  /* Setters */
  void SetTrack(const CbmLitTrack* track) { fTrack = track; }
  void SetHit(const CbmLitHit* hit) { fHit = hit; }
  void SetTrackParam(const CbmLitTrackParam* par) { fTrackParam = *par; }
  void SetDistance(litfloat distance) { fDistance = distance; }

private:
  const CbmLitTrack* fTrack;     // Pointer to track
  const CbmLitHit* fHit;         // Pointer to TOF hit
  CbmLitTrackParam fTrackParam;  // Track parameters.
  litfloat fDistance;            // Contribution to chi-square of updated track parameters and hit.
};

#endif /* CBMLITTOFTRACK_H_ */
