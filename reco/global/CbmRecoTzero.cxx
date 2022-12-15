/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#include "CbmRecoTzero.h"

#include "CbmEvent.h"
#include "CbmTzdDigi.h"

#include <FairRootManager.h>
#include <Logger.h>

#include <TClonesArray.h>
#include <TStopwatch.h>

#include <cassert>
#include <iomanip>
#include <sstream>


using std::fixed;
using std::left;
using std::right;
using std::setprecision;
using std::setw;


// -----   Constructor   ---------------------------------------------------
CbmRecoTzero::CbmRecoTzero(const char* name) : FairTask(name) {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmRecoTzero::~CbmRecoTzero() {}
// -------------------------------------------------------------------------


// -----   Initialization   ------------------------------------------------
InitStatus CbmRecoTzero::Init()
{

  std::cout << std::endl;
  LOG(info) << "==========================================================";
  LOG(info) << GetName() << ": Initialisation";

  // --- Get FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Get TzdDigi array
  fTzdDigis = ioman->InitObjectAs<const std::vector<CbmTzdDigi>*>("TzdDigi");
  if (!fTzdDigis) {
    LOG(error) << GetName() << ": No TzdDigi array!";
    return kERROR;
  }
  LOG(info) << "--- Found branch TzdDigi";

  // --- Get CbmEvent array
  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("Event"));
  if (fEvents) { LOG(info) << "--- Found branch Event"; }
  else {
    fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
    if (!fEvents) {
      LOG(error) << GetName() << ": No Event nor CbmEvent branch! Task will be inactive.";
      return kERROR;
    }
    LOG(info) << "--- Found branch CbmEvent";
  }

  LOG(info) << GetName() << ": Initialisation successful";
  LOG(info) << "==========================================================";
  std::cout << std::endl;
  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Public method Exec   --------------------------------------------
void CbmRecoTzero::Exec(Option_t*)
{

  // Timer
  TStopwatch timer;
  timer.Start();
  CbmRecoTzeroMoniData tsMonitor {};

  // Event loop
  Int_t nEvents = fEvents->GetEntriesFast();
  for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
    CbmEvent* event = dynamic_cast<CbmEvent*>(fEvents->At(iEvent));
    assert(event);
    Int_t nDigis = event->GetNofData(ECbmDataType::kT0Digi);
    double tzero = -999999.;
    switch (nDigis) {

      // If there is no TZD digi, set t0 to -999999 (error code).
      case 0: {
        tzero = -999999.;
        tsMonitor.fNumEvtsTzd0++;
        break;
      }

      // If there is exactly one TZD digi, take the event time from there
      case 1: {
        uint32_t digiIndex = event->GetIndex(ECbmDataType::kT0Digi, 0);
        tzero              = fTzdDigis->at(digiIndex).GetTime();
        tsMonitor.fNumEvtsTzd1++;
        break;
      }

      // If there are more than one TZD digis, set t0 to -999999 (error code).
      default: {
        tzero = -999999.;
        tsMonitor.fNumEvtsTzdn++;
        break;
      }
    }

    event->SetTzero(tzero);
    tsMonitor.fNumEvents++;
  }


  // Timeslice monitor
  timer.Stop();
  tsMonitor.fExecTime = 1000. * timer.RealTime();
  tsMonitor.fNumTs    = 1;
  std::stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fMonitor.fNumTs << ", events " << tsMonitor.fNumEvents;
  logOut << " (1 TZD: " << tsMonitor.fNumEvtsTzd1;
  logOut << " , 0 TZD: " << tsMonitor.fNumEvtsTzd0;
  logOut << " , n TZD: " << tsMonitor.fNumEvtsTzdn << ")";
  LOG(info) << logOut.str();

  // Run monitor
  fMonitor += tsMonitor;
}
// -------------------------------------------------------------------------


// -----   Public method Finish   ------------------------------------------
void CbmRecoTzero::Finish()
{
  double tExec     = fMonitor.fExecTime / double(fMonitor.fNumTs);
  double evtsPerTs = double(fMonitor.fNumEvents) / double(fMonitor.fNumTs);
  double fracTzd1  = 100. * double(fMonitor.fNumEvtsTzd1) / double(fMonitor.fNumEvents);
  double fracTzd0  = 100. * double(fMonitor.fNumEvtsTzd0) / double(fMonitor.fNumEvents);
  double fracTzdn  = 100. * double(fMonitor.fNumEvtsTzdn) / double(fMonitor.fNumEvents);
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices         : " << fMonitor.fNumTs;
  LOG(info) << "Exec time  / TS     : " << fixed << setprecision(2) << tExec << " ms";
  LOG(info) << "Events / TS         : " << fixed << setprecision(2) << evtsPerTs;
  LOG(info) << "Fraction with 1 TZD : " << fixed << setprecision(2) << fracTzd1 << " %";
  LOG(info) << "Fraction with 0 TZD : " << fixed << setprecision(2) << fracTzd0 << " %";
  LOG(info) << "Fraction with n TZD : " << fixed << setprecision(2) << fracTzdn << " %";
  LOG(info) << "=====================================";
}
// -------------------------------------------------------------------------


ClassImp(CbmRecoTzero)
