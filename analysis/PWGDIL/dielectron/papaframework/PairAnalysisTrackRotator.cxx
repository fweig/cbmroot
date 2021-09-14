/* Copyright (C) 1998-2009 ALICE Experiment, CERN
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Julian Book [committer] */


///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                                                       //
// Authors:
//   * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
//   Julian Book   <Julian.Book@cern.ch>
/*

  This class keeps the configuration of the TrackRotator, the
  actual track rotation is done in PairAnalysisPair::RotateTrack

  Angles and charges used in the track rotation are provided.

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "PairAnalysisTrackRotator.h"

#include <TMath.h>
#include <TRandom3.h>

ClassImp(PairAnalysisTrackRotator)

  PairAnalysisTrackRotator::PairAnalysisTrackRotator()
  : PairAnalysisTrackRotator("TR", "TR")
{
  //
  // Default Constructor
  //
}

//______________________________________________
PairAnalysisTrackRotator::PairAnalysisTrackRotator(const char* name, const char* title) : TNamed(name, title)
{
  //
  // Named Constructor
  //
}

//______________________________________________
PairAnalysisTrackRotator::~PairAnalysisTrackRotator()
{
  //
  // Default Destructor
  //
}
