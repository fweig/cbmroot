/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmStsTrackerIF.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  27.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmStsTrackerIF.h"
#include "CbmStsStation.h"
#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>
#include "TMath.h"
#include "L1Def.h"

ClassImp(CbmStsTrackerIF)

CbmStsTrackerIF* CbmStsTrackerIF::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmStsTrackerIF::CbmStsTrackerIF() : FairTask("CbmStsTrackerIF")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmStsTrackerIF::~CbmStsTrackerIF()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetTimeResolution(int /*stationId*/) const { return 5.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetZ(int stationId) const
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetZ(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetXmax(int stationId) const 
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetXmax(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetYmax(int stationId) const 
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetYmax(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetRmin(int /*stationId*/) const { return 0.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetRmax(int stationId) const 
{ 
  return GetXmax(stationId) > GetYmax(stationId) ? GetXmax(stationId) : GetYmax(stationId); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmStsTrackerIF::GetNtrackingStations() const 
{ 
  return CbmStsSetup::Instance()->GetNofStations(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetThickness(int stationId) const 
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetSensorD(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetRadLength(int stationId) const 
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetRadLength(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetStripsStereoAngleFront(int stationId) const 
{ 
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorRotation() + station->GetSensorStereoAngle(0) * TMath::Pi() / 180.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetStripsStereoAngleBack(int stationId) const 
{ 
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorRotation() + station->GetSensorStereoAngle(1) * TMath::Pi() / 180.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetStripsSpatialRmsFront(int stationId) const
{
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorPitch(0) / TMath::Sqrt(12.);
}


//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmStsTrackerIF::GetStripsSpatialRmsBack(int stationId) const
{
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorPitch(0) / TMath::Sqrt(12.);
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
bool CbmStsTrackerIF::IsTimeInfoProvided(int /*stationId*/) const { return true; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmStsTrackerIF::Init()
{
  LOG(info) << "\033[1;33mCALL CbmStsTrackerIF::Init()\033[0m";

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
InitStatus CbmStsTrackerIF::ReInit()
{
  LOG(info) << "\033[1;33mCALL CbmStsTrackerIF::ReInit()\033[0m";
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmStsTrackerIF::SetParContainers()
{
  LOG(info) << "\033[1;33mCALL CbmStsTrackerIF::SetParContainer()\033[0m";
  auto runtimeDb = FairRunAna::Instance()->GetRuntimeDb();
  fStsParSetModule     = dynamic_cast<CbmStsParSetModule*>(runtimeDb->getContainer("CbmStsParSetModule"));
  fStsParSetSensor     = dynamic_cast<CbmStsParSetSensor*>(runtimeDb->getContainer("CbmStsParSetSensor"));
  fStsParSetSensorCond = dynamic_cast<CbmStsParSetSensorCond*>(runtimeDb->getContainer("CbmStsParSetSensorCond"));
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
