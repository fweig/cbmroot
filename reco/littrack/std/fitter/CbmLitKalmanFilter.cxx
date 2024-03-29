/* Copyright (C) 2007-2017 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Timur Ablyazimov */

/** CbmLitKalmanFilter.cxx
 *@author A.Lebedev <alebedev@jinr.ru>
 *@since 2007
 **/

#include "fitter/CbmLitKalmanFilter.h"

#include "data/CbmLitPixelHit.h"
#include "data/CbmLitStripHit.h"
#include "data/CbmLitTrackParam.h"
#include "utils/CbmLitMatrixMath.h"

#include "TMath.h"
#include "TMatrixD.h"

#include <iostream>

#include <cmath>

litfloat CbmLitTrackParam::fSpeedOfLight = 1.e-7 * TMath::C();

CbmLitKalmanFilter::CbmLitKalmanFilter() {}

CbmLitKalmanFilter::~CbmLitKalmanFilter() {}

LitStatus CbmLitKalmanFilter::Update(const CbmLitTrackParam* parIn, CbmLitTrackParam* parOut, const CbmLitHit* hit,
                                     litfloat& chiSq)
{
  *parOut = *parIn;
  return Update(parOut, hit, chiSq);
}

LitStatus CbmLitKalmanFilter::Update(CbmLitTrackParam* par, const CbmLitHit* hit, litfloat& chiSq)
{
  LitStatus result = kLITSUCCESS;
  if (hit->GetType() == kLITSTRIPHIT) { result = Update(par, static_cast<const CbmLitStripHit*>(hit), chiSq); }
  else if (hit->GetType() == kLITPIXELHIT) {
    result = Update(par, static_cast<const CbmLitPixelHit*>(hit), chiSq);
  }
  return result;
}

