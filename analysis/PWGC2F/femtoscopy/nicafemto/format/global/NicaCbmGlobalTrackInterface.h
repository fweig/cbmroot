/*
 * NicaCbmTrackInterface.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMGLOBTRACKINTERFACE_H_
#define NICACBMGLOBTRACKINTERFACE_H_

#include "CbmStsTrack.h"
#include "NicaTrackInterface.h"
class NicaCbmGlobalTrackInterface : public NicaTrackInterface {
public:
  NicaCbmGlobalTrackInterface();
  virtual ~NicaCbmGlobalTrackInterface();
  ClassDef(NicaCbmGlobalTrackInterface, 1)
};

#endif /* NICACBMGLOBTRACKINTERFACE_H_ */
