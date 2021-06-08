/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * CbmBasicTrackCut.cxx
 *
 *  Created on: 22 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CbmBasicTrackCuts.h"

#include "CbmNHitsCut.h"
#include "CbmTofCut.h"

#include "NicaCutMonitorX.h"
#include "NicaCutMonitorXY.h"
#include "NicaStdString.h"
#include "NicaTrackChargeCut.h"
#include "NicaTrackChi2Cut.h"
#include "NicaTrackDCACut.h"
#include "NicaTrackEtaCut.h"
#include "NicaTrackPCut.h"
#include "NicaTrackPtCut.h"

CbmBasicTrackCuts::CbmBasicTrackCuts()
{
  fMonitorSettingsChi2    = TVector3(20, 0, 10);
  fMonitorsSettingsKin[0] = TVector3(100, -2, 6);
  fMonitorsSettingsKin[1] = TVector3(100, 0, 3);
  fMonitorSettingsTof[0]  = TVector3(100, 0, 2);
  fMonitorSettingsTof[1]  = TVector3(140, -0.2, 1.2);
  fMonitorSettingsNHits   = TVector3(10, 0, 10);
  fMonitorSettingsDCA[0]  = TVector3(100, 0, 10);
  fMonitorSettingsDCA[1]  = TVector3(100, 0, 10);
  AddCut(NicaTrackChargeCut());  //0
  AddCut(CbmNHitsCut());         //1
  AddCut(NicaTrackChi2Cut());    //2
  AddCut(NicaTrackPCut());       //3
  AddCut(NicaTrackPtCut());      //4
  AddCut(NicaTrackEtaCut());     //5
  AddCut(NicaTrackDCACut());     //6
  AddCut(CbmTofCut());           //7
}

NicaTrackChargeCut* CbmBasicTrackCuts::GetChargeCut() const { return (NicaTrackChargeCut*) this->CutAt(0); }

CbmNHitsCut* CbmBasicTrackCuts::GetNHitsCut() const { return (CbmNHitsCut*) this->CutAt(1); }

NicaTrackChi2Cut* CbmBasicTrackCuts::GetChi2Cut() const { return (NicaTrackChi2Cut*) this->CutAt(2); }

NicaTrackPCut* CbmBasicTrackCuts::GetPCut() const { return (NicaTrackPCut*) this->CutAt(3); }

NicaTrackPtCut* CbmBasicTrackCuts::GetPtCut() const { return (NicaTrackPtCut*) this->CutAt(4); }

NicaTrackEtaCut* CbmBasicTrackCuts::GetEtaCut() const { return (NicaTrackEtaCut*) this->CutAt(5); }

NicaTrackDCACut* CbmBasicTrackCuts::GetDCACut() const { return (NicaTrackDCACut*) this->CutAt(6); }

void CbmBasicTrackCuts::CreateBasicMonitors()
{
  TString opt    = "";
  Int_t step     = 0;
  TString params = GetGlobalCutOption();
  if (NicaStd::FindParam(params, "re")) { opt = "re"; }
  if (NicaStd::FindParam(params, "im")) {
    opt  = "im";
    step = 1;
  }
  NicaCutMonitorX nHits(GetNHitsCut()->CutName(opt), step);
  nHits.SetXaxis(fMonitorSettingsNHits.X(), fMonitorSettingsNHits.Y(), fMonitorSettingsNHits.Z());
  AddCutMonitor(nHits);

  NicaCutMonitorX nchi(GetChi2Cut()->CutName(opt), step);
  nchi.SetXaxis(fMonitorSettingsChi2.X(), fMonitorSettingsChi2.Y(), fMonitorSettingsChi2.Z());
  AddCutMonitor(nchi);

  NicaCutMonitorXY kinM(GetEtaCut()->CutName(opt), step, GetPtCut()->CutName(opt), step);
  kinM.SetXaxis(fMonitorsSettingsKin[0].X(), fMonitorsSettingsKin[0].Y(), fMonitorsSettingsKin[0].Z());
  kinM.SetYaxis(fMonitorsSettingsKin[1].X(), fMonitorsSettingsKin[1].Y(), fMonitorsSettingsKin[1].Z());
  AddCutMonitor(kinM);

  NicaCutMonitorXY tofM(GetPCut()->CutName(opt), step, GetTofCut()->CutName(opt), step + GetTofCut()->M2());
  tofM.SetXaxis(fMonitorSettingsTof[0].X(), fMonitorSettingsTof[0].Y(), fMonitorSettingsTof[0].Z());
  tofM.SetYaxis(fMonitorSettingsTof[1].X(), fMonitorSettingsTof[1].Y(), fMonitorSettingsTof[1].Z());
  AddCutMonitor(tofM);

  NicaCutMonitorXY dcaM(GetDCACut()->CutName(opt), step + GetDCACut()->DCAz(), GetDCACut()->CutName(opt),
                        step + GetDCACut()->DCAxy());
  dcaM.SetXaxis(fMonitorSettingsDCA[0].X(), fMonitorSettingsDCA[0].Y(), fMonitorSettingsDCA[0].Z());
  dcaM.SetYaxis(fMonitorSettingsDCA[1].X(), fMonitorSettingsDCA[1].Y(), fMonitorSettingsDCA[1].Z());
  AddCutMonitor(dcaM);
}

