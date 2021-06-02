/** @file CbmStsExitSepCut.h
  * @copyright Copyright (C) 2020 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * CbmStsExitSep.h
 *
 *  Created on: 23 mar 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef PAIRCUTS_CBMSTSEXITSEPCUT_H_
#define PAIRCUTS_CBMSTSEXITSEPCUT_H_

#include "NicaCbmFormatTypes.h"
#include "NicaTwoTrackCut.h"


class CbmStsExitSepCut : public NicaTwoTrackCut {
  NicaCbmFormats::ECbmFormatType fDataType;

public:
  CbmStsExitSepCut();
  virtual Bool_t Init(Int_t task_id = 0);
  Bool_t Pass(NicaTwoTrack* pair);
  virtual ~CbmStsExitSepCut();
  ClassDef(CbmStsExitSepCut, 1)
};

#endif /* PAIRCUTS_CBMSTSEXITSEPCUT_H_ */
