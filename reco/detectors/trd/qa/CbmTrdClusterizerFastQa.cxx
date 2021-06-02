/** @file CbmTrdClusterizerFastQa.cxx
  * @copyright Copyright (C) 2013-2016 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Cyrano Bergmann [committer] **/

// -----------------------------------------------------------------------
// -----                     CbmTrdClusterizerFastQa                  -----
// -----               Created 19.03.13  by C. Bergmann               -----
// -----------------------------------------------------------------------


#include "CbmTrdClusterizerFastQa.h"

#include "CbmMCTrack.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"
#include "CbmTrdTrack.h"

#include "FairRootManager.h"

#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TVector3.h"

#include <iostream>
using std::cout;
using std::endl;

CbmTrdClusterizerFastQa::CbmTrdClusterizerFastQa() : CbmTrdClusterizerFastQa("TrdClusterizerFastQa", "") {}

CbmTrdClusterizerFastQa::CbmTrdClusterizerFastQa(const char* name, const char*) : FairTask(name) {}

CbmTrdClusterizerFastQa::~CbmTrdClusterizerFastQa() {}

InitStatus CbmTrdClusterizerFastQa::Init()
{
  FairRootManager* rootMgr = FairRootManager::Instance();
  if (NULL == rootMgr) {
    cout << "-E- CbmTrdClusterizerFastQa::Init : "
         << "ROOT manager is not instantiated !" << endl;
    return kFATAL;
  }
  PrepareHistograms();
  return kSUCCESS;
}
void CbmTrdClusterizerFastQa::Exec(Option_t*) {}
void CbmTrdClusterizerFastQa::Finish() { WriteHistograms(); }
void CbmTrdClusterizerFastQa::PrepareHistograms() {}
void CbmTrdClusterizerFastQa::WriteHistograms()
{
  gDirectory->mkdir("CbmTrdClusterizerFastQa");
  gDirectory->cd("CbmTrdClusterizerFastQa");
  gDirectory->cd("..");
}
ClassImp(CbmTrdClusterizerFastQa);
