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


#include "FairDetector.h"
#include "FairRunAna.h"
#include <FairLogger.h>

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
