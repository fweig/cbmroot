/** @file CbmTrdModuleSim.cxx
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmTrdModuleSim.h"

#include "CbmTrdParModDigi.h"
#include "CbmTrdParModGain.h"
#include "CbmTrdParModGas.h"
#include "CbmTrdParSetAsic.h"

#include <Logger.h>

#include <vector>

//_______________________________________________________________________________
CbmTrdModuleSim::CbmTrdModuleSim()
  : CbmTrdModuleAbstract()
  , fPointId(-1)
  , fEventId(-1)
  , fInputId(-1)
  , fDigitizer(nullptr)
  , fRadiator(NULL)
  , fDigiMap()
  , fBuffer()
{
  memset(fXYZ, 0, 3 * sizeof(Double_t));
}

//_______________________________________________________________________________
CbmTrdModuleSim::CbmTrdModuleSim(Int_t mod, Int_t ly, Int_t rot)
  : CbmTrdModuleAbstract(mod, ly, rot)
  , fPointId(-1)
  , fEventId(-1)
  , fInputId(-1)
  , fDigitizer(nullptr)
  , fRadiator(NULL)
  , fDigiMap()
  , fBuffer()
{
  memset(fXYZ, 0, 3 * sizeof(Double_t));
}

//_______________________________________________________________________________
CbmTrdModuleSim::~CbmTrdModuleSim()
{
  LOG(debug) << GetName() << "::delete[" << GetTitle() << "]";
  //if(fAsicPar) delete fAsicPar;
}


ClassImp(CbmTrdModuleSim)
