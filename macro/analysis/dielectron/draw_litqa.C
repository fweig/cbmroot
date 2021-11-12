/* Copyright (C) 2014-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Semen Lebedev */

void draw_litqa(const string& histRootFile = "/Users/slebedev/Development/cbm/data/lmvm/nov21/litqa.phi100k.all.root",
                const string& resultDir    = "/Users/slebedev/Development/cbm/data/lmvm/nov21/litqa/")

{
  string outputDirTracking = resultDir + "/tracking/";
  gSystem->mkdir(outputDirTracking.c_str(), true);

  CbmSimulationReport* trackingQaReport = new CbmLitTrackingQaReport();
  trackingQaReport->Create(histRootFile, outputDirTracking);

  string outputDirClustering = resultDir + "/clustering/";
  gSystem->mkdir(outputDirClustering.c_str(), true);

  CbmSimulationReport* clusteringQaReport = new CbmLitClusteringQaReport();
  clusteringQaReport->Create(histRootFile, outputDirClustering);

  //   CbmSimulationReport* fitQaReport = new CbmLitFitQaReport();
  //   fitQaReport->Create(fileName, outputDir);

  //   CbmLitRadLengthQaReport* radLengthQaReport = new CbmLitRadLengthQaReport();
  //   radLengthQaReport->Create(fileName, outputDir);

  //   CbmSimulationReport* tofQaReport = new CbmLitTofQaReport();
  //   tofQaReport->Create(fileName, outputDir);
}
