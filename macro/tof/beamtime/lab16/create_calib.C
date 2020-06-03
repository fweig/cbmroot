// -----------------------------------------------------------------------------
// ----- create_calib.C                                                    -----
// -----                                                                   -----
// ----- created by C. Simon on 2016-04-09                                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

void create_calib(Int_t nEvents = 100000000, char *cFileId="CbmTofPiHd_All")
{
	TStopwatch timer;
	timer.Start();

	//gLogger->SetLogScreenLevel("FATAL");
	gLogger->SetLogScreenLevel("ERROR");
	//gLogger->SetLogScreenLevel("WARNING");
	//gLogger->SetLogScreenLevel("INFO");
	//gLogger->SetLogScreenLevel("DEBUG");
	//gLogger->SetLogScreenLevel("DEBUG1");
	//gLogger->SetLogScreenLevel("DEBUG2");
	//gLogger->SetLogScreenLevel("DEBUG3");

	//gLogger->SetLogVerbosityLevel("LOW");
	//gLogger->SetLogVerbosityLevel("MEDIUM");
	//gLogger->SetLogVerbosityLevel("HIGH");

	TString workDir = gSystem->Getenv("VMCWORKDIR");
	TString paramDir = workDir + "/macro/tof/beamtime/lab16";

	TObjString unpParFile = paramDir + "/parUnpack_basic.txt";
	TObjString calParFile = paramDir + "/parCalib_basic.txt";

	TList *parFileList = new TList();
	parFileList->Add(&unpParFile);
	parFileList->Add(&calParFile);
	
	// TODO: FairRunOnline* run = new FairRunOnline() in created in new_hld_unpack.sh
	FairRuntimeDb* rtdb = run->GetRuntimeDb();
	FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
	parIo1->open(parFileList, "in");
	rtdb->setFirstInput(parIo1);
	TString cOutfileId = Form("%s",cFileId);
	TString outFile = paramDir + "/unpack_" + cOutfileId + ".calib.root";
   CbmHldSource* source = new CbmHldSource();
   //source->AddPath("./HLD/",Form("%s*.hld",cFileId));
   //source->AddFile("./HLD/CbmTofPiHd_01Apr1647_16092164739.hld");
	//source->AddFile("./HLD/CbmTofPiHd_08Apr1811_16099181155.hld");
	//source->AddFile("./HLD/CbmTofPiHd_14Apr1255_16105125514.hld");
	
  	// ----------------- New Data Files (As of Aug 01, 2016)
 	//source->AddFile("./HLD/CbmTofPiHd_09Aug1533_16222183515.hld");
 	//source->AddFile("./HLD/CbmTofPiHd_10Aug1728_16223172839.hld");
 	//source->AddFile("./HLD/CbmTofPiHd_11Aug1224_16224122413.hld");
 	//source->AddFile("./HLD/CbmTofPiHd_13Aug0855_16226085556.hld");
 	//source->AddFile("./HLD/CbmTofPiHd_15Aug0818_16228081806.hld");
 	//source->AddFile("./HLD/CbmTofPiHd_16Aug1016_16229101637.hld");
	//source->AddFile("./HLD/CbmTofPiHd_17Aug1724_16230172404.hld");
 	//source->AddFile("./HLD/CbmTofPiHd_22Aug1616_16235161606.hld");
 	source->AddFile("./HLD/CbmTofPiHd_29Aug1401_16242140119.hld");
 	//source->AddFile("./HLD/CbmTofPiHd_07Sep1333_16251133311.hld");
 	//source->AddFile("./HLD/CbmTofPiHd_12Sep1601_16256205114.hld");
 		
	TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10,1,31,0,0);
	tofTrbDataUnpacker->SetInspection(kTRUE);
	tofTrbDataUnpacker->SetSaveRawData(kFALSE);
	source->AddUnpacker( tofTrbDataUnpacker );

	TMbsUnpTofMonitor* tofUnpMonitor = new TMbsUnpTofMonitor("Tof Unp Moni");

	TTofTrbTdcCalib* tofCalibTrb = new TTofTrbTdcCalib();
	tofCalibTrb->CreateCalibration(kTRUE);
	tofCalibTrb->SaveCalibData(kTRUE);					   // aa: True to write in unpack_*.root
	tofCalibTrb->SetFineTimeMethod(0);
	tofCalibTrb->SetToTMethod(0);
	tofCalibTrb->SetMinEntriesBinByBin(100000);
	tofCalibTrb->SetMinEntriesLocalFineLimits(10000);
	tofCalibTrb->SetTimeContinuum(kTRUE);
	tofCalibTrb->SetTrailingOffsetCycles(2);
	//tofCalibTrb->SetLowerLinearFineLimit(31);  		// feb15
	//tofCalibTrb->SetUpperLinearFineLimit(490); 		// feb15
	tofCalibTrb->SetLowerLinearFineLimit(16);  			// nov15
	tofCalibTrb->SetUpperLinearFineLimit(490); 			// nov15
	tofCalibTrb->SetReferenceBoard(3);						// aa: 1st reference tdc should be active one
	tofCalibTrb->SetToTSingleLeading(-100.);

	CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
	//display->MonitorTdcOccupancy(kTRUE);
	//display->MonitorCts(kTRUE);
	//display->MonitorSebStatus(kTRUE);
	//display->MonitorTdcStatus(kTRUE);
	//display->MonitorFSMockup(kTRUE);
	display->MonitorInspection(kTRUE);
	display->MonitorCalibration(kTRUE);						// aa: if fit failure see SetReferenceBoard() above
	display->SetUpdateInterval(10000);						// aa: entries
	//display->SetUpdateInterval(100000000); 				// disables live updates (speed-up)

	// Remember no mapping in create_calib.C
	run->AddTask(tofUnpMonitor);
	run->AddTask(tofCalibTrb);
	run->AddTask(display);

	run->SetSource(source);
	run->SetOutputFile(outFile);
	run->SetAutoFinish(kFALSE);

	run->Init();

	cout << "Starting run" << endl;
	run->Run(0, nEvents);

	cout << "Finishing run" << endl;
	run->Finish();

	tofTrbDataUnpacker->WriteHistograms();

	timer.Stop();
	Double_t rtime = timer.RealTime();
	Double_t ctime = timer.CpuTime();
	cout << endl << endl;
	cout << "Run finished successfully." << endl;
	cout << "Output file is " << outFile << endl;
	cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
	cout << endl;
}

