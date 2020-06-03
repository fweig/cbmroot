
TString caveGeom="";
TString pipeGeom="";
TString magnetGeom="";
TString mvdGeom="";
TString stsGeom="";
TString richGeom="";
TString muchGeom="";
TString shieldGeom="";
TString trdGeom="";
TString tofGeom="";
TString ecalGeom="";
TString platformGeom="";
TString psdGeom="";
Double_t psdZpos=0.;
Double_t psdXpos=0.;

TString mvdTag="";
TString stsTag="";
TString trdTag="";
TString tofTag="";  

TString stsDigi="";
TString trdDigi="";
TString tofDigi="";
TString muchDigi="";

TString mvdMatBudget="";
TString stsMatBudget="";

TString  fieldMap="";
Double_t fieldZ=0.;
Double_t fieldScale=0.;
Int_t    fieldSymType=0;


// Input Parameter
TString input="pau";
TString inputGEV="10gev";
TString system="centr";
TString signal="d0";
Int_t  iVerbose=0;
TString setup="sis100_electron";
bool littrack=false;
Bool_t useMC=kFALSE;


void PairSelection(Int_t nEvents = 10, Int_t ProcID = 1, bool PileUp = false, Int_t PidTyp = 0)
{
// -------------------------------------------------------------------------


switch (PidTyp)
{
case 0:
    TString pidMode = "NONE";
    break;
case 1:
    TString pidMode = "MC";
    break;
case 2:
    TSTring pidMode = "TOF";
    break;
default:
    TString pidMode = "NONE";

}

// Input file (MC events)
 TString mcFile = Form("data/opencharm.mc.urqmd.%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());



  // Parameter file
  TString parFile = Form("data/paramsunigen.urqmd.%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());

    // Reco file
    TString rcSystem = Form("data/opencharm.reco.urqmd.%s.%s.%i.%i.%s.%s", input.Data(), inputGEV.Data(), nEvents, ProcID,  signal.Data(), setup.Data());
    if(!PileUp)
      {
      if(littrack)
         TString rcFile = rcSystem + ".littrack.root";
      else 
         TString rcFile = rcSystem + ".l1.root";
      }
      else if(littrack)
         TString rcFile = rcSystem + ".PileUp.littrack.root";
      else 
         TString rcFile = rcSystem + ".PileUp.l1.root";

  // Output file
  TString outSystem = Form("data/opencharm.pairs.urqmd.%s.%s.%i.%i.%s.%s.pidMode_%s", input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data(), pidMode.Data());
  if(useMC) outSystem += ".mcMode";
  if(!PileUp)
    {
    if(littrack)
       TString outFile = outSystem + ".littrack.root";
    else 
       TString outFile = outSystem + ".l1.root";
    }
    else if(littrack)
       TString outFile = outSystem + ".PileUp.littrack.root";
    else 
       TString outFile = outSystem + ".PileUp.l1.root";
    

    // Track file
  TString trackSystem = Form("data/opencharm.tracks.urqmd.%s.%s.%i.%i.%s.%s.pidMode_%s", input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data(), pidMode.Data());
  if(!PileUp)
    {
    if(littrack)
       TString trackFile = trackSystem + ".littrack.root";
    else 
       TString trackFile = trackSystem + ".l1.root";
    }
    else if(littrack)
       TString trackFile = trackSystem + ".PileUp.littrack.root";
    else 
       TString trackFile = trackSystem + ".PileUp.l1.root";

    cout << endl
	<< "  mcFile  :   " << mcFile  << endl
	<< "  rcFile  :   " << rcFile  << endl
        << "  outFile    :   " << outFile << endl;

// -------------------------------------------------------------------------	
//  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly 
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList *parFileList = new TList();

  TString inDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = inDir + "/parameters/";

  TString setupFile = inDir + "/geometry/setup/" + setup + "_setup.C";
  TString setupFunct = setup;
  setupFunct += "_setup()";

  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  TObjString stsDigiFile = paramDir + stsDigi;
  parFileList->Add(&stsDigiFile);
  cout << "macro/run/run_reco.C using: " << stsDigi << endl;
  // -------------------------------------------------------------------------

// -------------------------------------------------------------------------
    TStopwatch timer;
    timer.Start();

    gDebug=0;
    // -------------------------------------------------------------------------

// ------------------------------------------------------------------------

    FairRunAna *fRun = new FairRunAna();
    fRun->SetInputFile(mcFile);
    fRun->AddFriend(rcFile);
    fRun->AddFriend(trackFile);
    fRun->SetOutputFile(outFile);
// -------------------------------------------------------------------------

    CbmKF* KF = new CbmKF();
    fRun->AddTask(KF);

    CbmL1* l1 = new CbmL1();
    fRun->AddTask(l1);                                                 // name     verbose  ipD0   SvZ
    
    CbmD0CandidateSelection* D0selection  = new CbmD0CandidateSelection("d0selection", 1, 0.04,  0.01 );
    D0selection->SetTestMode(useMC);

    fRun->AddTask(D0selection);
    
  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------
    
// -------------------------------------------------------------------------
    fRun->Init();
    fRun->Run(0,nEvents);
// -------------------------------------------------------------------------

    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    printf("\nRealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
    cout << "output file is : " << outFile << endl;

}
