#include <string>

void draw_lit_reco_qa(bool lit, bool reco)
{
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
	loadlibs();

	string dir1 = "/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/";
	string fileName = "", outputDirTracking = "", outputDirRecoQa = "";

	for ( int i = 1; i<3; i++ )
//	int i = 2;
	{
		PrepareInputs(i, dir1, fileName, outputDirTracking, outputDirRecoQa);
		gSystem->mkdir(outputDirTracking.c_str(), true);

		if ( lit == 1 )
		{
			CbmSimulationReport* trackingQaReport = new CbmLitTrackingQaReport();
			trackingQaReport->Create(fileName, outputDirTracking);
		}

		if (reco == 1 )
		{
			CbmRichRecoQa* recoQa = new CbmRichRecoQa();
			recoQa->DrawFromFile(fileName, outputDirRecoQa);
		}
	}

/*	string dir2 = "/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Matching/";
	for ( int j = 4; j<6; j++ )
	{
		PrepareInputs(j, dir2, fileName, outputDirTracking, outputDirRecoQa);
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
*/
}

void PrepareInputs(int Flag, string dir, string& fileName, string& outputDirTracking, string& outputDirRecoQa)
{
	string folder = "";
	if ( Flag == 0 )
	{
		folder = "Aligned/";
	}

	else if ( Flag == 1 )
	{
		folder = "Misaligned_1mrad_Full/";
	}

	else if ( Flag == 2 )
	{
		folder = "Misaligned_5mrad_Full/";
	}

	else if ( Flag == 3 )
	{
		folder = "Misaligned_Single_Tile/";
	}

	else if ( Flag == 4 )
	{
		folder = "Reference/";
	}

	else if ( Flag == 5 )
	{
		folder = "WO_Corrections/";
	}

	else if ( Flag == 6 )
	{
		folder = "W_Corrections/";
	}

	else if { std::cout << "PrepareInputs: Error no valid Flag" << endl; }

	fileName = dir + folder + "merge.root";
	outputDirTracking = dir + folder + "results_litqa_tracking/";
	outputDirRecoQa = dir + folder + "results_RecoQa/";

	std::cout << "fileName, outputDirTracking, outputDirRecoQa:" << std::endl;
	std::cout << fileName << std::endl << outputDirTracking << std::endl << outputDirRecoQa << std::endl;
}
