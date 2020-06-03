void run_analysis_jpsi(Int_t nEvents = 1000)
{
   TTree::SetMaxTreeSize(90000000000);

	Int_t iVerbose = 0;

	TString script = TString(gSystem->Getenv("SCRIPT"));
	TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters");
    TString stsMatBudgetFileName = parDir + "/sts/sts_matbudget_v13d.root"; // Material budget file for L1 STS tracking

	gRandom->SetSeed(10);

        //TString mcFile = "/hera/cbm/users/adrian/data/jun15_25gev/jpsi_urqmd/mc.auau.25gev.centr.00001.root";
	//TString parFile = "/hera/cbm/users/adrian/data/jun15_25gev/jpsi_urqmd/params.auau.25gev.centr.00001.root";
	//TString recoFile ="/hera/cbm/users/adrian/data/jun15_25gev/jpsi_urqmd/reco.auau.25gev.centr.00001.root";
	//TString anaFile ="/hera/cbm/users/adrian/data/jun15_25gev/jpsi_urqmd/analysis.auau.25gev.centr.00001.root";

	//TString mcFile = "/hera/cbm/users/adrian/data/test/mc.0001.root";
	//TString parFile = "/hera/cbm/users/adrian/data/test/param.0001.root";
	//TString recoFile ="/hera/cbm/users/adrian/data/test/reco.0001.root";
	//TString anaFile ="/hera/cbm/users/adrian/data/test/analysis.0001.root";

	TString parFile = "/Users/slebedev/Development/cbm/data/jpsi/param.0001.root";
	TString recoFile = "/Users/slebedev/Development/cbm/data/jpsi/reco.0001.root";
	TString mcFile = "/Users/slebedev/Development/cbm/data/jpsi/mc.0001.root";
	TString anaFile = "/Users/slebedev/Development/cbm/data/jpsi/analysis.0001.root";

	TObjString stsDigiFile = parDir + "/sts/sts_v13d_std.digi.par"; // STS digi file
    TObjString trdDigiFile = parDir + "/trd/trd_v14a_3e.digi.par"; // TRD digi file
    TObjString tofDigiFile = parDir + "/tof/tof_v13b.digi.par"; // TOF digi file
    Double_t trdAnnCut = 0.85;
    Int_t minNofPointsTrd = 6;
	if (script == "yes") {
		mcFile = TString(gSystem->Getenv("MC_FILE"));
		recoFile = TString(gSystem->Getenv("RECO_FILE"));
		parFile = TString(gSystem->Getenv("PAR_FILE"));
		anaFile = TString(gSystem->Getenv("ANALYSIS_FILE"));
		stsDigiFile = TString(gSystem->Getenv("STS_DIGI"));
		trdDigiFile = TString(gSystem->Getenv("TRD_DIGI"));
		tofDigiFile = TString(gSystem->Getenv("TOF_DIGI"));
		stsMatBudgetFileName = TString(gSystem->Getenv("STS_MATERIAL_BUDGET_FILE"));
	}

   TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters");
   TList *parFileList = new TList();
   if (stsDigiFile.GetString() != "") parFileList->Add(&stsDigiFile);
   if (trdDigiFile.GetString() != "") parFileList->Add(&trdDigiFile);
   if (tofDigiFile.GetString() != "") parFileList->Add(&tofDigiFile);
   gDebug = 0;

    TStopwatch timer;
    timer.Start();

	// ----  Load libraries   -------------------------------------------------
   gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
   loadlibs();
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/determine_setup.C");


	// Remove output jpsi analysis file
	remove(anaFile.Data());

	// -----   Reconstruction run   -------------------------------------------
	FairRunAna *run= new FairRunAna();
	if (mcFile != "") run->SetInputFile(mcFile);
	run->AddFriend(recoFile);
	if (recoFile != "") run->SetOutputFile(anaFile);

	//CbmKF is needed for Extrapolation
	CbmKF* kf = new CbmKF();
	run->AddTask(kf);

	CbmL1* l1 = new CbmL1();
	l1->SetMaterialBudgetFileName(stsMatBudgetFileName);
	run->AddTask(l1);

	CbmAnaJpsiTask* jpsiTask= new CbmAnaJpsiTask();
	run->AddTask(jpsiTask);


    // -----  Parameter database   --------------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    FairParRootFileIo* parIo1 = new FairParRootFileIo();
    FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
    parIo1->open(parFile.Data());
    parIo2->open(parFileList, "in");
    rtdb->setFirstInput(parIo1);
    rtdb->setSecondInput(parIo2);
    rtdb->setOutput(parIo1);
    rtdb->saveOutput();

    run->Init();
    cout << "Starting run" << endl;
    run->Run(0,nEvents);

    // -----   Finish   -------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished successfully." << endl;
    cout << "Output ana file is "    << anaFile << endl;
    cout << "Parameter file is " << parFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
    cout << endl;

    cout << " Test passed" << endl;
    cout << " All ok " << endl;
}
