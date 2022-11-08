/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackingDetectorInterfaceInit.cxx
 * @brief  FairTask for the tracker detector interfaces initialization (implementation)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmTrackingDetectorInterfaceInit.h"

#include "CbmMuchTrackingInterface.h"
#include "CbmMvdTrackingInterface.h"
#include "CbmSetup.h"
#include "CbmStsTrackingInterface.h"
#include "CbmTofTrackingInterface.h"
#include "CbmTrdTrackingInterface.h"

ClassImp(CbmTrackingDetectorInterfaceInit)

  CbmTrackingDetectorInterfaceInit* CbmTrackingDetectorInterfaceInit::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackingDetectorInterfaceInit::CbmTrackingDetectorInterfaceInit() : FairTask("CbmTrackingDetectorInterfaceInit")
{
  if (!fpInstance) {
    fpInstance = this;

    // Check presence of the desired detectors
    fbUseMvd  = CbmSetup::Instance()->IsActive(ECbmModuleId::kMvd);
    fbUseSts  = CbmSetup::Instance()->IsActive(ECbmModuleId::kSts);
    fbUseMuch = CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch);
    fbUseTrd  = CbmSetup::Instance()->IsActive(ECbmModuleId::kTrd);
    fbUseTof  = CbmSetup::Instance()->IsActive(ECbmModuleId::kTof);

    // Invoke the detector interfaces
    if (fbUseMvd) { fpMvdTrackingInterface = new CbmMvdTrackingInterface(); }
    if (fbUseSts) { fpStsTrackingInterface = new CbmStsTrackingInterface(); }
    if (fbUseMuch) { fpMuchTrackingInterface = new CbmMuchTrackingInterface(); }
    if (fbUseTrd) { fpTrdTrackingInterface = new CbmTrdTrackingInterface(); }
    if (fbUseTof) { fpTofTrackingInterface = new CbmTofTrackingInterface(); }

    // Add subtasks - tracker detector interfaces
    if (fbUseMvd) { this->Add(fpMvdTrackingInterface); }
    if (fbUseSts) { this->Add(fpStsTrackingInterface); }
    if (fbUseMuch) { this->Add(fpMuchTrackingInterface); }
    if (fbUseTrd) { this->Add(fpTrdTrackingInterface); }
    if (fbUseTof) { this->Add(fpTofTrackingInterface); }
  }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackingDetectorInterfaceInit::~CbmTrackingDetectorInterfaceInit()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}
