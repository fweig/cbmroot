/* Copyright (C) 2021 Goethe-University, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Pascal Raisig [committer], Dominik Smith */

#include "CbmStsUnpackConfig.h"

#include "CbmStsUnpackAlgo.h"
#include "CbmStsUnpackAlgoLegacy.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmStsUnpackConfig::CbmStsUnpackConfig(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmStsUnpackConfig", detGeoSetupTag, runid)
{
}

CbmStsUnpackConfig::~CbmStsUnpackConfig()
{
  LOG(debug) << fName << "::~CbmStsUnpackConfig";
  fWalkMap.clear();
  LOG(debug) << fName << "::~CbmStsUnpackConfig done";
}

// ---- Init ----
void CbmStsUnpackConfig::InitAlgo()
{
  // Set the minimum adc cut
  fAlgo->SetMinAdcCut(fdAdcCut);

  // Set the minimum adc cut Feb independent
  for (auto cut = fdAdcCut_perFeb.begin(); cut != fdAdcCut_perFeb.end(); cut++) {
    fAlgo->SetMinAdcCut(cut->first, cut->second);
  }

  // Set the single asics time offsets
  fAlgo->SetAsicTimeOffsetVec(fvdTimeOffsetNsAsics);

  // Set Time Walk Correction map
  if (!fWalkMap.empty()) { fAlgo->SetWalkMap(fWalkMap); }

  // Set the flags for duplicate digis rejections
  fAlgo->SetDuplicatesRejection(fbRejectDuplicateDigis, fbDupliWithoutAdc);

  if (fMonitor) { fAlgo->SetMonitor(fMonitor); }

  // Set firmware binning (only relevant for legacy mode)
  if (fbUseFwBinning) { fAlgo->SetFwBinning(true); }

  // Now we have all information required to initialise the algorithm
  fAlgo->Init();

  // Mask the noisy channels set by the user
  for (auto chmask : fvChanMasks)
    fAlgo->MaskNoisyChannel(chmask.uFeb, chmask.uChan, chmask.bMasked);
}

// ---- chooseAlgo ----
std::shared_ptr<CbmStsUnpackAlgoBase> CbmStsUnpackConfig::chooseAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Non default unpacker selection
  // Legacy unpacker for data taken before mcbm 2021
  if (fGeoSetupTag.find("mcbm_beam_2020_03") != fGeoSetupTag.npos) {
    auto algo = std::make_shared<CbmStsUnpackAlgoLegacy>();
    LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
    return algo;
  }

  // Default unpacker selection
  // Unpacker algo from mcbm 2021 on and hopefully default for a long time.
  auto algo = std::make_shared<CbmStsUnpackAlgo>();
  LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
  return algo;

  LOG(error) << fName
             << "::Init - chooseAlgo() - no algorithm created something went wrong. We can not work like this!";
  return nullptr;
}


ClassImp(CbmStsUnpackConfig)
