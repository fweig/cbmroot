/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrdTrackingInterface.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmTrdTrackingInterface.h"

#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>
#include "TGeoManager.h"
#include "TString.h"

ClassImp(CbmTrdTrackingInterface)

  CbmTrdTrackingInterface* CbmTrdTrackingInterface::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrdTrackingInterface::CbmTrdTrackingInterface() : FairTask("CbmTrdTrackingInterface")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrdTrackingInterface::~CbmTrdTrackingInterface()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmTrdTrackingInterface::GetNtrackingStations() const
{
  // NOTE: For TRD detector subsystem, a TRD layer is assigned as a tracking station:
  int nTrdLayers = 0;
  auto topNodes  = gGeoManager->GetTopNode()->GetNodes();
  for (int iTopNode = 0; iTopNode < topNodes->GetEntriesFast(); ++iTopNode) {
    auto topNode = static_cast<TGeoNode*>(topNodes->At(iTopNode));
    if (TString(topNode->GetName()).Contains("trd")) {
      auto layers = topNode->GetNodes();
      for (int iLayer = 0; iLayer < layers->GetEntriesFast(); ++iLayer) {
        auto layer = static_cast<TGeoNode*>(layers->At(iLayer));
        if (TString(layer->GetName()).Contains("layer")) { ++nTrdLayers; }
      }
    }
  }
  return nTrdLayers;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrdTrackingInterface::Init()
{
  // Check access to TRD modules
  for (int iSt = 0; iSt < this->GetNtrackingStations(); ++iSt) {
    if (!dynamic_cast<CbmTrdParModDigi*>(fTrdDigiPar->GetModulePar(fTrdDigiPar->GetModuleId(iSt)))) {
      LOG(fatal) << "CbmTrdTrackingInterface::Init: error accessing the TRD tracking station " << iSt
                 << " (failed dynamic cast to CbmTrdParModDigi)";
    }
  }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrdTrackingInterface::ReInit()
{
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmTrdTrackingInterface::SetParContainers()
{
  auto runtimeDb = FairRunAna::Instance()->GetRuntimeDb();
  fTrdDigiPar    = dynamic_cast<CbmTrdParSetDigi*>(runtimeDb->getContainer("CbmTrdParSetDigi"));
  if (!fTrdDigiPar) {
    LOG(fatal) << "CbmTrdTrackingInterface::SetParContainers: error accessing to CbmTrdParSetDigi container";
  }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
