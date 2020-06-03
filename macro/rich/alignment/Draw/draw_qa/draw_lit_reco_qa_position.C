#include <string>

void draw_lit_reco_qa_position(bool lit, bool reco)
{
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
	loadlibs();

	string dir = "/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Batch_7/";
	string fileName = "", outputDirTracking = "", outputDirRecoQa = "";

	for ( int i=0; i<3; i++ )
//	int i = 6;
	{
		PrepareInputs(i, dir, fileName, outputDirTracking, outputDirRecoQa);
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
}

void PrepareInputs(int Flag, string dir, string& fileName, string& outputDirTracking, string& outputDirRecoQa)
{
	string folder = "";
	if ( Flag == 0 )
	{
		folder = "Aligned/";
		fileName = dir + folder + "merge_align.root";
	}

	else if ( Flag == 1 )
	{
		folder = "Misaligned_1mrad_Full/";
		fileName = dir + folder + "merge_misalign_1mrad.root";
	}

	else if ( Flag == 2 )
	{
		folder = "Misaligned_2mrad_Full/";
		fileName = dir + folder + "merge_misalign_2mrad.root";
	}

	else if ( Flag == 3 )
	{
		folder = "Misaligned_3mrad_XY_Full/";
		fileName = dir + folder + "merge_misalign_3mrad.root";
	}

	else if ( Flag == 4 )
	{
		folder = "Misaligned_4mrad_Full/";
		fileName = dir + folder + "merge_misalign_4mrad.root";
	}

	else if ( Flag == 5 )
	{
		folder = "Misaligned_5mrad_Full/";
		fileName = dir + folder + "merge_misalign_5mrad.root";
	}

	else if ( Flag == 6 )
	{
		folder = "Misaligned_OFF/";
		fileName = dir + folder + "merge_misalign_OFF.root";
	}

	else if { std::cout << "PrepareInputs: Error no valid Flag" << endl; }

	outputDirTracking = dir + folder + "results_litqa_tracking/";
	outputDirRecoQa = dir + folder + "results_RecoQa/";

	std::cout << "fileName, outputDirTracking, outputDirRecoQa:" << std::endl;
	std::cout << fileName << std::endl << outputDirTracking << std::endl << outputDirRecoQa << std::endl;
}
