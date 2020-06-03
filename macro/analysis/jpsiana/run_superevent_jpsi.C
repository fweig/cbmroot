#include "Riostream.h"

void run_superevent_jpsi()
{
	TStopwatch timer;
	timer.Start();
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
	loadlibs();

	std::string outputDir ="results_analysis/";
	//std::string fileName ="/data/CbmRoot/sim_data/analysis.0001.root";

	std::string inputDir = "/data/CbmRoot/sim_data/";
    std::string fileArray = "analysis."; //"analysis.auau.25gev.centr.";

    Int_t nofFiles = 1;

	//std::string fileName ="/hera/cbm/users/adrian/data/jun15_25gev/jpsi_urqmd/analysis.auau.25gev.centr.all.root";
	//std::string fileName ="/Users/slebedev/Development/cbm/data/jpsi/analysis.0001.root";

	CbmAnaJpsiSuperEvent* se = new CbmAnaJpsiSuperEvent();
	se->SetRunAfterPtCut(false);

    //se->SetOutputFile("/Users/slebedev/Development/cbm/data/jpsi/analysis.superevent.root");
    se->SetOutputFile("/data/CbmRoot/sim_data/analysis.superevent.root");

    for (int i=1;i<=nofFiles;i++)
	{
    	stringstream ss;
    	ss.fill('0');
    	ss.width(4);
    	ss  << i << ".root";
    	se->AddFile(inputDir + fileArray + ss.str().c_str() );
	}




	se->Run();
}

