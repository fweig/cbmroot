/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmTofUnpackConfig.h"

#include "CbmTofUnpackAlgo.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmTofUnpackConfig::CbmTofUnpackConfig(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmTofUnpackConfig", detGeoSetupTag, runid)
{
}

CbmTofUnpackConfig::~CbmTofUnpackConfig() {}

// ---- Init ----
void CbmTofUnpackConfig::InitAlgo()
{
  fAlgo->SetFlagEpochCountHack2021(fbEpochCountHack2021);

  // Now we have all information required to initialise the algorithm
  fAlgo->Init();
}

// ---- chooseAlgo ----
std::shared_ptr<CbmTofUnpackAlgo> CbmTofUnpackConfig::chooseAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Default unpacker selection
  // Unpacker algo from mcbm 2021 on and hopefully default for a long time.
  auto algo = std::make_shared<CbmTofUnpackAlgo>();
  LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
  return algo;

  LOG(error) << fName
             << "::Init - chooseAlgo() - no algorithm created something went wrong. We can not work like this!";
  return nullptr;
}

ClassImp(CbmTofUnpackConfig)
