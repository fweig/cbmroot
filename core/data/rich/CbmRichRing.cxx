/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Semen Lebedev, Denis Bertini [committer] */

/* $Id: CbmRichRing.cxx,v 1.8 2006/09/13 14:51:28 hoehne Exp $*/

/* History of CVS commits:
 *
 * $Log: CbmRichRing.cxx,v $
 * Revision 1.8  2006/09/13 14:51:28  hoehne
 * two variables (Selection2D, SelectionNN) added in which values between 0 and 1 are stored for fake rejection
 * ReconstructedFlag removed
 *
 * Revision 1.7  2006/08/11 14:03:57  hoehne
 * move SetUncertainty and GetUncertainty to SetChi2 and GetChi2
 *
 * Revision 1.6  2006/07/12 06:27:54  hoehne
 * new functions: SetDistance and GetDistance added which set/ give the distance between ring center and track attached to this
 * ring
 *
 * Revision 1.5  2006/02/23 11:24:10  hoehne
 * RecFlag added (Simeon Lebedev)
 *
 * Revision 1.4  2006/01/23 11:40:13  hoehne
 * MCMotherID added
 *
 * Revision 1.3  2006/01/19 10:40:06  hoehne
 * restructured according to new RinfFinder Class:
 * array of hits belonging to a certain ring added
 *
 *
 *
 */


// -------------------------------------------------------------------------
// -----                      CbmRichRing source file                  -----
// -----   Created 05/07/04 by A. Soloviev <solovjev@cv.jinr.ru>       -----
// -------------------------------------------------------------------------

#include "CbmRichRing.h"

#include <Logger.h>  // for Logger, LOG

#include <TObject.h>  // for TObject

#include <cmath>  // for fabs, sqrt, atan, cos, sin

using std::atan;
using std::fabs;
using std::sqrt;

