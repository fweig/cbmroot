/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * hbt_anatree.C
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
/**
 * example macro for pi-pi analysis
 */
#ifndef __CLING__
#include "CbmAnaTreeSource.h"
#include "CbmBasicFemtoPairCut.h"
#include "CbmBasicTrackCuts.h"
#include "CbmDeltaPhiDeltaThetaCut.h"
#include "CbmDetectorID.h"
#include "CbmFieldMap.h"
#include "CbmHasTofTrackCut.h"
#include "CbmHbtFullEvent.h"
#include "CbmStsExitSepCut.h"
#include "CbmStsTrackHitsCut.h"
#include "CbmTofTrackCuts.h"
#include "data/CbmDefs.h"

#include "FairFileSource.h"
#include "FairParRootFileIo.h"
#include <FairLogger.h>
#include <FairRunAna.h>

#include "TFile.h"
#include <RtypesCore.h>
#include <TStopwatch.h>
#include <TString.h>
#include <TTimer.h>

#include <iostream>

#include "NicaCbmFirstTask.h"
#include "NicaCbmFullEvent.h"
#include "NicaCbmGlobalMCEvent.h"
#include "NicaCbmMCEvent.h"
#include "NicaCut.h"
#include "NicaCutMonitorX.h"
#include "NicaCutMonitorXY.h"
#include "NicaCutMonitorXYZ.h"
#include "NicaEventComplexCut.h"
#include "NicaEventImpactParameterCut.h"
#include "NicaEventPhiCut.h"
#include "NicaEventVertexCut.h"
#include "NicaEventVirtualCut.h"
#include "NicaFemto1DCF.h"
#include "NicaFemtoBasicAna.h"
#include "NicaFemtoCorrFuncKt.h"
#include "NicaFemtoPairKinematics.h"
#include "NicaFemtoSourceModelGausMt.h"
#include "NicaFemtoWeightGenerator.h"
#include "NicaFemtoWeightGeneratorSupressed.h"
#include "NicaPdgBinPairCut.h"
#include "NicaStd.h"
#include "NicaTrackAna.h"
#include "NicaTrackBasicMCCut.h"
#include "NicaTrackBoostedMomentaCuts.h"
#include "NicaTrackChargeCut.h"
#include "NicaTrackDCACut.h"
#include "NicaTrackDeltaMomCut.h"
#include "NicaTrackEtaCut.h"
#include "NicaTrackOnlyPrimariesCut.h"
#include "NicaTrackPCut.h"
#include "NicaTrackPdgCut.h"
#include "NicaTrackPtCut.h"
#include "NicaTrackPzCut.h"
#include "NicaTwoTrackFemtoQinvCut.h"
using namespace std;
#endif

/**
 * macro for calcution of final CF
 */

#define KRONOS

void SetEventCut(NicaEventAna* task)
{
  NicaEventVertexCut vtx;
  task->AddCut(vtx, "im");
  NicaCutMonitorXY vtxM(vtx.CutName("im"), vtx.Z(), vtx.CutName("im"), vtx.Rt());
  vtxM.SetXaxis(100, 0, 1);
  vtxM.SetYaxis(200, 0, 2);
  task->AddCutMonitor(vtxM);
  NicaEventImpactParameterCut b;
  NicaEventPhiCut phi;
  NicaCutMonitorX phiM(phi.CutName("im"), 0);
  phiM.SetXaxis(100, -TMath::TwoPi(), TMath::TwoPi());
  NicaCutMonitorX bM(b.CutName("im"), 0);
  bM.SetXaxis(100, 0, 15);
  task->AddCut(b, "im");
  task->AddCut(phi, "im");
  task->AddCutMonitor(phiM);
  task->AddCutMonitor(bM);
}
void SetTrackCuts(NicaEventAna* task, Bool_t mc = kTRUE)
{
  CbmBasicTrackCuts basic;
  basic.SetCharge(-1);
  basic.SetM2(-0.1, 0.1);
  basic.SetPt(0.1, 10);
  basic.SetOptionForAllCuts("re");
  basic.CreateBasicMonitors();  // must be called after SetOptionForAllCuts
  task->AddCutsAndMonitors(basic);
  NicaTrackPdgCut pid;
  pid.SetMinAndMax(-211);
  if (mc) { task->AddCut(pid, "im"); }
}

void SetPairCuts(NicaTwoTrackAna* task)
{
  CbmBasicFemtoPairCut combinedCut;
  combinedCut.SetR(25);
  combinedCut.SetDeltaPhiStarCut(-0.01, 0.01);
  combinedCut.SetDeltaEtaStarCut(-0.01, 0.01);
  combinedCut.SetDeltaEtaStarAxis(100, -0.1, 0.1);
  combinedCut.SetDeltaPhiStarAxis(100, -0.1, 0.1);
  combinedCut.SetOptionForAllCuts("re");
  combinedCut.CreateBasicMonitors();
  task->AddCutsAndMonitors(combinedCut);
}

void hbt_anatree(TString inFile = "/media/daniel/WD/anatree/1.analysistree.root", TString outFile = "test2.root")
{
  FairRunAna* ana = new FairRunAna();
  //  inFile = "/home/daniel/temp/00001.mini.root";
  FairSource* source = new CbmAnaTreeSource(inFile);
  ana->SetSource(source);
  ana->SetOutputFile(outFile);

  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  FairLogger* log     = FairLogger::GetLogger();
  log->SetColoredLog(kTRUE);
  log->SetLogVerbosityLevel("high");

  NicaFemtoBasicAna* task = new NicaFemtoBasicAna();
  task->SetCorrFctn(NicaFemtoCorrFuncKt(NicaFemto1DCF("cf", 100, 0, 1, ENicaFemtoKinematics::kLCMS), {0, 10}));
  task->SetOption(NicaTwoTrackAna::BackgroundOptionMixed());
  task->SetWeight(NicaFemtoWeightGenerator());
  task->SetPdg(211);
  task->SetMixSize(5);
#ifdef SPEEDUP
  /**
   * speed up analysis by using compression and special HBT format
   * the sts exit distance and phi* are calculated once per track
   */
  task->SetFormat(new NicaCbmFullEvent(), ENicaFormatDepth::kNonBuffered);
  task->SetFormat(new CbmHbtFullEvent(), ENicaFormatDepth::kBuffered);
  task->SetFormatOption(NicaEventAna::EFormatOption::kCompress);
#else
  task->SetFormat(new NicaCbmFullEvent());
#endif
  SetEventCut(task);
  SetTrackCuts(task);
  SetPairCuts(task);
  NicaCbmFirstTask* first_task = new NicaCbmFirstTask();
  CbmFieldMap* field           = new CbmFieldMap("field_v18a.root");
  first_task->SetMagField(field);
  ana->AddTask(first_task);
  ana->AddTask(task);

  TStopwatch timer;
  timer.Start();

  ana->Init();
  ana->Run(0, 1000);
  Double_t rtime = timer.RealTime(), ctime = timer.CpuTime();
  printf("RealTime=%f seconds, CpuTime=%f seconds\n", rtime, ctime);
}
