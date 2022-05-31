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
#include "CbmTrackerDetInitializer.h"
#include "CbmTrackerInterfaceSts.h"
#include "CbmTrackerInterfaceMvd.h"
#include <FairLogger.h>

ClassImp(CbmTrackerDetInitializer)

CbmTrackerDetInitializer* CbmTrackerDetInitializer::fpInstance = nullptr;

//-------------------------------------------------------------------------------------------------------------------------------------
//
CbmTrackerDetInitializer::CbmTrackerDetInitializer(): FairTask("CbmTrackerDetInitializer")
{
  if (!fpInstance) { 
    fpInstance = this; 
    
    /** Add subtasks - tracker detector interfaces **/
    this->Add(new CbmTrackerInterfaceMvd());
    this->Add(new CbmTrackerInterfaceSts());
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

