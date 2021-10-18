/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmTrdUnpackConfigFasp2D.h"

#include "CbmTrdUnpackAlgoFasp2D.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmTrdUnpackConfigFasp2D::CbmTrdUnpackConfigFasp2D(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmTrdUnpackConfigFasp2D", detGeoSetupTag, runid)
{
}

CbmTrdUnpackConfigFasp2D::~CbmTrdUnpackConfigFasp2D() {}

// ---- Init ----

// ---- chooseAlgo ----
std::shared_ptr<CbmTrdUnpackAlgoFasp2D> CbmTrdUnpackConfigFasp2D::chooseAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Default unpacker selection
  // Unpacker algo from mcbm 2021 on and hopefully default for a long time.
  auto algo = std::make_shared<CbmTrdUnpackAlgoFasp2D>();
  LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
  return algo;

  LOG(error) << fName
             << "::Init - chooseAlgo() - no algorithm created something went wrong. We can not work like this!";
  return nullptr;
}


ClassImp(CbmTrdUnpackConfigFasp2D)
