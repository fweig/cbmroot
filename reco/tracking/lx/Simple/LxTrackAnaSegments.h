/** @file LxTrackAnaSegments.h
  * @copyright Copyright (C) 2014-2016 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [orginator] **/

#ifndef LXTRACKANASEGMENTS_INCLUDED
#define LXTRACKANASEGMENTS_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "TString.h"

struct LxSimpleTrack;
class LxTrackAna;

class LxTrackAnaSegments {
public:
  explicit LxTrackAnaSegments(LxTrackAna& o);
  void Init();
  void Finish();
  void BuildStatistics();
  void SetParticleType(TString v);

private:
  void StatForTrack(LxSimpleTrack* track);
  LxTrackAna& owner;
  Int_t stationsInAlgo;
};

#endif  //LXTRACKANASEGMENTS_INCLUDED
