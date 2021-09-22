/* Copyright (C) 2019-2020 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * CbmHelix.cxx
 *
 *  Created on: 27 sie 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmHelix.h"

#include <TMath.h>

#include <vector>
// matrix x, y, tx,ty, qp, z
FairField* CbmHelix::fgField = NULL;
CbmHelix::CbmHelix() {}

void CbmHelix::Build(const CbmGlobalTrack* tr)
{
  const FairTrackParam* parameters = tr->GetParamVertex();
  SetParameters(parameters);
}

void CbmHelix::Build(const CbmStsTrack* tr, Bool_t firstPoint)
{
  if (firstPoint) { SetParameters(tr->GetParamFirst()); }
  else {
    SetParameters(tr->GetParamLast());
  }
}

TVector3 CbmHelix::Eval(Double_t z)
{
  Propagate(z);
  return TVector3(GetTrack()[0], GetTrack()[1], GetTrack()[5]);
}

TVector3 CbmHelix::Eval(Double_t z, TVector3& mom)
{
  Propagate(z);
  Double_t p  = (TMath::Abs(Qp()) > 1.e-4) ? 1. / TMath::Abs(Qp()) : 1.e4;
  Double_t pz = TMath::Sqrt(p * p / (Tx() * Tx() + Ty() * Ty() + 1));
  Double_t px = Tx() * pz;
  Double_t py = Ty() * pz;
  mom.SetXYZ(px, py, pz);
  return TVector3(GetTrack()[0], GetTrack()[1], GetTrack()[5]);
}

void CbmHelix::SetParameters(const FairTrackParam* param)
{
  fTb[0] = param->GetX();
  fTb[1] = param->GetY();
  fTb[2] = param->GetTx();
  fTb[3] = param->GetTy();
  fTb[4] = param->GetQp();
  fTb[5] = param->GetZ();
  /*
    for (Int_t i=0,iCov=0; i<5; i++)
    	for (Int_t j=0; j<=i; j++,iCov++)
    		fC[iCov] = param->GetCovariance(i,j);
    		*/
}

void CbmHelix::Build(const TVector3& pos, const TVector3& mom, Double_t charge)
{
  fTb[0]     = pos.X();
  fTb[1]     = pos.Y();
  Double_t p = mom.Mag();
  fTb[2]     = mom.Px() / mom.Pz();
  fTb[3]     = mom.Py() / mom.Pz();
  fTb[4]     = charge / p;
  fTb[5]     = pos.Z();
}

CbmHelix::~CbmHelix() {}

CbmHelix::CbmHelix(const CbmHelix& other) : TObject()
{
  for (int i = 0; i < 6; i++) {
    fT[i]  = other.fT[i];
    fTb[i] = other.fTb[i];
  }
  /*
	for(int i=0;i<15;i++){
		fC[i] = other.fC[i];
	}*/
}

CbmHelix& CbmHelix::operator=(const CbmHelix& other)
{
  if (&other == this) return *this;
  for (int i = 0; i < 6; i++) {
    fT[i]  = other.fT[i];
    fTb[i] = other.fTb[i];
  }
  //for(int i=0;i<15;i++)
  //	fC[i] = other.fC[i];
  return *this;
}

Int_t CbmHelix::Propagate(Double_t z)
{
  Int_t fMethod = 1;
  Bool_t err    = 0;
  for (int i = 0; i < 6; i++) {
    fT[i] = fTb[i];
  }
  if (fabs(fT[5] - z) < 1.e-5) return 0;

  if (!fgField || (300 <= z && 300 <= fT[5])) {
    // ExtrapolateLine(Z );
    //  return 0;
  }
  Double_t zz = z;
  if (z < 300. && 300 <= fT[5]) ExtrapolateLine(300.);

  if (fT[5] < 300. && 300. < z) { zz = 300.; }
  Bool_t repeat = 1;
  while (!err && repeat) {
    const Double_t max_step = 5.;
    Double_t zzz;
    if (fabs(fT[5] - zz) > max_step) zzz = fT[5] + ((zz > fT[5]) ? max_step : -max_step);
    else {
      zzz    = zz;
      repeat = 0;
    }
    switch (fMethod) {
      case 0: {
        ExtrapolateLine(zzz);
        break;
      }
      case 1: {
        err = err || ExtrapolateALight(zzz);
        break;
      }
      case 2: {
        err = err || ExtrapolateRK4(zzz);
        break;
      }
    }
  }
  if (fT[5] != z) ExtrapolateLine(z);
  return err;
}

