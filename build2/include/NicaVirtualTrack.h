/*
 * NicaTrackVirtual.h
 *
 *  Created on: 4 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAVIRTUALTRACK_H_
#define NICAVIRTUALTRACK_H_
#include "NicaTrack.h"
class NicaVirtualTrack : public NicaTrack {
 public:
  NicaVirtualTrack();
  virtual ~NicaVirtualTrack();
  ClassDef(NicaVirtualTrack, 1)
};

#endif /* NICAVIRTUALTRACK_H_ */
