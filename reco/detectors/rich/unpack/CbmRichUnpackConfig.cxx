/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmRichUnpackConfig.h"

#include "CbmRichUnpackAlgo.h"
#include "CbmRichUnpackAlgo2022.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmRichUnpackConfig::CbmRichUnpackConfig(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmRichUnpackConfig", detGeoSetupTag, runid)
{
}

CbmRichUnpackConfig::~CbmRichUnpackConfig() {}

// ---- Init ----
void CbmRichUnpackConfig::InitAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - SetMaskedDiRICHes";
  fAlgo->SetMaskedDiRICHes(&fMaskedDiRICHes);

  // Now we have all information required to initialise the algorithm
  fAlgo->Init();
}

// ---- chooseAlgo ----
std::shared_ptr<CbmRichUnpackAlgoBase> CbmRichUnpackConfig::chooseAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Default unpacker selection
  // Unpacker algo from mcbm 2021 on and hopefully default for a long time.
  if (fUnpackerVersion == CbmRichUnpackerVersion::v02) {
    auto algo = std::make_shared<CbmRichUnpackAlgo>();
    LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
    return algo;
  }

  if (fUnpackerVersion == CbmRichUnpackerVersion::v03) {
    auto algo = std::make_shared<CbmRichUnpackAlgo2022>();
    LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
    return algo;
  }

  LOG(error) << fName
             << "::Init - chooseAlgo() - no algorithm created something went wrong. We can not work like this!";
  return nullptr;
}


ClassImp(CbmRichUnpackConfig)
