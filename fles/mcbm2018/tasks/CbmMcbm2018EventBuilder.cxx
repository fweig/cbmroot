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
#include "THttpServer.h"
#include <TFile.h>

#include "CbmStsDigi.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmTofDigi.h"
#include "CbmRichDigi.h"
#include "CbmPsdDigi.h"

#include <iomanip>
using std::fixed;
using std::setprecision;
using std::make_pair;

#include "CbmDigiManager.h"

// ---- Default constructor -------------------------------------------
CbmMcbm2018EventBuilder::CbmMcbm2018EventBuilder()
  : FairTask("CbmMcbm2018EventBuilder")
{

}

// ---- Destructor ----------------------------------------------------
CbmMcbm2018EventBuilder::~CbmMcbm2018EventBuilder()
{

}

// ----  Initialisation  ----------------------------------------------
void CbmMcbm2018EventBuilder::SetParContainers()
{
  // Load all necessary parameter containers from the runtime data base
  /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <CbmMcbm2018EventBuilderDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmMcbm2018EventBuilder::Init()
{

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Get a pointer to the previous already existing data level
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->UseMuchBeamTimeDigi();
  fDigiMan->Init();

  // T0 is not included in DigiManager
  fT0DigiVec = ioman->InitObjectAs<std::vector<CbmTofDigi> const *>("T0Digi");
  if ( ! fT0DigiVec ) {
    fT0DigiArr = static_cast<TClonesArray*>(ioman->GetObject("T0Digi"));
    if ( ! fT0DigiArr ) {
      LOG(info) << "No T0 digi input.";
    }
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kSts) ) {
    LOG(info) << "No STS digi input.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kMuch) ) {
    LOG(info) << "No MUCH digi input.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kTof) ) {
    LOG(info) << "No TOF digi input.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kRich) ) {
    LOG(info) << "No RICH digi input.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kPsd) ) {
    LOG(info) << "No PSD digi input.";
  }

  // Register output array (CbmEvent)
  fEvents = new TClonesArray("CbmEvent",100);
  ioman->Register("CbmEvent", "Cbm Event", fEvents,
                                        IsOutputBranchPersistent("CbmEvent"));

  if ( ! fEvents ) LOG(fatal) << "Output branch was not created";


  fDiffTime = new TH1F("fDiffTime","Time difference between two consecutive digis;time diff [ns];Counts", 420, -100.5, 1999.5);

  return kSUCCESS;
}