LitStatus CbmLitKalmanFilter::Update(CbmLitTrackParam* par, const CbmLitPixelHit* hit, litfloat& chiSq)
{
  std::vector<litfloat> cIn = par->GetCovMatrix();

  static const litfloat ONE = 1., TWO = 2.;

  litfloat dxx = hit->GetDx() * hit->GetDx();
  litfloat dxy = hit->GetDxy();
  litfloat dyy = hit->GetDy() * hit->GetDy();
  litfloat dtt = hit->GetDt() * hit->GetDt();

  // calculate residuals
  litfloat dx = hit->GetX() - par->GetX();
  litfloat dy = hit->GetY() - par->GetY();
  litfloat dt = hit->GetT() - par->GetTime();

  // Calculate and inverse residual covariance matrix
  //litfloat t = ONE / (dxx * dyy + dxx * cIn[5] + dyy * cIn[0] + cIn[0] * cIn[5] -
  //dxy * dxy - TWO * dxy * cIn[1] - cIn[1] * cIn[1]);
  litfloat t = ONE
               / ((cIn[0] + dxx) * ((cIn[6] + dyy) * (cIn[20] + dtt) - cIn[10] * cIn[10])
                  - (cIn[1] + dxy) * ((cIn[1] + dxy) * (cIn[20] + dtt) - cIn[5] * cIn[10])
                  + cIn[5] * ((cIn[1] + dxy) * cIn[10] - (cIn[6] + dyy) * cIn[5]));
  //litfloat R00 = (dyy + cIn[5]) * t;
  //litfloat R01 = -(dxy + cIn[1]) * t;
  //litfloat R11 = (dxx + cIn[0]) * t;
  litfloat R00 = ((cIn[6] + dyy) * (cIn[20] + dtt) - cIn[10] * cIn[10]) * t;
  litfloat R01 = (cIn[5] * cIn[10] - (cIn[1] + dxy) * (cIn[20] + dtt)) * t;
  litfloat R02 = ((cIn[1] + dxy) * cIn[10] - (cIn[6] + dyy) * cIn[5]) * t;
  litfloat R11 = ((cIn[0] + dxx) * (cIn[20] + dtt) - cIn[5] * cIn[5]) * t;
  litfloat R12 = ((cIn[1] + dxy) * cIn[5] - (cIn[0] + dxx) * cIn[10]) * t;
  litfloat R22 = ((cIn[0] + dxx) * (cIn[6] + dyy) - (cIn[1] + dxy) * (cIn[1] + dxy)) * t;

  /*TMatrixD ResM(3, 3);
   ResM(0, 0) = dxx + cIn[0];
   ResM(0, 1) = dxy + cIn[1];
   ResM(0, 2) = cIn[5];
   ResM(1, 0) = dxy + cIn[1];
   ResM(1, 1) = dyy + cIn[6];
   ResM(1, 2) = cIn[10];
   ResM(2, 0) = cIn[5];
   ResM(2, 1) = cIn[10];
   ResM(2, 2) = dtt + cIn[20];
   ResM.Invert();
   
   litfloat R00 = ResM(0, 0);
   litfloat R01 = ResM(0, 1);
   litfloat R02 = ResM(0, 2);
   litfloat R11 = ResM(1, 1);
   litfloat R12 = ResM(1, 2);
   litfloat R22 = ResM(2, 2);*/

  // Calculate Kalman gain matrix
  litfloat K00 = cIn[0] * R00 + cIn[1] * R01 + cIn[5] * R02;
  litfloat K01 = cIn[0] * R01 + cIn[1] * R11 + cIn[5] * R12;
  litfloat K02 = cIn[0] * R02 + cIn[1] * R12 + cIn[5] * R22;
  litfloat K10 = cIn[1] * R00 + cIn[6] * R01 + cIn[10] * R02;
  litfloat K11 = cIn[1] * R01 + cIn[6] * R11 + cIn[10] * R12;
  litfloat K12 = cIn[1] * R02 + cIn[6] * R12 + cIn[10] * R22;
  litfloat K20 = cIn[2] * R00 + cIn[7] * R01 + cIn[14] * R02;
  litfloat K21 = cIn[2] * R01 + cIn[7] * R11 + cIn[14] * R12;
  litfloat K22 = cIn[2] * R02 + cIn[7] * R12 + cIn[14] * R22;
  litfloat K30 = cIn[3] * R00 + cIn[8] * R01 + cIn[17] * R02;
  litfloat K31 = cIn[3] * R01 + cIn[8] * R11 + cIn[17] * R12;
  litfloat K32 = cIn[3] * R02 + cIn[8] * R12 + cIn[17] * R22;
  litfloat K40 = cIn[4] * R00 + cIn[9] * R01 + cIn[19] * R02;
  litfloat K41 = cIn[4] * R01 + cIn[9] * R11 + cIn[19] * R12;
  litfloat K42 = cIn[4] * R02 + cIn[9] * R12 + cIn[19] * R22;
  litfloat K50 = cIn[5] * R00 + cIn[10] * R01 + cIn[20] * R02;
  litfloat K51 = cIn[5] * R01 + cIn[10] * R11 + cIn[20] * R12;
  litfloat K52 = cIn[5] * R02 + cIn[10] * R12 + cIn[20] * R22;

  // Calculate filtered state vector
  litfloat xOut[6] = {par->GetX(), par->GetY(), par->GetTx(), par->GetTy(), par->GetQp(), par->GetTime()};
  xOut[0] += K00 * dx + K01 * dy + K02 * dt;
  xOut[1] += K10 * dx + K11 * dy + K12 * dt;
  xOut[2] += K20 * dx + K21 * dy + K22 * dt;
  xOut[3] += K30 * dx + K31 * dy + K32 * dt;
  xOut[4] += K40 * dx + K41 * dy + K42 * dt;
  xOut[5] += K50 * dx + K51 * dy + K52 * dt;

  // Calculate filtered covariance matrix
  std::vector<litfloat> cOut = cIn;

  cOut[0] -= K00 * cIn[0] + K01 * cIn[1] + K02 * cIn[5];
  cOut[1] -= K00 * cIn[1] + K01 * cIn[6] + K02 * cIn[10];
  cOut[2] -= K00 * cIn[2] + K01 * cIn[7] + K02 * cIn[14];
  cOut[3] -= K00 * cIn[3] + K01 * cIn[8] + K02 * cIn[17];
  cOut[4] -= K00 * cIn[4] + K01 * cIn[9] + K02 * cIn[19];
  cOut[5] -= K00 * cIn[5] + K01 * cIn[10] + K02 * cIn[20];

  cOut[6] -= K10 * cIn[1] + K11 * cIn[6] + K12 * cIn[10];
  cOut[7] -= K10 * cIn[2] + K11 * cIn[7] + K12 * cIn[14];
  cOut[8] -= K10 * cIn[3] + K11 * cIn[8] + K12 * cIn[17];
  cOut[9] -= K10 * cIn[4] + K11 * cIn[9] + K12 * cIn[19];
  cOut[10] -= K10 * cIn[5] + K11 * cIn[10] + K12 * cIn[20];

  cOut[11] -= K20 * cIn[2] + K21 * cIn[7] + K22 * cIn[14];
  cOut[12] -= K20 * cIn[3] + K21 * cIn[8] + K22 * cIn[17];
  cOut[13] -= K20 * cIn[4] + K21 * cIn[9] + K22 * cIn[19];
  cOut[14] -= K20 * cIn[5] + K21 * cIn[10] + K22 * cIn[20];

  cOut[15] -= K30 * cIn[3] + K31 * cIn[8] + K32 * cIn[17];
  cOut[16] -= K30 * cIn[4] + K31 * cIn[9] + K32 * cIn[19];
  cOut[17] -= K30 * cIn[5] + K31 * cIn[10] + K32 * cIn[20];

  cOut[18] -= K40 * cIn[4] + K41 * cIn[9] + K42 * cIn[19];
  cOut[19] -= K40 * cIn[5] + K41 * cIn[10] + K42 * cIn[20];

  cOut[20] -= K50 * cIn[5] + K51 * cIn[10] + K52 * cIn[20];

  // Copy filtered state to output
  par->SetX(xOut[0]);
  par->SetY(xOut[1]);
  par->SetTx(xOut[2]);
  par->SetTy(xOut[3]);
  par->SetQp(xOut[4]);
  par->SetTime(xOut[5]);
  par->SetCovMatrix(cOut);

  // Calculate chi-square
  litfloat xmx = hit->GetX() - par->GetX();
  litfloat ymy = hit->GetY() - par->GetY();
  litfloat tmt = hit->GetT() - par->GetTime();
  litfloat C0  = cOut[0];
  litfloat C1  = cOut[1];
  litfloat C5  = cOut[6];

  /*litfloat norm = dxx * dyy - dxx * C5 - dyy * C0 + C0 * C5
              - dxy * dxy + 2 * dxy * C1 - C1 * C1;

   chiSq = ((xmx * (dyy - C5) - ymy * (dxy - C1)) * xmx
            +(-xmx * (dxy - C1) + ymy * (dxx - C0)) * ymy) / norm +
      (hit->GetT() - par->GetTime()) * (hit->GetT() - par->GetTime()) / (hit->GetDt() * hit->GetDt() + cOut[20]);*/
  litfloat norm = (dxx - cOut[0]) * ((dyy - cOut[6]) * (dtt - cOut[20]) - cOut[10] * cOut[10])
                  + (dxy - cOut[1]) * (cOut[5] * cOut[10] - (dxy - cOut[1]) * (dtt - cOut[20]))
                  + cOut[5] * ((dxy - cOut[1]) * cOut[10] - (dyy - cOut[6]) * cOut[5]);

  if (norm == 0.) { norm = 1e-10; }

  // Mij is the (symmetric) inverse of the residual matrix
  litfloat M00 = ((dyy - cOut[6]) * (dtt - cOut[20]) - cOut[10] * cOut[10]) / norm;
  litfloat M01 = ((dxy - cOut[1]) * (dtt - cOut[20]) - cOut[5] * cOut[10]) / norm;
  litfloat M02 = ((dxy - cOut[1]) * cOut[10] - (dyy - cOut[6]) * cOut[5]) / norm;
  litfloat M11 = ((dxx - cOut[0]) * (dtt - cOut[20]) - cOut[5] * cOut[5]) / norm;
  litfloat M12 = ((dxx - cOut[0]) * cOut[10] - (dxy - cOut[1]) * cOut[5]) / norm;
  litfloat M22 = ((dxx - cOut[0]) * (dyy - cOut[6]) - (dxy - cOut[1]) * (dxy - cOut[1])) / norm;
  /*TMatrixD Chi2M(3, 3);
   Chi2M(0, 0) = dxx - cOut[0];
   Chi2M(0, 1) = dxy - cOut[1];
   Chi2M(0, 2) = -cOut[5];
   Chi2M(1, 0) = dxy - cOut[1];
   Chi2M(1, 1) = dyy - cOut[6];
   Chi2M(1, 2) = -cOut[10];
   Chi2M(2, 0) = -cOut[5];
   Chi2M(2, 1) = -cOut[10];
   Chi2M(2, 2) = dtt - cOut[20];
   Chi2M.Invert();
   
   litfloat M00 = Chi2M(0, 0);
   litfloat M01 = Chi2M(0, 1);
   litfloat M02 = Chi2M(0, 2);
   litfloat M11 = Chi2M(1, 1);
   litfloat M12 = Chi2M(1, 2);
   litfloat M22 = Chi2M(2, 2);*/

  chiSq = xmx * (xmx * M00 + ymy * M01 + tmt * M02) + ymy * (xmx * M01 + ymy * M11 + tmt * M12)
          + tmt * (xmx * M02 + ymy * M12 + tmt * M22);

  return kLITSUCCESS;
}

