/* Copyright (C) 2015-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Gregor Pitsch, Semen Lebedev */

#ifndef LMVM_CUTS_H
#define LMVM_CUTS_H

#include <Logger.h>

#include <math.h>

#include "LmvmDef.h"


class LmvmCuts {
public:
  LmvmCuts() {}

  bool IsTopologyCutOk(ELmvmTopologyCut cut, double mom1, double mom2, double minAngle)
  {
    double angleCut = 0., ppCut = 0.;
    if (cut == ELmvmTopologyCut::ST) {
      angleCut = fStCutAngle;
      ppCut    = fStCutPP;
    }
    else if (cut == ELmvmTopologyCut::RT) {
      angleCut = fRtCutAngle;
      ppCut    = fRtCutPP;
    }
    else if (cut == ELmvmTopologyCut::TT) {
      angleCut = fTtCutAngle;
      ppCut    = fTtCutPP;
    }
    else {
      LOG(error) << "LmvmCuts::IsTopologyCut cut is not defined.";
    }
    double sqrt_mom = std::sqrt(mom1 * mom2);
    double val      = -1. * (angleCut / ppCut) * sqrt_mom + angleCut;
    if (!(sqrt_mom < ppCut && val > minAngle)) return true;
    return false;
  }

  bool IsChi2PrimaryOk(double chi2Prim) { return (chi2Prim < fChi2PrimCut); }

  bool IsGammaCutOk(double minv) { return (minv >= fGammaCut); }

  bool IsPtCutOk(double pt) { return (pt > fPtCut); }

  bool IsMvdCutOk(int stationNum, double dmvd, double mom)
  {
    if (stationNum <= 0 || stationNum > 2) {
      LOG(error) << "LmvmCuts::IsMvdCut stationNum is not in valid. stationNum = " << stationNum;
      return false;
    }
    // it is assumed that stationNum can be 1 or 2
    LOG(info) << "I am in LmvmCuts::IsMvdCutOk";  // TODO: delete this line
    double cutD = (stationNum == 1) ? fMvd1CutD : fMvd2CutD;
    double cutP = (stationNum == 1) ? fMvd1CutP : fMvd2CutP;
    double val  = -1. * (cutP / cutD) * dmvd + cutP;
    if (!(dmvd < cutD && val > mom)) {LOG(info) << "MVD cut passed."; return true;} // TODO: delete cout
    else {	// TODO: delete cout and set back 'return' without else bracket
      LOG(info) << "MVD cut not passed.";
      return false;
   }
  }

  std::string ToString()
  {
    std::stringstream ss;
    ss << "LMVM cuts:" << std::endl
       << "fChiPrimCut = " << fChi2PrimCut << std::endl
       << "fPtCut = " << fPtCut << std::endl
       << "fAngleCut = " << fAngleCut << std::endl
       << "fGammaCut = " << fGammaCut << std::endl
       << "fStCut (ang,pp) = (" << fStCutAngle << "," << fStCutPP << ")" << std::endl
       << "fRtCut (ang,pp) = (" << fRtCutAngle << "," << fRtCutPP << ")" << std::endl
       << "fTtCut (ang,pp) = (" << fTtCutAngle << "," << fTtCutPP << ")" << std::endl
       << "fMvd1Cut (p,d) = (" << fMvd1CutP << "," << fMvd1CutD << ")" << std::endl
       << "fMvd2Cut (p,d) = (" << fMvd2CutP << "," << fMvd2CutD << ")" << std::endl
       << "fMomentumCut = " << fMomentumCut << std::endl;
    return ss.str();
  }

public:
  // ID cuts, we use CbmLitGlobalElectronId for identification
  double fMomentumCut = -1.;  // if fMomentumCut < 0 it is not used

  // Analysis cuts
  double fPtCut       = 0.2;
  double fAngleCut    = 1.;
  double fChi2PrimCut = 3.0;
  double fGammaCut    = 0.025;
  double fStCutAngle  = 2.3;
  double fStCutPP     = 2.9;
  double fTtCutAngle  = 2.2;
  double fTtCutPP     = 3.2;
  double fRtCutAngle  = 2.4;
  double fRtCutPP     = 3.0;
  double fMvd1CutP    = 1.2;
  double fMvd1CutD    = 0.4;
  double fMvd2CutP    = 1.5;
  double fMvd2CutD    = 0.5;
};

#endif
