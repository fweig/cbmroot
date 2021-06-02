/** @file CbmLitConverterFairTrackParam.h
  * @copyright Copyright (C) 2016-2017 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [committer], Timur Ablyazimov **/

#ifndef CBMLITCONVERTERFAIRTRACKPARAM_H_
#define CBMLITCONVERTERFAIRTRACKPARAM_H_

#include "base/CbmLitEnums.h"
#include "base/CbmLitTypes.h"

//#include "data/CbmLitHit.h"
//#include "data/CbmLitPixelHit.h"
//#include "data/CbmLitStripHit.h"
//#include "data/CbmLitTrack.h"
#include "data/CbmLitTrackParam.h"
//#include "data/CbmLitFitNode.h"
//#include "data/CbmLitTofTrack.h"

//#include "CbmTrack.h"
//#include "CbmTofTrack.h"
//#include "CbmHit.h"
//#include "CbmPixelHit.h"
//#include "CbmStripHit.h"
#include "CbmTrackParam.h"
//#include "CbmStsTrack.h"
//#include "CbmTrdTrack.h"
//#include "CbmMuchGeoScheme.h"
//#include "CbmMvdHit.h"
//#include "CbmMuchTrack.h"

//#include "CbmGlobalTrack.h"
//#include "CbmTrdAddress.h"

#include "TClonesArray.h"

#include <cassert>
#include <iostream>
#include <set>

#include <cmath>


class CbmLitConverterFairTrackParam {
public:
  static void FairTrackParamToCbmLitTrackParam(const FairTrackParam* par, CbmLitTrackParam* litPar)
  {
    litPar->SetX((litfloat) par->GetX());
    litPar->SetY((litfloat) par->GetY());
    litPar->SetZ((litfloat) par->GetZ());
    litPar->SetTx((litfloat) par->GetTx());
    litPar->SetTy((litfloat) par->GetTy());
    litPar->SetQp((litfloat) par->GetQp());
    litPar->SetTime(0);
    Double_t cov[15];
    par->CovMatrix(cov);
    std::vector<litfloat> covVec(21);
    for (UInt_t i = 0; i < 5; ++i) {
      covVec[i] = (litfloat) cov[i];
    }
    for (UInt_t i = 5; i < 9; ++i) {
      covVec[i + 1] = (litfloat) cov[i];
    }
    for (UInt_t i = 9; i < 12; ++i) {
      covVec[i + 2] = (litfloat) cov[i];
    }
    for (UInt_t i = 12; i < 14; ++i) {
      covVec[i + 3] = (litfloat) cov[i];
    }
    covVec[18] = (litfloat) cov[14];
    covVec[5]  = 0;
    covVec[10] = 0;
    covVec[5]  = 0;
    covVec[14] = 0;
    covVec[17] = 0;
    covVec[19] = 0;
    covVec[20] = 4 * 4;
    litPar->SetCovMatrix(covVec);
  }

  static void FairTrackParamToCbmLitTrackParam(const CbmTrackParam* par, CbmLitTrackParam* litPar)
  {
    litPar->SetX((litfloat) par->GetX());
    litPar->SetY((litfloat) par->GetY());
    litPar->SetZ((litfloat) par->GetZ());
    litPar->SetTx((litfloat) par->GetTx());
    litPar->SetTy((litfloat) par->GetTy());
    litPar->SetQp((litfloat) par->GetQp());
    litPar->SetTime((litfloat) par->GetTime());
    Double_t cov[15];
    par->CovMatrix(cov);
    std::vector<litfloat> covVec(21);
    for (UInt_t i = 0; i < 5; ++i) {
      covVec[i] = (litfloat) cov[i];
    }
    for (UInt_t i = 5; i < 9; ++i) {
      covVec[i + 1] = (litfloat) cov[i];
    }
    for (UInt_t i = 9; i < 12; ++i) {
      covVec[i + 2] = (litfloat) cov[i];
    }
    for (UInt_t i = 12; i < 14; ++i) {
      covVec[i + 3] = (litfloat) cov[i];
    }
    covVec[18] = (litfloat) cov[14];
    covVec[5]  = 0;
    covVec[10] = 0;
    covVec[5]  = 0;
    covVec[14] = 0;
    covVec[17] = 0;
    covVec[19] = 0;
    covVec[20] = (litfloat)(par->GetDTime() * par->GetDTime());
    litPar->SetCovMatrix(covVec);
  }

  static void CbmLitTrackParamToFairTrackParam(const CbmLitTrackParam* litPar, FairTrackParam* par)
  {
    par->SetX(litPar->GetX());
    par->SetY(litPar->GetY());
    par->SetZ(litPar->GetZ());
    par->SetTx(litPar->GetTx());
    par->SetTy(litPar->GetTy());
    par->SetQp(litPar->GetQp());
    Double_t cov[15];
    std::vector<litfloat> covVec(litPar->GetCovMatrix());
    for (UInt_t i = 0; i < 5; ++i) {
      cov[i] = (Double_t) covVec[i];
    }
    for (UInt_t i = 5; i < 9; ++i) {
      cov[i] = (Double_t) covVec[i + 1];
    }
    for (UInt_t i = 9; i < 12; ++i) {
      cov[i] = (Double_t) covVec[i + 2];
    }
    for (UInt_t i = 12; i < 14; ++i) {
      cov[i] = (Double_t) covVec[i + 3];
    }
    cov[14] = (Double_t) covVec[18];
    par->SetCovMatrix(cov);
  }
};

#endif /*CBMLITCONVERTERFAIRTRACKPARAM_H_*/
