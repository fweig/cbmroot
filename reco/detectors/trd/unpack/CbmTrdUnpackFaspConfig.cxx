/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer], Alexandru Bercuci*/

#include "CbmTrdUnpackFaspConfig.h"

CbmTrdUnpackFaspConfig::CbmTrdUnpackFaspConfig(std::string detGeoSetupTag, UInt_t runid)
  : CbmRecoUnpackConfig("CbmTrdUnpackFaspConfig", detGeoSetupTag, runid)
  , fFaspMap()
{
}

CbmTrdUnpackFaspConfig::~CbmTrdUnpackFaspConfig() {}

// ---- Init ----

// ---- chooseAlgo ----
std::shared_ptr<CbmTrdUnpackFaspAlgo> CbmTrdUnpackFaspConfig::chooseAlgo()
{
  if (fDoLog) LOG(info) << fName << "::Init - chooseAlgo";

  // Default unpacker selection
  // Unpacker algo from mcbm 2021 on and hopefully default for a long time.
  auto algo = std::make_shared<CbmTrdUnpackFaspAlgo>();
  LOG(info) << fName << "::chooseAlgo() - selected algo = " << algo->Class_Name();
  return algo;

  LOG(error) << fName
             << "::Init - chooseAlgo() - no algorithm created something went wrong. We can not work like this!";
  return nullptr;
}

//_____________________________________________________________________
void CbmTrdUnpackFaspConfig::InitAlgo()
{
  if (fDoLog) LOG(info) << fName << "::InitAlgo - Setup Fasp mapping";
  fAlgo->SetAsicMapping(fFaspMap);
  fAlgo->SetCrobMapping(fCrobMap);
  /*if (fDoLog) */ fAlgo->PrintAsicMapping();

  // If we have a monitor in the config add it to the algo
  if (fMonitor) fAlgo->SetMonitor(fMonitor);

  // Now we have all information required to initialise the algorithm
  fAlgo->Init();

  // Finish initialization of the monitoring task after all information for the run are gathered
  if (fMonitor) {
    /*if (fDoLog)*/ LOG(info) << fName << "::InitAlgo - Setup monitoring task";
    fMonitor->Init();
  }
}

//_____________________________________________________________________
void CbmTrdUnpackFaspConfig::SetFaspMapping(int modAddress, uint8_t faspMap[NFASPMOD])
{
  memcpy(fFaspMap[modAddress], faspMap, NFASPMOD * sizeof(uint8_t));
}

//_____________________________________________________________________
void CbmTrdUnpackFaspConfig::SetCrobMapping(int modAddress, uint16_t crobMap[NCROBMOD])
{
  memcpy(fCrobMap[modAddress], crobMap, NCROBMOD * sizeof(uint16_t));
}

ClassImp(CbmTrdUnpackFaspConfig)
