/* Copyright (C) 2018-2020 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci[committer] */

#include "CbmTrdDigiRec.h"

#include "CbmTrdFASP.h"

ClassImp(CbmTrdDigiRec)

  //_____________________________________________________________________________
  CbmTrdDigiRec::CbmTrdDigiRec()
  : CbmTrdDigi()
  , fStatus(0)
{
  fG[0] = 1.;
  fG[1] = 1.;
  memset(fT, 0, 3 * sizeof(Double_t));
}

//_____________________________________________________________________________
CbmTrdDigiRec::CbmTrdDigiRec(const CbmTrdDigi& d, Double_t* G, Double_t* T) : CbmTrdDigi(d), fStatus(0)
{
  if (G) {
    fG[0] = G[0];
    fG[1] = G[1];
  }
  else {
    fG[0] = 1.;
    fG[1] = 1.;
  }
  if (T) memcpy(fT, T, 3 * sizeof(Double_t));
  else
    memset(fT, 0, 3 * sizeof(Double_t));

  Int_t dt;
  Double_t t, r = CbmTrdDigi::GetCharge(t, dt);
  if (t > 4094) SETBIT(fStatus, 0);
  if (r > 4094) SETBIT(fStatus, 1);
}

//_____________________________________________________________________________
CbmTrdDigiRec::CbmTrdDigiRec(const CbmTrdDigi& d, const CbmTrdDigi& dr, Double_t* G, Double_t* T)
  : CbmTrdDigi(d)
  , fStatus(0)
{
  /** Constructor and RAW digi merger
*/
  if (G) {
    fG[0] = G[0];
    fG[1] = G[1];
  }
  else {
    fG[0] = 1.;
    fG[1] = 1.;
  }
  if (T) memcpy(fT, T, 3 * sizeof(Double_t));
  else
    memset(fT, 0, 3 * sizeof(Double_t));

  Int_t dt;
  Double_t t, r = dr.GetCharge(t, dt);
  if (r > 4094) SETBIT(fStatus, 1);
  CbmTrdDigi::GetCharge(t, dt);
  if (t > 4094) SETBIT(fStatus, 0);
  dt = dr.GetTimeDAQ() - GetTimeDAQ();
  SetCharge(t, r, dt);
  Int_t rtrg(dr.GetTriggerType() & 2), ttrg(GetTriggerType() & 1);
  SetTriggerType(rtrg | ttrg);  //merge the triggers
}

//_____________________________________________________________________________
Double_t CbmTrdDigiRec::GetCharge(Int_t typ, Bool_t& on) const
{
  Int_t dT;
  Double_t T, R = CbmTrdDigi::GetCharge(T, dT);
  on = kTRUE;
  if (typ) {  // rectangular pairing
    if (R > 0) {
      R -= CbmTrdFASP::GetBaselineCorr();
      return fG[1] * R;
    }
    else
      on = kFALSE;
  }
  else {  // tilt pairing
    if (T > 0) {
      T -= CbmTrdFASP::GetBaselineCorr();
      return fG[0] * T;
    }
    else
      on = kFALSE;
  }
  return 0.;
}

//_____________________________________________________________________________
Double_t CbmTrdDigiRec::GetTime(Int_t typ) const
{
  Int_t dT;
  Double_t T, R = CbmTrdDigi::GetCharge(T, dT);
  if (typ) {  // rectangular pairing
    Double_t R0 = R - fT[2];
    return GetTimeDAQ() + dT + fT[0] * R0 * R0 + fT[1] * R0;
  }
  else {  // tilt pairing
    Double_t T0 = T - fT[2];
    return GetTimeDAQ() + fT[0] * T0 * T0 + fT[1] * T0;
  }
}

//_____________________________________________________________________________
void CbmTrdDigiRec::Init(Double_t g[2], Double_t t[3])
{
  fG[0] = g[0];
  fG[1] = g[1];
  memcpy(fT, t, 3 * sizeof(Double_t));
}
