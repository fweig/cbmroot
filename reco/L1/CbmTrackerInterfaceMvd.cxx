/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackerInterfaceMvd.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmTrackerInterfaceMvd.h"
#include "CbmKF.h"
#include "CbmKFMaterial.h" // for CbmKFTube
#include "CbmMvdDetector.h"
#include "CbmMvdStationPar.h"
#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>
#include "TMath.h"
#include "L1Def.h"

ClassImp(CbmTrackerInterfaceMvd)

CbmTrackerInterfaceMvd* CbmTrackerInterfaceMvd::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackerInterfaceMvd::CbmTrackerInterfaceMvd() : FairTask("CbmTrackerInterfaceMvd")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackerInterfaceMvd::~CbmTrackerInterfaceMvd()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetTimeResolution(int /*stationId*/) const { return 1000.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetZ(int stationId) const
{ 
  return (fMvdMaterial->at(stationId)).z; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetXmax(int stationId) const 
{ 
  return (fMvdMaterial->at(stationId)).R; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetYmax(int stationId) const 
{ 
  return (fMvdMaterial->at(stationId)).R; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetRmin(int stationId) const
{ 
  return (fMvdMaterial->at(stationId)).r; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetRmax(int stationId) const 
{ 
  return (fMvdMaterial->at(stationId)).R; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmTrackerInterfaceMvd::GetNstations() const 
{ 
  return fMvdMaterial->size(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetThickness(int stationId) const 
{ 
  return (fMvdMaterial->at(stationId)).dz; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetRadLength(int stationId) const 
{ 
  return (fMvdMaterial->at(stationId)).RadLength; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetStripsStereoAngleFront(int /*stationId*/) const 
{ 
  return 0.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetStripsStereoAngleBack(int /*stationId*/) const 
{ 
  return TMath::Pi() / 2.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetStripsSpatialRmsFront(int stationId) const
{
  return fMvdStationPar->GetXRes(stationId) / 10000.;
}


//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrackerInterfaceMvd::GetStripsSpatialRmsBack(int stationId) const
{
  return fMvdStationPar->GetYRes(stationId) / 10000.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
bool CbmTrackerInterfaceMvd::IsTimeInfoProvided(int /*stationId*/) const { return false; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackerInterfaceMvd::Init()
{
  LOG(info) << "\033[1;33mCALL CbmTrackerInterfaceMvd::Init()\033[0m";
  
  fMvdMaterial = &(CbmKF::Instance()->vMvdMaterial);
  fMvdStationPar = CbmMvdDetector::Instance()->GetParameterFile();

  if (!fMvdMaterial) { return kFATAL; }
  if (!fMvdStationPar) { return kFATAL; }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackerInterfaceMvd::ReInit()
{
  LOG(info) << "\033[1;33mCALL CbmTrackerInterfaceMvd::ReInit()\033[0m";
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmTrackerInterfaceMvd::SetParContainers()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
