/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackingInputQaSts.h
 * @brief  Class for the tracking input QA for STS (declaration)
 * @since  27.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmTrackingInputQaSts_h
#define CbmTrackingInputQaSts_h 1

#include "CbmTrackingInputQaBase.h"

#include "FairRootManager.h"

#include "TClonesArray.h"

class CbmMCDataArray;
class CbmMatch;
class CbmCluster;

class CbmTrackingInputQaSts : public CbmTrackingInputQaBase {
public:
  CbmTrackingInputQaSts(int verbose = 1);
  ~CbmTrackingInputQaSts();

  InitStatus Init();


  /// Gets pointer to hits array of a particular detector type
  TClonesArray* GetHitsInput() { return dynamic_cast<TClonesArray*>(FairRootManager::Instance()->GetObject("StsHit")); }

  /// Gets pointer to clusters array of a particular detector type
  TClonesArray* GetClustersInput()
  {
    return dynamic_cast<TClonesArray*>(FairRootManager::Instance()->GetObject("StsCluster"));
  }

  /// Gets pointer to hit matches array for a particular detector type
  TClonesArray* GetHitMatchesInput()
  {
    return dynamic_cast<TClonesArray*>(FairRootManager::Instance()->GetObject("StsHitMatch"));
  }

  /// Gets pointer to MC points for a particular detector type
  CbmMCDataArray* GetMcPointsInput() { return fpMcManager->InitBranch("StsPoint"); }


  ClassDef(CbmTrackingInputQaSts, 0);
};

#endif  // CbmTrackingInputQaSts_h
