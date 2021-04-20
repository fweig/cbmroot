// -------------------------------------------------------------------------
// -----                CbmPsdHitProducer source file             -----
// -----                  Created 15/05/12  by     Alla & SELIM               -----
// -------------------------------------------------------------------------
#include "CbmPsdHitProducer.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmPsdDigi.h"
#include "CbmPsdHit.h"

#include <FairRootManager.h>
#include <Logger.h>

#include <TClonesArray.h>
#include <TFile.h>
#include <TMath.h>
#include <TStopwatch.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

using std::cout;
using std::endl;
using std::fixed;
using std::left;
using std::pair;
using std::setprecision;
using std::setw;


// -----   Default constructor   -------------------------------------------
CbmPsdHitProducer::CbmPsdHitProducer() : FairTask("Ideal Psd Hit Producer", 1), fXi(), fYi() {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmPsdHitProducer::~CbmPsdHitProducer()
{
  if (fHitArray) {
    fHitArray->Delete();
    delete fHitArray;
  }
}
// -------------------------------------------------------------------------


// -----   Public method Init   --------------------------------------------
InitStatus CbmPsdHitProducer::Init()
{

  fhModXNewEn = new TH1F("hModXNewEn", "X distr, En", 300, -150., 150.);
  fhModXNewEn->Print();

  std::cout << std::endl << std::endl;
  LOG(info) << "=======   " << GetName() << ": Init   =====================";

  // --- Get RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    LOG(fatal) << "-W- CbmPsdHitProducer::Init: RootManager not instantised!";  //FLORIAN & SELIM
    return kFATAL;
  }

  // --- Get event array, if present
  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("Event"));
  if (fEvents) LOG(info) << GetName() << ": found Event branch, run event-by-event";
  else {
    fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
    if (fEvents) LOG(info) << GetName() << ": found CbmEvent branch, run event-by-event";
  }
  if (!fEvents) {
    LOG(info) << GetName() << ": no event branch found; run in time-based mode";
    LOG(warn) << "*** Note that the current PSD hit producer is not suited for time-based reconstruction!";
    LOG(warn) << "*** Unless you have run the simulation event-by-event, it will not produce sensible results.";
  }

  // --- Initialise digi manager
  CbmDigiManager* digiMan = CbmDigiManager::Instance();
  digiMan->Init();

  // --- Check input branch (PsdDigi). If not present, set task inactive.
  if (!digiMan->IsPresent(ECbmModuleId::kPsd)) {
    LOG(error) << GetName() << ": No PsdDigi input array present; "
               << "task will be inactive.";
    return kERROR;
  }
  LOG(info) << GetName() << ": found PsdDigi branch";

  // --- Create and register output array
  fHitArray = new TClonesArray("CbmPsdHit", 1000);
  ioman->Register("PsdHit", "PSD", fHitArray, IsOutputBranchPersistent("PsdHit"));
  fHitArray->Dump();
  LOG(info) << GetName() << ": registered branch PsdHit";

  LOG(info) << GetName() << ": intialisation successfull";
  LOG(info) << "======================================================\n";
  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Public method Exec   --------------------------------------------
void CbmPsdHitProducer::Exec(Option_t* /*opt*/)
{

  // --- Timer
  TStopwatch timer;
  timer.Start();

  // --- Reset output array
  Reset();

  // --- Local variables
  pair<Int_t, Int_t> result;
  Int_t nEvents   = 0;
  Int_t nDigis    = 0;
  Int_t nHits     = 0;
  Int_t nDigisAll = fDigiMan->GetNofDigis(ECbmModuleId::kPsd);
  CbmEvent* event = nullptr;

  // --- Time-slice mode: process entire digi array
  if (!fEvents) {
    result = ProcessData(nullptr);
    nDigis = result.first;
    nHits  = result.second;
  }

  // --- Event mode: loop over events and process digis within
  else {
    assert(fEvents);
    nEvents = fEvents->GetEntriesFast();
    for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
      event = dynamic_cast<CbmEvent*>(fEvents->At(iEvent));
      assert(event);
      result = ProcessData(event);
      LOG(debug) << GetName() << ": Event " << iEvent << " / " << nEvents << ", digis " << result.first << ", hits "
                 << result.second;
      nDigis += result.first;
      nHits += result.second;
    }  //# events
  }    //? event mode

  // --- Timeslice log and statistics
  timer.Stop();
  if (fEvents)
    LOG(info) << std::setw(20) << std::left << GetName() << fixed << setprecision(2) << " [" << timer.RealTime() * 1000.
              << " ms] TS " << fNofTs << ", events " << nEvents << ", digis " << nDigis << " / " << nDigisAll
              << ", hits " << nHits;
  else
    LOG(info) << std::setw(20) << std::left << GetName() << fixed << setprecision(2) << " [" << timer.RealTime() * 1000.
              << " ms] TS " << fNofTs << ", digis " << nDigis << " / " << nDigisAll << ", hits " << nHits;
  fNofTs++;
  fNofEvents += nEvents;
  fNofDigis += nDigis;
  fNofHits += nHits;
  fTimeTot += timer.RealTime();
}
// -------------------------------------------------------------------------


