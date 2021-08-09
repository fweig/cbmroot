/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmStsUnpackConfig.h"

#include "CbmStsUnpackAlgo.h"
#include "CbmStsUnpackAlgoLegacy.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmStsUnpackConfig::CbmStsUnpackConfig(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmStsUnpackConfig")
  , fGeoSetupTag(detGeoSetupTag)
  , fRunId(runid)
{
}

CbmStsUnpackConfig::~CbmStsUnpackConfig() {}

// ---- Init ----
void CbmStsUnpackConfig::InitUnpacker()
{
  LOG(info) << fName << "::Init -";

  auto initOk = kTRUE;

  // First choose the derived unpacking algorithm to be used and pass the raw to digi method
  auto algo = chooseAlgo();

  if (fDoLog) LOG(info) << fName << "::Init - SetParFilesBasePath";
  algo->SetParFilesBasePath(fParFilesBasePath);

  // Initialise the parameter containers required by the unpacker algo. Includes loading the corresponding ascii files
  auto reqparvec = algo->GetParContainerRequest(fGeoSetupTag, fRunId);
  initOk &= initParContainers(reqparvec);

  // Set the minimum adc cut
  algo->SetMinAdcCut(fdAdcCut);

  // Set the single asics time offsets
  algo->SetAsicTimeOffsetVec(fvdTimeOffsetNsAsics);

  // Mask the noisy channels set by the user
  for (auto chmask : fvChanMasks)
    algo->MaskNoisyChannel(chmask.uFeb, chmask.uChan, chmask.bMasked);

  // Now we have all information required to initialise the algorithm
  algo->Init();

  if (fMonitor) {
    fMonitor->Init(static_cast<CbmMcbm2018StsPar*>(reqparvec->at(0).second.get()));
    algo->SetMonitor(fMonitor);
  }

  // Pass the algo to its member in the base class
  fAlgo = algo;
}

// ---- chooseAlgo ----
std::shared_ptr<CbmStsUnpackAlgoBase> CbmStsUnpackConfig::chooseAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Default unpacker selection
  // Unpacker algo from mcbm 2021 on and hopefully default for a long time.
  auto algo = std::make_shared<CbmStsUnpackAlgo>();
  //auto algo = std::make_shared<CbmStsUnpackAlgoLegacy>();
  LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
  return algo;

  LOG(error) << fName
             << "::Init - chooseAlgo() - no algorithm created something went wrong. We can not work like this!";
  return nullptr;
}


ClassImp(CbmStsUnpackConfig)
