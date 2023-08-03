/*
 * NicaTwoTrackKtCut.h
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATWOTRACKKTCUT_H_
#define NICATWOTRACKKTCUT_H_

#include "NicaTwoTrackCut.h"
/**
 * check pair kt value
 */
class NicaTwoTrackKtCut : public NicaTwoTrackCut {
 protected:
 public:
  NicaTwoTrackKtCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackKtCut();
  ClassDef(NicaTwoTrackKtCut, 1)
};

#endif /* NICAKTCUT_H_ */
