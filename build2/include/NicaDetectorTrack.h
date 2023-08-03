/*
 * NicaDetectorTrack.h
 *
 *  Created on: 6 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICADETECTORTRACK_H_
#define NICADETECTORTRACK_H_

#include <TObject.h>


/**
 * class for holding information about information about track from single
 * detector
 */
class NicaDetectorTrack : public TObject {
 public:
  NicaDetectorTrack();
  virtual ~NicaDetectorTrack();
  ClassDef(NicaDetectorTrack, 1)
};

#endif /* NICADETECTORTRACK_H_ */