void CbmBasicTrackCuts::SetCharge(Int_t charge) { GetChargeCut()->SetMinAndMax(charge); }

void CbmBasicTrackCuts::SetNHits(Int_t min, Int_t max) { GetNHitsCut()->SetMinMax(min, max); }

void CbmBasicTrackCuts::SetPt(Double_t min, Double_t max) { GetPtCut()->SetMinMax(min, max); }

void CbmBasicTrackCuts::SetEta(Double_t min, Double_t max) { GetEtaCut()->SetMinMax(min, max); }

void CbmBasicTrackCuts::SetM2(Double_t min, Double_t max) { GetTofCut()->SetMinMax(min, max); }

void CbmBasicTrackCuts::SetDCAXY(Double_t min, Double_t max) { GetDCACut()->SetMinMax(min, max, GetDCACut()->DCAxy()); }

void CbmBasicTrackCuts::SetDCAZ(Double_t min, Double_t max) { GetDCACut()->SetMinMax(min, max, GetDCACut()->DCAz()); }

void CbmBasicTrackCuts::SetTofMonitorPAxis(Int_t nbins, Double_t min, Double_t max)
{
  fMonitorSettingsTof[0].SetXYZ(nbins, min, max);
}

void CbmBasicTrackCuts::SetTofMonitorM2Axis(Int_t nbins, Double_t min, Double_t max)
{
  fMonitorSettingsTof[1].SetXYZ(nbins, min, max);
}

void CbmBasicTrackCuts::SetPtEtaMonitorPtAxis(Int_t nbins, Double_t min, Double_t max)
{
  fMonitorsSettingsKin[1].SetXYZ(nbins, min, max);
}

void CbmBasicTrackCuts::SetPtEtaMonitorEtaAxis(Int_t nbins, Double_t min, Double_t max)
{
  fMonitorsSettingsKin[0].SetXYZ(nbins, min, max);
}

void CbmBasicTrackCuts::SetNHitsMonitorAxis(Int_t nbins, Double_t min, Double_t max)
{
  fMonitorSettingsNHits.SetXYZ(nbins, min, max);
}

void CbmBasicTrackCuts::SetChi2MonitorAxis(Int_t nbins, Double_t min, Double_t max)
{
  fMonitorSettingsChi2.SetXYZ(nbins, min, max);
}

void CbmBasicTrackCuts::SetDCAMonitorZAxis(Int_t nbins, Double_t min, Double_t max)
{
  fMonitorSettingsDCA[0].SetXYZ(nbins, min, max);
}

void CbmBasicTrackCuts::SetDCAMonitorXYAxis(Int_t nbins, Double_t min, Double_t max)
{
  fMonitorSettingsDCA[1].SetXYZ(nbins, min, max);
}

CbmTofCut* CbmBasicTrackCuts::GetTofCut() const { return (CbmTofCut*) this->CutAt(7); }

void CbmBasicTrackCuts::AcceptOnlyWithTofHit(Bool_t val)
{
  if (val) GetTofCut()->AcceptTracksOnlyWithToF();
  else
    GetTofCut()->SetMinMax(0, 1, 1);
}

CbmBasicTrackCuts::~CbmBasicTrackCuts() {}