LitStatus CbmLitKalmanFilter::UpdateWMF(CbmLitTrackParam* par, const CbmLitPixelHit* hit, litfloat& chiSq)
{
  litfloat xIn[5] = {par->GetX(), par->GetY(), par->GetTx(), par->GetTy(), par->GetQp()};

  std::vector<litfloat> cIn    = par->GetCovMatrix();
  std::vector<litfloat> cInInv = par->GetCovMatrix();

  litfloat dxx = hit->GetDx() * hit->GetDx();
  litfloat dxy = hit->GetDxy();
  litfloat dyy = hit->GetDy() * hit->GetDy();

  // Inverse predicted cov matrix
  InvSym15(cInInv);
  // Calculate C1
  std::vector<litfloat> C1 = cInInv;
  litfloat det             = dxx * dyy - dxy * dxy;
  C1[0] += dyy / det;
  C1[1] += -dxy / det;
  C1[5] += dxx / det;
  // Inverse C1 -> output updated covariance matrix
  InvSym15(C1);

  std::vector<litfloat> t(5);
  t[0] = cInInv[0] * par->GetX() + cInInv[1] * par->GetY() + cInInv[2] * par->GetTx() + cInInv[3] * par->GetTy()
         + cInInv[4] * par->GetQp() + (dyy * hit->GetX() - dxy * hit->GetY()) / det;
  t[1] = cInInv[1] * par->GetX() + cInInv[5] * par->GetY() + cInInv[6] * par->GetTx() + cInInv[7] * par->GetTy()
         + cInInv[8] * par->GetQp() + (-dxy * hit->GetX() + dxx * hit->GetY()) / det;
  t[2] = cInInv[2] * par->GetX() + cInInv[6] * par->GetY() + cInInv[9] * par->GetTx() + cInInv[10] * par->GetTy()
         + cInInv[11] * par->GetQp();
  t[3] = cInInv[3] * par->GetX() + cInInv[7] * par->GetY() + cInInv[10] * par->GetTx() + cInInv[12] * par->GetTy()
         + cInInv[13] * par->GetQp();
  t[4] = cInInv[4] * par->GetX() + cInInv[8] * par->GetY() + cInInv[11] * par->GetTx() + cInInv[13] * par->GetTy()
         + cInInv[14] * par->GetQp();

  std::vector<litfloat> xOut(5);
  Mult15On5(C1, t, xOut);

  // Copy filtered state to output
  par->SetX(xOut[0]);
  par->SetY(xOut[1]);
  par->SetTx(xOut[2]);
  par->SetTy(xOut[3]);
  par->SetQp(xOut[4]);
  par->SetCovMatrix(C1);

  // Calculate chi square
  litfloat dx0 = xOut[0] - xIn[0];
  litfloat dx1 = xOut[1] - xIn[1];
  litfloat dx2 = xOut[2] - xIn[2];
  litfloat dx3 = xOut[3] - xIn[3];
  litfloat dx4 = xOut[4] - xIn[4];
  litfloat xmx = hit->GetX() - par->GetX();
  litfloat ymy = hit->GetY() - par->GetY();
  chiSq        = ((xmx * dyy - ymy * dxy) * xmx + (-xmx * dxy + ymy * dxx) * ymy) / det
          + (dx0 * cInInv[0] + dx1 * cInInv[1] + dx2 * cInInv[2] + dx3 * cInInv[3] + dx4 * cInInv[4]) * dx0
          + (dx0 * cInInv[1] + dx1 * cInInv[5] + dx2 * cInInv[6] + dx3 * cInInv[7] + dx4 * cInInv[8]) * dx1
          + (dx0 * cInInv[2] + dx1 * cInInv[6] + dx2 * cInInv[9] + dx3 * cInInv[10] + dx4 * cInInv[11]) * dx2
          + (dx0 * cInInv[3] + dx1 * cInInv[7] + dx2 * cInInv[10] + dx3 * cInInv[12] + dx4 * cInInv[13]) * dx3
          + (dx0 * cInInv[4] + dx1 * cInInv[8] + dx2 * cInInv[11] + dx3 * cInInv[13] + dx4 * cInInv[14]) * dx4;

  return kLITSUCCESS;
}

