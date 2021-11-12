/* Copyright (C) 2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

#ifndef LMVM_DEF_H
#define LMVM_DEF_H

#include "Rtypes.h"

#include <string>

class TH1D;

enum class ELmvmTopologyCut : int
{
  ST,
  RT,
  TT
};


enum class ELmvmSrc : int
{
  Signal    = 0,
  Bg        = 1,
  Pi0       = 2,
  Gamma     = 3,
  Eta       = 4,
  Undefined = 5
};


enum class ELmvmAnaStep : int
{
  Mc        = 0,
  Acc       = 1,
  Reco      = 2,
  Chi2Prim  = 3,
  ElId      = 4,
  GammaCut  = 5,
  Mvd1Cut   = 6,
  Mvd2Cut   = 7,
  StCut     = 8,
  RtCut     = 9,
  TtCut     = 10,
  PtCut     = 11,
  Undefined = 12
};

enum class ELmvmBgPairSrc : int
{
  GG        = 0,  // gamma-gamma
  PP        = 1,  // pi0-pi0
  OO        = 2,  // other-other
  GP        = 3,  // gamma-pi0
  GO        = 4,  // gamma-other
  PO        = 5,  // pi0-other
  Undefined = 6
};

enum class ELmvmSignal : int
{
  Inmed  = 0,
  Qgp    = 1,
  Omega  = 2,
  Phi    = 3,
  OmegaD = 4
};

class LmvmDataXYInd {
public:
  LmvmDataXYInd() {}
  LmvmDataXYInd(double x, double y, int ind) : fX(x), fY(y), fInd(ind) {}
  double fX = 0.;
  double fY = 0.;
  int fInd  = 0;
};

class LmvmDataAngMomInd {
public:
  LmvmDataAngMomInd() {}
  LmvmDataAngMomInd(double angle, double mom, int ind) : fAngle(angle), fMom(mom), fInd(ind) {}
  double fAngle = 0.;
  double fMom   = 0.;
  int fInd      = 0;
};

class LmvmDrawMinvData {
public:
  LmvmDrawMinvData() {}
  LmvmDrawMinvData(TH1D* h, Color_t fillColor, Color_t lineColor, int lineWidth, Style_t fillStyle,
                   const std::string& legend)
    : fH(h)
    , fFillColor(fillColor)
    , fLineColor(lineColor)
    , fLineWidth(lineWidth)
    , fFillStyle(fillStyle)
    , fLegend(legend)
  {
  }

  TH1D* fH            = nullptr;
  Color_t fFillColor  = 0;
  Color_t fLineColor  = 0;
  int fLineWidth      = 0;
  Style_t fFillStyle  = -1;
  std::string fLegend = "";
};

class LmvmSBgResultData {
public:
  LmvmSBgResultData() {}
  LmvmSBgResultData(double sBgRatio, double signallEff, double fitMean, double fitSigma)
    : fSBgRatio(sBgRatio)
    , fSignallEff(signallEff)
    , fFitMean(fitMean)
    , fFitSigma(fitSigma)
  {
  }
  double fSBgRatio   = 0.;
  double fSignallEff = 0.;
  double fFitMean    = 0.;
  double fFitSigma   = 0;
};


#endif
