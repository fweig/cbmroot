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
#include "CbmTrdTrackingInterface.h"

#include <FairLogger.h>

ClassImp(CbmTrackingDetectorInterfaceInit)

  CbmTrackingDetectorInterfaceInit* CbmTrackingDetectorInterfaceInit::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackingDetectorInterfaceInit::CbmTrackingDetectorInterfaceInit() : FairTask("CbmTrackingDetectorInterfaceInit")
{
  if (!fpInstance) {
    fpInstance = this;

    /** Check presence of the desired detectors **/
    bool useMvd  = CbmSetup::Instance()->IsActive(ECbmModuleId::kMvd);
    bool useSts  = CbmSetup::Instance()->IsActive(ECbmModuleId::kSts);
    bool useMuch = CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch);
    bool useTrd  = CbmSetup::Instance()->IsActive(ECbmModuleId::kTrd);
    bool useTof  = CbmSetup::Instance()->IsActive(ECbmModuleId::kTof);

    /** Invoke the detector interfaces **/
    if (useMvd) { fpMvdTrackingInterface = new CbmMvdTrackingInterface(); }
    if (useSts) { fpStsTrackingInterface = new CbmStsTrackingInterface(); }
    if (useMuch) { fpMuchTrackingInterface = new CbmMuchTrackingInterface(); }
    if (useTrd) { fpTrdTrackingInterface = new CbmTrdTrackingInterface(); }

    /** Add subtasks - tracker detector interfaces **/
    if (useMvd) { this->Add(fpMvdTrackingInterface); }
    if (useSts) { this->Add(fpStsTrackingInterface); }
    if (useMuch) { this->Add(fpMuchTrackingInterface); }
    if (useTrd) { this->Add(fpTrdTrackingInterface); }
  }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackingDetectorInterfaceInit::~CbmTrackingDetectorInterfaceInit()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
