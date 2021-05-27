/** @file mcbm_hadron_analysis_nh.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

void mcbm_hadron_analysis_nh(Int_t nEvents = 10, TString RunId = "test", TString InDir = "./data/",
                             TString OutDir = "./data/", TString setup = "mcbm_beam_2021_03", bool timebased = kTRUE,
                             Double_t eventRate       = 1.E7,
                             Double_t timeSliceLength = 1.e4,  // Length of time-slice [ns]
                             Double_t Tint = 100., Double_t ReqTofMul = 2,
                             Int_t parSet = 0  // cut parameter
)
{
  //TString TraDir ="../../../../../../uhlig/mcbm_proposal/data";
  TString TraDir    = InDir;
  TString InputFile = TraDir + "/" + RunId + ".tra.root";

  TString dataset  = InDir + "/" + RunId;
  TString ParFile  = dataset + ".par.root";
  TString DigiFile = dataset + ".event.raw.root";
  TString RecoFile = dataset + ".rec.root";
  TString OutFile  = OutDir + "/" + RunId + Form(".par%d", parSet) + ".ana.root";
  if (timebased) {
    DigiFile = dataset + Form(".%2.1e", eventRate) + ".raw.root";
    RecoFile = dataset + Form(".%2.1e.%d.%d", eventRate, (Int_t) Tint, (Int_t) ReqTofMul) + ".rec.root";
    OutFile  = OutDir + "/" + RunId + Form(".%2.1e.%d.%d", eventRate, (Int_t) Tint, (Int_t) ReqTofMul)
              + Form(".par%d", parSet) + ".ana.root";
  }

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("VERYHIGH");

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Reconstruction run   -------------------------------------------
  FairFileSource* inputFiles = new FairFileSource(RecoFile.Data());
  inputFiles->AddFriend(DigiFile.Data());
  //  inputFiles->AddFriend(InputFile.Data());

  FairRootFileSink* sink = new FairRootFileSink(OutFile.Data());

  FairRunAna* fRun = new FairRunAna();
  fRun->SetSource(inputFiles);
  fRun->SetSink(sink);

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb          = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(ParFile.Data());
  rtdb->setFirstInput(parInput1);

  /*
  CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 1);
  mcManager->AddFile(InputFile);
  fRun->AddTask(mcManager);

  CbmMatchRecoToMC* match = new CbmMatchRecoToMC();
  fRun->AddTask(match);
*/
  // TFile *fHist = fRun->GetOutputFile();

  CbmHadronAnalysis* HadronAna = new CbmHadronAnalysis();  // in hadron
  HadronAna->SetBeamMomentum(2.73);                        // beam momentum
  HadronAna->SetDY(0.5);                                   // flow analysis exclusion window
  HadronAna->SetRecSec(kTRUE);                             // enable lambda reconstruction
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
  }
  fRun->AddTask(HadronAna);

  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  cout << "Starting run" << endl;
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // save histos to file
  // fHist->Write();

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << OutFile << std::endl;
  std::cout << "Parameter file is " << ParFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl << std::endl;
  // ------------------------------------------------------------------------

  // -----   Resource monitoring   ------------------------------------------
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;

  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;


  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------

  //RemoveGeoManager();
}
