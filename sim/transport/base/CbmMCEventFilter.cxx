/** @file CbmMCEventFilter.cxx
  * @copyright Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer], Florian Uhlig **/

/** @file CbmMCEventFilter.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 07.08.2019
 **/

#include "CbmMCEventFilter.h"

#include <Logger.h>

#include <cassert>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;


// ----- Constructor  -------------------------------------------------------
CbmMCEventFilter::CbmMCEventFilter()
  : FairTask("MCEventFilter")
  , fData()
  , fMinNofData()
  , fNofEventsIn(0)
  , fNofEventsOut(0)
{
}
// --------------------------------------------------------------------------


// -----   Get a data object by index   -------------------------------------
TObject* CbmMCEventFilter::GetData(ECbmDataType type, Int_t index) const
{
  if (index < 0 || index >= GetNofData(type)) return nullptr;
  return fData.at(type)->UncheckedAt(index);
}
// --------------------------------------------------------------------------


// -----   Execution   ------------------------------------------------------
void CbmMCEventFilter::Exec(Option_t*)
{

  fNofEventsIn++;
  Bool_t test = SelectEvent();
  if (test) {
    LOG(INFO) << GetName() << ": Current event " << fNofEventsIn << " selected for output";
    fNofEventsOut++;
  }  //? Event selected
  else
    LOG(INFO) << GetName() << ": Current event " << fNofEventsIn << " discarded for output";
  FairMCApplication::Instance()->SetSaveCurrentEvent(test);
}
// --------------------------------------------------------------------------


// -----   End-of-run action   ----------------------------------------------
void CbmMCEventFilter::Finish()
{

  cout << endl;
  LOG(INFO) << GetName() << ": Number of input events  " << fNofEventsIn;
  LOG(INFO) << GetName() << ": Number of output events " << fNofEventsOut << " = "
            << 100. * Double_t(fNofEventsOut) / Double_t(fNofEventsIn) << " %";
  cout << endl;
}
// --------------------------------------------------------------------------


// -----   Initialisation   -------------------------------------------------
InitStatus CbmMCEventFilter::Init()
{

  GetBranch(ECbmDataType::kMCTrack);
  GetBranch(ECbmDataType::kMvdPoint);
  GetBranch(ECbmDataType::kStsPoint);
  GetBranch(ECbmDataType::kRichPoint);
  GetBranch(ECbmDataType::kMuchPoint);
  GetBranch(ECbmDataType::kTrdPoint);
  GetBranch(ECbmDataType::kTofPoint);
  GetBranch(ECbmDataType::kPsdPoint);

  return kSUCCESS;
}
// --------------------------------------------------------------------------


// -----   Get a branch of MC data from FairRootManager   -------------------
void CbmMCEventFilter::GetBranch(ECbmDataType type)
{

  FairRootManager* rm = FairRootManager::Instance();
  assert(rm);

  TString branchName = GetBranchName(type);
  if (!branchName.IsNull()) {
    fData[type] = dynamic_cast<TClonesArray*>(rm->GetObject(branchName));
    if (fData.at(type)) { LOG(INFO) << GetName() << ": Add branch " << branchName; }
  }
}
// --------------------------------------------------------------------------


// -----   Get branch name of data type   -----------------------------------
TString CbmMCEventFilter::GetBranchName(ECbmDataType type) const
{

  TString name = "";
  switch (type) {
    case ECbmDataType::kMCTrack: name = "MCTrack"; break;
    case ECbmDataType::kMvdPoint: name = "MvdPoint"; break;
    case ECbmDataType::kStsPoint: name = "StsPoint"; break;
    case ECbmDataType::kRichPoint: name = "RichPoint"; break;
    case ECbmDataType::kMuchPoint: name = "MuchPoint"; break;
    case ECbmDataType::kTrdPoint: name = "TrdPoint"; break;
    case ECbmDataType::kTofPoint: name = "TofPoint"; break;
    case ECbmDataType::kPsdPoint: name = "PsdPoint"; break;
    default: name = ""; break;
  }

  return name;
}
// --------------------------------------------------------------------------


// -----   Event selector   -------------------------------------------------
Bool_t CbmMCEventFilter::SelectEvent() const
{

  LOG(INFO) << GetName() << ": " << Statistics();
  Bool_t check = kTRUE;
  for (auto cut : fMinNofData) {
    if (GetNofData(cut.first) < cut.second) {
      LOG(INFO) << GetName() << ": Cut on branch " << GetBranchName(cut.first) << " not passed (number of data "
                << GetNofData(cut.first) << ", required " << cut.second << ")";
      check = kFALSE;
      break;
    }
  }

  return check;
}
// --------------------------------------------------------------------------


// -----   Statistics info  -------------------------------------------------
std::string CbmMCEventFilter::Statistics() const
{

  std::stringstream ss;
  ss << "MCTracks " << GetNofData(ECbmDataType::kMCTrack) << ", Points: ";
  if (fData.at(ECbmDataType::kMvdPoint)) ss << "MVD " << GetNofData(ECbmDataType::kMvdPoint) << "  ";
  if (fData.at(ECbmDataType::kStsPoint)) ss << "STS " << GetNofData(ECbmDataType::kStsPoint) << "  ";
  if (fData.at(ECbmDataType::kRichPoint)) ss << "RICH " << GetNofData(ECbmDataType::kRichPoint) << "  ";
  if (fData.at(ECbmDataType::kMuchPoint)) ss << "MUCH " << GetNofData(ECbmDataType::kMuchPoint) << "  ";
  if (fData.at(ECbmDataType::kTrdPoint)) ss << "TRD " << GetNofData(ECbmDataType::kTrdPoint) << "  ";
  if (fData.at(ECbmDataType::kTofPoint)) ss << "TOF " << GetNofData(ECbmDataType::kTofPoint) << "  ";
  if (fData.at(ECbmDataType::kPsdPoint)) ss << "PSD " << GetNofData(ECbmDataType::kPsdPoint) << "  ";

  return ss.str();
}
// --------------------------------------------------------------------------


ClassImp(CbmMCEventFilter)
