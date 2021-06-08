/* Copyright (C) 2008-2011 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

#include "selection/CbmLitTrackSelectionEmpty.h"

CbmLitTrackSelectionEmpty::CbmLitTrackSelectionEmpty() {}

CbmLitTrackSelectionEmpty::~CbmLitTrackSelectionEmpty() {}

LitStatus CbmLitTrackSelectionEmpty::DoSelect(TrackPtrIterator itBegin, TrackPtrIterator itEnd) { return kLITSUCCESS; }

LitStatus CbmLitTrackSelectionEmpty::DoSelect(TrackPtrVector& tracks) { return DoSelect(tracks.begin(), tracks.end()); }
