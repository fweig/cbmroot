#include <string>

void draw_lit_reco_qa_position_2(bool lit, bool reco)
{
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
	loadlibs();

//	string dir = "/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Batch_3/";	// e- only
	string dir = "/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Batch_4/";	// e+ only
//	string dir = "/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Batch_5/";	// e- and e+
	string fileName = "", outputDirTracking = "", outputDirRecoQa = "";

	for ( int i = 0; i<9; i++ )
//	int i = 0;
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
		folder = "Misaligned_3mrad_minusX_Full/";
		fileName = dir + folder + "merge_3mrad_minusX.root";
	}

	else if ( Flag == 2 )
	{
		folder = "Misaligned_3mrad_minusX_minusY_Full/";
		fileName = dir + folder + "merge_3mrad_minusX_minusY.root";
	}

	else if ( Flag == 3 )
	{
		folder = "Misaligned_3mrad_minusX_plusY_Full/";
		fileName = dir + folder + "merge_3mrad_minusX_plusY.root";
	}

	else if ( Flag == 4 )
	{
		folder = "Misaligned_3mrad_minusY_Full/";
		fileName = dir + folder + "merge_3mrad_minusY.root";
	}

	else if ( Flag == 5 )
	{
		folder = "Misaligned_3mrad_plusX_Full/";
		fileName = dir + folder + "merge_3mrad_plusX.root";
	}

	else if ( Flag == 6 )
	{
		folder = "Misaligned_3mrad_plusX_minusY_Full/";
		fileName = dir + folder + "merge_3mrad_plusX_minusY.root";
	}

	else if ( Flag == 7 )
	{
		folder = "Misaligned_3mrad_plusX_plusY_Full/";
		fileName = dir + folder + "merge_3mrad_plusX_plusY.root";
	}

	else if ( Flag == 8 )
	{
		folder = "Misaligned_3mrad_plusY_Full/";
		fileName = dir + folder + "merge_3mrad_plusY.root";
	}

	else if { std::cout << "PrepareInputs: Error no valid Flag" << endl; }

	outputDirTracking = dir + folder + "results_litqa_tracking/";
	outputDirRecoQa = dir + folder + "results_RecoQa/";

	std::cout << "fileName, outputDirTracking, outputDirRecoQa:" << std::endl;
	std::cout << fileName << std::endl << outputDirTracking << std::endl << outputDirRecoQa << std::endl;
}
