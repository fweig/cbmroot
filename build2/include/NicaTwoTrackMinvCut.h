/*
 * NicaTwoTrackMinvCut.h
 *
 *  Created on: 25-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATWOTRACKMINVCUT_H_
#define NICATWOTRACKMINVCUT_H_

#include "NicaTwoTrackCut.h"
/**
 * check pair mass invariant value
 */
class NicaTwoTrackMinvCut : public NicaTwoTrackCut {
 public:
  NicaTwoTrackMinvCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackMinvCut();
  ClassDef(NicaTwoTrackMinvCut, 1)
};

#endif /* NICATWOTRACKMINVCUT_H_ */
