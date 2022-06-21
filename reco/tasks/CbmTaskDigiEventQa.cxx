/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "CbmTaskDigiEventQa.h"

#include "CbmDigiBranchBase.h"
#include "CbmDigiManager.h"
#include "CbmDigiTimeslice.h"
#include "CbmModuleList.h"
#include "CbmReco.h"  // for CbmRecoConfig

#include <FairLogger.h>
#include <FairRunOnline.h>

#include <TH1F.h>
#include <THttpServer.h>
#include <TStopwatch.h>

#include <cassert>
#include <iomanip>

using namespace std;

// -----   Constructor   -----------------------------------------------------
CbmTaskDigiEventQa::CbmTaskDigiEventQa() : FairTask("DigiEventQa") {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTaskDigiEventQa::~CbmTaskDigiEventQa() {}
// ---------------------------------------------------------------------------


// -----   Configuration   ---------------------------------------------------
void CbmTaskDigiEventQa::Config(const CbmRecoConfig& config)
{
  auto limitIt = config.fEvtbuildWindows.find(ECbmModuleId::kSts);
  if (limitIt != config.fEvtbuildWindows.end()) {
    double lower     = limitIt->second.first - 10.;
    double upper     = limitIt->second.second + 10.;
    fHistDigiTimeSts = new TH1F("hDigiTimeSts", "STS digi time in event", 100, lower, upper);
  }
}
// ---------------------------------------------------------------------------


// -----   Execution   -------------------------------------------------------
void CbmTaskDigiEventQa::Exec(Option_t*)
{

  // --- Timer and counters
  TStopwatch timer;
  timer.Start();
  double sumT          = 0.;
  double sumTsq        = 0.;
  double sumNumDigis   = 0.;
  double sumNumDigisSq = 0.;
  size_t numEvents     = 0;
  size_t numDigis      = 0;

  // --- Event loop
  for (const auto& event : *fEvents) {
    numEvents++;
    double numDigisEvt = double(event.fData.fSts.fDigis.size());
    sumNumDigis += numDigisEvt;
    sumNumDigisSq += numDigisEvt * numDigisEvt;
    for (const auto& digi : event.fData.fSts.fDigis) {
      numDigis++;
      const double tDigi = digi.GetTime() - event.fTime;
      if (fHistDigiTimeSts) fHistDigiTimeSts->Fill(tDigi);
      sumT += tDigi;
      sumTsq += tDigi * tDigi;
    }
  }

  // --- First and second moments of digi times and digi numbers
  double tMean          = sumT / double(numDigis);
  double tSqMean        = sumTsq / double(numDigis);
  double tRms           = sqrt(tSqMean - tMean * tMean);
  double numDigisMean   = sumNumDigis / double(numEvents);
  double numDigisSqMean = sumNumDigisSq / double(numEvents);
  double numDigisRms    = sqrt(numDigisSqMean - numDigisMean * numDigisMean);

  // --- Timeslice log
  timer.Stop();
  fExecTime += timer.RealTime();
  stringstream logOut;
  logOut << setw(15) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << ", events " << numEvents << ", digis " << numDigis << ", digis per event ("
         << numDigisMean << " +- " << numDigisRms << ")"
         << ",  digi time (" << tMean << " +- " << tRms << ") ns";
  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  fNumEvents += numEvents;
  fNumDigis += numDigis;


  // See: macro/run/run_unpack_online.C
  // See : recp/detectors/sts/unpack/CbmStsUnpackMonitor
}
// ----------------------------------------------------------------------------


// -----   End-of-timeslice action   ------------------------------------------
void CbmTaskDigiEventQa::Finish()
{
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Timeslices : " << fNumTs;
  LOG(info) << "Events     : " << fNumEvents;
  LOG(info) << "Digis      : " << fNumDigis;
  LOG(info) << "Exec time  : " << fixed << setprecision(2) << 1000. * fExecTime / double(fNumTs) << " ms / TS";
  if (fHistDigiTimeSts)
    LOG(info) << "STS digi times : entries " << fHistDigiTimeSts->GetEntries() << ", mean "
              << fHistDigiTimeSts->GetMean() << ", stddev " << fHistDigiTimeSts->GetStdDev();
  LOG(info) << "=====================================";
  if (fHistDigiTimeSts) fHistDigiTimeSts->Write();
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
InitStatus CbmTaskDigiEventQa::Init()
{
  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Initialising...";

  // --- Input data
  fEvents = ioman->InitObjectAs<const std::vector<CbmDigiEvent>*>("DigiEvent");
  if (!fEvents) {
    LOG(error) << GetName() << ": No input branch DigiEvent!";
    return kFATAL;
  }
  LOG(info) << "--- Found branch DigiEvent";

  // --- Register histograms
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (server) {
    LOG(info) << "--- Http server present; registering histograms";
    if (fHistDigiTimeSts) server->Register("DigiEvent", fHistDigiTimeSts);
  }
  else
    LOG(info) << "--- No Http server present";

  LOG(info) << "==================================================";

  return kSUCCESS;
}
// ----------------------------------------------------------------------------


ClassImp(CbmTaskDigiEventQa)
