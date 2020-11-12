/*
 * NicaCbmTrackInterface.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATRECOTRACKINTERFACE_H_
#define NICACBMATRECOTRACKINTERFACE_H_

#include "CbmStsTrack.h"
#include "NicaCbmGlobalTrackInterface.h"
#include "NicaTrackInterface.h"
class NicaCbmATTrackInterface : public NicaTrackInterface {
public:
  NicaCbmATTrackInterface();
  virtual ~NicaCbmATTrackInterface();
  ClassDef(NicaCbmATTrackInterface, 1)
};

#endif /* NICACBMATTRACKINTERFACE_H_ */
