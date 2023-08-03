/*
 * NicaTwoTrackPtOrderCut.h
 *
 *  Created on: 30 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATWOTRACKPTORDERCUT_H_
#define NICATWOTRACKPTORDERCUT_H_

#include "NicaTwoTrackCut.h"

/**
 * pass pair if Pt of first particle is larger than Pt of second particle
 */
class NicaTwoTrackPtOrderCut : public NicaTwoTrackCut {
 public:
  NicaTwoTrackPtOrderCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  ClassDef(NicaTwoTrackPtOrderCut, 1) virtual ~NicaTwoTrackPtOrderCut();
};

#endif /* NICATWOTRACKPTORDERCUT_H_ */
