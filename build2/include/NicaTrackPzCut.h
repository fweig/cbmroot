/*
 * NicaTrackPzCut.h
 *
 *  Created on: 5 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKPZCUT_H_
#define NICATRACKPZCUT_H_
#include "NicaTrackCut.h"
class NicaTrackPzCut : public NicaTrackCut {
 public:
  NicaTrackPzCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackPzCut();
  ClassDef(NicaTrackPzCut, 1)
};

#endif /* NICATRACKPZCUT_H_ */
