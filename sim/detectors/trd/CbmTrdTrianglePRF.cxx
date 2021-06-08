/* Copyright (C) 2017-2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alex Bercuci, Florian Uhlig [committer] */

/**
 * \file CbmTrdTrianglePRF.cxx
 * \author Alex Bercuci
 * \date 10/04/2017
 **/


#include "CbmTrdTrianglePRF.h"

#include <TF1.h>
#include <TMath.h>

using std::cout;
using std::endl;
using std::flush;

//___________________________________________________________________________
CbmTrdTrianglePRF::CbmTrdTrianglePRF(Float_t W, Float_t H, Int_t n)
  : TObject()
  , fN(0)
  , fBinx(0)
  , fBinx0(0)
  , fBiny(0)
  , fBiny0(0)
  , fX0(0.)
  , fY0(0.)
  , fW(W)
  , fH(H)
  , fdW(W / n / 2.)
  , fdH(H / n / 2.)
  , fSlope(H / W)
  , fNorm(0.)
  , fUp()
  , fX()
  , fY()
  , fPRFx(NULL)
  , fPRFy(NULL)
{
  /**
 *  Build the map for triangular pad integration. The dimension of the map is given by the no of adjacent columns/rows considered in the map (default 5 columns and 3 rows)
*/
  fN = n * (2 * NR + 1);
  Double_t epsilon(1.e-3 * W), dw(2 * fdW), dh(2 * fdH);

  Double_t bx0((-NC - 0.5) * W), by0, xv, yv;
  for (Int_t jcol(-NC); jcol <= NC; jcol++) {
    for (Int_t jx(0); jx < n; jx++, bx0 += dw) {
      // compute x bin center
      xv = bx0 + fdW;

      by0 = (-NR - 0.5) * H;
      for (Int_t jrow(-NR); jrow <= NR; jrow++) {
        for (Int_t jy(0); jy < n; jy++, by0 += dh) {
          // compute y bin center
          yv = by0 + fdH;
          fX.push_back(xv);
          fY.push_back(yv);

          // compute pad type
          Int_t up1 = GetSide(bx0 - jcol * W + dw - epsilon, by0 - jrow * H + epsilon),
                up2 = GetSide(bx0 - jcol * W + epsilon, by0 - jrow * H + dh - epsilon);
          //printf("v(%5.2f %5.2f) lu(%5.2f %5.2f)[%d] rl(%5.2f %5.2f)[%d]\n", xv, yv, bx0-jcol*W, by0-jrow*H+fdH, up2, bx0-jcol*W+fdW, by0-jrow*H, up1);
          if (up1 != up2) fUp.push_back(0);
          else
            fUp.push_back(up1);
        }
      }
    }
  }

  // build the PRF model
  fPRFx = new TF1("prfx", "gaus", -(NC + .5) * fW, (NC + .5) * fW);
  fPRFx->SetParameters(1, 0., 0.46 * fW);  // TODO should be loaded from DB
  fPRFy = new TF1("prfy", "gaus", -(NR + .5) * fH, (NR + .5) * fH);
  fPRFy->SetParameters(1, 0., 0.46 * fW);  // TODO should be loaded from DB
  fNorm = fPRFx->Integral(-(NC + .5) * fW, (NC + .5) * fW) * fPRFy->Integral(-(NR + .5) * fH, (NR + .5) * fH);
}

//___________________________________________________________________________
CbmTrdTrianglePRF::~CbmTrdTrianglePRF()
{
  if (fPRFy) delete fPRFy;
  if (fPRFx) delete fPRFx;
}

//_________________________________________________________
Bool_t CbmTrdTrianglePRF::GetBin(Double_t x, Double_t y, Int_t& binx, Int_t& biny) const
{
  /**  
 * Function looks for the bin containing the point (x,y). The function is optimized for points in the center column/pad of the map. 
 */

  // look to the right
  Int_t nbinx(fX.size() / fN / 2);
  binx = -1;
  for (Int_t ix(nbinx); ix < 2 * nbinx; ix++) {
    if (x < fX[ix * fN] - fdW) break;
    if (x > fX[ix * fN] + fdW) continue;

    if (TMath::Abs(fX[ix * fN] - x) <= fdW) {
      binx = ix;
      break;
    }
  }
  if (binx < 0) {  // if not found look to the left
    for (Int_t ix(nbinx); ix--;) {
      if (x > fX[ix * fN] + fdW) break;
      if (x < fX[ix * fN] - fdW) continue;

      if (TMath::Abs(fX[ix * fN] - x) <= fdW) {
        binx = ix;
        break;
      }
    }
  }
  if (binx < 0) return kFALSE;

  // look upward
  Int_t nbiny(fN / 2);
  biny = -1;
  for (Int_t iy(nbiny); iy < fN; iy++) {
    if (y < fY[iy] - fdH) break;
    if (y > fY[iy] + fdH) continue;

    if (TMath::Abs(fY[iy] - y) <= fdH) {
      biny = iy;
      break;
    }
  }
  if (biny < 0) {  // if not found look to downwards
    for (Int_t iy(nbiny); iy--;) {
      if (y > fY[iy] + fdH) break;
      if (y < fY[iy] - fdH) continue;

      if (TMath::Abs(fY[iy] - y) <= fdH) {
        biny = iy;
        break;
      }
    }
  }
  if (biny < 0) return kFALSE;

  return kTRUE;
}

