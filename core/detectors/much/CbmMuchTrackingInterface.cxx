/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmMuchTrackingInterface.cxx
 * @brief  Input data and parameters interface from MuCh subsystem used in L1 tracker (definition)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmMuchTrackingInterface.h"

#include "FairDetector.h"
#include "FairRunAna.h"
#include <Logger.h>

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
InitStatus CbmMuchTrackingInterface::Init()
{
  LOG(info) << "\033[1;33mCALL CbmMuchTrackingInterface::Init()\033[0m";

  fGeoScheme = CbmMuchGeoScheme::Instance();
  if (!fGeoScheme) { LOG(fatal) << "CbmMuchTrackingInterface::Init: CbmMuchGeoScheme instance is nullptr"; }

  // Check initialization of the MuCh digi parameters file
  if (!fGeoScheme->IsInitialized()) {
    LOG(error) << "CbmMuchTrackingInterface::Init: MuCh digi parameters were not initialized\n"
               << "\033[4;1;32mNOTE\033[0m: For the MuCh digi parameters initialization please place the following "
               << "code to your macro:\n"
               << "\n\t// ----- MuCh digi parameters initialization --------------------------------------"
               << "\n\tif (CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch)) {"
               << "\n\t  // Parameter file name"
               << "\n\t  TString geoTag;"
               << "\n\t  CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kMuch, geoTag);"
               << "\n\t  Int_t muchFlag  = (geoTag.Contains(\"mcbm\") ? 1 : 0);"
               << "\n\t  TString parFile = gSystem->Getenv(\"VMCWORKDIR\");"
               << "\n\t  parFile += \"/parameters/much/much_\" + geoTag(0, 4) + \"_digi_sector.root\";"
               << "\n\t"
               << "\n\t  // Initialization of the geometry scheme"
               << "\n\t  auto muchGeoScheme = CbmMuchGeoScheme::Instance();"
               << "\n\t  if (!muchGeoScheme->IsInitialized()) { muchGeoScheme->Init(parFile, muchFlag); }"
               << "\n\t}"
               << "\n\t// --------------------------------------------------------------------------------";
    return kFATAL;
  }

  // Check the validity of the parameters
  if (!this->Check()) {
    LOG(error)
      << "Some errors occurred in the tracking detector interface initialization for MuCh (see information above)";
    return kFATAL;
  }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmMuchTrackingInterface::ReInit()
{
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmMuchTrackingInterface::SetParContainers() {}

//-------------------------------------------------------------------------------------------------------------------------------------
//
