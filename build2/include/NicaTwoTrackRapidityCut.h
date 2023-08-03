/*
 * NicaTwoTrackRapidityCut.h
 *
 *  Created on: 30-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATWOTRACKRAPIDITYCUT_H_
#define NICATWOTRACKRAPIDITYCUT_H_

#include "NicaTwoTrackCut.h"
/**
 * check pair rapidity
 */
class NicaTwoTrackRapidityCut : public NicaTwoTrackCut {
 public:
  NicaTwoTrackRapidityCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackRapidityCut();
  ClassDef(NicaTwoTrackRapidityCut, 1)
};

#endif /* NICATWOTRACKRAPIDITYCUT_H_ */
