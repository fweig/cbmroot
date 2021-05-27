/** @file CbmEvBuildSource.cxx
  * @copyright Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [orginator] **/

#include "CbmEvBuildSource.h"

#include "CbmDigi.h"
#include "CbmStsAddress.h"
#include "CbmStsDigi.h"
#include "CbmTimeSlice.h"

#include "FairEventHeader.h"
#include "FairRootManager.h"
#include <Logger.h>

#include "TChain.h"
#include "TClonesArray.h"

#include <iostream>

using namespace std;

CbmEvBuildSource::CbmEvBuildSource()
  : FairSource()
  , fCh(NULL)
  , fN(-1111)
  , fI(-1111)
  , fSTSDigi(NULL)
  , fNSTSDigis(-1111)
  , fSlice(NULL)
  , fISts(0)
  , fEv(0)
  , fNDigis(0)
  , fSN(-1111)
  , fST(-1111)
  , fWindDur(2)
  , fMinusDeltaT(2)
  , fPlusDeltaT(5)
  , fDeadT(10)
  , fMinHitStations(8)
  , fMinDigis(2000)
  , fEvHeader(NULL)
{
  fCh = new TChain("cbmsim");
}

CbmEvBuildSource::CbmEvBuildSource(const char* fname)
  : FairSource()
  , fCh(NULL)
  , fN(-1111)
  , fI(-1111)
  , fSTSDigi(NULL)
  , fNSTSDigis(-1111)
  , fSlice(NULL)
  , fISts(0)
  , fEv(0)
  , fNDigis(0)
  , fSN(-1111)
  , fST(-1111)
  , fWindDur(2)
  , fMinusDeltaT(2)
  , fPlusDeltaT(5)
  , fDeadT(10)
  , fMinHitStations(8)
  , fMinDigis(2000)
  , fEvHeader(NULL)
{
  fCh = new TChain("cbmsim");
  AddFile(fname);
}

CbmEvBuildSource::CbmEvBuildSource(const CbmEvBuildSource& source)
  : FairSource(source)
  , fCh(NULL)
  , fN(-1111)
  , fI(-1111)
  , fSTSDigi(NULL)
  , fNSTSDigis(-1111)
  , fSlice(NULL)
  , fISts(0)
  , fEv(0)
  , fNDigis(0)
  , fSN(-1111)
  , fST(-1111)
  , fWindDur(2)
  , fMinusDeltaT(2)
  , fPlusDeltaT(5)
  , fDeadT(10)
  , fMinHitStations(8)
  , fMinDigis(2000)
  , fEvHeader(NULL)
{
  ;
}

