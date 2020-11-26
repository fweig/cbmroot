/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmMcbm2018EventBuilder.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include <TFile.h>

#include "CbmMuchBeamTimeDigi.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

#include <iomanip>
using std::fixed;
using std::make_pair;
using std::setprecision;

#include "CbmDigiManager.h"

// ---- Default constructor -------------------------------------------
CbmMcbm2018EventBuilder::CbmMcbm2018EventBuilder()
  : FairTask("CbmMcbm2018EventBuilder") {}

// ---- Destructor ----------------------------------------------------
CbmMcbm2018EventBuilder::~CbmMcbm2018EventBuilder() {}

// ----  Initialisation  ----------------------------------------------
void CbmMcbm2018EventBuilder::SetParContainers() {
  // Load all necessary parameter containers from the runtime data base
  /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <CbmMcbm2018EventBuilderDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmMcbm2018EventBuilder::Init() {

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Get a pointer to the previous already existing data level
  fDigiMan = CbmDigiManager::Instance();
  if (kFALSE == fbUseBaseMuchDigi) fDigiMan->UseMuchBeamTimeDigi();
  fDigiMan->Init();

  // T0 is not included in DigiManager
  fT0DigiVec = ioman->InitObjectAs<std::vector<CbmTofDigi> const*>("T0Digi");
  if (!fT0DigiVec) {
    fT0DigiArr = static_cast<TClonesArray*>(ioman->GetObject("T0Digi"));
    if (!fT0DigiArr) { LOG(info) << "No T0 digi input."; }
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    LOG(info) << "No STS digi input.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    LOG(info) << "No MUCH digi input.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kTrd)) {
    LOG(info) << "No TRD digi input.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(info) << "No TOF digi input.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) {
    LOG(info) << "No RICH digi input.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kPsd)) {
    LOG(info) << "No PSD digi input.";
  }

  // Register output array (CbmEvent)
  fEvents = new TClonesArray("CbmEvent", 100);
  ioman->Register(
    "CbmEvent", "Cbm_Event", fEvents, IsOutputBranchPersistent("CbmEvent"));

  if (!fEvents) LOG(fatal) << "Output branch was not created";


  if (fFillHistos) {
    fDiffTime = new TH1F(
      "fDiffTime",
      "Time difference between two consecutive digis;time diff [ns];Counts",
      420,
      -100.5,
      1999.5);
    fhEventTime = new TH1F("hEventTime",
                           "seed time of the events; Seed time [s]; Events",
                           60000,
                           0,
                           600);
    fhEventDt   = new TH1F(
      "fhEventDt",
      "interval in seed time of consecutive events; Seed time [s]; Events",
      2100,
      -100.5,
      1999.5);
    fhEventSize =
      new TH1F("hEventSize",
               "nb of all  digis in the event; Nb Digis []; Events []",
               10000,
               0,
               10000);
    fhNbDigiPerEvtTime =
      new TH2I("hNbDigiPerEvtTime",
               "nb of all  digis per event vs seed time of the events; Seed "
               "time [s]; Nb Digis []; Events []",
               600,
               0,
               600,
               1000,
               0,
               10000);

    fhNbDigiPerEvtTimeT0 =
      new TH2I("hNbDigiPerEvtTimeT0",
               "nb of T0   digis per event vs seed time of the events; Seed "
               "time [s]; Nb Digis []; Events []",
               600,
               0,
               600,
               4000,
               0,
               4000);
    fhNbDigiPerEvtTimeSts =
      new TH2I("hNbDigiPerEvtTimeSts",
               "nb of STS  digis per event vs seed time of the events; Seed "
               "time [s]; Nb Digis []; Events []",
               600,
               0,
               600,
               4000,
               0,
               4000);
    fhNbDigiPerEvtTimeMuch =
      new TH2I("hNbDigiPerEvtTimeMuch",
               "nb of MUCH digis per event vs seed time of the events; Seed "
               "time [s]; Nb Digis []; Events []",
               600,
               0,
               600,
               4000,
               0,
               4000);
    fhNbDigiPerEvtTimeTrd =
      new TH2I("hNbDigiPerEvtTimeTrd",
               "nb of TRD  digis per event vs seed time of the events; Seed "
               "time [s]; Nb Digis []; Events []",
               600,
               0,
               600,
               4000,
               0,
               4000);
    fhNbDigiPerEvtTimeTof =
      new TH2I("hNbDigiPerEvtTimeTof",
               "nb of TOF  digis per event vs seed time of the events; Seed "
               "time [s]; Nb Digis []; Events []",
               600,
               0,
               600,
               4000,
               0,
               4000);
    fhNbDigiPerEvtTimeRich =
      new TH2I("hNbDigiPerEvtTimeRich",
               "nb of RICH digis per event vs seed time of the events; Seed "
               "time [s]; Nb Digis []; Events []",
               600,
               0,
               600,
               4000,
               0,
               4000);
    fhNbDigiPerEvtTimePsd =
      new TH2I("hNbDigiPerEvtTimePsd",
               "nb of PSD  digis per event vs seed time of the events; Seed "
               "time [s]; Nb Digis []; Events []",
               600,
               0,
               600,
               4000,
               0,
               4000);
  }  // if( fFillHistos )

  return kSUCCESS;
}

