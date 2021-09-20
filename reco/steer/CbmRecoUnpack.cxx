/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Pierre-Alain Loizeau, Pascal Raisig  */

/** @file CbmRecoUnpack.cxx
 ** @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
 ** @license SPDX-License-Identifier: GPL-3.0-only
 ** @author Volker Friese [originator]
 **/


#include "CbmRecoUnpack.h"

#include "CbmRecoUnpackConfig.tmpl"
#include "CbmTrdDigi.h"
#include "CbmTsEventHeader.h"

#include <FairRootManager.h>
#include <Logger.h>

#include <RtypesCore.h>
#include <TH1.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>


using fles::Timeslice;
using std::unique_ptr;


// -----   Constructor   ------------------------------------------------------
CbmRecoUnpack::CbmRecoUnpack() {}
// ----------------------------------------------------------------------------


// -----   Finish   -----------------------------------------------------------
void CbmRecoUnpack::Finish()
{
  LOG(info) << "CbmRecoUnpack::Finish() I do let the unpackers talk first :\n";

  if (fPsdConfig) fPsdConfig->GetUnpacker()->Finish();
  if (fRichConfig) fRichConfig->GetUnpacker()->Finish();
  if (fStsConfig) fStsConfig->GetUnpacker()->Finish();
  if (fTofConfig) fTofConfig->GetUnpacker()->Finish();
  if (fTrd1DConfig) fTrd1DConfig->GetUnpacker()->Finish();
  if (fTrd2DConfig) fTrd2DConfig->GetUnpacker()->Finish();

  // Create some default performance profiling histograms and write them to a file
  if (fDoPerfProf) performanceProfiling();
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
  if (fPsdConfig) {
    fPsdConfig->Init(ioman);
    initPerformanceMaps(fkFlesPsd, "PSD");
  }
  // --- Rich
  if (fRichConfig) {
    fRichConfig->Init(ioman);
    initPerformanceMaps(fkFlesRich, "RICH");
  }

  // --- Sts
  if (fStsConfig) {
    fStsConfig->Init(ioman);
    initPerformanceMaps(fkFlesSts, "STS");
  }
  // --- Tof
  if (fTofConfig) {
    fTofConfig->Init(ioman);
    initPerformanceMaps(fkFlesTof, "TOF");
  }
  // --- Trd
  if (fTrd1DConfig) {
    fTrd1DConfig->Init(ioman);
    initPerformanceMaps(fkFlesTrd, "TRD1D");
  }
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
    initPerformanceMaps(fkFlesTrd2D, "TRD2D");
  }
  // This is an ugly work around, because the TRD and TRD2D want to access the same vector and there is no function to retrieve a writeable vector<obj> from the FairRootManager, especially before the branches are created, as far as I am aware. The second option workaround is in in Init() to look for the fasp config and create a separate branch for fasp created CbmTrdDigis PR 072021

  return kTRUE;
}
// ----------------------------------------------------------------------------


// -----   initPerformanceMaps   ----------------------------------------------
void CbmRecoUnpack::initPerformanceMaps(std::uint16_t subsysid, std::string name)
{
  if (fDoPerfProf) {
    fNameMap.emplace(std::make_pair(subsysid, std::make_pair(name, 0)));
    fTimeMap.emplace(std::make_pair(subsysid, std::make_pair(0, 0)));
    fDataSizeMap.emplace(std::make_pair(subsysid, std::make_pair(0, 0)));
  }
}
// ----------------------------------------------------------------------------


// -----   performanceProfiling   ---------------------------------------------
void CbmRecoUnpack::performanceProfiling()
{
  std::unique_ptr<TH1D> hProducedDigis =
    std::unique_ptr<TH1D>(new TH1D("ProducedDigis", "ProducedDigis", fNameMap.size(), -0.5, fNameMap.size() - 0.5));
  hProducedDigis->SetXTitle("Subsystem");
  hProducedDigis->SetYTitle("N-Digis");
  std::unique_ptr<TH1D> hSpeedPerf = std::unique_ptr<TH1D>(
    new TH1D("SpeedPerformance", "SpeedPerformance", fNameMap.size() * 2, -0.5, fNameMap.size() * 2 - 0.5));
  hSpeedPerf->SetXTitle("Subsystem");
  hSpeedPerf->SetYTitle("Unpacking performance [#mus/Digi]");
  std::unique_ptr<TH1D> hDataPerf = std::unique_ptr<TH1D>(
    new TH1D("DataPerformance", "DataPerformance", fNameMap.size() * 2, -0.5, fNameMap.size() * 2 - 0.5));
  hDataPerf->SetXTitle("Subsystem");
  hDataPerf->SetYTitle("Data [MB]");
  size_t iunpackerbin = 1;
  for (auto namepair : fNameMap) {

    // Speed performance
    auto timeit = fTimeMap.find(namepair.first);
    double cpu  = timeit->second.first / namepair.second.second;
    double wall = timeit->second.second / namepair.second.second;

    // Data performance
    auto datait    = fDataSizeMap.find(namepair.first);
    double indata  = datait->second.first;
    double outdata = datait->second.second;


    // N-Digis
    std::string label = namepair.second.first;
    hProducedDigis->GetXaxis()->SetBinLabel(iunpackerbin, label.data());
    hProducedDigis->SetBinContent(iunpackerbin, namepair.second.second);

    // Cpu time
    label = namepair.second.first;
    label += " cpu";
    hSpeedPerf->GetXaxis()->SetBinLabel(iunpackerbin * 2 - 1, label.data());
    hSpeedPerf->SetBinContent(iunpackerbin * 2 - 1, cpu);
    // Wall time
    label = namepair.second.first;
    label += " wall";
    hSpeedPerf->GetXaxis()->SetBinLabel(iunpackerbin * 2, label.data());
    hSpeedPerf->SetBinContent(iunpackerbin * 2, wall);

    // In data
    label = namepair.second.first;
    label += " in";
    hDataPerf->GetXaxis()->SetBinLabel(iunpackerbin * 2 - 1, label.data());
    hDataPerf->SetBinContent(iunpackerbin * 2 - 1, indata);

    // Out data
    label = namepair.second.first;
    label += " out";
    hDataPerf->GetXaxis()->SetBinLabel(iunpackerbin * 2, label.data());
    hDataPerf->SetBinContent(iunpackerbin * 2, outdata);

    ++iunpackerbin;
  }

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  /// (Re-)Create ROOT file to store the histos
  TFile histofile(fOutfilename.data(), "RECREATE");

  histofile.cd();
  hProducedDigis->Write();
  hSpeedPerf->Write();
  hDataPerf->Write();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  // histofile->Close();
  histofile.Close();
}
// ----------------------------------------------------------------------------

