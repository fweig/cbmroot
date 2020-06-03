#include <string>

void draw_lit_reco_qa_Jan2019_8AGeV(bool lit, bool reco)
{
//	string dir = "/data/Sim_Outputs/Sim_Thesis/AuAu_8AGeV_Jan2019/Aligned/";
//	string dir = "/data/Sim_Outputs/Sim_Thesis/AuAu_8AGeV_Jan2019/1mrad/";
//	string dir = "/data/Sim_Outputs/Sim_Thesis/AuAu_8AGeV_Jan2019/2mrad/";
//	string dir = "/data/Sim_Outputs/Sim_Thesis/AuAu_8AGeV_Jan2019/3mrad/";
	string dir = "/data/Sim_Outputs/Sim_Thesis/AuAu_8AGeV_Jan2019/5mrad/";
	string fileName = "", outputDirTracking = "", outputDirRecoQa = "";

	fileName = dir + "param_merge.root";
        outputDirTracking = dir + "results_litqa_tracking/";
        outputDirRecoQa = dir + "results_RecoQa/";
//	fileName = dir + "Corrected/merge_param.root";
//      outputDirTracking = dir + "Corrected/results_litqa_tracking/";
//      outputDirRecoQa = dir + "Corrected/results_RecoQa/";

        std::cout << "fileName, outputDirTracking, outputDirRecoQa:" << std::endl;
        std::cout << fileName << std::endl << outputDirTracking << std::endl << outputDirRecoQa << std::endl;

	if ( lit == 1 ) {
		CbmSimulationReport* trackingQaReport = new CbmLitTrackingQaReport();
		trackingQaReport->Create(fileName, outputDirTracking);
	}
	if (reco == 1 ) {
		CbmRichRecoQa* recoQa = new CbmRichRecoQa();
		recoQa->DrawFromFile(fileName, outputDirRecoQa);
	}

}