// ---- ReInit  -------------------------------------------------------
InitStatus CbmMcbm2018EventBuilder::ReInit()
{
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void CbmMcbm2018EventBuilder::Exec(Option_t* /*option*/)
{

  LOG_IF(info, fNrTs%1000 == 0) <<"Begin of TS " << fNrTs;

  InitSorter();

  BuildEvents();

  LOG(debug) << "Found " << fEventVector.size() << " events";

  FillHisto();

  DefineGoodEvents();

  LOG(debug) << "Found " << fEventVector.size() << " triggered events";

  FillOutput();

  fNrTs++;

}

void CbmMcbm2018EventBuilder::InitSorter()
{
  // Fill the first entry of each TClonesarray to the std::set
  // The sorting should be done using the time of the digi which
  // can be received using the GetTime() function of CbmDigi

  Int_t nrT0Digis{0};
  if ( fT0DigiVec ) nrT0Digis = fT0DigiVec->size();
  else if ( fT0DigiArr ) nrT0Digis = fT0DigiArr->GetEntriesFast();
  Int_t nrStsDigis  = fDigiMan->GetNofDigis( ECbmModuleId::kSts );
  Int_t nrMuchDigis = fDigiMan->GetNofDigis( ECbmModuleId::kMuch );
  Int_t nrTofDigis  = fDigiMan->GetNofDigis( ECbmModuleId::kTof );
  Int_t nrRichDigis = fDigiMan->GetNofDigis( ECbmModuleId::kRich );
  Int_t nrPsdDigis  = fDigiMan->GetNofDigis( ECbmModuleId::kPsd );

  LOG(debug) << "T0Digis: " << nrT0Digis;
  LOG(debug) << "StsDigis: " << nrStsDigis;
  LOG(debug) << "MuchDigis: " << nrMuchDigis;
  LOG(debug) << "TofDigis: " << nrTofDigis;
  LOG(debug) << "RichDigis: " << nrRichDigis;
  LOG(debug) << "PsdDigis: " << nrPsdDigis;

//  CbmDigi* digi = nullptr;

  if (nrT0Digis>0) {
    AddDigiToSorter< CbmTofDigi >(ECbmModuleId::kHodo, 0);
  }
  if (nrStsDigis>0) {
    AddDigiToSorter< CbmStsDigi >(ECbmModuleId::kSts, 0);
  }
  if (nrMuchDigis>0) {
    AddDigiToSorter< CbmMuchBeamTimeDigi >(ECbmModuleId::kMuch, 0);
  }
  if (nrTofDigis>0) {
    AddDigiToSorter< CbmTofDigi >(ECbmModuleId::kTof, 0);
  }
  if (nrRichDigis>0) {
    AddDigiToSorter< CbmRichDigi >(ECbmModuleId::kRich, 0);
  }
  if (nrPsdDigis>0) {
    AddDigiToSorter< CbmPsdDigi >(ECbmModuleId::kPsd, 0);
  }
  for ( const auto& data: fSorter) {
    LOG(debug) << "Array, Entry(" << data.second.first << ", "
	       << data.second.second << "): " << fixed
	       << setprecision(15) << data.first << " ns";
  }

  // Get the first element of the set from which one gets the first
  // element of the tuple (digi) from which one gets the smallest time
  // of all digis of the new TS
  if (fSorter.size() > 0) {
    fPrevTime = fSorter.begin()->first;
    fStartTimeEvent = fPrevTime;
  }
}

void CbmMcbm2018EventBuilder::BuildEvents()
{
  // Create a first CbmEvent
  fCurrentEvent = new CbmEvent(fCurEv++, fStartTimeEvent, 0.);

  while (fSorter.size() > 0) {

    // Extract the needed information from the first element of the set
    // The first element is the one with the smallest time
    auto it = fSorter.begin();
    Double_t time = it->first;
    ECbmModuleId system = it->second.first;
    Int_t entry = it->second.second;

    // Decide if the digi belongs to the current event or if
    // it starts a new event
    if (! IsDigiInEvent(time)) {
      fCurrentEvent->SetEndTime(fPrevTime);
      fEventVector.push_back(fCurrentEvent);
      // Create then next CbmEvent
      fStartTimeEvent = time;
      fCurrentEvent = new CbmEvent(fCurEv++, fStartTimeEvent, 0.);
    }
    AddDigiToEvent(system, entry);

    if (fFillHistos) fVect.emplace_back(make_pair(system, entry));

    // Remove the first element from the set and insert the next digi
    // from the same system
    fSorter.erase(fSorter.begin());

    switch( system )
    {
      case ECbmModuleId::kSts:
      {
        AddDigiToSorter< CbmStsDigi >(system, ++entry);
        break;
      } // case ECbmModuleId::kHodo
      case ECbmModuleId::kMuch:
      {
        AddDigiToSorter< CbmMuchBeamTimeDigi >(system, ++entry);
        break;
      } // case ECbmModuleId::kHodo
      case ECbmModuleId::kTof:
      {
        AddDigiToSorter< CbmTofDigi >(system, ++entry);
        break;
      } // case ECbmModuleId::kHodo
      case ECbmModuleId::kRich:
      {
        AddDigiToSorter< CbmRichDigi >(system, ++entry);
        break;
      } // case ECbmModuleId::kHodo
      case ECbmModuleId::kPsd:
      {
        AddDigiToSorter< CbmPsdDigi >(system, ++entry);
        break;
      } // case ECbmModuleId::kHodo
      case ECbmModuleId::kHodo:
      {
        AddDigiToSorter< CbmTofDigi >(system, ++entry);
        break;
      } // case ECbmModuleId::kHodo
      default:
      {
        break;
      } // default:
    } //? system

    fPrevTime = time;
  }
  fCurrentEvent->SetEndTime(fPrevTime);
  fEventVector.push_back(fCurrentEvent);
}

Bool_t CbmMcbm2018EventBuilder::IsDigiInEvent(Double_t time)
{
  // here the different possibilities have to be implemented
  if (EventBuilderAlgo::FixedTimeWindow == fEventBuilderAlgo) {
    return ((time-fStartTimeEvent < fFixedTimeWindow) ? kTRUE : kFALSE);
  } else {
    return ((time-fPrevTime < fMaximumTimeGap) ? kTRUE : kFALSE);
  }
}

Bool_t CbmMcbm2018EventBuilder::HasTrigger(CbmEvent* event)
{
  Bool_t hasTrigger{kTRUE};
  if ( (fT0DigiVec || fT0DigiArr) && fTriggerMinT0Digis > 0) {
    hasTrigger = hasTrigger && (event->GetNofData(ECbmDataType::kT0Digi) >= fTriggerMinT0Digis);
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kSts) && fTriggerMinStsDigis > 0) {
    hasTrigger = hasTrigger && (event->GetNofData(ECbmDataType::kStsDigi) >= fTriggerMinStsDigis);
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kMuch) && fTriggerMinMuchDigis > 0) {
    hasTrigger = hasTrigger && (event->GetNofData(ECbmDataType::kMuchDigi) >= fTriggerMinMuchDigis);
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kTof) && fTriggerMinTofDigis > 0) {
    hasTrigger = hasTrigger && (event->GetNofData(ECbmDataType::kTofDigi) >= fTriggerMinTofDigis);
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kRich) && fTriggerMinRichDigis > 0) {
    hasTrigger = hasTrigger && (event->GetNofData(ECbmDataType::kRichDigi) >= fTriggerMinRichDigis);
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kPsd) && fTriggerMinPsdDigis > 0) {
    hasTrigger = hasTrigger && (event->GetNofData(ECbmDataType::kPsdDigi) >= fTriggerMinPsdDigis);
  }

  return hasTrigger;
}

