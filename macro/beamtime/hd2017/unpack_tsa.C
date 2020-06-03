/** @file FHodoLabSetup
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

//void unpack_tsa(Int_t nEvt=100, TString FileId = "cosmic_2016110701_safe_4links_4")
void unpack_tsa(Int_t nEvt=10000, Double_t dDeltaT=50., Int_t iReqDet=2, Bool_t bEpSupp=kTRUE, TString FileId = "r0031_20170602_1343")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  //  TString inDir  = "./input/";
  //TString inDir  = "./input/" + FileId + "/";
  TString inDir  = "./input1/" + FileId + "/";
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
  TString outDir = srcDir + "/macro/beamtime/hd2017/data/";
  TString Mode = Form("_DT%d_Req%d",(Int_t)dDeltaT,iReqDet);
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
  /*
  TString paramFile = paramDir + "FHodoUnpackPar.par";
  TObjString* tutDetDigiFile = new TObjString(paramFile);
  parFileList->Add(tutDetDigiFile);
  */
  TString paramFileTof = paramDir + "MapTofHdCosmics2017.par";
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
  CbmTSUnpackTof* test_unpacker_tof = new CbmTSUnpackTof(4);  //argument is number of gDpb
  test_unpacker_tof->SetEpochSuppressedMode(bEpSupp);

  // --- Source task
  CbmFlibTestSource* source = new CbmFlibTestSource();
  source->SetMaxDeltaT(dDeltaT);
  switch(iReqDet){
    case 0:                               // no request 
      break;
    case 1:
      source->SetReqDigiAddr(0x00019006);  // request USTC 1
      break;
    case 2:
      source->AddReqDigiAddr(0x00019006);  // request USTC 1 for output events
      source->AddReqDigiAddr(0x00019026);  // request THU  1 for output events
      break;
    case 3:
      source->AddReqDigiAddr(0x00006016);  // request USTC 0 for output events
      source->AddReqDigiAddr(0x00016016);  // request USTC 1 for output events
      break;
    case 4:
      source->AddReqDigiAddr(0x00006006);  // request BUC 0 for output events
      source->AddReqDigiAddr(0x00016006);  // request BUC 1 for output events
      break;
    case 5:
      source->AddReqDigiAddr(0x00006006);  // request BUC SS 0 for output events
      source->AddReqDigiAddr(0x00019006);  // request USTC SS 1 for output events
      break;
    default:
      std::cout<<"Request mode not implemented"<<std::endl;
      return;
  }
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
