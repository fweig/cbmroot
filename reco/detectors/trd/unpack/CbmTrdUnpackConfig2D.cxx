/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmTrdUnpackConfig2D.h"

#include "CbmTrdUnpackAlgo2D.h"

#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <memory>
#include <vector>

CbmTrdUnpackConfig2D::CbmTrdUnpackConfig2D(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmTrdUnpackConfig2D")
  , fGeoSetupTag(detGeoSetupTag)
  , fRunId(runid)
{
}

CbmTrdUnpackConfig2D::~CbmTrdUnpackConfig2D() {}

// ---- Init ----
void CbmTrdUnpackConfig2D::InitUnpacker()
{
  LOG(info) << fName << "::Init -";

  auto initOk = kTRUE;

  // First choose the derived unpacking algorithm to be used and pass the raw to digi method
  auto algo = chooseAlgo();

  // If we got a handmade spadic we pass it to the algorithm and the RTD method
  if (fSpadic) {
    algo->SetSpadicObject(fSpadic);
    fRTDMethod->SetSpadicObject(fSpadic);
  }

  if (fDoLog) LOG(info) << fName << "::Init - SetRawToDigiMethod";
  algo->SetRawToDigiMethod(fRTDMethod);

  if (fDoLog) LOG(info) << fName << "::Init - SetParFilesBasePath";
  algo->SetParFilesBasePath(fParFilesBasePath);

  // If we have a monitor in the config add it to the algo
  if (fMonitor) algo->SetMonitor(fMonitor);

  // Initialise the parameter containers required by the unpacker algo. Includes loading the corresponding ascii files
  auto reqparvec = algo->GetParContainerRequest(fGeoSetupTag, fRunId);
  initOk &= initParContainers(reqparvec);

  // Now we have all information required to initialise the algorithm
  algo->Init();

  // Pass the algo to its member in the base class
  fAlgo = algo;
}

// ---- chooseAlgo ----
std::shared_ptr<CbmTrdUnpackAlgo2D> CbmTrdUnpackConfig2D::chooseAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Default unpacker selection
  // Unpacker algo from mcbm 2021 on and hopefully default for a long time.
  auto algo = std::make_shared<CbmTrdUnpackAlgo2D>();
  LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
  return algo;

  LOG(error) << fName
             << "::Init - chooseAlgo() - no algorithm created something went wrong. We can not work like this!";
  return nullptr;
}


ClassImp(CbmTrdUnpackConfig2D)