void CbmMcbm2018EventBuilder::FillHisto()
{
  /*
  fPrevTime = 0.;

  ECbmModuleId prevSystem{ECbmModuleId::kNofSystems};
  Int_t prevEntry{-1};


  for ( const auto& _pair: fVect) {
    ECbmModuleId _system = _pair.first;
    Int_t _entry = _pair.second;
    CbmDigi* digi = static_cast<CbmDigi*>(fLinkArray[_system]->At(_entry));
    Double_t difftime = digi->GetTime() - fPrevTime;
    if (fFillHistos) fDiffTime->Fill(difftime);
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
  fVect.clear();

  */
}

void CbmMcbm2018EventBuilder::DefineGoodEvents()
{
  for (auto it = fEventVector.begin(); it != fEventVector.end(); /*increased in the loop*/ ) {
    if (!HasTrigger((*it))) {
      delete (*it);
      it = fEventVector.erase(it);
    } else {
      ++it;
    }
  }
}

void CbmMcbm2018EventBuilder::FillOutput()
{
  // Clear TClonesArray before usage.
  fEvents->Delete();

  // Move CbmEvent from temporary vector to TClonesArray
  for (auto event: fEventVector) {
    LOG(debug) << "Vector: " << event->ToString();
    new ( (*fEvents)[fEvents->GetEntriesFast()] ) CbmEvent(std::move(*event));
     LOG(debug) << "TClonesArray: "
               << static_cast<CbmEvent*>(fEvents->At(fEvents->GetEntriesFast()-1))->ToString();
  }

  // Clear event vector after usage
  // Need to delete the object the pointer points to first
  int counter = 0;
  for (auto event:  fEventVector) {
     LOG(debug) << "Event " << counter << " has " << event->GetNofData() << " digis";
     delete event;
     counter++;
  }

  fEventVector.clear();

}

