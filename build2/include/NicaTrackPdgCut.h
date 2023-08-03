/*
 * NicaTrackPdgCut.h
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATRACKPDGCUT_H_
#define NICATRACKPDGCUT_H_

#include "NicaTrackMCCut.h"
/**
 * check track pid
 */
class NicaTrackPdgCut : public NicaTrackMCCut {
 protected:
 public:
  NicaTrackPdgCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackPdgCut();
  ClassDef(NicaTrackPdgCut, 1)
};

class NicaTrackPdgAbsCut : public NicaTrackMCCut {
 protected:
 public:
  NicaTrackPdgAbsCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackPdgAbsCut();
  ClassDef(NicaTrackPdgAbsCut, 1)
};

#endif /* NICAPDGCUT_H_ */
