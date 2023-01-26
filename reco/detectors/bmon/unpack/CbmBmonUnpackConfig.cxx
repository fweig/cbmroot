/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

#include "CbmBmonUnpackConfig.h"

#include "CbmBmonUnpackAlgo.h"
#include "CbmTofDigi.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmBmonUnpackConfig::CbmBmonUnpackConfig(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmBmonUnpackConfig", detGeoSetupTag, runid)
{
  SetFlagBmonParMode();
}

CbmBmonUnpackConfig::~CbmBmonUnpackConfig() {}

// ---- Init ----
void CbmBmonUnpackConfig::InitAlgo()
{
  fAlgo->SetFlagEpochCountHack2021(fbEpochCountHack2021);

  if (fMonitor) { fAlgo->SetMonitor(fMonitor); }

  // Now we have all information required to initialise the algorithm
  fAlgo->Init();
}

// ---- chooseAlgo ----
std::shared_ptr<CbmBmonUnpackAlgo> CbmBmonUnpackConfig::chooseAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Default unpacker selection
  // Unpacker algo from mcbm 2021 on and hopefully default for a long time.
  auto algo = std::make_shared<CbmBmonUnpackAlgo>();
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

ClassImp(CbmBmonUnpackConfig)
