/** CbmLitLineTrackExtrapolator.cxx
 *@author A.Lebedev <andrey.lebedev@gsi.de>
 *@since 2007
 **
 **/

#include "propagation/CbmLitLineTrackExtrapolator.h"

#include "data/CbmLitTrackParam.h"

CbmLitLineTrackExtrapolator::CbmLitLineTrackExtrapolator()
{
}

CbmLitLineTrackExtrapolator::~CbmLitLineTrackExtrapolator()
{
}

LitStatus CbmLitLineTrackExtrapolator::Extrapolate(
   const CbmLitTrackParam* parIn,
   CbmLitTrackParam* parOut,
   litfloat zOut,
   std::vector<litfloat>* F)
{
   *parOut = *parIn;
   return Extrapolate(parOut, zOut, F);
}

LitStatus CbmLitLineTrackExtrapolator::Extrapolate(
   CbmLitTrackParam* par,
   litfloat zOut,
   std::vector<litfloat>* F)
{
   litfloat X[6] = { par->GetX(), par->GetY(),
                par->GetTx(), par->GetTy(),
                par->GetQp(), par->GetTime()
              };

   litfloat dz = zOut - par->GetZ();

   //transport state vector F*X*F.T()
   X[0] = X[0] + dz * X[2];
   X[1] = X[1] + dz * X[3];

   par->SetX(X[0]);
   par->SetY(X[1]);

   std::vector<litfloat> C = par->GetCovMatrix();
   litfloat txSq = std::pow(X[2], 2);
   litfloat tySq = std::pow(X[3], 2);
   litfloat timeCoeff = dz / std::sqrt(1 + txSq + tySq) / CbmLitTrackParam::fSpeedOfLight;
   litfloat dttx = X[2] * timeCoeff;
   litfloat dttxz = dttx * dz;
   litfloat dtty = X[3] * timeCoeff;
   litfloat dttyz = dtty * dz;
   //transport covariance matrix F*C*F.T()
   litfloat t3 = C[2] + dz * C[11];
   litfloat t7 = dz * C[12];
   litfloat t8 = C[3] + t7;
   litfloat t19 = C[8] + dz * C[15];
   C[0] = C[0] + dz * C[2] + t3 * dz;
   C[1] = C[1] + dz * C[7] + t8 * dz;
   C[2] = t3;
   C[3] = t8;
   C[4] = C[4] + dz * C[13];
   C[5] = C[5] + dz * C[14] + dttx * dttxz * C[11] + dttyz * C[12] + dttx * C[2] + dtty * C[3];
   C[6] = C[6] + dz * C[8] + t19 * dz;
   C[7] = C[7] + t7;
   C[8] = t19;
   C[9] = C[9] + dz * C[16];
   C[10] = C[10] + dz * C[17] + dttxz * C[12] + dttyz * C[15] + dttx * C[7] + dtty * C[8];
   C[14] = C[14] + dttx * C[11] + dtty * C[12];
   C[17] = C[17] + dttx * C[12] + dtty * C[15];
   C[19] = C[19] + dttx * C[13] + dtty * C[16];
   C[20] = C[20] + 2 * dttx * dtty * C[12] + dttx * C[14] + dtty * C[17] + dttx * dttx * C[11] + dtty * dtty * C[15];

   par->SetCovMatrix(C);
   par->SetZ(zOut);
   par->SetTime(par->GetTime() + std::sqrt(1 + std::pow(X[2], 2) + std::pow(X[3], 2)) * dz / CbmLitTrackParam::fSpeedOfLight);

   // Transport matrix calculation
   if (F != NULL) {
      F->assign(36, 0.);
      (*F)[0] = 1.;
      (*F)[7] = 1.;
      (*F)[14] = 1.;
      (*F)[21] = 1.;
      (*F)[28] = 1.;
      (*F)[35] = 1.;
      (*F)[2] = dz;
      (*F)[9] = dz;
      (*F)[32] = dttx;
      (*F)[33] = dtty;
   }

   return kLITSUCCESS;
}
