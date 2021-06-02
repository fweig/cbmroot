/** @file CbmMCInputSet.cxx
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmMCInputSet.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.11.2018
 **/

#include "CbmMCInputSet.h"

#include "FairRootManager.h"
#include <Logger.h>

#include <cassert>


// -----   Default constructor   ---------------------------------------------
CbmMCInputSet::CbmMCInputSet() : CbmMCInputSet(-1.) {}
// ---------------------------------------------------------------------------


// -----   Constructor   -----------------------------------------------------
CbmMCInputSet::CbmMCInputSet(Double_t rate)
  : TObject()
  , fRate(rate)
  , fInputs()
  , fInputHandle()
  , fBranches()
  , fDeltaDist(nullptr)
{

  if (rate > 0.) {
    Double_t mean = 1.e9 / rate;  // mean time between events
    fDeltaDist    = new TF1("DeltaDist", "exp(-x/[0])/[0]", 0., 10. * mean);
    fDeltaDist->SetParameter(0, mean);
  }
  fInputHandle = fInputs.begin();
}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmMCInputSet::~CbmMCInputSet()
{
  if (fDeltaDist) delete fDeltaDist;
  for (auto const& entry : fInputs)
    if (entry.second) delete entry.second;
}
// ---------------------------------------------------------------------------


// -----   Set the branch address of an input branch   -----------------------
Bool_t CbmMCInputSet::ActivateObject(TObject** object, const char* branchName)
{

  // The branch address has to be set for each input chain
  for (auto const& mapEntry : fInputs) {
    CbmMCInput* input = mapEntry.second;
    assert(input);
    input->GetChain()->SetBranchStatus(branchName, 1);
    input->GetChain()->SetBranchAddress(branchName, object);
  }

  return kTRUE;
}
// ---------------------------------------------------------------------------


// -----   Add an input to the set   -----------------------------------------
void CbmMCInputSet::AddInput(UInt_t inputId, TChain* chain, ECbmTreeAccess mode)
{

  // Catch invalid chain pointer.
  if (!chain) {
    LOG(fatal) << "MCInputSet: invalid chain for input ID " << inputId << "!";
    return;
  }  //? No valid input chain

  // Catch input ID already being used.
  if (fInputs.find(inputId) != fInputs.end()) {
    LOG(fatal) << "MCInputSet: input ID " << inputId << " is already defined!";
    return;
  }  //? Input ID already used

  // Create CbmMCInput object
  CbmMCInput* input = new CbmMCInput(chain, mode);

  // The first input defines the reference branch list.
  if (fInputs.empty()) { fBranches = input->GetBranchList(); }  //? First input

  // Check compatibility of the input branch list with the reference list.
  else {
    if (!CheckBranchList(input)) {
      LOG(fatal) << "MCInputSet: Incompatible branch list!";
      return;
    }  //? Branch list not compatible
  }    //? Not first input

  // Register input and set input handle
  fInputs[inputId] = input;
  fInputHandle     = fInputs.begin();
}
// ---------------------------------------------------------------------------


// -----   Check the branch list of an input   -------------------------------
Bool_t CbmMCInputSet::CheckBranchList(CbmMCInput* input)
{

  assert(input);
  Bool_t success = kTRUE;
  for (auto const& entry : fBranches) {
    auto it = input->GetBranchList().find(entry);
    if (it == input->GetBranchList().end()) {
      LOG(debug) << "MCInputSet: Required branch " << entry << " not present in input!";
      success = kFALSE;
      break;
    }  //? Global branch not in input
  }    //# Global branches

  if (!success) {
    std::stringstream ss;
    ss << "MCInputSet: Reference branch list is ";
    for (auto const& entry : fBranches)
      ss << entry << " ";
    LOG(info) << ss.str();
    std::stringstream ss1;
    ss1 << "MCInputSet: Input branch list is ";
    for (auto const& entry : input->GetBranchList())
      ss1 << entry << " ";
    LOG(info) << ss1.str();
  }  //? Branches not compatible

  return success;
}
// ---------------------------------------------------------------------------


// -----   Time difference to next event   -----------------------------------
Double_t CbmMCInputSet::GetDeltaT()
{
  if (!fDeltaDist) return 0.;
  return fDeltaDist->GetRandom();
}
// ---------------------------------------------------------------------------


// -----   Maximal number of events to be read from the input   --------------
Int_t CbmMCInputSet::GetMaxNofEvents() const
{

  Int_t minimum = -1;

  for (auto const& entry : fInputs) {
    Int_t test = entry.second->GetMaxNofEvents();
    LOG(info) << "MCInputSet: Max. number of events for input " << entry.first << " is " << test;
    if (test >= 0 && (minimum == -1 || test < minimum)) minimum = test;
  }  //# Inputs

  minimum *= fInputs.size();
  LOG(info) << "MCInputSet: Maximal number of events is " << minimum;

  return minimum;
}
// ---------------------------------------------------------------------------


// -----   Get next entry from chain   ---------------------------------------
std::tuple<Bool_t, UInt_t, Int_t> CbmMCInputSet::GetNextEntry()
{

  // Flag for having reached the last input
  Bool_t allInputsUsed = kFALSE;

  // The input handle points to the input to be used
  Int_t entry   = fInputHandle->second->GetNextEntry();
  Int_t inputId = fInputHandle->first;

  // Increment input handle. If end of set reached, signal that and
  // reset the handle to the begin.
  fInputHandle++;
  if (fInputHandle == fInputs.end()) {
    allInputsUsed = kTRUE;
    fInputHandle  = fInputs.begin();
  }

  return std::make_tuple(allInputsUsed, inputId, entry);
}
// ---------------------------------------------------------------------------


// -----   Register input chains to FairRootManager   ------------------------
void CbmMCInputSet::RegisterChains()
{
  for (auto const& mapEntry : fInputs) {
    FairRootManager::Instance()->SetInChain(mapEntry.second->GetChain(), mapEntry.first);
  }
}
// ---------------------------------------------------------------------------


ClassImp(CbmMCInputSet)
