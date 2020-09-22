void draw_litqa() {
  //gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
  //loadlibs();

  std::string outputDir = "rich_carb/results_litqa_ac/";
  //std::string fileName = "/Users/slebedev/Development/cbm/data/simulations/rich/richreco/25gev.reco.0005.root";
  std::string fileName = "qa.all.ac.root";

  CbmSimulationReport* trackingQaReport = new CbmLitTrackingQaReport();
  trackingQaReport->Create(fileName, outputDir + "/tracking/");

  // CbmSimulationReport* fitQaReport = new CbmLitFitQaReport();
  // fitQaReport->Create(fileName, outputDir + "/fit/");

  CbmSimulationReport* clusteringQaReport = new CbmLitClusteringQaReport();
  clusteringQaReport->Create(fileName, outputDir + "clustering/");

  //   CbmLitRadLengthQaReport* radLengthQaReport = new CbmLitRadLengthQaReport();
  //   radLengthQaReport->Create(fileName, outputDir);

  //   CbmSimulationReport* tofQaReport = new CbmLitTofQaReport();
  //   tofQaReport->Create(fileName, outputDir);
}
