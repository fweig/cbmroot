/** @file CbmRecoUnpack.cxx
 ** @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
 ** @license SPDX-License-Identifier: GPL-3.0-only
 ** @author Volker Friese [originator]
 **/


#include "CbmRecoUnpack.h"

#include "CbmTrdDigi.h"
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


// -----   Initialisation   ---------------------------------------------------
void CbmRecoUnpack::Finish()
{
  LOG(info) << "CbmRecoUnpack::Finish() I do let the unpackers talk first :\n";

  if (fPsdConfig) fPsdConfig->GetUnpacker()->Finish();
  if (fRichConfig) fRichConfig->GetUnpacker()->Finish();
  if (fStsConfig) fStsConfig->GetUnpacker()->Finish();
  if (fTofConfig) fTofConfig->GetUnpacker()->Finish();
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
  LOG(info) << "CbmRecoUnpack::Init() registered CbmTsEventHeader to output tree!\n";


  // --- Psd
  if (fPsdConfig) fPsdConfig->Init(ioman);
  // --- Rich
  if (fRichConfig) fRichConfig->Init(ioman);
  // --- Sts
  if (fStsConfig) fStsConfig->Init(ioman);
  // --- Tof
  // if (fTofConfig) fTofConfig->Init(ioman);
  // --- Trd
  if (fTrdConfig) fTrdConfig->Init(ioman);
  // --- TRD2D
  if (fTrdConfig2D->GetOutputBranchName() == fTrdConfig->GetOutputBranchName()) {
    fTrdConfig2D->SetOutputVec(fTrdConfig->GetOutputVec());
    if (fTrdConfig2D) fTrdConfig2D->InitUnpacker();
  }
  else {
    if (fTrdConfig2D) fTrdConfig2D->Init(ioman);
  }
  // This is an ugly work around, because the TRD and TRD2D want to access the same vector and there is no function to retrieve a writeable vector<obj> from the FairRootManager, especially before the branches are created, as far as I am aware. The second option workaround is in in Init() to look for the fasp config and create a separate branch for fasp created CbmTrdDigis PR 072021
  // --- Tof
  if (fTofConfig) fTofConfig->Init(ioman);

  return kTRUE;
}
// ----------------------------------------------------------------------------


// -----   Reset   ------------------------------------------------------------
void CbmRecoUnpack::Reset()
{
  // Reset the unpackers for a new timeslice, e.g. clear the output vectors

  // ---- Psd ----
  if (fPsdConfig) fPsdConfig->Reset();
  // ---- Rich ----
  if (fRichConfig) fRichConfig->Reset();
  // ---- Sts ----
  if (fStsConfig) fStsConfig->Reset();
  // ---- Tof ----
  // if (fTofConfig) fTofConfig->Reset();
  // ---- Trd ----
  if (fTrdConfig) fTrdConfig->Reset();
  // ---- Trd2D ----
  if (fTrdConfig2D) fTrdConfig2D->Reset();
  // ---- Tof ----
  if (fTofConfig) fTofConfig->Reset();
}

// ----------------------------------------------------------------------------

// -----   Unpacking   --------------------------------------------------------
void CbmRecoUnpack::Unpack(unique_ptr<Timeslice> ts)
{
  // Prepare timeslice
  const fles::Timeslice& timeslice = *ts;

  fCbmTsEventHeader->SetTsStartTime(ts->start_time());

  uint64_t nComponents = ts->num_components();
  // if (fDoDebugPrints) LOG(info) << "Unpack: TS index " << ts->index() << " components " << nComponents;
  LOG(info) << "Unpack: TS index " << ts->index() << " components " << nComponents;

  for (uint64_t component = 0; component < nComponents; component++) {

    auto systemId = static_cast<std::uint16_t>(ts->descriptor(component, 0).sys_id);

    switch (systemId) {
      case fkFlesPsd: {
        if (fPsdConfig)
          fCbmTsEventHeader->SetNDigisPsd(
            unpack(&timeslice, component, fPsdConfig, fPsdConfig->GetOptOutAVec(), fPsdConfig->GetOptOutBVec()));
        break;
      }
      case fkFlesRich: {
        if (fRichConfig)
          fCbmTsEventHeader->SetNDigisRich(
            unpack(&timeslice, component, fRichConfig, fRichConfig->GetOptOutAVec(), fRichConfig->GetOptOutBVec()));
        break;
      }
      case fkFlesSts: {
        if (fStsConfig)
          fCbmTsEventHeader->SetNDigisSts(
            unpack(&timeslice, component, fStsConfig, fStsConfig->GetOptOutAVec(), fStsConfig->GetOptOutBVec()));
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
      case fkFlesTof: {
        if (fTofConfig)
          fCbmTsEventHeader->SetNDigisTof(
            unpack(&timeslice, component, fTofConfig, fTofConfig->GetOptOutAVec(), fTofConfig->GetOptOutBVec()));
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
