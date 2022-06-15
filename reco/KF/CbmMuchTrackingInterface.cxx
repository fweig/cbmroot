/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmMuchTrackingInterface.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmMuchTrackingInterface.h"

#include "CbmMuchModuleGem.h"
#include "CbmMuchPad.h"
#include "CbmMuchStation.h"

#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>

#include "TFile.h"
#include "TMath.h"

ClassImp(CbmMuchTrackingInterface)

  CbmMuchTrackingInterface* CbmMuchTrackingInterface::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmMuchTrackingInterface::CbmMuchTrackingInterface() : FairTask("CbmMuchTrackingInterface")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmMuchTrackingInterface::~CbmMuchTrackingInterface()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetTimeResolution(int /*stationId*/) const { return 3.9; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetZ(int stationId) const { return GetMuchLayer(stationId)->GetZ(); }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetXmax(int /*stationId*/) const { return 100.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetYmax(int /*stationId*/) const { return 100.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetRmin(int /*stationId*/) const { return 10.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetRmax(int /*stationId*/) const { return 100.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmMuchTrackingInterface::GetNtrackingStations() const
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
double CbmMuchTrackingInterface::GetThickness(int stationId) const { return GetMuchLayer(stationId)->GetDz(); }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetRadLength(int /*stationId*/) const
{
  return 0;  // TODO: Replace with correct value
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetStripsStereoAngleFront(int /*stationId*/) const { return 0.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetStripsStereoAngleBack(int /*stationId*/) const { return TMath::Pi() / 2.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetStripsSpatialRmsFront(int /*stationId*/) const { return 0.35; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmMuchTrackingInterface::GetStripsSpatialRmsBack(int /*stationId*/) const { return 0.35; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
bool CbmMuchTrackingInterface::IsTimeInfoProvided(int /*stationId*/) const { return true; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmMuchTrackingInterface::Init()
{
  LOG(info) << "\033[1;33mCALL CbmMuchTrackingInterface::Init()\033[0m";

  fGeoScheme = CbmMuchGeoScheme::Instance();
  if (!fGeoScheme) { LOG(fatal) << "CbmMuchTrackingInterface::Init: CbmMuchGeoScheme instance is nullptr"; }

  // Check initialization of the MuCh digi parameters file
  if (!fGeoScheme->IsInitialized()) {
    LOG(fatal) << "CbmMuchTrackingInterface::Init: MuCh digi parameters were not intialized\n"
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
InitStatus CbmMuchTrackingInterface::ReInit()
{
  LOG(info) << "\033[1;33mCALL CbmMuchTrackingInterface::ReInit()\033[0m";
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmMuchTrackingInterface::SetParContainers() {}

//-------------------------------------------------------------------------------------------------------------------------------------
//
