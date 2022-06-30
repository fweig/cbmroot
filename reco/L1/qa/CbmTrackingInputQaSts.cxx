/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackingInputQaSts.cxx
 * @brief  Class for the tracking input QA for STS (definition)
 * @since  27.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmTrackingInputQaSts.h"

#include "CbmDigiManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"
#include "CbmMatch.h"
#include "CbmSetup.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsTrackingInterface.h"

ClassImp(CbmTrackingInputQaSts);

// --------------------------------------------------------------------------------------------------------------------
//
CbmTrackingInputQaSts::CbmTrackingInputQaSts(int verbose) : CbmTrackingInputQaBase("CbmTrackingInputQaSts", verbose)
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaSts::CbmTrackingInputQaSts()\n";

  // Setup detector-specific parameters
  fDetId              = ECbmModuleId::kSts;
  fDetName            = "sts";
  fDetTitle           = "STS";
  fpDetectorInterface = CbmStsTrackingInterface::Instance();
}

// --------------------------------------------------------------------------------------------------------------------
//
CbmTrackingInputQaSts::~CbmTrackingInputQaSts() {}

// --------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaSts::Init()
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaSts::Init()\n";
  // Check, if STS is in the setup
  if (!CbmSetup::Instance()->IsActive(ECbmModuleId::kSts)) {
    LOG(warn) << "STS is not active in the current setup";
    return kSUCCESS;
  }

  // Initialize base histograms
  CbmTrackingInputQaBase::Init();

  // Additional STS-specific histograms can be initialized here
  // ...

  return kSUCCESS;
}

// --------------------------------------------------------------------------------------------------------------------
//


// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
