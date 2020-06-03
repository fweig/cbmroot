/** @file CbmDaq.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 20 July 2012
 **/
#include "CbmDaq.h"

#include "CbmDigitizeBase.h"    // for CbmDigitizeBase
#include "CbmLink.h"            // for CbmLink
#include "CbmMatch.h"           // for CbmMatch
#include "CbmModuleList.h"      // for CbmModuleList
#include "CbmTimeSlice.h"       // for CbmTimeSlice, CbmTimeSlice::kEvent

#include <FairRootManager.h>    // for FairRootManager
#include <FairTask.h>           // for FairTask, InitStatus, kSUCCESS
#include <FairLogger.h>         // for Logger, LOG, Severity, Severity::debug
#include <FairEventHeader.h>    // for FairEventHeader
#include <FairLogger.h>         // for FairLogger, gLogger
#include <FairRunAna.h>         // for FairRunAna

#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TStopwatch.h>         // for TStopwatch
#include <TString.h>            // for operator<<, TString

#include <cassert>              // for assert
#include <iomanip>              // for setw, setprecision, __iom_t6, __iom_t5
#include <iostream>             // for operator<<, string, basic_ostream, right
#include <sstream>              // for basic_stringstream<>::string_type
#include <string>               // for char_traits
#include <utility>              // for pair

using std::fixed;
using std::left;
using std::map;
using std::pair;
using std::right;
using std::setprecision;
using std::setw;
using std::string;
using std::stringstream;


// =====   Constructor   =====================================================
CbmDaq::CbmDaq(Bool_t eventMode) : FairTask("Daq"),
                   fIsEventByEvent(eventMode),
                   fTimeSliceLength(-1.),
                   fLatency(5000.),
                   fStoreEmptySlices(kFALSE),
                   fTimeEventPrevious(-1.),
                   fNofEvents(0),
                   fNofDigis(0),
									 fNofDigisIgnored(0),
                   fNofTimeSlices(0),
                   fNofTimeSlicesEmpty(0),
                   fTimeDigiFirst(-1.),
                   fTimeDigiLast(-1.),
                   fTimeSliceFirst(-1.),
                   fTimeSliceLast(-1.),
                   fTimer(),
                   fDigis(),
                   fDigitizers(),
                   fTimeSlice(nullptr),
                   fEventList(),
                   fEventsCurrent(nullptr),
                   fEventRange() {
}
// ===========================================================================



// =====   Constructor for event by event mode  ==============================
CbmDaq::CbmDaq(Double_t tsLength) : FairTask("Daq"),
                   fIsEventByEvent(kFALSE),
                   fTimeSliceLength(tsLength),
                   fLatency(2000.),
                   fStoreEmptySlices(kFALSE),
                   fTimeEventPrevious(-1.),
                   fNofEvents(0),
                   fNofDigis(0),
                   fNofDigisIgnored(0),
                   fNofTimeSlices(0),
                   fNofTimeSlicesEmpty(0),
                   fTimeDigiFirst(-1.),
                   fTimeDigiLast(-1.),
                   fTimeSliceFirst(-1.),
                   fTimeSliceLast(-1.),
                   fTimer(),
                   fDigis(),
                   fDigitizers(),
                   fTimeSlice(nullptr),
                   fEventList(),
                   fEventsCurrent(nullptr),
                   fEventRange() {
}
// ===========================================================================



// =====   Destructor   ======================================================
CbmDaq::~CbmDaq() {
}
// ===========================================================================



// =====   Check output for time sorting   ===================================
Bool_t CbmDaq::CheckOutput() const {
  Bool_t result = kTRUE;
  for ( auto & digitizer : fDigitizers )
    result = ( result && digitizer.second->CheckOutput() );
  return result;
}
// ===========================================================================