void CbmHelix::ExtrapolateLine(Double_t z_out)
{
  Double_t dz = z_out - fT[5];

  fT[0] += dz * fT[2];
  fT[1] += dz * fT[3];
  fT[5] = z_out;
  /*
	const Double_t dzC_in8 = dz * fC[8];

	fC[4] = fC[4] + dzC_in8;
	fC[1] = fC[1] + dz * (fC[4] + fC[6]);

	const Double_t C_in3 = fC[3];

	fC[3] = C_in3 + dz * fC[5];
	fC[0] = fC[0] + dz * (fC[3] + C_in3);

	const Double_t C_in7 = fC[7];

	fC[7] = C_in7 + dz * fC[9];
	fC[2] = fC[2] + dz * (fC[7] + C_in7);
	fC[6] = fC[6] + dzC_in8;
*/
}

Int_t CbmHelix::ExtrapolateRK4(Double_t z_out)
{
  const Double_t c_light = 0.000299792458;

  static Double_t a[4] = {0.0, 0.5, 0.5, 1.0};
  static Double_t c[4] = {1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0};
  static Double_t b[4] = {0.0, 0.5, 0.5, 1.0};

  Int_t step4;
  Double_t k[16], x0[4], x[4], k1[16];
  Double_t Ax[4], Ay[4], Ax_tx[4], Ay_tx[4], Ax_ty[4], Ay_ty[4];

  //----------------------------------------------------------------

  Double_t qp_in = fT[4];
  Double_t z_in  = fT[5];
  Double_t h     = z_out - z_in;
  Double_t hC    = h * c_light;
  x0[0]          = fT[0];
  x0[1]          = fT[1];
  x0[2]          = fT[2];
  x0[3]          = fT[3];
  //
  //   Runge-Kutta step
  //

  Int_t step;
  Int_t i;

  for (step = 0; step < 4; ++step) {
    for (i = 0; i < 4; ++i) {
      if (step == 0) { x[i] = x0[i]; }
      else {
        x[i] = x0[i] + b[step] * k[step * 4 - 4 + i];
      }
    }

    Double_t point[3] = {x[0], x[1], z_in + a[step] * h};
    Double_t B[3];
    if (fgField) fgField->GetFieldValue(point, B);
    else {
      B[0] = B[1] = B[2] = 0.;
    }

    Double_t tx      = x[2];
    Double_t ty      = x[3];
    Double_t tx2     = tx * tx;
    Double_t ty2     = ty * ty;
    Double_t txty    = tx * ty;
    Double_t tx2ty21 = 1.0 + tx2 + ty2;
    if (tx2ty21 > 1.e4) return 1;
    Double_t I_tx2ty21 = 1.0 / tx2ty21 * Qp();
    Double_t tx2ty2    = sqrt(tx2ty21);
    //   Double_t I_tx2ty2 = qp0 * hC / tx2ty2 ; unsused ???
    tx2ty2 *= hC;
    Double_t tx2ty2qp = tx2ty2 * Qp();
    Ax[step]          = (txty * B[0] + ty * B[2] - (1.0 + tx2) * B[1]) * tx2ty2;
    Ay[step]          = (-txty * B[1] - tx * B[2] + (1.0 + ty2) * B[0]) * tx2ty2;

    Ax_tx[step] = Ax[step] * tx * I_tx2ty21 + (ty * B[0] - 2.0 * tx * B[1]) * tx2ty2qp;
    Ax_ty[step] = Ax[step] * ty * I_tx2ty21 + (tx * B[0] + B[2]) * tx2ty2qp;
    Ay_tx[step] = Ay[step] * tx * I_tx2ty21 + (-ty * B[1] - B[2]) * tx2ty2qp;
    Ay_ty[step] = Ay[step] * ty * I_tx2ty21 + (-tx * B[1] + 2.0 * ty * B[0]) * tx2ty2qp;

    step4        = step * 4;
    k[step4]     = tx * h;
    k[step4 + 1] = ty * h;
    k[step4 + 2] = Ax[step] * Qp();
    k[step4 + 3] = Ay[step] * Qp();

  }  // end of Runge-Kutta steps

  for (i = 0; i < 4; ++i) {
    fT[i] = x0[i] + c[0] * k[i] + c[1] * k[4 + i] + c[2] * k[8 + i] + c[3] * k[12 + i];
  }
  fT[5] = z_out;
  //
  //     Derivatives    dx/dqp
  //

  x0[0] = 0.0;
  x0[1] = 0.0;
  x0[2] = 0.0;
  x0[3] = 0.0;

  //
  //   Runge-Kutta step for derivatives dx/dqp

  for (step = 0; step < 4; ++step) {
    for (i = 0; i < 4; ++i) {
      if (step == 0) { x[i] = x0[i]; }
      else {
        x[i] = x0[i] + b[step] * k1[step * 4 - 4 + i];
      }
    }
    step4         = step * 4;
    k1[step4]     = x[2] * h;
    k1[step4 + 1] = x[3] * h;
    k1[step4 + 2] = Ax[step] + Ax_tx[step] * x[2] + Ax_ty[step] * x[3];
    k1[step4 + 3] = Ay[step] + Ay_tx[step] * x[2] + Ay_ty[step] * x[3];

  }  // end of Runge-Kutta steps for derivatives dx/dqp

  Double_t J[25];

  for (i = 0; i < 4; ++i) {
    J[20 + i] = x0[i] + c[0] * k1[i] + c[1] * k1[4 + i] + c[2] * k1[8 + i] + c[3] * k1[12 + i];
  }
  J[24] = 1.;
  //
  //      end of derivatives dx/dqp
  //

  //     Derivatives    dx/tx
  //

  x0[0] = 0.0;
  x0[1] = 0.0;
  x0[2] = 1.0;
  x0[3] = 0.0;

  //
  //   Runge-Kutta step for derivatives dx/dtx
  //

  for (step = 0; step < 4; ++step) {
    for (i = 0; i < 4; ++i) {
      if (step == 0) { x[i] = x0[i]; }
      else if (i != 2) {
        x[i] = x0[i] + b[step] * k1[step * 4 - 4 + i];
      }
    }
    step4         = step * 4;
    k1[step4]     = x[2] * h;
    k1[step4 + 1] = x[3] * h;
    //    k1[step4+2] = Ax_tx[step] * x[2] + Ax_ty[step] * x[3];
    k1[step4 + 3] = Ay_tx[step] * x[2] + Ay_ty[step] * x[3];

  }  // end of Runge-Kutta steps for derivatives dx/dtx

  for (i = 0; i < 4; ++i) {
    if (i != 2) { J[10 + i] = x0[i] + c[0] * k1[i] + c[1] * k1[4 + i] + c[2] * k1[8 + i] + c[3] * k1[12 + i]; }
  }
  //      end of derivatives dx/dtx
  J[12] = 1.0;
  J[14] = 0.0;

  //     Derivatives    dx/ty
  //

  x0[0] = 0.0;
  x0[1] = 0.0;
  x0[2] = 0.0;
  x0[3] = 1.0;

  //
  //   Runge-Kutta step for derivatives dx/dty
  //

  for (step = 0; step < 4; ++step) {
    for (i = 0; i < 4; ++i) {
      if (step == 0) {
        x[i] = x0[i];  // ty fixed
      }
      else if (i != 3) {
        x[i] = x0[i] + b[step] * k1[step * 4 - 4 + i];
      }
    }
    step4         = step * 4;
    k1[step4]     = x[2] * h;
    k1[step4 + 1] = x[3] * h;
    k1[step4 + 2] = Ax_tx[step] * x[2] + Ax_ty[step] * x[3];
    //    k1[step4+3] = Ay_tx[step] * x[2] + Ay_ty[step] * x[3];

  }  // end of Runge-Kutta steps for derivatives dx/dty

  for (i = 0; i < 3; ++i) {
    J[15 + i] = x0[i] + c[0] * k1[i] + c[1] * k1[4 + i] + c[2] * k1[8 + i] + c[3] * k1[12 + i];
  }
  //      end of derivatives dx/dty
  J[18] = 1.;
  J[19] = 0.;

  //
  //    derivatives dx/dx and dx/dy

  for (i = 0; i < 10; ++i) {
    J[i] = 0.;
  }
  J[0] = 1.;
  J[6] = 1.;

  // extrapolate inverse momentum

  fT[4] = qp_in;

  Double_t dqp = qp_in - Qp();

  {
    for (Int_t ip = 0; ip < 4; ip++) {
      fT[ip] += J[5 * 4 + ip] * dqp;
    }
  }

  //          covariance matrix transport

  multQtSQ(5, J);
  return 0;
}

