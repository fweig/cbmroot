/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackerDetInitializer.cxx
 * @brief  FairTask for the tracker detector interfaces initialization (implementation)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "FairTask.h"
#include "CbmMuchTrackerIF.h"
#include "CbmMvdTrackerIF.h"
#include "CbmSetup.h"
#include "CbmStsTrackerIF.h"
#include "CbmTrackerDetInitializer.h"
#include "CbmTrdTrackerIF.h"
#include <FairLogger.h>

ClassImp(CbmTrackerDetInitializer)

CbmTrackerDetInitializer* CbmTrackerDetInitializer::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackerDetInitializer::CbmTrackerDetInitializer()
: FairTask("CbmTrackerDetInitializer")
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
    if (useMvd)  { fpMvdTrackerIF  = new CbmMvdTrackerIF(); }
    if (useSts)  { fpStsTrackerIF  = new CbmStsTrackerIF(); }
    if (useMuch) { fpMuchTrackerIF = new CbmMuchTrackerIF(); }
    if (useTrd)  { fpTrdTrackerIF  = new CbmTrdTrackerIF(); }

    /** Add subtasks - tracker detector interfaces **/
    if (useMvd)  { this->Add(fpMvdTrackerIF); }
    if (useSts)  { this->Add(fpStsTrackerIF); }
    if (useMuch) { this->Add(fpMuchTrackerIF); }
    if (useTrd)  { this->Add(fpTrdTrackerIF); }
  }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackerDetInitializer::~CbmTrackerDetInitializer()
{
  if (fpInstance == this) { fpInstance = nullptr; }
}

//-------------------------------------------------------------------------------------------------------------------------------------
//

