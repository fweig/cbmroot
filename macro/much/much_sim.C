/* Copyright (C) 2020 Variable Energy Cyclotron Centre, Kolkata
   SPDX-License-Identifier: GPL-3.0-only
   Authors: M.Ryzhinskiy m.ryzhinskiy@gsi.de, Vikas Singhal [committer] */

/**
 * Performs standard transport simulation with GEANT3
 * in the CBM MuCh setup: STS + MUCH.
 *
 * @author M.Ryzhinskiy m.ryzhinskiy@gsi.de
 * @param inputSignal    Input file containing signal
 * @param inputBgr       Input file containing background
 * @param outFile        Output file for transport data
 * @param nEvents        Number of events to process
 */

#include <iostream>
using namespace std;

void much_sim(TString inputSignal = "", TString inputBgr = "", TString outFile = "", Int_t nEvents = 3)
{
  // ========================================================================
  //          Adjust this part according to your requirements

  TString inputdir = gSystem->Getenv("VMCWORKDIR");
  if (inputSignal == "") { inputSignal = inputdir + "/macro/much/data/jpsi.root"; }
  if (inputBgr == "") {
    inputBgr = inputdir + "/input/urqmd.auau.10gev.centr.root";
    //  inputBgr = inputdir + "/input/urqmd.auau.8gev.centr.01000.root";
    //  inputBgr = inputdir + "/input/urqmd.auau.25gev.centr.root";
  }
  if (outFile == "") {
    outFile = "data/mc.root";
    //    outFile = "data/mc_parallelopiped.root";
  }
  TString parFile = "data/params.root";
  //  TString parFile = "data/params_parallelopiped.root";
  TString geoFile = "data/much_geofile_full.root";
  //  TString geoFile = "data/parallelopiped_geofile_full.root";

  // -----   Confirm input parameters    ------------------------------------
  cout << endl;
  cout << "========  CBMROOT Macro much_sim  =================" << endl;
  cout << "First input file  is " << inputSignal << endl;
  cout << "Second input file is " << inputBgr << endl;
  cout << "Output file       is " << outFile << endl;
  cout << "Events to process:   " << nEvents << endl;
  cout << "===================================================" << endl;
  cout << endl;

  // -----   Specify MUCH related geometry   --------------------------------
  // Use pipe_much.geo for the beam pipe in both cases.
  // In case you want the additional W shielding around the pipe,
  // use shield_standard.geo or shield_compact.geo, respective to the
  // MUCH geometry. Otherwise, define an empty string.

  // LMVM setup using Rootified Geometry

  TString muchGeom = "much/much_v17b_sis100_1m_lmvm.geo.root";
  //  TString muchGeom   = "much/much_v17b.geo.root";
  TString pipeGeom   = "pipe/pipe_much_v15b_125cm_no.geo";
  TString shieldGeom = "";

  // LMVM setup without Rootified.
  //  TString muchGeom   = "much/much_v15b_STS100-B_125cm_no.geo";
  //  TString pipeGeom   = "pipe/pipe_much_v15b_125cm_no.geo";
  //  TString shieldGeom = "much/shield_v15b_SIS100B_149_3part_125cm.geo";

  // JPSI setup
  //  TString muchGeom   = "much/much_v15c_SIS100-C_gemtrd.geo";
  //  TString pipeGeom   = "pipe/pipe_much_v15b_125cm_no.geo";
  //  TString shieldGeom = "much/shield_v15c_SIS100C_149_3part_125cm.geo";

  // -----   Other geometries   ---------------------------------------------
  TString caveGeom = "cave.geo";
  //  CbmTarget* target = new CbmTarget("Gold", 0.025);
  TString magnetGeom = "magnet/magnet_v15b_much.geo.root";
  TString mvdGeom    = "";  // "mvd/mvd_v15a.geo.root";
  TString stsGeom    = "sts/sts_v15a.geo.root";

  // --- Define the target geometry -----------------------------------------
  //
  // The target is not part of the setup, since one and the same setup can
  // and will be used with different targets.
  // The target is constructed as a tube in z direction with the specified
  // diameter (in x and y) and thickness (in z). It will be placed at the
  // specified position as daughter volume of the volume present there. It is
  // in the responsibility of the user that no overlaps or extrusions are
  // created by the placement of the target.
  //
  TString targetElement    = "Gold";
  Double_t targetThickness = 0.025;  // full thickness in cm
  Double_t targetDiameter  = 2.5;    // diameter in cm
  Double_t targetPosX      = 0.;     // target x position in global c.s. [cm]
  Double_t targetPosY      = 0.;     // target y position in global c.s. [cm]
  Double_t targetPosZ      = 0.;     // target z position in global c.s. [cm]
  Double_t targetRotY      = 0.;     // target rotation angle around the y axis [deg]
  // ------------------------------------------------------------------------


  // -----   Magnetic field   -----------------------------------------------
  TString fieldMap    = "field_v18a";  // name of field map
  Double_t fieldZ     = 40.;           // field center z position
  Double_t fieldScale = 1.;            // field scaling factor

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Set unique random generator seed   -----------------------------
  // Comment this out if you want to have a defined seed for reproducibility.
  gRandom->SetSeed(1);
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  // -----   Create simulation run   ----------------------------------------
  cout << endl << "=== much_sim.C : Creating run and database ..." << endl;
  FairRunSim* fRun = new FairRunSim();
  fRun->SetName("TGeant3");         // Transport engine
  fRun->SetOutputFile(outFile);     // Output file
  fRun->SetGenerateRunInfo(kTRUE);  // Create FairRunInfo file
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  // ------------------------------------------------------------------------

  // -----   Create media   -------------------------------------------------
  cout << endl << "=== much_sim.C : Set materials ..." << endl;
  fRun->SetMaterials("media.geo");  // Materials
  // ------------------------------------------------------------------------

  // -----   Create detectors and passive volumes   -------------------------
  cout << endl << "=== much_sim.C : Create geometry ..." << endl;
  if (caveGeom != "") {
    FairModule* cave = new CbmCave("CAVE");
    cave->SetGeometryFileName(caveGeom);
    fRun->AddModule(cave);
    cout << "    --- " << caveGeom << endl;
  }

  if (pipeGeom != "") {
    FairModule* pipe = new CbmPipe("PIPE");
    pipe->SetGeometryFileName(pipeGeom);
    cout << "    --- " << pipeGeom << endl;
    fRun->AddModule(pipe);
  }

  // --- Target
  CbmTarget* target = new CbmTarget(targetElement.Data(), targetThickness, targetDiameter);
  target->SetPosition(targetPosX, targetPosY, targetPosZ);
  target->SetRotation(targetRotY);
  fRun->AddModule(target);

  if (magnetGeom != "") {
    FairModule* magnet = new CbmMagnet("MAGNET");
    magnet->SetGeometryFileName(magnetGeom);
    cout << "    --- " << magnetGeom << endl;
    fRun->AddModule(magnet);
  }

  if (mvdGeom != "") {
    FairDetector* mvd = new CbmMvd("MVD", kTRUE);
    mvd->SetGeometryFileName(mvdGeom);
    mvd->SetMotherVolume("pipevac1");
    fRun->AddModule(mvd);
  }

  if (stsGeom != "") {
    FairDetector* sts = new CbmStsMC(kTRUE);
    sts->SetGeometryFileName(stsGeom);
    cout << "    --- " << stsGeom << endl;
    fRun->AddModule(sts);
  }

  if (muchGeom != "") {
    FairDetector* much = new CbmMuch("MUCH", kTRUE);
    much->SetGeometryFileName(muchGeom);
    cout << "    --- " << muchGeom << endl;
    fRun->AddModule(much);
  }

  if (shieldGeom != "") {
    FairModule* shield = new CbmShield("SHIELD");
    shield->SetGeometryFileName(shieldGeom);
    cout << "    --- " << shieldGeom << endl;
    fRun->AddModule(shield);
  }
  // ------------------------------------------------------------------------

  // -----   Create magnetic field   ----------------------------------------
  CbmFieldMap* magField = new CbmFieldMapSym3(fieldMap);
  magField->SetPosition(0., 0., fieldZ);
  magField->SetScale(fieldScale);
  fRun->SetField(magField);
  // ------------------------------------------------------------------------

  // -----   Create PrimaryGenerator   --------------------------------------
  cout << endl << "=== much_sim.C : Create generators ..." << endl;
  //  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  if (inputSignal != "") {
    CbmPlutoGenerator* plutoGen = new CbmPlutoGenerator(inputSignal);
    primGen->AddGenerator(plutoGen);
  }
  if (inputBgr != "") {
    CbmUnigenGenerator* uniGen = new CbmUnigenGenerator(inputBgr);
    primGen->AddGenerator(uniGen);
  }
  fRun->SetGenerator(primGen);
  // ------------------------------------------------------------------------


  // -----   Run initialization   -------------------------------------------
  cout << endl << "=== much_sim.C : Initialize run ..." << endl;
  fRun->Init();
  // ------------------------------------------------------------------------


  // -----   Runtime database   ---------------------------------------------
  cout << endl << "=== much_sim.C : Set up database ..." << endl;
  cout << "                Parameters will be saved to output file" << endl;
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(fRun->GetRunId(), 1);
  Bool_t kParameterMerged   = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile);
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  cout << endl << "=== much_sim.C : Starting timer ..." << endl;
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Start run   ----------------------------------------------------
  cout << endl << "=== much_sim.C : Start run ..." << endl;
  fRun->Run(nEvents);
  // Write geometry
  fRun->CreateGeometryFile(geoFile);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "=== much_sim.C : Macro finished successfully." << endl;
  cout << "=== much_sim.C : Output file is " << outFile << endl;
  cout << "=== much_sim.C : Real time used: " << rtime << "s " << endl;
  cout << "=== much_sim.C : CPU time used : " << ctime << "s " << endl;
  cout << endl << endl;
  // ------------------------------------------------------------------------

  cout << " Test passed" << endl;
  cout << " All ok " << endl;

  RemoveGeoManager();
}
