/*
 * NicaCbmTrackInterface.h
 *
 *  Created on: 11 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMTRACKINTERFACE_H_
#define NICACBMTRACKINTERFACE_H_

#include "CbmLink.h"
#include "CbmMuchTrack.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTofTrack.h"
#include "CbmTrdTrack.h"
#include "NicaCbmGlobalTrack.h"
#include "NicaCbmGlobalTrackInterface.h"

class NicaCbmTrackInterface : public NicaCbmGlobalTrackInterface {
  CbmStsTrack* fStsTrack;
  CbmTofHit* fTofTrack;
  CbmTrdTrack* fTrdTrack;
  CbmRichRing* fRichRing;
  CbmMuchTrack* fMuchTrack;

public:
  NicaCbmTrackInterface();
  void SetStsTrack(CbmStsTrack* tr) { fStsTrack = tr; };
  void SetTofHit(CbmTofHit* tr) { fTofTrack = tr; }
  void SetTrdTrack(CbmTrdTrack* tr) { fTrdTrack = tr; };
  void SetRichRing(CbmRichRing* tr) { fRichRing = tr; };
  void SetMuchTrack(CbmMuchTrack* tr) { fMuchTrack = tr; }
  CbmStsTrack* GetStsTrack() const { return fStsTrack; };
  CbmTofHit* GetTofHit() const { return fTofTrack; };
  CbmRichRing* GetRichRing() const { return fRichRing; };
  CbmMuchTrack* GetMuchTrack() const { return fMuchTrack; };
  CbmTrdTrack* GetTrdTrack() const { return fTrdTrack; }
  virtual ~NicaCbmTrackInterface();
  ClassDef(NicaCbmTrackInterface, 1)
};

#endif /* NICACBMTRACKINTERFACE_H_ */
