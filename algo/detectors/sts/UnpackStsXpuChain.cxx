/* Copyright (C) 2023 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */


#include "UnpackStsXpuChain.h"

#include "CbmDefs.h"
#include "CbmDigiBranchBase.h"
#include "CbmDigiEvent.h"
#include "CbmDigiTimeslice.h"

#include "MicrosliceDescriptor.hpp"

#include "AlgoFairloggerCompat.h"

#include <TStopwatch.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#ifdef WITH_EXECUTION
#include <execution>
#endif
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>

#include <xpu/host.h>

using namespace std;
using cbm::algo::UnpackStsXpuElinkPar;
using cbm::algo::UnpackStsXpuPar;

// -----   Constructor   -----------------------------------------------------
UnpackStsXpuChain::UnpackStsXpuChain() {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
UnpackStsXpuChain::~UnpackStsXpuChain()
{
}
// ---------------------------------------------------------------------------


// -----   Execution   -------------------------------------------------------
std::vector<CbmStsDigi> UnpackStsXpuChain::Exec(const fles::Timeslice& ts)
{

  assert(fStsConfig != std::nullopt);

  // --- Timer and counters
  TStopwatch timer;
  timer.Start();

  //Run STS unpacker and store result
  xpu::push_timer("Dominik Smiths XPU Unpacker");
  auto resultSts = fAlgoStsXpu(&ts, *fStsConfig);
  xpu::pop_timer();

  // --- Sorting of output digis. Is required by both digi trigger and event builder.
#ifdef WITH_EXECUTION
  std::sort(std::execution::par_unseq, resultSts.first.begin(), resultSts.first.end(),
            [](CbmStsDigi digi1, CbmStsDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
#else
  std::sort(resultSts.first.begin(), resultSts.first.end(),
            [](CbmStsDigi digi1, CbmStsDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
#endif

  // --- Timeslice log
  timer.Stop();
  stringstream logOut;
  logOut << setw(15) << left << "UnpackStsXpu()" << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << " (index " << ts.index() << ")";
  logOut << ", digis " << resultSts.first.size();
  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  // TO DO: implement these
  //fNumMs += numMs;
  //fNumBytes += numBytes;
  //fNumDigis += numDigis;
  fTime += timer.RealTime();

  L_(info) << "Timeslice contains " << resultSts.first.size() << " STS digis (discarded " << 0 << " pulser hits)";

  return resultSts.first;
}


// -----   End-of-run action   ------------------------------------------------
void UnpackStsXpuChain::Finish()
{
  double timePerTs = 1000. * fTime / double(fNumTs);  // in ms
  double rate      = fNumBytes / 1.e6 / fTime;        // in MB/s
  LOG(info) << "=====================================";
  LOG(info) << "UnpackStsXPU" << ": Run summary";
  LOG(info) << "Timeslices     : " << fNumTs;
  LOG(info) << "Microslices    : " << fNumMs;
  LOG(info) << "Digis          : " << fNumDigis;
  LOG(info) << "Av. input rate : " << fixed << setprecision(2) << rate << " MB/s";
  LOG(info) << "Time / TS      : " << fixed << setprecision(2) << timePerTs << " ms";
  LOG(info) << "=====================================";
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
void UnpackStsXpuChain::Init(const cbm::algo::StsReadoutConfig& stsConfig)
{
  LOG(info) << "==================================================";
  LOG(info) << "Initialising Dominik Smiths's XPU Unpacker";

  fStsConfig = stsConfig;

  // Initialize parameter buffers for STS
  auto equipIdsSts          = fStsConfig->GetEquipmentIds();
  const size_t numStsElinks = fStsConfig->GetNumElinks();
  const size_t numStsComps  = equipIdsSts.size();
  fAlgoStsXpu.fParams       = xpu::hd_buffer<UnpackStsXpuPar>(numStsComps);
  fAlgoStsXpu.fElinkParams  = xpu::hd_buffer<UnpackStsXpuElinkPar>(numStsElinks);

  // --- Common parameters for all components for STS
  uint32_t numChansPerAsicSts   = 128;  // R/O channels per ASIC for STS
  uint32_t numAsicsPerModuleSts = 16;   // Number of ASICs per module for STS

  // Fill parameter buffers for STS
  for (size_t comp = 0; comp < numStsComps; comp++) {
    auto equip                      = equipIdsSts[comp];
    auto params                     = fAlgoStsXpu.fParams.h();
    auto numElinks                  = fStsConfig->GetNumElinks(equip);
    params[comp].fNumElinks         = numElinks;
    params[comp].fNumChansPerAsic   = numChansPerAsicSts;
    params[comp].fNumAsicsPerModule = numAsicsPerModuleSts;
    params[comp].fElinkOffset       = (comp == 0) ? 0 : params[comp - 1].fElinkOffset + params[comp - 1].fNumElinks;
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackStsXpuElinkPar& elinkPar = fAlgoStsXpu.fElinkParams.h()[params[comp].fElinkOffset + elink];
      auto mapEntry                  = fStsConfig->Map(equip, elink);
      elinkPar.fAddress              = mapEntry.first;   // Module address for this elink
      elinkPar.fAsicNr               = mapEntry.second;  // ASIC number within module
      elinkPar.fTimeOffset           = 0.;
      elinkPar.fAdcOffset            = 1.;
      elinkPar.fAdcGain              = 1.;
    }
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }
  xpu::copy(fAlgoStsXpu.fParams, xpu::host_to_device);
  xpu::copy(fAlgoStsXpu.fElinkParams, xpu::host_to_device);
}
// ----------------------------------------------------------------------------
