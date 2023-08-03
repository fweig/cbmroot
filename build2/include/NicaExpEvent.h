/*
 * NicaDetectedEvent.h
 *
 *  Created on: 10 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAEXPEVENT_H_
#define NICAEXPEVENT_H_
#include <TString.h>

#include "NicaEvent.h"

class NicaDetectorEvent;

/**
 * basic class for representation of detected event
 */
class NicaExpEvent : public NicaEvent {
protected:
  Int_t fRunInfoId;
  Int_t fEventTrigger;
  TVector3* fMagField;
  TLorentzVector* fVertexError;
  virtual void ShallowCopyEvent(NicaEvent* event);
  NicaExpEvent(TString track_class, TString v0_class = "NicaV0Track");

public:
  NicaExpEvent();
  NicaExpEvent(const NicaExpEvent& other);
  /**
   *
   * @return NicaDetectorEvent
   */
  virtual NicaDetectorEvent* GetDetEvent() const { return NULL; };
  TLorentzVector* GetVertexError() const { return fVertexError; };
  TVector3* GetMagField() const { return fMagField; };
  virtual ~NicaExpEvent();
  ClassDef(NicaExpEvent, 1)
};

class NicaExpEventHelix : public NicaExpEvent {
protected:
  NicaExpEventHelix(TString track_class, TString v0_class = "NicaV0Track");

public:
  NicaExpEventHelix();
  NicaExpEventHelix(const NicaExpEventHelix& other);
  virtual ~NicaExpEventHelix();
};
#endif /* NICAEXPEVENT_H_ */
