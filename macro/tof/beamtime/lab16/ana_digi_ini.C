void ana_digi_ini(Int_t nEvents = 10000, Int_t calMode=0, Int_t calSel=-1, Int_t calSm=200, Int_t RefSel=1, char *cFileId="CbmTofPiHd_22Aug1616", Int_t iCalSet=0, Int_t iBRef=500) 
{
  
	Int_t iVerbose = 1;
	
	//Specify log level (INFO, DEBUG, DEBUG1, ...)
	
	//TString logLevel = "FATAL";
	//TString logLevel = "ERROR";
	TString logLevel = "INFO";
	//TString logLevel = "DEBUG";
	//TString logLevel = "DEBUG1";
	//TString logLevel = "DEBUG2";
	//TString logLevel = "DEBUG3";
	FairLogger* log;  

	TString workDir    = gSystem->Getenv("VMCWORKDIR");
	TString paramDir   = workDir + "/macro/tof/beamtime/lab16";
	TString ParFile    = paramDir + "/unpack_" + cFileId + ".params.root";
	TString InputFile  = paramDir + "/unpack_" + cFileId + ".out.root";
	TString OutputFile = paramDir + "/digi_"   + cFileId + ".out.root";

	TList *parFileList = new TList();

	TObjString mapParFile = paramDir + "/parMapLab16.txt";
	parFileList->Add(&mapParFile);

	TString TofGeo="v15d"; 
	TString FId=cFileId;
	if( FId.Contains("CernSps02Mar") || FId.Contains("CernSps03Mar") ){
		TofGeo="v15b";
	}
	if(FId.Contains("CernSps28Feb")){
		TofGeo="v15a";
	}
	
	cout << "Geometry version "<<TofGeo<<endl;

	TString FPar="";
	if(FId.Contains("tsu")) {
		cout << FId << ": Analyse TSU counter "<< endl;
		TofGeo="v14b_tsu";
		FPar="tsu.";
	}

	TObjString tofDigiFile = workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"; // TOF digi file
	parFileList->Add(&tofDigiFile);   

	TObjString tofDigiBdfFile = workDir  + "/parameters/tof/tof_" + TofGeo +".digibdf.par";
	parFileList->Add(&tofDigiBdfFile);

	TString geoDir  = gSystem->Getenv("VMCWORKDIR");
	TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
	
	TFile* fgeo = new TFile(geoFile);
	TGeoManager *geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
	
	if (NULL == geoMan){
	
		cout << "<E> FAIRGeom not found in geoFile"<<endl;
		return;
		
	}//end-if
	
	if (0) {
		TGeoVolume* master=geoMan->GetTopVolume();
		master->SetVisContainers(1); 
		master->Draw("ogl");
	}//end-if

	/* **************************************************************************
											Reconstruction Run
	************************************************************************** */
	FairRunAna *run= new FairRunAna();
	run->SetInputFile(InputFile.Data());
	//run->AddFriend(InputFile.Data());
	run->SetOutputFile(OutputFile);
	gLogger->SetLogScreenLevel(logLevel.Data());

	/* **************************************************************************
												Mapping
	************************************************************************** */
	
	TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
	// run->AddTask(tofMapping);


	/* **************************************************************************
										TOF TestBeam Clusterizer
	************************************************************************** */
	CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, kFALSE);
	
	// CbmTofTestBeamClusterizer defaults
	tofTestBeamClust->SetCalMode(calMode);
	tofTestBeamClust->SetCalSel(calSel);
	tofTestBeamClust->SetCaldXdYMax(500.); // 3.   		// Geometrical matching window in cm 
	tofTestBeamClust->SetCalCluMulMax(20.);       		// Max counter cluster multiplicity for filling calib histos  
	tofTestBeamClust->SetCalRpc(calSm);           		// Select detector for calibration update  
	tofTestBeamClust->SetTRefId(RefSel);          		// Reference trigger for offset calculation 
	tofTestBeamClust->SetTotMax(10000.);          		// Tot upper limit for walk corection
	tofTestBeamClust->SetTotMin(1.); 						// (12000.); Tot lower limit for walk correction
	tofTestBeamClust->SetTotPreRange(5000.);      		// Effective lower Tot limit  in ps from peak position
	tofTestBeamClust->SetTotMean(2000.);         		// Tot calibration target value in ps 
	tofTestBeamClust->SetMaxTimeDist(500.); //50000.	// Default cluster range in ps, aa: cluster building radius

	Int_t calSelRead = calSel;
	if (calSel<0) calSelRead=0;
	TString cFname=Form("%s_set%09d_%02d_%01dtofTestBeamClust.hst.root",cFileId,iCalSet,calMode,calSelRead);
	tofTestBeamClust->SetCalParFileName(cFname);
	TString cOutFname=Form("tofTestBeamClust_%s_set%09d.hst.root",cFileId,iCalSet);
	tofTestBeamClust->SetOutHstFileName(cOutFname);
	TString cAnaFile=Form("%s_%09d_tofAnaTestBeam.hst.root",cFileId,iCalSet);

	/* **************************************************************************
	calMode: is a 2-digit number, 1st digit shows cuts applied and 2nd digit
	shows type of correction applied.

	   1 : Walk Correction
	   2 : Position alignment and gain correction at detector level
	   3 : Position alignment and gain correction at strip level
	   4 : Velocity corrections
	************************************************************************** */
	switch (calMode) {
	case 0:           											// initial calibration 
		tofTestBeamClust->SetTotMax(100000.);      		// 100 ns (5 >> 1)
		//tofTestBeamClust->SetTotMin(1.);
		//tofTestBeamClust->SetTotOff(0.);
		//tofTestBeamClust->SetTRefDifMax(2000000.); 	// in ps 
		tofTestBeamClust->SetTRefDifMax(3000000000000.);// in ps
		tofTestBeamClust->PosYMaxScal(2000.);      		// in % of length 
		tofTestBeamClust->SetMaxTimeDist(0.);      		// no cluster building  
		break;
		
	case 1:                                      		// save offsets, update walks 
		tofTestBeamClust->SetTRefDifMax(5000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);        		// in % of length 
		break;
	case 11:
		tofTestBeamClust->SetTRefDifMax(4000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);        		// in % of length 
		break;   
	case 21:
		tofTestBeamClust->SetTRefDifMax(3000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);        		// in % of length 
		break;
	case 31:
		tofTestBeamClust->SetTRefDifMax(2000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);        		// in % of length 
		break;
	case 41:
		tofTestBeamClust->SetTRefDifMax(1000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length 
		break;   
	case 51:
		tofTestBeamClust->SetTRefDifMax(700.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.7);        		// in % of length 
		break;
	case 61:
		tofTestBeamClust->SetTRefDifMax(500.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.7);        		// in % of length 
		break;   
	case 71:
		tofTestBeamClust->SetTRefDifMax(400.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.6);        		// in % of length 
		break;

	case 2:                                      		// time difference calibration
		tofTestBeamClust->SetTRefDifMax(300000.);  		// in ps 
		tofTestBeamClust->PosYMaxScal(1000.);      		// in % of length
		break;
	case 3:                                      		// time offsets
		//tofTestBeamClust->SetTRefDifMax(200000.);     // in ps 
		tofTestBeamClust->SetTRefDifMax(300000000000.); // in ps
		tofTestBeamClust->PosYMaxScal(1000.);       		// in % of length
		tofTestBeamClust->SetMaxTimeDist(0.);      		// no cluster building  
		break;
	 
	case 12:
	case 13:
		//tofTestBeamClust->SetTRefDifMax(100000.);   	// in ps 
		tofTestBeamClust->SetTRefDifMax(3000000000.); 	// in ps 
		tofTestBeamClust->PosYMaxScal(100.);        		// in % of length
		break;
	case 22:
	case 23:
		//tofTestBeamClust->SetTRefDifMax(50000.);    	// in ps 
		tofTestBeamClust->SetTRefDifMax(300000000.); 	// in ps 
		tofTestBeamClust->PosYMaxScal(50.);         		// in % of length
		break;
	case 32:
	case 33:
		//tofTestBeamClust->SetTRefDifMax(25000.);    	// in ps 
		tofTestBeamClust->SetTRefDifMax(30000000.); 		// in ps 
		tofTestBeamClust->PosYMaxScal(20.);         		// in % of length
		break;
	case 42:
	case 43:
		//tofTestBeamClust->SetTRefDifMax(13000.);   	// in ps 
		tofTestBeamClust->SetTRefDifMax(3000000.); 		// in ps 
		tofTestBeamClust->PosYMaxScal(10.);        		// in % of length
		break;
	case 52:
	case 53:
		//tofTestBeamClust->SetTRefDifMax(6000.);   		// in ps 
		tofTestBeamClust->SetTRefDifMax(400000.); 		// in ps 
		tofTestBeamClust->PosYMaxScal(10.);        		// in % of length
		break;
	case 62:
	case 63:
		tofTestBeamClust->SetTRefDifMax(200000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(10.);        		// in % of length
		break;
	case 72:
	case 73:
		tofTestBeamClust->SetTRefDifMax(2000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);        		// in % of length
		break;
	case 82:
	case 83:
		tofTestBeamClust->SetTRefDifMax(1000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length   
		break;
	case 92:
	case 93:
		tofTestBeamClust->SetTRefDifMax(600.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length   
		break;

	case 4:                                      		// velocity dependence (DelTOF)
		tofTestBeamClust->SetTRefDifMax(6000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.5);        		// in % of length
		break;
	case 14:
		tofTestBeamClust->SetTRefDifMax(2500.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(1.);        		// in % of length
		break;
	case 24:
		tofTestBeamClust->SetTRefDifMax(1000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length
		break;
	case 34:
		 tofTestBeamClust->SetTRefDifMax(1000.);   		// in ps 
		 tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length
	case 54:
		tofTestBeamClust->SetTRefDifMax(700.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.7);        		// in % of length
		break;
	case 64:
		tofTestBeamClust->SetTRefDifMax(500.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.7);        		// in % of length
		break;
	default:
		cout << "<E> Calib mode not implemented! stop execution of script"<<endl;
		return;
	}// end-switch(calMode)
	run->AddTask(tofTestBeamClust);











	/* **************************************************************************
									TOF TestBeam Analysis (Optional)
	************************************************************************** */
	CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);
	
	tofAnaTestbeam->SetDXMean(0.);							//CbmTofAnaTestbeam Defaults:
	tofAnaTestbeam->SetDYMean(0.);
	tofAnaTestbeam->SetDTMean(0.);  							// in ps
	
	tofAnaTestbeam->SetDXWidth(0.4);
	tofAnaTestbeam->SetDYWidth(0.4);
	tofAnaTestbeam->SetDTWidth(80.); 						// in ps
	
	tofAnaTestbeam->SetCalParFileName(cAnaFile);
	tofAnaTestbeam->SetPosY4Sel(0.5); 						// Y Position selection in fraction of strip length
	tofAnaTestbeam->SetDTDia(0.);   							// Time difference to additional diamond
	tofAnaTestbeam->SetCorMode(1);    						// 1 - DTD4, 2 - X4
	 
	tofAnaTestbeam->SetMul0Max(10);   						// Max Multiplicity in dut 
	tofAnaTestbeam->SetMul4Max(5);    						// Max Multiplicity in HDRef - RPC 
	tofAnaTestbeam->SetMulDMax(5);    						// Max Multiplicity in Diamond
	   
	tofAnaTestbeam->SetHitDistMin(30.);  					// initialization
	tofAnaTestbeam->SetPosYS2Sel(0.5);   					// Y Position selection in fraction of strip length
	tofAnaTestbeam->SetChS2Sel(0.);      					// Center of channel selection window
	tofAnaTestbeam->SetDChS2Sel(100.);   					// Width  of channel selection window
	
	/* *************************************************************************  
		 								iRSel, iRSelSm, iRSelRpc
	************************************************************************* */
	
	Int_t iRSel=iBRef;
	Int_t iRSelRpc = iRSel%10;
	iRSel = (iRSel - iRSelRpc)/10;
	Int_t iRSelSm = iRSel%10;
	iRSel = (iRSel - iRSelSm)/10;
	
	cout << "dispatch: Set BeamRefId to " << iRSel << ", " << iRSelSm << ", " << iRSelRpc << endl;
	
	tofTestBeamClust->SetBeamRefId(iRSel);    			// define Beam reference counter
	tofTestBeamClust->SetBeamRefSm(iRSelSm);				// define Beam reference counter
	tofTestBeamClust->SetBeamRefDet(iRSelRpc);			// define Beam reference counter
	
	tofTestBeamClust->SetBeamAddRefMul(-1);
	
	/* *************************************************************************  
				iDut, iRef (~iMRef) based on iCalSet=901900-920 & iSet=901900
	************************************************************************* */
	
   Int_t iBRef= iCalSet%1000;
   Int_t iSet = (iCalSet - iBRef)/1000;
   Int_t iRef = iSet %1000;

   Int_t iDut = (iSet - iRef)/1000;
   Int_t iDutRpc = iDut%10;
   iDut = (iDut - iDutRpc)/10;
   Int_t iDutSm = iDut%10;
   iDut = (iDut - iDutSm)/10;

   tofTestBeamClust->SetDutId(iDut);
   tofTestBeamClust->SetDutSm(iDutSm);
   tofTestBeamClust->SetDutRpc(iDutRpc);

   Int_t iRefRpc = iRef%10;
   iRef = (iRef - iRefRpc)/10;
   Int_t iRefSm = iRef%10;
   iRef = (iRef - iRefSm)/10;

   tofTestBeamClust->SetSelId(iRef);						// Selector RPC (Sel)
   tofTestBeamClust->SetSelSm(iRefSm);						// Selector RPC (Sel)			
   tofTestBeamClust->SetSelRpc(iRefRpc);					// Selector RPC (Sel)

   tofAnaTestbeam->SetDut(iDut);              			// Device Under Test (Dut)
   tofAnaTestbeam->SetDutSm(iDutSm);          			// Device Under Test (Dut)
   tofAnaTestbeam->SetDutRpc(iDutRpc);        			// Device Under Test (Dut)
   
   tofAnaTestbeam->SetMrpcRef(iRef);          			// Reference RPC (MRef)
   tofAnaTestbeam->SetMrpcRefSm(iRefSm);      			// Reference RPC (MRef)
   tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);    			// Reference RPC (MRef)

	tofAnaTestbeam->SetChi2Lim(100.);             		// initialization of Chi2 selection limit
	
	cout << endl;
	cout << "dispatch: iCalSet = " << iCalSet << ", iSet = " << iSet << ", iBRef = " << iBRef  << endl;
	cout << "dispatch: iRSel = " << iRSel << ", iRSelSm = " << iRSelSm << ", iRSelRpc = " << iRSelRpc << endl;
	cout << "dispatch:  iDut = " << iDut << ",  iDutSm = " << iDutSm << ",   iDutRpc = " << iDutRpc << endl;
	cout << "dispatch:  iRef = " << iRef << ",  iRefSm = " << iRefSm << ",   iRefRpc = " << iRefRpc << endl;
	
	/* **************************************************************************
	Remember in init_calib.sh the parmeter iCalSet=iDutiMRefiBRef==920921400, so
	iSet == iDutiMRef == 920921 etc
	************************************************************************** */
	switch (iSet) {
	
		case 400300:
		case 900901:
		case 901900:
		case 921920:
		case 920921:
		case 400400:
		case 400900:
		case 900900:
		case 921900:
		case 901901:
		case 921901:
		
			tofAnaTestbeam->SetTShift(0.);       			// Shift DTD4 to 0
			tofAnaTestbeam->SetSel2TOff(0.);     			// Shift Sel2 time peak to 0 
			tofAnaTestbeam->SetTOffD4(13000.);   			// Shift DTD4 to physical value
			
			// additional counter specific settings
			tofAnaTestbeam->SetCh4Sel(16.);      			// Center of channel selection window
			tofAnaTestbeam->SetDCh4Sel(20.);     			// Width  of channel selection window
			break;
			
		case 601600:
		case 600601:
		
			tofAnaTestbeam->SetTShift(0.);       			// Shift DTD4 to 0
			tofAnaTestbeam->SetSel2TOff(0.);     			// Shift Sel2 time peak to 0 
			tofAnaTestbeam->SetTOffD4(13000.);   			// Shift DTD4 to physical value
			
			// additional counter specific settings
			tofAnaTestbeam->SetCh4Sel(16.);      			// Center of channel selection window
			tofAnaTestbeam->SetDCh4Sel(20.);     			// Width  of channel selection window
			break;
				
		default:
			cout << "<E> detector setup " << iSet << " unknown, stop!" << endl;
			return;
			;
	} //end-switch(iSet)
	
	//run->AddTask(tofAnaTestbeam);
	
	// =========================================================================

	/*
	CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
	display->SetUpdateInterval(1000);
	run->AddTask(display);   
	*/

	/* **************************************************************************
												Parameter Database	   
	************************************************************************** */
	FairRuntimeDb* rtdb = run->GetRuntimeDb();
	Bool_t kParameterMerged = kTRUE;
	
	FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
	parIo2->open(ParFile.Data(), "UPDATE");
	parIo2->print();
	rtdb->setFirstInput(parIo2);

	FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
	parIo1->open(parFileList, "in");
	parIo1->print();
	rtdb->setSecondInput(parIo1);
	rtdb->print();
	rtdb->printParamContexts();

	//FairParRootFileIo* parInput1 = new FairParRootFileIo();
	//parInput1->open(ParFile.Data());
	//rtdb->setFirstInput(parInput1);

	/* **************************************************************************
							  					Intialise and Run	   
	************************************************************************** */
	run->Init();
	cout << "Starting run" << endl;
	run->Run(0, nEvents);

	/* **************************************************************************
													Plotting	   
	************************************************************************** */
	// default display 
	TString Display_Status = "pl_over_Mat04D4best.C";
	TString Display_Funct = "pl_over_Mat04D4best()";
	gROOT->LoadMacro(Display_Status);
	//gInterpreter->ProcessLine(Display_Funct);

	gROOT->LoadMacro("pl_over_cluSel.C");
	gROOT->LoadMacro("pl_over_clu.C");
	gROOT->LoadMacro("pl_all_dTSel.C");

	switch (iSet) {
	
		case 400300:
		case 921920:
		case 920921:
		case 920300:
		case 921300:
		case 400400:
		case 400900:
		case 900900:
		case 901900:
		case 921900:
		case 901901:
		case 601600:
		case 600601:
				
			// void pl_over_clu(Int_t SmT=0, Int_t iSm=0, Int_t iRpc=0)
			
			//gInterpreter->ProcessLine("pl_over_clu(3,0,0)");			
			//gInterpreter->ProcessLine("pl_over_clu(4,0,0)");
			//gInterpreter->ProcessLine("pl_over_clu(5,0)");
			//gInterpreter->ProcessLine("pl_over_clu(5,1)");
			//gInterpreter->ProcessLine("pl_over_clu(5,2)");
			
			gInterpreter->ProcessLine("pl_over_clu(6,0,0)");
			gInterpreter->ProcessLine("pl_over_clu(6,0,1)");
			
			gInterpreter->ProcessLine("pl_over_clu(9,0,0)");
			gInterpreter->ProcessLine("pl_over_clu(9,0,1)");
			gInterpreter->ProcessLine("pl_over_clu(9,1,0)");
			//gInterpreter->ProcessLine("pl_over_clu(9,1,1)");
			//gInterpreter->ProcessLine("pl_over_clu(9,2,0)");
			gInterpreter->ProcessLine("pl_over_clu(9,2,1)");

			// void pl_over_cluSel(Int_t iSel=0, Int_t iSmT=0, Int_t iSm=0, Int_t iRpc=0)
			
			//gInterpreter->ProcessLine("pl_over_cluSel(0,3)");		
			//gInterpreter->ProcessLine("pl_over_cluSel(0,4)");
			//gInterpreter->ProcessLine("pl_over_cluSel(0,5,0)");
			//gInterpreter->ProcessLine("pl_over_cluSel(0,5,1)");
			//gInterpreter->ProcessLine("pl_over_cluSel(0,5,2)");
			
			gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,1)");
			
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,0)");
			//gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,1)");
			//gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,1)");
			
			// void pl_over_cluSel(Int_t iSel=0, Int_t iSmT=0, Int_t iSm=0, Int_t iRpc=0)
			
			//gInterpreter->ProcessLine("pl_over_cluSel(1,3)");
			//gInterpreter->ProcessLine("pl_over_cluSel(1,4)");
			//gInterpreter->ProcessLine("pl_over_cluSel(1,5,0)");
			//gInterpreter->ProcessLine("pl_over_cluSel(1,5,1)");
			//gInterpreter->ProcessLine("pl_over_cluSel(1,5,2)");
			
			gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,1)");
			
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,0)");
			//gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,1)");
			//gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,1)");
			
			// void pl_all_dTSel(Int_t iNSel=2)
			gInterpreter->ProcessLine("pl_all_dTSel()");
			break;
						
		default:
			cout << "iSet = " << iSet << "\tnot found for plotting..." << endl;
			;
	}//end-switch(iSet)
}//end-macro
