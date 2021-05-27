/** @file CbmBinnedTrackerConfigurator.cxx
  * @copyright Copyright (C) 2017-2020 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [orginator], Florian Uhlig **/

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CbmBinnedTrackerConfigurator.h"

#include "CbmMCDataManager.h"
#include "CbmStsSetup.h"

#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include "GeoReader.h"

CbmBinnedTrackerConfigurator::CbmBinnedTrackerConfigurator(bool useAllDetectors)
  : fUseAllDetectors(useAllDetectors)
  , fTracker(0)
  , fSettings(0)
  , fPrimaryParticleIds()
  , fMCTracks(0)
  , fStsPoints(0)
  , fMuchPoints(0)
  , fTrdPoints(0)
  , fTofPoints(0)
{
  fill_n(fUseModules, int(ECbmModuleId::kLastModule), fUseAllDetectors);
  fPrimaryParticleIds.push_back(ppiNone);
  //fUseModules[kMuch] = false;// Temporary hack
  fUseModules[ToIntegralType(ECbmModuleId::kRich)] = false;  // Temporary hack
}

CbmBinnedTrackerConfigurator::~CbmBinnedTrackerConfigurator() { delete fTracker; }

InitStatus CbmBinnedTrackerConfigurator::Init()
{
  CbmStsSetup* stsSetup = CbmStsSetup::Instance();

  if (!stsSetup->IsInit()) stsSetup->Init();

  fSettings->SetConfiguring(true);
  fSettings->SetPrimaryParticles(fPrimaryParticleIds);
  fSettings->SetUse(fUseModules);
  fSettings->SetNofStsStations(0);
  fSettings->SetNofMuchStations(0);
  fSettings->SetNofTrdStations(0);
  fSettings->SetNofStations(0);
  fSettings->ResetScats();
  CbmBinnedGeoReader* geoReader = CbmBinnedGeoReader::Instance();

  if (0 == geoReader) LOG(fatal) << "Couldn't instantiate CbmBinnedGeoReader";

  geoReader->Read();

  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) LOG(fatal) << "No FairRootManager";

  fMCTracks = static_cast<TClonesArray*>(ioman->GetObject("MCTrack"));

  if (0 == fMCTracks) LOG(fatal) << "No MC tracks in the input file";

  if (fSettings->Use(ECbmModuleId::kSts)) {
    fStsPoints = static_cast<TClonesArray*>(ioman->GetObject("StsPoint"));

    if (0 == fStsPoints) LOG(fatal) << "No sts MC points in the input file";
  }

  if (fSettings->Use(ECbmModuleId::kMuch)) {
    fMuchPoints = static_cast<TClonesArray*>(ioman->GetObject("MuchPoint"));

    if (0 == fMuchPoints) LOG(fatal) << "No much MC points in the input file";
  }

  if (fSettings->Use(ECbmModuleId::kTrd)) {
    fTrdPoints = static_cast<TClonesArray*>(ioman->GetObject("TrdPoint"));

    if (0 == fTrdPoints) LOG(fatal) << "No trd MC points in the input file";
  }

  if (fSettings->Use(ECbmModuleId::kTof)) {
    fTofPoints = static_cast<TClonesArray*>(ioman->GetObject("TofPoint"));

    if (0 == fTofPoints) LOG(fatal) << "No tof MC points in the input file";
  }

  return kSUCCESS;
}

void CbmBinnedTrackerConfigurator::Exec(Option_t*)
{
  CbmBinnedHitReader::Instance()->Read();
  CbmBinnedHitReader::Instance()->Handle();
}

void CbmBinnedTrackerConfigurator::Finish() { CbmBinnedHitReader::Instance()->Finish(); }

void CbmBinnedTrackerConfigurator::SetParContainers()
{
  fSettings =
    static_cast<CbmBinnedSettings*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmBinnedSettings"));
}

ClassImp(CbmBinnedTrackerConfigurator)
