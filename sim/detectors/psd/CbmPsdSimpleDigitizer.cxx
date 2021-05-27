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

  TStopwatch timer;
  timer.Start();

  LOG(debug) << fName << ": processing event " << fCurrentEvent << " at t = " << fCurrentEventTime << " ns";

  // Declare some variables
  CbmPsdPoint* point = NULL;
  Int_t modID        = -1;  // module ID
  Int_t scinID       = -1;  // #sciillator

  Double_t edep
    [N_PSD_SECT]
    [N_PSD_MODS];  //SELIM: 49 modules, including central & corner modules (rejected in analysis/flow/eventPlane.cxx)
  memset(edep, 0, (N_PSD_SECT * N_PSD_MODS) * sizeof(Double_t));

  map<pair<int, int>, double> edepmap;

  TVector3 pos;  // Position vector

  //for (Int_t imod=0; imod<100; imod++)                   //SELIM: 49 modules, including central & corner modules (rejected in analysis/flow/eventPlane.cxx)
  for (Int_t imod = 0; imod < N_PSD_MODS; imod++)  //marina
  {
    for (Int_t isec = 0; isec < N_PSD_SECT; isec++) {
      edep[isec][imod] = 0.;
    }
  }

  // Event info (for event time)
  GetEventInfo();

  // Loop over PsdPoints
  Int_t nPoints = fPointArray->GetEntriesFast();

  Int_t sec;

  for (Int_t iPoint = 0; iPoint < nPoints; iPoint++) {
    point = (CbmPsdPoint*) fPointArray->At(iPoint);
    if (!point) continue;

    modID          = point->GetModuleID();    //marina  1-44 (45)
    scinID         = point->GetDetectorID();  //1-60
    Double_t eLoss = point->GetEnergyLoss();

    sec                = (Int_t)((scinID - 1) / 6) + 1;  //marina   1-10
    auto insert_result = edepmap.insert(std::make_pair(std::make_pair(modID, sec), point->GetEnergyLoss()));

    if (!insert_result.second) {  // this entry has existed before
      (*insert_result.first).second += point->GetEnergyLoss();
    }
    //cout <<"PSD modID,scinID,eloss " << modID << ", " << scinID << ", " << eLoss <<endl;

    if (((sec - 1) >= 0 && (sec - 1) < N_PSD_SECT) && ((modID - 1) >= 0 && (modID - 1) < N_PSD_MODS)) {
      edep[sec - 1][modID - 1] += eLoss;
    }

  }  // Loop over MCPoints

  Int_t nDigis = 0;
  /*
     for (Int_t imod=0; imod<N_PSD_MODS; imod++) {
     for (Int_t isec=0; isec<N_PSD_SECT; isec++) {
  //if (edep[isec][imod]<=0.) cout << "!!  edep  !! : " << edep[isec][imod] << endl;
  if ( edep[isec][imod] <= 0. ) continue;
  else {
   */
  for (auto edep_entry : edepmap) {
    modID         = edep_entry.first.first;
    int secID     = edep_entry.first.second;
    Double_t eDep = edep_entry.second;

    //Double_t eLossMIP = edep[isec][imod] / 0.005; // 5MeV per MIP
    Double_t eLossMIP        = eDep / 0.005;  // 5MeV per MIP
    Double_t pixPerMIP       = 15.;           // 15 pix per MIP
    Double_t eLossMIPSmeared = gRandom->Gaus(eLossMIP * pixPerMIP, sqrt(eLossMIP * pixPerMIP)) / pixPerMIP;
    Double_t eLossSmeared    = eLossMIPSmeared * 0.005;
    Double_t eNoise          = gRandom->Gaus(0, 15) / 50. * 0.005;
    eLossSmeared += eNoise;
    // V.F. The digi time is set to the event time. This is a workaround only
    // to integrate PSD in the common digitisation scheme.
    CbmPsdDigi* digi = new CbmPsdDigi(modID, secID, eLossSmeared, fCurrentEventTime);
    SendData(digi);
    nDigis++;
    LOG(debug1) << fName << ": Digi " << nDigis << " Section " << secID << " Module " << modID << " energy "
                << eLossSmeared;
  }
  /*
     }
     }// section
     }//module
   */


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
