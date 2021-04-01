/** @file  CbmMCDataArray.cxx
 ** @author //Dr.Sys <mikhail.prokudin@cern.ch>
 **/
#include "CbmMCDataArray.h"

#include <FairRootManager.h>  // for FairRootManager
#include <Logger.h>           // for Logger, LOG

#include <TChain.h>        // for TChain
#include <TClonesArray.h>  // for TClonesArray
#include <TString.h>       // for TString, operator<<

#include <utility>  // for pair

using namespace std;


// --- Standard constructor
CbmMCDataArray::CbmMCDataArray(const char* branchname,
                               const std::vector<std::list<TString>>& fileList)
  : fLegacy(0)
  , fLegacyArray(nullptr)
  , fBranchName(branchname)
  , fSize(-1111)
  , fChains()
  , fTArr()
  , fN()
  , fArrays() {
  list<TString>::const_iterator p;
  Int_t i;
  Int_t s = fileList.size();

  fSize = s;
  fChains.resize(s);
  fTArr.resize(s);
  fN.resize(s);

  for (i = 0; i < s; i++) {
    fN[i]      = 0;
    fTArr[i]   = nullptr;
    fChains[i] = nullptr;
    if (fileList[i].size() == 0) continue;
    fChains[i] = new TChain("cbmsim");
    for (p = fileList[i].begin(); p != fileList[i].end(); ++p)
      fChains[i]->AddFile(*p);
    fChains[i]->SetBranchAddress(branchname, &(fTArr[i]));
    fN[i] = fChains[i]->GetEntries();
  }

  fArrays.resize(s);
  for (i = 0; i < s; i++)
    fArrays[i].clear();
}

// --- Make TChain number chainNum2 friend of TChain number chainNum2
void CbmMCDataArray::AddFriend(Int_t chainNum1, Int_t chainNum2) {
  if (fLegacy == 1) {
    LOG(error) << "AddFriend method should not be called in legacy mode";
    return;
  }
  if (chainNum1 < 0 || chainNum1 >= static_cast<Int_t>(fChains.size())
      || fChains[chainNum1] == nullptr) {
    LOG(error) << "chainNum1=" << chainNum1
               << " is not a correct chain number.";
    return;
  }
  if (chainNum2 < 0 || chainNum2 >= static_cast<Int_t>(fChains.size())
      || fChains[chainNum2] == nullptr) {
    LOG(error) << "chainNum2=" << chainNum2
               << " is not a correct chain number.";
    return;
  }
  fChains[chainNum1]->AddFriend(fChains[chainNum2]);
}

// --- Legacy constructor
CbmMCDataArray::CbmMCDataArray(const char* branchname)
  : fLegacy(1)
  , fLegacyArray(nullptr)
  , fBranchName(branchname)
  , fSize(-1111)
  , fChains()
  , fTArr()
  , fN()
  , fArrays() {
  FairRootManager* fManager = FairRootManager::Instance();
  if (!fManager) {
    LOG(fatal) << "CbmMCDataArray():	Can't find a Root Manager.";
    return;
  }
  fLegacyArray = (TClonesArray*) fManager->GetObject(branchname);
  if (!fLegacyArray) {
    LOG(fatal) << "CbmMCDataArray(): Can't find " << fBranchName
               << " in the system.";
    return;
  }
}

// --- Legacy Get
TObject*
CbmMCDataArray::LegacyGet(Int_t fileNumber, Int_t eventNumber, Int_t index) {
  if (fileNumber >= 0 || eventNumber >= 0)
    LOG(debug1) << "LegacyGet:	Trying to get object with fileNum="
                << fileNumber << ", entryNum=" << eventNumber
                << " in legacy mode.";
  if (index < 0) return 0;
  return fLegacyArray->At(index);
}


// --- Get an object
TObject* CbmMCDataArray::Get(Int_t fileNumber, Int_t eventNumber, Int_t index) {
  if (fLegacy == 1) return LegacyGet(fileNumber, eventNumber, index);
  if (fileNumber < 0 || fileNumber >= fSize) return nullptr;
  if (eventNumber < 0 || eventNumber >= fN[fileNumber]) return nullptr;
  if (index < 0) return nullptr;

  // --- Cached arrays
  map<Int_t, TClonesArray*>& arr = fArrays[fileNumber];

  // --- If the array for this event is already in the cache, use it.
  if (arr.find(eventNumber) != arr.end()) return arr[eventNumber]->At(index);

  // --- If not, copy the array from the chain into the cache
  TChain* ch = fChains[fileNumber];
  ch->GetEntry(eventNumber);
  arr[eventNumber] = (TClonesArray*) (fTArr[fileNumber]->Clone());

  return arr[eventNumber]->At(index);
}

// --- Get a size of TClonesArray . Slow if TClonesArray not in cache
Int_t CbmMCDataArray::Size(Int_t fileNumber, Int_t eventNumber) {
  if (fLegacy == 1) return fLegacyArray->GetEntriesFast();
  if (fileNumber < 0 || fileNumber >= fSize) return -1111;
  if (eventNumber < 0 || eventNumber >= fN[fileNumber]) return -1111;

  // --- Cached arrays
  map<Int_t, TClonesArray*>& arr = fArrays[fileNumber];

  // --- If the array for this event is already in the cache, use it.
  if (arr.find(eventNumber) != arr.end())
    return arr[eventNumber]->GetEntriesFast();

  // --- If not, get the array from the chain (slow)
  TChain* ch = fChains[fileNumber];
  ch->GetEntry(eventNumber);

  return fTArr[fileNumber]->GetEntriesFast();
}


// --- At end of one event: clear the cache to free the memory
void CbmMCDataArray::FinishEvent() {
  if (fLegacy == 1) return;

  Int_t i;
  map<Int_t, TClonesArray*>::const_iterator p;

  for (i = 0; i < fSize; i++) {
    for (p = fArrays[i].begin(); p != fArrays[i].end(); ++p)
      delete (p->second);
    fArrays[i].clear();
  }
}


// --- Clean up
void CbmMCDataArray::Done() {
  if (fLegacy == 1) return;
  Int_t i;

  FinishEvent();
  for (i = 0; i < fSize; i++)
    delete fChains[i];
}

ClassImp(CbmMCDataArray)
