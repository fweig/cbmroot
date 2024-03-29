/* Copyright (C) 2016-2017 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void ana_hits(Int_t nEvents = 10, Int_t iSel = 1, Int_t iGenCor = 1, TString cFileId = "Cern", TString cSet = "345",
              Int_t iSel2 = 0, Int_t iTrackingSetup = 0, Double_t dScalFac = 1.)
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
  TString paramDir      = workDir + "/macro/tof/beamtime/nov15";
  TString ParFile       = paramDir + "/unpack_" + cFileId + ".params.root";
  TString InputFile     = paramDir + "/unpack_" + cFileId + ".out.root";
  TString InputDigiFile = paramDir + "/data/digi_" + cFileId + Form("_%s", cSet.Data()) + ".out.root";
  TString OutputFile =
    paramDir + "/data/hits_" + cFileId + Form("_%s_%06d_%03d", cSet.Data(), iSel, iSel2) + ".out.root";
  TString cAnaFile = Form("%s_%s_%06d_%03d_tofAnaTestBeam.hst.root", cFileId.Data(), cSet.Data(), iSel, iSel2);
  TString cHstFile =
    paramDir + Form("/hst/%s_%s_%06d_%03d_%04.1f_tofAna.hst.root", cFileId.Data(), cSet.Data(), iSel, iSel2, dScalFac);
  TString cTrkFile = Form("%s_tofFindTracks.hst.root", cFileId.Data());

  cout << " InputDigiFile = " << InputDigiFile << endl;

  TList* parFileList = new TList();

  TObjString* mapParFile = new TObjString(paramDir + "/parMapCernNov2015.txt");
  parFileList->Add(mapParFile);

  TString TofGeo = "v15c";  //default
  TString FId    = cFileId;
  if (FId.Contains("CernSps02Mar")) { TofGeo = "v15b"; }
  if (FId.Contains("CernSps28Feb")) { TofGeo = "v15a"; }
  cout << "Geometry version " << TofGeo << endl;

  TString FPar = "";
  if (FId.Contains("tsu")) {
    cout << FId << ": Analyse TSU counter " << endl;
    TofGeo = "v14c_tsu";
    FPar   = "tsu.";
  }

  TObjString* tofDigiFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par");  // TOF digi file
  parFileList->Add(tofDigiFile);

  // TObjString tofDigiBdfFile =  paramDir + "/tof.digibdf.par";
  // TObjString tofDigiBdfFile =  paramDir + "/tof." + FPar + "digibdf.par";
  TObjString* tofDigiBdfFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digibdf.par");
  parFileList->Add(tofDigiBdfFile);

  TString geoDir      = gSystem->Getenv("VMCWORKDIR");
  TString geoFile     = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
  TFile* fgeo         = new TFile(geoFile);
  TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
  if (NULL == geoMan) {
    cout << "<E> FAIRGeom not found in geoFile" << endl;
    return;
  }
  if (0) {
    TGeoVolume* master = geoMan->GetTopVolume();
    master->SetVisContainers(1);
    master->Draw("ogl");
  }

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  cout << "InputFile:     " << InputFile.Data() << endl;
  cout << "InputDigiFile: " << InputDigiFile.Data() << endl;

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
  //   run->AddTask(tofMapping);

  // =========================================================================
  // ===                       Tracking                                    ===
  // =========================================================================
  /*
   CbmStsDigitize* stsDigitize = new CbmStsDigitize(); //necessary for kalman !!
   CbmKF* kalman = new CbmKF();

   CbmTofTrackFinder* tofTrackFinder= new CbmTofTrackFinderNN();
   tofTrackFinder->SetMaxTofTimeDifference(5000.); // in ps/cm 
   tofTrackFinder->SetTxLIM(0.05);                 // max slope dx/dz
   tofTrackFinder->SetTyLIM(0.05);                  // max dev from mean slope dy/dz
   tofTrackFinder->SetTyMean(0.1);                 // mean slope dy/dz
   tofTrackFinder->SetSIGLIM(2.);                  // max matching chi2

   CbmTofTrackFitter* tofTrackFitter= new CbmTofTrackFitterKF(0,211);
   TFitter *MyFit = new TFitter(1);                // initialize Minuit
   tofTrackFinder->SetFitter(tofTrackFitter);
   CbmTofFindTracks* tofFindTracks  = new CbmTofFindTracks("TOF Track Finder");
   tofFindTracks->UseFinder(tofTrackFinder);
   tofFindTracks->UseFitter(tofTrackFitter);
   tofFindTracks->SetCorMode(iGenCor);           // valid options: 0,1,2
   tofFindTracks->SetTtTarg(33.7);               // target value for inverse velocity, > 33.3 !
   tofFindTracks->SetCalParFileName(cTrkFile);   // Tracker parameter value file name
     
   tofFindTracks->SetT0MAX(dScalFac*1000.);       // in ps
   tofFindTracks->SetSIGT(100.);                  // default in ps
   tofFindTracks->SetSIGX(0.5);                   // default in cm
   tofFindTracks->SetSIGY(1.5);                   // default in cm 
   tofFindTracks->SetSIGZ(5.0);                   // default in cm 

   switch (iTrackingSetup){
   case 0:                                       // bypass mode
     tofFindTracks->SetNStations(1);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     break;

   case 1:                                       // calibration mode
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

   case 2:                                       // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(6);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(2, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(3, 9, 1, 0);           // 
     tofFindTracks->SetStation(4, 9, 0, 0);           //  
     tofFindTracks->SetStation(5, 9, 0, 1);           // 
     tofTrackFinder->SetSIGT(150.);                  // in ps
     break;

   case 3:                                            // calibration mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(1, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(2, 9, 0, 0);           //  
     tofFindTracks->SetStation(3, 9, 0, 1);           // 
     tofTrackFinder->SetSIGT(150.);                  // in ps
     break;

   case 4:                                            // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(1, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(2, 9, 0, 0);           //  
     tofFindTracks->SetStation(3, 9, 0, 1);           // 
     tofTrackFinder->SetSIGT(150.);                  // in ps
     break;

   case 5:                                            // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStation(0, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(1, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(2, 9, 0, 0);           //  
     tofFindTracks->SetStation(3, 9, 0, 1);           // 
     tofFindTracks->SetStation(4, 3, 0, 0);           // P2 
     tofTrackFinder->SetSIGT(150.);                  // in ps
     break;

   case 10:                                       // full lower setup
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
     tofFindTracks->SetStation(10, 7, 0, 1);           // Buc2012
     tofFindTracks->SetStation(11, 7, 0, 2);           // Buc2012
     tofFindTracks->SetStation(12, 7, 0, 3);           // Buc2012
     tofFindTracks->SetStation(13, 1, 0, 0);           // BucRef  
     break;

   case 11:                                       // partial lower setup
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
  // ===                       Analysis                                    ===
  // =========================================================================

  CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis", iVerbose);
  tofAnaTestbeam->SetCorMode(iGenCor);  // 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp
  tofAnaTestbeam->SetHitDistMin(30.);   // initialization

  //CbmTofAnaTestbeam defaults
  tofAnaTestbeam->SetDXMean(0.);
  tofAnaTestbeam->SetDYMean(0.);
  tofAnaTestbeam->SetDTMean(0.);  // in ps
  tofAnaTestbeam->SetDXWidth(0.7);
  tofAnaTestbeam->SetDYWidth(1.0);
  tofAnaTestbeam->SetDTWidth(0.1);  // in ps
  tofAnaTestbeam->SetCalParFileName(cAnaFile);
  tofAnaTestbeam->SetPosY4Sel(0.5 * dScalFac);  // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetDTDia(0.);                 // Time difference to additional diamond
  tofAnaTestbeam->SetMul0Max(200);              // Max Multiplicity in dut
  tofAnaTestbeam->SetMul4Max(3);                // Max Multiplicity in Ref - RPC
  tofAnaTestbeam->SetMulDMax(20);               // Max Multiplicity in Diamond
  tofAnaTestbeam->SetTOffD4(10.);               // initialization
  tofAnaTestbeam->SetDTD4MAX(6.);               // initialization of Max time difference Ref - BRef

  //tofAnaTestbeam->SetTShift(-28000.);// initialization
  tofAnaTestbeam->SetPosYS2Sel(0.5);  // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetChS2Sel(0.);     // Center of channel selection window
  tofAnaTestbeam->SetDChS2Sel(100.);  // Width  of channel selection window
  tofAnaTestbeam->SetSel2TOff(0.);    // Shift Sel2 time peak to 0
  tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit
  tofAnaTestbeam->SetChi2Lim2(2.);    // initialization of Chi2 selection limit for Mref-Sel2 pair

  Int_t iRSel    = 0;
  Int_t iRSelSm  = 0;
  Int_t iRSelRpc = 0;
  if (iSel2 >= 0) {
    iRSel   = 5;  // use diamond
    iRSelSm = 0;
    if (InputDigiFile.Contains("510_")) iRSelSm = 1;
  }
  else {
    iSel2    = -iSel2;
    iRSel    = iSel2;
    iRSelRpc = iRSel % 10;
    iRSel    = (iRSel - iRSelRpc) / 10;
    iRSelSm  = iRSel % 10;
    iRSel    = (iRSel - iRSelSm) / 10;
  }

  tofAnaTestbeam->SetBeamRefSmType(iRSel);  // common reaction reference
  tofAnaTestbeam->SetBeamRefSmId(iRSelSm);

  Int_t iSel2Rpc = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Rpc) / 10;
  Int_t iSel2Sm  = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Sm) / 10;
  if (iSel2 > 0) {
    tofAnaTestbeam->SetMrpcSel2(iSel2);        // initialization of second selector Mrpc Type
    tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);    // initialization of second selector Mrpc SmId
    tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);  // initialization of second selector Mrpc RpcId
  }

  tofAnaTestbeam->SetSIGLIM(3.);  // max matching chi2
  tofAnaTestbeam->SetSIGT(100.);  // in ps
  tofAnaTestbeam->SetSIGX(1.);    // in cm
  tofAnaTestbeam->SetSIGY(1.);    // in cm

  Int_t iRef    = iSel % 1000;
  Int_t iDut    = (iSel - iRef) / 1000;
  Int_t iDutRpc = iDut % 10;
  iDut          = (iDut - iDutRpc) / 10;
  Int_t iDutSm  = iDut % 10;
  iDut          = (iDut - iDutSm) / 10;
  Int_t iRefRpc = iRef % 10;
  iRef          = (iRef - iRefRpc) / 10;
  Int_t iRefSm  = iRef % 10;
  iRef          = (iRef - iRefSm) / 10;

  tofAnaTestbeam->SetDut(iDut);            // Device under test
  tofAnaTestbeam->SetDutSm(iDutSm);        // Device under test
  tofAnaTestbeam->SetDutRpc(iDutRpc);      // Device under test
  tofAnaTestbeam->SetMrpcRef(iRef);        // Reference RPC
  tofAnaTestbeam->SetMrpcRefSm(iRefSm);    // Reference RPC
  tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);  // Reference RPC

  cout << "dispatch iSel = " << iSel << ", iSel2 = " << iSel2 << ", iRSel = " << iRSel << endl;

  switch (iSel) {
    case 0:  // upper part of setup: P2 - P5
    case 400300:
      cout << "Run with iRSel = " << iRSel << endl;
      switch (iRSel) {
        case 4:
          tofAnaTestbeam->SetTShift(0.);      // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(16000.);  // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(0.);    // Shift Sel2 time peak to 0
          break;

        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(-3000.);  // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(16000.);  // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(0.);    // Shift Sel2 time peak to 0
          break;

        case 9:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(100.);    // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(16000.);  // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(500.);  // Shift Sel2 time peak to 0
          break;

        default:;
      }
      tofAnaTestbeam->SetCh4Sel(15);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(15 * dScalFac);  // Width  of channel selection window
      break;

    case 900300:
    case 901300:
    case 910300:
    case 920300:
    case 921300:
      switch (iRSel) {
        case 4:
          tofAnaTestbeam->SetTShift(0.);    // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
          break;

        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(2.5);   // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
          break;

        case 9:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(0.1);    // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(16.);    // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(0.5);  // Shift Sel2 time peak to 0
          break;

        default:;
      }
      tofAnaTestbeam->SetCh4Sel(15);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(15 * dScalFac);  // Width  of channel selection window
      break;

    case 300920:
    case 400920:
    case 900920:
    case 901920:
    case 910920:
    case 921920:
      tofAnaTestbeam->SetTOffD4(13000.);  // initialization
      //	 tofAnaTestbeam->SetTShift(-2000.);  // initialization
      cout << "Run with iRSel = " << iRSel << endl;
      switch (iRSel) {
        case 5:
          switch (iRSelSm) {
            case 0:
              tofAnaTestbeam->SetTShift(3.5);  // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(20.);  // Shift DTD4 to physical value
              break;
            case 1:
              tofAnaTestbeam->SetTShift(2.5);   // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(16.0);  // initialization
              break;
          }

          switch (iSel2) {
            case 3:
              tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
              break;
            case 4:
              tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0	   break
              break;

            case 9:
              switch (iSel2Sm) {
                case 2:
                  switch (iSel2Rpc) {
                    case 0:
                      tofAnaTestbeam->SetSel2TOff(-0.08);  // Shift Sel2 time peak to 0
                      break;
                    case 1:
                      tofAnaTestbeam->SetSel2TOff(0.04);  // Shift Sel2 time peak to 0  //921
                      break;
                    default:
                      cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc
                           << " not configured yet as iSel2" << endl;
                      return;
                  }
                  break;
                case 0:
                  switch (iSel2Rpc) {
                    case 0:
                      tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                      break;
                    case 1:
                      tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                      break;
                    default:
                      cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc
                           << " not configured yet as iSel2" << endl;
                      return;
                  }
                  break;
                default:
                  cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc << " not configured yet as iSel2"
                       << endl;
                  return;
              }
              break;

            default:;
          }
          break;

        case 3:
          tofAnaTestbeam->SetTShift(1.950);    // initialization
          tofAnaTestbeam->SetTOffD4(13.000);   // initialization
          tofAnaTestbeam->SetSel2TOff(2.250);  // Shift Sel2 time peak to 0
          break;

        case 9:
          switch (iSel2Sm) {
            case 0:
              tofAnaTestbeam->SetTShift(-0.12);     // initialization
              tofAnaTestbeam->SetTOffD4(14.);       // initialization
              tofAnaTestbeam->SetSel2TOff(-0.140);  // Shift Sel2 time peak to 0
              break;
          }
          break;

        default:
          cout << "Define setup! " << endl;
          return;
          ;
      }
      tofAnaTestbeam->SetCh4Sel(15);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(15 * dScalFac);  // Width  of channel selection window
      break;


    case 300921:
    case 400921:
    case 900921:
    case 901921:
    case 910921:
    case 911921:
    case 920921:
      cout << "Run with iRSel = " << iRSel << endl;
      switch (iRSel) {
        case 5:
          switch (iRSelSm) {
            case 0:
              tofAnaTestbeam->SetTShift(4.);   // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(20.);  // initialization
              break;
            case 1:
              tofAnaTestbeam->SetTShift(2.6);  // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(17.);  // initialization
              break;
          }

          switch (iSel2) {
            case 3:
              tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
              break;
            case 4:
              tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
              break;
            case 9:
              switch (iSel2Sm) {
                case 2:
                  tofAnaTestbeam->SetSel2TOff(-0.07);  // Shift Sel2 time peak to 0   //920
                  break;
                default:
                  cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc << " not configured yet as iSel2"
                       << endl;
                  return;
              }
              break;
            default:;
          }
          break;

        case 3:
          tofAnaTestbeam->SetTShift(1950.);    // initialization
          tofAnaTestbeam->SetTOffD4(13000.);   // initialization
          tofAnaTestbeam->SetSel2TOff(2070.);  // Shift Sel2 time peak to 0
          break;

        case 9:
          switch (iSel2Sm) {
            case 0:
              tofAnaTestbeam->SetTShift(-120.);    // initialization
              tofAnaTestbeam->SetTOffD4(14000.);   // initialization
              tofAnaTestbeam->SetSel2TOff(-140.);  // Shift Sel2 time peak to 0
              break;
          }
          break;

        default:
          cout << "Define setup! " << endl;
          return;
          ;
      }
      tofAnaTestbeam->SetCh4Sel(15);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(15 * dScalFac);  // Width  of channel selection window
      break;

    case 300901:
    case 400901:
    case 920901:
    case 921901:
    case 910901:
    case 900901:
      cout << "Run with iRSel = " << iRSel << endl;
      switch (iRSel) {
        case 5:
          tofAnaTestbeam->SetTShift(3500.);   // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(20000.);  // initialization
          switch (iSel2) {
            case 3:
              tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
              break;
            case 4:
              tofAnaTestbeam->SetSel2TOff(70.);  // Shift Sel2 time peak to 0
              break;
            case 9:
              switch (iSel2Sm) {
                case 0:
                  tofAnaTestbeam->SetSel2TOff(20.);  // Shift Sel2 time peak to 0   //900
                  break;
                default:
                  cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc << " not configured yet as iSel2"
                       << endl;
                  return;
              }
              break;
            default:;
          }
          break;
        case 3:
          tofAnaTestbeam->SetTShift(1950.);    // initialization
          tofAnaTestbeam->SetTOffD4(13000.);   // initialization
          tofAnaTestbeam->SetSel2TOff(2070.);  // Shift Sel2 time peak to 0
          break;
        default:
          cout << "Define setup! " << endl;
          return;
          ;
      }
      tofAnaTestbeam->SetCh4Sel(15);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(15 * dScalFac);  // Width  of channel selection window
      break;

    case 300900:
    case 400900:
    case 920900:
    case 921900:
    case 910900:
    case 901900:
      cout << "Run with iRSel = " << iRSel << endl;
      switch (iRSel) {
        case 5:
          tofAnaTestbeam->SetTShift(3500.);   // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(20000.);  // initialization
          switch (iSel2) {
            case 3:
              tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
              break;
            case 4:
              tofAnaTestbeam->SetSel2TOff(70.);  // Shift Sel2 time peak to 0
              break;
            case 9:
              switch (iSel2Sm) {
                case 0:
                  tofAnaTestbeam->SetSel2TOff(20.);  // Shift Sel2 time peak to 0   //901
                  break;
                default:
                  cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc << " not configured yet as iSel2"
                       << endl;
                  return;
              }
              break;
            default:;
          }
          break;
        case 3:
          tofAnaTestbeam->SetTShift(1950.);    // initialization
          tofAnaTestbeam->SetTOffD4(13000.);   // initialization
          tofAnaTestbeam->SetSel2TOff(2070.);  // Shift Sel2 time peak to 0
          break;
        default:
          cout << "Define setup! " << endl;
          return;
          ;
      }
      tofAnaTestbeam->SetCh4Sel(15);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(15 * dScalFac);  // Width  of channel selection window
      break;

    case 300400:
    case 900400:
    case 901400:
    case 910400:
    case 920400:
    case 921400:
      cout << "Run with iRSel = " << iRSel << endl;
      switch (iRSel) {
        case 3:
          tofAnaTestbeam->SetTShift(2.5);     // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(0.);      // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(2.76);  // Shift Sel2 time peak to 0
          break;

        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(2.5);  // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(17.);  // Shift DTD4 to physical value
          switch (iSel2) {
            case 3:
              tofAnaTestbeam->SetSel2TOff(-0.045);  // Shift Sel2 time peak to 0
              break;
            case 9:
              switch (iSel2Sm) {
                case 2:
                  switch (iSel2Rpc) {
                    case 0:
                      tofAnaTestbeam->SetSel2TOff(0.0);  // Shift Sel2 time peak to 0
                      break;
                    case 1:
                      tofAnaTestbeam->SetSel2TOff(0.024);  // Shift Sel2 time peak to 0
                      break;
                    default:
                      cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc
                           << " not configured yet as iSel2" << endl;
                      return;
                  }
                  break;
                case 0:
                  switch (iSel2Rpc) {
                    case 0:
                      tofAnaTestbeam->SetSel2TOff(-40.);  // Shift Sel2 time peak to 0
                      break;
                    case 1:
                      tofAnaTestbeam->SetSel2TOff(-45.);  // Shift Sel2 time peak to 0
                      break;
                    default:
                      cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc
                           << " not configured yet as iSel2" << endl;
                      return;
                  }
                  break;
                default:
                  cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc << " not configured yet as iSel2"
                       << endl;
                  return;
              }
              break;
            default:
              cout << "Counter " << iSel2 << ", " << iSel2Sm << ", " << iSel2Rpc << " not configured yet as iSel2"
                   << endl;
              return;
          }
          break;

        case 9:
          tofAnaTestbeam->SetTShift(100.);    // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(16000.);  // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(500.);  // Shift Sel2 time peak to 0
          //tofTestBeamClust->SetBeamAddRefMul(1);
          break;

        default:
          cout << "Define setup! " << endl;
          return;
          ;
      }
      tofAnaTestbeam->SetCh4Sel(8);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(8 * dScalFac);  // Width  of channel selection window
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
      cout << "Run with iRSel = " << iRSel << ", iSel2 = " << iSel2 << endl;
      switch (iRSel) {
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          switch (iRSelSm) {
            case 0:
              tofAnaTestbeam->SetTShift(-8300.);  // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(17000.);  // Shift DTD4 to physical value
              break;
            case 1:
              tofAnaTestbeam->SetTShift(200.);    // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(17000.);  // Shift DTD4 to physical value
              break;
          }

          switch (iSel2) {
            case 0:
              tofAnaTestbeam->SetSel2TOff(2900.);  // Shift Sel2 time peak to 0
              break;
            case 1:
              tofAnaTestbeam->SetSel2TOff(-85.);  // Shift Sel2 time peak to 0
              break;
            case 6:
              tofAnaTestbeam->SetSel2TOff(85.);  // Shift Sel2 time peak to 0
              break;
            default:;
          }  // iSel2 switch end
          break;
        case 1:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(2500.);    // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(17000.);   // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(2900.);  // Shift Sel2 time peak to 0
          switch (iSel2) {
            case 0:
              tofAnaTestbeam->SetSel2TOff(2900.);  // Shift Sel2 time peak to 0
              break;
            case 1:
              tofAnaTestbeam->SetTShift(0.);    // Shift DTD4 to 0
              tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
              break;
            case 6:
              tofAnaTestbeam->SetSel2TOff(85.);  // Shift Sel2 time peak to 0
              break;
            default:;
          }  // iSel2 switch end
          break;
        case 6:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(-70.);    // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(15000.);  // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(-50.);  // Shift Sel2 time peak to 0
          break;

        case 7:
          switch (iSel2Rpc) {
            case 0:
              tofAnaTestbeam->SetTShift(-38.);    // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(15000.);  // Shift DTD4 to physical value
              tofAnaTestbeam->SetSel2TOff(-42.);  // Shift Sel2 time peak to 0
              break;
          }
          break;

        default: cout << "Define setup! " << endl; return;
      }
      tofAnaTestbeam->SetCh4Sel(16);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(16 * dScalFac);  // Width  of channel selection window
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
      cout << "Run with iRSel = " << iRSel << endl;
      switch (iRSel) {
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          switch (iRSelSm) {
            case 0:
              tofAnaTestbeam->SetTShift(-5300.);  // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(17000.);  // Shift DTD4 to physical value
              break;
            case 1:
              tofAnaTestbeam->SetTShift(290.);    // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(17000.);  // Shift DTD4 to physical value
              break;
          }
          switch (iSel2) {
            case 0:
              tofAnaTestbeam->SetSel2TOff(2900.);  // Shift Sel2 time peak to 0
              break;
            case 1:
              tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
              break;
            case 6:
              tofAnaTestbeam->SetSel2TOff(70.);  // Shift Sel2 time peak to 0
              break;
            default:;
          }  // iSel2 switch end
          break;
        case 1:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(2500.);    // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(17000.);   // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(2900.);  // Shift Sel2 time peak to 0
          break;
        case 6:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(-500.);   // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(17000.);  // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(0.);    // Shift Sel2 time peak to 0
          break;
        default: cout << "Define setup! " << endl; return;
      }
      tofAnaTestbeam->SetCh4Sel(16);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(16 * dScalFac);  // Width  of channel selection window
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
      cout << "Run with iRSel = " << iRSel << endl;
      switch (iRSel) {
        case 5:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(-6300.);   // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(17000.);   // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(2900.);  // Shift Sel2 time peak to 0
          switch (iSel2) {
            case 0: break;
            case 6:
              switch (iSel2Rpc) {
                case 0:
                  tofAnaTestbeam->SetSel2TOff(2000.);  // Shift Sel2 time peak to 0 // 600
                  break;
                case 1:
                  tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0 // 601
                  break;
              }
              break;
            default:;
          }  // iSel2 switch end
          break;
        case 6:
          //tofTestBeamClust->SetBeamAddRefMul(1);
          tofAnaTestbeam->SetTShift(-3000.);    // Shift DTD4 to 0
          tofAnaTestbeam->SetTOffD4(17000.);    // Shift DTD4 to physical value
          tofAnaTestbeam->SetSel2TOff(-2900.);  // Shift Sel2 time peak to 0 for 601
          break;
        default: cout << "Define setup! " << endl; return;
      }

      tofAnaTestbeam->SetCh4Sel(32);              // Center of channel selection window
      tofAnaTestbeam->SetDCh4Sel(32 * dScalFac);  // Width  of channel selection window
      break;
    default: cout << "Define analysis setup! " << endl; return;
  }  // end of different subsets

  cout << " Initialize TSHIFT to " << tofAnaTestbeam->GetTShift() << endl;
  run->AddTask(tofAnaTestbeam);

  // =========================================================================
  /*
   CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
   display->SetUpdateInterval(1000);
   run->AddTask(display);   
   */
  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  Bool_t kParameterMerged   = kTRUE;
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

  //  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  //  parInput1->open(ParFile.Data());
  //  rtdb->setFirstInput(parInput1);

  /*
  FairEventManager *fMan= new FairEventManager();
  CbmPixelHitSetDraw *TofHits =   new CbmPixelHitSetDraw ("TofHit", kRed, kFullSquare);
  fMan->AddTask(TofHits);   
  fMan->Init();                     
   */

  // -----   Intialise and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------
  // default display
  TString Display_Status = "pl_over_Mat04D4best.C";
  TString Display_Funct  = "pl_over_Mat04D4best(1)";
  gROOT->LoadMacro(Display_Status);
  gInterpreter->ProcessLine(Display_Funct);

  gROOT->LoadMacro("pl_over_MatD4sel.C");
  gROOT->LoadMacro("pl_over_trk.C");
  gROOT->LoadMacro("pl_calib_trk.C");
  gROOT->LoadMacro("pl_TIS.C");
  gROOT->LoadMacro("pl_eff_XY.C");
  gROOT->LoadMacro("save_hst.C");

  gInterpreter->ProcessLine("pl_over_MatD4sel()");
  gInterpreter->ProcessLine("pl_TIS()");
  gInterpreter->ProcessLine("pl_eff_XY()");
  //gInterpreter->ProcessLine("pl_over_trk(9)");
  //gInterpreter->ProcessLine("pl_calib_trk()");
  TString SaveToHstFile = "save_hst(\"" + cHstFile + "\")";
  gInterpreter->ProcessLine(SaveToHstFile);
}
