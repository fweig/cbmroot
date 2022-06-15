/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmMvdTrackingInterface.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmMvdTrackingInterface.h"

#include "CbmMvdDetector.h"
#include "CbmMvdStationPar.h"

#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>

#include "TMath.h"

#include "L1Def.h"

ClassImp(CbmMvdTrackingInterface)

  CbmMvdTrackingInterface* CbmMvdTrackingInterface::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmMvdTrackingInterface::CbmMvdTrackingInterface() : FairTask("CbmMvdTrackingInterface")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmMvdTrackingInterface::~CbmMvdTrackingInterface()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetTimeResolution(int /*stationId*/) const { return 1000.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetZ(int stationId) const { return fMvdStationPar->GetZPosition(stationId); }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetXmax(int stationId) const { return this->GetRmax(stationId); }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetYmax(int stationId) const { return this->GetRmax(stationId); }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetRmin(int stationId) const
{
  return std::min(fMvdStationPar->GetBeamHeight(stationId), fMvdStationPar->GetBeamWidth(stationId));
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetRmax(int stationId) const
{
  return std::max(fMvdStationPar->GetHeight(stationId), fMvdStationPar->GetWidth(stationId));
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmMvdTrackingInterface::GetNtrackingStations() const { return fMvdStationPar->GetStationCount(); }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetThickness(int stationId) const { return fMvdStationPar->GetZThickness(stationId); }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetRadLength(int stationId) const
{
  // NOTE: Taken from CbmKF::Init() (S.Zharko)
  return fMvdStationPar->GetZThickness(stationId) / (10. * fMvdStationPar->GetZRadThickness(stationId));
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetStripsStereoAngleFront(int /*stationId*/) const { return 0.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetStripsStereoAngleBack(int /*stationId*/) const { return TMath::Pi() / 2.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetStripsSpatialRmsFront(int stationId) const
{
  return fMvdStationPar->GetXRes(stationId) / 10000.;
}


//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMvdTrackingInterface::GetStripsSpatialRmsBack(int stationId) const
{
  return fMvdStationPar->GetYRes(stationId) / 10000.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
bool CbmMvdTrackingInterface::IsTimeInfoProvided(int /*stationId*/) const { return false; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmMvdTrackingInterface::Init()
{
  LOG(info) << "\033[1;33mCALL CbmMvdTrackingInterface::Init()\033[0m";

  fMvdStationPar = CbmMvdDetector::Instance()->GetParameterFile();

  if (!fMvdStationPar) { return kFATAL; }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmMvdTrackingInterface::ReInit()
{
  LOG(info) << "\033[1;33mCALL CbmMvdTrackingInterface::ReInit()\033[0m";
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmMvdTrackingInterface::SetParContainers() {}

//-------------------------------------------------------------------------------------------------------------------------------------
//