//_________________________________________________________
Double_t CbmTrdTrianglePRF::GetChargeFraction() const
{
  /**
 * Compute charge fraction on the current bin
 */

  Int_t bin(fBinx * fN + fBiny) /*, bin0(fBinx0*fN+fBiny0)*/;
  return fPRFx->Eval(fX[bin] - fX0) * fPRFy->Eval(fY[bin] - fY0) * 4 * fdW * fdH;
}

//_________________________________________________________
void CbmTrdTrianglePRF::GetCurrentPad(Int_t& col, Int_t& row, Int_t& u) const
{
  /**
 * Find the column/row for the current bin. The up parameter describe the pad holding the bin:\n
 *  -1 bottom pad\n
 *  0 on the split bin\n
 *  1 up pad\n
 */

  Int_t bin(fBinx * fN + fBiny);
  col = TMath::Nint(fX[bin] / fW) + NC;
  row = TMath::Nint(fY[bin] / fH) + NR;
  u   = fUp[bin];
}

//_________________________________________________________
Int_t CbmTrdTrianglePRF::GetSide(const Float_t x, const Float_t y) const
{
  /**
 *  Define the type of triangular pad for the current bin defined by the position in the local column coordinates.\n
 * Return 1 for the upper pad and -1 for the bottom
 */

  if (x < -0.5 * fW || x > 0.5 * fW) {
    printf("x[%f] outside range\n", x);
    return 0;
  }

  if (y < -0.5 * fH || y > 0.5 * fH) {
    printf("y[%f] outside range\n", y);
    return 0;
  }

  if (y > fSlope * x) return 1;  // up
  return -1;                     // down
}

//_________________________________________________________
Bool_t CbmTrdTrianglePRF::NextBinX()
{
  /**
 * Move current bin to the right. Check we are still in the allocated map
 */
  if ((fBinx + 1) * fN >= static_cast<Int_t>(fX.size())) return kFALSE;
  fBinx++;
  return kTRUE;
}

//_________________________________________________________
Bool_t CbmTrdTrianglePRF::NextBinY()
{
  /**
 * Move current bin upwards. Check we are still in the allocated map
 */
  if (fBiny + 1 >= fN) return kFALSE;
  fBiny++;
  return kTRUE;
}

//_________________________________________________________
void CbmTrdTrianglePRF::Print(Option_t* opt) const
{
  printf("N=%d dw=%f dh=%f Slope=%f\n", fN, fdW, fdH, fSlope);
  if (strcmp(opt, "all") != 0) return;
  for (UInt_t i(0); i < fX.size(); i++) {
    if (i && i % fN == 0) printf("\n\n");
    printf("%2d(%5.2f %5.2f) ", fUp[i], fX[i], fY[i]);
  }
  printf("\n");
}

//_________________________________________________________
Bool_t CbmTrdTrianglePRF::PrevBinX()
{
  /**
 * Move current bin to the left. Check we are still in the allocated map
 */
  if ((fBinx - 1) * fN < 0) return kFALSE;
  fBinx--;
  return kTRUE;
}

//_________________________________________________________
Bool_t CbmTrdTrianglePRF::PrevBinY()
{
  /**
 * Move current bin downwards. Check we are still in the allocated map
 */
  if (fBiny - 1 < 0) return kFALSE;
  fBiny--;
  return kTRUE;
}

//_________________________________________________________
Bool_t CbmTrdTrianglePRF::SetOrigin(Double_t x, Double_t y)
{
  Bool_t ret;
  if ((ret = GetBin(x, y, fBinx0, fBiny0))) {
    fBinx = fBinx0;
    fBiny = fBiny0;
    fX0   = x;
    fY0   = y;
  }
  return ret;
}

ClassImp(CbmTrdTrianglePRF)
