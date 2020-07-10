// -------------------------------------------------------------------------
// -----                    CbmTrackParam source file                 -----
// -----                  Created 05/02/16  by T. Ablyazimov          -----
// -------------------------------------------------------------------------
#include "CbmTrackParam.h"

#include <FairTrackParam.h>  // for FairTrackParam

#include <TMath.h>      // for Sqrt
#include <TMathBase.h>  // for Abs

void CbmTrackParam::Set(const FairTrackParam& ftp,
                        Double_t time,
                        Double_t timeError) {
  SetX(ftp.GetX());
  SetY(ftp.GetY());
  SetZ(ftp.GetZ());
  SetTx(ftp.GetTx());
  SetTy(ftp.GetTy());
  SetQp(ftp.GetQp());
  fTime  = time;
  fDTime = timeError;
  Double_t cov[15];
  ftp.CovMatrix(cov);
  SetCovMatrix(cov);
  Double_t p =
    (TMath::Abs(ftp.GetQp()) > 1.e-4) ? 1. / TMath::Abs(ftp.GetQp()) : 1.e4;
  Double_t norma =
    TMath::Sqrt(ftp.GetTx() * ftp.GetTx() + ftp.GetTy() * ftp.GetTy() + 1);
  fPz               = p / norma;
  fPx               = ftp.GetTx() * fPz;
  fPy               = ftp.GetTy() * fPz;
  Double_t DpzByDqp = -p * p / norma;
  Double_t DpzByDtx = -p * ftp.GetTx() / (norma * norma * norma);
  Double_t DpzByDty = -p * ftp.GetTy() / (norma * norma * norma);
  fDpz              = TMath::Sqrt(
    DpzByDqp * DpzByDqp * cov[14] + DpzByDtx * DpzByDtx * cov[9]
    + DpzByDty * DpzByDty * cov[12] + 2 * DpzByDqp * DpzByDtx * cov[11]
    + 2 * DpzByDqp * DpzByDty * cov[13] + 2 * DpzByDtx * DpzByDty * cov[10]);
  Double_t DpxByDqp = ftp.GetTx() * DpzByDqp;
  Double_t DpxByDtx =
    p * (ftp.GetTy() * ftp.GetTy() + 1) / (norma * norma * norma);
  Double_t DpxByDty = -p * ftp.GetTx() * ftp.GetTy() / (norma * norma * norma);
  fDpx              = TMath::Sqrt(
    DpxByDqp * DpxByDqp * cov[14] + DpxByDtx * DpxByDtx * cov[9]
    + DpxByDty * DpxByDty * cov[12] + 2 * DpxByDqp * DpxByDtx * cov[11]
    + 2 * DpxByDqp * DpxByDty * cov[13] + 2 * DpxByDtx * DpxByDty * cov[10]);
  Double_t DpyByDqp = ftp.GetTy() * DpzByDqp;
  Double_t DpyByDtx = -p * ftp.GetTx() * ftp.GetTy() / (norma * norma * norma);
  Double_t DpyByDty =
    p * (ftp.GetTx() * ftp.GetTx() + 1) / (norma * norma * norma);
  fDpy = TMath::Sqrt(
    DpyByDqp * DpyByDqp * cov[14] + DpyByDtx * DpyByDtx * cov[9]
    + DpyByDty * DpyByDty * cov[12] + 2 * DpyByDqp * DpyByDtx * cov[11]
    + 2 * DpyByDqp * DpyByDty * cov[13] + 2 * DpyByDtx * DpyByDty * cov[10]);
}

ClassImp(CbmTrackParam)