// -----   Reset   ------------------------------------------------------------
void CbmRecoUnpack::Reset()
{
  // Reset the event header for a new timeslice
  fCbmTsEventHeader->Reset();

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

  fCbmTsEventHeader->SetTsIndex(ts->index());
  fCbmTsEventHeader->SetTsStartTime(ts->start_time());

  uint64_t nComponents = ts->num_components();
  // if (fDoDebugPrints) LOG(info) << "Unpack: TS index " << ts->index() << " components " << nComponents;
  LOG(info) << "Unpack: TS index " << ts->index() << " components " << nComponents;

  for (uint64_t component = 0; component < nComponents; component++) {

    auto systemId = static_cast<std::uint16_t>(ts->descriptor(component, 0).sys_id);

    switch (systemId) {
      case fkFlesPsd: {
        if (fPsdConfig) {
          fCbmTsEventHeader->AddNDigisPsd(unpack(systemId, &timeslice, component, fPsdConfig,
                                                 fPsdConfig->GetOptOutAVec(), fPsdConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesRich: {
        if (fRichConfig) {
          fCbmTsEventHeader->AddNDigisRich(unpack(systemId, &timeslice, component, fRichConfig,
                                                  fRichConfig->GetOptOutAVec(), fRichConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesSts: {
        if (fStsConfig) {
          fCbmTsEventHeader->AddNDigisSts(unpack(systemId, &timeslice, component, fStsConfig,
                                                 fStsConfig->GetOptOutAVec(), fStsConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesTof: {
        if (fTofConfig) {
          fCbmTsEventHeader->AddNDigisTof(unpack(systemId, &timeslice, component, fTofConfig,
                                                 fTofConfig->GetOptOutAVec(), fTofConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesTrd: {
        if (fTrd1DConfig) {
          fCbmTsEventHeader->AddNDigisTrd1D(unpack(systemId, &timeslice, component, fTrd1DConfig,
                                                   fTrd1DConfig->GetOptOutAVec(), fTrd1DConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesTrd2D: {
        if (fTrd2DConfig) {
          fCbmTsEventHeader->AddNDigisTrd2D(unpack(systemId, &timeslice, component, fTrd2DConfig,
                                                   fTrd2DConfig->GetOptOutAVec(), fTrd2DConfig->GetOptOutBVec()));
        }
        break;
      }
      default: {
        if (fDoDebugPrints) LOG(error) << "Unpack: Unknown system ID " << systemId << " for component " << component;
        break;
      }
    }
  }

  if (bOutputFullTimeSorting) {
    /// Time sort the output vectors of all unpackers present
    if (fPsdConfig && fPsdConfig->GetOutputVec()) { timesort(fPsdConfig->GetOutputVec()); }
    if (fRichConfig && fRichConfig->GetOutputVec()) { timesort(fRichConfig->GetOutputVec()); }
    if (fStsConfig && fStsConfig->GetOutputVec()) { timesort(fStsConfig->GetOutputVec()); }
    if (fTofConfig && fTofConfig->GetOutputVec()) { timesort(fTofConfig->GetOutputVec()); }
    if (fTrd1DConfig && fTrd1DConfig->GetOutputVec()) { timesort(fTrd1DConfig->GetOutputVec()); }
    if (fTrd2DConfig && fTrd2DConfig->GetOutputVec()) { timesort(fTrd2DConfig->GetOutputVec()); }

    /// Time sort the output vectors of all unpackers present
    if (fPsdConfig && fPsdConfig->GetOptOutAVec()) { timesort(fPsdConfig->GetOptOutAVec()); }
    if (fRichConfig && fRichConfig->GetOptOutAVec()) { timesort(fRichConfig->GetOptOutAVec()); }
    if (fStsConfig && fStsConfig->GetOptOutAVec()) { timesort(fStsConfig->GetOptOutAVec()); }
    if (fTofConfig && fTofConfig->GetOptOutAVec()) { timesort(fTofConfig->GetOptOutAVec()); }
    if (fTrd1DConfig && fTrd1DConfig->GetOptOutAVec()) { timesort(fTrd1DConfig->GetOptOutAVec()); }
    if (fTrd2DConfig && fTrd2DConfig->GetOptOutAVec()) { timesort(fTrd2DConfig->GetOptOutAVec()); }
  }
}
// ----------------------------------------------------------------------------


ClassImp(CbmRecoUnpack)
