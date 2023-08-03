/*
 * NicaTwoTrackPhiCut.h
 *
 *  Created on: 08-12-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATWOTRACKPHICUT_H_
#define NICATWOTRACKPHICUT_H_
#include "NicaTwoTrackCut.h"
/**
 * check pair azimuthal angle
 */
class NicaTwoTrackPhiCut : public NicaTwoTrackCut {
 public:
  NicaTwoTrackPhiCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackPhiCut();
  ClassDef(NicaTwoTrackPhiCut, 1)
};

#endif /* NICATWOTRACKPHICUT_H_ */
