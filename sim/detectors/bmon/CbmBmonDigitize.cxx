/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmBmonDigitize.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 07.11.2022
 **/

#include "CbmBmonDigitize.h"

#include "CbmMatch.h"

#include <Logger.h>

#include <TRandom.h>
#include <TStopwatch.h>

#include <iomanip>

using std::fixed;
using std::left;
using std::setprecision;
using std::setw;


// -----   Standard constructor   ------------------------------------------
CbmBmonDigitize::CbmBmonDigitize(double sigma) : CbmDigitize<CbmBmonDigi>("BmonDigitize"), fResolution(sigma) {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmBmonDigitize::~CbmBmonDigitize() {}
// -------------------------------------------------------------------------


// -----   Task execution   ------------------------------------------------
void CbmBmonDigitize::Exec(Option_t*)
{

  // --- Start timer and reset counters
  TStopwatch timer;
  timer.Start();

  // --- Get MC event information
  GetEventInfo();

  // --- Create digi and send it to DAQ
  double digiTime   = fCurrentEventTime + gRandom->Gaus(0., fResolution);
  double charge     = 1.;  // Placeholder
  CbmBmonDigi* digi = new CbmBmonDigi(ToIntegralType<ECbmModuleId>(ECbmModuleId::kT0), digiTime, charge);
  if (fCreateMatches) {
    CbmMatch* digiMatch = new CbmMatch();
    digiMatch->AddLink(1., -1, fCurrentMCEntry, fCurrentInput);
    SendData(digiTime, digi, digiMatch);
  }
  else
    SendData(digiTime, digi);

  // --- Event log
  LOG(info) << left << setw(15) << GetName() << "[" << fixed << setprecision(3) << timer.RealTime() << " s]"
            << " event time: " << fCurrentEventTime << " ns, measurement time " << digiTime << " ns";

  // --- Monitor
  timer.Stop();
  fNofEvents++;
  fTimeTot += timer.RealTime();
}
// -------------------------------------------------------------------------


// -----   Finish run    ---------------------------------------------------
void CbmBmonDigitize::Finish()
{
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Events processed       : " << fNofEvents;
  LOG(info) << "Real time per event    : " << fTimeTot / Double_t(fNofEvents) << " s";
  LOG(info) << "=====================================";
}
// -------------------------------------------------------------------------


// -----   Initialisation    -----------------------------------------------
InitStatus CbmBmonDigitize::Init()
{
  std::cout << std::endl;
  LOG(info) << "==========================================================";
  LOG(info) << GetName() << ": Initialisation";
  LOG(info) << "Time resolution is " << fResolution << " [ns]";

  RegisterOutput();

  // --- Read list of inactive channels
  if (!fInactiveChannelFileName.IsNull()) {
    LOG(info) << GetName() << ": Reading inactive channels from " << fInactiveChannelFileName;
    auto result = ReadInactiveChannels();
    if (!result.second) {
      LOG(error) << GetName() << ": Error in reading from file! Task will be inactive.";
      return kFATAL;
    }
    LOG(info) << GetName() << ": " << std::get<0>(result) << " lines read from file, " << fInactiveChannels.size()
              << " channels set inactive";
  }

  LOG(info) << GetName() << ": Initialisation successful";
  LOG(info) << "==========================================================";
  std::cout << std::endl;
  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Private method ReInit   -----------------------------------------
InitStatus CbmBmonDigitize::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


ClassImp(CbmBmonDigitize)
