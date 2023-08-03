/*
 * NicaEventSmeared.h
 *
 *  Created on: 20 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICASMEAREDEVENT_H_
#define NICASMEAREDEVENT_H_

#include "NicaComplexEvent.h"

/**
 * class for reading events with smeared parameters
 */
class NicaSmearedEvent : public NicaComplexEvent {
  Bool_t fSmearing;
  Bool_t fRealMC;

 public:
  /**
   * default c-tor
   */
  NicaSmearedEvent();
  NicaSmearedEvent(const NicaSmearedEvent &other);
  /**
   * create non-allocated event that can be passed to analysis
   * @param event oryginal format
   */
  NicaSmearedEvent(NicaEvent *event);
  void LinkWithTree();
  void ActivateSmearing() { fSmearing = kTRUE; };
  virtual void Update();
  void Clear(Option_t *opt = " ");
  TString GetFormatName() const;
  virtual ~NicaSmearedEvent();
  ClassDef(NicaSmearedEvent, 1)
};

#endif /* NICASMEAREDEVENT_H_ */
