/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmTzdDigitize.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 07.11.2022
 **/

#include "CbmTzdDigitize.h"

#include <Logger.h>

#include <TRandom.h>
#include <TStopwatch.h>

#include <iomanip>

using std::fixed;
using std::left;
using std::setprecision;
using std::setw;


// -----   Standard constructor   ------------------------------------------
CbmTzdDigitize::CbmTzdDigitize(double sigma) : CbmDigitize<CbmTzdDigi>("TzdDigitize"), fResolution(sigma) {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTzdDigitize::~CbmTzdDigitize() {}
// -------------------------------------------------------------------------


// -----   Task execution   ------------------------------------------------
void CbmTzdDigitize::Exec(Option_t*)
{

  // --- Start timer and reset counters
  TStopwatch timer;
  timer.Start();

  // --- Get event time
  GetEventInfo();

  // --- Create digi and send it to DAQ
  double digiTime  = fCurrentEventTime + gRandom->Gaus(0., fResolution);
  double charge    = 1.;  // Placeholder
  CbmTzdDigi* digi = new CbmTzdDigi(digiTime, charge);
  if (fCreateMatches) {
    CbmMatch* digiMatch = new CbmMatch();
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
void CbmTzdDigitize::Finish()
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
InitStatus CbmTzdDigitize::Init()
{
  std::cout << std::endl;
  LOG(info) << "==========================================================";
  LOG(info) << GetName() << ": Initialisation";
  LOG(info) << "Time resolution is " << fResolution << " [ns]";
  RegisterOutput();
  LOG(info) << GetName() << ": Initialisation successful";
  LOG(info) << "==========================================================";
  std::cout << std::endl;
  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Private method ReInit   -----------------------------------------
InitStatus CbmTzdDigitize::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


ClassImp(CbmTzdDigitize)
