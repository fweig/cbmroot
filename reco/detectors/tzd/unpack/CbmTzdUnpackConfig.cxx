/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

#include "CbmTzdUnpackConfig.h"

#include "CbmTofDigi.h"
#include "CbmTzdUnpackAlgo.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmTzdUnpackConfig::CbmTzdUnpackConfig(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmTzdUnpackConfig", detGeoSetupTag, runid)
{
  SetFlagBmonParMode();
}

CbmTzdUnpackConfig::~CbmTzdUnpackConfig() {}

// ---- Init ----
void CbmTzdUnpackConfig::InitAlgo()
{
  fAlgo->SetFlagEpochCountHack2021(fbEpochCountHack2021);

  if (fMonitor) { fAlgo->SetMonitor(fMonitor); }

  // Now we have all information required to initialise the algorithm
  fAlgo->Init();
}

// ---- chooseAlgo ----
std::shared_ptr<CbmTzdUnpackAlgo> CbmTzdUnpackConfig::chooseAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Default unpacker selection
  // Unpacker algo from mcbm 2021 on and hopefully default for a long time.
  auto algo = std::make_shared<CbmTzdUnpackAlgo>();
  LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
  if (fbBmonParMode) {
    LOG(info) << fName << "::chooseAlgo - Setting the new algo in BMon Par mode";
    algo->SetFlagBmonParMode(fbBmonParMode);
  }

  return algo;

  LOG(error) << fName
             << "::Init - chooseAlgo() - no algorithm created something went wrong. We can not work like this!";
  return nullptr;
}

ClassImp(CbmTzdUnpackConfig)