LitStatus CbmLitKalmanFilter::Update(CbmLitTrackParam* par, const CbmLitStripHit* hit, litfloat& chiSq)
{
  litfloat xIn[5]           = {par->GetX(), par->GetY(), par->GetTx(), par->GetTy(), par->GetQp()};
  std::vector<litfloat> cIn = par->GetCovMatrix();

  litfloat u          = hit->GetU();
  litfloat duu        = hit->GetDu() * hit->GetDu();
  litfloat phiCos     = hit->GetCosPhi();
  litfloat phiSin     = hit->GetSinPhi();
  litfloat phiCosSq   = phiCos * phiCos;
  litfloat phiSinSq   = phiSin * phiSin;
  litfloat phi2SinCos = 2 * phiCos * phiSin;

  // Inverted covariance matrix of predicted residual
  litfloat R = 1. / (duu + cIn[0] * phiCosSq + phi2SinCos * cIn[1] + cIn[5] * phiSinSq);

  // Calculate Kalman gain matrix
  litfloat K0 = cIn[0] * phiCos + cIn[1] * phiSin;
  litfloat K1 = cIn[1] * phiCos + cIn[5] * phiSin;
  litfloat K2 = cIn[2] * phiCos + cIn[6] * phiSin;
  litfloat K3 = cIn[3] * phiCos + cIn[7] * phiSin;
  litfloat K4 = cIn[4] * phiCos + cIn[8] * phiSin;

  litfloat KR0 = K0 * R;
  litfloat KR1 = K1 * R;
  litfloat KR2 = K2 * R;
  litfloat KR3 = K3 * R;
  litfloat KR4 = K4 * R;

  // Residual of predictions
  litfloat r = u - xIn[0] * phiCos - xIn[1] * phiSin;

  // Calculate filtered state vector
  std::vector<litfloat> xOut(5);
  xOut[0] = xIn[0] + KR0 * r;
  xOut[1] = xIn[1] + KR1 * r;
  xOut[2] = xIn[2] + KR2 * r;
  xOut[3] = xIn[3] + KR3 * r;
  xOut[4] = xIn[4] + KR4 * r;

  // Calculate filtered covariance matrix
  std::vector<litfloat> cOut(15);
  cOut[0] = cIn[0] - KR0 * K0;
  cOut[1] = cIn[1] - KR0 * K1;
  cOut[2] = cIn[2] - KR0 * K2;
  cOut[3] = cIn[3] - KR0 * K3;
  cOut[4] = cIn[4] - KR0 * K4;

  cOut[5] = cIn[5] - KR1 * K1;
  cOut[6] = cIn[6] - KR1 * K2;
  cOut[7] = cIn[7] - KR1 * K3;
  cOut[8] = cIn[8] - KR1 * K4;

  cOut[9]  = cIn[9] - KR2 * K2;
  cOut[10] = cIn[10] - KR2 * K3;
  cOut[11] = cIn[11] - KR2 * K4;

  cOut[12] = cIn[12] - KR3 * K3;
  cOut[13] = cIn[13] - KR3 * K4;

  cOut[14] = cIn[14] - KR4 * K4;

  // Copy filtered state to output
  par->SetX(xOut[0]);
  par->SetY(xOut[1]);
  par->SetTx(xOut[2]);
  par->SetTy(xOut[3]);
  par->SetQp(xOut[4]);
  par->SetCovMatrix(cOut);

  // Filtered resuduals
  litfloat ru = u - xOut[0] * phiCos - xOut[1] * phiSin;

  // Calculate chi-square
  chiSq = (ru * ru) / (duu - phiCosSq * cOut[0] - phi2SinCos * cOut[1] - phiSinSq * cOut[5]);

  return kLITSUCCESS;
}

