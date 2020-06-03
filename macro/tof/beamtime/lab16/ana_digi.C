void ana_digi(Int_t nEvents = 100000, Int_t calMode=0, Int_t calSel=-1, Int_t calSm=300, Int_t RefSel=1, char *cFileId="MbsTrbThu1715", Int_t iCalSet=901900921, Bool_t bOut=0, Int_t iSel2=0, Bool_t bAvWalk=kTRUE) 
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
	TString OutputFile = paramDir + "/digi_"   + cFileId + Form("_%09d_%03d",iCalSet,iSel2) + ".out.root";

	TList *parFileList = new TList();

	TObjString mapParFile = paramDir + "/parMapLab16.txt";
	parFileList->Add(&mapParFile);
	TString FId=cFileId;
	TString TofGeo="v15d";
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
	 TofGeo="v14c_tsu";
	 FPar="tsu.";
	}

	TObjString tofDigiFile = workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"; // TOF digi file
	parFileList->Add(&tofDigiFile);   

	//   TObjString tofDigiBdfFile =  paramDir + "/tof." + FPar + "digibdf.par";
	TObjString tofDigiBdfFile = workDir  + "/parameters/tof/tof_" + TofGeo +".digibdf.par";
	parFileList->Add(&tofDigiBdfFile);

	TString geoDir  = gSystem->Getenv("VMCWORKDIR");
	TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
	TFile* fgeo = new TFile(geoFile);
	TGeoManager *geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
	if (NULL == geoMan){
	 cout << "<E> FAIRGeom not found in geoFile"<<endl;
	 return;
	}
	if(0){
	TGeoVolume* master=geoMan->GetTopVolume();
	master->SetVisContainers(1); 
	master->Draw("ogl"); 
	}

	/* **************************************************************************
								Reconstruction Run
	************************************************************************** */
	FairRunAna *run= new FairRunAna();
	run->SetInputFile(InputFile.Data());	//run->AddFriend(InputFile.Data());
	run->SetOutputFile(OutputFile);
	gLogger->SetLogScreenLevel(logLevel.Data());

	/* **************************************************************************
									Mapping
	************************************************************************** */
	TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
	//run->AddTask(tofMapping);
	
	/* **************************************************************************
							TOF TestBeam Clusterizer
	************************************************************************** */
	CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, bOut);
	
	//defaults
	tofTestBeamClust->SetCalMode(calMode);
	tofTestBeamClust->SetCalSel(calSel);
	tofTestBeamClust->SetCaldXdYMax(6.);         		// geometrical matching window in cm 
	tofTestBeamClust->SetCalCluMulMax(2.);       		// Max Counter Cluster Multiplicity for filling calib histos  
	tofTestBeamClust->SetCalRpc(calSm);           		// select detector for calibration update  
	tofTestBeamClust->SetTRefId(RefSel);          		// reference trigger for offset calculation 
	tofTestBeamClust->SetTotMax(10000.);          		// Tot upper limit for walk corection
	tofTestBeamClust->SetTotMin(1.); //(12000.);  		// Tot lower limit for walk correction
	tofTestBeamClust->SetTotPreRange(5000.);      		// effective lower Tot limit  in ps from peak position
	tofTestBeamClust->SetTotMean(2000.);          		// Tot calibration target value in ps 
	tofTestBeamClust->SetMaxTimeDist(600.);       		// default cluster range in ps 
   tofTestBeamClust->SetDelTofMax(3000.);        		// maximum time difference from BRef to Selector 
	tofTestBeamClust->SetEnableMatchPosScaling(kFALSE);// disable projective geometry Ansatz 
	tofTestBeamClust->SetEnableAvWalk(bAvWalk);        // average over all channels for each RPC
	tofTestBeamClust->SetBeamRefMulMax(2);					// ???
	//end-defaults
	
	Int_t calSelRead = calSel;
	if (calSel<0) 	calSelRead=0;

	TString cFname=Form("%s_set%09d_%02d_%01dtofTestBeamClust.hst.root",cFileId,iCalSet,calMode,calSelRead);
	tofTestBeamClust->SetCalParFileName(cFname);
	TString cOutFname=Form("tofTestBeamClust_%s_set%09d.hst.root",cFileId,iCalSet);
	tofTestBeamClust->SetOutHstFileName(cOutFname);

	TString cAnaFile=Form("%s_%09d%03d_tofAnaTestBeam.hst.root",cFileId,iCalSet,iSel2);
  
	/* **************************************************************************
	   calMode: is a 2-digit number, 1st digit shows cuts applied and 2nd digit
	   shows type of correction applied.
	   1 : Walk Correction
	   2 : Position alignment and gain correction at detector level
	   3 : Position alignment and gain correction at strip level
	   4 : Velocity corrections
	***************************************************************************/
	switch (calMode) {
	case 0:                                      		// initial calibration 
		tofTestBeamClust->SetTotMax(100000.);      		// 100 ns
		//tofTestBeamClust->SetTotMin(1.);
		tofTestBeamClust->SetTRefDifMax(2000000.); 		// in ps 
		tofTestBeamClust->PosYMaxScal(2000.);      		// in % of length 
		tofTestBeamClust->SetMaxTimeDist(0.);      		// no cluster building  
		break;
	case 1:                                      		// save offsets, update walks, for diamonds 
		tofTestBeamClust->SetTRefDifMax(50000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(0.1);        		// in % of length 
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

	case 3:                                       		// time offsets 
		tofTestBeamClust->SetTRefDifMax(200000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(1000.);       		// in % of length
		tofTestBeamClust->SetMaxTimeDist(0.);       		// no cluster building  
		break;
	case 12:
	case 13:
		tofTestBeamClust->SetTRefDifMax(100000.);   		// in ps 
		tofTestBeamClust->PosYMaxScal(100.);        		// in % of length
		break;
	case 22:
	case 23:
		tofTestBeamClust->SetTRefDifMax(50000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(50.);         		// in % of length
		break;
	case 32:
	case 33:
		tofTestBeamClust->SetTRefDifMax(25000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(20.);         		// in % of length
		break;
	case 42:
	case 43:
		tofTestBeamClust->SetTRefDifMax(12000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(10.);         		// in % of length
		break;
	case 52:
	case 53:
		tofTestBeamClust->SetTRefDifMax(5000.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(4.);          		// in % of length
		break;
	case 62:
	case 63:
		tofTestBeamClust->SetTRefDifMax(2500.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(2.);          		// in % of length
		break;
	case 72:
	case 73:
		tofTestBeamClust->SetTRefDifMax(2000.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(1.0);         		// in % of length
		break;
	case 82:
	case 83:
		tofTestBeamClust->SetTRefDifMax(1000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		//in % of length   
		break;
	case 92:
	case 93:
		tofTestBeamClust->SetTRefDifMax(600.);     		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length   
		break;

	case 4:                                      		// velocity dependence (DelTOF)
		tofTestBeamClust->SetTRefDifMax(6000.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.5);        		// in % of length
		break;
	case 14:
		tofTestBeamClust->SetTRefDifMax(2500.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(1.);         		// in % of length
		break;
	case 24:
		tofTestBeamClust->SetTRefDifMax(1500.);    		// in ps 
		tofTestBeamClust->PosYMaxScal(0.8);        		// in % of length
		break;
	case 44:
		tofTestBeamClust->SetTRefDifMax(1000.);    		// in ps 
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
									TOF TestBeam Analysis (Optional)
	************************************************************************** */
	CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis", iVerbose);

   //CbmTofAnaTestbeam defaults 
	tofAnaTestbeam->SetReqTrg(-1);   						// 0 - no selection
	tofAnaTestbeam->SetCorMode(RefSel); 					// 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp, 5 - Mstrip, 6 - Mstrip, 7 - ln(TOT0), 8 - ln(TOT4) (iGenCor is same as iCal in iter_hits.sh, appears on Mat04D4best.pdf)
   tofAnaTestbeam->SetHitDistMin(3.);  					// Minimal spatial distance of correlated hits
   tofAnaTestbeam->SetDTDia(0.);       					// Max time-difference b/w diamonds
   //tofAnaTestbeam->SetDTD4MAX(0.);  						// Max time-difference b/w MRef & Diamond
   tofAnaTestbeam->SetCalParFileName(cAnaFile);			// Input file name with calibration parameters
	tofAnaTestbeam->SetEnableMatchPosScaling(kFALSE);  // disable projective geometry Ansatz 
	
	//Means
   tofAnaTestbeam->SetDXMean(0.);							// DX in cm
   tofAnaTestbeam->SetDYMean(0.);							// DY in cm
   tofAnaTestbeam->SetDTMean(0.);      					// DT in ps
   
   //Widths
   tofAnaTestbeam->SetDXWidth(1.5);							// 3.5, DX in cm
   tofAnaTestbeam->SetDYWidth(1.5);							// 3.5, DY in cm
   tofAnaTestbeam->SetDTWidth(90.);    					// 100., DT in ps
   
   //Multiplicities
   tofAnaTestbeam->SetMul0Max(20);     					// 100, Max Multiplicity in Dut
   tofAnaTestbeam->SetMul4Max(2);     						// Max Multiplicity in MRef
   tofAnaTestbeam->SetMulDMax(2);     						// Max Multiplicity in Diamond
	
	//Parameter Shifts
 	tofAnaTestbeam->SetTShift(0.);     						// General time-shift of all hits
	tofAnaTestbeam->SetSel2TOff(0.);    					// Offset for matching Sel2 time
	tofAnaTestbeam->SetTOffD4(16000.);    					// Offset to calibrate TD4 to time-of-flight
   
   //MRef (4) Selector
   tofAnaTestbeam->SetCh4Sel(15.);                		// 15., Center of selected strip numbers
   tofAnaTestbeam->SetDCh4Sel(15.);      					// 15., Width of selected strip numbers
   tofAnaTestbeam->SetPosY4Sel(0.5);    					// 0.5, Fraction of Y-size
 	
 	//Sel2 (S2) Selector
   tofAnaTestbeam->SetChS2Sel(15.);     					// 15., Center of selected strip numbers
   tofAnaTestbeam->SetDChS2Sel(15.);   					// 15., Width of selected strip numbers
 	tofAnaTestbeam->SetPosYS2Sel(0.5);   					// 0.5, Fraction of Y-size
   
   //Chi2 Limits
	tofAnaTestbeam->SetChi2Lim(20.);     					// Chi2 Limit to accept Selector conincidence; increase efficiency
	tofAnaTestbeam->SetChi2Lim2(10.);     					// Chi2 Limit to accept MRef-Sel2 conincidence; narrow down area
		
	/* *************************************************************************  
		 		iRSel, iRSelTyp, iRSelSm, iRSelRpc based on iSel2=-921
	************************************************************************* */
   Int_t iBRef=iCalSet%1000;
   Int_t iSet = (iCalSet - iBRef)/1000;
   cout << "dispatch: iCalSet = " << iCalSet << ", iSet = " << iSet << ", iBRef = " << iBRef << endl;
   
	Int_t iRSel=0, iRSelTyp=0, iRSelSm=0, iRSelRpc=0;
   if(iSel2>=0){
     iRSel=iBRef;     // use 'beam' reference
   }else{
     iSel2=-iSel2;
     iRSel=iSel2;
   }
   
   // define iRSelin, iSel2in
   Int_t iRSelin=iRSel; 
   Int_t iSel2in=iSel2;
   
   // define iRSel, iRSelTyp, iRSelSm, iRSelRpc
   iRSelRpc=iRSel%10;
   iRSelTyp = (iRSel-iRSelRpc)/10;
   iRSelSm=iRSelTyp%10;
   iRSelTyp = (iRSelTyp-iRSelSm)/10;
	iRSel=iRSelTyp;
	
   tofTestBeamClust->SetBeamRefId(iRSelTyp);    		// define Beam reference counter 
   tofTestBeamClust->SetBeamRefSm(iRSelSm);
   tofTestBeamClust->SetBeamRefDet(iRSelRpc);
   tofTestBeamClust->SetBeamAddRefMul(-1);

   tofAnaTestbeam->SetBeamRefSmType(iRSelTyp);
   tofAnaTestbeam->SetBeamRefSmId(iRSelSm);
   tofAnaTestbeam->SetBeamRefRpc(iRSelRpc);
	
	/* *************************************************************************  
		 				iSel2Rpc, iSel2Sm, iSel2 based on iSel2
	************************************************************************* */
   Int_t iSel2Rpc= iSel2%10;
   iSel2=(iSel2-iSel2Rpc)/10;
   Int_t iSel2Sm=iSel2%10;
   iSel2=(iSel2-iSel2Sm)/10;
   if(iSel2 > 0) {
     tofTestBeamClust->SetSel2Id(iSel2); 
     tofTestBeamClust->SetSel2Sm(iSel2Sm); 
     tofTestBeamClust->SetSel2Rpc(iSel2Rpc);

     tofAnaTestbeam->SetMrpcSel2(iSel2);           	// initialization of second selector Mrpc Type 
     tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);       	// initialization of second selector Mrpc SmId
     tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);     	// initialization of second selector Mrpc RpcId
   }
	
	/* *************************************************************************  
		iDut, iRef (~iMRef) based on iCalSet=901900-920, iSet=901900, iRSel = 9
	************************************************************************* */
   Int_t iRef = iSet %1000;
   Int_t iDut = (iSet - iRef)/1000;
   Int_t iDutRpc = iDut%10;
   iDut = (iDut - iDutRpc)/10;
   Int_t iDutSm = iDut%10;
   iDut = (iDut - iDutSm)/10;

   tofTestBeamClust->SetDutId(iDut);						// Device Under Test (Dut) for Clusterizer
   tofTestBeamClust->SetDutSm(iDutSm);						// Device Under Test (Dut) for Clusterizer
   tofTestBeamClust->SetDutRpc(iDutRpc);					// Device Under Test (Dut) for Clusterizer

   Int_t iRefRpc = iRef%10;
   iRef = (iRef - iRefRpc)/10;
   Int_t iRefSm = iRef%10;
   iRef = (iRef - iRefSm)/10;

   tofTestBeamClust->SetSelId(iRef);						// Selector RPC (Sel) for Clusterizer
   tofTestBeamClust->SetSelSm(iRefSm);						// Selector RPC (Sel) for Clusterizer
   tofTestBeamClust->SetSelRpc(iRefRpc);					// Selector RPC (Sel) for Clusterizer

   tofAnaTestbeam->SetDut(iDut);              			// Device Under Test (Dut) for AnaTestBeam
   tofAnaTestbeam->SetDutSm(iDutSm);          			// Device Under Test (Dut) for AnaTestBeam
   tofAnaTestbeam->SetDutRpc(iDutRpc);        			// Device Under Test (Dut) for AnaTestBeam
    
   tofAnaTestbeam->SetMrpcRef(iRef);          			// Reference RPC (MRef) for AnaTestBeam
   tofAnaTestbeam->SetMrpcRefSm(iRefSm);      			// Reference RPC (MRef) for AnaTestBeam
   tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);    			// Reference RPC (MRef) for AnaTestBeam

	cout << endl;
	cout << "dispatch: iRSel = " << iRSel << ", iRSelSm = " << iRSelSm << ", iRSelRpc = " << iRSelRpc << ", iRSelTyp = " << iRSelTyp << endl;
	cout << "dispatch:  iDut = " << iDut << ",  iDutSm = " << iDutSm << ",   iDutRpc = " << iDutRpc << endl;
	cout << "dispatch:  iRef = " << iRef << ",  iRefSm = " << iRefSm << ",   iRefRpc = " << iRefRpc << endl;
	cout << "dispatch: iSel2 = " << iSel2 << ", iSel2Sm = " << iSel2Sm << ",  iSel2Rpc = " << iSel2Rpc << endl;     
	/* **************************************************************************
	In init_calib.sh/iter_calib.sh the parmeter iCalSet=iDutiMRefiBRef==901900921,
	so iSet == iDutiMRef == 901900 etc...
	************************************************************************** */
	cout << "dispatch: switch(iSet) = " << iSet << ", switch(iRSel <<->> iBRef) = " << iRSel << endl << endl;
	switch (iSet) {
	
	case 601600:		// iSel= XXX600
   case 900600:
   case 901600:
   case 910600:
   case 921600:
		cout << "Run with iSet = " << iSet << ",	iRSel = " << iRSel << " and iSel2in = " << iSel2in << endl << endl;
		switch (iRSel) {
			case 3:		// iRSel
				//tofTestBeamClust->SetBeamAddRefMul(1);
				tofAnaTestbeam->SetTShift(1950.); 						// initialization
				tofAnaTestbeam->SetTOffD4(13000.);  					// initialization
				tofAnaTestbeam->SetSel2TOff(2070.);  					// Shift Sel2 time peak to 0
				break;		
			case 4:		// iRSel
				//tofTestBeamClust->SetBeamAddRefMul(1);
				tofAnaTestbeam->SetTShift(-1200.);   					// Shift DTD4 to 0
				tofAnaTestbeam->SetSel2TOff(-1290.);     				// Shift Sel2 time peak to 0
				tofAnaTestbeam->SetTOffD4(16000.);   					// Shift DTD4 to physical value
				break;
			case 5:		// iRSel
				//tofTestBeamClust->SetBeamAddRefMul(1);
				tofAnaTestbeam->SetTShift(200.);     					// Shift DTD4 to 0
				tofAnaTestbeam->SetSel2TOff(0.);     					// Shift Sel2 time peak to 0
				tofAnaTestbeam->SetTOffD4(16000.);   					// Shift DTD4 to physical value
				break;
				
			case 6:		// iRSel
				//tofTestBeamClust->SetBeamAddRefMul(1);
				switch (iSel2in) { 											// iSel2in = iSel2 = iBRef = 921 line # 269
					case 600: 
						tofAnaTestbeam->SetTShift(200.);     			// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(500.);     		// Shift Sel2 time peak to 0
						tofAnaTestbeam->SetTOffD4(16000.);    			// Shift DTD4 to physical value
						break;

					case 601:
						tofAnaTestbeam->SetTShift(-964.5);     		// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-920.);   			// Shift Sel2 time peak to 0
						tofAnaTestbeam->SetTOffD4(16000.);   			// Shift DTD4 to physical value
						break;

					default:
						;
				}//end-switch(iSel2in)
				
				// Specific parameters for iRSel = 6
     			if ((1)) {
		  			tofAnaTestbeam->SetChi2Lim(20.);     				// initialization of Chi2 selection limit  
					tofAnaTestbeam->SetChi2Lim2(10.);     		// initialization of Chi2 selection limit
				
					//Widths
					tofAnaTestbeam->SetDXWidth(1.5);						// 1.5; DX in cm
					tofAnaTestbeam->SetDYWidth(1.5);						// 1.5; DY in cm
					tofAnaTestbeam->SetDTWidth(200.);    				// 100; DT in ps

					//Multiplicities
					tofAnaTestbeam->SetMul0Max(20);     				// 10; Max Multiplicity in Dut
					tofAnaTestbeam->SetMul4Max(2.);     				// 10; Max Multiplicity in MRef
					tofAnaTestbeam->SetMulDMax(2.);     				// 10; Max Multiplicity in Diamond
				
					//MRef (4) Selector
					tofAnaTestbeam->SetCh4Sel(15.);                	// Center of selected strip numbers
					tofAnaTestbeam->SetDCh4Sel(15.0);      			// Width of selected strip numbers
					tofAnaTestbeam->SetPosY4Sel(0.50);    				// Fraction of Y-size

					//Sel2 (S2) Selector
					tofAnaTestbeam->SetChS2Sel(15.);     				// Center of selected strip numbers
					tofAnaTestbeam->SetDChS2Sel(15.);   				// Width of selected strip numbers
					tofAnaTestbeam->SetPosYS2Sel(0.5);   				// Fraction of Y-size
				}//end-if(1)
				break;
		
			case 9:
				//tofTestBeamClust->SetBeamAddRefMul(1);
				switch (iSel2in) {
			  		case 900: 
						tofAnaTestbeam->SetTShift(-1332.);     		// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-1246.);   		// Shift Sel2 time peak to 0,
						tofAnaTestbeam->SetTOffD4(16000.);   			// Shift DTD4 to physical value
						break;

					case 901:
						tofAnaTestbeam->SetTShift(-30.);     			// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(10.);   			// Shift Sel2 time peak to 0,
						tofAnaTestbeam->SetTOffD4(33000.);   			// Shift DTD4 to physical value
						break;

					case 910: 
						tofAnaTestbeam->SetTShift(-1332.);     		// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-1246.);   		// Shift Sel2 time peak to 0,
						tofAnaTestbeam->SetTOffD4(16000.);   			// Shift DTD4 to physical value
						break;

					case 921: 
						tofAnaTestbeam->SetTShift(-1332.);     		// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-1246.);   		// Shift Sel2 time peak to 0,
						tofAnaTestbeam->SetTOffD4(16000.);   			// Shift DTD4 to physical value
						break;

					default:
					  	;
				}//end-switch(iSel2in)
				
				// Specific parameters for iRSel = 9
     			if ((1)) {
		  			tofAnaTestbeam->SetChi2Lim(20.);     				// initialization of Chi2 selection limit  
					tofAnaTestbeam->SetChi2Lim2(10.);     				// initialization of Chi2 selection limit
				
					//Widths
					tofAnaTestbeam->SetDXWidth(1.5);						// 1.5; DX in cm
					tofAnaTestbeam->SetDYWidth(1.5);						// 1.5; DY in cm
					tofAnaTestbeam->SetDTWidth(200.);    				// 100; DT in ps

					//Multiplicities
					tofAnaTestbeam->SetMul0Max(20);     				// 10; Max Multiplicity in Dut
					tofAnaTestbeam->SetMul4Max(2.);     				// 10; Max Multiplicity in MRef
					tofAnaTestbeam->SetMulDMax(2.);     				// 10; Max Multiplicity in Diamond
				
					//MRef (4) Selector
					tofAnaTestbeam->SetCh4Sel(15.);                	// Center of selected strip numbers
					tofAnaTestbeam->SetDCh4Sel(15.);      				// Width of selected strip numbers
					tofAnaTestbeam->SetPosY4Sel(0.5);    				// Fraction of Y-size

					//Sel2 (S2) Selector
					tofAnaTestbeam->SetChS2Sel(15.);     				// Center of selected strip numbers
					tofAnaTestbeam->SetDChS2Sel(15.);   				// Width of selected strip numbers
					tofAnaTestbeam->SetPosYS2Sel(0.5);   				// Fraction of Y-size
				}//end-if(1)
				break;

         default:		// iRSel
				cout << "Undefined setup! iRSel: " << iRSel << " doesn't exits" << endl;
				return;
				;
     	}//end-switch(iRSel)
     	break;

	case 600601:      // iSel= XXX601
	case 900601:
	case 901601:
	case 910601:
	case 921601:
		cout << "Run with iSet = " << iSet << ",	iRSel = " << iRSel << " and iSel2in = " << iSel2in << endl << endl;
		switch (iRSel) {
			case 3:		// iRSel
				//tofTestBeamClust->SetBeamAddRefMul(1);
				tofAnaTestbeam->SetTShift(1950.); 						// initialization
				tofAnaTestbeam->SetTOffD4(13000.);  					// initialization
				tofAnaTestbeam->SetSel2TOff(2070.);  					// Shift Sel2 time peak to 0
				break;		
			case 4:		// iRSel
				//tofTestBeamClust->SetBeamAddRefMul(1);
				tofAnaTestbeam->SetTShift(-1200.);   					// Shift DTD4 to 0
				tofAnaTestbeam->SetSel2TOff(-1290.);     				// Shift Sel2 time peak to 0
				tofAnaTestbeam->SetTOffD4(16000.);   					// Shift DTD4 to physical value
				break;
			case 5:		// iRSel
				//tofTestBeamClust->SetBeamAddRefMul(1);
				tofAnaTestbeam->SetTShift(200.);     					// Shift DTD4 to 0
				tofAnaTestbeam->SetSel2TOff(0.);     					// Shift Sel2 time peak to 0
				tofAnaTestbeam->SetTOffD4(16000.);   					// Shift DTD4 to physical value
				break;
			case 6:		// iRSel
				//tofTestBeamClust->SetBeamAddRefMul(1);
				switch (iSel2in) { 											// iSel2in = iSel2 = iBRef = 921 line # 269
					case 600: 
						tofAnaTestbeam->SetTShift(200.);     			// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(500.);     		// Shift Sel2 time peak to 0
						tofAnaTestbeam->SetTOffD4(16000.);    			// Shift DTD4 to physical value
						break;

					case 601:
						tofAnaTestbeam->SetTShift(-964.5);     		// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-920.);   			// Shift Sel2 time peak to 0
						tofAnaTestbeam->SetTOffD4(16000.);   			// Shift DTD4 to physical value
						break;

					default:
						;
				}//end-switch(iSel2in))
				
				// Specific parameters for iRSel = 6
     			if ((1)) {
		  			tofAnaTestbeam->SetChi2Lim(20.);     				// initialization of Chi2 selection limit  
					tofAnaTestbeam->SetChi2Lim2(10.);     				// initialization of Chi2 selection limit
				
					//Widths
					tofAnaTestbeam->SetDXWidth(1.5);						// 1.5; DX in cm
					tofAnaTestbeam->SetDYWidth(1.5);						// 1.5; DY in cm
					tofAnaTestbeam->SetDTWidth(200.);    				// 100; DT in ps

					//Multiplicities
					tofAnaTestbeam->SetMul0Max(20);     				// 10; Max Multiplicity in Dut
					tofAnaTestbeam->SetMul4Max(2.);     				// 10; Max Multiplicity in MRef
					tofAnaTestbeam->SetMulDMax(2.);     				// 10; Max Multiplicity in Diamond
				
					//MRef (4) Selector
					tofAnaTestbeam->SetCh4Sel(15.);                	// Center of selected strip numbers
					tofAnaTestbeam->SetDCh4Sel(15.);      				// Width of selected strip numbers
					tofAnaTestbeam->SetPosY4Sel(0.5);    				// Fraction of Y-size

					//Sel2 (S2) Selector
					tofAnaTestbeam->SetChS2Sel(15.);     				// Center of selected strip numbers
					tofAnaTestbeam->SetDChS2Sel(15.);   				// Width of selected strip numbers
					tofAnaTestbeam->SetPosYS2Sel(0.5);   				// Fraction of Y-size
				}//end-if(1)
				break;

	 		case 9:		// iRSel
				//tofTestBeamClust->SetBeamAddRefMul(1);
				switch (iSel2in) {
			  		case 900: 
						tofAnaTestbeam->SetTShift(-1332.);     		// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-1246.);   		// Shift Sel2 time peak to 0,
						tofAnaTestbeam->SetTOffD4(16000.);   			// Shift DTD4 to physical value
						break;

					case 901:
						tofAnaTestbeam->SetTShift(55.);     			// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-90.);   			// Shift Sel2 time peak to 0,
						tofAnaTestbeam->SetTOffD4(33000.);   			// Shift DTD4 to physical value
						break;

					case 910: 
						tofAnaTestbeam->SetTShift(-1332.);     		// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-1246.);   		// Shift Sel2 time peak to 0,
						tofAnaTestbeam->SetTOffD4(16000.);   			// Shift DTD4 to physical value
						break;

					case 921: 
						tofAnaTestbeam->SetTShift(-1332.);     		// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-1246.);   		// Shift Sel2 time peak to 0,
						tofAnaTestbeam->SetTOffD4(16000.);   			// Shift DTD4 to physical value
						break;

					default:
					  	;
				}//end-switch(iSel2in)
				
				// Specific parameters for iRSel = 9
     			if ((1)) {
		  			tofAnaTestbeam->SetChi2Lim(20.);     				// initialization of Chi2 selection limit  
					tofAnaTestbeam->SetChi2Lim2(10.);     				// initialization of Chi2 selection limit
				
					//Widths
					tofAnaTestbeam->SetDXWidth(1.5);						// 1.5; DX in cm
					tofAnaTestbeam->SetDYWidth(1.5);						// 1.5; DY in cm
					tofAnaTestbeam->SetDTWidth(200.);    				// 100; DT in ps

					//Multiplicities
					tofAnaTestbeam->SetMul0Max(20);     				// 10; Max Multiplicity in Dut
					tofAnaTestbeam->SetMul4Max(2.);     				// 10; Max Multiplicity in MRef
					tofAnaTestbeam->SetMulDMax(2.);     				// 10; Max Multiplicity in Diamond
				
					//MRef (4) Selector
					tofAnaTestbeam->SetCh4Sel(15.);                	// Center of selected strip numbers
					tofAnaTestbeam->SetDCh4Sel(15.);      				// Width of selected strip numbers
					tofAnaTestbeam->SetPosY4Sel(0.5);    				// Fraction of Y-size

					//Sel2 (S2) Selector
					tofAnaTestbeam->SetChS2Sel(15.);     				// Center of selected strip numbers
					tofAnaTestbeam->SetDChS2Sel(15.);   				// Width of selected strip numbers
					tofAnaTestbeam->SetPosYS2Sel(0.5);   				// Fraction of Y-size
				}//end-if(1)
				break;
			
			default:		// iRSel
				cout << "Undefined setup! iRSel: " << iRSel << " doesn't exits" << endl;
				return;
     }//end-switch(iRSel)
     break;

   default:
     cout << "Define Analysis Setup! iSel: " << iSel << endl << endl;
     return;
		
	}//end-switch(iSet)
	run->AddTask(tofAnaTestbeam);

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
							  				Intialise And Run	   
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
	gInterpreter->ProcessLine(Display_Funct);
		
	gROOT->LoadMacro("pl_over_cluSel.C");
	gROOT->LoadMacro("pl_over_clu.C");
	gROOT->LoadMacro("pl_all_dTSel.C");
	gROOT->LoadMacro("pl_over_MatD4sel.C");

	// Plotting iSet:
	switch (iSet) {
	case 921920:
	case 920921:
	case 900900:
	case 901900:
	case 921900:
	case 901901:
	case 600601:
	case 601600:
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
		
		// void pl_over_MatD4sel()
		gInterpreter->ProcessLine("pl_over_MatD4sel()");
		break;

	default:
		cout << "iSet = " << iSet << "\tnot found for plotting..." << endl;
		;
	}//end-switch(iSet)
	
	gInterpreter->ProcessLine(Display_Funct);

}//end-macro
