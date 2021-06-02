/** @file LxMC.h
  * @copyright Copyright (C) 2016 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [committer] **/

#ifndef LXMC_INCLUDED
#define LXMC_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "CbmStsTrack.h"

#include "Rtypes.h"

#include <list>
#include <vector>

#include "LxSettings.h"

struct LxPoint;
struct LxStsMCPoint;
struct LxMCTrack;

struct LxMCPoint {
  Double_t p, q, x, y, z, px, py, pz;
  Int_t stationNumber, layerNumber;
  std::list<LxPoint*> lxPoints;
  Int_t trackId;  // Original track id -- an index (offset) in the array of MC tracks.
  LxMCTrack* track;
};

struct LxMCTrack {
  Double_t p, q, x, y, z, px, py, pz;
  Int_t mother_ID;
  Int_t pdg;
  std::vector<LxMCPoint*> Points;
#ifdef MAKE_STS_MC_RESEARCHES
  std::list<LxStsMCPoint*> stsPoints[8];
#endif  //MAKE_STS_MC_RESEARCHES
  UInt_t fUniqueID;
  CbmStsTrack* externalTrack;
  Int_t stationsWithHits;
  Int_t layersWithHits;
  bool hitsOnStations[LXSTATIONS][LXLAYERS];
};

#endif  //LXMC_INCLUDED
