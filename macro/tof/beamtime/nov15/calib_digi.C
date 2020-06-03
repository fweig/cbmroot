// -----------------------------------------------------------------------------
// ----- calib_digi.C                                                      -----
// -----                                                                   -----
// ----- adapted by C. Simon on 2017-03-08                                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

void calib_digi(Int_t nEvents = 100000, Int_t calMode = 0, Int_t calSel = -1, Int_t calSm = 300, Int_t RefSel = 1, TString cFileId = "CbmTofSps_27Nov1728", Int_t iCalSet = 0, Bool_t bOut = 0, Int_t iSel2 = 0)
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

  TString InputFile  = "../../unpack_" + cFileId + ".out.root";
  TString OutputFile = "./digi.out.root";

  TString HistoFile  = "./calib_cluster.hst.root";

  TString InputClustCalibFile  = "./calib_cluster.cor_in.root";
  TString OutputClustCalibFile = "./calib_cluster.cor_out.root";
  TString InputAnaCalibFile    = "./calib_ana.cor_in.root";
  TString OutputAnaCalibFile   = "./calib_ana.cor_out.root";

  Int_t iBRef = iCalSet%1000;
  Int_t iSet = (iCalSet - iBRef)/1000;
  Int_t iRef = iSet %1000;
  Int_t iDut = (iSet - iRef)/1000;

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



  CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer", 1, bOut);
  tofTestBeamClust->SetPs2Ns(kTRUE);
  tofTestBeamClust->SetCalMode(calMode);
  tofTestBeamClust->SetCalSel(calSel);
  tofTestBeamClust->SetCaldXdYMax(2.);        // geometrical matching window in cm 
  tofTestBeamClust->SetCalCluMulMax(10.);     // Max Counter Cluster Multiplicity for filling calib histos  
  tofTestBeamClust->SetCalRpc(calSm);         // select detector for calibration update  
  tofTestBeamClust->SetTRefId(RefSel);        // reference trigger for offset calculation 
  tofTestBeamClust->SetTotMax(10.);           // Tot upper limit for walk corection
  tofTestBeamClust->SetTotMin(0.01);          // Tot lower limit for walk correction
  tofTestBeamClust->SetTotPreRange(5.);       // effective lower Tot limit  in ns from peak position
  tofTestBeamClust->SetTotMean(2.);           // Tot calibration target value in ns 
  tofTestBeamClust->SetMaxTimeDist(0.5);      // default cluster range in ns 
  //tofTestBeamClust->SetMaxTimeDist(0.);     //Deb// default cluster range in ns 
  tofTestBeamClust->SetDelTofMax(6.);         // acceptance range for cluster correlation  
  tofTestBeamClust->SetBeamRefMulMax(4);      // limit Multiplicity in beam counter
  tofTestBeamClust->SetBeamAddRefMul(-1);

  tofTestBeamClust->SetBeamRefId(iRSel);
  tofTestBeamClust->SetBeamRefSm(iRSelSm);
  tofTestBeamClust->SetBeamRefDet(iRSelRpc);

  tofTestBeamClust->SetDutId(iDut);
  tofTestBeamClust->SetDutSm(iDutSm);
  tofTestBeamClust->SetDutRpc(iDutRpc);

  tofTestBeamClust->SetSelId(iRef);
  tofTestBeamClust->SetSelSm(iRefSm);
  tofTestBeamClust->SetSelRpc(iRefRpc);

  tofTestBeamClust->SetSel2Id(iSel2); 
  tofTestBeamClust->SetSel2Sm(iSel2Sm); 
  tofTestBeamClust->SetSel2Rpc(iSel2Rpc);

  tofTestBeamClust->SetCalParFileName(InputClustCalibFile);
  tofTestBeamClust->SetOutHstFileName(OutputClustCalibFile);



  switch (calMode)
  {
    // initial calibration
    case 0:
      tofTestBeamClust->SetTotMax(10.);               // in ns
      //tofTestBeamClust->SetTotMin(1.);
      tofTestBeamClust->SetTRefDifMax(2000.);         // in ns 
      tofTestBeamClust->PosYMaxScal(2000.);           // in % of length 
      tofTestBeamClust->SetMaxTimeDist(0.);           // no cluster building  
      break;

    // save offsets, update walks, for diamonds
    case 1:
      tofTestBeamClust->SetTRefDifMax(50.);           // in ns 
      tofTestBeamClust->PosYMaxScal(0.1);             // in % of length 
      break;
    case 11:
      tofTestBeamClust->SetTRefDifMax(4.);            // in ns 
      tofTestBeamClust->PosYMaxScal(1.0);             // in % of length 
      break;   
    case 21:
      tofTestBeamClust->SetTRefDifMax(3.);            // in ns 
      tofTestBeamClust->PosYMaxScal(1.0);             // in % of length 
      break;
    case 31:
      tofTestBeamClust->SetTRefDifMax(2.);            // in ns 
      tofTestBeamClust->PosYMaxScal(1.0);             // in % of length 
      break;
    case 41:
      tofTestBeamClust->SetTRefDifMax(1.);            // in ns 
      tofTestBeamClust->PosYMaxScal(0.8);             // in % of length 
      break;   
    case 51:
      tofTestBeamClust->SetTRefDifMax(0.7);           // in ns 
      tofTestBeamClust->PosYMaxScal(0.7);             // in % of length 
      break;
    case 61:
      tofTestBeamClust->SetTRefDifMax(0.5);           // in ns 
      tofTestBeamClust->PosYMaxScal(0.7);             // in % of length 
      break;   
    case 71:
      tofTestBeamClust->SetTRefDifMax(0.4);           // in ns 
      tofTestBeamClust->PosYMaxScal(0.6);             // in % of length 
      break;

    // time difference calibration
    case 2:
      tofTestBeamClust->SetTRefDifMax(300.);          // in ns 
      tofTestBeamClust->PosYMaxScal(1000.);           // in % of length
      break;

    // time offsets
    case 3:
      tofTestBeamClust->SetTRefDifMax(200.);          // in ns 
      tofTestBeamClust->PosYMaxScal(1000.);           // in % of length
      tofTestBeamClust->SetMaxTimeDist(0.);           // no cluster building  
      break;
    case 12:
    case 13:
      tofTestBeamClust->SetTRefDifMax(100.);          // in ns 
      tofTestBeamClust->PosYMaxScal(100.);            // in % of length
      break;
    case 22:
    case 23:
      tofTestBeamClust->SetTRefDifMax(50.);           // in ns 
      tofTestBeamClust->PosYMaxScal(50.);             // in % of length
      break;
    case 32:
    case 33:
      tofTestBeamClust->SetTRefDifMax(25.);           // in ns 
      tofTestBeamClust->PosYMaxScal(20.);             // in % of length
      break;
    case 42:
    case 43:
      tofTestBeamClust->SetTRefDifMax(12.);           // in ns 
      tofTestBeamClust->PosYMaxScal(10.);             // in % of length
      break;
    case 52:
    case 53:
      tofTestBeamClust->SetTRefDifMax(5.);            // in ns 
      tofTestBeamClust->PosYMaxScal(4.);              // in % of length
      break;
    case 62:
    case 63:
      tofTestBeamClust->SetTRefDifMax(2.5);           // in ns 
      tofTestBeamClust->PosYMaxScal(2.);              // in % of length
      break;
    case 72:
    case 73:
      tofTestBeamClust->SetTRefDifMax(2.);            // in ns 
      tofTestBeamClust->PosYMaxScal(1.0);             // in % of length
      break;
    case 82:
    case 83:
      tofTestBeamClust->SetTRefDifMax(1.);            // in ns 
      tofTestBeamClust->PosYMaxScal(0.8);             // in % of length   
      break;
    case 92:
    case 93:
      tofTestBeamClust->SetTRefDifMax(0.6);           // in ns 
      tofTestBeamClust->PosYMaxScal(0.8);             // in % of length   
      break;

    // velocity dependence (DelTOF)
    case 4:
      tofTestBeamClust->SetTRefDifMax(6.);            // in ns 
      tofTestBeamClust->PosYMaxScal(1.5);             // in % of length
      break;
    case 14:
      tofTestBeamClust->SetTRefDifMax(3.);            // in ns 
      tofTestBeamClust->PosYMaxScal(1.);              // in % of length
      break;
    case 24:
      tofTestBeamClust->SetTRefDifMax(1.5);           // in ns 
      tofTestBeamClust->PosYMaxScal(0.8);             // in % of length
      break;
    case 44:
      tofTestBeamClust->SetTRefDifMax(1.);            // in ns 
      tofTestBeamClust->PosYMaxScal(0.8);             // in % of length
      break;
    case 54:
      tofTestBeamClust->SetTRefDifMax(0.7);           // in ns 
      tofTestBeamClust->PosYMaxScal(0.7);             // in % of length
      break;
    case 64:
      tofTestBeamClust->SetTRefDifMax(0.5);           // in ns 
      tofTestBeamClust->PosYMaxScal(0.7);             // in % of length
      break;

    default:
      cout << "<E> Calib mode not implemented! stop execution of script"<<endl;
      return;
  } 



  CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis", 1);
  // CbmTofAnaTestbeam defaults  
  tofAnaTestbeam->SetDXMean(0.);
  tofAnaTestbeam->SetDYMean(0.);
  tofAnaTestbeam->SetDTMean(0.);             // in ns
  tofAnaTestbeam->SetDXWidth(0.4);
  tofAnaTestbeam->SetDYWidth(0.4);
  tofAnaTestbeam->SetDTWidth(0.08);          // in ns

  tofAnaTestbeam->SetPosY4Sel(0.5);          // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetDTDia(0.);              // Time difference to additional diamond
  tofAnaTestbeam->SetCorMode(RefSel);        // 1 - DTD4, 2 - X4 
  tofAnaTestbeam->SetMul0Max(10);            // Max Multiplicity in dut 
  tofAnaTestbeam->SetMul4Max(10);            // Max Multiplicity in Ref - RPC 
  tofAnaTestbeam->SetMulDMax(10);            // Max Multiplicity in Diamond    
  tofAnaTestbeam->SetHitDistMin(30.);        // initialization

  tofAnaTestbeam->SetPosYS2Sel(0.5);         // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetChS2Sel(0.);            // Center of channel selection window
  tofAnaTestbeam->SetDChS2Sel(100.);         // Width  of channel selection window

  tofAnaTestbeam->SetTShift(0.);             // Shift DTD4 to 0
  tofAnaTestbeam->SetSel2TOff(0.);           // Shift Sel2 time peak to 0 
  tofAnaTestbeam->SetTOffD4(13.);            // Shift DTD4 to physical value

  tofAnaTestbeam->SetChi2Lim(10.);           // initialization of Chi2 selection limit  

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

  Bool_t BEff = kFALSE;

  switch (iSet)
  {
    case 400300:
      cout << "Run with iRSel = "<<iRSel<<endl;
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
	      tofAnaTestbeam->SetChi2Lim(100.);   // initialization of Chi2 selection limit  
	      tofAnaTestbeam->SetMulDMax(3);      // Max Multiplicity in BeamRef // Diamond    
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
	      break;

        default:
	      ;
      }

      tofAnaTestbeam->SetChi2Lim(30.);     // initialization of Chi2 selection limit  
      tofAnaTestbeam->SetDXWidth(1.5);
      tofAnaTestbeam->SetDYWidth(1.5);
      tofAnaTestbeam->SetDTWidth(0.12);   // in ps
      tofAnaTestbeam->SetCh4Sel(15);      // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(3);      // Width  of channel selection window
      tofAnaTestbeam->SetPosY4Sel(0.25);  // Y Position selection in fraction of strip length
      tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
      tofAnaTestbeam->SetMul0Max(4);      // Max Multiplicity in dut 
      tofAnaTestbeam->SetMul4Max(4);      // Max Multiplicity in Ref - RPC 
      break;

    case 920300:
    case 921300:
      switch (iRSel)
      {
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in BeamRef // Diamond    
	      tofAnaTestbeam->SetTShift(0.2);     // Shift DTD4 to 0
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

      tofAnaTestbeam->SetChi2Lim(30.);   // initialization of Chi2 selection limit  
      tofAnaTestbeam->SetDXWidth(2.);
      tofAnaTestbeam->SetDYWidth(2.);
      tofAnaTestbeam->SetDTWidth(0.2); // in ps
      tofAnaTestbeam->SetCh4Sel(15);      // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(10);      // Width  of channel selection window
      tofAnaTestbeam->SetPosY4Sel(0.5);  // Y Position selection in fraction of strip length
      tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
      tofAnaTestbeam->SetMul0Max(3);      // Max Multiplicity in dut 
      tofAnaTestbeam->SetMul4Max(3);      // Max Multiplicity in Ref - RPC 
      break; 

    case 920400:
    case 921400:
      switch (iRSel)
      {
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in BeamRef // Diamond    
	      tofAnaTestbeam->SetTShift(0.2);     // Shift DTD4 to 0
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

      tofAnaTestbeam->SetChi2Lim(30.);   // initialization of Chi2 selection limit  
      tofAnaTestbeam->SetDXWidth(2.);
      tofAnaTestbeam->SetDYWidth(2.);
      tofAnaTestbeam->SetDTWidth(0.2);    // in ps
      tofAnaTestbeam->SetCh4Sel(8);        // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(10);      // Width  of channel selection window
      tofAnaTestbeam->SetPosY4Sel(0.5);    // Y Position selection in fraction of strip length
      tofAnaTestbeam->SetMulDMax(1);       // Max Multiplicity in Diamond    
      tofAnaTestbeam->SetMul0Max(30);      // Max Multiplicity in dut 
      tofAnaTestbeam->SetMul4Max(30);      // Max Multiplicity in Ref - RPC 
      break; 

    case 300920:
      switch (iRSel)
      {
        case 9:
	      tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(0.);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        default:
	      ;
      }

      tofAnaTestbeam->SetChi2Lim(1000.);   // initialization of Chi2 selection limit  
      tofAnaTestbeam->SetDXWidth(2.);
      tofAnaTestbeam->SetDYWidth(2.);
      tofAnaTestbeam->SetDTWidth(0.5); // in ps
      tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
      tofAnaTestbeam->SetMul0Max(5);      // Max Multiplicity in dut 
      tofAnaTestbeam->SetMul4Max(5);      // Max Multiplicity in Ref - RPC 
      break; 

    case 900920:
      switch (iRSel)
      {
        case 4:
	      tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(0.);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        default:
	      ;
      }

      tofAnaTestbeam->SetChi2Lim(10.);   // initialization of Chi2 selection limit  
      tofAnaTestbeam->SetDXWidth(1.);
      tofAnaTestbeam->SetDYWidth(2.);
      tofAnaTestbeam->SetDTWidth(0.1); // in ps
      tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
      tofAnaTestbeam->SetMul0Max(15);      // Max Multiplicity in dut 
      tofAnaTestbeam->SetMul4Max(15);      // Max Multiplicity in Ref - RPC 
      break; 

    case 300921:
    case 400921:
    case 900921:
    case 910921:
    case 920921:
      switch (iRSel)
      {
        case 3:
	      tofAnaTestbeam->SetTShift(-8.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(-0.15);     // Shift Sel2 time peak to 0
	      break;

        case 5: // sel2 = 920
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(-5.);  // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.1);  // Shift Sel2 time peak to 0
	      break;

        case 9:   // sel2 = -920
	      tofAnaTestbeam->SetTShift(1.2);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(-0.15);     // Shift Sel2 time peak to 0
	      break;

        default:
	      ;
      }

      tofAnaTestbeam->SetChi2Lim(10.);   // initialization of Chi2 selection limit  
      tofAnaTestbeam->SetDXWidth(1.0);
      tofAnaTestbeam->SetDYWidth(2.0);
      tofAnaTestbeam->SetDTWidth(0.15); // in ps
      BEff = kFALSE;
      //BEff=kTRUE;
      if(BEff)
      {
        tofAnaTestbeam->SetCh4Sel(16);      // Center of channel selection window
        tofAnaTestbeam->SetDCh4Sel(13);      // Width  of channel selection window
        tofAnaTestbeam->SetPosY4Sel(0.4);   // Y Position selection in fraction of strip length
      }
      break; 

    case 901900:
    case 400900:
    case 300900:
      switch (iRSel)
      {
        case 4:
	      tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
	      tofAnaTestbeam->SetTShift(-20.);     // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(-0.17);     // Shift Sel2 time peak to 0
	      break;

        default:
	      ;
      }

      tofAnaTestbeam->SetChi2Lim(40.);   // initialization of Chi2 selection limit  
      tofAnaTestbeam->SetDXWidth(1.0);
      tofAnaTestbeam->SetDYWidth(2.0);
      tofAnaTestbeam->SetDTWidth(0.15); // in ps
      break; 

    case 300400:
      switch (iRSel)
      {
        case 9:
	      tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;
        default:
	      ;
      }
      break;

    case 210200:
      switch (iRSel)
      {
        case 9:
	      tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;
        default:
	      ;
      }
      break;

    case 700600:
    case 701600:
    case 702600:
    case 703600:
    case 100600:
    case 200600:
    case 210600:
    case 601600:
      switch (iRSel)
      {
        case 5:
	      tofAnaTestbeam->SetTShift(-11.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        case 6:
	      tofAnaTestbeam->SetTShift(-6.);   // Shift DTD4 to 0
	      tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	      tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	      break;

        default:
	      ;
      }

      tofAnaTestbeam->SetChi2Lim(5.);   // initialization of Chi2 selection limit  
      tofAnaTestbeam->SetDXWidth(0.5);
      tofAnaTestbeam->SetDYWidth(1.);
      tofAnaTestbeam->SetDTWidth(0.1); // in ps
      BEff = kFALSE;
      if(BEff)
      {
        tofAnaTestbeam->SetCh4Sel(20);      // Center of channel selection window
        tofAnaTestbeam->SetDCh4Sel(8);      // Width  of channel selection window
        tofAnaTestbeam->SetPosY4Sel(0.4);   // Y Position selection in fraction of strip length
      }
      tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
      tofAnaTestbeam->SetMul0Max(10);      // Max Multiplicity in dut 
      tofAnaTestbeam->SetMul4Max(10);      // Max Multiplicity in Ref - RPC 
      break;

    default:
	  cout<<"<E> detector setup "<<iSet<<" unknown, stop!"<<endl;
	  return;
  }

  run->AddTask(tofTestBeamClust);
  run->AddTask(tofAnaTestbeam);

  run->SetInputFile(InputFile);
  run->SetOutputFile(OutputFile);

  run->Init();

  cout << "Starting run" << endl;
  run->Run(0, nEvents);

  cout << "Finishing run" << endl;

  gROOT->LoadMacro((plotDir + "/save_hst.C").Data());
  gROOT->LoadMacro((plotDir + "/fit_ybox.h").Data());
  gROOT->LoadMacro((plotDir + "/pl_all_CluMul.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_all_CluRate.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_over_cluSel.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_over_clu.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_all_dTSel.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_over_MatD4sel.C").Data());
  gROOT->LoadMacro((plotDir + "/pl_over_Mat04D4best.C").Data());

  gInterpreter->ProcessLine("save_hst(\"" + HistoFile + "\")");

  switch(iSet)
  {
    case 0:
    case 300400:
    case 300900:
    case 300921:
    case 400300:
    case 400900:
    case 900901:
    case 900921:
    case 901900:
    case 901921:
    case 910900:
    case 911921:
    case 920300:
    case 920921:
    case 921300:
    case 921920:
      gInterpreter->ProcessLine("pl_over_clu(3)");
      gInterpreter->ProcessLine("pl_over_clu(4)");
      gInterpreter->ProcessLine("pl_over_clu(5,0)");
      gInterpreter->ProcessLine("pl_over_clu(5,1)");
      gInterpreter->ProcessLine("pl_over_clu(5,2)");
      gInterpreter->ProcessLine("pl_over_clu(9,0,0)");
      gInterpreter->ProcessLine("pl_over_clu(9,0,1)");
      gInterpreter->ProcessLine("pl_over_clu(9,1,0)");
      gInterpreter->ProcessLine("pl_over_clu(9,1,1)");
      gInterpreter->ProcessLine("pl_over_clu(9,2,0)");
      gInterpreter->ProcessLine("pl_over_clu(9,2,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,3)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,4)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,5,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,5,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,5,2,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,3)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,4)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,5,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,5,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,5,2,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,1)");
      break;

    case 1:
    case 100600:
    case 100601:
    case 200600:
    case 200601:
    case 210200:
    case 600601:
    case 601600:
    case 700600:
    case 700601:
      gInterpreter->ProcessLine("pl_over_clu(1)");
      gInterpreter->ProcessLine("pl_over_clu(2,0,0)");
      gInterpreter->ProcessLine("pl_over_clu(2,1,0)");
      gInterpreter->ProcessLine("pl_over_clu(5,0,0)");
      gInterpreter->ProcessLine("pl_over_clu(6,0,0)");
      gInterpreter->ProcessLine("pl_over_clu(6,0,1)");
      gInterpreter->ProcessLine("pl_over_clu(7,0,0)");
      gInterpreter->ProcessLine("pl_over_clu(7,0,1)");
      gInterpreter->ProcessLine("pl_over_clu(7,0,2)");
      gInterpreter->ProcessLine("pl_over_clu(7,0,3)");
      gInterpreter->ProcessLine("pl_over_clu(8,0,0)");
      gInterpreter->ProcessLine("pl_over_clu(8,0,1)");
      gInterpreter->ProcessLine("pl_over_clu(8,0,2)");
      gInterpreter->ProcessLine("pl_over_clu(8,0,3)");
      gInterpreter->ProcessLine("pl_over_clu(8,0,4)");
      gInterpreter->ProcessLine("pl_over_clu(8,0,5)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,2,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,2,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,5,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,5,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,5,2,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,7,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,7,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,7,0,2)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,7,0,3)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,8,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,8,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,8,0,2)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,8,0,3)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,8,0,4)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,8,0,5)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,2,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,2,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,5,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,5,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,5,2,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,7,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,7,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,7,0,2)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,7,0,3)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,2)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,3)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,4)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,5)");
      break;

    default:
      ;
  }

  gInterpreter->ProcessLine("pl_all_dTSel()");
  gInterpreter->ProcessLine("pl_over_MatD4sel()");
  gInterpreter->ProcessLine("pl_over_Mat04D4best()");
  gInterpreter->ProcessLine("pl_all_CluMul()");
  gInterpreter->ProcessLine("pl_all_CluRate()");

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Run finished successfully." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
}
