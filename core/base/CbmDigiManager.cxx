/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

/** @file CbmDigiManager.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 8 May 2007
 **/
#include "CbmDigiManager.h"

#include "CbmBmonDigi.h"          // for CbmBmonDigi
#include "CbmDefs.h"              // for kMuch
#include "CbmDigiBranch.h"        // for CbmDigiBranch
#include "CbmMuchBeamTimeDigi.h"  // for CbmMuchBeamTimeDigi
#include "CbmMuchDigi.h"          // for CbmMuchDigi
#include "CbmMvdDigi.h"           // for CbmMvdDigi
#include "CbmPsdDigi.h"           // for CbmPsdDigi
#include "CbmRichDigi.h"          // for CbmRichDigi
#include "CbmStsDigi.h"           // for CbmStsDigi
#include "CbmTofDigi.h"           // for CbmTofDigi
#include "CbmTrdDigi.h"           // for CbmTrdDigi

#include <FairTask.h>  // for kSUCCESS, InitStatus

#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TString.h>            // for operator+, operator<<

#include <iostream>  // for string, endl, basic_ostream, cout
#include <string>    // for operator==, basic_string


using std::string;

// -----   Initialisation of static variables   ----------------------------
CbmDigiManager* CbmDigiManager::fgInstance                           = nullptr;
std::map<ECbmModuleId, CbmDigiBranchBase*> CbmDigiManager::fBranches = std::map<ECbmModuleId, CbmDigiBranchBase*>();
Bool_t CbmDigiManager::fIsInitialised                                = kFALSE;
Bool_t CbmDigiManager::fUseMuchBeamTimeDigi                          = kFALSE;
// -------------------------------------------------------------------------


// -----   Constructor   ---------------------------------------------------
CbmDigiManager::CbmDigiManager() {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmDigiManager::~CbmDigiManager()
{
  for (auto& entry : fBranches) {
    if (entry.second) delete entry.second;
  }
}
// -------------------------------------------------------------------------


// -----   Get a match object   --------------------------------------------
const CbmMatch* CbmDigiManager::GetMatch(ECbmModuleId systemId, UInt_t index) const
{
  assert(fIsInitialised);
  if (fBranches.find(systemId) == fBranches.end()) return nullptr;
  return fBranches[systemId]->GetDigiMatch(index);
}

// -----   Get number of digis in branch   ---------------------------------
Int_t CbmDigiManager::GetNofDigis(ECbmModuleId systemId)
{
  assert(fIsInitialised);
  if (fBranches.find(systemId) == fBranches.end()) return -1;
  return fBranches[systemId]->GetNofDigis();
}
// -------------------------------------------------------------------------


// -----   Initialisation   ------------------------------------------------
InitStatus CbmDigiManager::Init()
{

  if (fIsInitialised) return kSUCCESS;

  std::cout << std::endl << std::endl;
  LOG(info) << "==================================================";
  LOG(info) << "DigiManager: Initialising...";

  SetBranch<CbmMvdDigi>();
  SetBranch<CbmStsDigi>();
  SetBranch<CbmRichDigi>();
  if (fUseMuchBeamTimeDigi) SetBranch<CbmMuchBeamTimeDigi>();
  else
    SetBranch<CbmMuchDigi>();
  SetBranch<CbmTrdDigi>();
  SetBranch<CbmTofDigi>();
  SetBranch<CbmPsdDigi>();
  SetBranch<CbmBmonDigi>();
  LOG(info) << "Present branches:";
  for (auto const& branch : fBranches) {
    LOG(info) << "   " << branch.second->ToString();
  }

  fIsInitialised = kTRUE;
  LOG(info) << "==================================================";
  std::cout << std::endl << std::endl;

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Check presence of a match branch   ------------------------------
Bool_t CbmDigiManager::IsMatchPresent(ECbmModuleId systemId)
{
  if (fBranches.find(systemId) == fBranches.end()) return kFALSE;
  return fBranches[systemId]->HasMatches();
}
// -------------------------------------------------------------------------


// -----   Check presence of a digi branch   -------------------------------
Bool_t CbmDigiManager::IsPresent(ECbmModuleId systemId)
{
  if (fBranches.find(systemId) == fBranches.end()) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------


// -----   Set a digi branch   ---------------------------------------------
template<class Digi>
void CbmDigiManager::SetBranch()
{

  // Get system ID and class name from digi class.
  ECbmModuleId systemId = Digi::GetSystem();
  string className      = Digi::GetClassName();

  // TODO: Remove ugly fix for CbmMuchBeamTimeDigi once class has disappeared.
  if (systemId == ECbmModuleId::kMuch && fUseMuchBeamTimeDigi) className = "CbmMuchBeamTimeDigi";

  // --- Catch branch being already set
  if (fBranches.find(systemId) != fBranches.end()) {
    LOG(warn) << "DigiManager: Branch for system " << systemId << " is already set.";
    return;
  }  //? branch already present

  // --- Branch name. If not set explicitly, taken from the class name
  // --- minus the leading "Cbm" (CBM convention)
  string branchName {};
  if (fBranchNames.find(systemId) != fBranchNames.end()) {
    branchName = fBranchNames[systemId];
  }  //? branch name explicitly set
  else {
    if (className.substr(0, 3) == "Cbm") branchName = className.substr(3);
    else
      branchName = className;
  }  //? Branch name not explicitly set

  // --- Add branch object and connect it to the tree
  CbmDigiBranchBase* branch = new CbmDigiBranch<Digi>(branchName.c_str());
  if (branch->ConnectToTree()) {
    LOG(debug) << "DigiManager: Search branch " << branchName << " for class " << className << ": successful";
    fBranches[systemId] = branch;
  }
  else {
    LOG(debug) << "DigiManager: Search branch " << branchName << " for class " << className << ": failed";
    delete branch;
  }

  // Special cases for mCBM TOF
  if (systemId == ECbmModuleId::kTof) {
    if (fBranches.find(systemId) == fBranches.end()) {
      branchName = "TofCalDigi";
      branch     = new CbmDigiBranch<Digi>(branchName.c_str());
      if (branch->ConnectToTree()) {
        LOG(info) << "DigiManager: Search branch " << branchName << " for class " << className << ": successful";
        fBranches[systemId] = branch;
      }
      else {
        LOG(info) << "DigiManager: Search branch " << branchName << " for class " << className << ": failed";
        delete branch;
      }
    }
    if (fBranches.find(systemId) == fBranches.end()) {
      branchName = "CbmTofDigi";
      branch     = new CbmDigiBranch<Digi>(branchName.c_str());
      if (branch->ConnectToTree()) {
        LOG(info) << "DigiManager: Search branch " << branchName << " for class " << className << ": successful";
        fBranches[systemId] = branch;
      }
      else {
        LOG(info) << "DigiManager: Search branch " << branchName << " for class " << className << ": failed";
        delete branch;
      }
    }
    if (fBranches.find(systemId) == fBranches.end()) {
      branchName = "CbmTofCalDigi";
      branch     = new CbmDigiBranch<Digi>(branchName.c_str());
      if (branch->ConnectToTree()) fBranches[systemId] = branch;
      else
        delete branch;
    }
  }
}
// -------------------------------------------------------------------------


ClassImp(CbmDigiManager)
