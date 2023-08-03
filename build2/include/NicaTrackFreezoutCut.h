/*
 * NicaTrackFreezoutCut.h
 *
 *  Created on: 05-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKFREEZOUTCUT_H_
#define NICATRACKFREEZOUTCUT_H_
#include "NicaTrackCut.h"
#include "NicaTrackMCCut.h"
/**
 * check freezout parameters, has four for X,Y,Z and T
 */
class NicaTrackFreezoutCut : public NicaTrackMCCut {
 public:
  NicaTrackFreezoutCut();
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackFreezoutCut();
  ClassDef(NicaTrackFreezoutCut, 1)
};

/**
 * check track freezout time
 */
class NicaTrackTFreezCut : public NicaTrackMCCut {
 public:
  NicaTrackTFreezCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackTFreezCut();
  ClassDef(NicaTrackTFreezCut, 1)
};

/**
 * check freezout tau for track
 */
class NicaTrackTauCut : public NicaTrackMCCut {
 public:
  NicaTrackTauCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackTauCut();
  ClassDef(NicaTrackTauCut, 1)
};

#endif /* NICATRACKFREEZOUTCUT_H_ */
