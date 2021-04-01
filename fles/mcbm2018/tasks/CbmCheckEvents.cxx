/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmCheckEvents.h"

//#include "CbmTofDigi.h"
#include "CbmDefs.h"
#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"

#include "FairRootManager.h"
#include "FairRunOnline.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include "TProfile.h"
#include <TDirectory.h>
#include <TFile.h>
#include <typeinfo>

#include <iomanip>

using std::fixed;
using std::setprecision;

// ---- Default constructor -------------------------------------------
CbmCheckEvents::CbmCheckEvents() : FairTask("CbmCheckEvents") {}

// ---- Destructor ----------------------------------------------------
CbmCheckEvents::~CbmCheckEvents() {}

// ----  Initialisation  ----------------------------------------------
void CbmCheckEvents::SetParContainers() {
  // Load all necessary parameter containers from the runtime data base
  /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <CbmCheckEventsDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmCheckEvents::Init() {

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // DigiManager
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->UseMuchBeamTimeDigi();
  fDigiMan->Init();

  // Get a pointer to the previous already existing data level

  fT0DigiVec = ioman->InitObjectAs<std::vector<CbmTofDigi> const*>("T0Digi");
  if (!fT0DigiVec) {
    fT0DigiArr = dynamic_cast<TClonesArray*>(ioman->GetObject("T0Digi"));
    if (!fT0DigiArr) { LOG(fatal) << "No TClonesArray with T0 digis found."; }
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    LOG(info) << "No TClonesArray with STS digis found.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    LOG(info) << "No TClonesArray with MUCH digis found.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(info) << "No TClonesArray with TOF digis found.";
  }

  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  if (nullptr == fEvents) {

    if (nullptr != (ioman->GetObject("CbmEvent"))) {
      LOG(error) << "Got pointer of type"
                 << typeid(ioman->GetObject("CbmEvent")).name();
      LOG(error) << "Got Object of type"
                 << typeid(*(ioman->GetObject("CbmEvent"))).name();
    }  // if( nullptr != (ioman->GetObject("CbmEvent") )
    LOG(fatal) << "No TClonesArray with events found.";
  }  // if (nullptr == fEvents)


  CreateHistos();

  return kSUCCESS;
}

// ---- ReInit  -------------------------------------------------------
InitStatus CbmCheckEvents::ReInit() { return kSUCCESS; }

void CbmCheckEvents::CreateHistos() {
  fEventSize =
    new TH1F("fEventSize", "Event Size; # Digis; Counts", 1000, -0.5, 999.5);
  fEventLength = new TH1F(
    "fEventLength", "Event Length; time [ns]; Counts", 1000, -0.5, 999.5);
  fEventsPerTS = new TH1F("fEventsPerTS",
                          "Events per time slice; # Events; Counts",
                          1000,
                          -0.5,
                          999.5);
  fT0InEvent   = new TH1F("fT0InEvent",
                        "Number of T0 digis in Event; # digis; Counts",
                        1000,
                        -0.5,
                        999.5);
  fStsInEvent  = new TH1F("fStsInEvent",
                         "Number of Sts digis in Event; # digis; Counts",
                         1000,
                         -0.5,
                         999.5);
  fMuchInEvent = new TH1F("fMuchInEvent",
                          "Number of Much digis in Event; # digis; Counts",
                          1000,
                          -0.5,
                          999.5);
  fTofInEvent  = new TH1F("fTofInEvent",
                         "Number of Tof digis in Event; # digis; Counts",
                         1000,
                         -0.5,
                         999.5);
  fT0DeltaT =
    new TH1F("fT0DeltaT",
             "Time diff between first and last T0 digi;dt [ns]; Counts",
             1000,
             -0.5,
             999.5);
  fStsDeltaT =
    new TH1F("fStsDeltaT",
             "Time diff between first and last Sts digi;dt [ns]; Counts",
             1000,
             -0.5,
             999.5);
  fMuchDeltaT =
    new TH1F("fMuchDeltaT",
             "Time diff between first and last Much digi;dt [ns]; Counts",
             1000,
             -0.5,
             999.5);
  fTofDeltaT =
    new TH1F("fTofDeltaT",
             "Time diff between first and last Tof digi;dt [ns]; Counts",
             1000,
             -0.5,
             999.5);

  fEventsvsTS = new TH2F("fEventsvsTS",
                         "Nr. of events as fct. of TS",
                         10000,
                         -0.5,
                         9999.5,
                         1000,
                         -0.5,
                         999.5);
  fLengthvsTS = new TProfile("fLengthvsTS",
                             "Length of events as fct. of TS",
                             10000,
                             -0.5,
                             9999.5,
                             -0.5,
                             999.5);
}
// ---- Exec ----------------------------------------------------------
void CbmCheckEvents::Exec(Option_t* /*option*/) {

  LOG_IF(info, fNrTs % 1000 == 0) << "Analysing TS " << fNrTs;

  Int_t nrEvents = fEvents->GetEntriesFast();

  fEventsPerTS->Fill(nrEvents);
  fEventsvsTS->Fill(fNrTs, nrEvents);

  Int_t nrT0Digis = -1;
  if (fT0DigiVec)
    nrT0Digis = fT0DigiVec->size();
  else if (fT0DigiArr)
    nrT0Digis = fT0DigiArr->GetEntriesFast();
  Int_t nrStsDigis  = fDigiMan->GetNofDigis(ECbmModuleId::kSts);
  Int_t nrMuchDigis = fDigiMan->GetNofDigis(ECbmModuleId::kMuch);
  Int_t nrTofDigis  = fDigiMan->GetNofDigis(ECbmModuleId::kTof);

  LOG(debug) << "Events: " << nrEvents;
  LOG(debug) << "T0Digis: " << nrT0Digis;
  LOG(debug) << "StsDigis: " << nrStsDigis;
  LOG(debug) << "MuchDigis: " << nrMuchDigis;
  LOG(debug) << "TofDigis: " << nrTofDigis;

  // Loop over all CbmEvents in the time slice
  for (Int_t iEvent = 0; iEvent < nrEvents; iEvent++) {
    CbmEvent* event = dynamic_cast<CbmEvent*>(fEvents->At(iEvent));
    fEventSize->Fill(event->GetNofData());
    fEventLength->Fill(event->GetEndTime() - event->GetStartTime());
    fLengthvsTS->Fill(fNrTs, event->GetEndTime() - event->GetStartTime(), 1);
    AnalyseEvent(event);
  }

  fNrTs++;
}

void CbmCheckEvents::AnalyseEvent(CbmEvent* event) {
  // Loop over the the digis and extract the maximum time
  // difference between the digis
  GetTimeDiffT0(event, fT0DeltaT, fT0InEvent);
  GetTimeDiff<CbmStsDigi>(
    event, fStsDeltaT, fStsInEvent, ECbmDataType::kStsDigi);
  GetTimeDiff<CbmMuchBeamTimeDigi>(
    event, fMuchDeltaT, fMuchInEvent, ECbmDataType::kMuchDigi);
  GetTimeDiff<CbmTofDigi>(
    event, fTofDeltaT, fTofInEvent, ECbmDataType::kTofDigi);
}

template<class Digi>
void CbmCheckEvents::GetTimeDiff(CbmEvent* event,
                                 TH1* deltaT,
                                 TH1* size,
                                 ECbmDataType dataType) {
  Double_t startTime {1.e18};
  Double_t stopTime {0.};
  Int_t nDigis = event->GetNofData(dataType);
  size->Fill(nDigis);
  for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
    UInt_t index     = event->GetIndex(dataType, iDigi);
    const Digi* digi = fDigiMan->Get<Digi>(index);
    assert(digi);
    if (digi->GetTime() < startTime) startTime = digi->GetTime();
    if (digi->GetTime() > stopTime) stopTime = digi->GetTime();
  }
  deltaT->Fill(stopTime - startTime);
}


