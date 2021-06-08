/* Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jordan Bendarouach [committer] */

#include <string>

void draw_lit_reco_qa(bool lit, bool reco)
{
  //	string dir = "/data/Sim_Outputs/Sim_Thesis/AuAu_10AGeV/Aligned/";
  string dir      = "/data/Sim_Outputs/Sim_Thesis/AuAu_10AGeV/Misaligned_3mrad/";
  string fileName = "", outputDirTracking = "", outputDirRecoQa = "";

  fileName          = dir + "merge_param.root";
  outputDirTracking = dir + "Corrected/results_litqa_tracking/";
  outputDirRecoQa   = dir + "Corrected/results_RecoQa/";

  std::cout << "fileName, outputDirTracking, outputDirRecoQa:" << std::endl;
  std::cout << fileName << std::endl << outputDirTracking << std::endl << outputDirRecoQa << std::endl;

  if (lit == 1) {
    CbmSimulationReport* trackingQaReport = new CbmLitTrackingQaReport();
    trackingQaReport->Create(fileName, outputDirTracking);
  }
  if (reco == 1) {
    CbmRichRecoQa* recoQa = new CbmRichRecoQa();
    recoQa->DrawFromFile(fileName, outputDirRecoQa);
  }
}
