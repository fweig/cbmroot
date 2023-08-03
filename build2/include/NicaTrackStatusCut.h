/*
 * NicaTrackStatusCut.h
 *
 *  Created on: 15-07-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKSTATUSCUT_H_
#define NICATRACKSTATUSCUT_H_
#include "NicaTrackCut.h"

/**
 * check track status
 */
class NicaTrackStatusCut : public NicaTrackCut {
 public:
  NicaTrackStatusCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackStatusCut();
  ClassDef(NicaTrackStatusCut, 1)
};

#endif /* NICATRACKSTATUSCUT_H_ */
