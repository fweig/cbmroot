#include <string>

void draw_lit_reco_qa_matching(bool lit, bool reco)
{
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
	loadlibs();

	string dir = "/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/";
	string fileName = "", outputDirTracking = "", outputDirRecoQa = "";

	for ( int j=1; j<6; j++ )
//	int j=1;
	{
		PrepareInputs(j, dir, fileName, outputDirTracking, outputDirRecoQa);
		gSystem->mkdir(outputDirTracking.c_str(), true);

		if ( lit == 1 )
		{
	               CbmSimulationReport* trackingQaReport = new CbmLitTrackingQaReport();
	               trackingQaReport->Create(fileName, outputDirTracking);
		}

                if ( reco == 1 )
		{
			CbmRichRecoQa* recoQa = new CbmRichRecoQa();
	                recoQa->DrawFromFile(fileName, outputDirRecoQa);
		}
	}
}

void PrepareInputs(int Flag, string dir, string& fileName, string& outputDirTracking, string& outputDirRecoQa)
{
	string folder = "";
	if ( Flag == 0 )
	{
		folder = "Reference/";
		fileName = dir + folder + "merge_reference.root";
	}

	else if ( Flag == 1 )
	{
		folder = "WO_Corrections_1mrad/";
		fileName = dir + folder + "merge_1mrad.root";
	}

	else if ( Flag == 2 )
	{
		folder = "WO_Corrections_2mrad/";
		fileName = dir + folder + "merge_2mrad.root";
	}

	else if ( Flag == 3 )
	{
		folder = "WO_Corrections_3mrad/";
		fileName = dir + folder + "merge_3mrad.root";
	}

	else if ( Flag == 4 )
	{
		folder = "WO_Corrections_4mrad/";
		fileName = dir + folder + "merge_4mrad.root";
	}

	else if ( Flag == 5 )
	{
		folder = "WO_Corrections_5mrad/";
		fileName = dir + folder + "merge_5mrad.root";
	}

	else if ( Flag == 6 )
	{
		folder = "W_Corrections/";
		fileName = dir + folder + "merge_corrected.root";
	}

	else if { std::cout << "PrepareInputs: Error no valid Flag" << endl; }

	outputDirTracking = dir + folder + "results_litqa_tracking/";
	outputDirRecoQa = dir + folder + "results_RecoQa/";

	std::cout << "fileName, outputDirTracking, outputDirRecoQa:" << std::endl;
	std::cout << fileName << std::endl << outputDirTracking << std::endl << outputDirRecoQa << std::endl;
}
