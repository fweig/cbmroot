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

#include <TMath.h>
#include <TRandom3.h>

#include "PairAnalysisTrackRotator.h"

ClassImp(PairAnalysisTrackRotator)

PairAnalysisTrackRotator::PairAnalysisTrackRotator() :
  PairAnalysisTrackRotator("TR","TR")
{
  //
  // Default Constructor
  //
}

//______________________________________________
PairAnalysisTrackRotator::PairAnalysisTrackRotator(const char* name, const char* title) :
  TNamed(name, title)
{
  //
  // Named Constructor
  //
  gRandom->SetSeed();
}

//______________________________________________
PairAnalysisTrackRotator::~PairAnalysisTrackRotator()
{
  //
  // Default Destructor
  //

}


