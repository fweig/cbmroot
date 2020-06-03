// -----------------------------------------------------------------------------
// ----- ana_calib_hits.C                                                  -----
// -----                                                                   -----
// ----- adapted by C. Simon on 2017-03-11                                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

void ana_calib_hits(Int_t nEvents = 10, Int_t iGenCor = 1, TString cFileId = "CbmTofSps_27Nov1728", TString cCalSet = "920921510_000", Int_t iDut = 920, Int_t iRef = 921, Int_t iBRef = 510, Int_t iSel2 = 0, Double_t dScalFac = 1.) 
{
  TStopwatch timer;
  timer.Start();

  FairLogger::GetLogger();

//  gLogger->SetLogScreenLevel("FATAL");
//  gLogger->SetLogScreenLevel("ERROR");
//  gLogger->SetLogScreenLevel("WARNING");
  gLogger->SetLogScreenLevel("INFO");
//  gLogger->SetLogScreenLevel("DEBUG");
//  gLogger->SetLogScreenLevel("DEBUG1");
//  gLogger->SetLogScreenLevel("DEBUG2");
//  gLogger->SetLogScreenLevel("DEBUG3");

//  gLogger->SetLogVerbosityLevel("LOW");
  gLogger->SetLogVerbosityLevel("MEDIUM");
//  gLogger->SetLogVerbosityLevel("HIGH");

  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  TString plotDir = srcDir + "/macro/beamtime";
  TString TofGeo = "v15c"; 

  TString geoFile = srcDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";

  TObjString tofDigiParFile = (srcDir + "/parameters/tof/tof_" + TofGeo + ".digi.par").Data();
  TObjString tofDigiBdfParFile = (srcDir + "/parameters/tof/tof_" + TofGeo + ".digibdf.par").Data();

  TString InputFile  = "../../../unpack_" + cFileId + ".out.root";
  TString InputDigiFile = "../../digi_" + cFileId + "_" + cCalSet + ".out.root";
  TString OutputFile    = "./hits.out.root";

  TString HistoFile  = "./ana_cluster.hst.root";

  TString InputAnaCalibFile    = "./calib_ana.cor_in.root";
  TString OutputAnaCalibFile   = "./calib_ana.cor_out.root";


  Int_t iSel = iDut*1000 +iRef;

  Int_t iRSel = iBRef;
  Int_t iRSelRpc = iRSel%10;
  iRSel = (iRSel - iRSelRpc)/10;
  Int_t iRSelSm = iRSel%10;
  iRSel = (iRSel - iRSelSm)/10;

  Int_t iSel2Rpc = iSel2%10;
  iSel2 = (iSel2 - iSel2Rpc)/10;
  Int_t iSel2Sm = iSel2%10;
  iSel2 = (iSel2 - iSel2Sm)/10;

  Int_t iDutRpc = iDut%10;
  iDut = (iDut - iDutRpc)/10;
  Int_t iDutSm = iDut%10;
  iDut = (iDut - iDutSm)/10;

  Int_t iRefRpc = iRef%10;
  iRef = (iRef - iRefRpc)/10;
  Int_t iRefSm = iRef%10;
  iRef = (iRef - iRefSm)/10;

  TFile* fgeo = new TFile(geoFile);
  TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
  if(NULL == geoMan)
  {
    cout << "<E> FAIRGeom not found in geoFile"<<endl;
    return;
  }

  TList *parFileList = new TList();
  parFileList->Add(&tofDigiParFile);
  parFileList->Add(&tofDigiBdfParFile);


  FairRunAna* run= new FairRunAna();

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  
  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
  parIo1->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);



  CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis", 1);
  //CbmTofAnaTestbeam defaults  
  tofAnaTestbeam->SetDXMean(0.);
  tofAnaTestbeam->SetDYMean(0.);
  tofAnaTestbeam->SetDTMean(0.);             // in ns
  tofAnaTestbeam->SetDXWidth(0.7);
  tofAnaTestbeam->SetDYWidth(1.0);
  tofAnaTestbeam->SetDTWidth(0.1);           // in ns

  tofAnaTestbeam->SetPosY4Sel(0.5*dScalFac); // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetDTDia(0.);              // Time difference to additional diamond
  tofAnaTestbeam->SetCorMode(iGenCor);       // 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp 
  tofAnaTestbeam->SetMul0Max(10);            // Max Multiplicity in dut 
  tofAnaTestbeam->SetMul4Max(10);            // Max Multiplicity in Ref - RPC 
  tofAnaTestbeam->SetMulDMax(10);            // Max Multiplicity in Diamond
  //tofAnaTestbeam->SetMul0Max(200);
  //tofAnaTestbeam->SetMul4Max(3);
  //tofAnaTestbeam->SetMulDMax(20);
  tofAnaTestbeam->SetHitDistMin(30.);        // initialization
  tofAnaTestbeam->SetDTD4MAX(6.);            // initialization of Max time difference Ref - BRef

  tofAnaTestbeam->SetPosYS2Sel(0.5);         // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetChS2Sel(0.);            // Center of channel selection window
  tofAnaTestbeam->SetDChS2Sel(100.);         // Width  of channel selection window

  //tofAnaTestbeam->SetTShift(-28.);         // initialization
  tofAnaTestbeam->SetSel2TOff(0.45);         // Shift Sel2 time peak to 0
  //tofAnaTestbeam->SetSel2TOff(0.);
  tofAnaTestbeam->SetTOffD4(10.);            // initialization

  tofAnaTestbeam->SetChi2Lim(5.);            // initialization of Chi2 selection limit  
  tofAnaTestbeam->SetChi2Lim2(2.);           // initialization of Chi2 selection limit for Mref-Sel2 pair   

  tofAnaTestbeam->SetSIGLIM(3.);             // max matching chi2
  tofAnaTestbeam->SetSIGT(0.1);              // in ns
  tofAnaTestbeam->SetSIGX(1.);               // in cm
  tofAnaTestbeam->SetSIGY(1.);               // in cm

  tofAnaTestbeam->SetBeamRefSmType(iRSel);
  tofAnaTestbeam->SetBeamRefSmId(iRSelSm);

  tofAnaTestbeam->SetDut(iDut);
  tofAnaTestbeam->SetDutSm(iDutSm);
  tofAnaTestbeam->SetDutRpc(iDutRpc);

  tofAnaTestbeam->SetMrpcRef(iRef);
  tofAnaTestbeam->SetMrpcRefSm(iRefSm);
  tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);

  tofAnaTestbeam->SetMrpcSel2(iSel2);
  tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);
  tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);


  tofAnaTestbeam->SetCalParFileName(InputAnaCalibFile);
  tofAnaTestbeam->SetCalOutFileName(OutputAnaCalibFile);


  cout<< "dispatch iSel = "<<iSel<<", iSel2 = "<<iSel2<<", iRSel = "<<iRSel<<endl;

  switch (iSel)
  {
    case 400300:
      switch (iRSel)
      {
        case 4:
	      tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(-3.);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        case 9:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
	      break;

        default:
	      ;
      }

      tofAnaTestbeam->SetCh4Sel(15);                // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(15*dScalFac);      // Width  of channel selection window
      break;

    case 900300:
    case 901300:
    case 910300:
    case 920300:
    case 921300:
      switch (iRSel)
      {
        case 4:
	      tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;
	   
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(-17.);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        case 9:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
	      break;

        default:
	      ;
      }

      tofAnaTestbeam->SetCh4Sel(15);                // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(15*dScalFac);      // Width  of channel selection window
      break;

    case 300920:  
    case 400920:  
    case 900920:  
    case 901920:  
    case 910920:  
    case 921920:  
      tofAnaTestbeam->SetTOffD4(13.);  // initialization
      // tofAnaTestbeam->SetTShift(-2.);  // initialization
      switch (iRSel)
      {
        case 5:	 
	      switch(iRSelSm)
          {
	        case 0:
	          tofAnaTestbeam->SetTShift(3.5);       // Shift DTD4 to 0
	          tofAnaTestbeam->SetTOffD4(20.);   // Shift DTD4 to physical value
	          break;

	        case 1:
       	      tofAnaTestbeam->SetTShift(2.05);  // Shift DTD4 to 0
	          tofAnaTestbeam->SetTOffD4(12.);  // initialization	     
	          break;   
	      }

          switch(iSel2)
          {
	        case 3:
	          tofAnaTestbeam->SetSel2TOff(-0.020);     // Shift Sel2 time peak to 0
	          break;

            case 4:
	          tofAnaTestbeam->SetSel2TOff(-0.050);     // Shift Sel2 time peak to 0	   break
	          break;

	        case 9:
	          switch(iSel2Sm)
              {
	            case 2:
	              switch(iSel2Rpc)
                  {
	                case 0:
	                  tofAnaTestbeam->SetSel2TOff(0.030);     // Shift Sel2 time peak to 0
		              break;

                    case 1:
		              tofAnaTestbeam->SetSel2TOff(0.140);     // Shift Sel2 time peak to 0	   break  //921
                      break;

                    default:
		              cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
                      return;
	              }
                  break;

                case 0:
                  switch(iSel2Rpc)
                  {
                    case 0:
                      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
                      break;

                    case 1:
		              tofAnaTestbeam->SetSel2TOff(-0.010);     // Shift Sel2 time peak to 0
		              break;

  	                default:
                      cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
                      return;
	              }
                  break;

	            default:
                  cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
	              return; 
	          }
              break;

            default:
              ;
          }
	      break;

        case 3:
          tofAnaTestbeam->SetTShift(1.95);  // initialization
          tofAnaTestbeam->SetTOffD4(13.);  // initialization
          tofAnaTestbeam->SetSel2TOff(2.25);  // Shift Sel2 time peak to 0
	      break;

        case 9:
	      switch(iSel2Sm)
          {
            case 0: 
	          tofAnaTestbeam->SetTShift(-0.12);  // initialization
              tofAnaTestbeam->SetTOffD4(14.);  // initialization
              tofAnaTestbeam->SetSel2TOff(-0.14);  // Shift Sel2 time peak to 0
              break;
          }
          break;

        default:
          cout << "Define setup! "<< endl;
          return;
            ;
      }

      tofAnaTestbeam->SetCh4Sel(15);                // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(15*dScalFac);      // Width  of channel selection window
      break;


    case 300921:  
    case 400921:  
    case 900921:  
    case 901921:  
    case 910921:  
    case 911921:
    case 920921:  
	  switch (iRSel)
      {
	    case 5:
	      switch(iRSelSm)
          {
	        case 0:
              tofAnaTestbeam->SetTShift(3.);  // Shift DTD4 to 0
	          tofAnaTestbeam->SetTOffD4(20.);  // initialization
	          break;

	        case 1:
       	      tofAnaTestbeam->SetTShift(1.8);  // Shift DTD4 to 0
	          tofAnaTestbeam->SetTOffD4(17.);  // initialization	     
	          break;   
	      }

	      switch(iSel2)
          {
	        case 3:
	          tofAnaTestbeam->SetSel2TOff(-0.1);  // Shift Sel2 time peak to 0
	          break;

	        case 4:
	          tofAnaTestbeam->SetSel2TOff(-0.1);  // Shift Sel2 time peak to 0
	          break;

	        case 9:
	          switch(iSel2Sm)
              {
	            case 2:
	              tofAnaTestbeam->SetSel2TOff(-0.17);  // Shift Sel2 time peak to 0   //920
	              break;

	            default:
	              cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
	              return; 
	          }
	          break;

   	       default:
	         ;
	      }
	      break;

	    case 3:
	      tofAnaTestbeam->SetTShift(1.95);  // initialization
	      tofAnaTestbeam->SetTOffD4(13.);  // initialization
	      tofAnaTestbeam->SetSel2TOff(2.07);  // Shift Sel2 time peak to 0
	      break;

    	case 9:
	      switch(iSel2Sm)
          {
	        case 0: 
	          tofAnaTestbeam->SetTShift(-0.12);  // initialization
	          tofAnaTestbeam->SetTOffD4(14.);  // initialization
	          tofAnaTestbeam->SetSel2TOff(-0.14);  // Shift Sel2 time peak to 0
	          break;
	      }
	      break;

	    default:
	      cout << "Define setup! "<< endl;
	      return;
	      ;
	  }

	  tofAnaTestbeam->SetCh4Sel(15);                // Center of channel selection window
 	  tofAnaTestbeam->SetDCh4Sel(15*dScalFac);      // Width  of channel selection window
	  break;

    case 300901:  
    case 400901:  
    case 920901: 
    case 921901: 
    case 910901: 
    case 900901:  
	  switch (iRSel)
      {
	    case 5:
	      tofAnaTestbeam->SetTShift(3.5);  // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(20.);  // initialization
	      switch(iSel2)
          {
	        case 3:
	          tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
	          break;

	        case 4:
	          tofAnaTestbeam->SetSel2TOff(0.07);  // Shift Sel2 time peak to 0
	          break;

	        case 9:
	          switch(iSel2Sm)
              {
	            case 0:
	              tofAnaTestbeam->SetSel2TOff(0.02);  // Shift Sel2 time peak to 0   //900
	              break;

      	        default:
	              cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
	              return; 
	          }
	          break;

	        default:
	          ;
	      }
 	      break;

	    case 3:
	      tofAnaTestbeam->SetTShift(1.95);  // initialization
	      tofAnaTestbeam->SetTOffD4(13.);  // initialization
	      tofAnaTestbeam->SetSel2TOff(2.07);  // Shift Sel2 time peak to 0
	      break;

	    default:
	      cout << "Define setup! "<< endl;
	      return;
	      ;
	  }

  	  tofAnaTestbeam->SetCh4Sel(15);                // Center of channel selection window
	  tofAnaTestbeam->SetDCh4Sel(15*dScalFac);      // Width  of channel selection window
	  break;

    case 300900:  
    case 400900:  
    case 920900:  
    case 921900:  
    case 910900:  
    case 901900:  
	  switch (iRSel)
      {
	    case 5:
	      tofAnaTestbeam->SetTShift(3.5);  // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(20.);  // initialization
	      switch(iSel2)
          {
	        case 3:
	          tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
	          break;

	        case 4:
	          tofAnaTestbeam->SetSel2TOff(0.07);  // Shift Sel2 time peak to 0
	          break;

	        case 9:
	          switch(iSel2Sm)
              {
	            case 0:
	              tofAnaTestbeam->SetSel2TOff(0.02);  // Shift Sel2 time peak to 0   //901
	              break;

 	            default:
	              cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
	              return; 
	          }
	          break;

	        default:
	          ;
	      }
	      break;

	    case 3:
	      tofAnaTestbeam->SetTShift(1.95);  // initialization
	      tofAnaTestbeam->SetTOffD4(13.);  // initialization
	      tofAnaTestbeam->SetSel2TOff(2.07);  // Shift Sel2 time peak to 0
	      break;

	    default:
	      cout << "Define setup! "<< endl;
	      return;
	      ;
	  }

	  tofAnaTestbeam->SetCh4Sel(15);                // Center of channel selection window
 	  tofAnaTestbeam->SetDCh4Sel(15*dScalFac);      // Width  of channel selection window
	  break;

    case 300400:
    case 900400:
    case 901400:
    case 910400:
    case 920400:
    case 921400:
      switch (iRSel)
      {
        case 3:
	      tofAnaTestbeam->SetTShift(2.5);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(0.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(2.76);     // Shift Sel2 time peak to 0
	      break;
	   
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(3.5);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(17.);   // Shift DTD4 to physical value
	      switch(iSel2)
          {
	        case 3:
	          tofAnaTestbeam->SetSel2TOff(-0.045);     // Shift Sel2 time peak to 0
	          break;

	        case 9:
	          switch(iSel2Sm)
              {
	            case 2:
	              switch(iSel2Rpc)
                  {
	                case 0:
		              tofAnaTestbeam->SetSel2TOff(0.03);     // Shift Sel2 time peak to 0
		              break;

	                case 1:
		              tofAnaTestbeam->SetSel2TOff(0.124);     // Shift Sel2 time peak to 0
		              break;

	                default:
		              cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
		              return; 
	              }
	              break;

	            case 0:
	              switch(iSel2Rpc)
                  {
	                case 0:
		              tofAnaTestbeam->SetSel2TOff(-0.040);     // Shift Sel2 time peak to 0
		              break;

	                case 1:
		              tofAnaTestbeam->SetSel2TOff(-0.045);     // Shift Sel2 time peak to 0
		              break;

	                default:
		              cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
		              return; 
	              }
	              break;

	            default:
	              cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
	              return; 
	          }
	          break;

	        default:
	          cout << "Counter "<<iSel2<<", "<<iSel2Sm<<", "<<iSel2Rpc<<" not configured yet as iSel2"<<endl;
	          return; 	     
	      }
	      break;

        case 9:
	      tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      break;

        default:
	      cout << "Define setup! "<< endl;
	      return;
	      ;
      }

      tofAnaTestbeam->SetCh4Sel(8);                // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(8*dScalFac);      // Width  of channel selection window
      break;

    case 100600:
    case 200600:
    case 210600:
    case 700600:
    case 701600:
    case 702600:
    case 703600:
    case 800600:
    case 801600:
    case 802600:
    case 803600:
    case 804600:
    case 805600:
    case 601600:
      switch (iRSel)
      {	   
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      switch(iRSelSm)
          {
	        case 0:
	          tofAnaTestbeam->SetTShift(-8.3);     // Shift DTD4 to 0
	          tofAnaTestbeam->SetTOffD4(17.);      // Shift DTD4 to physical value
	          break;

	        case 1:
	          tofAnaTestbeam->SetTShift(0.2);     // Shift DTD4 to 0
	          tofAnaTestbeam->SetTOffD4(17.);      // Shift DTD4 to physical value	
	          break;   
	      }

	      switch(iSel2)
          {
	        case 0:
	          tofAnaTestbeam->SetSel2TOff(2.9);     // Shift Sel2 time peak to 0
	          break;

	        case 1:
	          tofAnaTestbeam->SetSel2TOff(-0.085);     // Shift Sel2 time peak to 0
	          break;

	        case 6:
	          tofAnaTestbeam->SetSel2TOff(0.085);     // Shift Sel2 time peak to 0
	          break;

	        default:
	          ;
	      }      // iSel2 switch end 
	      break;

        case 1:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(2.5);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(17.);      // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(2.9);     // Shift Sel2 time peak to 0
	      switch(iSel2)
          {
	        case 0:
	          tofAnaTestbeam->SetSel2TOff(2.9);     // Shift Sel2 time peak to 0
	          break;

	        case 1:
	          tofAnaTestbeam->SetTShift(0.);     // Shift DTD4 to 0
	          tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	          break;

	        case 6:
	          tofAnaTestbeam->SetSel2TOff(0.085);     // Shift Sel2 time peak to 0
	          break;

	        default:
	          ;
	      }      // iSel2 switch end 
	      break;

        case 6:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(-0.070);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(15.);      // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(-0.050);     // Shift Sel2 time peak to 0
	      break;

        case 7:
	      switch(iSel2Rpc)
          {
	        case 0:
	          tofAnaTestbeam->SetTShift(-0.038);     // Shift DTD4 to 0
	          tofAnaTestbeam->SetTOffD4(15.);      // Shift DTD4 to physical value
	          tofAnaTestbeam->SetSel2TOff(-0.042);     // Shift Sel2 time peak to 0
	          break;
	      }
	      break;

        default:
	      cout << "Define setup! "<< endl;
	      return;
      }

      tofAnaTestbeam->SetCh4Sel(16);                // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(16*dScalFac);      // Width  of channel selection window
      break;

    case 100601:
    case 200601:
    case 210601:
    case 700601:
    case 701601:
    case 702601:
    case 703601:
    case 800601:
    case 801601:
    case 802601:
    case 803601:
    case 804601:
    case 805601:
    case 600601:
      switch (iRSel)
      {	   
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      switch(iRSelSm)
          {
	        case 0:
	          tofAnaTestbeam->SetTShift(-5.3);     // Shift DTD4 to 0
	          tofAnaTestbeam->SetTOffD4(17.);      // Shift DTD4 to physical value
	          break;

	        case 1:
	          tofAnaTestbeam->SetTShift(0.29);     // Shift DTD4 to 0
	          tofAnaTestbeam->SetTOffD4(17.);      // Shift DTD4 to physical value	
	          break;   
	      }

	      switch(iSel2)
          {
	        case 0:
	          tofAnaTestbeam->SetSel2TOff(2.9);     // Shift Sel2 time peak to 0
	          break;

	        case 1:
	          tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	          break;

	        case 6:
	          tofAnaTestbeam->SetSel2TOff(0.070);     // Shift Sel2 time peak to 0
	          break;

	        default:
	          ;
	      }      // iSel2 switch end 
	      break;

        case 1:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(2.5);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(17.);      // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(2.9);     // Shift Sel2 time peak to 0
	      break;

        case 6:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(-0.5);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(17.);      // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        default:
	      cout << "Define setup! "<< endl;
	      return;
      }

      tofAnaTestbeam->SetCh4Sel(16);                // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(16*dScalFac);      // Width  of channel selection window
      break;

    case 200100:
    case 210100:
    case 700100:
    case 701100:
    case 702100:
    case 703100:
    case 800100:
    case 801100:
    case 802100:
    case 803100:
    case 804100:
    case 805100:
    case 600100:
    case 601100:
      switch (iRSel)
      {	   
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(-6.3);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(17.);      // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(2.9);     // Shift Sel2 time peak to 0
	      switch(iSel2)
          {
	        case 0:
	          break;

	        case 6:
	          switch(iSel2Rpc)
              {
	            case 0:
	              tofAnaTestbeam->SetSel2TOff(2.);     // Shift Sel2 time peak to 0 // 600
	              break;

	            case 1:
	              tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0 // 601
	              break;
	          }
	          break;

	        default:
	          ;
	      }      // iSel2 switch end 
	      break;

        case 6:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(-3.);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(17.);      // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(-2.9);   // Shift Sel2 time peak to 0 for 601
	      break;

        default:
	      cout << "Define setup! "<< endl;
	      return;
      }

      tofAnaTestbeam->SetCh4Sel(32);                // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(32*dScalFac);      // Width  of channel selection window
      break;

    default:
      cout << "Define analysis setup! "<< endl;
      return;

  }  // end of different subsets



  run->AddTask(tofAnaTestbeam);

  run->SetInputFile(InputFile);
  run->AddFriend(InputDigiFile);
  run->SetOutputFile(OutputFile);

  run->Init();

  cout << "Starting run" << endl;
  run->Run(0, nEvents);

  cout << "Finishing run" << endl;

  gROOT->LoadMacro((plotDir + "/save_hst.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_over_MatD4sel.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_over_Mat04D4best.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_TIS.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_eff_XY.C").Data());
  
  gInterpreter->ProcessLine("save_hst(\"" + HistoFile + "\")");
  gInterpreter->ProcessLine("pl_over_MatD4sel()");
  gInterpreter->ProcessLine("pl_over_Mat04D4best(1)");
  gInterpreter->ProcessLine("pl_TIS()");
  gInterpreter->ProcessLine("pl_eff_XY()");

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Run finished successfully." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
}
