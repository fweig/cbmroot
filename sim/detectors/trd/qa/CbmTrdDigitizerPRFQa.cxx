/** @file CbmTrdDigitizerPRFQa.cxx
  * @copyright Copyright (C) 2013-2016 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Cyrano Bergmann [committer] **/

// -----------------------------------------------------------------------
// -----                     CbmTrdDigitizerPRFQa                     -----
// -----               Created 19.03.13  by C. Bergmann               -----
// -----------------------------------------------------------------------


#include "CbmTrdDigitizerPRFQa.h"

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

CbmTrdDigitizerPRFQa::CbmTrdDigitizerPRFQa() : CbmTrdDigitizerPRFQa("TrdDigitizerPRFQa", "") {}

CbmTrdDigitizerPRFQa::CbmTrdDigitizerPRFQa(const char* name, const char*) : FairTask(name) {}

CbmTrdDigitizerPRFQa::~CbmTrdDigitizerPRFQa() {}

InitStatus CbmTrdDigitizerPRFQa::Init()
{
  FairRootManager* rootMgr = FairRootManager::Instance();
  if (NULL == rootMgr) {
    cout << "-E- CbmTrdDigitizerPRFQa::Init : "
         << "ROOT manager is not instantiated !" << endl;
    return kFATAL;
  }
  PrepareHistograms();
  return kSUCCESS;
}
void CbmTrdDigitizerPRFQa::Exec(Option_t*) {}
void CbmTrdDigitizerPRFQa::Finish() { WriteHistograms(); }
void CbmTrdDigitizerPRFQa::PrepareHistograms() {}
void CbmTrdDigitizerPRFQa::WriteHistograms()
{
  gDirectory->mkdir("CbmTrdDigitizerPRFQa");
  gDirectory->cd("CbmTrdDigitizerPRFQa");
  gDirectory->cd("..");
}
ClassImp(CbmTrdDigitizerPRFQa);
