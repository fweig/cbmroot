/*
 * NicaCbmATMMCTrackInterface.h
 *
 *  Created on: 21 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATMMCTRACKINTERFACE_H_
#define NICACBMATMMCTRACKINTERFACE_H_

#include "NicaMCTrackInterface.h"

class NicaCbmATMCTrackInterface : public NicaMCTrackInterface {
public:
  NicaCbmATMCTrackInterface();
  virtual ~NicaCbmATMCTrackInterface();
  ClassDef(NicaCbmATMCTrackInterface, 1)
};

#endif /* NICACBMATMMCTRACKINTERFACE_H_ */