// =====   Close the current time slice and fill it to the tree   ============
void CbmDaq::CloseTimeSlice() {

  fNofTimeSlices++;
  if ( fTimeSlice->IsEmpty() ) {
    fNofTimeSlicesEmpty++;
    LOG(debug) << fName << ": Closing " << fTimeSlice->ToString();
  }
  else LOG(info) << fName << ": Closing " << fTimeSlice->ToString();

  // --- No action if time slice is empty and empty slices are not stored
  if ( fTimeSlice->IsEmpty() && ( ! fStoreEmptySlices ) ) return;

  // --- Fill the list of events contributing to the time slice
  CopyEventList();
  fEventsCurrent->Sort();

  // --- Call user-defined method FillCustomData
  for ( auto entry : fDigitizers ) {
    if ( entry.second ) entry.second->FillCustomData(1, kTRUE);
  } //# Digitisers

  // --- Check the output for being time-sorted
  assert( CheckOutput() );

  // --- Fill the ROOT tree
  FairRootManager::Instance()->Fill();

  // --- Bookkeeping and debug
  fTimeSliceLast = fTimeSlice->GetEndTime();
  if ( fNofTimeSlices == 1 || fTimeSlice->GetTimeDataFirst() < fTimeDigiFirst )
    fTimeDigiFirst = fTimeSlice->GetTimeDataFirst();
  if ( fNofTimeSlices == 1 || fTimeSlice->GetTimeDataLast() > fTimeDigiLast )
    fTimeDigiLast = fTimeSlice->GetTimeDataLast();
  if ( gLogger->IsLogNeeded(fair::Severity::debug) ) PrintCurrentEventRange();
  LOG(info) << GetName() << " " << GetBufferStatus();

}
// ===========================================================================



// =====   Copy event list to output branch   ================================
Int_t CbmDaq::CopyEventList() {

  Int_t nMCEvents = 0;
  CbmMatch match = fTimeSlice->GetMatch();
  for (Int_t iLink = 0; iLink < match.GetNofLinks(); iLink++) {
    Int_t input = match.GetLink(iLink).GetFile();
    Int_t event = match.GetLink(iLink).GetEntry();
    Double_t time = fEventList.GetEventTime(event, input);
    fEventsCurrent->Insert(event, input, time);
    nMCEvents++;
  }

  return nMCEvents;
}
// ===========================================================================



// =====   Task execution   ==================================================
void CbmDaq::Exec(Option_t*) {

  // Start timer and digi counter
  fTimer.Start();
  Int_t nDigis = 0;

  // Event info
  Int_t file  = FairRunAna::Instance()->GetEventHeader()->GetInputFileId();
  Int_t event = FairRootManager::Instance()->GetEntryNr();
  Double_t eventTime = FairRunAna::Instance()->GetEventHeader()->GetEventTime();
  fEventList.Insert(event, file, eventTime);

  // Status
  LOG(debug) << GetName() << " " << GetBufferStatus();
  Double_t fillTime = fTimeEventPrevious - fLatency;
  if ( fTimeSlice->IsEvent() ) fillTime = -1.;
  LOG(debug) << GetName() << ": Fill time is " << fillTime << " ns";

  // Regular mode: Time slices up to the previous event time minus the latency
  // can be filled and closed.
  if ( fTimeSlice->IsRegular() ) {
    while ( fTimeSlice->GetEndTime() < fTimeEventPrevious - fLatency ) {
      nDigis += FillTimeSlice(kTRUE, fillTime);
      CloseTimeSlice();
      StartNextTimeSlice();
    }
  }

  // Flexible mode: Fill the time slice from the buffers, but do not close it.
  else if ( fTimeSlice->IsFlexible() ) {
    nDigis += FillTimeSlice(kTRUE, fillTime);
  }

  // Event mode: Fill the time slice and close it.
  else if ( fTimeSlice->IsEvent() ) {
    nDigis += FillTimeSlice(kFALSE);
    CloseTimeSlice();
    StartNextTimeSlice();
  }

  // --- Save event time for next execution
  fTimeEventPrevious = eventTime;

  // --- Event log
  LOG(info) << left << setw(15) << GetName() << "[" << fixed
            << setprecision(3) << fTimer.RealTime() << " s]"
            << " Transported digis: " << nDigis
            << ", " << GetBufferStatus();

  // --- Increase exec counter
  fNofEvents++;
  fNofDigis += nDigis;
}
// ===========================================================================