// -----   End-of-timeslice action   ---------------------------------------
void CbmPsdHitProducer::Finish()
{

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices     : " << fNofTs;
  LOG(info) << "Digis / TS      : " << fixed << setprecision(2) << Double_t(fNofDigis) / Double_t(fNofTs);
  LOG(info) << "Hits  / TS      : " << fixed << setprecision(2) << Double_t(fNofHits) / Double_t(fNofTs);
  LOG(info) << "Time  / TS      : " << fixed << setprecision(2) << 1000. * fTimeTot / Double_t(fNofTs) << " ms";
  if (fEvents) {
    LOG(info) << "Events          : " << fNofEvents;
    LOG(info) << "Events / TS     : " << fixed << setprecision(2) << Double_t(fNofEvents) / Double_t(fNofTs);
    LOG(info) << "Digis  / event  : " << fixed << setprecision(2) << Double_t(fNofDigis) / Double_t(fNofEvents);
    LOG(info) << "Hits   / event  : " << fixed << setprecision(2) << Double_t(fNofHits) / Double_t(fNofEvents);
  }

  // --- Write energy deposition histogram
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;
  TFile* outfile     = new TFile("EdepHistos.root", "RECREATE");
  outfile->cd();
  fhModXNewEn->Write();
  outfile->Close();
  gFile      = oldFile;
  gDirectory = oldDir;
  LOG(info) << "Histograms written to EdepHistos.root";
  LOG(info) << "=====================================\n";
}
// -------------------------------------------------------------------------


// -----   Data processing   -----------------------------------------------
pair<Int_t, Int_t> CbmPsdHitProducer::ProcessData(CbmEvent* event)
{

  // --- Local variables
  std::map<int, Double_t> edepmap;  // energy deposition per module, key is module ID
  Int_t nHits            = 0;
  const CbmPsdDigi* digi = nullptr;
  Int_t nDigis = (event ? event->GetNofData(ECbmDataType::kPsdDigi) : fDigiMan->GetNofDigis(ECbmModuleId::kPsd));

  // --- Loop over PsdDigis. Distribute energy deposition into the modules.
  Int_t digiIndex = -1;
  for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {

    digiIndex = (event ? event->GetIndex(ECbmDataType::kPsdDigi, iDigi) : iDigi);
    digi      = fDigiMan->Get<const CbmPsdDigi>(digiIndex);
    assert(digi);

    Int_t mod          = digi->GetModuleID();
    Double_t eDep      = (Double_t) digi->GetEdep();
    auto insert_result = edepmap.insert(std::make_pair(mod, eDep));
    if (!insert_result.second) {  // entry was here before
      (*insert_result.first).second += eDep;
    }

  }  // # digis

  // --- Create hits, one per activated module
  UInt_t hitIndex = -1;
  for (auto edep_entry : edepmap) {
    int modID     = edep_entry.first;
    Double_t eDep = edep_entry.second;
    hitIndex      = fHitArray->GetEntriesFast();
    new ((*fHitArray)[hitIndex]) CbmPsdHit(modID, eDep);
    if (event) event->AddData(ECbmDataType::kPsdHit, hitIndex);
    nHits++;
  }

  return std::make_pair(nDigis, nHits);
}
// -------------------------------------------------------------------------


// -----   Reset: clear output array   -------------------------------------
void CbmPsdHitProducer::Reset()
{
  if (fHitArray) fHitArray->Delete();
}
// -------------------------------------------------------------------------


ClassImp(CbmPsdHitProducer)
