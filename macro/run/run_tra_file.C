/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file run_tra_file.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 3 November 2020
 **/

// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmTransport.h"

#include "FairSystemInfo.h"

#include "TStopwatch.h"
#endif


/** @brief Macro for transport simulation of events from file
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  3 November 2020
 ** @param input      Name of input file
 ** @param nEvents    Number of events to transport
 ** @param output     Name of output data set
 ** @param setup      Name of predefined geometry setup
 ** @param engine     kGeant3 or kGeant4
 **
 ** This macro performs the transport simulation of externally generated
 ** events from an input file. The macro demonstrates the minimal settings
 ** needed to perform the transport: input and output file names,
 ** geometry setup, number of events and the transport engine. These can be
 ** specified through the arguments. Also for the arguments, defaults are
 ** defined, such that the macro should execute from scratch with no arguments.
 **
 ** First argument [input]: input file name. If left empty, a default input
 ** file (urqmd) distributed with cbmroot will be used. If the file name
 ** starts with "pluto", CbmPlutoGenerator will be used, otherwise
 ** the input is assumed to be in unigen format. Other file formats require
 ** intervention into the macro code.
 **
 ** Second argument: [nEvents]: Number of events to be processed.
 **
 ** Third argument [output]: specifies the names of the output files:
 ** - [output].tra.root:  Output MC data (MCPoints and MCTracks)
 ** - [output].par.root:  Parameter file with e.g. the geometry and field in
 ** - [output].geo.root:  Used geometry in TGeoManager format
 ** If left empty, the term "test" will be used.
 **
 ** Fourth argument [setup]: specifies the geometry setup to be used in the
 ** transport run. This has to be chosen from the available predefined
 ** setups (see the folder geometry/setup). By default, the setup
 ** sis100_electron will be used.
 **
 ** Fifth argument [engine]: transport engine. The user can choose between
 ** GEANT3 (default) and GEANT4.
 **
 ** Some other required definitions are coded in the macro body:
 ** the target properties and the beam profile. The event vertex will be
 ** sampled along the intersection of the beam trajectory with the target
 ** volume. A beam trajectory is sampled for each event from the specified
 ** beam profile. The beam angle is zero. Random rotation of the event
 ** plane around the z axis is activated.
 **
 ** All other settings are the default ones in the steering class
 ** CbmTransport. For the options, consult the documentation of that class:
 ** http://computing.gitpages.cbm.gsi.de/cbmroot/classCbmTransport.html
 **/
void run_tra_file(const char* input = "", Int_t nEvents = 1, const char* output = "",
                  const char* setup = "sis100_electron", ECbmEngine engine = kGeant3, int randomSeed = 0)
{

  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "run_tra_file";                 // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   Input file   ---------------------------------------------------
  // Use default input if not specified by the user
  TString defaultInput = srcDir + "/input/urqmd.auau.10gev.centr.root";
  TString inFile       = (strcmp(input, "") == 0 ? defaultInput : TString(input));
  std::cout << "-I- " << myName << ": Using input file " << inFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   Output files   -------------------------------------------------
  TString dataset(output);
  if (dataset.IsNull()) dataset = "test";
  TString outFile = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString geoFile = dataset + ".geo.root";
  std::cout << std::endl;
  // ------------------------------------------------------------------------


  // -----   Target properties   --------------------------------------------
  // For flexibility, the target volume is not part of the predefined
  // geometry setup. It thus has to be specified in this macro. By default,
  // a Gold target of 250 micrometer thickness and 2.5 cm diameter is used.
  // The target shifts by 4 cm upstream by decision of the technical board
  // in Apr 2020 which is 40 cm from center of magnet.
  const char* targetMedium = "Gold";
  Double_t targetThickness = 0.025;  // in cm
  Double_t targetDiameter  = 2.5;    // in cm

  Double_t targetZpos = -40.0;
  // The target position is at z=0 for the old coordinate system but is intended
  // to be moved to -4cm in the DEC21 release. The global coordinate system will
  // also shift from the old target position to the center of the magnet which
  // is a net displacement of -40 cm. In terms of the new coordainte system
  // the target is therefore to be at -44 cm. In order not to cause forgetting
  // we will automate the shifting process for a short time, until the full move
  // has been completed.
  if (strstr(setup, "_APR21")) targetZpos = 0.0;
  if (strstr(setup, "_DEC21")) targetZpos = -44.0;
  std::cout << "Target is at " << targetZpos << "cm from origin" << std::endl;
  // ------------------------------------------------------------------------

  // -----   Beam properties   ----------------------------------------------
  // The beam particles themselves are not subject to the transport,
  // but the properties of the beam determine where the main collision
  // vertex is placed and how the event is rotated. The collision vertex
  // is determined from the intersection of the beam through the target.
  // The beam defined here is along the z axis and its distributions in
  // (x,y) are Gaussian with mean values 0 and sigma 1 mm.
  Double_t beamPosX   = 0.;
  Double_t beamPosY   = 0.;
  Double_t beamSigmaX = 0.1;  // cm
  Double_t beamSigmaY = 0.1;  // cm
  // ------------------------------------------------------------------------


  // -----   Event plane rotation   -----------------------------------------
  // Some generators, e.g., UrQMD, generate the event plane always at
  // zero angle, i.e., the impact parameter is in the x direction. In order
  // get a realistic distribution, the event plane has to be randomly
  // rotated around the z axis. This changes the momentum direction of
  // all primary particles coherently.
  Bool_t rotateEvents = kTRUE;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // --- Transport run   ----------------------------------------------------
  CbmTransport run;
  run.SetEngine(engine);
  if (inFile.Contains("pluto", TString::kIgnoreCase)) { run.AddInput(inFile, kPluto); }
  else
    run.AddInput(inFile, kUnigen);
  run.SetOutFileName(outFile);
  run.SetParFileName(parFile);
  run.SetGeoFileName(geoFile);
  run.LoadSetup(setup);
  run.SetTarget(targetMedium, targetThickness, targetDiameter, 0, 0, targetZpos);
  run.SetBeamPosition(beamPosX, beamPosY, beamSigmaX, beamSigmaY);
  if (rotateEvents) run.SetRandomEventPlane();
  run.SetRandomSeed(randomSeed);
  run.Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl;
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;
  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;
  // ------------------------------------------------------------------------

}  // End of macro