// -----   Default constructor   -------------------------------------------
CbmRichRing::CbmRichRing()
  : TObject()
  , fHitCollection()
  , fAPar(0.)
  , fBPar(0.)
  , fCPar(0.)
  , fDPar(0.)
  , fEPar(0.)
  , fFPar(0.)
  , fCenterX(0.)
  , fCenterY(0.)
  , fRadius(0.)
  , fAaxis(0.)
  , fBaxis(0.)
  , fAaxisCor(0.)
  , fBaxisCor(0.)
  , fPhi(0.)
  , fChi2(0.)
  , fAngle(0.)
  , fNofHitsOnRing(-1)
  , fSelectionNN(-1.)
  , fRecFlag(0)
  , fTime(0.)
{
  fHitCollection.reserve(40);
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmRichRing::CbmRichRing(float x, float y, float r)
  : TObject()
  , fHitCollection()
  , fAPar(0.)
  , fBPar(0.)
  , fCPar(0.)
  , fDPar(0.)
  , fEPar(0.)
  , fFPar(0.)
  , fCenterX(x)
  , fCenterY(y)
  , fRadius(r)
  , fAaxis(0.)
  , fBaxis(0.)
  , fAaxisCor(0.)
  , fBaxisCor(0.)
  , fPhi(0.)
  , fChi2(0.)
  , fAngle(0.)
  , fNofHitsOnRing(-1)
  , fSelectionNN(-1.)
  , fRecFlag(0)
  , fTime(0.)
{
  fHitCollection.reserve(40);
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmRichRing::~CbmRichRing() { fHitCollection.clear(); }
// -------------------------------------------------------------------------

void CbmRichRing::SetXYABPhi(double x, double y, double a, double b, double phi)
{
  fCenterX = x;
  fCenterY = y;
  fAaxis   = a;
  fBaxis   = b;
  fPhi     = phi;
}

bool CbmRichRing::RemoveHit(uint32_t hitId)
{
  //int32_t nofHits = fHitCollection.size();
  std::vector<uint32_t>::iterator it;
  for (it = fHitCollection.begin(); it != fHitCollection.end(); it++) {
    if (hitId == *it) {
      fHitCollection.erase(it);
      return true;
    }
  }
  return false;
}

double CbmRichRing::GetXF1() const
{
  double c  = sqrt(fAaxis * fAaxis - fBaxis * fBaxis);
  double xc = c * cos(fabs(fPhi));

  return fCenterX + xc;
}

double CbmRichRing::GetYF1() const
{
  double c  = sqrt(fAaxis * fAaxis - fBaxis * fBaxis);
  double yc = c * sin(fabs(fPhi));
  if (fPhi >= 0) { return fCenterY + yc; }
  else {
    return fCenterY - yc;
  }
}

double CbmRichRing::GetXF2() const
{
  double c  = sqrt(fAaxis * fAaxis - fBaxis * fBaxis);
  double xc = c * cos(fabs(fPhi));

  return fCenterX - xc;
}

double CbmRichRing::GetYF2() const
{
  double c  = sqrt(fAaxis * fAaxis - fBaxis * fBaxis);
  double yc = c * sin(fabs(fPhi));
  if (fPhi >= 0) { return fCenterY - yc; }
  else {
    return fCenterY + yc;
  }
}

void CbmRichRing::Print(Option_t*) const
{
  LOG(info) << " Ring parameters: "
            << " Aaxis = " << GetAaxis() << ", Baxis = " << GetBaxis() << ", Phi = " << GetPhi()
            << ", CenterX = " << GetCenterX() << ", CenterY = " << GetCenterY() << ", Radius = " << GetRadius()
            << ", NofHits = " << GetNofHits() << ", RadialPosition = " << GetRadialPosition()
            << ", Chi2 = " << GetChi2() << ", Angle() = " << GetAngle() << ", NofHitsOnRing = " << GetNofHitsOnRing();
}

float CbmRichRing::GetRadialPosition() const
{
  if (fCenterY > 0.f) { return sqrt(fCenterX * fCenterX + (fCenterY - 110.f) * (fCenterY - 110.f)); }
  else {
    return sqrt(fCenterX * fCenterX + (fCenterY + 110.f) * (fCenterY + 110.f));
  }
}

double CbmRichRing::GetRadialAngle() const
{
  /*  if (fCenterY > 0){
            return  atan((100 - fCenterY) / (0 - fCenterX));
    } else {
            return atan((-100 - fCenterY) / (0 - fCenterX));
    }*/

  if (fCenterX > 0 && fCenterY > 0) { return atan(fabs((100 - fCenterY) / (0 - fCenterX))); }
  if (fCenterX < 0 && fCenterY > 0) { return M_PI - atan(fabs((100 - fCenterY) / (0 - fCenterX))); }
  if (fCenterX < 0 && fCenterY < 0) { return M_PI + atan(fabs((-100 - fCenterY) / (0 - fCenterX))); }
  if (fCenterX > 0 && fCenterY < 0) { return 2 * M_PI - atan(fabs((-100 - fCenterY) / (0 - fCenterX))); }

  return 999.;
}
//
//CbmRichRingLight* CbmRichRing::toLightRing()
//{
//	CbmRichRingLight* rl = new CbmRichRingLight();
//
//	for (int i = 0; i < this->GetNofHits(); i ++){
//		rl->AddHit(this->GetHit(i));
//	}
//	rl->SetCenterX(this->GetCenterX());
//	rl->SetCenterY(this->GetCenterY());
//	rl->SetRadius(this->GetRadius());
//	rl->SetAngle(this->GetAngle());
//	rl->SetChi2(this->GetChi2());
//	rl->SetNofHitsOnRing(this->GetNofHitsOnRing());
//	rl->SetSelectionNN(this->GetSelectionNN());
//	rl->SetRecFlag(this->GetRecFlag());
//
//	return rl;
//}

ClassImp(CbmRichRing)
