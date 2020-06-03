
void draw_analysis()
{
	TStopwatch timer;
	timer.Start();
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
	loadlibs();

	std::string outputDir = "recoA/";
	std::string runTitle = "run_title_a";
	std::string histFileName = "results/te14325234809.hld.root";

	TString script = TString(gSystem->Getenv("SCRIPT"));

	if (script == "yes") {
	   histFileName= TString(gSystem->Getenv("OUTPUT_ROOT_FILE_ALL"));
	   outputDir = TString(gSystem->Getenv("OUTPUT_DIR"));
	   runTitle = TString(gSystem->Getenv("RUN_TITLE"));
	}

	CbmRichTrbRecoQa* qaReco = new CbmRichTrbRecoQa();
	qaReco->SetOutputDir(outputDir);
	qaReco->SetRunTitle(runTitle);
	qaReco->SetDrawHist(true);
	qaReco->SetMaxNofEventsToDraw(10);
	qaReco->DrawHistFromFile(histFileName);
}

