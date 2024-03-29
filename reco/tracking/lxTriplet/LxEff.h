/* Copyright (C) 2016 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

#ifndef LXEFF_INCLUDED
#define LXEFF_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "Rtypes.h"

#include <iostream>
#include <list>
#include <map>

#include "LxSettings.h"

class LxFinderTriplet;
class LxPoint;

struct LxEff {
  explicit LxEff(LxFinderTriplet& owner);
  void CalcRecoEff(bool joinExt);
  void AnalyzeNotMatchedMC(std::ofstream& out, std::list<LxPoint*> hits[LXSTATIONS][LXLAYERS]);
#ifdef CLUSTER_MODE
  void AnalyzeNotMatched(std::ofstream& out, std::list<LxPoint*> hits[LXSTATIONS][LXLAYERS]);
  void AnalyzeNotMatched2(std::ofstream& out, std::list<LxPoint*> hits[LXSTATIONS][LXLAYERS]);
  void DumpNotMatchedMC(std::list<LxPoint*> hits[LXSTATIONS][LXLAYERS]);
#endif  //CLUSTER_MODE
  LxFinderTriplet& finder;
};

struct LxMCTrack;

struct LxStsMCPoint {
  Double_t p, q, x, y, z, px, py, pz;
  Int_t stationNumber;
  LxMCTrack* mcTrack;
};

#endif  //LXEFF_INCLUDED
