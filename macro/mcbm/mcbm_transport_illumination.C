/* Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/** @file mcbm_transport_illumination
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @since 21 February 2019
 **/

/** @brief Main method
 ** @param nEvents    Number of events to be generated.
 ** @param dMinX      Min value of X position of the track starting point [cm]
 ** @param dMaxX      Max value of X position of the track starting point [cm]
 ** @param dMinY      Min value of Y position of the track starting point [cm]
 ** @param dMaxY      Max value of Y position of the track starting point [cm]
 ** @param dMinP      Min value of momentum of the proton [GeV]
 ** @param dMaxP      Max value of momentum of the proton [GeV]
 ** @param dThetaMin  Min value of momentum of the track angle against the Z axis [deg]
 ** @param dThetaMax  Max value of momentum of the track angle against the Z axis [deg]
 ** @param output     Pattern for output file names
 ** @param setup      Tag of the mCBM setup file
 **
 ** Shoot protons following the given source properties.
 **/
void mcbm_transport_illumination(UInt_t nEvents = 1, Double_t dMinX = -10.0, Double_t dMaxX = 10.0,
                                 Double_t dMinY = -10.0, Double_t dMaxY = 10.0, Double_t dMinP = 1.3,
                                 Double_t dMaxP = 1.4, Double_t dThetaMin = 0.0, Double_t dThetaMax = 0.0,
                                 TString output = "test", TString setup = "mcbm_beam_2019_11")
{

  // -----   Define file names   ---------------------------------------------
  TString outFileName = output + ".tra.root";
  TString parFileName = output + ".par.root";
  TString geoFileName = output + ".geo.root";
  // -------------------------------------------------------------------------

  // --- Primary protons (PDG=2212) generator , 100 per events.
  FairBoxGenerator* boxGen = new FairBoxGenerator(2212, 1);

  // The starting points in x and y or the angles are chosen such as to
  // illuminate the standard mSTS station
  // Cartesian coord: in z direction, starting at z = 0.
  boxGen->SetBoxXYZ(dMinX, dMinY, dMaxX, dMaxY, 0.);
  boxGen->SetPRange(dMinP,
                    dMaxP);  // arbitrary, not sure if any effect to expect here
  boxGen->SetThetaRange(dThetaMin, dThetaMax);
  boxGen->SetPhiRange(0., 360.);

  // -----   Transport run   -------------------------------------------------
  TStopwatch timer;
  timer.Start();
  CbmTransport run;

  run.AddInput(boxGen);

  run.SetOutFileName(outFileName);
  run.SetParFileName(parFileName);
  run.SetGeoFileName(geoFileName);

  run.LoadSetup(setup);
  run.SetField(new CbmFieldConst());

  run.SetVertexSmearZ(kFALSE);
  run.SetBeamPosition(0., 0., 0., 0., 0.);
  run.SetBeamAngle(0., 0., 0., 0.);
  run.ForceVertexInTarget(kFALSE);

  // Store trajectories should not be done for large statistics.
  // Un-comment if needed.
  //  run.StoreTrajectories();

  run.Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFileName << std::endl;
  std::cout << "Parameter file is " << parFileName << std::endl;
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
}
