// -----------------------------------------------------------------------------
// ----- setup_analysis FairRoot macro                                     -----
// -----                                                                   -----
// ----- created by N. Herrmann on 2014-04-18                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void setup_unpack(Int_t calMode=1, char *cFileId="CbmTofPiHd_17Aug1057", Int_t iSet=0) 
{
	// Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug, 4=raw debug)
	Int_t iVerbose = 2;
	
	// Specify log level (INFO, FATAL, ERROR, DEBUG, DEBUG1, DEBUG2, DEBUG3...)
	TString logLevel = "INFO";
	//TString logLevel = "ERROR";
	//TString logLevel = "FATAL";
	//TString logLevel = "DEBUG";
	//TString logLevel = "DEBUG1";
	
	// Fair Logger Instantiation...
	FairLogger* log;  

	// Parameter files:
	// Add TObjectString containing the different file names to a TList which is passed as
	// input to the FairParAsciiFileIo. The FairParAsciiFileIo will take care to create on 
	// the fly a concatenated input parameter file.
	TList *parFileList = new TList();
	TString workDir = gSystem->Getenv("VMCWORKDIR");
	TString paramDir = workDir + "/macro/tof/beamtime/lab16";
	TObjString unpParFile = paramDir + "/parUnpack_basic.txt";		
	parFileList->Add(&unpParFile);

	TObjString calParFile;
	TString cOutfileId;
	TString TofGeo="v15d";
	TObjString mapParFile;

	// ---------------------------  Give file names  ---------------------------
	switch(iSet)
	{
		case 0:
			calParFile = paramDir + "/parCalib_basic.txt";										
			cOutfileId = Form("%s",cFileId);
			mapParFile = paramDir + "/parMapLab16.txt";
			break;
		case 1:
			/*
			calParFile = paramDir + "/parCalib_basic.txt";
			cOutfileId = Form("%s%s",cFileId,"_nopla");
			mapParFile = paramDir + "/parMapCosmicsThuMay2015_nopla.txt";
			int mapping= 1;   
			*/  
			break;
		default:
			;
	}//end-switch(iSet)
	
	parFileList->Add(&calParFile);
	cout << " Output File tag "<< cOutfileId << endl;
	parFileList->Add(&mapParFile);
	// TObjString convParFile = paramDir + "/parConvFeb2015.txt";
	// parFileList->Add(&convParFile);
	
	// TOF digi file
	TObjString tofDigiFile = workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"; 
	// parFileList->Add(&tofDigiFile);   

	TObjString tofDigiBdfFile = workDir + "/parameters/tof/tof_" + TofGeo + ".digibdf.par"; 
	// parFileList->Add(&tofDigiBdfFile);

	// -----------------------------  Geo Manager  -----------------------------
	TString geoDir  = gSystem->Getenv("VMCWORKDIR");
	TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
	TFile* fgeo = new TFile(geoFile);
	TGeoManager *geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
	if (NULL == geoMan)
	{
		cout << "<E> FAIRGeom not found in geoFile"<<endl;
		// return;
	}
	if(0)
	{
		TGeoVolume* master=geoMan->GetTopVolume();
		master->SetVisContainers(1); 
		master->Draw("ogl"); 
	}


	// Parameter file output
	TString parFile = paramDir + "/unpack_" + cOutfileId + ".params.root";

	// Output file
	TString outFile = paramDir + "/unpack_" + cOutfileId + ".out.root";


	// ----------------------------  Debug option   ----------------------------
	gDebug = 0;

	// -----------------------------    Timer    -------------------------------
	TStopwatch timer;
	timer.Start();

	// =========================================================================
	// ===                             Unpacker                              ===
	// =========================================================================

	// In case of HLD files and using "CbmHldSource" class. Give path to the file
	// directory, all files will automatically be fetched by AddPath() with names 
	// starting cFieldID*.hld, see the flag "%s*".
	CbmHldSource* source = new CbmHldSource();
	//source->AddPath("./HLD/",Form("%s*.hld",cFileId));
	
	//source->AddFile("./HLD/CbmTofPiHd_26Feb1747_16057174732.hld");
	//source->AddFile("./HLD/CbmTofPiHd_26Feb1747_16058011819.hld");
	//source->AddFile("./HLD/CbmTofPiHd_01Mar1558_16061155836.hld");
	//source->AddFile("./HLD/CbmTofPiHd_01Apr1647_16092164739.hld");
	//source->AddFile("./HLD/CbmTofPiHd_08Apr1811_16099181155.hld");
	//source->AddFile("./HLD/CbmTofPiHd_14Apr1255_16105125514.hld");
	
	// -------------------- New Files ------------------------------
	//source->AddFile("./HLD/CbmTofPiHd_11Aug1224_16224122413.hld");
	//source->AddFile("./HLD/CbmTofPiHd_13Aug0855_16226085556.hld");
	source->AddFile("./HLD/CbmTofPiHd_15Aug0818_16228081806.hld");
	//source->AddFile("./HLD/CbmTofPiHd_16Aug1016_16229101637.hld");
	//source->AddFile("./HLD/CbmTofPiHd_17Aug1121_16230112115.hld");
	//source->AddFile("./HLD/CbmTofPiHd_17Aug1724_16230172404.hld");
	
	// -----------------------  TTriglogUnpackTof  -----------------------------
	/*
	TTriglogUnpackTof* tofTriglogUnpacker = new TTriglogUnpackTof();
	//tofTriglogUnpacker->SetSaveTriglog(kTRUE);
	tofTriglogUnpacker->SetSaveTriglog(kFALSE);
	source->AddUnpacker( tofTriglogUnpacker );
	
	TMbsUnpackTof* tofMbsDataUnpacker = new TMbsUnpackTof();
	source->AddUnpacker( tofMbsDataUnpacker );
	*/

	// --------------------------  TTrbUnpackTof  ------------------------------
	TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10,1,31,0,0);
	source->AddUnpacker( tofTrbDataUnpacker );
	gLogger->SetLogScreenLevel(logLevel.Data());

	// ----------------------   Online/Offline MBS run   -----------------------
	FairRunOnline *run = FairRunOnline::Instance();
	run->SetSource(source);
	run->SetOutputFile(outFile);
	run->SetAutoFinish(kFALSE);

	// ===                        End of Unpacker                            ===
	// =========================================================================



	// =========================================================================
	// ===                      Unpacker monitoring                          ===
	// =========================================================================

	TMbsUnpTofMonitor* tofUnpMonitor = new TMbsUnpTofMonitor("Tof Unp Moni", iVerbose);
	run->AddTask(tofUnpMonitor);

	// ===                    End of Unpacker monitoring                     ===
	// =========================================================================



	// =========================================================================
	// ===                           Calibration                             ===
	// =========================================================================

	TMbsCalibTof* tofCalibration = new TMbsCalibTof("Tof Calibration", iVerbose);
	tofCalibration->SetTdcCalibOutFoldername("./");
	//tofCalibration->SetTdcCalibFilename("TofTdcCalibHistos");
	tofCalibration->SetTdcCalibFilename(cFileId);
	run->AddTask(tofCalibration);

	// ===                         End of Calibration                        ===
	// =========================================================================



	// =========================================================================
	// ===                             Mapping                               ===
	// =========================================================================
	if(calMode > 0)
	{
		TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
		tofMapping->SetSaveDigis(kTRUE);
		run->AddTask(tofMapping);
	}
	// ===                          End of Mapping                           ===
	// =========================================================================

	// =========================================================================
	// ===                  GO4 like output formatting                       ===
	// =========================================================================

	// TMbsConvTof* tofConversion = new TMbsConvTof("Tof Conversion", iVerbose);
	// run->AddTask(tofConversion);

	// ===                  End of output conversion                         ===
	// =========================================================================

	// =========================================================================
	// ===                         Online Display                            ===
	// =========================================================================
	CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
	display->SetUpdateInterval(1000);
	run->AddTask(display);   

	// ===                       End of Online Display                       ===
	// =========================================================================

	// =========================================================================
	// ===                       Parameter Database                          ===
	// =========================================================================
	FairRuntimeDb* rtdb = run->GetRuntimeDb();
	Bool_t kParameterMerged = kTRUE;
	FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
	parIo2->open(parFile.Data(), "UPDATE");
	parIo2->print();
	rtdb->setFirstInput(parIo2);

	FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
	parIo1->open(parFileList, "in");
	parIo1->print();
	//rtdb->setFirstInput(parIo1);
	rtdb->setSecondInput(parIo1);

	rtdb->print();
	rtdb->printParamContexts();

	// ===                      End of Parameter Database                    ===
	// =========================================================================

	// =========================================================================
	// ===                       Intialise and Run                           ===
	// =========================================================================
	run->Init();
	cout << "Starting Run" << endl;
	
	// We shifted "run->Run(nEvents);" outside of this macro, i.e. inside shell 
	// script named "lmd_unpack.sh" or "hld_unpack.sh"
	//run->Run(nEvents, 0);

	// ===                      				                                  ===
	// =========================================================================



	/*
	timer.Stop();
	Double_t rtime = timer.RealTime();
	Double_t ctime = timer.CpuTime();
	cout << endl << endl;
	cout << "Macro finished successfully." << endl;
	cout << "Output file is " << outFile << endl;
	cout << "Parameter file is " << parFile << endl;
	cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
	cout << endl;
	*/
	//delete run;
}

