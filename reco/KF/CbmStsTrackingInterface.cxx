/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmStsTrackingInterface.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  27.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmStsTrackingInterface.h"

#include "CbmStsStation.h"

#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>

#include "TMath.h"

#include "L1Def.h"

ClassImp(CbmStsTrackingInterface)

  CbmStsTrackingInterface* CbmStsTrackingInterface::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmStsTrackingInterface::CbmStsTrackingInterface() : FairTask("CbmStsTrackingInterface")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmStsTrackingInterface::~CbmStsTrackingInterface()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetTimeResolution(int /*stationId*/) const { return 5.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetZ(int stationId) const
{
  return CbmStsSetup::Instance()->GetStation(stationId)->GetZ();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetXmax(int stationId) const
{
  return CbmStsSetup::Instance()->GetStation(stationId)->GetXmax();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetYmax(int stationId) const
{
  return CbmStsSetup::Instance()->GetStation(stationId)->GetYmax();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetRmin(int /*stationId*/) const { return 0.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetRmax(int stationId) const
{
  return GetXmax(stationId) > GetYmax(stationId) ? GetXmax(stationId) : GetYmax(stationId);
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmStsTrackingInterface::GetNtrackingStations() const { return CbmStsSetup::Instance()->GetNofStations(); }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetThickness(int stationId) const
{
  return CbmStsSetup::Instance()->GetStation(stationId)->GetSensorD();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetRadLength(int stationId) const
{
  return CbmStsSetup::Instance()->GetStation(stationId)->GetRadLength();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetStripsStereoAngleFront(int stationId) const
{
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorRotation() + station->GetSensorStereoAngle(0) * TMath::Pi() / 180.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetStripsStereoAngleBack(int stationId) const
{
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorRotation() + station->GetSensorStereoAngle(1) * TMath::Pi() / 180.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetStripsSpatialRmsFront(int stationId) const
{
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorPitch(0) / TMath::Sqrt(12.);
}


//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetStripsSpatialRmsBack(int stationId) const
{
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorPitch(0) / TMath::Sqrt(12.);
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
bool CbmStsTrackingInterface::IsTimeInfoProvided(int /*stationId*/) const { return true; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmStsTrackingInterface::Init()
{
  LOG(info) << "\033[1;33mCALL CbmStsTrackingInterface::Init()\033[0m";

  // Check, if all the necessary parameter containers were found
  if (!fStsParSetModule) { return kFATAL; }
  if (!fStsParSetSensor) { return kFATAL; }
  if (!fStsParSetSensorCond) { return kFATAL; }

  // Initialize CbmStsSetup instance
  auto stsSetup = CbmStsSetup::Instance();
  if (!stsSetup->IsInit()) { stsSetup->Init(nullptr); }
  if (!stsSetup->IsModuleParsInit()) { stsSetup->SetModuleParameters(fStsParSetModule); }
  if (!stsSetup->IsSensorParsInit()) { stsSetup->SetSensorParameters(fStsParSetSensor); }
  if (!stsSetup->IsSensorCondInit()) { stsSetup->SetSensorConditions(fStsParSetSensorCond); }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmStsTrackingInterface::ReInit()
{
  LOG(info) << "\033[1;33mCALL CbmStsTrackingInterface::ReInit()\033[0m";
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmStsTrackingInterface::SetParContainers()
{
  auto runtimeDb       = FairRunAna::Instance()->GetRuntimeDb();
  fStsParSetModule     = dynamic_cast<CbmStsParSetModule*>(runtimeDb->getContainer("CbmStsParSetModule"));
  fStsParSetSensor     = dynamic_cast<CbmStsParSetSensor*>(runtimeDb->getContainer("CbmStsParSetSensor"));
  fStsParSetSensorCond = dynamic_cast<CbmStsParSetSensorCond*>(runtimeDb->getContainer("CbmStsParSetSensorCond"));
  if (!fStsParSetModule) {
    LOG(fatal) << "CbmStsTrackingInterface::SetParContainers: error accessing to CbmStsParSetModule container";
  }
  if (!fStsParSetSensor) {
    LOG(fatal) << "CbmStsTrackingInterface::SetParContainers: error accessing to CbmStsParSetSensor container";
  }
  if (!fStsParSetSensorCond) {
    LOG(fatal) << "CbmStsTrackingInterface::SetParContainers: error accessing to CbmStsParSetSensorCond container";
  }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
