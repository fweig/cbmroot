/** @file CbmStsHitProducerIdealWrapper.cxx
  * @copyright Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors V. Friese, Florian Uhlig [committer] **/

// -------------------------------------------------------------------------
// -----                CbmStsHitProducerIdealWrapper source file             -----
// -----                  Created 10/01/06  by V. Friese               -----
// -------------------------------------------------------------------------
#include "CbmStsHitProducerIdealWrapper.h"

#include "CbmStsHit.h"
#include "CbmTrdParSetGas.h"

#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include "TClonesArray.h"

#include <iostream>

using std::cout;
using std::endl;

// -----   Default constructor   -------------------------------------------
CbmStsHitProducerIdealWrapper::CbmStsHitProducerIdealWrapper() : FairTask("Ideal STS Hit Producer Task") {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsHitProducerIdealWrapper::~CbmStsHitProducerIdealWrapper() {}
// -------------------------------------------------------------------------

// -----   Public method Init   --------------------------------------------
InitStatus CbmStsHitProducerIdealWrapper::Init()
{

  // Get RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- CbmStsHitProducerIdealWrapper::Init: "
         << "RootManager not instantised!" << endl;
    return kFATAL;
  }

  // Get input array
  fPointArray = (TClonesArray*) ioman->GetObject("StsPoint");
  if (!fPointArray) {
    cout << "-W- CbmStsHitProducerIdealWrapper::Init: "
         << "No STSPoint array!" << endl;
    return kERROR;
  }

  // Create and register output array
  fHitArray = new TClonesArray("CbmStsHit");
  ioman->Register("StsHit", "STS", fHitArray, IsOutputBranchPersistent("StsHit"));


  fAlgo->Init();
  fAlgo->InitContainers();

  cout << "-I- CbmStsHitProducerIdealWrapper: Intialisation successfull" << endl;
  return kSUCCESS;
}
// -------------------------------------------------------------------------

void CbmStsHitProducerIdealWrapper::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();

  TList* fParCList = fAlgo->GetParList();

  for (Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);

    std::string sParamName {tempObj->GetName()};
    FairParGenericSet* newObj =
      dynamic_cast<FairParGenericSet*>(FairRun::Instance()->GetRuntimeDb()->getContainer(sParamName.data()));

    if (nullptr == newObj) {
      LOG(error) << "Failed to obtain parameter container " << sParamName << ", for parameter index " << iparC;
      return;
    }  // if( nullptr == newObj )

    fParCList->AddAt(newObj, iparC);
    //      delete tempObj;
  }  // for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
}

std::vector<CbmStsPoint> CbmStsHitProducerIdealWrapper::Convert(TClonesArray* arr)
{

  std::vector<CbmStsPoint> vec;
  Int_t entries = arr->GetEntriesFast();
  if (entries > 0) {
    CbmStsPoint* point = static_cast<CbmStsPoint*>(arr->At(0));
    LOG(info) << "Entries in TCA for data type " << point->GetName() << ": " << entries;
  }
  for (int i = 0; i < entries; ++i) {
    CbmStsPoint* point = static_cast<CbmStsPoint*>(arr->At(i));
    vec.emplace_back(*point);
  }
  return vec;
}


// -----   Public method Exec   --------------------------------------------
void CbmStsHitProducerIdealWrapper::Exec(Option_t* /*opt*/)
{

  // Reset output array
  if (!fHitArray) Fatal("Exec", "No StsHit array");

  //  fHitArray->Clear();
  fHitArray->Delete();

  // ConvertToVector
  std::vector<CbmStsPoint> points = Convert(fPointArray);

  // Pass the vector to the algorithm
  // Get the vector with the newly created data objects from the algorithm
  std::vector<CbmStsHit> hits = fAlgo->ProcessInputData(points);

  // Fill the content of vector into TCA
  int iPoint = 0;
  for (const auto& hit : hits) {
    new ((*fHitArray)[iPoint]) CbmStsHit(hit);
    iPoint++;
  }

  // Event summary
  cout << "-I- CbmStsHitProducerIdealWrapper: " << points.size() << " StsPoints, " << hits.size() << " Hits created."
       << endl;
}
// -------------------------------------------------------------------------

ClassImp(CbmStsHitProducerIdealWrapper)
