/*
 * NicaTwoTrackVirtualCut.h
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATWOTRACKVIRTUALCUT_H_
#define NICATWOTRACKVIRTUALCUT_H_

#include "NicaTwoTrackCut.h"
/**
 * virtual cut that pass all pairs of tracks and doesn't compute any values
 */
class NicaTwoTrackVirtualCut : public NicaTwoTrackCut {
 protected:
 public:
  NicaTwoTrackVirtualCut();
  virtual Bool_t Pass(NicaTwoTrack* /*pair*/) {
    ForcedUpdate(kTRUE);
    return kTRUE;
  };
  virtual ~NicaTwoTrackVirtualCut();
  ClassDef(NicaTwoTrackVirtualCut, 1)
};

#endif /* NICATWOTRACKVIRTUALCUT_H_ */