Int_t CbmEvBuildSource::ReadEvent(UInt_t par)
{
  Int_t j = 0;
  //  Int_t n=0;
  Int_t k;
  Int_t nsts;
  Double_t t;
  CbmStsDigi* digi;
  CbmStsDigi* digi2;
  Int_t n0;
  Int_t n1;

  cout << par << " " << fEv << endl;
  if (par == 0) {
    fI  = -1;
    fEv = 0;
  }
  else if (par != fEv) {
    LOG(fatal) << "CbmEvBuildSource::ReadEvent(" << par << "): Can't jump to arbitrary event but 0. Current event is "
               << fEv;
    return 3;
  }

  if (fI == -1)
    if (GetNextTimeSlice() < 0) return 2;  //No entries in input chain

  for (;;) {
    nsts = fSlice->GetDataSize(kSts);
    for (; fISts < nsts; fISts++) {
      digi = (CbmStsDigi*) fSlice->GetData(kSts, fISts);
      if (digi == NULL) continue;
      t = digi->GetTime();
      fNDigis++;
      k = CbmStsAddress::GetElementId(digi->GetAddress(), kStsUnit);
      fNStsDigis[k]++;
      if (fST == -1111) {
        fSN = fISts;
        fST = t;
      }
      if (t - fST > fWindDur) {
        for (j = fSN; j < fISts; j++) {
          digi2 = (CbmStsDigi*) fSlice->GetData(kSts, j);
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
        if (j == fISts) {
          fSN = fISts;
          fST = t;
        }
      }

      if (fNDigis >= fMinDigis) {
        k = 0;
        for (j = 0; j < 16; j++)
          if (fNStsDigis[j] > 0) k++;
        if (k >= fMinHitStations) {
          // Reached required number of digis and hit stations
          for (j = fISts; j > -1; j--) {
            digi2 = (CbmStsDigi*) fSlice->GetData(kSts, j);
            if (digi2 == NULL) continue;
            if (fST - digi2->GetTime() > fMinusDeltaT) break;
          }
          n0 = j + 1;
          for (j = fISts + 1; j < nsts; j++) {
            digi2 = (CbmStsDigi*) fSlice->GetData(kSts, j);
            if (digi2 == NULL) continue;
            if (digi2->GetTime() - t > fPlusDeltaT) break;
          }
          n1 = j - 1;
          FillEvent(n0, n1);
          if (j == nsts) {
            //Reached the end of the slice
            fISts = j;
            return 0;
          }
          for (j = n1 + 1; j < nsts; j++) {
            digi2 = (CbmStsDigi*) fSlice->GetData(kSts, j);
            if (digi2 == NULL) continue;
            if (digi2->GetTime() - t > fPlusDeltaT + fDeadT) break;
          }
          if (j == nsts) {
            //Reached the end of the slice
            fISts = j;
            return 0;
          }
          fISts = j;
          for (j = 0; j < 16; j++)
            fNStsDigis[j] = 0;
          fNDigis = 0;
          fSN     = fISts;
          digi2   = (CbmStsDigi*) fSlice->GetData(kSts, fISts);
          fST     = digi2->GetTime();
          return 0;
          /*	
	  cout << "--> ";
          digi2=(CbmStsDigi*)fSlice->GetData(kSTS, n0);
	  cout << n0 << "(" << digi2->GetTime() << ")" << ", ";
          digi2=(CbmStsDigi*)fSlice->GetData(kSTS, n1);
	  cout << n1 << "(" << digi2->GetTime() << ")" << ": ";
          digi2=(CbmStsDigi*)fSlice->GetData(kSTS, i);
	  cout << i << "(" << digi2->GetTime() << ")" << endl;
*/
        }  // if (k>=fMinHitStations)
      }    // if (fNDigis>=fMinDigis)
    }      // for(;fISts<nsts;fISts++)
    if (GetNextTimeSlice() < 0) return 1;
    //    cout << t << " " << fST << " " << fNDigis << " :";
    //    for(k=0;k<8;k++) cout << " " << fNStsDigis[k];
    //    cout <<  endl;
  }

  // This should be unreachable
  return 1111;
}

void CbmEvBuildSource::Close() { return; }

Int_t CbmEvBuildSource::GetNextTimeSlice()
{
  Int_t i;

  fI++;
  if (fI == fN) return -1;
  fCh->GetEntry(fI);

  cout << fEvHeader << endl;
  LOG(info) << "CbmBuildEventsSimple:	Sts digis in slice " << fSlice->GetDataSize(kSts)
            << ". Slice start: " << fSlice->GetStartTime();
  fNDigis = 0;
  for (i = 0; i < 16; i++)
    fNStsDigis[i] = 0;
  fSN   = 0;
  fST   = -1111;
  fISts = 0;
  return fI;
}

Bool_t CbmEvBuildSource::Init()
{
  FairRootManager* mgr = FairRootManager::Instance();

  fCh->SetBranchAddress("TimeSlice.", &(fSlice));
  fCh->SetBranchAddress("EventHeader.", &(fEvHeader));
  fN = fCh->GetEntries();
  fI = -1;

  if (mgr == NULL) {
    LOG(fatal) << "Can't find root manager in the system.";
    return kFALSE;
  }

  fSTSDigi = new TClonesArray("CbmStsDigi", 10000);
  mgr->Register("StsDigi", "Digital response in STS", fSTSDigi, kTRUE);
  fSTSDigi->Delete();
  fNSTSDigis = 0;

  LOG(info) << "CbmEvBuildSource::Init() success";
  return kTRUE;
}

void CbmEvBuildSource::Reset()
{
  cout << "Reset()" << endl;
  //  fI=0;
}

/** Fills Stsdigis array. STS separate, because start and end] end digi is known. **/
void CbmEvBuildSource::FillEvent(Int_t st, Int_t end)
{
  Int_t i;
  CbmStsDigi* digi;

  fNSTSDigis = 0;
  fSTSDigi->Delete();
  fEv++;
  for (i = st; i <= end; i++) {
    digi = (CbmStsDigi*) fSlice->GetData(kSts, i);
    new ((*fSTSDigi)[fNSTSDigis]) CbmStsDigi(*digi);
    fNSTSDigis++;
    //    cout << fSTSDigi->GetEntriesFast() << " " << fNSTSDigis << endl;
  }
  LOG(info) << "CbmEvBuildSource:	Event constructed. Digis used from " << st << " to " << end << ".";
}


void CbmEvBuildSource::AddFile(const char* fname) { fCh->AddFile(fname); }

CbmEvBuildSource::~CbmEvBuildSource()
{
  if (fCh) delete fCh;
  if (fSTSDigi) delete fSTSDigi;
}

void CbmEvBuildSource::FillEventHeader(FairEventHeader* feh)
{
  feh->SetEventTime(fEvHeader->GetEventTime());
  feh->SetRunId(fEvHeader->GetRunId());
  feh->SetMCEntryNumber(fEvHeader->GetMCEntryNumber());
  feh->SetInputFileId(fEvHeader->GetInputFileId());
}

ClassImp(CbmEvBuildSource)
