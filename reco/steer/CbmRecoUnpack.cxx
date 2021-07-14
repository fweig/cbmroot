/** @file CbmRecoUnpack.cxx
 ** @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
 ** @license SPDX-License-Identifier: GPL-3.0-only
 ** @author Volker Friese [originator]
 **/


#include "CbmRecoUnpack.h"

#include "CbmTsEventHeader.h"

#include "MicrosliceDescriptor.hpp"

#include <FairRootManager.h>
#include <Logger.h>

#include <RtypesCore.h>

#include <cstddef>
#include <memory>
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

  if (fPsdConfig) fPsdConfig->GetUnpacker()->Finish();
  if (fRichConfig) fRichConfig->GetUnpacker()->Finish();
  if (fStsConfig) fStsConfig->GetUnpacker()->Finish();
  // if (fTofConfig) fTofConfig->GetUnpacker()->Finish();
  if (fTrdConfig) fTrdConfig->GetUnpacker()->Finish();
  if (fTrdConfig2D) fTrdConfig2D->GetUnpacker()->Finish();
}

// ----------------------------------------------------------------------------

// -----   Initialisation   ---------------------------------------------------
Bool_t CbmRecoUnpack::Init()
{

  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Register the branch for the Timeslice start time
  fCbmTsEventHeader = new CbmTsEventHeader();
  ioman->RegisterAny("TsEventHeader", fCbmTsEventHeader, kTRUE);
  LOG(info) << "CbmRecoUnpack::Init() registered CbmTsEventHeader to output tree!";


  // --- Psd
  if (fPsdConfig) fPsdConfig->Init(ioman);
  if (fPsdConfig) fPsdConfig->InitUnpacker();
  // --- Rich
  if (fRichConfig) fRichConfig->Init(ioman);
  if (fRichConfig) fRichConfig->InitUnpacker();
  // --- Sts
  if (fStsConfig) fStsConfig->Init(ioman);
  if (fStsConfig) fStsConfig->InitUnpacker();
  // --- Tof
  // if (fTofConfig) fTofConfig->Init(ioman);
  // if (fTofConfig) fTofConfig->InitUnpacker();

  // --- Trd
  if (fTrdConfig) fTrdConfig->Init(ioman);
  if (fTrdConfig) fTrdConfig->InitUnpacker();

  // --- TRD2D
  if (fTrdConfig2D) fTrdConfig2D->Init(ioman);
  if (fTrdConfig2D) fTrdConfig2D->InitUnpacker();

  return kTRUE;
}
// ----------------------------------------------------------------------------


// -----   Reset   ------------------------------------------------------------
void CbmRecoUnpack::Reset()
{
  // Reset the unpackers for a new timeslice, e.g. clear the output vectors

  // ---- Psd ----
  if (fPsdConfig) fPsdConfig->GetUnpacker()->Finish();
  // ---- Rich ----
  if (fRichConfig) fRichConfig->GetUnpacker()->Finish();
  // ---- Sts ----
  if (fStsConfig) fStsConfig->GetUnpacker()->Finish();
  // ---- Tof ----
  // if (fTofConfig) fTofConfig->GetUnpacker()->Finish();
  // ---- Trd ----
  if (fTrdConfig) fTrdConfig->GetUnpacker()->Finish();
  // ---- Trd2D ----
  if (fTrdConfig2D) fTrdConfig2D->GetUnpacker()->Finish();
}

// ----------------------------------------------------------------------------

// -----   Unpacking   --------------------------------------------------------
void CbmRecoUnpack::Unpack(unique_ptr<Timeslice> ts)
{
  // Prepare timeslice
  const fles::Timeslice& timeslice = *ts;

  fCbmTsEventHeader->SetTsStartTime(ts->start_time());

  uint64_t nComponents = ts->num_components();
  if (fDoDebugPrints) LOG(info) << "Unpack: TS index " << ts->index() << " components " << nComponents;

  for (uint64_t component = 0; component < nComponents; component++) {

    auto systemId = static_cast<std::uint16_t>(ts->descriptor(component, 0).sys_id);

    switch (systemId) {
      case fkFlesPsd: {
        if (fPsdConfig)
          fCbmTsEventHeader->SetNDigisTrd(
            unpack(&timeslice, component, fPsdConfig, fPsdConfig->GetOptOutAVec(), fPsdConfig->GetOptOutBVec()));
        break;
      }
      case fkFlesTrd: {
        if (fTrdConfig)
          fCbmTsEventHeader->SetNDigisTrd(
            unpack(&timeslice, component, fTrdConfig, fTrdConfig->GetOptOutAVec(), fTrdConfig->GetOptOutBVec()));
        break;
      }
      case fkFlesTrd2D: {
        if (fTrdConfig2D)
          fCbmTsEventHeader->SetNDigisTrd2D(
            unpack(&timeslice, component, fTrdConfig2D, fTrdConfig2D->GetOptOutAVec(), fTrdConfig2D->GetOptOutBVec()));
        break;
      }

      default: {
        if (fDoDebugPrints) LOG(error) << "Unpack: Unknown system ID " << systemId << " for component " << component;
        break;
      }
    }
  }
}
// ----------------------------------------------------------------------------


ClassImp(CbmRecoUnpack)
