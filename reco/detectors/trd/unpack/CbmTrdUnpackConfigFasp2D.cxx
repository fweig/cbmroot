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
  : CbmRecoUnpackConfig("CbmTrdUnpackConfigFasp2D")
  , fGeoSetupTag(detGeoSetupTag)
  , fRunId(runid)
{
}

CbmTrdUnpackConfigFasp2D::~CbmTrdUnpackConfigFasp2D() {}

// ---- Init ----
void CbmTrdUnpackConfigFasp2D::InitUnpacker()
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

  // Now we have all information required to initialise the algorithm
  algo->Init();

  // Pass the algo to its member in the base class
  fAlgo = algo;
}

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
