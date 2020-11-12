/*
 * NicaCbmTrack.h
 *
 *  Created on: 11 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMTRACK_H_
#define NICACBMTRACK_H_

#include "CbmMuchTrack.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTofTrack.h"
#include "CbmTrdTrack.h"
#include "NicaCbmGlobalTrack.h"
#include "NicaToFTrack.h"
class NicaCbmTrack : public NicaCbmGlobalTrack {
  CbmStsTrack* fStsTrack;
  CbmTofHit* fTofHit;
  CbmTrdTrack* fTrdTrack;
  CbmRichRing* fRichRing;
  CbmMuchTrack* fMuchTrack;
  NicaToFTrack* fTofTrack;
  Int_t fStsTrackIndex;
  Int_t fTofTrackIndex;
  Int_t fTrdTrackIndex;
  Int_t fRichRingIndex;
  Int_t fMuchTrackIndex;

public:
  NicaCbmTrack();
  virtual void Update(CbmGlobalTrack* track,
                      NicaCbmGlobalEventInterface* interface);
  virtual void CopyData(NicaTrack* track);
  CbmStsTrack* GetStsTrack() const { return fStsTrack; }
  CbmTofHit* GetTofHit() const { return fTofHit; }
  CbmTrdTrack* GetTrdTrack() const { return fTrdTrack; }
  CbmRichRing* GetRichRing() const { return fRichRing; };
  CbmMuchTrack* GetMuchTrack() const { return fMuchTrack; };
  Int_t GetStsTrackIndex() const { return fStsTrackIndex; }
  Int_t GetTofHitIndex() const { return fTofTrackIndex; }
  Int_t GetTrdTrackIndex() const { return fTrdTrackIndex; }
  Int_t GetRichRingIndex() const { return fRichRingIndex; };
  Int_t GetMuchIndex() const { return fMuchTrackIndex; };
  void SetStsTrackIndex(Int_t stsTrackId) { fStsTrackIndex = stsTrackId; }
  void SetTofTrackIndex(Int_t tofTrackId) { fTofTrackIndex = tofTrackId; }
  void SetTrdTrackIndex(Int_t trdTrackId) { fTrdTrackIndex = trdTrackId; }
  void SetRichRingIndex(Int_t no) { fRichRingIndex = no; };
  void SetMuchIndex(Int_t id) { fMuchTrackIndex = id; };
  void SetStsTrack(CbmStsTrack* stsTrack) { fStsTrack = stsTrack; }
  void SetTofHit(CbmTofHit* tofTrack) { fTofHit = tofTrack; }
  void SetTrdTrack(CbmTrdTrack* trdTrack) { fTrdTrack = trdTrack; }
  void SetRichRing(CbmRichRing* tr) { fRichRing = tr; };
  void SetMuchTrack(CbmMuchTrack* tr) { fMuchTrack = tr; };
  /** curently return only pointer to ToF **/
  virtual TObject* GetDetTrack(const UInt_t detID) const;
  virtual ~NicaCbmTrack();
  ClassDef(NicaCbmTrack, 1)
};

#endif /* NICACBMTRACK_H_ */
