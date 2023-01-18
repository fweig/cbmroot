/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmMvdTrackingInterface.cxx
 * @brief  Input data and parameters interface from MVD subsystem used in L1 tracker (definition)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmMvdTrackingInterface.h"

#include "FairDetector.h"
#include "FairRunAna.h"
#include <Logger.h>

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
InitStatus CbmMvdTrackingInterface::Init()
{
  fMvdStationPar = CbmMvdDetector::Instance()->GetParameterFile();

  if (!fMvdStationPar) { return kFATAL; }

  // Check the validity of the parameters
  if (!this->Check()) {
    LOG(error)
      << "Some errors occurred in the tracking detector interface initialization for MVD (see information above)";
    return kFATAL;
  }

  return kSUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmMvdTrackingInterface::ReInit()
{
  this->SetParContainers();
  return Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
void CbmMvdTrackingInterface::SetParContainers() {}

//-------------------------------------------------------------------------------------------------------------------------------------
//
