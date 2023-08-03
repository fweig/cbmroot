/*
 * NicaTrackPdgBinCut.h
 *
 *  Created on: 27 gru 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKPDGBINCUT_H_
#define NICATRACKPDGBINCUT_H_

#include "NicaTrackCut.h"

class NicaTrackPdgBinCut : public NicaTrackCut {
 public:
  NicaTrackPdgBinCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackPdgBinCut();
  ClassDef(NicaTrackPdgBinCut, 1);
};

#endif /* NICATRACKPDGBINCUT_H_ */
