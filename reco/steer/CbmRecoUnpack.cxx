/** @file CbmRecoUnpack.cxx
 ** @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
 ** @license SPDX-License-Identifier: GPL-3.0-only
 ** @author Volker Friese [originator]
 **/


#include "CbmRecoUnpack.h"

#include "MicrosliceDescriptor.hpp"

#include <FairRootManager.h>
#include <Logger.h>

#include <utility>
#include <vector>


using fles::Timeslice;
using std::unique_ptr;


// -----   Constructor   ------------------------------------------------------
CbmRecoUnpack::CbmRecoUnpack() {}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
void CbmRecoUnpack::Finish()
{
  LOG(info) << "CbmRecoUnpack::Finish() I do let the unpackers talk first : ";
  fTrdConfig->GetUnpacker()->Finish();
}

// ----------------------------------------------------------------------------

// -----   Initialisation   ---------------------------------------------------
Bool_t CbmRecoUnpack::Init()
{

  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- TRD
  fTrdConfig->Init(ioman);
  fTrdConfig->InitUnpacker();

  return kTRUE;
}
// ----------------------------------------------------------------------------


// -----   Reset   ------------------------------------------------------------
void CbmRecoUnpack::Reset()
{
  // Reset the unpackers for a new timeslice, e.g. clear the output vectors

  // ---- Trd ----
  fTrdConfig->Reset();
}

// ----------------------------------------------------------------------------

// -----   Unpacking   --------------------------------------------------------
void CbmRecoUnpack::Unpack(unique_ptr<Timeslice> ts)
{
  // Prepare timeslice
  const fles::Timeslice& timeslice = *ts;

  uint64_t nComponents = ts->num_components();
  LOG(info) << "Unpack: TS index " << ts->index() << " components " << nComponents;

  for (uint64_t component = 0; component < nComponents; component++) {

    auto systemId = static_cast<std::uint16_t>(ts->descriptor(component, 0).sys_id);

    switch (systemId) {
      case fkFlesTrd: {
        if (fTrdConfig)
          unpack(&timeslice, component, fTrdConfig, fTrdConfig->GetOptOutAVec(), fTrdConfig->GetOptOutBVec());
        break;
      };

      default: {
        if (fDoDebugPrints) LOG(error) << "Unpack: Unknown system ID " << systemId << " for component " << component;
        break;
      }
    }
  }
}
// ----------------------------------------------------------------------------


ClassImp(CbmRecoUnpack)
