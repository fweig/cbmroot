/* Copyright (C) 2015-2020 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: T. Balog, Philipp Klaus [committer] */

// --------------------------------------------------------------------------
//
// Macro for transport simulation with the MVD
// to determine the material budget of the MVD stations.
// ROOTinos parallel to the z axis will be transported,
// using the RadLenRegister functionality of FairRunSim.
// From the output MC file, the material budget can be
// determined with the macro matbudget_ana.C
//
// Philipp Klaus, 30.04.2020
//
// derived from version created for STS by
// T. Balog, 11.05.2015
//
// --------------------------------------------------------------------------

// void matbudget_mc(Int_t nEvents = 10      , const char* mvdGeo = "v17a_tr")
// void matbudget_mc(Int_t nEvents = 10000 , const char* mvdGeo = "v17a_tr")
void matbudget_mc(Int_t nEvents = 10000000, const char* mvdGeo = "v17a_tr")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // ----- Paths and file names  --------------------------------------------
  TString mvdVersion(mvdGeo);
  TString inDir   = gSystem->Getenv("VMCWORKDIR");
  TString inFile  = inDir + "/input/urqmd.ftn14";
  TString outDir  = "data";
  TString outFile = outDir + "/matbudget." + mvdVersion + ".mc.root";
  TString parFile = outDir + "/matbudget." + mvdVersion + ".params.root";

  // -----  Geometries  -----------------------------------------------------
  TString caveGeom   = "cave.geo";
  TString targetGeom = "";
  TString pipeGeom   = "";
  TString magnetGeom = "";
  TString mvdGeom    = "mvd/mvd_" + mvdVersion + ".geo.root";
  TString stsGeom    = "";
  TString richGeom   = "";
  TString trdGeom    = "";
  TString tofGeom    = "";

  // In general, the following parts need not be touched
  // ========================================================================

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "ERROR";  // "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");      // Transport engine
  run->SetOutputFile(outFile);  // Output file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // ------------------------------------------------------------------------

  // -----   Logger settings   ----------------------------------------------
  gLogger->SetLogScreenLevel(logLevel.Data());
  gLogger->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------

  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");  // Materials
  // ------------------------------------------------------------------------

  // -----   Create detectors and passive volumes   -------------------------
  if (caveGeom != "") {
    FairModule* cave = new CbmCave("CAVE");
    cave->SetGeometryFileName(caveGeom);
    run->AddModule(cave);
  }

  if (pipeGeom != "") {
    FairModule* pipe = new CbmPipe("PIPE");
    pipe->SetGeometryFileName(pipeGeom);
    run->AddModule(pipe);
  }

  if (targetGeom != "") {
    FairModule* target = new CbmTarget("Target");
    target->SetGeometryFileName(targetGeom);
    run->AddModule(target);
  }

  if (magnetGeom != "") {
    FairModule* magnet = new CbmMagnet("MAGNET");
    magnet->SetGeometryFileName(magnetGeom);
    run->AddModule(magnet);
  }

  if (mvdGeom != "") {
    FairDetector* mvd = new CbmMvd("MVD", kTRUE);
    mvd->SetGeometryFileName(mvdGeom);
    run->AddModule(mvd);
  }

  if (stsGeom != "") {
    FairDetector* sts = new CbmStsMC(kTRUE);
    sts->SetGeometryFileName(stsGeom);
    run->AddModule(sts);
  }

  if (richGeom != "") {
    FairDetector* rich = new CbmRich("RICH", kTRUE);
    rich->SetGeometryFileName(richGeom);
    run->AddModule(rich);
  }

  if (trdGeom != "") {
    FairDetector* trd = new CbmTrd("TRD", kTRUE);
    trd->SetGeometryFileName(trdGeom);
    run->AddModule(trd);
  }

  if (tofGeom != "") {
    FairDetector* tof = new CbmTof("TOF", kTRUE);
    tof->SetGeometryFileName(tofGeom);
    run->AddModule(tof);
  }

  // ------------------------------------------------------------------------

  // -----   Create magnetic field   ----------------------------------------
  // Zero field
  CbmFieldConst* magField = new CbmFieldConst();
  magField->SetField(0, 0, 0);  // values are in kG
  magField->SetFieldRegion(-74, -39, -22, 74, 39, 160);
  run->SetField(magField);

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  run->SetGenerator(primGen);

  // --- Primary particles
  // Generated are ROOTinos (PDG=0) in z direction (one per event),
  // starting at z = 0.
  // The starting points in x and y are chosen such as to illuminate the
  // detector.
  Int_t pdgId              = 0;  // ROOTinos
  Int_t multiplicity       = 1;  // particles per event
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, multiplicity);

  boxGen->SetBoxXYZ(-20., -20., 20., 20., -4.);
  boxGen->SetPRange(0.1, 0.5);
  boxGen->SetThetaRange(0., 0.);
  boxGen->SetPhiRange(0., 360.);

  primGen->AddGenerator(boxGen);

  run->SetRadLenRegister(kTRUE);
  // ------------------------------------------------------------------------

  // -----   Run initialisation   -------------------------------------------
  run->Init();
  // ------------------------------------------------------------------------

  // -----   Runtime database   ---------------------------------------------
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(run->GetRunId(), 1);
  Bool_t kParameterMerged   = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------

  // -----   Start run   ----------------------------------------------------
  run->Run(nEvents);
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

  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