// ---- ReInit  -------------------------------------------------------
InitStatus CbmMcbm2018EventBuilder::ReInit() { return kSUCCESS; }

// ---- Exec ----------------------------------------------------------
void CbmMcbm2018EventBuilder::Exec(Option_t* /*option*/) {

  LOG_IF(info, fNrTs % 1000 == 0) << "Begin of TS " << fNrTs;

  InitSorter();

  BuildEvents();

  LOG(debug) << "Found " << fEventVector.size() << " events";

  DefineGoodEvents();

  if (fFillHistos) { FillHisto(); }  // if( fFillHistos )

  LOG(debug) << "Found " << fEventVector.size() << " triggered events";

  FillOutput();

  fNrTs++;
}

void CbmMcbm2018EventBuilder::InitSorter() {
  // Fill the first entry of each TClonesarray to the std::set
  // The sorting should be done using the time of the digi which
  // can be received using the GetTime() function of CbmDigi

  Int_t nrT0Digis {0};
  if (fT0DigiVec)
    nrT0Digis = fT0DigiVec->size();
  else if (fT0DigiArr)
    nrT0Digis = fT0DigiArr->GetEntriesFast();
  Int_t nrStsDigis  = fDigiMan->GetNofDigis(ECbmModuleId::kSts);
  Int_t nrMuchDigis = fDigiMan->GetNofDigis(ECbmModuleId::kMuch);
  Int_t nrTrdDigis  = fDigiMan->GetNofDigis(ECbmModuleId::kTrd);
  Int_t nrTofDigis  = fDigiMan->GetNofDigis(ECbmModuleId::kTof);
  Int_t nrRichDigis = fDigiMan->GetNofDigis(ECbmModuleId::kRich);
  Int_t nrPsdDigis  = fDigiMan->GetNofDigis(ECbmModuleId::kPsd);

  LOG(debug) << "T0Digis: " << nrT0Digis;
  LOG(debug) << "StsDigis: " << nrStsDigis;
  LOG(debug) << "MuchDigis: " << nrMuchDigis;
  LOG(debug) << "TrdDigis: " << nrTrdDigis;
  LOG(debug) << "TofDigis: " << nrTofDigis;
  LOG(debug) << "RichDigis: " << nrRichDigis;
  LOG(debug) << "PsdDigis: " << nrPsdDigis;

  //  CbmDigi* digi = nullptr;

  if (nrT0Digis > 0) { AddDigiToSorter<CbmTofDigi>(ECbmModuleId::kHodo, 0); }
  if (nrStsDigis > 0) { AddDigiToSorter<CbmStsDigi>(ECbmModuleId::kSts, 0); }
  if (nrMuchDigis > 0) {
    if (fbUseBaseMuchDigi) {
      AddDigiToSorter<CbmMuchDigi>(ECbmModuleId::kMuch, 0);
    }  // if( fbUseBaseMuchDigi )
    else {
      AddDigiToSorter<CbmMuchBeamTimeDigi>(ECbmModuleId::kMuch, 0);
    }  // else of if( fbUseBaseMuchDigi )
  }
  if (nrTrdDigis > 0) { AddDigiToSorter<CbmTrdDigi>(ECbmModuleId::kTrd, 0); }
  if (nrTofDigis > 0) { AddDigiToSorter<CbmTofDigi>(ECbmModuleId::kTof, 0); }
  if (nrRichDigis > 0) { AddDigiToSorter<CbmRichDigi>(ECbmModuleId::kRich, 0); }
  if (nrPsdDigis > 0) { AddDigiToSorter<CbmPsdDigi>(ECbmModuleId::kPsd, 0); }
  for (const auto& data : fSorter) {
    LOG(debug) << "Array, Entry(" << data.second.first << ", "
               << data.second.second << "): " << fixed << setprecision(15)
               << data.first << " ns";
  }

  // Get the first element of the set from which one gets the first
  // element of the tuple (digi) from which one gets the smallest time
  // of all digis of the new TS
  if (fSorter.size() > 0) {
    fPrevTime       = fSorter.begin()->first;
    fStartTimeEvent = fPrevTime;
  }
}

