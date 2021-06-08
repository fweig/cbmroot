/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * CbmBasicFemtoPairCut.cxx
 *
 *  Created on: 22 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmBasicFemtoPairCut.h"

#include "CbmDeltaPhiDeltaThetaCut.h"
#include "CbmStsExitSepCut.h"

#include "NicaCutMonitorX.h"
#include "NicaCutMonitorXY.h"

CbmBasicFemtoPairCut::CbmBasicFemtoPairCut()
{
  AddCut(CbmStsExitSepCut());
  AddCut(CbmDeltaPhiDeltaThetaStarCut());
  SetSeparationMonitorAxis(100, 0, 10);
  SetDeltaPhiStarAxis(100, -0.1, 0.1);
  SetDeltaEtaStarAxis(100, -0.1, 0.1);
}

void CbmBasicFemtoPairCut::SetDeltaPhiStarCut(Double_t min, Double_t max)
{
  GetDeltaPhiEtaStarCut()->SetMinMax(min, max, 0);
}

void CbmBasicFemtoPairCut::SetDeltaEtaStarCut(Double_t min, Double_t max)
{
  GetDeltaPhiEtaStarCut()->SetMinMax(min, max, 1);
}

void CbmBasicFemtoPairCut::SetR(Double_t R) { GetDeltaPhiEtaStarCut()->SetR(R); }

void CbmBasicFemtoPairCut::SetSTSExitSeparationCut(Double_t min, Double_t max) { GetStsExitCut()->SetMinMax(min, max); }

void CbmBasicFemtoPairCut::CreateBasicMonitors()
{
  TString opt    = "";
  Int_t step     = 0;
  TString params = GetGlobalCutOption();
  if (NicaStd::FindParam(params, "re")) { opt = "re"; }
  if (NicaStd::FindParam(params, "im")) {
    opt  = "im";
    step = 1;
  }
  NicaCutMonitorX exitM(GetStsExitCut()->CutName(opt), step);
  exitM.SetXaxis(fExitSeparation.X(), fExitSeparation.Y(), fExitSeparation.Z());
  AddCutMonitor(exitM);
  NicaCutMonitorXY phiM(GetDeltaPhiEtaStarCut()->CutName(opt), 0 + step, GetDeltaPhiEtaStarCut()->CutName(opt),
                        1 + step);
  phiM.SetXaxis(fMonitorPhiPair[0].X(), fMonitorPhiPair[0].Y(), fMonitorPhiPair[0].Z());
  phiM.SetYaxis(fMonitorPhiPair[1].X(), fMonitorPhiPair[1].Y(), fMonitorPhiPair[1].Z());
  AddCutMonitor(phiM);
}

CbmBasicFemtoPairCut::~CbmBasicFemtoPairCut() {}
