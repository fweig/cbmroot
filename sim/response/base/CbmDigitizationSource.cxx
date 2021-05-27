/*
 * CbmDigitizationSource.cxx
 *
 *  Created on: 06.11.2018
 *      Author: vfriese
 */

#include "CbmDigitizationSource.h"

#include "FairRootManager.h"

#include "TChain.h"
#include "TFolder.h"
#include "TROOT.h"
#include <TFile.h>

#include <cassert>
#include <iomanip>
#include <utility>


// -----   Constructor   -----------------------------------------------------
CbmDigitizationSource::CbmDigitizationSource()
  : FairSource()
  , fInputSets()
  , fInputMap()
  , fNextEvent()
  , fMCEventHeader()
  , fListOfFolders(new TObjArray())
  , fBranches()
  , fTimeStart(1000.)
  , fCurrentTime(0.)
  , fCurrentEntryId(0)
  , fCurrentInputId(0)
  , fCurrentRunId(0)
  , fFirstCall(kTRUE)
  , fEventMode(kFALSE)
  , fCurrentInputSet(nullptr)
  , fSwitchInputSet(kFALSE)
{
}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmDigitizationSource::~CbmDigitizationSource()
{
  for (auto const& inputSet : fInputSets)
    if (inputSet) delete inputSet;
  fBranches.clear();
}
// ---------------------------------------------------------------------------


// -----   Set the branch address of a branch   -----------------------------
Bool_t CbmDigitizationSource::ActivateObject(TObject** object, const char* branchName)
{

  // The branch address has to be set for each input chain of each input set
  for (auto const& inputSet : fInputSets)
    inputSet->ActivateObject(object, branchName);

  return kTRUE;
}
// ---------------------------------------------------------------------------


// -----   Add a transport input   -------------------------------------------
void CbmDigitizationSource::AddInput(UInt_t inputId, TChain* chain, Double_t rate, ECbmTreeAccess mode)
{

  // Catch input ID already being used
  if (fInputMap.find(inputId) != fInputMap.end()) {
    LOG(fatal) << "DigitizationSource: Input ID " << inputId << " is already defined!";
    return;
  }  //? Input ID already in use

  // Create a new inputSet and add the input to it
  CbmMCInputSet* inputSet = new CbmMCInputSet(rate);
  inputSet->AddInput(inputId, chain, mode);

  // If it is the first input set, it defines the reference branch list.
  if (fInputSets.size() == 0) { fBranches = inputSet->GetBranchList(); }  //? First input set

  // If it is not the first set, check compatibility of branch list.
  else {
    if (!CheckBranchList(inputSet)) {
      LOG(fatal) << "DigitizationSource: Incompatible branch list!";
      return;
    }  //? Branch list in new input set not compatible
  }    //? Not the first input set

  // Register the new input set and input
  fInputSets.push_back(inputSet);
  fInputMap[inputId] = inputSet;

  LOG(info) << "DigitizationSource: Added input " << inputId << " with rate " << rate << " / s, mode: "
            << (mode == ECbmTreeAccess::kRegular ? "regular" : (mode == ECbmTreeAccess::kRepeat ? "repeat" : "random"));
}
// ---------------------------------------------------------------------------


// -----   Check the branch list of an input   -------------------------------
Bool_t CbmDigitizationSource::CheckBranchList(CbmMCInputSet* inputSet)
{

  assert(inputSet);
  Bool_t success = kTRUE;
  for (auto const& entry : fBranches) {
    auto it = inputSet->GetBranchList().find(entry);
    if (it == inputSet->GetBranchList().end()) {
      LOG(debug) << "DigitizationSource: required branch " << entry << " not present in input set!";
      success = kFALSE;
      break;
    }  //? Global branch not in input
  }    //# Global branches

  if (!success) {
    std::stringstream ss;
    ss << "DigitizationSource: Global branch list is ";
    for (auto const& entry : fBranches)
      ss << entry << " ";
    LOG(info) << ss.str();
    std::stringstream ss1;
    ss1 << "DigitizationSource: Input set branch list is ";
    for (auto const& entry : inputSet->GetBranchList()) {
      ss1 << entry << " ";
    }
    LOG(info) << ss1.str();
  }  //? Branches not compatible

  return success;
}
// ---------------------------------------------------------------------------


