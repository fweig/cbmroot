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
#include "CbmEvent.h"
#include "CbmRichHit.h"
#include "CbmRichRing.h"

#include <Logger.h>

#include "TClonesArray.h"
#include "TStopwatch.h"

#include <iostream>

using std::cout;
using std::endl;
using std::vector;

CbmRichRingFinderHough::CbmRichRingFinderHough()
{
#ifdef HOUGH_SERIAL
  fHTImpl = new CbmRichRingFinderHoughImpl();
#endif

#ifdef HOUGH_SIMD
  fHTImpl = new CbmRichRingFinderHoughSimd();
#endif
}

void CbmRichRingFinderHough::Init()
{
  fHTImpl->SetUseAnnSelect(fUseAnnSelect);
  fHTImpl->Init();
}

CbmRichRingFinderHough::~CbmRichRingFinderHough() { delete fHTImpl; }

Int_t CbmRichRingFinderHough::DoFind(CbmEvent* event, TClonesArray* rHitArray, TClonesArray* /*rProjArray*/,
                                     TClonesArray* rRingArray)
{
  TStopwatch timer;
  timer.Start();
  fEventNum++;

  vector<CbmRichHoughHit> UpH;
  vector<CbmRichHoughHit> DownH;

  if (rHitArray == nullptr) {
    LOG(error) << "CbmRichRingFinderHough::DoFind(): Hit array is nullptr.";
    return -1;
  }

  const Int_t nofRichHits = event ? event->GetNofData(ECbmDataType::kRichHit) : rHitArray->GetEntriesFast();
  if (nofRichHits <= 0) {
    LOG(debug) << "CbmRichRingFinderHough::DoFind(): No RICH hits in this event.";
    return -1;
  }

  UpH.reserve(nofRichHits / 2);
  DownH.reserve(nofRichHits / 2);

  // convert CbmRichHit to CbmRichHoughHit and
  // sort hits according to the photodetector (up or down)
  for (Int_t iH0 = 0; iH0 < nofRichHits; iH0++) {
    Int_t iH        = event ? event->GetIndex(ECbmDataType::kRichHit, iH0) : iH0;
    CbmRichHit* hit = static_cast<CbmRichHit*>(rHitArray->At(iH));
    if (hit != nullptr) {
      CbmRichHoughHit tempPoint;
      tempPoint.fHit.fX   = hit->GetX();
      tempPoint.fHit.fY   = hit->GetY();
      tempPoint.fHit.fId  = iH;
      tempPoint.fX2plusY2 = hit->GetX() * hit->GetX() + hit->GetY() * hit->GetY();
      tempPoint.fTime     = hit->GetTime();
      tempPoint.fIsUsed   = false;
      if (hit->GetY() >= 0) UpH.push_back(tempPoint);
      else
        DownH.push_back(tempPoint);
    }
  }

  timer.Stop();
  Double_t dt1 = timer.RealTime();

  timer.Start();

  fHTImpl->SetData(UpH);
  fHTImpl->DoFind();
  if (rRingArray != nullptr) AddRingsToOutputArray(event, rRingArray, rHitArray, fHTImpl->GetFoundRings());
  //for_each(UpH.begin(), UpH.end(), DeleteObject());
  UpH.clear();

  timer.Stop();
  Double_t dt2 = timer.RealTime();

  timer.Start();
  fHTImpl->SetData(DownH);
  fHTImpl->DoFind();
  if (rRingArray != nullptr) AddRingsToOutputArray(event, rRingArray, rHitArray, fHTImpl->GetFoundRings());
  //for_each(DownH.begin(), DownH.end(), DeleteObject());
  DownH.clear();
  timer.Stop();
  Double_t dt3 = timer.RealTime();

  int nofFoundRings = event ? event->GetNofData(ECbmDataType::kRichRing) : rRingArray->GetEntriesFast();
  LOG(info) << "CbmRichRingFinderHough::DoFind(): Event:" << fEventNum << " hits:" << nofRichHits
            << " rings:" << nofFoundRings << " ringsInTS:" << rRingArray->GetEntriesFast()
            << " Time:" << dt1 + dt2 + dt3;

  return 1;
}

void CbmRichRingFinderHough::AddRingsToOutputArray(CbmEvent* event, TClonesArray* rRingArray, TClonesArray* rHitArray,
                                                   const vector<CbmRichRingLight*>& rings)
{

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
    int nofRings = rRingArray->GetEntriesFast();
    new ((*rRingArray)[nofRings]) CbmRichRing(*r);
    if (event != nullptr) event->AddData(ECbmDataType::kRichRing, nofRings);
  }
}
