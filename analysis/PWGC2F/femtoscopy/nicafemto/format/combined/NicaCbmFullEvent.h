/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaCbmATEvent.h
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATEVENT_H_
#define NICACBMATEVENT_H_
#include "NicaComplexEvent.h"

/**
 * class for analyses that base on anatree events
 */

class NicaCbmFullEvent : public NicaComplexEvent {
protected:
  NicaCbmFullEvent(NicaEvent* re, NicaEvent* im);

public:
  NicaCbmFullEvent();
  virtual void Update();
  virtual TString GetFormatName() const { return "NicaCbmFullEvent"; };
  virtual NicaEvent* GetNewEvent() const;
  virtual ~NicaCbmFullEvent();
  ClassDef(NicaCbmFullEvent, 1)
};

#endif /* NICACBMATEVENT_H_ */