Int_t CbmHelix::ExtrapolateALight(Double_t z_out)
{
  //
  //  Part of the analytic extrapolation formula with error (c_light*B*dz)^4/4!
  //
  {
    bool ok = 1;
    for (int i = 0; i < 6; i++)
      ok = ok && !TMath::IsNaN(fT[i]) && (fT[i] < 1.e5);
    /*	for (int i = 0; i < 15; i++)
			ok = ok && !TMath::IsNaN(fC[i]);
		if (!ok) {
			for (int i = 0; i < 15; i++)
				fC[i] = 0;
			fC[0] = fC[2] = fC[5] = fC[9] = fC[14] = 100.;
			return 1;
		}
		*/
  }
  const Double_t c_light = 0.000299792458;

  //Double_t qp_in = fT[4];
  Double_t z_in = fT[5];
  Double_t dz   = z_out - z_in;

  // construct coefficients

  Double_t x = fT[2],  // tx !!
    y        = fT[3],  // ty !!

    xx = x * x, xy = x * y, yy = y * y, x2 = x * 2, x4 = x * 4, xx31 = xx * 3 + 1, xx159 = xx * 15 + 9, y2 = y * 2;

  Double_t Ax = xy, Ay = -xx - 1, Az = y, Ayy = x * (xx * 3 + 3), Ayz = -2 * xy, Ayyy = -(15 * xx * xx + 18 * xx + 3),

           Ax_x = y, Ay_x = -x2, Az_x = 0, Ayy_x = 3 * xx31, Ayz_x = -y2, Ayyy_x = -x4 * xx159,

           Ax_y = x, Ay_y = 0, Az_y = 1, Ayy_y = 0, Ayz_y = -x2, Ayyy_y = 0,

           Bx = yy + 1, By = -xy, Bz = -x, Byy = y * xx31, Byz = 2 * xx + 1, Byyy = -xy * xx159,

           Bx_x = 0, By_x = -y, Bz_x = -1, Byy_x = 6 * xy, Byz_x = x4, Byyy_x = -y * (45 * xx + 9),

           Bx_y = y2, By_y = -x, Bz_y = 0, Byy_y = xx31, Byz_y = 0, Byyy_y = -x * xx159;

  // end of coefficients calculation

  Double_t t2 = 1. + xx + yy;
  if (t2 > 1.e4) return 1;
  Double_t t = sqrt(t2), h = Qp() * c_light, ht = h * t;

  Double_t sx = 0, sy = 0, sz = 0, syy = 0, syz = 0, syyy = 0, Sx = 0, Sy = 0, Sz = 0, Syy = 0, Syz = 0, Syyy = 0;

  {  // get field integrals

    Double_t B[3][3];
    Double_t r0[3], r1[3], r2[3];

    // first order track approximation

    r0[0] = fT[0];
    r0[1] = fT[1];
    r0[2] = fT[5];

    r2[0] = fT[0] + fT[2] * dz;
    r2[1] = fT[1] + fT[3] * dz;
    r2[2] = z_out;

    r1[0] = 0.5 * (r0[0] + r2[0]);
    r1[1] = 0.5 * (r0[1] + r2[1]);
    r1[2] = 0.5 * (r0[2] + r2[2]);

    fgField->GetFieldValue(r0, B[0]);
    fgField->GetFieldValue(r1, B[1]);
    fgField->GetFieldValue(r2, B[2]);

    Sy    = (7 * B[0][1] + 6 * B[1][1] - B[2][1]) * dz * dz / 96.;
    r1[0] = fT[0] + x * dz / 2 + ht * Sy * Ay;
    r1[1] = fT[1] + y * dz / 2 + ht * Sy * By;

    Sy    = (B[0][1] + 2 * B[1][1]) * dz * dz / 6.;
    r2[0] = fT[0] + x * dz + ht * Sy * Ay;
    r2[1] = fT[1] + y * dz + ht * Sy * By;

    Sy = 0;

    // integrals

    fgField->GetFieldValue(r0, B[0]);
    fgField->GetFieldValue(r1, B[1]);
    fgField->GetFieldValue(r2, B[2]);

    sx = (B[0][0] + 4 * B[1][0] + B[2][0]) * dz / 6.;
    sy = (B[0][1] + 4 * B[1][1] + B[2][1]) * dz / 6.;
    sz = (B[0][2] + 4 * B[1][2] + B[2][2]) * dz / 6.;

    Sx = (B[0][0] + 2 * B[1][0]) * dz * dz / 6.;
    Sy = (B[0][1] + 2 * B[1][1]) * dz * dz / 6.;
    Sz = (B[0][2] + 2 * B[1][2]) * dz * dz / 6.;

    Double_t c2[3][3] = {{5, -4, -1}, {44, 80, -4}, {11, 44, 5}};    // /=360.
    Double_t C2[3][3] = {{38, 8, -4}, {148, 208, -20}, {3, 36, 3}};  // /=2520.
    for (Int_t n = 0; n < 3; n++)
      for (Int_t m = 0; m < 3; m++) {
        syz += c2[n][m] * B[n][1] * B[m][2];
        Syz += C2[n][m] * B[n][1] * B[m][2];
      }

    syz *= dz * dz / 360.;
    Syz *= dz * dz * dz / 2520.;

    syy  = (B[0][1] + 4 * B[1][1] + B[2][1]) * dz;
    syyy = syy * syy * syy / 1296;
    syy  = syy * syy / 72;

    Syy = (B[0][1] * (38 * B[0][1] + 156 * B[1][1] - B[2][1]) + B[1][1] * (208 * B[1][1] + 16 * B[2][1])
           + B[2][1] * (3 * B[2][1]))
          * dz * dz * dz / 2520.;
    Syyy = (B[0][1]
              * (B[0][1] * (85 * B[0][1] + 526 * B[1][1] - 7 * B[2][1]) + B[1][1] * (1376 * B[1][1] + 84 * B[2][1])
                 + B[2][1] * (19 * B[2][1]))
            + B[1][1] * (B[1][1] * (1376 * B[1][1] + 256 * B[2][1]) + B[2][1] * (62 * B[2][1]))
            + B[2][1] * B[2][1] * (3 * B[2][1]))
           * dz * dz * dz * dz / 90720.;
  }

  Double_t

    sA1   = sx * Ax + sy * Ay + sz * Az,
    sA1_x = sx * Ax_x + sy * Ay_x + sz * Az_x, sA1_y = sx * Ax_y + sy * Ay_y + sz * Az_y,

    sB1 = sx * Bx + sy * By + sz * Bz, sB1_x = sx * Bx_x + sy * By_x + sz * Bz_x,
    sB1_y = sx * Bx_y + sy * By_y + sz * Bz_y,

    SA1 = Sx * Ax + Sy * Ay + Sz * Az, SA1_x = Sx * Ax_x + Sy * Ay_x + Sz * Az_x,
    SA1_y = Sx * Ax_y + Sy * Ay_y + Sz * Az_y,

    SB1 = Sx * Bx + Sy * By + Sz * Bz, SB1_x = Sx * Bx_x + Sy * By_x + Sz * Bz_x,
    SB1_y = Sx * Bx_y + Sy * By_y + Sz * Bz_y,

    sA2 = syy * Ayy + syz * Ayz, sA2_x = syy * Ayy_x + syz * Ayz_x, sA2_y = syy * Ayy_y + syz * Ayz_y,
    sB2 = syy * Byy + syz * Byz, sB2_x = syy * Byy_x + syz * Byz_x, sB2_y = syy * Byy_y + syz * Byz_y,

    SA2 = Syy * Ayy + Syz * Ayz, SA2_x = Syy * Ayy_x + Syz * Ayz_x, SA2_y = Syy * Ayy_y + Syz * Ayz_y,
    SB2 = Syy * Byy + Syz * Byz, SB2_x = Syy * Byy_x + Syz * Byz_x, SB2_y = Syy * Byy_y + Syz * Byz_y,

    sA3 = syyy * Ayyy, sA3_x = syyy * Ayyy_x, sA3_y = syyy * Ayyy_y, sB3 = syyy * Byyy, sB3_x = syyy * Byyy_x,
    sB3_y = syyy * Byyy_y,

    SA3 = Syyy * Ayyy, SA3_x = Syyy * Ayyy_x, SA3_y = Syyy * Ayyy_y, SB3 = Syyy * Byyy, SB3_x = Syyy * Byyy_x,
    SB3_y = Syyy * Byyy_y;
#ifdef SKIP_LOSSES
  fT[0] = fT[0] + x * dz + ht * (SA1 + ht * (SA2 + ht * SA3));
  fT[1] = fT[1] + y * dz + ht * (SB1 + ht * (SB2 + ht * SB3));
  fT[2] = fT[2] + ht * (sA1 + ht * (sA2 + ht * sA3));
  fT[3] = fT[3] + ht * (sB1 + ht * (sB2 + ht * sB3));
  fT[5] = z_out;
#else
  T_out[0] = fT[0] + x * dz + ht * (SA1 + ht * (SA2 + ht * SA3));
  T_out[1] = fT[1] + y * dz + ht * (SB1 + ht * (SB2 + ht * SB3));
  T_out[2] = fT[2] + ht * (sA1 + ht * (sA2 + ht * sA3));
  T_out[3] = fT[3] + ht * (sB1 + ht * (sB2 + ht * sB3));
  T_out[4] = fT[4];
  T_out[5] = z_out;
#endif
  Double_t J[25];

  // derivatives '_x

  J[0] = 1;
  J[1] = 0;
  J[2] = 0;
  J[3] = 0;
  J[4] = 0;

  // derivatives '_y

  J[5] = 0;
  J[6] = 1;
  J[7] = 0;
  J[8] = 0;
  J[9] = 0;

  // derivatives '_tx

  J[10] = dz + h * x * (1. / t * SA1 + h * (2 * SA2 + 3 * ht * SA3)) + ht * (SA1_x + ht * (SA2_x + ht * SA3_x));
  J[11] = h * x * (1. / t * SB1 + h * (2 * SB2 + 3 * ht * SB3)) + ht * (SB1_x + ht * (SB2_x + ht * SB3_x));
  J[12] = 1 + h * x * (1. / t * sA1 + h * (2 * sA2 + 3 * ht * sA3)) + ht * (sA1_x + ht * (sA2_x + ht * sA3_x));
  J[13] = h * x * (1. / t * sB1 + h * (2 * sB2 + 3 * ht * sB3)) + ht * (sB1_x + ht * (sB2_x + ht * sB3_x));
  J[14] = 0;

  // derivatives '_ty

  J[15] = h * y * (1. / t * SA1 + h * (2 * SA2 + 3 * ht * SA3)) + ht * (SA1_y + ht * (SA2_y + ht * SA3_y));
  J[16] = dz + h * y * (1. / t * SB1 + h * (2 * SB2 + 3 * ht * SB3)) + ht * (SB1_y + ht * (SB2_y + ht * SB3_y));
  J[17] = h * y * (1. / t * sA1 + h * (2 * sA2 + 3 * ht * sA3)) + ht * (sA1_y + ht * (sA2_y + ht * sA3_y));
  J[18] = 1 + h * y * (1. / t * sB1 + h * (2 * sB2 + 3 * ht * sB3)) + ht * (sB1_y + ht * (sB2_y + ht * sB3_y));
  J[19] = 0;

  // derivatives '_qp

  J[20] = c_light * t * (SA1 + ht * (2 * SA2 + 3 * ht * SA3));
  J[21] = c_light * t * (SB1 + ht * (2 * SB2 + 3 * ht * SB3));
  J[22] = c_light * t * (sA1 + ht * (2 * sA2 + 3 * ht * sA3));
  J[23] = c_light * t * (sB1 + ht * (2 * sB2 + 3 * ht * sB3));
  J[24] = 1;

  // extrapolate inverse momentum
#ifdef SKIP_LOSSES
#else
  T_out[4] = qp_in;

  Double_t dqp = qp_in - Qp();


  for (Int_t i = 0; i < 4; i++) {
    fT[i] = T_out[i] + J[5 * 4 + i] * dqp;
  }
  fT[4] = T_out[4];
  fT[5] = T_out[5];
#endif
  //          covariance matrix transport

  multQtSQ(5, J);
  return 0;
}

void CbmHelix::multQtSQ(const Int_t /*N*/, Double_t /*Q*/[])
{
  /*Double_t A[N * N];

	  for( Int_t i=0, n=0; i<N; i++ ){
	    for( Int_t j=0; j<N; j++, ++n ){
	      A[n] = 0 ;
	      for( Int_t k=0; k<N; ++k ) A[n]+= fC[indexS(i,k)] * Q[ k*N+j];
	    }
	  }

	  for( Int_t i=0; i<N; i++ ){
	    for( Int_t j=0; j<=i; j++ ){
	      Int_t n = indexS(i,j);
	      fC[n] = 0 ;
	      for( Int_t k=0; k<N; k++ )  fC[n] += Q[ k*N+i ] * A[ k*N+j ];
	    }
	  }*/
}