// -----   Check the maximal entry the source can run to   -------------------
Int_t CbmDigitizationSource::CheckMaxEventNo(Int_t)
{

  // Catch the case when no input is connected
  if (fInputSets.empty()) return 0;

  Int_t maxEvents = (fInputSets.size() == 1 ? fInputSets.front()->GetMaxNofEvents() : -1);

  // If the maximal number of events is not defined, the method returns a
  // practically infinite number. The run will proceed until one of the
  // inputs is exhausted or if terminated by CTRL+C.
  // This is the case when there is more than one input set (mixing)
  // or when the only input set has only unlimited inputs (all inputs
  // are accessed with mode kRepeat or kRandom).
  return (maxEvents >= 0 ? maxEvents : 1e6);
}
// ---------------------------------------------------------------------------


// -----   Embed a transport input   -----------------------------------------
void CbmDigitizationSource::EmbedInput(UInt_t inputId, TChain* chain, UInt_t targetInputId, ECbmTreeAccess mode)
{

  // Catch input ID already being used
  if (fInputMap.find(inputId) != fInputMap.end()) {
    LOG(fatal) << "DigitizationSource: Input ID " << inputId << " is already defined!";
    return;
  }  //? Input ID already in use

  // Catch target input not existing
  if (fInputMap.find(targetInputId) == fInputMap.end()) {
    LOG(fatal) << "DigitizationSource: Target input ID " << targetInputId << " for input " << inputId
               << " does not exist!";
    return;
  }  //? Target input does not exist

  // Add the new input to the respective input set
  fInputMap[targetInputId]->AddInput(inputId, chain, mode);
  fInputMap[inputId] = fInputMap[targetInputId];

  LOG(info) << "DigitizationSource: Embedded input " << inputId << " into input " << targetInputId << ", mode: "
            << (mode == ECbmTreeAccess::kRegular ? "regular" : (mode == ECbmTreeAccess::kRepeat ? "repeat" : "random"));
}
// ---------------------------------------------------------------------------


// -----   Fill the event header   -------------------------------------------
void CbmDigitizationSource::FillEventHeader(FairEventHeader* event)
{
  assert(event);
  event->SetEventTime(fCurrentTime);
  event->SetMCEntryNumber(fCurrentEntryId);
  event->SetInputFileId(fCurrentInputId);
  event->SetRunId(fCurrentRunId);
  LOG(debug) << "DigitizationSource: Event with RunId " << fCurrentRunId << ", input " << fCurrentInputId << ", entry "
             << fCurrentEntryId << ", time " << fCurrentTime << " ns";
}
// ---------------------------------------------------------------------------


// -----   Get an input   ----------------------------------------------------
CbmMCInput* CbmDigitizationSource::GetFirstInput()
{
  assert(!fInputMap.empty());
  return fInputMap.begin()->second->GetFirstInput().second;
}
// ---------------------------------------------------------------------------


// -----   Initialisation   --------------------------------------------------
Bool_t CbmDigitizationSource::Init()
{

  // Catch missing or too many input sets
  if (fInputSets.empty()) {
    LOG(fatal) << "DigitizationSource: No input sets defined!";
    return kFALSE;
  }
  if (fEventMode && fInputMap.size() != 1) {
    LOG(fatal) << "DigitizationSource: More than one input defined "
               << "in event-by-event mode!";
    return kFALSE;
  }

  // Register all input chains to FairRootManager
  for (auto const& inputSet : fInputSets)
    inputSet->RegisterChains();

  // Get folder from first input file and register it to FairRootManager
  CbmMCInput* input = fInputSets.front()->GetFirstInput().second;
  TFile* file       = input->GetChain()->GetFile();
  TFolder* folder   = dynamic_cast<TFolder*>(file->Get("cbmroot"));
  assert(folder);
  gROOT->GetListOfBrowsables()->Add(folder);
  fListOfFolders->Add(folder);
  FairRootManager::Instance()->SetListOfFolders(fListOfFolders);

  // Activate the MC event header to all respective input branches.
  // This is necessary since it is used from this class.
  // The other branches will be activated if needed by a task
  // through the method ActivateObject called from FairRootManager
  fMCEventHeader   = new FairMCEventHeader();
  TObject** object = reinterpret_cast<TObject**>(&fMCEventHeader);
  ActivateObject(object, "MCEventHeader.");

  // Set the time of the first event for each input set
  if (!fEventMode) {
    for (size_t iSet = 0; iSet < fInputSets.size(); iSet++) {
      CbmMCInputSet* inputSet = fInputSets.at(iSet);
      Double_t time           = fTimeStart + inputSet->GetDeltaT();
      LOG(info) << "First time for input set " << iSet << " is " << time << " ns.";
      fNextEvent.insert(std::make_pair(time, inputSet));
    }  //# Input sets
  }

  // Select the input set with smallest event time
  fCurrentTime     = fNextEvent.begin()->first;
  fCurrentInputSet = fNextEvent.begin()->second;

  return kTRUE;
}
// ---------------------------------------------------------------------------


