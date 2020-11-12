/*
 * NicaCbmATMCTrack.h
 *
 *  Created on: 21 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATMCTRACK_H_
#define NICACBMATMCTRACK_H_

#include "NicaMCTrack.h"
class NicaCbmATMCTrack : public NicaMCTrack {
public:
  NicaCbmATMCTrack();
  virtual ~NicaCbmATMCTrack();
  ClassDef(NicaCbmATMCTrack, 1)
};

#endif /* NICACBMATMCTRACK_H_ */
