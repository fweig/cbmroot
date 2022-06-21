/* Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer] */

#include "L1Event.h"

void L1Event::Clear() { fHit.clear(); }

void L1Event::TopoReconstructor()
{

  fTopoReconstructor.Init(fKFPTrack, fKFPTrack);
  fTopoReconstructor.ReconstructPrimVertex();
  fTopoReconstructor.SortTracks();
  fTopoReconstructor.ReconstructParticles();
  //cout<<"fTopoReconstructor "<<fTopoReconstructor.GetParticles().size();
}
