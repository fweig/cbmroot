/*
 * NicaTwoTrackEtaCut.h
 *
 *  Created on: 24-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATWOTRACKETACUT_H_
#define NICATWOTRACKETACUT_H_

#include "NicaTwoTrackCut.h"
/**
 * check pair eta value
 */
class NicaTwoTrackEtaCut : public NicaTwoTrackCut {
 public:
  NicaTwoTrackEtaCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackEtaCut();
  ClassDef(NicaTwoTrackEtaCut, 1)
};

#endif /* NICATWOTRACKETACUT_H_ */
