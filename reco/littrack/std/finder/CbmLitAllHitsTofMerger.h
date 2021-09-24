/* Copyright (C) 2013 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file CbmLitAllHitsTofMerger.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 * \brief Hit-to-track merging in TOF detector attaching all hits
 * in the validation region.
 **/

#ifndef CBMLITALLHITSTOFMERGER_H_
#define CBMLITALLHITSTOFMERGER_H_

#include "base/CbmLitPtrTypes.h"
#include "interface/CbmLitHitToTrackMerger.h"

class CbmLitTrackPropagator;
class CbmLitTrackUpdate;

class CbmLitAllHitsTofMerger : public CbmLitHitToTrackMerger {
public:
  /**
    * \brief Constructor.
    */
  CbmLitAllHitsTofMerger();

  /**
    * \brief Destructor.
    */
  virtual ~CbmLitAllHitsTofMerger();

  /**
    * \brief Inherited from CbmLitHitToTrackMerger */
  virtual LitStatus DoMerge(HitPtrVector& hits,  //TODO: add const here
                            TrackPtrVector& tracks, TofTrackPtrVector& tofTracks);

  /** Setters **/
  void SetFieldPropagator(TrackPropagatorPtr propagator) { fFieldPropagator = propagator; }
  void SetLinePropagator(TrackPropagatorPtr propagator) { fLinePropagator = propagator; }
  void SetFilter(TrackUpdatePtr filter) { fFilter = filter; }
  void SetPDG(Int_t pdg) { fPDG = pdg; }
  void SetChiSqCut(litfloat chiSqCut) { fChiSqCut = chiSqCut; }

private:
  TrackPropagatorPtr fFieldPropagator;  // Field track propagation tool
  TrackPropagatorPtr fLinePropagator;   // Line track propagation tool
  TrackUpdatePtr fFilter;               // Track update tool
  Int_t fPDG;                           // PDG hypothesis
  litfloat fChiSqCut;                   // Chi square cut for hit to be attached to track.
};

#endif /* CBMLITALLHITSTOFMERGER_H_ */
