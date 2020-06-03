void draw_litqa()
{
   std::string dir = "/Users/slebedev/Development/cbm/data/lmvm/may18_100k/8gev/omegaepem/";
   std::string fileName = dir + "litqa.auau.8gev.centr.all.root";

   std::string script = std::string(TString(gSystem->Getenv("SCRIPT")).Data());
    if (script == "yes"){
      dir = std::string(TString(gSystem->Getenv("LMVM_MAIN_DIR")).Data());
      fileName = dir + std::string(TString(gSystem->Getenv("LMVM_LITQA_FILE_NAME")).Data());
   }

   std::string outputDirTracking = dir + "results_litqa_tracking/";
   std::string outputDirClustering = dir + "results_litqa_clustering/";
   gSystem->mkdir(outputDirTracking.c_str(), true);
   gSystem->mkdir(outputDirClustering.c_str(), true);

   CbmSimulationReport* trackingQaReport = new CbmLitTrackingQaReport();
   trackingQaReport->Create(fileName, outputDirTracking);

 //  CbmSimulationReport* clusteringQaReport = new CbmLitClusteringQaReport();
  // clusteringQaReport->Create(fileName, outputDirClustering);

//   CbmSimulationReport* fitQaReport = new CbmLitFitQaReport();
//   fitQaReport->Create(fileName, outputDir);

//   CbmLitRadLengthQaReport* radLengthQaReport = new CbmLitRadLengthQaReport();
//   radLengthQaReport->Create(fileName, outputDir);

//   CbmSimulationReport* tofQaReport = new CbmLitTofQaReport();
//   tofQaReport->Create(fileName, outputDir);
}
