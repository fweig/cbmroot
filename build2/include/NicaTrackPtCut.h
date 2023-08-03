/*
 * NicaTrackPtCut.h
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATRACKPTCUT_H_
#define NICATRACKPTCUT_H_

#include "NicaTrackCut.h"
/**
 * check track transverse momentum
 */
class NicaTrackPtCut : public NicaTrackCut {
 protected:
 public:
  NicaTrackPtCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackPtCut();
  ClassDef(NicaTrackPtCut, 1)
};

#endif /* NICAPTCUT_H_ */
