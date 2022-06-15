/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrdTrackerIF.cxx
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (definition)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmTrdTrackerIF.h"
#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>
#include "TMath.h"
#include "TFile.h"
#include "TGeoManager.h"
#include "TString.h"

ClassImp(CbmTrdTrackerIF)

CbmTrdTrackerIF* CbmTrdTrackerIF::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrdTrackerIF::CbmTrdTrackerIF() : FairTask("CbmTrdTrackerIF")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrdTrackerIF::~CbmTrdTrackerIF()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetTimeResolution(int /*stationId*/) const { return 10.; }

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetZ(int stationId) const
{ 
  return GetTrdModulePar(stationId)->GetZ(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetXmax(int stationId) const 
{ 
  return GetTrdModulePar(stationId)->GetSizeX(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetYmax(int stationId) const 
{ 
  return GetTrdModulePar(stationId)->GetSizeY(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetRmin(int /*stationId*/) const
{ 
  return 0.; 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetRmax(int stationId) const 
{ 
  return 2. * this->GetXmax(stationId);
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
int CbmTrdTrackerIF::GetNtrackingStations() const 
{
  // NOTE: For TRD detector subsystem, a TRD layer is assigned as a tracking station:
  int nTrdLayers = 0;
  auto topNodes = gGeoManager->GetTopNode()->GetNodes();
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
double CbmTrdTrackerIF::GetThickness(int stationId) const 
{ 
  return 2. * GetTrdModulePar(stationId)->GetSizeZ(); 
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetRadLength(int /*stationId*/) const 
{ 
  return 1.6;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetStripsStereoAngleFront(int /*stationId*/) const 
{ 
  return 0.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetStripsStereoAngleBack(int /*stationId*/) const 
{ 
  return TMath::Pi() / 2.;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetStripsSpatialRmsFront(int /*stationId*/) const
{
  return 0.15;
}


//-------------------------------------------------------------------------------------------------------------------------------------
//
double CbmTrdTrackerIF::GetStripsSpatialRmsBack(int /*stationId*/) const
{
  return 0.15;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
bool CbmTrdTrackerIF::IsTimeInfoProvided(int /*stationId*/) const { return true; }


//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrdTrackerIF::Init()
{
  // Check access to TRD modules 
  for (int iSt = 0; iSt < this->GetNtrackingStations(); ++iSt) {
    if (!dynamic_cast<CbmTrdParModDigi*>(fTrdDigiPar->GetModulePar(fTrdDigiPar->GetModuleId(iSt)))) {
      LOG(fatal) << "CbmTrdTrackerIF::Init: error accessing the TRD tracking station " << iSt
                 << " (failed dynamic cast to CbmTrdParModDigi)"; 
   }
  }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrdTrackerIF::ReInit()
{
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmTrdTrackerIF::SetParContainers()
{
  auto runtimeDb = FairRunAna::Instance()->GetRuntimeDb();
  fTrdDigiPar = dynamic_cast<CbmTrdParSetDigi*>(runtimeDb->getContainer("CbmTrdParSetDigi"));
  if (!fTrdDigiPar) { LOG(fatal) << "CbmTrdTrackerIF::SetParContainers: error accessing to CbmTrdParSetDigi container"; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
