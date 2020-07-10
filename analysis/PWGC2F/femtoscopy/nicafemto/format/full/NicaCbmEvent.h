/*
 * NicaCbmMatcheEvent.h
 *
 *  Created on: 11 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMMATCHEEVENT_H_
#define NICACBMMATCHEEVENT_H_

#include "CbmDetectorID.h"
#include "NicaCbmGlobalEvent.h"
#include "NicaTrackClones.h"
class NicaCbmEvent : public NicaCbmGlobalEvent {
public:
protected:
  NicaCbmSetup fMode;
  NicaTrackClones* fStsTracks;
  NicaTrackClones* fTrdTracks;
  NicaTrackClones* fTofHits;
  NicaTrackClones* fRichRings;
  NicaTrackClones* fMuchTracks;
  virtual void ShallowCopyTracks(NicaEvent* event);
  virtual void ShallowCopyEvent(NicaEvent* event);
  virtual void CreateSource();
  NicaCbmEvent(TString classname, NicaCbmSetup mode);

public:
  NicaCbmEvent(NicaCbmSetup mode = kSis100Hadron);
  NicaCbmSetup GetMode() const { return fMode; }
  virtual Bool_t ExistInTree() const;
  NicaTrackClones* GetStsTracks() const { return fStsTracks; }
  NicaTrackClones* GetTofHits() const { return fTofHits; }
  NicaTrackClones* GetTrdTracks() const { return fTrdTracks; }
  NicaTrackClones* GetRichRings() const { return fRichRings; };
  NicaTrackClones* GetMuchTracks() const { return fMuchTracks; };
  virtual void Update();
  virtual TString GetFormatName() const;
  virtual NicaEvent* GetNewEvent() const { return new NicaCbmEvent(fMode); };
  virtual ~NicaCbmEvent();
  ClassDef(NicaCbmEvent, 1)
};

#endif /* NICACBMMATCHEEVENT_H_ */
