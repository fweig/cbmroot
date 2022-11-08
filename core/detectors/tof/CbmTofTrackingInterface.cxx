/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTofTrackingInterface.cxx
 * @brief  Input data and parameters interface from TOF subsystem used in L1 tracker (definition)
 * @since  23.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmTofTrackingInterface.h"

#include "CbmTofCell.h"
#include "CbmTofCreateDigiPar.h"
#include "CbmTofDigiPar.h"

#include "FairDetector.h"
#include "FairRunAna.h"
#include <Logger.h>

ClassImp(CbmTofTrackingInterface)

  CbmTofTrackingInterface* CbmTofTrackingInterface::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTofTrackingInterface::CbmTofTrackingInterface() : FairTask("CbmTofTrackingInterface")
{
  if (!fpInstance) { fpInstance = this; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTofTrackingInterface::~CbmTofTrackingInterface()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTofTrackingInterface::Init()
{
  // create digitization parameters from geometry file
  CbmTofCreateDigiPar* tofDigiPar = new CbmTofCreateDigiPar("TOF Digi Producer", "TOF task");
  LOG(info) << "Create DigiPar";
  tofDigiPar->Init();
  // ** ToF tracking station position z-components initialization **

  // Init ToF stations position z-components. For each ToF tracking station the position z-component is calculated
  // as an average of the components for each ToF module inside the tracking station.
  std::vector<int> nTofStationModules(this->GetNtrackingStations(),
                                      0);  // Number of ToF modules for a given tracking station

  fTofStationZ.clear();
  fTofStationZ.resize(this->GetNtrackingStations(), 0);

  for (int iSmType = 0; iSmType < fDigiBdfPar->GetNbSmTypes(); iSmType++) {
    for (int iSm = 0; iSm < fDigiBdfPar->GetNbSm(iSmType); iSm++) {
      for (int iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
        int iAddr         = CbmTofAddress::GetUniqueAddress(iSm, iRpc, 0, 0, iSmType);
        int station       = fDigiBdfPar->GetTrackingStation(iSmType, iSm, iRpc);
        auto* channelInfo = dynamic_cast<CbmTofCell*>(fDigiPar->GetCell(iAddr));
        if (nullptr == channelInfo) { break; }

        float z = channelInfo->GetZ();
        float x = channelInfo->GetX();
        //float y = channelInfo->GetY();

        if (station < 0) { continue; }

        // NOTE: the hack for fixing stations hits in "mcbm_beam_2021_07_surveyed"
        if (fIfMissingHits) {
          if ((x > 20.) && (z > 270.) && (station == 1)) { station = 2; }
          if (z > 400) { continue; }
        }
        fTofStationZ[station] += z;
        nTofStationModules[station] += 1;
      }
    }
  }

  /// Get the average values
  for (int iSt = 0; iSt < this->GetNtrackingStations(); ++iSt) {
    fTofStationZ[iSt] = fTofStationZ[iSt] / nTofStationModules[iSt];
  }

  // Check the validity of the parameters
  if (!this->Check()) {
    LOG(error)
      << "Some errors occurred in the tracking detector interface initialization for TOF (see information above)";
    return kFATAL;
  }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTofTrackingInterface::ReInit()
{
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmTofTrackingInterface::SetParContainers()
{

  auto runtimeDb = FairRunAna::Instance()->GetRuntimeDb();
  fDigiPar       = dynamic_cast<CbmTofDigiPar*>(runtimeDb->getContainer("CbmTofDigiPar"));
  fDigiBdfPar    = dynamic_cast<CbmTofDigiBdfPar*>(runtimeDb->getContainer("CbmTofDigiBdfPar"));
  if (!fDigiPar) {
    LOG(fatal) << "CbmTofTrackingInterface::SetParContainers: error accessing to CbmTofDigiPar container";
  }
  if (!fDigiBdfPar) {
    LOG(fatal) << "CbmTofTrackingInterface::SetParContainers: error accessing to CbmTofDigiBdfPar container";
  }
  runtimeDb->initContainers(FairRunAna::Instance()->GetRunId());
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmTofTrackingInterface::FixHitsStationsMismatch()
{
  LOG(warn) << "CbmTofTrackingInterface::FixHitsStationsMismatch was invoked (please, be sure that you are running "
            << "the mcbm_beam_2021_07_surveyed setup at the moment)";
  fIfMissingHits = true;
  this->ReInit();
}
