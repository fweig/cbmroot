/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmMvdTrackerIF.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmMvdTrackerIF.h"
#include "CbmMvdDetector.h"
#include "CbmMvdStationPar.h"
#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>
#include "TMath.h"
#include "L1Def.h"

ClassImp(CbmMvdTrackerIF)

CbmMvdTrackerIF* CbmMvdTrackerIF::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmMvdTrackerIF::CbmMvdTrackerIF() : FairTask("CbmMvdTrackerIF")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmMvdTrackerIF::~CbmMvdTrackerIF()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetTimeResolution(int /*stationId*/) const { return 1000.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetZ(int stationId) const
{ 
  return fMvdStationPar->GetZPosition(stationId); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetXmax(int stationId) const 
{ 
  return this->GetRmax(stationId);
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetYmax(int stationId) const 
{ 
  return this->GetRmax(stationId); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetRmin(int stationId) const
{ 
  return std::min(fMvdStationPar->GetBeamHeight(stationId), fMvdStationPar->GetBeamWidth(stationId)); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetRmax(int stationId) const 
{ 
  return std::max(fMvdStationPar->GetHeight(stationId), fMvdStationPar->GetWidth(stationId)); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmMvdTrackerIF::GetNtrackingStations() const 
{ 
  return fMvdStationPar->GetStationCount(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetThickness(int stationId) const 
{ 
  return fMvdStationPar->GetZThickness(stationId); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetRadLength(int stationId) const 
{ 
  // NOTE: Taken from CbmKF::Init() (S.Zharko)
  return fMvdStationPar->GetZThickness(stationId) / (10. * fMvdStationPar->GetZRadThickness(stationId));
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetStripsStereoAngleFront(int /*stationId*/) const 
{ 
  return 0.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetStripsStereoAngleBack(int /*stationId*/) const 
{ 
  return TMath::Pi() / 2.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetStripsSpatialRmsFront(int stationId) const
{
  return fMvdStationPar->GetXRes(stationId) / 10000.;
}


//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackerIF::GetStripsSpatialRmsBack(int stationId) const
{
  return fMvdStationPar->GetYRes(stationId) / 10000.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
bool CbmMvdTrackerIF::IsTimeInfoProvided(int /*stationId*/) const { return false; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmMvdTrackerIF::Init()
{
  LOG(info) << "\033[1;33mCALL CbmMvdTrackerIF::Init()\033[0m";
  
  fMvdStationPar = CbmMvdDetector::Instance()->GetParameterFile();
  
  if (!fMvdStationPar) { return kFATAL; }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmMvdTrackerIF::ReInit()
{
  LOG(info) << "\033[1;33mCALL CbmMvdTrackerIF::ReInit()\033[0m";
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmMvdTrackerIF::SetParContainers()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
