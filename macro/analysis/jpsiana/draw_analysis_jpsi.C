
void draw_analysis_jpsi()
{
	TStopwatch timer;
	timer.Start();
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
	loadlibs();

	std::string outputDir = "results_analysis/";
	//std::string fileName = "/hera/cbm/users/adrian/data/jun15_25gev/jpsi_urqmd/analysis.auau.25gev.centr.all.root";
        std::string fileName = "/Users/slebedev/Development/cbm/data/jpsi/analysis.0001.root";
         //std::string fileName = "/hera/cbm/users/slebedev/data/jpsi/jun15_25gev/jpsi_urqmd/analysis.auau.25gev.centr.all.root";
	CbmSimulationReport* jpsiReport = new CbmAnaJpsiReport();
	jpsiReport->Create(fileName, outputDir);
}

