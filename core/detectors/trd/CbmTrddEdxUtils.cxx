/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmTrddEdxUtils.h"

#include <RtypesCore.h>

#include <iostream>  // for cout

#include <cmath>  // for Sqrt/Pow/Log

CbmTrddEdxUtils::CbmTrddEdxUtils() {}
CbmTrddEdxUtils::~CbmTrddEdxUtils() {}

// ---- GetMipNormedBB ----
Double_t CbmTrddEdxUtils::GetMipNormedBB(Double_t betaGamma)
{
  const Double_t beta = betaGamma / std::sqrt(1. + betaGamma * betaGamma);

  Double_t nominator = (4.4 - std::pow(beta, 2.26) - std::log(0.004 + 1 / std::pow(betaGamma, 0.95)));

  Double_t denominator = std::pow(beta, 2.26);

  Double_t bb = 0.2 * nominator / denominator;

  return bb;
}

ClassImp(CbmTrddEdxUtils)
