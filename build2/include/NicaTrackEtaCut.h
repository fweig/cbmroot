/*
 * NicaTrackEtaCut.h
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATRACKETACUT_H_
#define NICATRACKETACUT_H_

#include "NicaTrackCut.h"
/**
 * check track pseudorapidity
 */
class NicaTrackEtaCut : public NicaTrackCut {
 protected:
 public:
  NicaTrackEtaCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackEtaCut();
  ClassDef(NicaTrackEtaCut, 1)
};

/**
 * check absolute value of track pseudorapidity
 */
class NicaTrackEtaAbsCut : public NicaTrackCut {
 public:
  NicaTrackEtaAbsCut();
  virtual Bool_t Pass(NicaTrack *track);
  virtual Bool_t Init(Int_t task_id);
  virtual ~NicaTrackEtaAbsCut();
  ClassDef(NicaTrackEtaAbsCut, 1)
};

#endif /* NICAETACUT_H_ */
