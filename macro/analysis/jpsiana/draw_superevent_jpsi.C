
void draw_superevent_jpsi()
{
	TStopwatch timer;
	timer.Start();
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
	loadlibs();

	std::string outputDir = "results_superevent/";
    std::string fileNameEventByEvent = "/Users/slebedev/Development/cbm/data/jpsi/analysis.0001.root";
    std::string fileNameSuperEvent = "/Users/slebedev/Development/cbm/data/jpsi/analysis.superevent.root";
    CbmAnaJpsiSuperEventReport* jpsiReport = new CbmAnaJpsiSuperEventReport();
	jpsiReport->Create(fileNameEventByEvent, fileNameSuperEvent, outputDir);
}

