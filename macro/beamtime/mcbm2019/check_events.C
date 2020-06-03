void check_events(Int_t nEvents = 10, UInt_t uRunId=0, TString inDir="data/", TString friendFile="", TString inFile="") 
{
  Int_t iVerbose = 1;
  Int_t iBugCor=0;
  //Specify log level (INFO, DEBUG, DEBUG1, ...)
  //TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel(logLevel);
  gLogger->SetLogVerbosityLevel("VERYHIGH");
  
  TString workDir    = gSystem->Getenv("VMCWORKDIR");

  TString runId = TString::Format("%u", uRunId);

  TString ParFile    = inDir + "/unp_mcbm_params_" + runId + ".root";

  TString InputFile  = inDir + "/unp_mcbm_" + runId + ".root";
  TString InputFileEvent="";
  if (friendFile.Length() > 0) {
    InputFileEvent  = inDir + friendFile;
  }
   TString OutputFile = inDir + "/test_" + runId + ".out.root";

   TList *parFileList = new TList();

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *run= new FairRunAna();

  FairFileSource* inputSource = new FairFileSource(InputFile);
  if (friendFile.Length() > 0) { 
    inputSource->AddFriend(InputFileEvent);
  }
  run->SetSource(inputSource);
  run->SetOutputFile(OutputFile);

  run->SetEventHeaderPersistence(kFALSE);

  CbmCheckEvents* checker = new CbmCheckEvents();
  run->AddTask(checker);

  // -----  Parameter database   --------------------------------------------
 
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

   //  FairParRootFileIo* parInput1 = new FairParRootFileIo();
   //  parInput1->open(ParFile.Data());
   //  rtdb->setFirstInput(parInput1);

  // -----   Intialise and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  //tofClust->Finish();
  // ------------------------------------------------------------------------
  // default display 
}
