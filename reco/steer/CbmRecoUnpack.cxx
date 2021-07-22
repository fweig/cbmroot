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
  if (fTrd1DConfig) fTrd1DConfig->GetUnpacker()->Finish();
  if (fTrd2DConfig) fTrd2DConfig->GetUnpacker()->Finish();
}

// ----------------------------------------------------------------------------

// -----   Initialisation   ---------------------------------------------------
Bool_t CbmRecoUnpack::Init()
{

  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);


  auto eh = FairRun::Instance()->GetEventHeader();
  if (eh->IsA() == CbmTsEventHeader::Class()) fCbmTsEventHeader = static_cast<CbmTsEventHeader*>(eh);
  else
    LOG(fatal)
      << "CbmRecoUnpack::Init() no CbmTsEventHeader was added to the run. Without it, we can not store the UTC of the "
         "Timeslices correctly. Hence, this causes a fatal. Please add it in the steering macro to the Run.";


  // --- Psd
  if (fPsdConfig) fPsdConfig->Init(ioman);
  // --- Rich
  if (fRichConfig) fRichConfig->Init(ioman);
  // --- Sts
  if (fStsConfig) fStsConfig->Init(ioman);
  // --- Tof
  if (fTofConfig) fTofConfig->Init(ioman);
  // --- Trd
  if (fTrd1DConfig) fTrd1DConfig->Init(ioman);
  // --- TRD2D
  if (fTrd2DConfig) {
    if (fTrd1DConfig) {
      if (fTrd2DConfig->GetOutputBranchName() == fTrd1DConfig->GetOutputBranchName()) {
        LOG(info) << fTrd2DConfig->GetName() << "::Init() ---------------------------------";
        fTrd2DConfig->SetOutputVec(fTrd1DConfig->GetOutputVec());
        fTrd2DConfig->InitUnpacker();
        LOG(info) << fTrd2DConfig->GetName() << " succesful initialized -----------------\n";
      }
      else {
        fTrd2DConfig->Init(ioman);
      }
    }
    else {
      fTrd2DConfig->Init(ioman);
    }
  }
  // This is an ugly work around, because the TRD and TRD2D want to access the same vector and there is no function to retrieve a writeable vector<obj> from the FairRootManager, especially before the branches are created, as far as I am aware. The second option workaround is in in Init() to look for the fasp config and create a separate branch for fasp created CbmTrdDigis PR 072021

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
  if (fTofConfig) fTofConfig->Reset();
  // ---- Trd ----
  if (fTrd1DConfig) fTrd1DConfig->Reset();
  // ---- Trd2D ----
  if (fTrd2DConfig) fTrd2DConfig->Reset();
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
      case fkFlesTof: {
        if (fTofConfig)
          fCbmTsEventHeader->SetNDigisTof(
            unpack(&timeslice, component, fTofConfig, fTofConfig->GetOptOutAVec(), fTofConfig->GetOptOutBVec()));
        break;
      }
      case fkFlesTrd: {
        if (fTrd1DConfig)
          fCbmTsEventHeader->SetNDigisTrd(
            unpack(&timeslice, component, fTrd1DConfig, fTrd1DConfig->GetOptOutAVec(), fTrd1DConfig->GetOptOutBVec()));
        break;
      }
      case fkFlesTrd2D: {
        if (fTrd2DConfig)
          fCbmTsEventHeader->SetNDigisTrd2D(
            unpack(&timeslice, component, fTrd2DConfig, fTrd2DConfig->GetOptOutAVec(), fTrd2DConfig->GetOptOutBVec()));
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