void CbmMcbm2018EventBuilder::AddDigiToEvent(ECbmModuleId _system, Int_t _entry)
{
  // Fill digi index into event
  switch (_system) {
    case ECbmModuleId::kMvd:  fCurrentEvent->AddData(ECbmDataType::kMvdDigi,  _entry); break;
    case ECbmModuleId::kSts:  fCurrentEvent->AddData(ECbmDataType::kStsDigi,  _entry); break;
    case ECbmModuleId::kRich: fCurrentEvent->AddData(ECbmDataType::kRichDigi, _entry); break;
    case ECbmModuleId::kMuch: fCurrentEvent->AddData(ECbmDataType::kMuchDigi, _entry); break;
    case ECbmModuleId::kTrd:  fCurrentEvent->AddData(ECbmDataType::kTrdDigi,  _entry); break;
    case ECbmModuleId::kTof:  fCurrentEvent->AddData(ECbmDataType::kTofDigi,  _entry); break;
    case ECbmModuleId::kPsd:  fCurrentEvent->AddData(ECbmDataType::kPsdDigi,  _entry); break;
    case ECbmModuleId::kHodo: fCurrentEvent->AddData(ECbmDataType::kT0Digi,   _entry); break;
    default:
    break;
  }
}

template < class Digi >
void CbmMcbm2018EventBuilder::AddDigiToSorter( ECbmModuleId _system, Int_t _entry )
{
  LOG(debug) << "Entry: " << _entry;

  Double_t time = -1.;

  switch( _system )
  {
    case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch:
    case ECbmModuleId::kTof:
    case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd:
    {
      const Digi * pDigi = fDigiMan->Get< Digi >(_entry);

      /// Check that _entry is not out of range
      if( nullptr != pDigi )
      {
        time = pDigi->GetTime();
        fSorter.emplace( make_pair( time, make_pair( _system, _entry ) ) );
      } // if( nullptr != pDigi )
      break;
    } // Digi containers controlled by DigiManager
    case ECbmModuleId::kHodo:
    {
//      CbmTofDigi * pDigi;
      if ( fT0DigiVec )
      {
        if( static_cast<UInt_t>(_entry) < fT0DigiVec->size() )
        {
          time = fT0DigiVec->at( _entry ).GetTime();
          fSorter.emplace( make_pair( time, make_pair( _system, _entry ) ) );
        } // if( _entry < fT0DigiVec->size() )
      } // if ( fT0DigiVec )
      else if ( fT0DigiArr )
      {
        if( _entry < fT0DigiArr->GetEntriesFast() )
        {
          time = dynamic_cast< CbmTofDigi * >( fT0DigiArr->At( _entry ) )->GetTime();
          fSorter.emplace( make_pair( time, make_pair( _system, _entry ) ) );
        } // if( _entry < fT0DigiArr->GetEntriesFast() )
      }  // else if ( fT0DigiArr )
        else return;
      break;
    } // case ECbmModuleId::kHodo
    default:
    {
      return;
      break;
    } // default:
  } //? system

}

// ---- Finish --------------------------------------------------------
void CbmMcbm2018EventBuilder::Finish()
{
  if (fFillHistos) {
    TFile* old = gFile;
    TFile* outfile = TFile::Open(fOutFileName,"RECREATE");

    fDiffTime->Write();

    outfile->Close();
    delete outfile;

    gFile = old;
  }
  LOG(info) << "Total errors: " << fErrors;
}


ClassImp(CbmMcbm2018EventBuilder)
