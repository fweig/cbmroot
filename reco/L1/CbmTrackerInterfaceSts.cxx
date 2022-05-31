/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackerInterfaceSts.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  27.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmTrackerInterfaceSts.h"
#include "CbmStsStation.h"
#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>
#include "TMath.h"
#include "L1Def.h"

ClassImp(CbmTrackerInterfaceSts)

CbmTrackerInterfaceSts* CbmTrackerInterfaceSts::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackerInterfaceSts::CbmTrackerInterfaceSts() : FairTask("CbmTrackerInterfaceSts")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackerInterfaceSts::~CbmTrackerInterfaceSts()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetTimeResolution(int /*stationId*/) const { return 5.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetZ(int stationId) const
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetZ(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetXmax(int stationId) const 
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetXmax(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetYmax(int stationId) const 
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetYmax(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetRmin(int /*stationId*/) const { return 0.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetRmax(int stationId) const 
{ 
  return GetXmax(stationId) > GetYmax(stationId) ? GetXmax(stationId) : GetYmax(stationId); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmTrackerInterfaceSts::GetNstations() const 
{ 
  return CbmStsSetup::Instance()->GetNofStations(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetThickness(int stationId) const 
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetSensorD(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetRadLength(int stationId) const 
{ 
  return CbmStsSetup::Instance()->GetStation(stationId)->GetRadLength(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetStripsStereoAngleFront(int stationId) const 
{ 
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorRotation() + station->GetSensorStereoAngle(0) * TMath::Pi() / 180.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetStripsStereoAngleBack(int stationId) const 
{ 
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorRotation() + station->GetSensorStereoAngle(1) * TMath::Pi() / 180.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetStripsSpatialRmsFront(int stationId) const
{
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorPitch(0) / TMath::Sqrt(12.);
}


//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceSts::GetStripsSpatialRmsBack(int stationId) const
{
  auto station = CbmStsSetup::Instance()->GetStation(stationId);
  return station->GetSensorPitch(0) / TMath::Sqrt(12.);
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
bool CbmTrackerInterfaceSts::IsTimeInfoProvided(int /*stationId*/) const { return true; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackerInterfaceSts::Init()
{
  LOG(info) << "\033[1;33mCALL CbmTrackerInterfaceSts::Init()\033[0m";

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
InitStatus CbmTrackerInterfaceSts::ReInit()
{
  LOG(info) << "\033[1;33mCALL CbmTrackerInterfaceSts::ReInit()\033[0m";
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmTrackerInterfaceSts::SetParContainers()
{
  LOG(info) << "\033[1;33mCALL CbmTrackerInterfaceSts::SetParContainer()\033[0m";
  auto runtimeDb = FairRunAna::Instance()->GetRuntimeDb();
  fStsParSetModule     = dynamic_cast<CbmStsParSetModule*>(runtimeDb->getContainer("CbmStsParSetModule"));
  fStsParSetSensor     = dynamic_cast<CbmStsParSetSensor*>(runtimeDb->getContainer("CbmStsParSetSensor"));
  fStsParSetSensorCond = dynamic_cast<CbmStsParSetSensorCond*>(runtimeDb->getContainer("CbmStsParSetSensorCond"));
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
