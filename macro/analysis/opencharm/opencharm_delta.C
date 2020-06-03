// -------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input with Pluto signal
//
// P.Sitzmann 08/06/2015
//
// --------------------------------------------------------------------------

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

TString mvdMatBudget="";
TString stsMatBudget="";

TString  fieldMap="";
Double_t fieldZ=0.;
Double_t fieldScale=0.;
Int_t    fieldSymType=0;

TString defaultInputFile="";

// Input Parameter
TString input="nini";
TString inputGEV="15gev";
TString system="centr";
TString signal="d0";
Int_t  iVerbose=0;
TString setup="sis100_electron";
bool littrack=false;
Bool_t useMC=kFALSE;

void opencharm_delta(Int_t nEvents = 100,Int_t ProcID=1)
{
  // ========================================================================
  //          Adjust this part according to your requirements

  // Output file
  TString outFile = Form("data/opencharm.mc.delta..%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());
  // ------------------------------------------------------------------------

  // Parameter file name
  TString parFile = Form("data/paramsunigen.urqmd.%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());
  // ------------------------------------------------------------------------

  TString inDir = gSystem->Getenv("VMCWORKDIR");                                       

  TString setupFile = inDir + "/geometry/setup/" + setup + "_setup.C";
  TString setupFunct = setup;
  setupFunct += "_setup()";
  
  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  gRandom->SetSeed(0);

  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

 
  // -----   Create simulation run   ----------------------------------------
  FairRunSim* fRun = new FairRunSim();
  fRun->SetName("TGeant3");              // Transport engine
  fRun->SetOutputFile(outFile);          // Output file
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  // ------------------------------------------------------------------------


  // -----   Create media   -------------------------------------------------
  fRun->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------


  // -----   Create geometry   ----------------------------------------------
  FairModule* cave= new CbmCave("CAVE");
  cave->SetGeometryFileName(caveGeom);
  fRun->AddModule(cave);

  FairModule* pipe= new CbmPipe("PIPE");
  pipe->SetGeometryFileName(pipeGeom);
  fRun->AddModule(pipe);
  
  FairModule* target= new CbmTarget(79, 0.25);
  fRun->AddModule(target);		

  FairModule* magnet = new CbmMagnet("MAGNET");
  magnet->SetGeometryFileName(magnetGeom);
  fRun->AddModule(magnet);
  
  FairDetector* mvd= new CbmMvd("MVD", kTRUE);
  mvd->SetGeometryFileName(mvdGeom); 
  mvd->SetMotherVolume("pipevac1");
  fRun->AddModule(mvd);

  FairDetector* sts = new CbmStsMC(kTRUE);
  sts->SetGeometryFileName(stsGeom);
  fRun->AddModule(sts);

  if ( richGeom != "" ) {
    FairDetector* rich = new CbmRich("RICH", kTRUE);
    rich->SetGeometryFileName(richGeom);
    fRun->AddModule(rich);
  }

  if ( trdGeom != "" ) {
    FairDetector* trd = new CbmTrd("TRD",kTRUE );
    trd->SetGeometryFileName(trdGeom);
    fRun->AddModule(trd);
  }

  if ( tofGeom != "" ) {
    FairDetector* tof = new CbmTof("TOF", kTRUE);
    tof->SetGeometryFileName(tofGeom);
    fRun->AddModule(tof);
  }
  // ------------------------------------------------------------------------



  // -----   Create magnetic field   ---------------------------------------
  CbmFieldMap* magField = NULL;
  if ( 2 == fieldSymType ) {
      magField = new CbmFieldMapSym2(fieldMap);
  }  else if ( 3 == fieldSymType ) {
   magField = new CbmFieldMapSym3(fieldMap);
  }
  else
    magField = new CbmFieldMapSym1(fieldMap);

  magField->SetPosition(0., 0., fieldZ);
  magField->SetScale(fieldScale);
  fRun->SetField(magField);
  cout << endl << "set mag field" << endl;
  // ------------------------------------------------------------------------

  // -- Nickel delta sim ----------------------------------------------------
 // FairIon *fIon = new FairIon("My_Ni", 58, 59, 79, 15., 58.693);
  FairIon *fIon = new FairIon("My_Au", 79, 197, 79, 25.,183.47324);
  fRun->AddNewIon(fIon);

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen   = new FairPrimaryGenerator();
 // FairIonGenerator*     fIongen   = new FairIonGenerator(58, 59, 58,1, 0.,0., 15, 0.,0.,-1.);   // Nickel
  FairIonGenerator*     fIongen   = new FairIonGenerator(79, 197,79,1, 0.,0., 25, 0.,0.,-1.); // Gold
  primGen->AddGenerator(fIongen);
  fRun->SetGenerator(primGen);
  // ------------------------------------------------------------------------
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  fRun->Init();
  // ------------------------------------------------------------------------

  // -----   Runtime database   ---------------------------------------------
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(fRun->GetRunId(),1);
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------

 
  // -----   Start run   ----------------------------------------------------
  fRun->Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime 
       << "s" << endl << endl;
  // ------------------------------------------------------------------------

}

