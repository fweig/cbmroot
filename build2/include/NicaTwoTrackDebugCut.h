/*
 * NicaTwoTrackDebugCut.h
 *
 *  Created on: 17-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATWOTRACKDEBUGCUT_H_
#define NICATWOTRACKDEBUGCUT_H_

#include "NicaTwoTrackCut.h"
/**
 * class for debugging two track cuts
 */
class NicaTwoTrackDebugCut : public NicaTwoTrackCut {
 public:
  NicaTwoTrackDebugCut();
  Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackDebugCut();
  ClassDef(NicaTwoTrackDebugCut, 1)
};

#endif /* NICATWOTRACKDEBUGCUT_H_ */