void CbmMcbm2018EventBuilder::BuildEvents() {
  // Create a first CbmEvent
  fCurrentEvent = new CbmEvent(fCurEv++, fStartTimeEvent, 0.);

  while (fSorter.size() > 0) {

    // Extract the needed information from the first element of the set
    // The first element is the one with the smallest time
    auto it             = fSorter.begin();
    Double_t time       = it->first;
    ECbmModuleId system = it->second.first;
    Int_t entry         = it->second.second;

    // Decide if the digi belongs to the current event or if
    // it starts a new event
    if (!IsDigiInEvent(time)) {
      fCurrentEvent->SetEndTime(fPrevTime);
      fEventVector.push_back(fCurrentEvent);
      // Create then next CbmEvent
      fStartTimeEvent = time;
      fCurrentEvent   = new CbmEvent(fCurEv++, fStartTimeEvent, 0.);
    }
    AddDigiToEvent(system, entry);

    if (fFillHistos) fVect.emplace_back(make_pair(system, entry));

    // Remove the first element from the set and insert the next digi
    // from the same system
    fSorter.erase(fSorter.begin());

    switch (system) {
      case ECbmModuleId::kSts: {
        AddDigiToSorter<CbmStsDigi>(system, ++entry);
        break;
      }  // case ECbmModuleId::kSts
      case ECbmModuleId::kMuch: {
        if (fbUseBaseMuchDigi) {
          AddDigiToSorter<CbmMuchDigi>(system, ++entry);
        }  // if( fbUseBaseMuchDigi )
        else {
          AddDigiToSorter<CbmMuchBeamTimeDigi>(system, ++entry);
        }  // else of if( fbUseBaseMuchDigi )
        break;
      }  // case ECbmModuleId::kMuch
      case ECbmModuleId::kTrd: {
        AddDigiToSorter<CbmTrdDigi>(system, ++entry);
        break;
      }  // case ECbmModuleId::kTrd
      case ECbmModuleId::kTof: {
        AddDigiToSorter<CbmTofDigi>(system, ++entry);
        break;
      }  // case ECbmModuleId::kTof
      case ECbmModuleId::kRich: {
        AddDigiToSorter<CbmRichDigi>(system, ++entry);
        break;
      }  // case ECbmModuleId::kRich
      case ECbmModuleId::kPsd: {
        AddDigiToSorter<CbmPsdDigi>(system, ++entry);
        break;
      }  // case ECbmModuleId::kPsd
      case ECbmModuleId::kHodo: {
        AddDigiToSorter<CbmTofDigi>(system, ++entry);
        break;
      }  // case ECbmModuleId::kHodo
      default: {
        break;
      }  // default:
    }    //? system

    if (fFillHistos) fDiffTime->Fill(time - fPrevTime);

    fPrevTime = time;
  }
  fCurrentEvent->SetEndTime(fPrevTime);
  fEventVector.push_back(fCurrentEvent);
}

Bool_t CbmMcbm2018EventBuilder::IsDigiInEvent(Double_t time) {
  // here the different possibilities have to be implemented
  if (EventBuilderAlgo::FixedTimeWindow == fEventBuilderAlgo) {
    return ((time - fStartTimeEvent < fFixedTimeWindow) ? kTRUE : kFALSE);
  } else {
    return ((time - fPrevTime < fMaximumTimeGap) ? kTRUE : kFALSE);
  }
}

Bool_t CbmMcbm2018EventBuilder::HasTrigger(CbmEvent* event) {
  Bool_t hasTrigger {kTRUE};
  if (hasTrigger && (fT0DigiVec || fT0DigiArr) && fTriggerMinT0Digis > 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kT0Digi) >= fTriggerMinT0Digis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kSts)
      && fTriggerMinStsDigis > 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kStsDigi) >= fTriggerMinStsDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kMuch)
      && fTriggerMinMuchDigis > 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kMuchDigi) >= fTriggerMinMuchDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kTrd)
      && fTriggerMinTrdDigis > 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kTrdDigi) >= fTriggerMinTrdDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kTof)
      && fTriggerMinTofDigis > 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kTofDigi) >= fTriggerMinTofDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kRich)
      && fTriggerMinRichDigis > 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kRichDigi) >= fTriggerMinRichDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kPsd)
      && fTriggerMinPsdDigis > 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kPsdDigi) >= fTriggerMinPsdDigis);
  }

  if (hasTrigger && (fT0DigiVec || fT0DigiArr) && fTriggerMaxT0Digis >= 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kT0Digi) < fTriggerMaxT0Digis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kSts)
      && fTriggerMaxStsDigis >= 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kStsDigi) < fTriggerMaxStsDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kMuch)
      && fTriggerMaxMuchDigis >= 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kMuchDigi) < fTriggerMaxMuchDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kTrd)
      && fTriggerMaxTrdDigis >= 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kTrdDigi) < fTriggerMaxTrdDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kTof)
      && fTriggerMaxTofDigis >= 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kTofDigi) < fTriggerMaxTofDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kRich)
      && fTriggerMaxRichDigis >= 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kRichDigi) < fTriggerMaxRichDigis);
  }
  if (hasTrigger && fDigiMan->IsPresent(ECbmModuleId::kPsd)
      && fTriggerMaxPsdDigis >= 0) {
    hasTrigger =
      hasTrigger
      && (event->GetNofData(ECbmDataType::kPsdDigi) < fTriggerMaxPsdDigis);
  }

  return hasTrigger;
}

