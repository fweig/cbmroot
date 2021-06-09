/* Copyright (C) 2014-2020 Institute for Theoretical and Experimental Physics named by A.I. Alikhanov of National Research Centre "Kurchatov Institute" (ITEP) in Moscow, Russia, Moscow
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mikhail Prokudin [committer], Volker Friese */

/** @file CbmBuildEventsSimple.cxx
 ** @author //Dr.Sys <Mikhail.Prokudin@cern.ch>
 ** @date 07.12.2016
 **/

#include "CbmBuildEventsSimple.h"

#include "CbmEvent.h"
#include "CbmLink.h"
#include "CbmMatch.h"
#include "CbmStsAddress.h"
#include "CbmStsDigi.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TStopwatch.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>

using namespace std;


// =====   Constructor   =====================================================
CbmBuildEventsSimple::CbmBuildEventsSimple()
  : FairTask("BuildEventsSimple")
  , fStsDigis(NULL)
  , fEvents(NULL)
  , fSliceN(0)
  , fEv(0)
  , fNDigis(0)
  , fSN(-1111)
  , fST(-1111)
  , fNStsDigis()
  , fWindDur(5)
  , fMinusDeltaT(10)
  , fPlusDeltaT(15)
  , fDeadT(10)
  , fMinHitStations(8)
  , fMinDigis(2000)

{
  ;
}
// ===========================================================================


// =====   Destructor   ======================================================
CbmBuildEventsSimple::~CbmBuildEventsSimple() { ; }
// ===========================================================================


// =====   FillEvent method   ================================================
void CbmBuildEventsSimple::FillEvent(Int_t st, Int_t end)
{
  Int_t i;
  //  CbmStsDigi* digi;
  Int_t nev       = fEvents->GetEntriesFast();
  CbmEvent* event = new ((*fEvents)[nev]) CbmEvent(fEv++);

  for (i = st; i <= end; i++) {
    //    digi=(CbmStsDigi*)fStsDigis->At(i);
    event->AddData(ECbmDataType::kStsDigi, i);
  }
  FairRootManager::Instance()->Fill();
  LOG(info) << "CbmBuildEventsSimple:	Event constructed. Digis used from " << st << " to " << end << ".";
}

// ===========================================================================

// =====   Task execution   ==================================================
void CbmBuildEventsSimple::Exec(Option_t*)
{

  // Reset output array
  fEvents->Delete();

  Int_t i = 0;
  Int_t j = 0;
  //  Int_t n=0;
  Int_t k;
  Int_t nsts = fStsDigis->GetEntriesFast();
  Double_t t;
  CbmStsDigi* digi;
  CbmStsDigi* digi2;
  Int_t n0;
  Int_t n1;

  LOG(info) << "CbmBuildEventsSimple:	Sts digis in slice " << nsts;
  fNDigis = 0;
  for (i = 0; i < 16; i++)
    fNStsDigis[i] = 0;
  fSN = 0;
  fST = -1111;

  for (i = 0; i < nsts; i++) {
    digi = (CbmStsDigi*) fStsDigis->At(i);
    if (digi == NULL) continue;
    t = digi->GetTime();
    fNDigis++;
    k = CbmStsAddress::GetElementId(digi->GetAddress(), kStsUnit);
    fNStsDigis[k]++;
    if (fST == -1111) {
      fSN = i;
      fST = t;
    }
    if (t - fST > fWindDur) {
      for (j = fSN; j < i; j++) {
        digi2 = (CbmStsDigi*) fStsDigis->At(j);
        if (digi2 == NULL) continue;
        fST = digi2->GetTime();
        if (t - fST <= fWindDur) {
          fSN = j;
          break;
        }
        fNDigis--;
        k = CbmStsAddress::GetElementId(digi2->GetAddress(), kStsUnit);
        fNStsDigis[k]--;
      }
      if (j == i) {
        fSN = i;
        fST = t;
      }
    }

    if (fNDigis >= fMinDigis) {
      k = 0;
      for (j = 0; j < 16; j++)
        if (fNStsDigis[j] > 0) k++;
      if (k >= fMinHitStations) {
        // Reached required number of digis and hit stations
        for (j = i; j > -1; j--) {
          digi2 = (CbmStsDigi*) fStsDigis->At(j);
          if (digi2 == NULL) continue;
          if (fST - digi2->GetTime() > fMinusDeltaT) break;
        }
        n0 = j + 1;
        for (j = i + 1; j < nsts; j++) {
          digi2 = (CbmStsDigi*) fStsDigis->At(j);
          if (digi2 == NULL) continue;
          if (digi2->GetTime() - t > fPlusDeltaT) break;
        }
        n1 = j - 1;
        FillEvent(n0, n1);
        if (j == nsts) {
          //Reached the end of the slice
          i = j;
          break;
        }
        for (j = n1 + 1; j < nsts; j++) {
          digi2 = (CbmStsDigi*) fStsDigis->At(j);
          if (digi2 == NULL) continue;
          if (digi2->GetTime() - t > fPlusDeltaT + fDeadT) break;
        }
        if (j == nsts) {
          //Reached the end of the slice
          i = j;
          break;
        }
        i = j - 1;
        for (j = 0; j < 16; j++)
          fNStsDigis[j] = 0;
        fNDigis = 0;
        fSN     = i + 1;
        digi2   = (CbmStsDigi*) fStsDigis->At(i + 1);
        fST     = digi2->GetTime();
        /*
	cout << "--> ";
        digi2=(CbmStsDigi*)fSlice->GetData(kSTS, n0);
	cout << n0 << "(" << digi2->GetTime() << ")" << ", ";
        digi2=(CbmStsDigi*)fSlice->GetData(kSTS, n1);
	cout << n1 << "(" << digi2->GetTime() << ")" << ": ";
        digi2=(CbmStsDigi*)fSlice->GetData(kSTS, i);
	cout << i << "(" << digi2->GetTime() << ")" << endl;
*/
      }
    }
    //    cout << t << " " << fST << " " << fNDigis << " :";
    //    for(k=0;k<8;k++) cout << " " << fNStsDigis[k];
    //    cout <<  endl;
  }
  fSliceN++;
}
// ===========================================================================


// =====   Task initialisation   =============================================
InitStatus CbmBuildEventsSimple::Init()
{
  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Get input array (CbmStsDigi)
  fStsDigis = (TClonesArray*) ioman->GetObject("StsDigi");
  assert(fStsDigis);

  // Register output array (CbmStsDigi)
  fEvents = new TClonesArray("CbmEvent", 100);
  ioman->Register("CbmEvent", "Cbm_Event", fEvents, IsOutputBranchPersistent("CbmEvent"));

  return kSUCCESS;
}
// ===========================================================================


ClassImp(CbmBuildEventsSimple)
