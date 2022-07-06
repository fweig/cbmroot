/* Copyright (C) 2021-2022 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void ini_AnaTestbeam(Int_t iSel = 910041, TString cFileId = "1255.50.3.0", Int_t iSel2 = 31, Int_t iRSel = 500,
                     Double_t dScalFac = 0.9, Int_t iAnaCor = 1, Int_t iMc = 0)
{
  Int_t iVerbose   = 1;
  TString cAnaFile = Form("%s_TrkAnaTestBeam.hst.root", cFileId.Data());

  if (cFileId == "") {
    cout << "<E> No action without cFileId!" << endl;
    return;
  }
  TString FId = cFileId;
  Int_t iNLen = FId.First(".");
  TString cRun(FId(0, iNLen));
  Int_t iRun = cRun.Atoi();
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

  Int_t iSel2in  = iSel2;
  Int_t iSel2Rpc = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Rpc) / 10;
  Int_t iSel2Sm  = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Sm) / 10;

  //Int_t iRSel    = 500;

  Int_t iRSelin  = iRSel;
  Int_t iRSelRpc = iRSel % 10;
  Int_t iRSelSm  = ((iRSel - iRSelRpc) / 10) % 10;
  Int_t iRSelTyp = iRSel / 100;

  cout << "Beam reference RSel " << iRSelin << ", TSR " << iRSelTyp << iRSelSm << iRSelRpc << endl;

  // =========================================================================
  // ===                       Analysis                                    ===
  // =========================================================================
  CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis", iVerbose);
  tofAnaTestbeam->SetCorMode(iAnaCor);  // 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp
  tofAnaTestbeam->SetHitDistMin(30.);   // initialization
  tofAnaTestbeam->SetEnableMatchPosScaling(kTRUE);
  tofAnaTestbeam->SetSpillDuration(3.);
  if (iMc == 1) {
    tofAnaTestbeam->SetSpillDuration(0.);
    tofAnaTestbeam->SetSpillBreak(0.);
  }
  //CbmTofAnaTestbeam defaults
  tofAnaTestbeam->SetR0LimFit(50.);  // limit distance of fitted track to nominal vertex
  tofAnaTestbeam->SetStartSpillTime(0.);

  tofAnaTestbeam->SetDXMean(0.);
  tofAnaTestbeam->SetDYMean(0.);
  tofAnaTestbeam->SetDTMean(0.);  // in ns
  tofAnaTestbeam->SetDXWidth(0.5);
  tofAnaTestbeam->SetDYWidth(0.8);
  tofAnaTestbeam->SetDTWidth(0.08);  // in ns
  tofAnaTestbeam->SetCalParFileName(cAnaFile);
  tofAnaTestbeam->SetPosY4Sel(0.5 * dScalFac);  // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetDTDia(0.);                 // Time difference to additional diamond
  tofAnaTestbeam->SetMul0Max(20);               // Max Multiplicity in dut
  tofAnaTestbeam->SetMul4Max(30);               // Max Multiplicity in Ref - RPC
  tofAnaTestbeam->SetMulDMax(3);                // Max Multiplicity in Diamond / BeamRef
  tofAnaTestbeam->SetTOffD4(14.);               // initialization
  tofAnaTestbeam->SetDTD4MAX(2.);               // 6.);      // initialization of Max time difference Ref - BRef

  //tofAnaTestbeam->SetTShift(-28000.);// initialization
  tofAnaTestbeam->SetPosYS2Sel(0.55);  // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetChS2Sel(0.);      // Center of channel selection window
  tofAnaTestbeam->SetDChS2Sel(100.);   // Width  of channel selection window
  tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
  tofAnaTestbeam->SetChi2Lim(5.);      // initialization of Chi2 selection limit
  tofAnaTestbeam->SetChi2Lim2(3.);     // initialization of Chi2 selection limit for Mref-Sel2 pair
  tofAnaTestbeam->SetDutDX(15.);       // limit inspection of tracklets to selected region
  tofAnaTestbeam->SetDutDY(15.);       // limit inspection of tracklets to selected region
  tofAnaTestbeam->SetSIGLIM(3.);       // max matching chi2
  tofAnaTestbeam->SetSIGT(0.08);       // in ns
  tofAnaTestbeam->SetSIGX(0.3);        // in cm
  tofAnaTestbeam->SetSIGY(0.6);        // in cm

  tofAnaTestbeam->SetBeamRefSmType(iRSelTyp);  // common reaction reference
  tofAnaTestbeam->SetBeamRefSmId(iRSelSm);
  tofAnaTestbeam->SetBeamRefRpc(iRSelRpc);

  if (iSel2 >= -1) {
    tofAnaTestbeam->SetMrpcSel2(iSel2);        // initialization of second selector Mrpc Type
    tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);    // initialization of second selector Mrpc SmId
    tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);  // initialization of second selector Mrpc RpcId
  }

  cout << "<I> AnaTestbeam init for Dut " << iDut << iDutSm << iDutRpc << ", Ref " << iRef << iRefSm << iRefRpc << endl;

  tofAnaTestbeam->SetDut(iDut);            // Device under test
  tofAnaTestbeam->SetDutSm(iDutSm);        // Device under test
  tofAnaTestbeam->SetDutRpc(iDutRpc);      // Device under test
  tofAnaTestbeam->SetMrpcRef(iRef);        // Reference RPC
  tofAnaTestbeam->SetMrpcRefSm(iRefSm);    // Reference RPC
  tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);  // Reference RPC

  cout << "<I> dispatch iSel = " << iSel << ", iSel2in = " << iSel2in << ", iRSelin = " << iRSelin
       << ", iRSel = " << iRSel << endl;

  if (1) {
    switch (iSel) {

      case 10:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(2.5);  // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(18.);  // Shift DTD4 to physical value
            switch (iSel2in) {
              case 20:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              default:;
            }
            break;
          default:;
        }
        break;

      case 700040:
      case 900040:
      case 901040:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(0.3);  // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(18.);  // Shift DTD4 to physical value

            switch (iSel2in) {
              case 30:
                tofAnaTestbeam->SetSel2TOff(-0.3);  // Shift Sel2 time peak to 0
                break;
              case 31:
                tofAnaTestbeam->SetSel2TOff(-0.41);  // Shift Sel2 time peak to 0
                break;

              default:;
            }
            break;
          default:;
        }
        break;

      case 700041:
      case 900041:
      case 901041:
      case 910041:
      case 911041:
        switch (iRSelin) {
          case 500:
            if (iMc == 0) {  // data
              //tofAnaTestbeam->SetTShift(7.);  // Shift DTD4 to 0
              tofAnaTestbeam->SetTShift(-1.);  // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(14.);  // Shift DTD4 to physical value
            }
            else {                             // MC
              tofAnaTestbeam->SetTShift(-2.);  // Shift DTD4 to 0
              tofAnaTestbeam->SetTOffD4(15.);  // Shift DTD4 to physical value
            }
            switch (iSel2in) {
              case 30:
                tofAnaTestbeam->SetSel2TOff(-0.3);  // Shift Sel2 time peak to 0
                break;

              case 910:
              case 911:
              case 31:
                if (iMc == 0) {
                  switch (iRun) {
                    case 727:
                    case 726:
                    case 723:
                    case 721:
                      tofAnaTestbeam->SetTShift(6.5);    // Shift DTD4 to 0
                      tofAnaTestbeam->SetSel2TOff(0.6);  // Shift Sel2 time peak to 0
                      break;
                    case 1044:
                    case 1051:
                    case 1058:
                      tofAnaTestbeam->SetTShift(-3.);   // Shift DTD4 to 0
                      tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                      break;

                    case 1255:
                    case 1258:
                      tofAnaTestbeam->SetSel2TOff(0.3);  // Shift Sel2 time peak to 0
                      break;

                    case 1256:
                    case 1259:
                    case 1260:
                    case 1262:
                    case 1263:
                      tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                      break;

                    case 1266:
                    case 1267:
                    case 1268:
                    case 1269:
                    case 1270:
                    case 1271:
                    case 1273:
                    case 1274:
                    case 1276:
                    case 1277:
                    case 1278:
                    case 1279:
                    case 1280:
                    case 1281:
                    case 1282:
                    case 1283:
                    case 1284:
                      //tofAnaTestbeam->SetSel2TOff(-5.3);  // Shift Sel2 time peak to 0
                      tofAnaTestbeam->SetSel2TOff(0.4);  // Shift Sel2 time peak to 0
                      break;

                    case 1275:
                      tofAnaTestbeam->SetSel2TOff(0.15);  // Shift Sel2 time peak to 0
                      break;

                    case 717:
                    default:  // 714
                      //tofAnaTestbeam->SetSel2TOff(-1.3);  // Shift Sel2 time peak to 0
                      tofAnaTestbeam->SetSel2TOff(-0.5);  // Shift Sel2 time peak to 0
                  }
                }
                else {                                // MC
                  tofAnaTestbeam->SetSel2TOff(-1.3);  // Shift Sel2 time peak to 0
                }
                break;
              case 600:
                tofAnaTestbeam->SetSel2TOff(-0.2);  // Shift Sel2 time peak to 0
                break;

              default:;
            }
            break;

          case 901:  // June 2021
            switch (iSel2in) {
              case 31:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
            }

          default:;
        }
        break;

      case 600041:
      case 601041:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(5.3);  // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(11.);  // Shift DTD4 to physical value

            switch (iSel2in) {
              case 33:
                tofAnaTestbeam->SetSel2TOff(-0.55);  // Shift Sel2 time peak to 0
                break;

              default:;
            }
            break;

          case 901:
            switch (iSel2in) {
              case 900:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;

              default:;
            }
            break;
          default:;
        }
        break;

      case 600043:
      case 601043:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(11.);  // Shift DTD4 to physical value

            switch (iSel2in) {
              case 33:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;

              default:;
            }
            break;
          default:;
        }
        break;

      case 900600:
      case 901600:
        switch (iRSelin) {
          case 900:
          case 901:
            tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(11.);  // Shift DTD4 to physical value
            switch (iSel2in) {
              case 31:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              case 41:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              default:;
            }
            break;
          default:;
        }
        break;

      case 12022:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(3.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(15.);  // Shift DTD4 to physical value

            switch (iSel2in) {
              case 2:
                tofAnaTestbeam->SetSel2TOff(0.25);  // Shift Sel2 time peak to 0
                break;

              default:;
            }
            break;
          default:;
        }
        break;

      case 911910:
      case 900910:
      case 901910:
      case 31910:
      case 41910:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(10.);  // Shift DTD4 to physical value

            switch (iSel2in) {
              case 31:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;

              case 41:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;

              default:; tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            }
            break;
          default:;
        }
        break;

      case 910911:
      case 900911:
      case 901911:
      case 31911:
      case 41911:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(11.);  // Shift DTD4 to physical value
            switch (iSel2in) {
              case 31:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              case 41:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              default:;
            }
            break;
          default:;
        }
        break;

      case 202042:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(11.);  // Shift DTD4 to physical value
            switch (iSel2in) {
              case 12:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              case 2:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              default:;
            }
            break;
          default:;
        }
        break;

      case 10000:
      case 11001:
      case 12002:
      case 13003:
      case 14004:
      case 202002:
      case 202012:
      case 202022:
      case 22002:
        tofAnaTestbeam->SetTOffD4(11.);  // Shift DTD4 to physical value
        switch (iRSelin) {
          case 11:
          case 22:
            tofAnaTestbeam->SetTShift(0.);  // Shift DTD4 to 0
            switch (iSel2in) {
              case 22:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              case 2:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              default:;
            }
            break;
          default:;
        }
        break;

      default:
        cout << "Setup " << iSel
             << " unknown, better to define analysis setup! Running with default offset parameter... " << endl;
        // return;
    }  // end of different subsets

    cout << "<I> Initialize TSHIFT to " << tofAnaTestbeam->GetTShift() << endl;
    FairRunAna* run = FairRunAna::Instance();
    if (NULL != run) run->AddTask(tofAnaTestbeam);
  }
}
