/*
 * NicaTrackMCCut.h
 *
 *  Created on: 5 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKMCCUT_H_
#define NICATRACKMCCUT_H_

#include "NicaTrackCut.h"

class NicaTrackMCCut : public NicaTrackCut {
 public:
  NicaTrackMCCut(const Int_t size = 1);
  virtual Bool_t Init(Int_t task_id);
  virtual ~NicaTrackMCCut();
  ClassDef(NicaTrackMCCut, 1)
};

#endif /* NICATRACKMCCUT_H_ */
