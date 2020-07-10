/**
* \file CbmRichRingFinderHough.cxx
*
* \author Semen Lebedev
* \date 2008
**/

#include "CbmRichRingFinderHough.h"
#include "CbmRichRingFinderHoughImpl.h"
//#include "CbmRichRingFinderHoughSimd.h"
//#include "../../littrack/utils/CbmLitMemoryManagment.h"
#include "CbmRichHit.h"
#include "CbmRichRing.h"

#include "FairLogger.h"
#include "TClonesArray.h"
#include "TStopwatch.h"

#include <iostream>

using std::cout;
using std::endl;
using std::vector;

CbmRichRingFinderHough::CbmRichRingFinderHough()
  : fNEvent(0), fRingCount(0), fHTImpl(NULL), fUseAnnSelect(true) {
#ifdef HOUGH_SERIAL
  fHTImpl = new CbmRichRingFinderHoughImpl();
#endif

#ifdef HOUGH_SIMD
  fHTImpl = new CbmRichRingFinderHoughSimd();
#endif
}

void CbmRichRingFinderHough::Init() {
  fHTImpl->SetUseAnnSelect(fUseAnnSelect);
  fHTImpl->Init();
}

CbmRichRingFinderHough::~CbmRichRingFinderHough() { delete fHTImpl; }

Int_t CbmRichRingFinderHough::DoFind(TClonesArray* rHitArray,
                                     TClonesArray* /*rProjArray*/,
                                     TClonesArray* rRingArray) {
  TStopwatch timer;
  timer.Start();
  fNEvent++;
  LOG(info) << "-I- CbmRichRingFinderHough  Event/Timeslice no. " << fNEvent;

  vector<CbmRichHoughHit> UpH;
  vector<CbmRichHoughHit> DownH;
  fRingCount = 0;

  if (NULL == rHitArray) {
    LOG(error) << "-E- CbmRichRingFinderHough::DoFind: Hit array missing!"
               << rHitArray;
    return -1;
  }
  const Int_t nhits = rHitArray->GetEntriesFast();
  if (nhits <= 0) {
    LOG(error) << "-E- CbmRichRingFinderHough::DoFind: No hits in this event!";
    return -1;
  }

  UpH.reserve(nhits / 2);
  DownH.reserve(nhits / 2);

  // convert CbmRichHit to CbmRichHoughHit and
  // sort hits according to the photodetector (up or down)
  for (Int_t iHit = 0; iHit < nhits; iHit++) {
    CbmRichHit* hit = static_cast<CbmRichHit*>(rHitArray->At(iHit));
    if (hit) {
      CbmRichHoughHit tempPoint;
      tempPoint.fHit.fX  = hit->GetX();
      tempPoint.fHit.fY  = hit->GetY();
      tempPoint.fHit.fId = iHit;
      tempPoint.fX2plusY2 =
        hit->GetX() * hit->GetX() + hit->GetY() * hit->GetY();
      tempPoint.fTime   = hit->GetTime();
      tempPoint.fIsUsed = false;
      if (hit->GetY() >= 0)
        UpH.push_back(tempPoint);
      else
        DownH.push_back(tempPoint);
    }
  }

  timer.Stop();
  Double_t dt1 = timer.RealTime();

  timer.Start();
  LOG(debug) << "-I- CbmRichRingFinderHough nofHits Up:" << UpH.size();

  fHTImpl->SetData(UpH);
  fHTImpl->DoFind();
  if (rRingArray != NULL)
    AddRingsToOutputArray(rRingArray, rHitArray, fHTImpl->GetFoundRings());
  //for_each(UpH.begin(), UpH.end(), DeleteObject());
  UpH.clear();

  timer.Stop();
  Double_t dt2 = timer.RealTime();

  timer.Start();
  LOG(debug) << "-I- CbmRichRingFinderHough nofHits Down:" << DownH.size();
  fHTImpl->SetData(DownH);
  fHTImpl->DoFind();
  if (rRingArray != NULL)
    AddRingsToOutputArray(rRingArray, rHitArray, fHTImpl->GetFoundRings());
  //for_each(DownH.begin(), DownH.end(), DeleteObject());
  DownH.clear();
  timer.Stop();
  Double_t dt3 = timer.RealTime();

  LOG(info) << "CbmRichRingFinderHough. Number of found rings "
            << rRingArray->GetEntriesFast();

  LOG(info) << "time1:" << dt1 << " time2:" << dt2 << " time3:" << dt3
            << " total:" << dt1 + dt2 + dt3;

  return 1;
}

void CbmRichRingFinderHough::AddRingsToOutputArray(
  TClonesArray* rRingArray,
  TClonesArray* rHitArray,
  const vector<CbmRichRingLight*>& rings) {
  for (UInt_t iRing = 0; iRing < rings.size(); iRing++) {
    if (rings[iRing]->GetRecFlag() == -1) continue;
    CbmRichRing* r    = new CbmRichRing();
    double ringTime   = 0.;
    Int_t ringCounter = 0;

    for (Int_t iH = 0; iH < rings[iRing]->GetNofHits(); iH++) {
      Int_t hitId = rings[iRing]->GetHitId(iH);
      r->AddHit(hitId);
      CbmRichHit* hit = static_cast<CbmRichHit*>(rHitArray->At(hitId));
      if (hit != nullptr) {
        ringCounter++;
        ringTime += hit->GetTime();
      }
    }
    r->SetTime(ringTime / (double) ringCounter);
    new ((*rRingArray)[fRingCount]) CbmRichRing(*r);
    fRingCount++;
  }
}
