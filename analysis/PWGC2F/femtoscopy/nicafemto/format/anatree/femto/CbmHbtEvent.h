/* Copyright (C) 2020 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * CbmHbtEventReco.h
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMHBTEVENT_H_
#define CBMHBTEVENT_H_

#include "NicaExpEvent.h"
/**
 * class for HBT analysis, note: CAN BE USED ONLY AS BUFFERED EVENT!
 */

class CbmHbtEvent : public NicaExpEvent {
protected:
  virtual void CreateSource() {};
  virtual void ShallowCopyTracks(NicaEvent* event);

public:
  CbmHbtEvent();
  virtual Bool_t IsCompatible(const NicaEvent* non_buffered) const;
  virtual NicaEvent* GetNewEvent() const { return new CbmHbtEvent(); };
  virtual ~CbmHbtEvent();
  ClassDef(CbmHbtEvent, 1)
};

#endif /* CBMHBTEVENT_H_ */
