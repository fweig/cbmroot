/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file run_tra_beam.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 3 November 2020
 **/

// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmBeamGenerator.h";
#include "CbmTransport.h"

#include "FairSystemInfo.h"

#include "TStopwatch.h"
#endif

/** @brief Macro for transport simulation of beam particles through CBM
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  3 November 2020
 ** @param nEvents    Number of beam particles to simulate (one per event)
 ** @param species    Beam ion species, e.g. "C" or "Au"
 ** @param beamP      Beam momentum per nucleon [GeV]
 ** @param beamStartZ Start z coordinate of beam
 ** @param output     Name of output data set
 ** @param setup      Name of predefined geometry setup
 ** @param engine     kGeant3 or kGeant4
 **
 ** This macro performs the transport simulation of beam particles
 ** through the CBM setup using the CbmBeamGenerator class. The macro
 ** demonstrates the minimal settings needed to perform this transport:
 ** number of events, beam species, beam momentum, start position of the beam,
 ** output file names, geometry setup and the transport engine. These can be
 ** specified through the arguments. Also for the arguments, defaults are
 ** defined, such that the macro should execute from scratch with no arguments.
 **
 ** The following output files are generated:
 ** - [output].tra.root:  Output MC data (MCPoints and MCTracks)
 ** - [output].par.root:  Parameter file with e.g. the geometry and field in
 ** - [output].geo.root:  Used geometry in TGeoManager format
 **
 ** [setup]: specifies the geometry setup to be used in the
 ** transport run. This has to be chosen from the available predefined
 ** setups (see the folder geometry/setup). By default, the setup
 ** sis100_electron will be used.
 **
 ** [engine]: transport engine. The user can choose between GEANT3 (default)
 ** and GEANT4.
 **
 ** Some other required definitions are coded in the macro body:
 ** the target properties and the beam profile. A beam trajectory is sampled
 ** for each event from the specified beam profile.
 **
 ** All other settings are the default ones in the steering class
 ** CbmTransport. For the options, consult the documentation of that class:
 ** http://computing.gitpages.cbm.gsi.de/cbmroot/classCbmTransport.html
 **/
void run_tra_beam(Int_t nEvents = 1, const char* species = "Au", Double_t beamP = 12., Double_t beamStartZ = -1.,
                  const char* output = "beam", const char* setup = "sis100_electron", ECbmEngine engine = kGeant3)
{

  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "run_tra_beam";                 // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   File names   ---------------------------------------------------
  TString dataset(output);
  TString outFile = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString geoFile = dataset + ".geo.root";
  // ------------------------------------------------------------------------


  // ----   Define the beam   -----------------------------------------------
  // Here, the most frequent CBM beams are pre-defined. If you want another
  // beam species, add to this list.
  UInt_t beamZ = 0;
  UInt_t beamA = 0;
  UInt_t beamQ = 0;
  if (TString(species).EqualTo("H", TString::kIgnoreCase)) {
    beamZ = 1;  // atomic number
    beamA = 1;  // mass number
    beamQ = 1;  // charge (fully stripped)
  }
  else if (TString(species).EqualTo("C", TString::kIgnoreCase)) {
    beamZ = 6;   // atomic number
    beamA = 12;  // mass number
    beamQ = 6;   // charge (fully stripped)
  }
  else if (TString(species).EqualTo("Ca", TString::kIgnoreCase)) {
    beamZ = 20;  // atomic number
    beamA = 40;  // mass number
    beamQ = 20;  // charge (fully stripped)
  }
  else if (TString(species).EqualTo("Ni", TString::kIgnoreCase)) {
    beamZ = 28;  // atomic number
    beamA = 60;  // mass number
    beamQ = 28;  // charge (fully stripped)
  }
  else if (TString(species).EqualTo("Ag", TString::kIgnoreCase)) {
    beamZ = 47;   // atomic number
    beamA = 108;  // mass number
    beamQ = 47;   // charge (fully stripped)
  }
  else if (TString(species).EqualTo("Au", TString::kIgnoreCase)) {
    beamZ = 79;   // atomic number
    beamA = 197;  // mass number
    beamQ = 79;   // charge (fully stripped)
  }
  if (beamZ == 0) {
    std::cout << "-E- " << myName << ": unknown beam species " << species << std::endl;
    return;
  }
  // ------------------------------------------------------------------------


  // ----   Define the beam profile   ---------------------------------------
  Double_t beamFocusZ  = 0.;     // z coordinate of beam focal plane
  Double_t beamMeanX   = 0.;     // mean x position of beam [cm]
  Double_t beamSigmaX  = 0.1;    // Gaussian sigma of beam x position [cm]
  Double_t beamMeanY   = 0.;     // mean y position of beam [cm]
  Double_t beamSigmaY  = 0.1;    // Gaussian sigma of beam y position [cm]
  Double_t beamMeanTx  = 0.;     // mean x-z angle of beam [rad]
  Double_t beamSigmaTx = 0.001;  // Gaussian sigma of beam x-z angle [rad]
  Double_t beamMeanTy  = 0.;     // mean x-y angle of beam [rad]
  Double_t beamSigmaTy = 0.001;  // Gaussian sigma of beam x-y angle [rad]
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // --- Transport run   ----------------------------------------------------
  CbmTransport run;
  run.SetEngine(engine);
  run.SetOutFileName(outFile);
  run.SetParFileName(parFile);
  run.SetGeoFileName(geoFile);
  run.LoadSetup(setup);
  run.SetTarget("Gold", 0.025, 2.5);
  run.SetBeamPosition(beamMeanX, beamMeanY, beamSigmaX, beamSigmaY, beamFocusZ);
  run.SetBeamAngle(beamMeanTx, beamMeanTy, beamSigmaTx, beamSigmaTy);
  run.AddInput(new CbmBeamGenerator(beamZ, beamA, beamQ, beamP, beamStartZ));
  run.Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl << std::endl;
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;
  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------

}  // End of macro
