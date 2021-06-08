/* Copyright (C) 2017-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer], Florian Uhlig */

void mcbm_Ana(Int_t nEvents = 1000, TString cSys = "lam", TString cEbeam = "2.5gev", TString cCentr = "-",
              Int_t iRun = 0, Int_t parSet = 0, const char* setupName = "sis18_mcbm_20deg_long")
{
  const Char_t* setup = "sis18_mcbm";
  TString outDir      = "data/";

  TString InputFile = outDir + setup + "_" + cSys + "." + cEbeam + "." + cCentr + ".mc." + Form("%05d", iRun) + ".root";
  TString RecoFile = outDir + setup + "_" + cSys + "." + cEbeam + "." + cCentr + ".eds." + Form("%05d", iRun) + ".root";
  TString ParFile =
    outDir + setup + "_" + cSys + "." + cEbeam + "." + cCentr + ".params." + Form("%05d", iRun) + ".root";

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogVerbosityLevel("MEDIUM");

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* fRun = new FairRunAna();

  fRun->SetInputFile(InputFile.Data());
  fRun->AddFriend(RecoFile.Data());

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb          = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(ParFile.Data());
  rtdb->setFirstInput(parInput1);

  //histograms
  fRun->SetOutputFile(
    Form("data/Ana.%s.%s.%s.%s.cut%d.%05d.root", setupName, cSys.Data(), cEbeam.Data(), cCentr.Data(), parSet, iRun));
  TFile* fHist = fRun->GetOutputFile();

  CbmHadronAnalysis* HadronAna = new CbmHadronAnalysis();  // in hadron
  HadronAna->SetBeamMomentum(8.);                          // beam momentum
  //HadronAna->SetBSelMax(8.99);     // maximum impact parameter to be analyzed
  //HadronAna->SetBSelMin(6.01);     // minimum impact parameter to be analyzed
  HadronAna->SetDY(0.5);        // flow analysis exclusion window
  HadronAna->SetRecSec(kTRUE);  // enable lambda reconstruction

  switch (parSet) {
    case 0:                             // with background
      HadronAna->SetDistPrimLim(1.2);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.5);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(0.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    case 1:                             // for signal with background Ni+Ni
      HadronAna->SetDistPrimLim(1.);    // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.4);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(0.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    case 2:                             // for signal with background Au+Au
      HadronAna->SetDistPrimLim(1.);    // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.4);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(8.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(0.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;

    case 3:                             // syst study  around case 1
      HadronAna->SetDistPrimLim(0.8);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.4);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(0.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    case 4:                             // syst study  around case 1
      HadronAna->SetDistPrimLim(0.9);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.4);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(0.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    case 5:                             // syst study  around case 1
      HadronAna->SetDistPrimLim(1.1);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.4);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(0.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    case 6:                             // syst study  around case 1
      HadronAna->SetDistPrimLim(1.2);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.4);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(0.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    case 7:                             // syst study  around case 1
      HadronAna->SetDistPrimLim(0.7);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.4);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(0.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    case 8:                             // syst study  around case 1
      HadronAna->SetDistPrimLim(0.6);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.4);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(0.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;

    case 10:                            // "0" with TRD Mul 1
      HadronAna->SetDistPrimLim(1.2);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.5);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(1.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    case 20:                            // "0" with TRD Mul 2
      HadronAna->SetDistPrimLim(1.2);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.5);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(10.);       // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(2.);     // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    default: cout << "Cut value set " << parSet << " not existing, stop macro " << endl; return;
  }
  fRun->AddTask(HadronAna);

  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  cout << "Starting run" << endl;
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // save histos to file
  fHist->Write();

  gROOT->LoadMacro("save_hst.C");
  TString FSave = Form("save_hst(\"data/status_%s_%s.%05d.hst.root\")", setup, cSys.Data(), iRun);
  //gInterpreter->ProcessLine(FSave.Data());
}
