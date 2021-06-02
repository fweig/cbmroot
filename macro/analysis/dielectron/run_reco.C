/** @file run_reco.C
  * @copyright Copyright (C) 2011-2020 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev, Semen Lebedev, Elena Lebedeva [committer] **/

void run_reco(const string& mcFile   = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/mc.1.root",
              const string& parFile  = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/param.1.root",
              const string& digiFile = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/digi.1.root",
              const string& recoFile = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/reco.1.root",
              const string& geoSetup = "sis100_electron", int nEvents = 1000)
{
  TTree::SetMaxTreeSize(90000000000);

  TString myName = "run_reco";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());

  CbmSetup::Instance()->LoadSetup(geoSetup.c_str());

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTrd, geoTag)) {
    const Char_t* npar[4] = {"asic", "digi", "gas", "gain"};
    TObjString* trdParFile(NULL);
    for (Int_t i(0); i < 4; i++) {
      trdParFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + "." + npar[i] + ".par");
      parFileList->Add(trdParFile);
      std::cout << "-I- " << myName << ": Using parameter file " << trdParFile->GetString() << std::endl;
    }
  }

  // - TOF digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTof, geoTag)) {
    TObjString* tofFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digi.par");
    parFileList->Add(tofFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofFile->GetString() << std::endl;
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;
  }

  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  inputSource->AddFriend(mcFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile.c_str());
  run->SetGenerateRunInfo(kFALSE);

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile.c_str());
  run->AddTask(mcManager);

  // Reconstruction tasks
  TString macroName = srcDir + "/macro/run/modules/reconstruct.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  Bool_t recoSuccess = gROOT->ProcessLine("reconstruct(true,true)");
  if (!recoSuccess) {
    std::cerr << "-E-" << myName << ": error in executing " << macroName << std::endl;
    return;
  }
  std::cout << "-I-" << myName << ": " << macroName << " excuted successfully" << std::endl;

  CbmTrdSetTracksPidLike* trdLI = new CbmTrdSetTracksPidLike("TRDLikelihood", "TRDLikelihood");
  trdLI->SetUseMCInfo(kTRUE);
  trdLI->SetUseMomDependence(kTRUE);
  run->AddTask(trdLI);

  CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
  run->AddTask(matchRecoToMc);

  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.c_str(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }

  std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();

  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();

  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);

  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Reco file is " << recoFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
