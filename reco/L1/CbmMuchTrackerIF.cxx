/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmMuchTrackerIF.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmMuchTrackerIF.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchModuleGem.h"
#include "CbmMuchPad.h"
#include "CbmMuchStation.h"
#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>
#include "TMath.h"
#include "TFile.h"

ClassImp(CbmMuchTrackerIF)

CbmMuchTrackerIF* CbmMuchTrackerIF::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmMuchTrackerIF::CbmMuchTrackerIF() : FairTask("CbmMuchTrackerIF")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmMuchTrackerIF::~CbmMuchTrackerIF()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetTimeResolution(int /*stationId*/) const { return 3.9; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetZ(int stationId) const
{ 
  return fGeoScheme->GetStation(GetMuchStationId(stationId))->GetLayer(GetMuchLayerId(stationId))->GetZ(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetXmax(int /*stationId*/) const 
{ 
  return 100.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetYmax(int /*stationId*/) const 
{ 
  return 100.; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetRmin(int /*stationId*/) const
{ 
  return 10.; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetRmax(int /*stationId*/) const 
{ 
  return 100.; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmMuchTrackerIF::GetNtrackingStations() const 
{
  // NOTE: For MuCh detector subsystem, a MuCh layer is assigned as a tracking station! Thus, we here separate names for MuCh stations:
  // iMuchSt, muchStationId and for the tracking stations - iSt, stationId
  int nStations = 0;
  for (int iMuchSt = 0; iMuchSt < fGeoScheme->GetNStations(); ++iMuchSt) {
    nStations += fGeoScheme->GetStation(iMuchSt)->GetNLayers();
  }
  return nStations;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetThickness(int stationId) const 
{ 
  return fGeoScheme->GetStation(GetMuchStationId(stationId))->GetLayer(GetMuchLayerId(stationId))->GetDz(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetRadLength(int /*stationId*/) const 
{ 
  return 0; // TODO: Replace with correct value
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetStripsStereoAngleFront(int /*stationId*/) const 
{ 
  return 0.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetStripsStereoAngleBack(int /*stationId*/) const 
{ 
  return TMath::Pi() / 2.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetStripsSpatialRmsFront(int /*stationId*/) const
{
  return 0.35;
}


//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackerIF::GetStripsSpatialRmsBack(int /*stationId*/) const
{
  return 0.35;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
bool CbmMuchTrackerIF::IsTimeInfoProvided(int /*stationId*/) const { return true; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmMuchTrackerIF::Init()
{
  LOG(info) << "\033[1;33mCALL CbmMuchTrackerIF::Init()\033[0m";
  
  fGeoScheme = CbmMuchGeoScheme::Instance();
  if (!fGeoScheme) { 
    LOG(fatal) << "CbmMuchTrackerIF::Init: CbmMuchGeoScheme instance is nullptr";
  }

  // Check initialization of the MuCh digi parameters file
  if (!fGeoScheme->IsInitialized()) { 
    LOG(fatal) << "CbmMuchTrackerIF::Init: MuCh digi parameters were not intialized\n"
               << "\033[4;1;32mNOTE\033[0m: For the MuCh digi parameters initialization one can place the following "
               << "code to the reconstruction macro:\n"
               << "\n\tif (CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch)) {"
               << "\n\t  // Parameter file name"
               << "\n\t  TString geoTag;"
               << "\n\t  geo->GetGeoTag(ECbmModuleId::kMuch, geoTag);"
               << "\n\t  Int_t muchFlag  = (geoTag.Contains(\"mcbm\") ? 1 : 0);"
               << "\n\t  TString parFile = gSystem->Getenv(\"VMCWORKDIR\");"
               << "\n\t  parFile += \"/parameters/much/much_\" + geoTag(0, 4) + \"_digi_sector.root\";"
               << "\n\t"                                                                          
               << "\n\t  // Initialization of the geometry scheme"                  
               << "\n\t  auto muchGeoScheme = CbmMuchGeoScheme::Instance();"
               << "\n\t  if (!muchGeoScheme->IsInitialized()) { muchGeoScheme->Init(parFile, muchFlag); }"
               << "\n\t}";
  }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmMuchTrackerIF::ReInit()
{
  LOG(info) << "\033[1;33mCALL CbmMuchTrackerIF::ReInit()\033[0m";
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmMuchTrackerIF::SetParContainers()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