// =====   Fill current time slice with data from buffers   ==================
ULong64_t CbmDaq::FillTimeSlice(Bool_t timeLimit, Double_t tMax) {

  if ( timeLimit ) LOG(debug) << GetName() << ": Fill time slice up to t = "
      << tMax << " ns";
  else LOG(debug) << GetName() << ": Fill time slice";
  LOG(debug) << GetName() << " " << GetBufferStatus(kTRUE);
  LOG(debug) << GetName() << ": " << fTimeSlice->ToString();

  // --- Move data from DAQ buffers into current time slice
  std::stringstream ss;
  ss << GetName() << ": Fill data: ";
  ULong64_t nDataAll = 0;
  ULong64_t nData = 0;
  for ( auto digitizer : fDigitizers ) {
    if ( timeLimit) nData = digitizer.second->FillTimeSlice(fTimeSlice, tMax);
    else nData = digitizer.second->FillTimeSlice(fTimeSlice);
    ss << " " << CbmModuleList::GetModuleNameCaps(digitizer.first) << " "
       << nData;
    nDataAll += nData;
  }
  LOG(debug) << ss.str();
  LOG(debug) << GetName() << " " << GetBufferStatus(kTRUE);
  LOG(debug) << GetName() << fTimeSlice->ToString();
  LOG(debug) << GetName() << ": total " << nData << " moved";

  return nDataAll;
}
// ===========================================================================



// =====   End-of-run action   ===============================================
void CbmDaq::Finish() {

  std::cout << std::endl;
  LOG(info) << fName << ": Finish run";
  LOG(info) << GetBufferStatus(kTRUE);

  // --- In regular mode: Fill the remaining buffer data into time slices
  // --- until the buffers are empty.
  if ( fTimeSlice->IsRegular() ) {
    do {
      fNofDigis += FillTimeSlice(kFALSE);
      CloseTimeSlice();
      StartNextTimeSlice();
    }
    while ( ! IsDaqBufferEmpty() );
  }

  // --- For flexible time slice: Fill the remaining buffer data
  // --- into the time slice. After that, the buffers should be empty.
  else if ( fTimeSlice->IsFlexible() ) {
    fNofDigis += FillTimeSlice(kFALSE);
    CloseTimeSlice();
    if ( ! IsDaqBufferEmpty() ) {
      LOG(info) << GetBufferStatus();
      LOG(fatal) << GetName() << ": Time-slice mode is flexible but "
          << " buffers are not empty after fill!";
    }
  }

  // --- In event mode, the buffers should be empty
  else {
    if ( ! IsDaqBufferEmpty() ) {
      LOG(info) << GetBufferStatus();
      LOG(fatal) << GetName() << ": Time-slice mode is event but "
          << " buffers are not empty!";
    }
  }


  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Events:        " << setw(10) << right << fNofEvents;
  LOG(info) << "Digis:         " << setw(10) << right << fNofDigis
      << " from " << setw(10) << right << fixed << setprecision(1)
      << fTimeDigiFirst << " ns  to " << setw(10) << right << fixed
      << setprecision(1) << fTimeDigiLast << " ns";
  LOG(info) << "Digis ignored: " << setw(10) << right << fNofDigisIgnored;
  if ( fTimeSlice->IsRegular() )
    LOG(info) << "Time slices:   " << setw(10) << right << fNofTimeSlices
              << " from " << setw(10) << right << fixed << setprecision(1)
              << fTimeSliceFirst << " ns  to " << setw(10) << right << fixed
              << setprecision(1) << fTimeSliceLast << " ns";
  else
    LOG(info) << "Time slices:   " << setw(10) << right << fNofTimeSlices;
  LOG(info) << "Empty slices:  " << setw(10) << right << fNofTimeSlicesEmpty;
  LOG(info) << "=====================================";

  std::cout << std::endl;
  LOG(info) << fEventList.ToString();
//  fEventList.Print();

}
// ===========================================================================



// =====   Number of data in DAQ buffers   ===================================
ULong64_t CbmDaq::GetBufferSize() const {
  ULong64_t nData = 0;
  for ( auto & digitizer : fDigitizers )
    nData += digitizer.second->GetDaqBufferSize();
  return nData;
}
// ===========================================================================



// =====   DAQ buffer status to string   =====================================
std::string CbmDaq::GetBufferStatus(Bool_t verbose) const {
  stringstream ss;
  if ( IsDaqBufferEmpty() ) {
    ss << "Buffer status: empty";
    return ss.str();
  }
  ss << "Buffer status: " << GetBufferSize() << " data from t = "
     << GetBufferTimeFirst() << " to " << GetBufferTimeLast() << " ns";
  if ( verbose ) {
    for ( auto & digitizer : fDigitizers )
      ss << "\n       " << CbmModuleList::GetModuleNameCaps(digitizer.first)
         << " " << digitizer.second->GetDaqBufferStatus();
  }
  return ss.str();
}
// ===========================================================================



