/** @file CbmTrdParMod.cxx
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmTrdParMod.h"

#include <Logger.h>  // for LOG

//___________________________________________________________________
CbmTrdParMod::CbmTrdParMod(const char* name, const char* title) : TNamed(name, title), fModuleId(0) {}

//___________________________________________________________________
CbmTrdParMod::~CbmTrdParMod() { LOG(debug) << GetName() << "::delete[" << GetTitle() << "]"; }

ClassImp(CbmTrdParMod)
