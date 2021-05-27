void draw_litqa(const string& histRootFile = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/litqa.all.root",
                const string& resultDir    = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/histograms/inmed/")

{
  string outputDirTracking = resultDir + "results_litqa_tracking/";
  gSystem->mkdir(outputDirTracking.c_str(), true);

  CbmSimulationReport* trackingQaReport = new CbmLitTrackingQaReport();
  trackingQaReport->Create(histRootFile, outputDirTracking);

  //   string outputDirClustering = resultDir + "results_litqa_clustering/";
  //   gSystem->mkdir(outputDirClustering.c_str(), true);

  //   CbmSimulationReport* clusteringQaReport = new CbmLitClusteringQaReport();
  //   clusteringQaReport->Create(fileName, outputDirClustering);

  //   CbmSimulationReport* fitQaReport = new CbmLitFitQaReport();
  //   fitQaReport->Create(fileName, outputDir);

  //   CbmLitRadLengthQaReport* radLengthQaReport = new CbmLitRadLengthQaReport();
  //   radLengthQaReport->Create(fileName, outputDir);

  //   CbmSimulationReport* tofQaReport = new CbmLitTofQaReport();
  //   tofQaReport->Create(fileName, outputDir);
}
