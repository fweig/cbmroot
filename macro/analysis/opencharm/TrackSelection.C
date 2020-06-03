
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

TString defaultInputFile="";

// Input Parameter
TString input="pau";
TString inputGEV="10gev";
TString system="centr";
TString signal="d0";
Int_t  iVerbose=0;
TString setup="sis100_electron";
bool littrack=false;
Bool_t useMC=kFALSE;

void TrackSelection(Int_t nEvents = 10, Int_t ProcID = 1, bool PileUp = false,Int_t PidTyp = 0 )
{

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

 // ------------------------------------------------------------
 // Monte Carlo file
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
  TString outSystem = Form("data/opencharm.tracks.urqmd.%s.%s.%i.%i.%s.%s.pidMode_%s", input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data(), pidMode.Data());
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

  // ========================================================================
  TStopwatch timer;
  timer.Start();
  gDebug=0;

  FairRunAna *fRun = new FairRunAna();
  fRun->SetInputFile(mcFile);
  fRun->AddFriend(rcFile);
  fRun->SetOutputFile(outFile);
  // ========================================================================

  CbmKF* KF = new CbmKF();                          
  fRun->AddTask(KF);
    CbmL1* l1 = new CbmL1();
  TString mvdMatBudgetFileName = paramDir + mvdMatBudget;
  TString stsMatBudgetFileName = paramDir + stsMatBudget;
  l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  fRun->AddTask(l1);
  // ========================================================================
                                    
                                                         // name, Verbose,  cutP,  cutPt, cutChi2, cutIP )
  CbmD0TrackSelection* dSelect = new CbmD0TrackSelection("Selection", 1,  0.0,   0.0,    3.,  10000.000);
  dSelect->SetNHitsOfLongTracks(4); // suppress short tracks
  dSelect->SetPIDMode(pidMode);     // set PID mode
  dSelect->SetUseMcInfo(kFALSE);    // set usage of MC information for D0
  fRun->AddTask(dSelect);

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data());
  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------

  fRun->Init();
  fRun->Run(0,nEvents);

  timer.Stop();  

  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  printf("\nRealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
  cout << "output file is: " << outFile << endl;

}
