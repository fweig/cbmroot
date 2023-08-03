/*
 * NicaDetectorEvent.h
 *
 *  Created on: 6 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICADETECTOREVENT_H_
#define NICADETECTOREVENT_H_

#include <TObject.h>

#include "NicaDataFormat.h"
/**
 * abstract class for holding information about event from given type of
 * detector
 */
class NicaDetectorEvent : public TObject {
public:
  NicaDetectorEvent();
  virtual ~NicaDetectorEvent();
  ClassDef(NicaDetectorEvent, 1)
};

#endif /* NICADETECTOREVENT_H_ */
