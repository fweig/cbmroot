/* Copyright (C) 2016 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

#ifndef LXTRACKANASEGMENTS_INCLUDED
#define LXTRACKANASEGMENTS_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "TString.h"

struct LxSimpleTrack;
class LxTrackAnaTriplet;

extern bool saveHistos;

class LxTrackAnaSegments {
public:
  explicit LxTrackAnaSegments(LxTrackAnaTriplet& o);
  void Init();
  void Finish();
  void BuildStatistics();
  void SetParticleType(TString v);
  void SetUseBgr(bool v)
  {
    useBgr     = v;
    saveHistos = !v;
  }

private:
  void StatForTrack(LxSimpleTrack* track);
  LxTrackAnaTriplet& owner;
  Int_t stationsInAlgo;
  bool useBgr;
};

#endif  //LXTRACKANASEGMENTS_INCLUDED
