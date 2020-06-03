void run_digi(
        const string& mcFile = "/Users/slebedev/Development/cbm/data/sim/rich/reco/mc.00000.root",
        const string& parFile = "/Users/slebedev/Development/cbm/data/sim/rich/reco/param.00000.root",
        const string& digiFile ="/Users/slebedev/Development/cbm/data/sim/rich/reco/digi.00000.root",
        int nEvents = 100
)
{
    TTree::SetMaxTreeSize(90000000000);
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");
    FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

    Double_t eventRate = 1.e7;
    Double_t timeSliceLength = 1.e4;
    Bool_t eventMode = true;
    Bool_t overwrite = true;

    remove(digiFile.c_str());

    TStopwatch timer;
    timer.Start();

    CbmDigitization run;
    run.AddInput(mcFile.c_str(), eventRate);
    run.SetOutputFile(digiFile.c_str(), overwrite);
    run.SetParameterRootFile(parFile.c_str());
    run.SetTimeSliceLength(timeSliceLength);
    run.SetEventMode(eventMode);
    run.SetMonitorFile("");

    CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
    richDigitizer->SetMaxNofHitsPerPmtCut(65);
    run.SetDigitizer(kRich, richDigitizer, "RichPoint", true);

    run.Run(nEvents);

    timer.Stop();
    std::cout << std::endl;
    std::cout << "Macro finished successfully." << std::endl;
    std::cout << "Digi file is " << digiFile << std::endl;
    std::cout << "Parameter file is " << parFile << std::endl;
    std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
    std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
