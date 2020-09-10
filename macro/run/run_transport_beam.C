/** @file run_transport_beam.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 9 September 2020
 **/

// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmBeamGenerator";
#include "CbmTransport.h"
#include "FairSystemInfo.h"
#include "TStopwatch.h"
#endif


/** @brief run_transport_beam
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  9 September 2020
 ** @param nEvents    Number of events from input to transport
 ** @param setupName  Name of predefined geometry setup
 ** @param output     Name of output data set
 ** @param inputFile  Name of input file
 **
 ** Macro for transport simulation of beam particles in the CBM setup.
 ** Properties of the beam have to be specified in the macro body.
 **
 ** For the setup (geometry and field), predefined setups can be chosen
 ** by the second argument. Available setups are in geometry/setup.
 **
 ** The output file will be named [output].tra.root.
 ** A parameter file [output].par.root will be created.
 ** The geometry (TGeoManager) will be written into [output].geo.root.
 **
 ** For the options for the transport simulation, see the documentation of
 ** CbmTransport.
 **/
void run_transport_beam(Int_t nEvents         = 1000,
                        const char* setupName = "sis100_electron",
                        const char* output    = "beam") {

  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "run_transport_beam";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   File names   ---------------------------------------------------
  TString dataset(output);
  TString outFile = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString geoFile = dataset + ".geo.root";
  // ------------------------------------------------------------------------


  // ----   Define the beam   -----------------------------------------------
  // Beam species here is full stripped Gold
  UInt_t beamZ        = 79;   // atomic number
  UInt_t beamA        = 197;  // mass number
  UInt_t beamQ        = 79;   // charge (fully stripped)
  Double_t beamP      = 12.;  // Momentum 12 GeV per nucleon
  Double_t beamStartZ = -1.;  // Starts at z = -1 cm
  // ------------------------------------------------------------------------


  // ----   Define the beam profile   ---------------------------------------
  Double_t beamFocusZ  = 0.;   // z coordinate of beam focal plane
  Double_t beamMeanX   = 0.;   // mean x position of beam [cm]
  Double_t beamSigmaX  = 0.1;  // Gaussian sigma of beam x position [cm]
  Double_t beamMeanY   = 0.;   // mean y position of beam [cm]
  Double_t beamSigmaY  = 0.1;  // Gaussian sigma of beam y position [cm]
  Double_t beamMeanTx  = 0.;   // mean x-z angle of beam [rad]
  Double_t beamSigmaTx = 1.;   // Gaussian sigma of beam x-z angle [rad]
  Double_t beamMeanTy  = 0.;   // mean x-y angle of beam [rad]
  Double_t beamSigmaTy = 1.;   // Gaussian sigma of beam x-y angle [rad]
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // --- Transport run   ----------------------------------------------------
  CbmTransport run;
  run.SetOutFileName(outFile);
  run.SetParFileName(parFile);
  run.SetGeoFileName(geoFile);
  run.LoadSetup(setupName);
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
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s"
            << std::endl
            << std::endl;
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
