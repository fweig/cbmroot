void ana_hits(Int_t nEvents=10000000, Int_t iSel=1, Int_t iGenCor=1, char *cFileId="CbmTofPiHd_29Aug1401", char *cSet="901900921_-921", Int_t iSel2=0, Int_t iTrackingSetup=0, Double_t dScalFac=1., Double_t dChi2Lim2=10.) 
{
   Int_t iVerbose = 1;
   // Specify log level (INFO, DEBUG, DEBUG1, ...)
   //TString logLevel = "FATAL";
   //TString logLevel = "ERROR";
   TString logLevel = "INFO";
   //TString logLevel = "DEBUG"; 
   //TString logLevel = "DEBUG1";
   //TString logLevel = "DEBUG2";
   //TString logLevel = "DEBUG3";
   FairLogger* log;

   TString workDir       = gSystem->Getenv("VMCWORKDIR");
   TString paramDir      = workDir  + "/macro/tof/beamtime/lab16";
   TString ParFile       = paramDir + "/unpack_" + cFileId + ".params.root";
   TString InputFile     = paramDir + "/unpack_" + cFileId + ".out.root";
   TString InputDigiFile = paramDir + "/digi_" + cFileId + Form("_%s",cSet) + ".out.root";
   TString OutputFile    = paramDir + "/hits_" + cFileId + Form("_%s_%06d_%03d",cSet,iSel,iSel2) + ".out.root";
   TString cAnaFile=Form("%s_%s_%06d_%03d_tofAnaTestBeam.hst.root",cFileId,cSet,iSel,iSel2);
   TString cHstFile=paramDir + Form("/hst/%s_%s_%06d_%03d_tofAna.hst.root",cFileId,cSet,iSel,iSel2);
   TString cTrkFile=Form("%s_tofFindTracks.hst.root",cFileId);

   cout << " InputDigiFile = "
	<< InputDigiFile
	<< endl;

   Int_t iCalSet=0;
   TString SetIn= cSet;
   TString CalSet ( SetIn(0,9) ); 
   cout << "CalSet identified as " << CalSet << endl;
   iCalSet = atoi((const char *)CalSet);
   cout  << "iCalSet = " << iCalSet << endl;

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapLab16.txt";
   parFileList->Add(&mapParFile);

   TString TofGeo="v15d";  									//default
   
   TString FId=cFileId;
   if(FId.Contains("CernSps02Mar")){
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

   // TObjString tofDigiBdfFile =  paramDir + "/tof.digibdf.par";
   // TObjString tofDigiBdfFile =  paramDir + "/tof." + FPar + "digibdf.par";
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

   // -------------------------  Reconstruction Run   -------------------------
	FairRunAna *run= new FairRunAna();
	cout << "InputFile:     "<<InputFile.Data()<<endl;
	cout << "InputDigiFile: "<<InputDigiFile.Data()<<endl;

	run->SetInputFile(InputFile.Data());
	run->AddFriend(InputDigiFile.Data());
	//run->SetInputFile(InputDigiFile.Data());
	//run->AddFriend(InputFile.Data());
	run->SetOutputFile(OutputFile);
	gLogger->SetLogScreenLevel(logLevel.Data());

 	// =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
   //run->AddTask(tofMapping);

   // =========================================================================
   // ===                        Tracking                                   ===
   // =========================================================================
   /*
   CbmStsDigitize* stsDigitize = new CbmStsDigitize();//necessary for kalman !!
   CbmKF* kalman = new CbmKF();

   CbmTofTrackFinder* tofTrackFinder= new CbmTofTrackFinderNN();
   tofTrackFinder->SetMaxTofTimeDifference(5000.); 	// in ps/cm 
   tofTrackFinder->SetTxLIM(0.05);                 	// max slope dx/dz
   tofTrackFinder->SetTyLIM(0.05);                  	// max dev from mean slope dy/dz
   tofTrackFinder->SetTyMean(0.1);                 	// mean slope dy/dz
   tofTrackFinder->SetSIGLIM(2.);                  	// max matching chi2

   CbmTofTrackFitter* tofTrackFitter= new CbmTofTrackFitterKF(0,211);
   TFitter *MyFit = new TFitter(1);                	// initialize Minuit
   tofTrackFinder->SetFitter(tofTrackFitter);
   CbmTofFindTracks* tofFindTracks  = new CbmTofFindTracks("TOF Track Finder");
   tofFindTracks->UseFinder(tofTrackFinder);
   tofFindTracks->UseFitter(tofTrackFitter);
   tofFindTracks->SetCorMode(iGenCor);           		// valid options: 0,1,2
   tofFindTracks->SetTtTarg(33.7);               		// target value for inverse velocity, > 33.3 !
   tofFindTracks->SetCalParFileName(cTrkFile);   		// Tracker parameter value file name
     
   tofFindTracks->SetT0MAX(dScalFac*1000.);       		// in ps
   tofFindTracks->SetSIGT(100.);                  		// default in ps
   tofFindTracks->SetSIGX(0.5);                   		// default in cm
   tofFindTracks->SetSIGY(1.5);                   		// default in cm 
   tofFindTracks->SetSIGZ(5.0);                   		// default in cm 

   switch (iTrackingSetup){
   case 0:                                       		// bypass mode
     tofFindTracks->SetNStations(1);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     break;

   case 1:                                       		// calibration mode
     tofFindTracks->SetMinNofHits(6);
     tofFindTracks->SetNStations(8);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 4, 0, 0);           // P5 
     tofFindTracks->SetStation(2, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(3, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(4, 9, 1, 0);           // 
     //tofFindTracks->SetStation(4, 9, 1, 1);         // broken
     tofFindTracks->SetStation(5, 9, 0, 0);           //  
     tofFindTracks->SetStation(6, 9, 0, 1);           //  
     tofFindTracks->SetStation(7, 3, 0, 0);           // P2 
     tofTrackFinder->SetSIGT(200.);                   // in ps
     break;

   case 2:                                       		// calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(6);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(2, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(3, 9, 1, 0);           // 
     tofFindTracks->SetStation(4, 9, 0, 0);           //  
     tofFindTracks->SetStation(5, 9, 0, 1);           // 
     tofTrackFinder->SetSIGT(150.);                  	// in ps
     break;

   case 3:                                            // calibration mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(1, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(2, 9, 0, 0);           //  
     tofFindTracks->SetStation(3, 9, 0, 1);           // 
     tofTrackFinder->SetSIGT(150.);                  	// in ps
     break;

   case 4:                                            // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(1, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(2, 9, 0, 0);           //  
     tofFindTracks->SetStation(3, 9, 0, 1);           // 
     tofTrackFinder->SetSIGT(150.);                  	// in ps
     break;

   case 5:                                            // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStation(0, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(1, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(2, 9, 0, 0);           //  
     tofFindTracks->SetStation(3, 9, 0, 1);           // 
     tofFindTracks->SetStation(4, 3, 0, 0);           // P2 
     tofTrackFinder->SetSIGT(150.);                  	// in ps
     break;

   case 10:                                       		// full lower setup
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(14);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 8, 0, 2);           // THUpad 
     tofFindTracks->SetStation(2, 8, 0, 3);           // THUpad
     tofFindTracks->SetStation(3, 8, 0, 0);           // THUpad
     tofFindTracks->SetStation(4, 8, 0, 1);           // THUpad
     tofFindTracks->SetStation(5, 8, 0, 4);           // THUpad
     tofFindTracks->SetStation(6, 8, 0, 5);           // THUpad
     tofFindTracks->SetStation(7, 6, 0, 0);           // Buc2015
     tofFindTracks->SetStation(8, 6, 0, 1);           // Buc2015
     tofFindTracks->SetStation(9, 7, 0, 0);           // Buc2012
     tofFindTracks->SetStation(10, 7, 0, 1);          // Buc2012
     tofFindTracks->SetStation(11, 7, 0, 2);          // Buc2012
     tofFindTracks->SetStation(12, 7, 0, 3);          // Buc2012
     tofFindTracks->SetStation(13, 1, 0, 0);          // BucRef  
     break;

   case 11:                                       		// partial lower setup
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 6, 0, 0);           // Buc2015
     tofFindTracks->SetStation(2, 6, 0, 1);           // Buc2015
     tofFindTracks->SetStation(3, 1, 0, 0);           // BucRef  
     break;

   default:
     cout << "Tracking setup "<<iTrackingSetup<<" not implemented "<<endl;
     return;
     ;
   }
   run->AddTask(tofFindTracks);
   
   */
   
   // =========================================================================
   // ===                       		Analysis                                ===
   // =========================================================================

   CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);
   
   //defaults
   tofAnaTestbeam->SetCorMode(iGenCor); 					// 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp, 5 - Mstrip, 6 - Mstrip, 7 - ln(TOT0), 8 - ln(TOT4) (iGenCor is same as iCal in iter_hits.sh, appears on Mat04D4best.pdf)
   tofAnaTestbeam->SetHitDistMin(3.);  					// Minimal spatial distance of correlated hits
   tofAnaTestbeam->SetDTDia(0.);       					// Max time-difference b/w diamonds
   tofAnaTestbeam->SetDTD4MAX(6000.);  					// Max time-difference b/w MRef & Diamond
   tofAnaTestbeam->SetCalParFileName(cAnaFile);			// Input file name with calibration parameters
   tofAnaTestbeam->SetEnableMatchPosScaling(kFALSE);  // Disable projective geometry Ansatz for Cosmics
   
   //Means
   tofAnaTestbeam->SetDXMean(0.);							// DX in cm
   tofAnaTestbeam->SetDYMean(0.);							// DY in cm
   tofAnaTestbeam->SetDTMean(0.);      					// DT in ps
   
   //Widths
   tofAnaTestbeam->SetDXWidth(3.5);							// DX in cm
   tofAnaTestbeam->SetDYWidth(3.5);							// DY in cm
   tofAnaTestbeam->SetDTWidth(100.);    					// DT in ps
   
   //Multiplicities
   tofAnaTestbeam->SetMul0Max(100);     					// Max Multiplicity in Dut
   tofAnaTestbeam->SetMul4Max(2);     						// Max Multiplicity in MRef
   tofAnaTestbeam->SetMulDMax(2);     						// Max Multiplicity in Diamond
   	
   //MRef (4) Selector
   tofAnaTestbeam->SetCh4Sel(15.);                		// Center of selected strip numbers
   tofAnaTestbeam->SetDCh4Sel(15.*dScalFac);      		// Width of selected strip numbers
   tofAnaTestbeam->SetPosY4Sel(0.5*dScalFac);    		// Fraction of Y-size
 	
 	//Sel2 (S2) Selector
   tofAnaTestbeam->SetChS2Sel(15.);     					// Center of selected strip numbers
   tofAnaTestbeam->SetDChS2Sel(15.*dScalFac);   		// Width of selected strip numbers
 	tofAnaTestbeam->SetPosYS2Sel(0.5*dScalFac);   		// Fraction of Y-size
 	
 	//Parameter Shifts
 	tofAnaTestbeam->SetTShift(0.);     						// General time-shift of all hits
	tofAnaTestbeam->SetSel2TOff(0.);     					// Offset for matching Sel2 time
	tofAnaTestbeam->SetTOffD4(16000.);    					// Offset to calibrate TD4 to time-of-flight
	
   //Chi2 Limits
	tofAnaTestbeam->SetChi2Lim(30.);     					// Chi2 Limit to accept Selector conincidence; increase efficiency
	tofAnaTestbeam->SetChi2Lim2(10.);     					// Chi2 Limit to accept MRef-Sel2 conincidence; narrow down area
	
	/* *************************************************************************  
		 			iRSel, iRSelTyp, iRSelSm, iRSelRpc, iRSelin, iSel2in
	************************************************************************* */
   Int_t iBRef=iCalSet%1000;
   Int_t iSet = (iCalSet - iBRef)/1000;
   cout << "dispatch: iCalSet = " << iCalSet << ", iSet = " << iSet << ", iBRef = " << iBRef << endl;
      
  	Int_t iRSel=0, iRSelSm=0, iRSelRpc=0, iSel2in=0;
  	
   if(iSel2 >= 0) {
     iRSel=iBRef;     											// use 'beam' reference
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
   iRSel = iRSelTyp;
   
   tofAnaTestbeam->SetBeamRefSmType(iRSelTyp); 			// Beam Reference (BRef) Type
   tofAnaTestbeam->SetBeamRefSmId(iRSelSm);				// Beam Reference (BRef) Module
   tofAnaTestbeam->SetBeamRefRpc(iRSelRpc);				// Beam Reference (BRef) Rpc
   
   /* *************************************************************************  
		 				iSel2Rpc, iSel2Sm, iSel2 based on iSel2
	************************************************************************* */
   Int_t iSel2Rpc= iSel2%10;
   iSel2=(iSel2-iSel2Rpc)/10;
   Int_t iSel2Sm=iSel2%10;
   iSel2=(iSel2-iSel2Sm)/10;
   
   if(iSel2 > 0) {
     tofAnaTestbeam->SetMrpcSel2(iSel2);           // initialization of second selector Mrpc Type 
     tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);       // initialization of second selector Mrpc Module
     tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);     // initialization of second selector Mrpc RpcId
   }
	
	// Should be shifted up in defaults...
   tofAnaTestbeam->SetSIGLIM(3.);                // max matching chi2
   tofAnaTestbeam->SetSIGT(100.);                // in ps
   tofAnaTestbeam->SetSIGX(1.);                  // in cm
   tofAnaTestbeam->SetSIGY(1.);                  // in cm
	
	/* *************************************************************************  
		iDut, iRef (~iMRef) based on iCalSet=901900-920, iSet=901900, iRSel = 9
	************************************************************************* */
   Int_t iRef = iSel %1000;
   Int_t iDut = (iSel - iRef)/1000;
   Int_t iDutRpc = iDut%10;
   iDut = (iDut - iDutRpc)/10;
   Int_t iDutSm = iDut%10;
   iDut = (iDut - iDutSm)/10;
   Int_t iRefRpc = iRef%10;
   iRef = (iRef - iRefRpc)/10;
   Int_t iRefSm = iRef%10;
   iRef = (iRef - iRefSm)/10;

	tofAnaTestbeam->SetDut(iDut);              			// Device Under Test (Dut) Type
	tofAnaTestbeam->SetDutSm(iDutSm);          			// Device Under Test (Dut) Module
	tofAnaTestbeam->SetDutRpc(iDutRpc);        			// Device Under Test (Dut) Rpc
	   
	tofAnaTestbeam->SetMrpcRef(iRef);          			// Reference MRPC (MRef) Type
	tofAnaTestbeam->SetMrpcRefSm(iRefSm);      			// Reference MRPC (MRef) Module
	tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);    			// Reference MRPC (MRef) Rpc 

   cout << endl;
	cout << "dispatch: iRSel = " << iRSel << ", iRSelSm = " << iRSelSm << ", iRSelRpc = " << iRSelRpc << endl;
	cout << "dispatch:  iDut = " << iDut << ",  iDutSm = " << iDutSm << ",   iDutRpc = " << iDutRpc << endl;
	cout << "dispatch:  iRef = " << iRef << ",  iRefSm = " << iRefSm << ",   iRefRpc = " << iRefRpc << endl;
	cout << "dispatch: iSel2 = " << iSel2 << ", iSel2Sm = " << iSel2Sm << ", iSel2Rpc = " << iSel2Rpc << endl;
   
	/* **************************************************************************
	In init_calib.sh/iter_calib.sh the parmeter iCalSet=iDutiMRefiBRef==901900921,
	so iSet == iDutiMRef == 901900 etc...
	************************************************************************** */
	cout << "dispatch: iSel  = " << iSel << ",	iRSel = " << iRSel << ",	iSel2in = " << iSel2in << endl;
   switch (iSel) {
   
   case 601600:		// iSel= XXX600
   case 900600:
   case 901600:
   case 910600:
   case 921600:
		cout << "Run with iSel = " << iSel << ",	iRSel = " << iRSel << " and iSel2in = " << iSel2in << endl << endl;
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
					tofAnaTestbeam->SetChi2Lim2(dChi2Lim2);     		// initialization of Chi2 selection limit
				
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
					tofAnaTestbeam->SetDCh4Sel(15.*dScalFac);      	// Width of selected strip numbers
					tofAnaTestbeam->SetPosY4Sel(0.5*dScalFac);    	// Fraction of Y-size

					//Sel2 (S2) Selector
					tofAnaTestbeam->SetChS2Sel(15.);     				// Center of selected strip numbers
					tofAnaTestbeam->SetDChS2Sel(15.*dScalFac);   	// Width of selected strip numbers
					tofAnaTestbeam->SetPosYS2Sel(0.5*dScalFac);   	// Fraction of Y-size
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
						tofAnaTestbeam->SetTShift(-250.);     			// Shift DTD4 to 0
						tofAnaTestbeam->SetSel2TOff(-150.);   			// Shift Sel2 time peak to 0,
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
		  			tofAnaTestbeam->SetChi2Lim(6.);     				// initialization of Chi2 selection limit  
					tofAnaTestbeam->SetChi2Lim2(dChi2Lim2);     		// initialization of Chi2 selection limit
				
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
					tofAnaTestbeam->SetDCh4Sel(15.*dScalFac);      	// Width of selected strip numbers
					tofAnaTestbeam->SetPosY4Sel(0.5*dScalFac);    	// Fraction of Y-size

					//Sel2 (S2) Selector
					tofAnaTestbeam->SetChS2Sel(15.);     				// Center of selected strip numbers
					tofAnaTestbeam->SetDChS2Sel(15.*dScalFac);   	// Width of selected strip numbers
					tofAnaTestbeam->SetPosYS2Sel(0.5*dScalFac);   	// Fraction of Y-size
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
		cout << "Run with iSel = " << iSel << ",	iRSel = " << iRSel << " and iSel2in = " << iSel2in << endl << endl;
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
		  			tofAnaTestbeam->SetChi2Lim(10.);     				// initialization of Chi2 selection limit  
					tofAnaTestbeam->SetChi2Lim2(dChi2Lim2);     		// initialization of Chi2 selection limit
				
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
					tofAnaTestbeam->SetDCh4Sel(15.*dScalFac);      	// Width of selected strip numbers
					tofAnaTestbeam->SetPosY4Sel(0.5*dScalFac);    	// Fraction of Y-size

					//Sel2 (S2) Selector
					tofAnaTestbeam->SetChS2Sel(15.);     				// Center of selected strip numbers
					tofAnaTestbeam->SetDChS2Sel(15.*dScalFac);   	// Width of selected strip numbers
					tofAnaTestbeam->SetPosYS2Sel(0.5*dScalFac);   	// Fraction of Y-size
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
		  			tofAnaTestbeam->SetChi2Lim(6.);     				// initialization of Chi2 selection limit  
					tofAnaTestbeam->SetChi2Lim2(dChi2Lim2);     		// initialization of Chi2 selection limit
				
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
					tofAnaTestbeam->SetDCh4Sel(15.*dScalFac);      	// Width of selected strip numbers
					tofAnaTestbeam->SetPosY4Sel(0.5*dScalFac);    	// Fraction of Y-size

					//Sel2 (S2) Selector
					tofAnaTestbeam->SetChS2Sel(15.);     				// Center of selected strip numbers
					tofAnaTestbeam->SetDChS2Sel(15.*dScalFac);   	// Width of selected strip numbers
					tofAnaTestbeam->SetPosYS2Sel(0.5*dScalFac);   	// Fraction of Y-size
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
     
   }//end-switch(iSel)
   
   cout << " Initialize TShift to " << tofAnaTestbeam->GetTShift() << endl;
   run->AddTask(tofAnaTestbeam);

   /* **************************************************************************
											CbmTofOnlineDisplay	   
	************************************************************************** */
   
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

	/*
	FairEventManager *fMan= new FairEventManager();
	CbmPixelHitSetDraw *TofHits = new CbmPixelHitSetDraw ("TofHit", kRed, kFullSquare);
	fMan->AddTask(TofHits);   
	fMan->Init();                     
	*/

	/* **************************************************************************
							  				Intialise and Run	   
	************************************************************************** */
	run->Init();
	cout << "Starting run" << endl;
	run->Run(0, nEvents);
	
	/* **************************************************************************
												 Plotting	   
	************************************************************************** */
	
	// Default Display
	TString Display_Status = "pl_over_Mat04D4best.C";
	TString Display_Funct = "pl_over_Mat04D4best(1)";  
	gROOT->LoadMacro(Display_Status);
	gInterpreter->ProcessLine(Display_Funct);

	gROOT->LoadMacro("pl_over_MatD4sel.C");
	gROOT->LoadMacro("pl_eff_XY.C");
	//gROOT->LoadMacro("pl_over_trk.C");
	//gROOT->LoadMacro("pl_calib_trk.C");
	gROOT->LoadMacro("save_hst.C");
	gInterpreter->ProcessLine("pl_eff_XY()");
	gInterpreter->ProcessLine("pl_over_MatD4sel()");
	//gInterpreter->ProcessLine("pl_over_trk(9)");
	//gInterpreter->ProcessLine("pl_calib_trk()");
	TString SaveToHstFile = "save_hst(\"" + cHstFile + "\")";
	gInterpreter->ProcessLine(SaveToHstFile);
}
