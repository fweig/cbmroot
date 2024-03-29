/* Copyright (C) 2006-2012 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dmytro Kresan, Denis Bertini [committer] */

// ------------------------------------------------------------------
// -----                    CbmGlobalTrackFitter                -----
// -----              Created 07/03/2006 by D.Kresan            -----
// ------------------------------------------------------------------
#ifndef CBMGLOBALTRACKFITTER
#define CBMGLOBALTRACKFITTER

#include "TObject.h"

class CbmGlobalTrack;


class CbmGlobalTrackFitter : public TObject {

protected:
  Int_t fVerbose;  // Verbosity level

public:
  CbmGlobalTrackFitter() : TObject(), fVerbose(0) {};
  virtual ~CbmGlobalTrackFitter() {};

  virtual void Init() {};
  virtual void DoFit(CbmGlobalTrack* glbTrack) = 0;

  inline void SetVerbose(Int_t verbose) { fVerbose = verbose; }

  ClassDef(CbmGlobalTrackFitter, 1);
};


#endif
