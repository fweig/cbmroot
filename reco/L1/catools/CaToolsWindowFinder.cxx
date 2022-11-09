/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsWindowFinder.cxx
/// \brief  Framework for CA tracking hit-finder window estimation from MC (implementation)
/// \since  14.10.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CaToolsWindowFinder.h"

#include <Logger.h>

#include "TChain.h"

#include "L1Constants.h"

using namespace ca::tools;
using namespace L1Constants::clrs;  // for colored logs

ClassImp(ca::tools::WindowFinder);

// ---------------------------------------------------------------------------------------------------------------------
//
WindowFinder::WindowFinder() : fpMcTripletsTree(new TChain(kTreeName))
{
  LOG(info) << kGNb << "Call" << kCLb << "WindowFinder constructor\n" << kCL;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::AddInputFile(const char* filename)
{
  assert(fpMcTripletsTree);
  auto addingRes = fpMcTripletsTree->AddFile(filename);
  if (addingRes != 1) {
    LOG(error) << "WindowFinder::AddInputFile: File \"" << filename << "\" cannot be added to the MC triplets chain, "
               << "some errors occurred\n";
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::Process(EBuildingMode) { fpMcTripletsTree->Draw("zv"); }


// ****************************
// ** Setters implementation **
// ****************************

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::SetBinning(int nBinsX, int nBinsY)
{
  assert(nBinsX > 0);
  assert(nBinsY > 0);
  fNbinsX = nBinsX;
  fNbinsY = nBinsY;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::SetEpsilon(float eps)
{
  assert(eps > 0. && eps <= 1.);
  fEps = eps;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::SetNslices(int nSlices)
{
  assert(nSlices > 0);
  fNslices = nSlices;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void WindowFinder::SetTarget(double x, double y, double z)
{
  fTargetPos[0] = x;
  fTargetPos[1] = y;
  fTargetPos[2] = z;
}
