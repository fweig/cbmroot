/** @file CbmHbtFullEvent.h
  * @copyright Copyright (C) 2020 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * NicaCbmHbtEvent.h
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMHBTFULLEVENT_H_
#define CBMHBTFULLEVENT_H_

#include "NicaComplexEvent.h"

/**
 * class for ana tree data and HBT analysis
 */

class CbmHbtFullEvent : public NicaComplexEvent {
public:
  CbmHbtFullEvent();
  virtual void Update() {};  // we are never reading directly from tree
  virtual TString GetFormatName() const { return "NicaCbmHbtEvent"; };
  virtual NicaEvent* GetNewEvent() const { return new CbmHbtFullEvent(); };
  //   virtual Bool_t IsCompatible(const NicaEvent *non_buffered) const;
  virtual ~CbmHbtFullEvent();
  ClassDef(CbmHbtFullEvent, 1)
};

#endif /* CBMHBTFULLEVENT_H_ */
