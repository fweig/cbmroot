/* Copyright (C) 2007-2012 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file CbmLitTrackFitterImp.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2007
 * \brief Implementation of Kalman Filter track fit.
 **/

#ifndef CBMLITTRACKFITTERIMP_H_
#define CBMLITTRACKFITTERIMP_H_

#include "base/CbmLitPtrTypes.h"
#include "interface/CbmLitTrackFitter.h"

/**
 * \class CbmLitTrackFitterImp
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2007
 * \brief Implementation of Kalman Filter track fit.
 **/
class CbmLitTrackFitterImp : public CbmLitTrackFitter {
public:
  /**
    * \brief Constructor.
    * \param[in] propagator Track propagation tool.
    * \param[in] update Track update tool.
    */
  CbmLitTrackFitterImp(TrackPropagatorPtr propagator, TrackUpdatePtr update);

  /**
    * \brief Destructor.
    */
  virtual ~CbmLitTrackFitterImp();

  /**
    * \brief Inherited from CbmLitTrackFitter.
    */
  virtual LitStatus Fit(CbmLitTrack* track, bool downstream = true);

private:
  TrackPropagatorPtr fPropagator;  // Track propagation tool
  TrackUpdatePtr fUpdate;          // Track update tool
};

#endif /*CBMLITTRACKFITTERIMP_H_*/