void CbmMcbm2018EventBuilder::FillHisto() {
  /*
  fPrevTime = 0.;

  ECbmModuleId prevSystem{ECbmModuleId::kNofSystems};
  Int_t prevEntry{-1};


  for ( const auto& _pair: fVect) {
    ECbmModuleId _system = _pair.first;
    Int_t _entry = _pair.second;
    CbmDigi* digi = static_cast<CbmDigi*>(fLinkArray[_system]->At(_entry));
    Double_t difftime = digi->GetTime() - fPrevTime;
    fDiffTime->Fill(difftime);
    if (difftime < 0.) {
      fErrors++;
      LOG(info) << fixed << setprecision(15)
                << "DiffTime: " << difftime *1.e-9
		<< "  Previous digi(" << prevSystem << ", "
                << prevEntry << "): "
                << fPrevTime * 1.e-9 << ", Current digi("
                << _system << ", " <<_entry  << "): "
                << digi->GetTime() * 1.e-9;
    }

    fPrevTime = digi->GetTime();
    prevSystem = _system;
    prevEntry = _entry;
  }
  */
  fVect.clear();

  Double_t dPreEvtTime = -1.0;
  for (CbmEvent* evt : fEventVector) {
    fhEventTime->Fill(evt->GetStartTime() * 1e-9);
    if (0.0 <= dPreEvtTime) {
      fhEventDt->Fill(evt->GetStartTime() - dPreEvtTime);
    }  // if( 0.0 <= dPreEvtTime )
    fhEventSize->Fill(evt->GetNofData());
    fhNbDigiPerEvtTime->Fill(evt->GetStartTime() * 1e-9, evt->GetNofData());

    fhNbDigiPerEvtTimeT0->Fill(evt->GetStartTime() * 1e-9,
                               evt->GetNofData(ECbmDataType::kT0Digi));
    fhNbDigiPerEvtTimeSts->Fill(evt->GetStartTime() * 1e-9,
                                evt->GetNofData(ECbmDataType::kStsDigi));
    fhNbDigiPerEvtTimeMuch->Fill(evt->GetStartTime() * 1e-9,
                                 evt->GetNofData(ECbmDataType::kMuchDigi));
    fhNbDigiPerEvtTimeTrd->Fill(evt->GetStartTime() * 1e-9,
                                evt->GetNofData(ECbmDataType::kTrdDigi));
    fhNbDigiPerEvtTimeTof->Fill(evt->GetStartTime() * 1e-9,
                                evt->GetNofData(ECbmDataType::kTofDigi));
    fhNbDigiPerEvtTimeRich->Fill(evt->GetStartTime() * 1e-9,
                                 evt->GetNofData(ECbmDataType::kRichDigi));
    fhNbDigiPerEvtTimePsd->Fill(evt->GetStartTime() * 1e-9,
                                evt->GetNofData(ECbmDataType::kPsdDigi));

    dPreEvtTime = evt->GetStartTime();
  }  // for( CbmEvent * evt: fEventVector )
}

void CbmMcbm2018EventBuilder::DefineGoodEvents() {
  for (auto it = fEventVector.begin(); it != fEventVector.end();
       /*increased in the loop*/) {
    if (!HasTrigger((*it))) {
      delete (*it);
      it = fEventVector.erase(it);
    } else {
      ++it;
    }
  }
}

