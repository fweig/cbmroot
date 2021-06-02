/** @file dimuon_transport.C
  * @copyright Copyright (C) 2018-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese, Florian Uhlig [committer], Anna Senger **/

// --------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
//
// V. Friese   22/02/2007
//
// Version 2019-02-01
//
// For the setup (geometry and field), predefined setups can be chosen
// by the second argument. Available setups are in geometry/setup.
// The input file is in UniGen format and can be specified by the last argument.
// If none is specified, a default input file distributed with the source code
// is selected.
// The user has to assure the consistency of input file and target, the
// latter being defined in the macro body.
//
// The output file will be named [output].tra.root.
// A parameter file [output].par.root will be created.
// The geometry (TGeoManager) will be written into [output].geo.root.
// --------------------------------------------------------------------------


/** @brief kf_transport_new
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  2 March 2019
 ** @param nEvents    Number of events from input to transport
 ** @param setupName  Name of predefined geometry setup
 ** @param output     Name of output data set
 ** @param inputFile  Name of input file
 ** @param iDecay     Decay mode from KFPartEfficiencies
 **/
//
// Anna Senger a.senger@gsi.de
// 2019
// add PLUTO input
//---------------------------------------------------

void dimuon_transport(Int_t nEvents = 1000, const char* setupName = "sis100_muon_lmvm", const char* output = "test",
                      const char* inputFile = "")
{

  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "run_transport";                // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString dataset(output);
  TString outFile = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString geoFile = dataset + ".geo.root";

  std::cout << std::endl;
  TString inFile;
  if (strcmp(inputFile, "") == 0) { inFile = srcDir + "/input/pluto.auau.8gev.omega.mpmm.0001.root"; }
  else {
    inFile = inputFile;
  }
  std::cout << "-I- " << myName << ": Using signal input file " << inFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // --- Transport run   ----------------------------------------------------
  CbmTransport run;
  run.AddInput(inFile.Data(), kPluto);
  run.SetOutFileName(outFile);
  run.SetParFileName(parFile);
  run.SetGeoFileName(geoFile);
  run.LoadSetup(setupName);
  run.SetTarget("Gold", 0.025, 2.5);
  run.SetBeamPosition(0., 0., 0.1, 0.1);
  run.SetEngine(kGeant3);
  // ------------------------------------------------------------------------

  run.Run(nEvents);

  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl << std::endl;
  // ------------------------------------------------------------------------


  // -----   Resource monitoring   ------------------------------------------
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
