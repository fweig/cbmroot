#include "utils/CbmLitMath.h"

#include "data/CbmLitTrackParam.h"
#include "data/CbmLitHit.h"
#include "data/CbmLitStripHit.h"
#include "data/CbmLitPixelHit.h"
#include "data/CbmLitTrack.h"

#include <iostream>
#include <cmath>

namespace lit
{

litfloat ChiSq(
   const CbmLitTrackParam* par,
   const CbmLitHit* hit)
{
   litfloat chisq = 0.;
   if (hit->GetType() == kLITSTRIPHIT) {
      chisq = ChiSq(par, static_cast<const CbmLitStripHit*>(hit));
   } else if (hit->GetType() == kLITPIXELHIT) {
      chisq = ChiSq(par, static_cast<const CbmLitPixelHit*>(hit));
   }
   return chisq;
}

litfloat ChiSq(
   const CbmLitTrackParam* par,
   const CbmLitStripHit* hit)
{
   litfloat duu = hit->GetDu() * hit->GetDu();
   litfloat phiCos = hit->GetCosPhi();
   litfloat phiSin = hit->GetSinPhi();
   litfloat phiCosSq = phiCos * phiCos;
   litfloat phiSinSq = phiSin * phiSin;
   litfloat phi2SinCos = 2 * phiCos * phiSin;
   litfloat C0 = par->GetCovariance(0);
   litfloat C1 = par->GetCovariance(1);
   litfloat C5 = par->GetCovariance(5);

   litfloat ru = hit->GetU() - par->GetX() * phiCos - par->GetY() * phiSin;

   return (ru * ru) / (duu - phiCosSq*C0 - phi2SinCos*C1 - phiSinSq*C5);
}

litfloat ChiSq(
   const CbmLitTrackParam* par,
   const CbmLitPixelHit* hit)
{
   litfloat dxx = hit->GetDx() * hit->GetDx();
   litfloat dxy = hit->GetDxy();
   litfloat dyy = hit->GetDy() * hit->GetDy();
   litfloat dtt = hit->GetDt() * hit->GetDt();
   litfloat xmx = hit->GetX() - par->GetX();
   litfloat ymy = hit->GetY() - par->GetY();
   litfloat tmt = hit->GetT() - par->GetTime();
   litfloat C0 = par->GetCovariance(0);
   litfloat C1 = par->GetCovariance(1);
   litfloat C5 = par->GetCovariance(5);
   litfloat C6 = par->GetCovariance(6);
   litfloat C10 = par->GetCovariance(10);
   litfloat C20 = par->GetCovariance(20);

   /*litfloat norm = dxx * dyy - dxx * C5 - dyy * C0 + C0 * C5
              - dxy * dxy + 2 * dxy * C1 - C1 * C1;
   if (norm == 0.) { norm = 1e-10; }
   return ((xmx * (dyy - C5) - ymy * (dxy - C1)) * xmx
           +(-xmx * (dxy - C1) + ymy * (dxx - C0)) * ymy) / norm;*/
   litfloat norm = (dxx - C0) * ((dyy - C6) * (dtt - C20) - C10 * C10) +
      (dxy - C1) * (C5 * C10 - (dxy - C1) * (dtt - C20)) +
      C5 * ((dxy - C1) * C10 - (dyy - C6) * C5);
   
   if (norm == 0.) { norm = 1e-10; }
   
   // Mij is the (symmetric) inverse of the residual matrix
   litfloat M00 = ((dyy - C6) * (dtt - C20) - C10 * C10) / norm;
   litfloat M01 = ((dxy - C1) * (dtt - C20) - C5 * C10) / norm;
   litfloat M02 = ((dxy - C1) * C10 - (dyy - C6) * C5) / norm;
   litfloat M11 = ((dxx - C0) * (dtt - C20) - C5 * C5) / norm;
   litfloat M12 = ((dxx - C0) * C10 - (dxy - C1) * C5) / norm;
   litfloat M22 = ((dxx - C0) * (dyy - C6) - (dxy - C1) * (dxy - C1)) / norm;
   
   return xmx * (xmx * M00 + ymy * M01 + tmt * M02) + ymy * (xmx * M01 + ymy * M11 + tmt * M12) + tmt * (xmx * M02 + ymy * M12 + tmt * M22);
}

Int_t NDF(
   const CbmLitTrack* track)
{
   Int_t ndf = 0;
   for (Int_t i = 0; i < track->GetNofHits(); i++) {
      if (track->GetHit(i)->GetType() == kLITPIXELHIT) { ndf += 2; }
      else if (track->GetHit(i)->GetType() == kLITSTRIPHIT) { ndf++; }
   }
   ndf -= 5;
   if (ndf > 0) { return ndf; }
   else { return 1; }
}

}

