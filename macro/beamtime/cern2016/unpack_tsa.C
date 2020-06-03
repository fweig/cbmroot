/** @file FHodoLabSetup
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

//void unpack_tsa(Int_t nEvt=100, TString FileId = "cosmic_2016110701_safe_4links_4")
void unpack_tsa(Int_t nEvt=100, Double_t dDeltaT=200., Int_t iReqDet=0, Bool_t bEpSupp=kFALSE, TString FileId = "sps2016111302_1945")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  //  TString inDir  = "./input/";
  TString inDir  = "./input/" + FileId + "/";
  //  TString inDir  = "/lustre/nyx/cbm/prod/beamtime/2016/11/cern/phase1/";
  //  inFile = inDir + FileId + ".tsa"; 
  // for Phase2 data
  //TString inDir  = "/lustre/nyx/cbm/prod/beamtime/2016/11/cern/phase2/" + FileId;
  //TString inDir  = "/lustre/nyx/cbm/prod/beamtime/2016/11/cern/phase3/" + FileId;
  TString inFile = "*.tsa";

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t bEvt=-1;
  Int_t nEvents = 10;
  if(nEvt>0) { 
   bEvt=0;
   nEvents = nEvt;
  }
  // --- Specify output file name (this is just an example)
  TString outDir = srcDir + "/macro/beamtime/cern2016/data/";
  TString Mode = Form("_DT%d_0x%08x",(Int_t)dDeltaT,iReqDet);
  TString outFile = outDir + FileId + Mode + ".root";
  TString parFile = outDir + FileId + Mode + ".params.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList *parFileList = new TList();
  TString paramDir = "./";
  TString paramFile = paramDir + "FHodoUnpackPar.par";
  TObjString* tutDetDigiFile = new TObjString(paramFile);
  parFileList->Add(tutDetDigiFile);
  
  TString paramFileTof = paramDir + "MapCern2016.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);

  // --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: Initialising..." << std::endl;

  // NXyter Unpacker
  CbmTSUnpackFHodo*   test_unpacker = new CbmTSUnpackFHodo();
  //  test_unpacker->CreateRawMessageOutput(kTRUE);
  
  // Get4 Unpacker
  CbmTSUnpackTof* test_unpacker_tof = new CbmTSUnpackTof(7);  //argument is number of gDpb
  test_unpacker_tof->SetEpochSuppressedMode(bEpSupp);

  // --- Source task
  CbmFlibTestSource* source = new CbmFlibTestSource();
  source->SetMaxDeltaT(dDeltaT);
  source->SetReqDigiAddr(iReqDet); //0x00005006);  // request diamond for output events
  //source->SetFileName(inFile);
  source->AddPath(inDir,inFile);
  source->AddUnpacker(test_unpacker_tof, 0x60, 6);//gDPB A & B
  //source->AddUnpacker(test_unpacker,     0x10, 10);//nDPB A & B = HODO 1 + 2

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  FairRunOnline *run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  //  run->SetEventHeader(event);

  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parOut->open(parFile.Data());
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);
  rtdb->setOutput(parOut);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> unpack_tsa: Starting run..." << std::endl;
  if(bEvt == -1)
  run->Run(bEvt, 0); // run until end of input file
  else 
  run->Run(0, nEvents); // process nEvents
  
  timer.Stop();
  
  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;
    
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> unpack_tsa: Macro finished successfully." << std::endl;
  std::cout << ">>> unpack_tsa: Output file is " << outFile << std::endl;
  std::cout << ">>> unpack_tsa: Real time " << rtime << " s, CPU time "
	    << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
