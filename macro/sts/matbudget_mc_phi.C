/* Copyright (C) 2015-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Thomas Balog, David Emschermann [committer] */

// --------------------------------------------------------------------------
//
// Macro for transport simulation with the STS
// to determine the material budget of the STS stations.
// ROOTinos parallel to the z axis will be transported,
// using the RadLenRegister functionality of FairRunSim.
// From the output MC file, the material budget can be
// determined with the macro matbudget_ana.C
//
// T. Balog, 11.05.2015
//
// --------------------------------------------------------------------------

#include "../../sim/transport/gconfig/g3Config.C"

//void matbudget_mc_phi(Int_t nEvents = 10      , const char* stsGeo = "v15c")
//void matbudget_mc_phi(Int_t nEvents = 1000000 , const char* stsGeo = "v15c")
void matbudget_mc_phi(Int_t nEvents = 1000000, const char* stsGeo = "v21e")
{
  // ========================================================================
  //          Adjust this part according to your requirements

  // ----- Paths and file names  --------------------------------------------
  TString stsVersion(stsGeo);
  TString inDir   = gSystem->Getenv("VMCWORKDIR");
  TString inFile  = inDir + "/input/urqmd.ftn14";
  TString outDir  = "data";
  TString outFile = outDir + "/matbudget." + stsVersion + ".mc.root";
  TString parFile = outDir + "/matbudget." + stsVersion + ".params.root";

  // -----  Geometries  -----------------------------------------------------
  TString caveGeom   = "cave.geo";
  TString targetGeom = "";
  TString pipeGeom   = "";
  TString magnetGeom = "";
  TString mvdGeom    = "";
  TString stsGeom    = "sts/sts_" + stsVersion + ".geo.root";
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
  // The starting points in x and y are chosen such as to illuminate the STS.
  Int_t pdgId              = 0;  // ROOTinos
  Int_t multiplicity       = 1;  // particles per event
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, multiplicity);

  boxGen->SetXYZ(0., 0., -44.0);  // The target is -44.0 cm from centre of the magnet.
  boxGen->SetPRange(0.1, 0.5);
  boxGen->SetThetaRange(0., 40.);
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