// -----   Define one input event   ------------------------------------------
Int_t CbmDigitizationSource::ReadEvent(UInt_t event)
{

  // Before the actual run, ReadEvent is once called from FairRunAna::Init().
  // This is to get the run ID needed for setting the parameter containers.
  // Since the input entries are read sequentially, this would mean
  // always losing the first entry. We here protect against that by calling
  // the first entry of the first input directly, without incrementing its
  // current entry bookkeeper.
  if (fFirstCall) {
    ReadRunId();
    fFirstCall = kFALSE;
    return 0;
  }

  // In the event-by-event mode, get the respective event from the first
  // input; the event time is zero.
  if (fEventMode) return ReadEventByEvent(event);

  // If the last used input set was exhausted, switch to a the next one
  if (fSwitchInputSet) {

    // Delete this entry from the list of next input sets
    fNextEvent.erase(fNextEvent.begin());

    // Generate a new event time for this input set and add it to the list
    Double_t newTime = fCurrentTime + fCurrentInputSet->GetDeltaT();
    fNextEvent.insert(std::make_pair(newTime, fCurrentInputSet));

    // Store current time and input set
    fCurrentTime     = fNextEvent.begin()->first;
    fCurrentInputSet = fNextEvent.begin()->second;

  }  //? Switch to next input set

  // Get the next entry from the current input set
  assert(fCurrentInputSet);
  auto result     = fCurrentInputSet->GetNextEntry();
  fSwitchInputSet = std::get<0>(result);
  fCurrentInputId = std::get<1>(result);
  fCurrentEntryId = std::get<2>(result);
  std::cout << std::endl;
  LOG(info) << "DigitizationSource: Event " << event << " at t = " << std::fixed << std::setprecision(3) << fCurrentTime
            << " ns"
            << " from input " << fCurrentInputId << " (entry " << fCurrentEntryId << ")";

  // Stop the run if the number of entries in this input is reached
  if (fCurrentEntryId < 0) {
    LOG(info) << "DigitizationSource: No more entries in input " << fCurrentInputId;
    return 1;
  }

  return 0;
}
// ---------------------------------------------------------------------------


// -----   Read event in the event-by-event mode   ---------------------------
Int_t CbmDigitizationSource::ReadEventByEvent(UInt_t event)
{

  // There should be only one input set with one input
  auto result       = fInputSets.front()->GetFirstInput();
  fCurrentInputId   = result.first;
  CbmMCInput* input = result.second;
  assert(input);

  // In mode kRegular: Get requested entry number
  if (input->GetMode() == ECbmTreeAccess::kRegular) {
    if (event >= input->GetNofEntries()) {
      LOG(info) << "DigitizationSource: Requested event " << event << " exceeds number of entries in input "
                << fCurrentInputId << "( " << input->GetNofEntries() << " )";
      return 1;
    }
    input->GetChain()->GetEntry(event);
    fCurrentEntryId = event;
  }  //? kRegular

  // In modes kRepeat or kRandom, get next entry
  else
    fCurrentEntryId = input->GetNextEntry();

  // Set entry properties
  fCurrentTime = 0.;
  LOG(info) << "DigitizationSource: Event " << event << " at t = " << fCurrentTime << " ns"
            << " from input " << fCurrentInputId << " (entry " << fCurrentEntryId << ")";

  return 0;
}
// ---------------------------------------------------------------------------


// -----   Read run ID   -----------------------------------------------------
void CbmDigitizationSource::ReadRunId()
{

  auto firstInput   = fInputSets.front()->GetFirstInput();
  fCurrentInputId   = firstInput.first;
  CbmMCInput* input = firstInput.second;
  assert(input);
  input->GetChain()->GetEntry(0);
  fCurrentRunId   = fMCEventHeader->GetRunID();
  fCurrentEntryId = 0;
  fFirstCall      = kFALSE;
  LOG(info) << "DigitizationSource: Run ID is " << fCurrentRunId;
}
// ---------------------------------------------------------------------------


ClassImp(CbmDigitizationSource)
