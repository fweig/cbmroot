/** @file run_ana.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Anna Senger [orginator], Florian Uhlig **/

//---------------------------------------------------
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void run_ana(Int_t nEvents = 1000, TString dataSet = "muons", TString setup = "sis100_muon_lmvm", Bool_t useMC = kTRUE,
             TString pluto = "", Double_t ANN = -1)
{
  TString dir      = "";
  TString traFile  = dir + dataSet + ".tra.root";
  TString parFile  = dir + dataSet + ".par.root";
  TString recoFile = dir + dataSet + ".rec.root";
  TString outFile;
  if (ANN < 0) outFile = dataSet + ".ana.root";
  else
    outFile = Form("%s.ana.ANN_%1.2f.root", dataSet.Data(), ANN);

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(recoFile);
  inputSource->AddFriend(traFile);
  run->SetSource(inputSource);

  run->SetOutputFile(outFile);
  // run->SetGenerateRunInfo(kTRUE);

  // -----   Load the geometry setup   -------------------------------------
  // -----   Environment   --------------------------------------------------
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------
  std::cout << std::endl;
  TString setupFile  = srcDir + "/geometry/setup/setup_" + setup + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setup + "()";
  std::cout << "-I- "
            << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  // CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  // See the class documentation of CbmSetup.
  // ————————————————————————————————————


  // ------------------------------------------------------------------------
  CbmKF* kf = new CbmKF();
  run->AddTask(kf);

  /* (VF) Not much sense in running L1 witout STS local reconstruction
  CbmL1* L1 = new CbmL1();
  TString stsGeoTag;
  if(CbmSetup::Instance()->GetGeoTag(kSts, stsGeoTag)) 
  {
    TString parFile = gSystem->Getenv("VMCWORKDIR");
    parFile = parFile + "/parameters/sts/sts_matbudget_" + stsGeoTag + ".root";
    std::cout << "Using material budget file " << parFile << std::endl;
    L1->SetStsMaterialBudgetFileName(parFile.Data());
  }
  run->AddTask(L1);
  */

  CbmAnaDimuonAnalysis* ana = new CbmAnaDimuonAnalysis(pluto, setup);
  /*
  ana->SetChi2MuchCut(3.); 
  ana->SetChi2StsCut(2.);  
  ana->SetChi2VertexCut(3.);
  
  ana->SetNofMuchCut(10);
  ana->SetNofStsCut(7);
  ana->SetNofTrdCut(1);
  ana->SetSigmaTofCut(2);
  */
  ana->UseCuts(kFALSE);
  //  ana->SetAnnCut(ANN, 5); // if SetAnnCut, than UseCuts(kFALSE) !
  ana->UseMC(useMC);
  run->AddTask(ana);

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  //FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  //parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  //rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------

  // -----   Initialize and run   --------------------------------------------
  run->Init();
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  cout << " Test passed" << endl;
  cout << " All ok " << endl;

  //   RemoveGeoManager();
}