void CbmMcbm2018EventBuilder::FillOutput() {
  // Clear TClonesArray before usage.
  fEvents->Delete();

  // Move CbmEvent from temporary vector to TClonesArray
  for (auto event : fEventVector) {
    LOG(debug) << "Vector: " << event->ToString();
    new ((*fEvents)[fEvents->GetEntriesFast()]) CbmEvent(std::move(*event));
    LOG(debug) << "TClonesArray: "
               << static_cast<CbmEvent*>(
                    fEvents->At(fEvents->GetEntriesFast() - 1))
                    ->ToString();
  }

  // Clear event vector after usage
  // Need to delete the object the pointer points to first
  int counter = 0;
  for (auto event : fEventVector) {
    LOG(debug) << "Event " << counter << " has " << event->GetNofData()
               << " digis";
    delete event;
    counter++;
  }

  fEventVector.clear();
}

void CbmMcbm2018EventBuilder::AddDigiToEvent(ECbmModuleId _system,
                                             Int_t _entry) {
  // Fill digi index into event
  switch (_system) {
    case ECbmModuleId::kMvd:
      fCurrentEvent->AddData(ECbmDataType::kMvdDigi, _entry);
      break;
    case ECbmModuleId::kSts:
      fCurrentEvent->AddData(ECbmDataType::kStsDigi, _entry);
      break;
    case ECbmModuleId::kRich:
      fCurrentEvent->AddData(ECbmDataType::kRichDigi, _entry);
      break;
    case ECbmModuleId::kMuch:
      fCurrentEvent->AddData(ECbmDataType::kMuchDigi, _entry);
      break;
    case ECbmModuleId::kTrd:
      fCurrentEvent->AddData(ECbmDataType::kTrdDigi, _entry);
      break;
    case ECbmModuleId::kTof:
      fCurrentEvent->AddData(ECbmDataType::kTofDigi, _entry);
      break;
    case ECbmModuleId::kPsd:
      fCurrentEvent->AddData(ECbmDataType::kPsdDigi, _entry);
      break;
    case ECbmModuleId::kHodo:
      fCurrentEvent->AddData(ECbmDataType::kT0Digi, _entry);
      break;
    default: break;
  }
}

template<class Digi>
void CbmMcbm2018EventBuilder::AddDigiToSorter(ECbmModuleId _system,
                                              Int_t _entry) {
  LOG(debug4) << "Entry: " << _entry;

  Double_t time = -1.;

  switch (_system) {
    case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:
    case ECbmModuleId::kTof:
    case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd: {
      const Digi* pDigi = fDigiMan->Get<Digi>(_entry);

      /// Check that _entry is not out of range
      if (nullptr != pDigi) {
        time = pDigi->GetTime();
        fSorter.emplace(make_pair(time, make_pair(_system, _entry)));
      }  // if( nullptr != pDigi )
      break;
    }  // Digi containers controlled by DigiManager
    case ECbmModuleId::kHodo: {
      //      CbmTofDigi * pDigi;
      if (fT0DigiVec) {
        if (static_cast<UInt_t>(_entry) < fT0DigiVec->size()) {
          time = fT0DigiVec->at(_entry).GetTime();
          fSorter.emplace(make_pair(time, make_pair(_system, _entry)));
        }  // if( _entry < fT0DigiVec->size() )
      }    // if ( fT0DigiVec )
      else if (fT0DigiArr) {
        if (_entry < fT0DigiArr->GetEntriesFast()) {
          time = dynamic_cast<CbmTofDigi*>(fT0DigiArr->At(_entry))->GetTime();
          fSorter.emplace(make_pair(time, make_pair(_system, _entry)));
        }  // if( _entry < fT0DigiArr->GetEntriesFast() )
      }    // else if ( fT0DigiArr )
      else
        return;
      break;
    }  // case ECbmModuleId::kHodo
    default: {
      return;
      break;
    }  // default:
  }    //? system
}

// ---- Finish --------------------------------------------------------
void CbmMcbm2018EventBuilder::Finish() {
  if (fFillHistos) {
    TFile* old     = gFile;
    TFile* outfile = TFile::Open(fOutFileName, "RECREATE");

    fDiffTime->Write();

    fhEventTime->Write();
    fhEventDt->Write();
    fhEventSize->Write();
    fhNbDigiPerEvtTime->Write();

    fhNbDigiPerEvtTimeT0->Write();
    fhNbDigiPerEvtTimeSts->Write();
    fhNbDigiPerEvtTimeMuch->Write();
    fhNbDigiPerEvtTimeTrd->Write();
    fhNbDigiPerEvtTimeTof->Write();
    fhNbDigiPerEvtTimeRich->Write();
    fhNbDigiPerEvtTimePsd->Write();

    outfile->Close();
    delete outfile;

    gFile = old;
  }
  LOG(info) << "Total errors: " << fErrors;
}


ClassImp(CbmMcbm2018EventBuilder)
