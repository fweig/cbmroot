/*
 * NicaTrackStartPositionCut.h
 *
 *  Created on: 4 lip 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKSTARTPOSITIONCUT_H_
#define NICATRACKSTARTPOSITIONCUT_H_

#include "NicaTrackMCCut.h"

class NicaTrackStartPositionCut : public NicaTrackMCCut {
 public:
  NicaTrackStartPositionCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackStartPositionCut();
  ClassDef(NicaTrackStartPositionCut, 1)
};

#endif /* NICATRACKSTARTPOSITIONCUT_H_ */
