/*
 * NicaTwoTrackPtSumCut.h
 *
 *  Created on: 12-06-2014
 *      Author: wielanek
 */

#ifndef NICATWOTRACKPTSUMCUT_H_
#define NICATWOTRACKPTSUMCUT_H_

#include "NicaTwoTrackCut.h"
/**
 * check half of pair pt sum value |pt1+pt2|/2
 */
class NicaTwoTrackPtSumCut : public NicaTwoTrackCut {
 public:
  NicaTwoTrackPtSumCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackPtSumCut();
  ClassDef(NicaTwoTrackPtSumCut, 1)
};

#endif /* NICATWOTRACKPTSUMCUT_H_ */