void CbmCheckEvents::GetTimeDiffT0(CbmEvent* event, TH1* deltaT, TH1* size) {
  Double_t startTime {1.e18};
  Double_t stopTime {0.};
  Int_t nDigis = event->GetNofData(ECbmDataType::kT0Digi);
  size->Fill(nDigis);
  for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
    UInt_t index = event->GetIndex(ECbmDataType::kT0Digi, iDigi);
    //Double_t digiTime; (VF) not used
    const CbmTofDigi* digi = nullptr;
    if (fT0DigiVec)
      digi = &(fT0DigiVec->at(index));
    else if (fT0DigiArr)
      digi = dynamic_cast<CbmTofDigi*>(fT0DigiArr->At(index));
    assert(digi);
    if (digi->GetTime() < startTime) startTime = digi->GetTime();
    if (digi->GetTime() > stopTime) stopTime = digi->GetTime();
  }
  deltaT->Fill(stopTime - startTime);
}

// ---- Finish --------------------------------------------------------
void CbmCheckEvents::Finish() {
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* outfile = TFile::Open("test2.root", "RECREATE");

  fEventSize->Write();
  fEventLength->Write();
  fEventsPerTS->Write();

  fT0InEvent->Write();
  fStsInEvent->Write();
  fMuchInEvent->Write();
  fTofInEvent->Write();

  fT0DeltaT->Write();
  fStsDeltaT->Write();
  fMuchDeltaT->Write();
  fTofDeltaT->Write();

  fEventsvsTS->Write();
  fLengthvsTS->Write();

  outfile->Close();
  delete outfile;

  gFile      = oldFile;
  gDirectory = oldDir;
}

ClassImp(CbmCheckEvents)
