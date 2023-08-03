/*
 * NicaEventVirtualCut.h
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAEVENTCUTVIRTUAL_H_
#define NICAEVENTCUTVIRTUAL_H_

#include "NicaEventCut.h"
/**
 * virtual cut that pass all events  and doesn't compute any values
 */
class NicaEventVirtualCut : public NicaEventCut {
 public:
  /**
   * default constructor
   * @param i - number of monitored parameters
   */
  NicaEventVirtualCut();
  virtual Bool_t Pass(NicaEvent* /*event*/) {
    ForcedUpdate(kTRUE);
    return kTRUE;
  };
  virtual ~NicaEventVirtualCut();
  ClassDef(NicaEventVirtualCut, 1)
};

#endif /* NICAEVENTCUTVIRTUAL_H_ */
