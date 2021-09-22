/* Copyright (C) 2019-2020 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaCbmFirstTask.h
 *
 *  Created on: 3 wrz 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMFIRSTTASK_H_
#define NICACBMFIRSTTASK_H_
#include "CbmHelix.h"

#include "FairField.h"
#include "FairTask.h"

class NicaCbmFirstTask : public FairTask {
  FairField* fMagField;

public:
  NicaCbmFirstTask();
  void SetMagField(FairField* f) { fMagField = f; };
  InitStatus Init();
  void Exec(Option_t* /*opt*/) {};
  virtual ~NicaCbmFirstTask();
  ClassDef(NicaCbmFirstTask, 1)
};

#endif /* CBM_ANALYSIS_NICA_CBM_HELPERS_NICACBMFIRSTTASK_H_ */
