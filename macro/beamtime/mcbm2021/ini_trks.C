/* Copyright (C) 2021 Physikalisches Institut, Universitaet Heidelberg, Heidelberg 
   SPDX-License-Identifier: GPL-3.0-only
   Authors:  Norbert Herrmann [committer]*/

void ini_trks(Int_t iSel = 910041, Int_t iTrackingSetup = 4, Int_t iGenCor = 1, Double_t dScalFac = 1.,
              Double_t dChi2Lim2 = 500., TString cCalId = "", Bool_t bUseSigCalib = kFALSE, Int_t iCalOpt = 1,
              Int_t iTrkPar = 2)
{
  if (cCalId == "") {
    cout << "<E> No action without cCalId!" << endl;
    return;
  }
  // =========================================================================
  // ===                   Tof Tracking                                    ===
  // =========================================================================
  cout << "<I> Initialize Tof tracker by ini_trks" << endl;
  TString cTrkFile = Form("%s_tofFindTracks.hst.root", cCalId.Data());
  // -----   Local selection variables  -------------------------------------------

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
  Int_t iBucRpc = 0;

  CbmTofTrackFinder* tofTrackFinder = new CbmTofTrackFinderNN();
  tofTrackFinder->SetMaxTofTimeDifference(0.2);  // in ns/cm

  CbmTofTrackFitter* tofTrackFitter = new CbmTofTrackFitterKF(0, 211);
  TFitter* MyFit                    = new TFitter(1);  // initialize Minuit
  tofTrackFinder->SetFitter(tofTrackFitter);
  CbmTofFindTracks* tofFindTracks = new CbmTofFindTracks("TOF Track Finder");
  tofFindTracks->UseFinder(tofTrackFinder);
  tofFindTracks->UseFitter(tofTrackFitter);

  tofFindTracks->SetCalOpt(iCalOpt);   // 1 - update offsets, 2 - update walk, 0 - bypass
  tofFindTracks->SetCorMode(iGenCor);  // valid options: 0,1,2,3,4,5,6, 10 - 19
  //tofFindTracks->SetTtTarg(0.047);     // target value for Mar2021 double stack, v21b
  tofFindTracks->SetTtTarg(0.035);  // target value for Jun2021 double stack, v21c, v21d
  //  0.0605);  // target value for Mar2020 triple stack -> betapeak ~ 0.95
  //tofFindTracks->SetTtTarg(0.062);           // target value for Mar2020 triple stack -> betapeak ~ 0.95
  //tofFindTracks->SetTtTarg(0.058);           // target value for Mar2020 double stack
  //tofFindTracks->SetTtTarg(0.051);           // target value Nov2019
  //tofFindTracks->SetTtTarg(0.035);           // target value for inverse velocity, > 0.033 ns/cm!
  tofFindTracks->SetCalParFileName(cTrkFile);  // Tracker parameter value file name
  tofFindTracks->SetR0Lim(5.);
  tofFindTracks->SetStationMaxHMul(30);  // Max Hit Multiplicity in any used station

  tofFindTracks->SetT0MAX(dScalFac);            // in ns
  tofFindTracks->SetSIGT(0.08);                 // default in ns
  tofFindTracks->SetSIGX(0.3);                  // default in cm
  tofFindTracks->SetSIGY(0.45);                 // default in cm
  tofFindTracks->SetSIGZ(0.05);                 // default in cm
  tofFindTracks->SetUseSigCalib(bUseSigCalib);  // ignore resolutions in CalPar file
  tofTrackFinder->SetSIGLIM(dChi2Lim2 * 2.);    // matching window in multiples of chi2
  tofTrackFinder->SetChiMaxAccept(dChi2Lim2);   // max tracklet chi2
  tofTrackFinder->SetSIGLIMMOD(5.);             // max deviation for last hit
  tofTrackFinder->SetAddVertex(kFALSE);         // disable virtual vertex

  cout << "<I> Tracker Parameter Set: " << iTrkPar << endl;
  switch (iTrkPar) {
    case 0:                                    // for full mTof setup
      tofTrackFinder->SetTxMean(0.);           // mean slope dy/dz
      tofTrackFinder->SetTyMean(0.);           // mean slope dy/dz
      tofTrackFinder->SetTxLIM(0.3);           // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.3);           // max dev from mean slope dy/dz
      tofFindTracks->SetBeamCounter(5, 0, 0);  // default beam counter
      break;
    case 1:                                    // for double stack test counters
      tofTrackFinder->SetTxMean(0.);           // mean slope dy/dz
      tofTrackFinder->SetTyMean(0.18);         // mean slope dy/dz
      tofTrackFinder->SetTxLIM(0.15);          // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.18);          // max dev from mean slope dy/dz
      tofFindTracks->SetBeamCounter(5, 0, 0);  // default beam counter
      break;
    case 2:                             // for double stack cosmics
      tofTrackFinder->SetTxMean(0.);    // mean slope dy/dz
      tofTrackFinder->SetTyMean(0.);    // mean slope dy/dz
      tofTrackFinder->SetTxLIM(2.);     // max slope dx/dz
      tofTrackFinder->SetTyLIM(20.);    // max dev from mean slope dy/dz
      tofFindTracks->SetTtMin(-100.);   // allow negative velocities with respect to z-axis
      tofFindTracks->SetTtTarg(0.033);  // target value for mCBM Cosmic2021 triple stack, v21c, v21d
      tofFindTracks->SetSIGT(0.2);      // default in ns
      break;
    case 3:                                 // for Jul2021-CRI
      tofTrackFinder->SetTxMean(-0.2);      // mean slope dy/dz
      tofTrackFinder->SetTyMean(0.);        // mean slope dy/dz
      tofTrackFinder->SetTxLIM(0.1);        // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.3);        // max dev from mean slope dy/dz
      tofTrackFinder->SetAddVertex(kTRUE);  // add virtual vertex
      tofFindTracks->SetTtMin(-100.);       // allow negative velocities with respect to z-axis
      tofFindTracks->SetR0Lim(30.);         // allow for large extrapolation errors
      break;
  }

  Int_t iMinNofHits   = -1;
  Int_t iNStations    = 0;
  Int_t iNReqStations = 3;

  switch (iTrackingSetup) {
    case 0:  // bypass mode
      iMinNofHits = -1;
      iNStations  = 1;
      tofFindTracks->SetStation(0, 5, 0, 0);  // Diamond
      break;

    case 1:  // for calibration mode of full setup
    {
      Double_t dTsig = dScalFac * 0.03;
      tofFindTracks->SetSIGT(dTsig);  // allow for variable deviations in ns
    }
      iMinNofHits   = 3;
      iNStations    = 30;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 2, 2);
      tofFindTracks->SetStation(2, 0, 1, 2);
      tofFindTracks->SetStation(3, 0, 0, 2);
      tofFindTracks->SetStation(4, 0, 2, 1);
      tofFindTracks->SetStation(5, 0, 1, 1);
      tofFindTracks->SetStation(6, 0, 0, 1);
      tofFindTracks->SetStation(7, 0, 2, 3);
      tofFindTracks->SetStation(8, 0, 1, 3);
      tofFindTracks->SetStation(9, 0, 0, 3);
      tofFindTracks->SetStation(10, 0, 2, 0);
      tofFindTracks->SetStation(11, 0, 1, 0);
      tofFindTracks->SetStation(12, 0, 0, 0);
      tofFindTracks->SetStation(13, 0, 2, 4);
      tofFindTracks->SetStation(14, 0, 1, 4);
      tofFindTracks->SetStation(15, 0, 0, 4);
      tofFindTracks->SetStation(16, 0, 4, 0);
      tofFindTracks->SetStation(17, 0, 3, 0);
      tofFindTracks->SetStation(18, 0, 4, 1);
      tofFindTracks->SetStation(19, 0, 3, 1);
      tofFindTracks->SetStation(20, 0, 4, 2);
      tofFindTracks->SetStation(21, 0, 3, 2);
      tofFindTracks->SetStation(22, 0, 4, 3);
      tofFindTracks->SetStation(23, 0, 3, 3);
      tofFindTracks->SetStation(24, 0, 4, 4);
      tofFindTracks->SetStation(25, 0, 3, 4);
      tofFindTracks->SetStation(26, 9, 0, 0);
      tofFindTracks->SetStation(27, 9, 1, 0);
      tofFindTracks->SetStation(28, 9, 0, 1);
      tofFindTracks->SetStation(29, 9, 1, 1);
      //tofFindTracks->SetStation(28, 6, 0, 0);
      //tofFindTracks->SetStation(29, 6, 0, 1);
      break;

    case 10:  // for calibration mode of full setup
    {
      Double_t dTsig = dScalFac * 0.03;
      tofFindTracks->SetSIGT(dTsig);  // allow for variable deviations in ns
    }
      iMinNofHits   = 3;
      iNStations    = 37;
      iNReqStations = 4;
      tofFindTracks->SetStation(36, 5, 0, 0);
      tofFindTracks->SetStation(0, 0, 2, 2);
      tofFindTracks->SetStation(1, 0, 1, 2);
      tofFindTracks->SetStation(2, 0, 0, 2);
      tofFindTracks->SetStation(3, 0, 2, 1);
      tofFindTracks->SetStation(4, 0, 1, 1);
      tofFindTracks->SetStation(5, 0, 0, 1);
      tofFindTracks->SetStation(6, 0, 2, 3);
      tofFindTracks->SetStation(7, 0, 1, 3);
      tofFindTracks->SetStation(8, 0, 0, 3);
      tofFindTracks->SetStation(9, 0, 2, 0);
      tofFindTracks->SetStation(10, 0, 1, 0);
      tofFindTracks->SetStation(11, 0, 0, 0);
      tofFindTracks->SetStation(12, 0, 2, 4);
      tofFindTracks->SetStation(13, 0, 1, 4);
      tofFindTracks->SetStation(14, 0, 0, 4);
      tofFindTracks->SetStation(15, 0, 4, 0);
      tofFindTracks->SetStation(16, 0, 3, 0);
      tofFindTracks->SetStation(17, 0, 4, 1);
      tofFindTracks->SetStation(18, 0, 3, 1);
      tofFindTracks->SetStation(19, 0, 4, 2);
      tofFindTracks->SetStation(20, 0, 3, 2);
      tofFindTracks->SetStation(21, 0, 4, 3);
      tofFindTracks->SetStation(22, 0, 3, 3);
      tofFindTracks->SetStation(23, 0, 4, 4);
      tofFindTracks->SetStation(24, 0, 3, 4);
      tofFindTracks->SetStation(25, 9, 0, 0);
      tofFindTracks->SetStation(26, 9, 1, 0);
      tofFindTracks->SetStation(27, 9, 0, 1);
      tofFindTracks->SetStation(28, 9, 1, 1);
      tofFindTracks->SetStation(29, 6, 0, 0);
      tofFindTracks->SetStation(30, 6, 0, 1);
      tofFindTracks->SetStation(31, 2, 0, 0);
      tofFindTracks->SetStation(32, 2, 0, 1);
      tofFindTracks->SetStation(33, 2, 0, 2);
      tofFindTracks->SetStation(34, 2, 0, 3);
      tofFindTracks->SetStation(35, 2, 0, 4);
      break;

    case 11:  // for calibration mode of 2-stack & test counters
      iMinNofHits   = 3;
      iNStations    = 17;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 0, 4, 0);
      tofFindTracks->SetStation(1, 0, 3, 0);
      tofFindTracks->SetStation(2, 0, 4, 1);
      tofFindTracks->SetStation(3, 0, 3, 1);
      tofFindTracks->SetStation(4, 0, 4, 2);
      tofFindTracks->SetStation(5, 0, 3, 2);
      tofFindTracks->SetStation(6, 0, 4, 3);
      tofFindTracks->SetStation(7, 0, 3, 3);
      tofFindTracks->SetStation(8, 0, 4, 4);
      tofFindTracks->SetStation(9, 0, 3, 4);
      tofFindTracks->SetStation(10, 9, 0, 0);
      tofFindTracks->SetStation(11, 9, 1, 0);
      tofFindTracks->SetStation(12, 9, 0, 1);
      tofFindTracks->SetStation(13, 9, 1, 1);
      tofFindTracks->SetStation(14, 6, 0, 0);
      tofFindTracks->SetStation(15, 6, 0, 1);
      tofFindTracks->SetStation(16, 5, 0, 0);
      break;

    case 12:  // for calibration mode of 2-stack & test counters
      iMinNofHits   = 3;
      iNStations    = 9;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      tofFindTracks->SetStation(2, 9, 0, 0);
      tofFindTracks->SetStation(3, 9, 1, 0);
      tofFindTracks->SetStation(4, 6, 0, 0);
      tofFindTracks->SetStation(5, 9, 0, 1);
      tofFindTracks->SetStation(6, 9, 1, 1);
      tofFindTracks->SetStation(7, 6, 0, 1);
      tofFindTracks->SetStation(8, 5, 0, 0);
      break;

    case 13:  // for calibration mode of triple-stack (july2021-cri)
      iMinNofHits   = 3;
      iNStations    = 10;
      iNReqStations = 3;
      tofFindTracks->SetStation(0, 0, 1, 2);
      tofFindTracks->SetStation(1, 0, 0, 2);
      tofFindTracks->SetStation(2, 0, 1, 1);
      tofFindTracks->SetStation(3, 0, 0, 1);
      tofFindTracks->SetStation(4, 0, 1, 3);
      tofFindTracks->SetStation(5, 0, 0, 3);
      tofFindTracks->SetStation(6, 0, 1, 0);
      tofFindTracks->SetStation(7, 0, 0, 0);
      tofFindTracks->SetStation(8, 0, 1, 4);
      tofFindTracks->SetStation(9, 0, 0, 4);
      break;

    case 113:  // for double stack TSHU counter (031)
      iMinNofHits   = 6;
      iNStations    = 7;
      iNReqStations = 7;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 9, 0, 1);
      tofFindTracks->SetStation(2, 9, 1, 1);
      tofFindTracks->SetStation(3, 9, 1, 0);
      tofFindTracks->SetStation(4, 9, 0, 0);
      tofFindTracks->SetStation(5, 5, 0, 0);
      tofFindTracks->SetStation(6, iDut, iDutSm, iDutRpc);
      break;

    case 114:  // for double stack TSHU counter (041)
      iMinNofHits   = 6;
      iNStations    = 7;
      iNReqStations = 7;
      tofFindTracks->SetStation(0, 9, 1, 1);
      tofFindTracks->SetStation(1, 9, 0, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, 9, 1, 0);
      tofFindTracks->SetStation(4, 9, 0, 0);
      tofFindTracks->SetStation(5, 5, 0, 0);
      tofFindTracks->SetStation(6, iDut, iDutSm, iDutRpc);
      break;

    case 116:  // for evaluation of BUC counter (600, 601)
      iMinNofHits   = 4;
      iNStations    = 5;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 9, 0, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, 9, 0, 0);
      tofFindTracks->SetStation(4, iDut, iDutSm, iDutRpc);
      break;

    case 117:  // for evaluation of BUC counter (600, 601)
      iMinNofHits   = 5;
      iNStations    = 6;
      iNReqStations = 6;
      iBucRpc       = 1 - iDutRpc;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 9, 0, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, 9, 0, 0);
      tofFindTracks->SetStation(4, 6, 0, iBucRpc);
      tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
      break;

    case 2:
      iMinNofHits   = 3;
      iNStations    = 28;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 0, 2, 2);
      tofFindTracks->SetStation(1, 0, 0, 2);
      tofFindTracks->SetStation(2, 0, 1, 2);
      tofFindTracks->SetStation(3, 0, 2, 1);
      tofFindTracks->SetStation(4, 0, 0, 1);
      tofFindTracks->SetStation(5, 0, 1, 1);
      tofFindTracks->SetStation(6, 0, 2, 3);
      tofFindTracks->SetStation(7, 0, 0, 3);
      tofFindTracks->SetStation(8, 0, 1, 3);
      tofFindTracks->SetStation(9, 0, 2, 0);
      tofFindTracks->SetStation(10, 0, 0, 0);
      tofFindTracks->SetStation(11, 0, 1, 0);
      tofFindTracks->SetStation(12, 0, 2, 4);
      tofFindTracks->SetStation(13, 0, 0, 4);
      tofFindTracks->SetStation(14, 0, 1, 4);
      tofFindTracks->SetStation(15, 0, 4, 0);
      tofFindTracks->SetStation(16, 0, 3, 0);
      tofFindTracks->SetStation(17, 0, 4, 1);
      tofFindTracks->SetStation(18, 0, 3, 1);
      tofFindTracks->SetStation(19, 0, 4, 2);
      tofFindTracks->SetStation(20, 0, 3, 2);
      tofFindTracks->SetStation(21, 0, 4, 3);
      tofFindTracks->SetStation(22, 0, 3, 3);
      tofFindTracks->SetStation(23, 0, 4, 4);
      tofFindTracks->SetStation(24, 0, 3, 4);
      tofFindTracks->SetStation(25, 9, 0, 0);
      tofFindTracks->SetStation(26, 9, 0, 1);
      tofFindTracks->SetStation(27, 5, 0, 0);
      break;

    case 3:
      iMinNofHits   = 3;
      iNStations    = 16;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 2, 2);
      tofFindTracks->SetStation(2, 0, 1, 2);
      tofFindTracks->SetStation(3, 0, 0, 2);

      tofFindTracks->SetStation(4, 0, 2, 1);
      tofFindTracks->SetStation(5, 0, 1, 1);
      tofFindTracks->SetStation(6, 0, 0, 1);

      tofFindTracks->SetStation(7, 0, 2, 3);
      tofFindTracks->SetStation(8, 0, 1, 3);
      tofFindTracks->SetStation(9, 0, 0, 3);

      tofFindTracks->SetStation(10, 0, 2, 0);
      tofFindTracks->SetStation(11, 0, 1, 0);
      tofFindTracks->SetStation(12, 0, 0, 0);

      tofFindTracks->SetStation(13, 0, 2, 4);
      tofFindTracks->SetStation(14, 0, 1, 4);
      tofFindTracks->SetStation(15, 0, 0, 4);

      /*
     tofFindTracks->SetStation(16, 0, 3, 2);         
     tofFindTracks->SetStation(17, 0, 4, 2);  
     tofFindTracks->SetStation(18, 0, 3, 1);         
     tofFindTracks->SetStation(19, 0, 4, 1);
     tofFindTracks->SetStation(20, 0, 3, 3);         
     tofFindTracks->SetStation(21, 0, 4, 3);
     tofFindTracks->SetStation(22, 0, 3, 0);         
     tofFindTracks->SetStation(23, 0, 4, 0);
     tofFindTracks->SetStation(24, 0, 3, 4);         
     tofFindTracks->SetStation(25, 0, 4, 4); 
     */
      break;

    case 31:  // cosmic triple stack, Aug 2021
      iMinNofHits   = 3;
      iNStations    = 20;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 0, 0, 2);
      tofFindTracks->SetStation(1, 0, 2, 2);
      tofFindTracks->SetStation(2, 0, 1, 2);
      tofFindTracks->SetStation(3, 2, 0, 2);

      tofFindTracks->SetStation(4, 0, 0, 1);
      tofFindTracks->SetStation(5, 0, 2, 1);
      tofFindTracks->SetStation(6, 0, 1, 1);
      tofFindTracks->SetStation(7, 2, 0, 1);

      tofFindTracks->SetStation(8, 0, 0, 3);
      tofFindTracks->SetStation(9, 0, 2, 3);
      tofFindTracks->SetStation(10, 0, 1, 3);
      tofFindTracks->SetStation(11, 2, 0, 3);

      tofFindTracks->SetStation(12, 0, 0, 0);
      tofFindTracks->SetStation(13, 0, 2, 0);
      tofFindTracks->SetStation(14, 0, 1, 0);
      tofFindTracks->SetStation(15, 2, 0, 0);

      tofFindTracks->SetStation(16, 0, 0, 4);
      tofFindTracks->SetStation(17, 0, 2, 4);
      tofFindTracks->SetStation(18, 0, 1, 4);
      tofFindTracks->SetStation(19, 2, 0, 4);
      break;

    case 4:  // for USTC evaluation (dut=910,911)
      iMinNofHits   = 4;
      iNStations    = 5;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      //tofFindTracks->SetStation(2, 9, 0, 1);  // broken in May2021
      tofFindTracks->SetStation(2, 9, 0, 0);
      tofFindTracks->SetStation(3, 5, 0, 0);
      tofFindTracks->SetStation(4, iDut, iDutSm, iDutRpc);
      break;

    case 40:  // for USTC evaluation (dut=910)
      iMinNofHits   = 4;
      iNStations    = 6;
      iNReqStations = 6;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      //tofFindTracks->SetStation(2, 9, 0, 1);
      tofFindTracks->SetStation(2, 9, 0, 0);
      tofFindTracks->SetStation(3, 9, 1, 1);
      tofFindTracks->SetStation(4, 5, 0, 0);
      tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
      break;

    case 41:  // for USTC evaluation (dut=911)
      iMinNofHits   = 4;
      iNStations    = 6;
      iNReqStations = 6;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      //tofFindTracks->SetStation(2, 9, 0, 1);
      tofFindTracks->SetStation(2, 9, 0, 0);
      tofFindTracks->SetStation(3, 9, 1, 0);
      tofFindTracks->SetStation(4, 5, 0, 0);
      tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
      break;

    case 42:  // for USTC evaluation (dut=900, 901)
      iMinNofHits   = 4;
      iNStations    = 5;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      tofFindTracks->SetStation(2, 6, 0, 0);
      tofFindTracks->SetStation(3, 6, 0, 1);
      tofFindTracks->SetStation(4, iDut, iDutSm, iDutRpc);
      break;

    case 43:  // for USTC evaluation (dut=900, 901)
      iMinNofHits   = 3;
      iNStations    = 4;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      tofFindTracks->SetStation(2, 6, 0, 1);
      tofFindTracks->SetStation(3, iDut, iDutSm, iDutRpc);
      break;

    case 14:
      iMinNofHits   = 3;
      iNStations    = 15;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 0, 2, 2);
      tofFindTracks->SetStation(1, 0, 1, 2);
      tofFindTracks->SetStation(2, 0, 0, 2);
      tofFindTracks->SetStation(0, 0, 2, 1);
      tofFindTracks->SetStation(1, 0, 1, 1);
      tofFindTracks->SetStation(2, 0, 0, 1);
      tofFindTracks->SetStation(0, 0, 2, 0);
      tofFindTracks->SetStation(1, 0, 1, 0);
      tofFindTracks->SetStation(2, 0, 0, 0);
      tofFindTracks->SetStation(0, 0, 2, 3);
      tofFindTracks->SetStation(1, 0, 1, 3);
      tofFindTracks->SetStation(2, 0, 0, 3);
      tofFindTracks->SetStation(0, 0, 2, 4);
      tofFindTracks->SetStation(1, 0, 1, 4);
      tofFindTracks->SetStation(2, 0, 0, 4);
      break;

    case 5:  // for evaluation of Buc in 2-stack
      iMinNofHits   = 3;
      iNStations    = 4;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 0, 4, 3);
      tofFindTracks->SetStation(1, 0, 3, 3);
      tofFindTracks->SetStation(2, 5, 0, 0);
      tofFindTracks->SetStation(3, iDut, iDutSm, iDutRpc);
      break;

    case 50:  // for evaluation of Buc in 2-stack
      iMinNofHits   = 4;
      iNStations    = 5;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 0, 4, 3);
      tofFindTracks->SetStation(1, 0, 3, 3);
      tofFindTracks->SetStation(2, 6, 0, 1);
      tofFindTracks->SetStation(3, 5, 0, 0);
      tofFindTracks->SetStation(4, iDut, iDutSm, iDutRpc);
      break;

    case 51:  // for evaluation of Buc in 2-stack
      iMinNofHits   = 4;
      iNStations    = 5;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 0, 4, 3);
      tofFindTracks->SetStation(1, 0, 3, 3);
      tofFindTracks->SetStation(2, 6, 0, 0);
      tofFindTracks->SetStation(3, 5, 0, 0);
      tofFindTracks->SetStation(4, iDut, iDutSm, iDutRpc);
      break;

    case 6:  // for double stack TSHU counter (900,901) evaluation
      iMinNofHits   = 5;
      iNStations    = 6;
      iNReqStations = 6;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      tofFindTracks->SetStation(2, 9, 1, 1);
      tofFindTracks->SetStation(3, 9, 1, 0);
      tofFindTracks->SetStation(4, 5, 0, 0);
      tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
      break;

    case 60:  // for double stack TSHU counter (900) evaluation
      iMinNofHits   = 5;
      iNStations    = 7;
      iNReqStations = 7;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      tofFindTracks->SetStation(2, 9, 1, 1);
      tofFindTracks->SetStation(3, 9, 1, 0);
      tofFindTracks->SetStation(4, 9, 0, 1);
      tofFindTracks->SetStation(5, 5, 0, 0);
      tofFindTracks->SetStation(6, iDut, iDutSm, iDutRpc);
      break;

    case 61:  // for double stack TSHU counter (901) evaluation
      iMinNofHits   = 5;
      iNStations    = 7;
      iNReqStations = 7;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      tofFindTracks->SetStation(2, 9, 1, 1);
      tofFindTracks->SetStation(3, 9, 1, 0);
      tofFindTracks->SetStation(4, 9, 0, 0);
      tofFindTracks->SetStation(5, 5, 0, 0);
      tofFindTracks->SetStation(6, iDut, iDutSm, iDutRpc);
      break;

    case 7:  // for double stack mTOF counter evaluation
      iMinNofHits   = 4;
      iNStations    = 5;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 9, 1, 1);
      tofFindTracks->SetStation(1, 9, 0, 0);
      tofFindTracks->SetStation(2, 9, 1, 0);
      tofFindTracks->SetStation(3, 5, 0, 0);
      tofFindTracks->SetStation(4, iDut, iDutSm, iDutRpc);
      break;

    case 8:  // evaluation of add-on counters (BUC)
      iMinNofHits = 5;
      iNStations  = 6;

      iNReqStations = 6;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 9, 0, 1);
      tofFindTracks->SetStation(2, 0, 4, 1);
      tofFindTracks->SetStation(3, 9, 0, 0);
      tofFindTracks->SetStation(4, 0, 3, 1);
      tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
      break;

    case 81:  // evaluation of add-on counters (BUC) in July 2021
      iMinNofHits   = 4;
      iNStations    = 5;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 0, 4, 1);
      tofFindTracks->SetStation(1, 0, 3, 1);
      tofFindTracks->SetStation(2, 9, 0, 0);
      tofFindTracks->SetStation(3, 9, 0, 1);
      tofFindTracks->SetStation(4, iDut, iDutSm, iDutRpc);
      break;

    case 9:  // evaluation of M6, Dut=202
      iMinNofHits   = 4;
      iNStations    = 5;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 0, 2, 2);
      tofFindTracks->SetStation(1, 0, 0, 2);
      tofFindTracks->SetStation(2, 0, 1, 2);
      tofFindTracks->SetStation(3, 5, 0, 0);
      tofFindTracks->SetStation(4, iDut, iDutSm, iDutRpc);
      break;

    case 992:  // evaluation of CRI cosmics 2021
      iMinNofHits   = 3;
      iNStations    = 4;
      iNReqStations = 4;
      {
        Int_t iType[4]   = {0, 0, 0, 2};
        Int_t iModule[4] = {0, 2, 1, 0};
        Int_t iSt        = 0;
        for (Int_t iM = 0; iM < iNStations; iM++) {
          if (iType[iM] == iDut && iModule[iM] == iDutSm) continue;
          tofFindTracks->SetStation(iSt, iType[iM], iModule[iM], iDutRpc);
          iSt++;
        }
      }
      tofFindTracks->SetStation(3, iDut, iDutSm, iDutRpc);
      break;

    default:
      cout << "Tracking setup " << iTrackingSetup << " not implemented " << endl;
      return;
      ;
  }
  tofFindTracks->SetMinNofHits(iMinNofHits);
  tofFindTracks->SetNStations(iNStations);
  tofFindTracks->SetNReqStations(iNReqStations);
  tofFindTracks->PrintSetup();

  FairRunAna* run = FairRunAna::Instance();
  if (NULL != run) run->AddTask(tofFindTracks);
}
