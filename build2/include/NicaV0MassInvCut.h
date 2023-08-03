/*
 * NicaV0MassInvCut.h
 *
 *  Created on: 26 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_CUTS_TRACKCUTS_V0CUTS_NICAV0MASSINVCUT_H_
#define NICAFEMTO_CUTS_TRACKCUTS_V0CUTS_NICAV0MASSINVCUT_H_

#include "NicaTrackCut.h"

class NicaV0MassInvCut : public NicaTrackCut {
public:
  NicaV0MassInvCut();
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~NicaV0MassInvCut();
  ClassDef(NicaV0MassInvCut, 1)
};


#endif /* NICAFEMTO_CUTS_TRACKCUTS_V0CUTS_NICAV0MASSINVCUT_H_ */
