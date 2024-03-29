/* Copyright (C) 2014-2015 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input with CbmUniGen
// and GEANT3 CBM setup with MVD only
//
// P.Sitzmann  04/09/2014
//
// --------------------------------------------------------------------------

TString caveGeom     = "";
TString pipeGeom     = "";
TString magnetGeom   = "";
TString mvdGeom      = "";
TString stsGeom      = "";
TString richGeom     = "";
TString muchGeom     = "";
TString shieldGeom   = "";
TString trdGeom      = "";
TString tofGeom      = "";
TString ecalGeom     = "";
TString platformGeom = "";
TString psdGeom      = "";
Double_t psdZpos     = 0.;
Double_t psdXpos     = 0.;

TString mvdTag = "";
TString stsTag = "";
TString trdTag = "";
TString tofTag = "";

TString stsDigi = "";
TString trdDigi = "";
TString tofDigi = "";

TString mvdMatBudget = "";
TString stsMatBudget = "";

TString fieldMap    = "";
Double_t fieldZ     = 0.;
Double_t fieldScale = 0.;
Int_t fieldSymType  = 0;

void mvd_CbmUniGen_sim_cluster(const char* input = "auau.25gev", const char* system = "centr", Int_t nEvents = 1,
                               const char* setup = "sis100_electron")
{
  // ========================================================================
  //          Adjust this part according to your requirements

  // Input file
  TString inDir  = gSystem->Getenv("VMCWORKDIR");
  TString inFile = inDir + "/input/urqmd.";
  inFile         = inFile + input;
  inFile         = inFile + ".";
  inFile         = inFile + system;
  inFile         = inFile + ".root";

  // Output file
  TString outFile = "data/mvd.mc.unigen.";
  outFile += input;
  outFile += ".";
  outFile += system;
  outFile += ".root";

  // Parameter file name
  TString parFile = "data/paramsunigen.";
  parFile         = parFile + input;
  parFile         = parFile + ".";
  parFile         = parFile + system;
  parFile         = parFile + ".root";

  // Cave geometry
  TString caveGeom = "cave.geo";

  // Beam pipe geometry
  TString pipeGeom = "pipe/pipe_v14l.root";

  // Magnet geometry and field map
  TString magnetGeom = "magnet/magnet_v12b.geo.root";
  TString fieldMap   = "field_v12b";  // name of field map
  Int_t fieldZ       = 40.;           // field centre z position
  Int_t fieldScale   = 1.;            // field scaling factor
  Int_t fieldSymType = 3;

  // MVD geometry
  TString mvdGeom = "mvd/mvd_v15a.geo.root";

  // StS geometry
  TString stsGeom = "sts/sts_v13d.geo.root";

  // Tof geometry
  // TString tofGeom = "tof/tof_v13b.geo.root";

  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  TString inDir = gSystem->Getenv("VMCWORKDIR");

  TString setupFile  = inDir + "/geometry/setup/" + setup + "_setup.C";
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


  // -----   Create simulation run   ----------------------------------------
  FairRunSim* fRun = new FairRunSim();
  fRun->SetName("TGeant3");      // Transport engine
  fRun->SetOutputFile(outFile);  // Output file
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  // ------------------------------------------------------------------------


  // -----   Create media   -------------------------------------------------
  fRun->SetMaterials("media.geo");  // Materials
  // ------------------------------------------------------------------------


  // -----   Create geometry   ----------------------------------------------
  FairModule* cave = new CbmCave("CAVE");
  cave->SetGeometryFileName(caveGeom);
  fRun->AddModule(cave);

  FairModule* pipe = new CbmPipe("PIPE");
  pipe->SetGeometryFileName(pipeGeom);
  fRun->AddModule(pipe);

  FairModule* target = new CbmTarget(79, 0.25);
  fRun->AddModule(target);

  FairModule* magnet = new CbmMagnet("MAGNET");
  magnet->SetGeometryFileName(magnetGeom);
  fRun->AddModule(magnet);

  FairDetector* mvd = new CbmMvd("MVD", kTRUE);
  mvd->SetGeometryFileName(mvdGeom);
  mvd->SetMotherVolume("pipevac1");
  fRun->AddModule(mvd);

  FairDetector* sts = new CbmStsMC(kTRUE);
  sts->SetGeometryFileName(stsGeom);
  fRun->AddModule(sts);

  // FairDetector* tof = new CbmTof("TOF", kTRUE);
  // tof->SetGeometryFileName(tofGeom);
  // fRun->AddModule(tof);
  // ------------------------------------------------------------------------


  // -----   Create magnetic field   ---------------------------------------
  if (2 == fieldSymType) { CbmFieldMap* magField = new CbmFieldMapSym2(fieldMap); }
  else if (3 == fieldSymType) {
    CbmFieldMap* magField = new CbmFieldMapSym3(fieldMap);
  }
  magField->SetPosition(0., 0., fieldZ);
  magField->SetScale(fieldScale);
  fRun->SetField(magField);
  // ------------------------------------------------------------------------


  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  CbmUnigenGenerator* uniGen    = new CbmUnigenGenerator(inFile);
  primGen->AddGenerator(uniGen);
  fRun->SetGenerator(primGen);
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  fRun->Init();
  // ------------------------------------------------------------------------


  // -----   Runtime database   ---------------------------------------------
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(fRun->GetRunId(), 1);
  Bool_t kParameterMerged   = kTRUE;
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
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << endl << endl;
  // ------------------------------------------------------------------------
}
