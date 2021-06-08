/* Copyright (C) 2008-2011 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/*
 * Does nothing, used as a stub.
 */

#ifndef CBMLITTRACKSELECTIONEMPTY_H_
#define CBMLITTRACKSELECTIONEMPTY_H_

#include "interface/CbmLitTrackSelection.h"

class CbmLitTrackSelectionEmpty : public CbmLitTrackSelection {
public:
  /* Constructor */
  CbmLitTrackSelectionEmpty();

  /* Destructor */
  virtual ~CbmLitTrackSelectionEmpty();

  /* Derived from CbmLitTrackSelection */
  virtual LitStatus DoSelect(TrackPtrIterator itBegin, TrackPtrIterator itEnd);

  /* Derived from CbmLitTrackSelection */
  virtual LitStatus DoSelect(TrackPtrVector& tracks);
};

#endif /*CBMLITTRACKSELECTIONEMPTY_H_*/
