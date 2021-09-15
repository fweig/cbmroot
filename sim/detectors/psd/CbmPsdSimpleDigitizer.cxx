/* Copyright (C) 2012-2020 Institute for Nuclear Research, Moscow
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alla Maevskaya, Selim Seddiki, Sergey Morozov [committer], Volker Friese, Evgeny Kashirin */

// -------------------------------------------------------------------------
// -----                CbmPsdSimpleDigitizer source file              -----
// -----                  Created 15/05/12  by  Alla & SELIM & FLORIAN -----
// -----                 Modified 17/03/18  by  Sergey Morozov         -----
// -------------------------------------------------------------------------
#include "CbmPsdSimpleDigitizer.h"

#include "CbmPsdDigi.h"
#include "CbmPsdPoint.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TMath.h"
#include "TStopwatch.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>

using std::cout;
using std::endl;
using std::fixed;
using std::right;
using std::setprecision;
using std::setw;

using std::map;
using std::pair;


// -----   Default constructor   -------------------------------------------
CbmPsdSimpleDigitizer::CbmPsdSimpleDigitizer()
  : CbmDigitize<CbmPsdDigi>("PsdDigitize")
  , fNofEvents(0)
  , fNofPoints(0.)
  , fNofDigis(0.)
  , fTimeTot(0.)
  , fPointArray(NULL)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmPsdSimpleDigitizer::~CbmPsdSimpleDigitizer() {}
// -------------------------------------------------------------------------


// -----   Public method Init   --------------------------------------------
InitStatus CbmPsdSimpleDigitizer::Init()
{

  // Matches are not produced
  fCreateMatches = kFALSE;

  // Get RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman),

    // Get input array
    fPointArray = (TClonesArray*) ioman->GetObject("PsdPoint");
  assert(fPointArray);

  // Create and register output array
  RegisterOutput();

  // Statistics
  fNofEvents = 0;
  fNofPoints = 0;
  fNofDigis  = 0.;
  fTimeTot   = 0.;

  LOG(info) << fName << ": Initialisation successful " << kSUCCESS;
  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Public method Exec   --------------------------------------------
void CbmPsdSimpleDigitizer::Exec(Option_t*)
{
  // Event info (for event time)
  GetEventInfo();

  TStopwatch timer;
  timer.Start();

  // Loop over PsdPoints
  Int_t nPoints = fPointArray->GetEntriesFast();
  LOG(debug) << fName << ": processing event " << fCurrentEvent << " at t = " << fCurrentEventTime << " ns";

  // Declare some variables
  CbmPsdPoint* point = nullptr;
  Int_t modID        = -1;  // module ID
  Int_t scinID       = -1;  // #sciillator
  Int_t sec;
  std::map<UInt_t, CbmPsdDigi> fired_digis_map;  // map<UInt_t uAddress, CbmPsdDigi>

  for (Int_t iPoint = 0; iPoint < nPoints; iPoint++) {
    point = (CbmPsdPoint*) fPointArray->At(iPoint);
    if (!point) continue;

    modID          = point->GetModuleID();    //marina  1-44 (45)
    scinID         = point->GetDetectorID();  //1-60
    Double_t eLoss = point->GetEnergyLoss();
    Double_t pTime     = point->GetTime();
    sec                = (Int_t)((scinID - 1) / 6) + 1;  //marina   1-10
    UInt_t uAddress    = CbmPsdAddress::GetAddress(modID, sec);

    auto it = fired_digis_map.find(uAddress);
    if (it != fired_digis_map.end()) {
      //this key exists
      it->second.SetEdep(it->second.GetEdep() + eLoss);
      if (it->second.GetTime() > pTime) it->second.SetTime(pTime);
    }
    else {
      //this key is new
      CbmPsdDigi digi = CbmPsdDigi(uAddress, pTime, eLoss);
      fired_digis_map.insert(std::make_pair(uAddress, digi));
    }
  }  // Loop over MCPoints


  Int_t nDigis = 0;
  for (auto entry : fired_digis_map) {
    Double_t eDep            = entry.second.GetEdep();
    Double_t eLossMIP        = eDep / 0.005;  // 5MeV per MIP
    Double_t pixPerMIP       = 15.;           // 15 pix per MIP
    Double_t eLossMIPSmeared = gRandom->Gaus(eLossMIP * pixPerMIP, sqrt(eLossMIP * pixPerMIP)) / pixPerMIP;
    Double_t eLossSmeared    = eLossMIPSmeared * 0.005;
    Double_t eNoise          = gRandom->Gaus(0, 15) / 50. * 0.005;
    eLossSmeared += eNoise;
    // The digi time is set to MC point time
    CbmPsdDigi* digi = new CbmPsdDigi(entry.first, entry.second.GetTime(), eLossSmeared);
    SendData(digi);
    nDigis++;
    LOG(debug1) << fName << ": Digi " << nDigis << " Section " << entry.second.GetSectionID() << " Module "
                << entry.second.GetModuleID() << " energy " << eLossSmeared;
  }

  // --- Event log
  timer.Stop();
  LOG(info) << "+ " << setw(15) << GetName() << ": Event " << setw(6) << right << fCurrentEvent << " at " << fixed
            << setprecision(3) << fCurrentEventTime << " ns, points: " << nPoints << ", digis: " << nDigis
            << ". Exec time " << setprecision(6) << timer.RealTime() << " s.";

  // --- Run statistics
  fNofEvents++;
  fNofPoints += nPoints;
  fNofDigis += nDigis;
  fTimeTot += timer.RealTime();
}
// -------------------------------------------------------------------------


// -----   End-of-run   ----------------------------------------------------
void CbmPsdSimpleDigitizer::Finish()
{
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Events processed    : " << fNofEvents;
  LOG(info) << "PsdPoint / event    : " << setprecision(1) << fNofPoints / Double_t(fNofEvents);
  LOG(info) << "PsdDigi / event     : " << fNofDigis / Double_t(fNofEvents);
  LOG(info) << "Digis per point     : " << setprecision(6) << fNofDigis / fNofPoints;
  LOG(info) << "Real time per event : " << fTimeTot / Double_t(fNofEvents) << " s";
  LOG(info) << "=====================================";
}
// -------------------------------------------------------------------------


ClassImp(CbmPsdSimpleDigitizer)
