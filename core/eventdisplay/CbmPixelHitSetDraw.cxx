#include "CbmPixelHitSetDraw.h"

#include "CbmPixelHit.h"        // for CbmPixelHit

#include <FairLogger.h>         // for LOG, Logger

#include <Rtypes.h>             // for ClassImp
#include <TVector3.h>           // for TVector3

CbmPixelHitSetDraw::CbmPixelHitSetDraw()
{
}

CbmPixelHitSetDraw::~CbmPixelHitSetDraw()
{
}

TVector3 CbmPixelHitSetDraw::GetVector(TObject* obj)
{
	CbmPixelHit* p = (CbmPixelHit*)obj;
	LOG(debug) << "-I- CbmPixelHitSetDraw::GetVector: " << p->GetX() << " " << p->GetY() << " " << p->GetZ();
	return TVector3(p->GetX(), p->GetY(), p->GetZ());
}


ClassImp(CbmPixelHitSetDraw)
