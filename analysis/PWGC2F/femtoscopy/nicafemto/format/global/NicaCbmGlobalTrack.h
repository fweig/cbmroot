/*
 * NicaCbmTrack.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMGLOBTRACK_H_
#define NICACBMGLOBTRACK_H_

#include "CbmGlobalTrack.h"
#include "CbmHelix.h"
#include "NicaCbmGlobalEventInterface.h"
#include "NicaExpTrack.h"
#include "NicaToFTrack.h"

class NicaCbmGlobalTrackInterface;
class NicaCbmGlobalTrack : public NicaExpTrack {
  TVector3 *fFirstPoint, *fLastPoint;
  CbmHelix fHelix;

public:
  NicaCbmGlobalTrack();
  NicaCbmGlobalTrack(const NicaCbmGlobalTrack& other);
  NicaCbmGlobalTrack& operator=(const NicaCbmGlobalTrack& other);
  virtual void Update(CbmGlobalTrack* track,
                      NicaCbmGlobalEventInterface* interface);
  CbmHelix* GetHelix() { return &fHelix; };
  virtual void CopyData(NicaTrack* other);
  virtual ~NicaCbmGlobalTrack();
  ClassDef(NicaCbmGlobalTrack, 1)
};

#endif /* NICACBMGLOBTRACK_H_ */
