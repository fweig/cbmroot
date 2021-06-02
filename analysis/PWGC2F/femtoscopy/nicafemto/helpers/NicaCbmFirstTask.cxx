/** @file NicaCbmFirstTask.cxx
  * @copyright Copyright (C) 2019-2020 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer], Daniel Wielanek [committer] **/

/*
 * NicaCbmFirstTask.cxx
 *
 *  Created on: 3 wrz 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmFirstTask.h"

#include "FairRunAna.h"

NicaCbmFirstTask::NicaCbmFirstTask() : fMagField(nullptr) {}

InitStatus NicaCbmFirstTask::Init()
{
  if (fMagField != nullptr) {
    CbmHelix::SetField(fMagField);
    std::cout << "SET FIELD " << fMagField << std::endl;
  }
  else {
    CbmHelix::SetField(FairRunAna::Instance()->GetField());
  }
  return kSUCCESS;
}

NicaCbmFirstTask::~NicaCbmFirstTask() {}
