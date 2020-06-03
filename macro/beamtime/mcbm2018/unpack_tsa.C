/** @file FHodoLabSetup
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

//void unpack_tsa(Int_t nEvt=100, TString FileId = "cosmic_2016110701_safe_4links_4")
void unpack_tsa(Int_t nEvt=100000, Double_t dDeltaT=100., Int_t iReqDet=0,
TString cGeo="v18j_cosmicHD", TString FileId = "r0088_20180905_1602")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  //  TString inDir  = "./input/";
  TString inDir  = "./input/" + FileId + "/";
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
  TString outDir = srcDir + "/macro/beamtime/mcbm2018/data/";
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
  TString paramFileTof = paramDir + "MapTofGbtx.par";
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
  //CbmTSUnpackTof* test_unpacker_tof = new CbmTSUnpackTof(3);  //argument is number of gDpb
  //test_unpacker_tof->SetEpochSuppressedMode(bEpSupp);
  //CbmUnpackTofStar2018* unpacker_tof = new CbmUnpackTofStar2018(2);  //argument is number of gDpb
  CbmMcbm2018TofUnpacker* unpacker_tof = new CbmMcbm2018TofUnpacker();  

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
      source->AddReqDigiAddr(0x00009016);  // request USTC DS1 for output events
      source->AddReqDigiAddr(0x00019016);  // request USTC DS" for output events
      break;
    case 4:
      source->AddReqDigiAddr(0x00006006);  // request BUC 0 for output events
      source->AddReqDigiAddr(0x00016006);  // request BUC 1 for output events
      break;
    case 9:
      source->AddReqDigiAddr(0x00009006);  // request USTC 1 for output events
      source->AddReqDigiAddr(0x00019006);  // request USTC 1 for output events
      source->AddReqDigiAddr(0x00009016);  // request USTC 1 for output events
      source->AddReqDigiAddr(0x00019016);  // request USTC 1 for output events
      source->AddReqDigiAddr(0x00009026);  // request USTC 1 for output events
      source->AddReqDigiAddr(0x00019026);  // request USTC 1 for output events
      source->SetReqMode(1);  // request any of the detectors in the list  
      break;
    default:
      if(iReqDet<0) {
	source->AddReqDigiAddr(0x00009006);  //
	source->AddReqDigiAddr(0x00019006);  // 
	source->AddReqDigiAddr(0x00009016);  //
	source->AddReqDigiAddr(0x00019016);  //
	source->AddReqDigiAddr(0x00009026);  // 
	source->AddReqDigiAddr(0x00019026);  //

	source->AddReqDigiAddr(0x00001006);  //
	source->AddReqDigiAddr(0x00011006);  // 
	source->AddReqDigiAddr(0x00021006);  // 
	source->AddReqDigiAddr(0x00001016);  //
	source->AddReqDigiAddr(0x00011016);  //
	source->AddReqDigiAddr(0x00021016);  // 
	source->AddReqDigiAddr(0x00001026);  // 
	source->AddReqDigiAddr(0x00011026);  //
	source->AddReqDigiAddr(0x00021026);  // 
	source->SetReqMode(-iReqDet);      // ask for detector multiplicity 
	break;
      }
      std::cout<<"Request mode not implemented"<<std::endl;
      return;
  }
  //source->SetFileName(inFile);
  source->AddPath(inDir,inFile);
  source->AddUnpacker(unpacker_tof, 0x60, 6);//gDPB A & B
  //source->AddUnpacker(test_unpacker,     0x10, 10);//nDPB A & B = HODO 1 + 2

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  FairRunOnline *run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  //  run->SetEventHeader(event);


  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
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
