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

#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>

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
double CbmStsTrackingInterface::GetStripsStereoAngleFront(int stationId) const
{
  auto station = GetStsStation(stationId);
  return station->GetSensorRotation() + station->GetSensorStereoAngle(0) * TMath::Pi() / 180.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackingInterface::GetStripsStereoAngleBack(int stationId) const
{
  auto station = GetStsStation(stationId);
  return station->GetSensorRotation() + station->GetSensorStereoAngle(1) * TMath::Pi() / 180.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmStsTrackingInterface::Init()
{
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
