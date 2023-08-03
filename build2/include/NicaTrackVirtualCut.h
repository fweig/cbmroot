/*
 * NicaTrackVirtualCut.h
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATRACKVIRTUALCUT_H_
#define NICATRACKVIRTUALCUT_H_

#include "NicaTrackCut.h"
/**
 * virtual cut that pass all tracks and doesn't compute any values
 */
class NicaTrackVirtualCut : public NicaTrackCut {
 public:
  /**
   * default constructor
   * @param i - number of monitored parameters
   */
  NicaTrackVirtualCut();
  virtual Bool_t Pass(NicaTrack* /*track*/) {
    ForcedUpdate(kTRUE);
    return kTRUE;
  }
  virtual ~NicaTrackVirtualCut();
  ClassDef(NicaTrackVirtualCut, 1)
};

#endif /* NICATRACKVIRTUALCUT_H_ */
