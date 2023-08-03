/*
 * NicaTrackPCut.h
 *
 *  Created on: 28 mar 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKPCUT_H_
#define NICATRACKPCUT_H_
#include "NicaTrackCut.h"
class NicaTrackPCut : public NicaTrackCut {
 public:
  NicaTrackPCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackPCut();
  ClassDef(NicaTrackPCut, 1)
};

class NicaTrackPSignedCut : public NicaTrackCut {
 public:
  NicaTrackPSignedCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackPSignedCut();
  ClassDef(NicaTrackPSignedCut, 1)
};

#endif /* NICATRACKPCUT_H_ */