// =====   Time of first datum in DAQ buffers   ==============================
Double_t CbmDaq::GetBufferTimeFirst() const {
  Double_t tMin = -1.;
  for ( auto & digitizer : fDigitizers ) {
    Double_t test = digitizer.second->GetDaqBufferTimeFirst();
    if ( tMin < 0. ) tMin = test;
    else tMin = ( tMin < test ? tMin : test );
  }
  return tMin;
}
// ===========================================================================



// =====   Time of last datum in DAQ buffers   ===============================
Double_t CbmDaq::GetBufferTimeLast() const {
  Double_t tMax = -1.;
  for ( auto & digitizer : fDigitizers ) {
    Double_t test = digitizer.second->GetDaqBufferTimeLast();
    if ( tMax < 0. ) tMax = test;
    else tMax = ( tMax > test ? tMax : test );
  }
  return tMax;
}
// ===========================================================================



// =====   Task initialisation   =============================================
InitStatus CbmDaq::Init() {

  std::cout << std::endl;
  LOG(info) << "==========================================================";
  LOG(info) << fName << ": Initialisation";


  // --- Set the initial time slice
  if ( fIsEventByEvent ) {
    LOG(info) << fName << ": Event mode";
    fTimeSlice = new CbmTimeSlice(CbmTimeSlice::kEvent);
  }
  else {
    if ( fTimeSliceLength > 0. ) {
      LOG(info) << fName << ": Time-based mode, time slice duration "
          << fTimeSliceLength << " ns";
      fTimeSlice = new CbmTimeSlice(0., fTimeSliceLength);
    }
    else {
      LOG(info) << fName << ": Time-based mode, flexible time slice";
      fTimeSlice = new CbmTimeSlice(CbmTimeSlice::kFlexible);
    }
  }
  fTimeEventPrevious = -1.;
  fTimeSliceFirst = fTimeSlice->GetStartTime();


  // --- Register output branch TimeSlice
  FairRootManager::Instance()->Register("TimeSlice.",
                                        "DAQ", fTimeSlice, kTRUE);

  // --- Register output branch MCEventList
  fEventsCurrent = new CbmMCEventList();
  FairRootManager::Instance()->Register("MCEventList.", "DAQ",
                                        fEventsCurrent, kTRUE);

  LOG(info) << GetName() << ": Initialisation successful";
  LOG(info) << "==========================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ===========================================================================



// =====   Check for empty DAQ buffers   =====================================
Bool_t CbmDaq::IsDaqBufferEmpty() const {
  Bool_t empty = kTRUE;
  for ( auto digitizer : fDigitizers ) {
    if ( digitizer.second->GetDaqBufferSize() ) {
      empty = kFALSE;
      break;
    }
  }
  return empty;
}
// ===========================================================================



// =====   Print current event range   =======================================
void CbmDaq::PrintCurrentEventRange() const {

  std::stringstream ss;
  ss << GetName() << ": Current MC event range: ";
  if ( fEventRange.empty() ) {
    ss << "empty";
    LOG(info) << ss.str();
    return;
  }
  auto it = fEventRange.begin();
  while ( it != fEventRange.end() ) {
    Int_t file = it->first;
    Int_t firstEvent = it->second.first;
    Int_t lastEvent = it->second.second;
    ss << "\n          Input file " << file << ", first event "
        << firstEvent << ", last event " << lastEvent;
    it++;
  }  //# inputs
  LOG(info) << ss.str();

}
// ===========================================================================



// =====   Set the digitizer for a given system   ============================
void CbmDaq::SetDigitizer(ECbmModuleId system, CbmDigitizeBase* digitizer) {
  assert(digitizer);
  fDigitizers[system] = digitizer;
}
// ===========================================================================



// =====   Start a new time slice   ==========================================
void CbmDaq::StartNextTimeSlice() {

  // --- Reset the time slice header
  if ( fTimeSlice->IsRegular() ) {
    Double_t newStart = fTimeSlice->GetStartTime() + fTimeSliceLength;
    fTimeSlice->Reset(newStart, fTimeSliceLength);
  }
  else fTimeSlice->Reset();

  // --- Reset event range and event list
  fEventRange.clear();
  fEventsCurrent->Clear("");

  // --- Clear data output arrays
  for ( auto entry : fDigitizers ) entry.second->ClearOutput();

}
// ===========================================================================








ClassImp(CbmDaq)

