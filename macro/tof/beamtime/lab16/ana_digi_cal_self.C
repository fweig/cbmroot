void ana_digi_cal_self(Int_t nEvents = 10000, Int_t calMode=0, Int_t calSel=-1, Int_t calSm=200, Int_t RefSel=1, char *cFileId="CbmTofSps_27Nov1728", Int_t iSet=0, Int_t iBRef=500, Bool_t bOut=0) 
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
	TString OutputFile = paramDir + "/digi_"   + cFileId + Form("_%06d_%03d",iSet,-iBRef) + ".out.root";

	TList *parFileList = new TList();

	TObjString mapParFile = paramDir + "/parMapLab16.txt";
	parFileList->Add(&mapParFile);

	TString TofGeo="v15d";  //use beamtime calib for buc PW
	TString FId=cFileId;
	if( FId.Contains("CernSps02Mar") || FId.Contains("CernSps03Mar")){
		TofGeo="v15b";
	}
	if(FId.Contains("CernSps28Feb")){
		TofGeo="v15a";
	}
	if( FId.Contains("BUC")){//use beamtime calib for buc PW
		TofGeo="v15c";
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
	
	if(0){
	
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
	
	//TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
	// run->AddTask(tofMapping);


	/* **************************************************************************
							TOF TestBeam Clusterizer
	************************************************************************** */
	CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, bOut);
	tofTestBeamClust->SetCalMode(calMode);
	tofTestBeamClust->SetCalSel(calSel);
	tofTestBeamClust->SetCaldXdYMax(20);         		// Geometrical matching window in cm 
	tofTestBeamClust->SetCalCluMulMax(32.);       		// Max counter cluster multiplicity for filling calib histos  
	tofTestBeamClust->SetCalRpc(calSm);           		// Select detector for calibration update  
	tofTestBeamClust->SetTRefId(RefSel);          		// Reference trigger for offset calculation 
	tofTestBeamClust->SetTotMax(12800.);          		// Tot upper limit for walk corection
	tofTestBeamClust->SetTotMin(1.); 					// (12000.);  // Tot lower limit for walk correction
	tofTestBeamClust->SetTotPreRange(5000.);      		// Effective lower Tot limit  in ps from peak position
	tofTestBeamClust->SetTotMean(2000.);         		// Tot calibration target value in ps 
	tofTestBeamClust->SetMaxTimeDist(500.);       		// Default cluster range in ps 
	tofTestBeamClust->SetBeamRefMulMax(32);
	//tofTestBeamClust->SetMaxTimeDist(0.);       		// Deb // default cluster range in ps 

	Int_t calSelRead = calSel;
	if (calSel<0) 
		calSelRead=0;

	TString cFname=Form("%s_set%06d_%02d_%01dtofTestBeamClust.hst.root",cFileId,iSet,calMode,calSelRead);
	tofTestBeamClust->SetCalParFileName(cFname);
	TString cOutFname=Form("tofTestBeamClust_%s_set%06d.hst.root",cFileId,iSet);
	tofTestBeamClust->SetOutHstFileName(cOutFname);
	TString cAnaFile=Form("%s_%06d_tofAnaTestBeam.hst.root",cFileId,iSet);
   
   
	/* **************************************************************************

	calMode: is a 2-digit number, 1st digit shows cuts applied and 2nd digit
	shows type of correction applied.

	   1 : Walk Correction
	   2 : Position alignment and gain correction at detector level
	   3 : Position alignment and gain correction at strip level
	   4 : Velocity corrections
	   
	************************************************************************** */
	switch (calMode) {
	case 0:                                      		// initial calibration 
		tofTestBeamClust->SetTotMax(100000.);     		// 100 ns
		//tofTestBeamClust->SetTotMin(1.);
		tofTestBeamClust->SetTRefDifMax(2000000.); 		// in ps 
		tofTestBeamClust->PosYMaxScal(2000.);      		// in % of length 
		tofTestBeamClust->SetMaxTimeDist(0.);      		// no cluster building  
		break;
	case 1:                                      		// save offsets, update walks 
		tofTestBeamClust->SetTRefDifMax(25000.);    	// in ps 
		tofTestBeamClust->PosYMaxScal(50.0);        	// in % of length
		tofTestBeamClust->SetMaxTimeDist(5000.); 
		break;
	case 11:
		tofTestBeamClust->SetTRefDifMax(4000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(2.0);        		// in % of length 
		tofTestBeamClust->SetMaxTimeDist(5000.);
		break;   
	case 21:
		tofTestBeamClust->SetTRefDifMax(3000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.5);        		// in % of length 
		tofTestBeamClust->SetMaxTimeDist(5000.);
		break;
	case 31:
		tofTestBeamClust->SetTRefDifMax(2000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);        		// in % of length
		tofTestBeamClust->SetMaxTimeDist(5000.); 
		break;
	case 41:
		tofTestBeamClust->SetTRefDifMax(1000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length
		tofTestBeamClust->SetMaxTimeDist(5000.); 
		break;   
	case 51:
		tofTestBeamClust->SetTRefDifMax(700.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.7);        		// in % of length
		tofTestBeamClust->SetMaxTimeDist(5000.); 
		break;
	case 61:
		tofTestBeamClust->SetTRefDifMax(500.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.7);        		// in % of length 
		tofTestBeamClust->SetMaxTimeDist(5000.);
		break;   
	case 71:
		tofTestBeamClust->SetTRefDifMax(3500.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);        		// in % of length 
		tofTestBeamClust->SetMaxTimeDist(4000.);
		break;
	case 81:
		tofTestBeamClust->SetTRefDifMax(2000.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length 
		tofTestBeamClust->SetMaxTimeDist(3000.);
		break;
	case 91:
		tofTestBeamClust->SetTRefDifMax(1200.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.7);        		// in % of length 
		tofTestBeamClust->SetMaxTimeDist(2000.);
		break;

	case 2:                                      		// time difference calibration
		tofTestBeamClust->SetTRefDifMax(300000.);  		// in ps 
		tofTestBeamClust->PosYMaxScal(1000.);      		// in % of length
		break;

	case 3:
	case 5:                                       		// time offsets 
		tofTestBeamClust->SetTRefDifMax(200000.);   	// in ps 
		tofTestBeamClust->PosYMaxScal(1000.);       	// in % of length
		tofTestBeamClust->SetMaxTimeDist(0.);       	// no cluster building  
		break;
	case 12:
	case 13:
	case 15:
		tofTestBeamClust->SetTRefDifMax(100000.);   	// in ps 
		tofTestBeamClust->PosYMaxScal(100.);        	// in % of length
		tofTestBeamClust->SetMaxTimeDist(5000.);
		break;
	case 22:
	case 23:
	case 25:
		tofTestBeamClust->SetTRefDifMax(50000.);    	// in ps 
		//tofTestBeamClust->PosYMaxScal(50.);         	// in % of length
		tofTestBeamClust->PosYMaxScal(10.);
		tofTestBeamClust->SetMaxTimeDist(5000.);
		break;
	case 32:
	case 33:
	case 35:
		tofTestBeamClust->SetTRefDifMax(25000.);    	// in ps 
		//tofTestBeamClust->PosYMaxScal(20.);         	// in % of length
		tofTestBeamClust->PosYMaxScal(5.);
		tofTestBeamClust->SetMaxTimeDist(5000.);
		break;
	case 42:
	case 43:
	case 45:
		tofTestBeamClust->SetTRefDifMax(13000.);   		// in ps 
		//tofTestBeamClust->PosYMaxScal(10.);        		// in % of length
		tofTestBeamClust->PosYMaxScal(4.);
		tofTestBeamClust->SetMaxTimeDist(5000.);
		break;
	case 52:
	case 53:
	case 55:
		//tofTestBeamClust->SetTRefDifMax(6000.);   		// in ps 
		//tofTestBeamClust->PosYMaxScal(4.);   
		tofTestBeamClust->SetTRefDifMax(6000.);   		// in ps 
		//tofTestBeamClust->PosYMaxScal(6.);        		// in % of length
		tofTestBeamClust->PosYMaxScal(3.);
		tofTestBeamClust->SetMaxTimeDist(5000.);
		break;
	case 62:
	case 63:
	case 65:
		//tofTestBeamClust->SetTRefDifMax(3000.);   		// in ps 
		//tofTestBeamClust->PosYMaxScal(2.); 
		tofTestBeamClust->SetTRefDifMax(3000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(2.);        		// in % of length
		tofTestBeamClust->SetMaxTimeDist(4000.);
		break;
	case 72:
	case 73:
	case 75:
		//tofTestBeamClust->SetTRefDifMax(2000.);    		// in ps 
		//tofTestBeamClust->PosYMaxScal(1.0);     
		tofTestBeamClust->SetTRefDifMax(1500.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);        		// in % of length
		tofTestBeamClust->SetMaxTimeDist(2000.);
		break;
	case 82:
	case 83:
	case 85:
		//tofTestBeamClust->SetTRefDifMax(1000.);    		// in ps 
		//tofTestBeamClust->PosYMaxScal(0.8);    
		tofTestBeamClust->SetTRefDifMax(800.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length   
		tofTestBeamClust->SetMaxTimeDist(1500.);
		break;
	case 92:
	case 93:
	case 95:
		tofTestBeamClust->SetTRefDifMax(300.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(0.7);        		// in % of length   
		tofTestBeamClust->SetMaxTimeDist(500.);
		break;


	case 4:                                      		// velocity dependence (DelTOF)
		tofTestBeamClust->SetTRefDifMax(60000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(1.5);        		// in % of length
		break;
	case 14:
		tofTestBeamClust->SetTRefDifMax(6000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(4.);        		// in % of length
		break;
	case 24:
		tofTestBeamClust->SetTRefDifMax(2000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);        		// in % of length
		break;
	case 34:
		tofTestBeamClust->SetTRefDifMax(1000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length
		break;
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
		
	}//end-switch(calMode)

	run->AddTask(tofTestBeamClust);


	/* **************************************************************************
							TOF TestBeam Analysis
	************************************************************************** */
	CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);
	
	tofAnaTestbeam->SetDXMean(0.);						//CbmTofAnaTestbeam Defaults:
	tofAnaTestbeam->SetDYMean(0.);
	tofAnaTestbeam->SetDTMean(0.);  					// in ps
	tofAnaTestbeam->SetDXWidth(0.4);
	tofAnaTestbeam->SetDYWidth(0.4);
	tofAnaTestbeam->SetDTWidth(80.); 					// in ps
	tofAnaTestbeam->SetCalParFileName(cAnaFile);
	tofAnaTestbeam->SetPosY4Sel(0.5); 					// Y Position selection in fraction of strip length
	tofAnaTestbeam->SetDTDia(0.);   					// Time difference to additional diamond
	tofAnaTestbeam->SetCorMode(1);    					// 1 - DTD4, 2 - X4 
	tofAnaTestbeam->SetMul0Max(1);   					// Max Multiplicity in dut 
	tofAnaTestbeam->SetMul4Max(1);    					// Max Multiplicity in HDRef - RPC 
	tofAnaTestbeam->SetMulDMax(5);    					// Max Multiplicity in Diamond    
	tofAnaTestbeam->SetHitDistMin(30.);  				// initialization
	tofAnaTestbeam->SetPosYS2Sel(0.5);   				// Y Position selection in fraction of strip length
	tofAnaTestbeam->SetChS2Sel(0.);      				// Center of channel selection window
	tofAnaTestbeam->SetDChS2Sel(100.);   				// Width  of channel selection window
	tofAnaTestbeam->SetTShift(0.);       				// Shift DTD4 to 0
	tofAnaTestbeam->SetSel2TOff(0.);     				// Shift Sel2 time peak to 0 
	tofAnaTestbeam->SetTOffD4(13000.);   				// Shift DTD4 to physical value

	Int_t iRSel=iBRef;
	Int_t iRSelRpc = iRSel%10;
	iRSel = (iRSel - iRSelRpc)/10;
	Int_t iRSelSm = iRSel%10;
	iRSel = (iRSel - iRSelSm)/10;
	
	tofTestBeamClust->SetBeamRefId(iRSel);    			// define Beam reference counter 
	tofTestBeamClust->SetBeamRefSm(iRSelSm);
	tofTestBeamClust->SetBeamRefDet(iRSelRpc);
	tofTestBeamClust->SetBeamAddRefMul(-1);
	
	tofTestBeamClust->SetSel2Id(iRSel);    			// PW: define Sel2 for Cluster building equal to the beam Ref
	tofTestBeamClust->SetSel2Sm(iRSelSm);
	tofTestBeamClust->SetSel2Rpc(iRSelRpc);

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

	tofTestBeamClust->SetSelId(iRef);
	tofTestBeamClust->SetSelSm(iRefSm);
	tofTestBeamClust->SetSelRpc(iRefRpc);

	tofAnaTestbeam->SetDut(iDut);              			// Device under test   
	tofAnaTestbeam->SetDutSm(iDutSm);          			// Device under test   
	tofAnaTestbeam->SetDutRpc(iDutRpc);        			// Device under test   
	tofAnaTestbeam->SetMrpcRef(iRef);          			// Reference RPC     
	tofAnaTestbeam->SetMrpcRefSm(iRefSm);      			// Reference RPC     
	tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);    			// Reference RPC     

	tofAnaTestbeam->SetChi2Lim(100.);             		// initialization of Chi2 selection limit  



	/* **************************************************************************
	Remember in init_calib.sh the parmeter iCalSet=iDutiMRefiBRef==920921400, so
	iSet == iDutiMRef == 920921 etc
	************************************************************************** */
	switch (iSet)
	{
		case 0:                                 		// upper part of setup: P2 - P5
		case 3:                                 		// upper part of setup: P2 - P5
		case 34:                                		// upper part of setup: P2 - P5
		case 400300:
		case 900901:
		case 901900:
		case 921920:
		case 920921:
		case 900900:
		case 901900:
		case 901901:
			tofAnaTestbeam->SetTOffD4(13000.);   		// Shift DTD4 to physical value
			tofAnaTestbeam->SetSel2TOff(-230.);  		// Shift Sel2 time peak to 0
			tofAnaTestbeam->SetCh4Sel(16.);      		// Center of channel selection window
			tofAnaTestbeam->SetDCh4Sel(20.);     		// Width  of channel selection window
			break;
			
		case 910910:
		case 911911:	
		case 400400:
		case 400200: //Moved from default case PW
		case 400210:
		case 601601:
		case 600600:
		case 921921:
		tofAnaTestbeam->SetTOffD4(0);   		// Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0);  		// Shift Sel2 time peak to 0
		tofAnaTestbeam->SetCh4Sel(16.);      		// Center of channel selection window
		tofAnaTestbeam->SetDCh4Sel(20.);     		// Width  of channel selection window
			break;
		
			
		default:
			cout<<"<E> detector setup "<<iSet<<" unknown, stop!"<<endl;
			return;
			;
	}// end-switch(iSet)

	switch(cFileId)
	{
		case " ":
			break;

		default:
			;
	}//end-switch(cFileId)
	
	// run->AddTask(tofAnaTestbeam);
	
	// =========================================================================

	/*
	CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
	display->SetUpdateInterval(1000);
	run->AddTask(display);   
	*/


	/* **************************************************************************
								Parameter database	   
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
							  Intialise and run	   
	************************************************************************** */
	run->Init();
	cout << "Starting run" << endl;
	run->Run(0, nEvents);


	/* **************************************************************************
									Plotting	   
	************************************************************************** */
	
	// Default Display 
	TString Display_Status = "pl_over_Mat04D4best.C";
	gROOT->LoadMacro(Display_Status);

	//TString Display_Funct = "pl_over_Mat04D4best()";
	//gInterpreter->ProcessLine(Display_Funct);

	gROOT->LoadMacro("pl_over_cluSel.C");
	gROOT->LoadMacro("pl_over_clu.C");
	gROOT->LoadMacro("pl_all_dTSel.C");

	switch(iSet)
	{
		case 0:
		case 3:
		case 49:
		case 79:
		case 34:
		case 94:
		case 37:
		case 97:
		case 39:
		case 99:
		case 93:
		case 400300:
		case 901900:
		case 921920:
		case 920921:
		case 920300:
		case 921300:
		case 400400:
		case 900900:
		case 901900:
		case 901901:
		case 910910:
		case 911911:
		case 601601:
		case 600600:
		case 400200:
		case 400210:
		case 921921:
			// void pl_over_clu(Int_t SmT=0, Int_t iSm=0, Int_t iRpc=0)
		
			gInterpreter->ProcessLine("pl_over_clu(3,0,0)");			
			gInterpreter->ProcessLine("pl_over_clu(4,0,0)");
			//gInterpreter->ProcessLine("pl_over_clu(5,0)");
			//gInterpreter->ProcessLine("pl_over_clu(5,1)");
			//gInterpreter->ProcessLine("pl_over_clu(5,2)");
			gInterpreter->ProcessLine("pl_over_clu(9,0,0)");
			gInterpreter->ProcessLine("pl_over_clu(9,0,1)");
			gInterpreter->ProcessLine("pl_over_clu(9,1,0)");
			gInterpreter->ProcessLine("pl_over_clu(9,1,1)");
			gInterpreter->ProcessLine("pl_over_clu(9,2,0)");
			gInterpreter->ProcessLine("pl_over_clu(9,2,1)");
			gInterpreter->ProcessLine("pl_over_clu(6,0,1)");
			gInterpreter->ProcessLine("pl_over_clu(6,0,0)");
			gInterpreter->ProcessLine("pl_over_clu(2,1,0)");
			gInterpreter->ProcessLine("pl_over_clu(2,0,0)");
		
			// void pl_over_cluSel(Int_t iSel=0, Int_t iSmT=0, Int_t iSm=0, Int_t iRpc=0)
		
			gInterpreter->ProcessLine("pl_over_cluSel(0,3)");		
			gInterpreter->ProcessLine("pl_over_cluSel(0,4)");
			//gInterpreter->ProcessLine("pl_over_cluSel(0,5,0)");
			//gInterpreter->ProcessLine("pl_over_cluSel(0,5,1)");
			//gInterpreter->ProcessLine("pl_over_cluSel(0,5,2)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,3)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,4)");
			//gInterpreter->ProcessLine("pl_over_cluSel(1,5,0)");
			//gInterpreter->ProcessLine("pl_over_cluSel(1,5,1)");
			//gInterpreter->ProcessLine("pl_over_cluSel(1,5,2)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,1)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,2,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,2,0,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(0,2,1,0)");
			gInterpreter->ProcessLine("pl_over_cluSel(1,2,1,0)");
		
			// void pl_all_dTSel(Int_t iNSel=2)
		
			gInterpreter->ProcessLine("pl_all_dTSel()");				
			break;
		
		default:
			;
	}//end-switch(iSet)
	
}//end-macro