LitStatus CbmLitKalmanFilter::UpdateWMF(CbmLitTrackParam* par, const CbmLitStripHit* hit, litfloat& chiSq)
{
  litfloat xIn[5] = {par->GetX(), par->GetY(), par->GetTx(), par->GetTy(), par->GetQp()};

  std::vector<litfloat> cIn    = par->GetCovMatrix();
  std::vector<litfloat> cInInv = par->GetCovMatrix();

  litfloat duu    = hit->GetDu() * hit->GetDu();
  litfloat phiCos = hit->GetCosPhi();
  litfloat phiSin = hit->GetSinPhi();

  // Inverse predicted cov matrix
  InvSym15(cInInv);
  // Calculate C1
  std::vector<litfloat> C1 = cInInv;
  C1[0] += phiCos * phiCos / duu;
  C1[1] += phiCos * phiSin / duu;
  C1[5] += phiSin * phiSin / duu;
  // Inverse C1 -> output updated covariance matrix
  InvSym15(C1);

  std::vector<litfloat> t(5);
  t[0] = cInInv[0] * par->GetX() + cInInv[1] * par->GetY() + cInInv[2] * par->GetTx() + cInInv[3] * par->GetTy()
         + cInInv[4] * par->GetQp() + phiCos * hit->GetU() / duu;
  t[1] = cInInv[1] * par->GetX() + cInInv[5] * par->GetY() + cInInv[6] * par->GetTx() + cInInv[7] * par->GetTy()
         + cInInv[8] * par->GetQp() + phiSin * hit->GetU() / duu;
  t[2] = cInInv[2] * par->GetX() + cInInv[6] * par->GetY() + cInInv[9] * par->GetTx() + cInInv[10] * par->GetTy()
         + cInInv[11] * par->GetQp();
  t[3] = cInInv[3] * par->GetX() + cInInv[7] * par->GetY() + cInInv[10] * par->GetTx() + cInInv[12] * par->GetTy()
         + cInInv[13] * par->GetQp();
  t[4] = cInInv[4] * par->GetX() + cInInv[8] * par->GetY() + cInInv[11] * par->GetTx() + cInInv[13] * par->GetTy()
         + cInInv[14] * par->GetQp();

  std::vector<litfloat> xOut(5);
  Mult15On5(C1, t, xOut);

  // Copy filtered state to output
  par->SetX(xOut[0]);
  par->SetY(xOut[1]);
  par->SetTx(xOut[2]);
  par->SetTy(xOut[3]);
  par->SetQp(xOut[4]);
  par->SetCovMatrix(C1);

  // Calculate chi square
  litfloat zeta = hit->GetU() - phiCos * xOut[0] - phiSin * xOut[1];
  litfloat dx0  = xOut[0] - xIn[0];
  litfloat dx1  = xOut[1] - xIn[1];
  litfloat dx2  = xOut[2] - xIn[2];
  litfloat dx3  = xOut[3] - xIn[3];
  litfloat dx4  = xOut[4] - xIn[4];
  chiSq         = zeta * zeta / duu
          + (dx0 * cInInv[0] + dx1 * cInInv[1] + dx2 * cInInv[2] + dx3 * cInInv[3] + dx4 * cInInv[4]) * dx0
          + (dx0 * cInInv[1] + dx1 * cInInv[5] + dx2 * cInInv[6] + dx3 * cInInv[7] + dx4 * cInInv[8]) * dx1
          + (dx0 * cInInv[2] + dx1 * cInInv[6] + dx2 * cInInv[9] + dx3 * cInInv[10] + dx4 * cInInv[11]) * dx2
          + (dx0 * cInInv[3] + dx1 * cInInv[7] + dx2 * cInInv[10] + dx3 * cInInv[12] + dx4 * cInInv[13]) * dx3
          + (dx0 * cInInv[4] + dx1 * cInInv[8] + dx2 * cInInv[11] + dx3 * cInInv[13] + dx4 * cInInv[14]) * dx4;

  return kLITSUCCESS;
}
