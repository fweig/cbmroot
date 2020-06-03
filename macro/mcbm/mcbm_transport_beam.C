/** @file mcbm_transport_beam
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 13 September 2019
 **/


// Includes needed for IDE
#if !defined(__CLING__)
#include <iostream>
#include "TStopwatch.h"
#include "TString.h"
#include "FairIonGenerator.h"
#include "FairSystemInfo.h"
#include "CbmFieldConst.h"
#include "CbmTransport.h"
#endif



/** @brief Main method
 ** @param nEvents    Number of events to be generated.
 ** @param output     Name for output files, without extension
 ** @param setup      Geometry setup name
 ** @param beamAngle  Angle of beam in global c.s. in x-z plane
 ** @param beamType   Beam ion species
 ** @param eKin       Beam kinetic energy per nucleon
 ** @param targetType Target element
 ** @param targetDz   Thickness of target [cm]
 ** @param targetR    Radius of target [cm]
 **
 ** One beam particle of the defined species will be generated per event.
 ** The beam will start at z = -1 cm along the beam trajectory defined
 ** by the beam angle. It is assumed to have a Gaussian width of 1 mm
 ** in both x and y and no dispersion. The target is rotated to be
 ** perpendicular to the beam.
 **
 ** The beam profile width must be changed in the macro body, if necessary.
 **/
void mcbm_transport_beam(UInt_t nEvents = 1,
                         TString output = "mcbm_beam",
                         TString setup = "mcbm_beam_2019_03",
                         Double_t beamAngle = 25.,
                         TString beamType = "Silver",
                         Double_t eKin = 1.65,
                         TString targetType = "Gold",
                         Double_t targetDz = 0.025,
                         Double_t targetR = 1.5)
{

  // -----   Define file names   ---------------------------------------------
  TString outFileName = output + ".tra.root";
  TString parFileName = output + ".par.root";
  TString geoFileName = output + ".geo.root";
  // -------------------------------------------------------------------------


  // -----   Define beam   ---------------------------------------------------
  Int_t beamZ = 0;      ///< Atomic number
  Int_t beamA = 0;      ///< Atomic mass number
  Int_t beamQ = 0;      ///< Beam charge per nucleon
  if ( beamType.EqualTo("Silver", TString::kIgnoreCase) ||
       beamType.EqualTo("Ag", TString::kIgnoreCase) ) {
    beamZ = 47;
    beamA = 107;
    beamQ = 47;
  }
  else {
    std::cout << "Beam species " << beamType
        << " not supported; please define in macro body." << std::endl;
    return;
  }
  Double_t beamWidthX = 0.1;
  Double_t beamWidthY = 0.1;
  Double_t beamStartZ = -1.;
  // -------------------------------------------------------------------------


  // -----   Define beam generator    ----------------------------------------
  beamAngle = beamAngle * TMath::DegToRad();
  Double_t beamStart = 1.;       // Distance of beam start from origin [cm]
  Double_t mProt = 0.937272;     // Proton mass
  Double_t eBeam = eKin + mProt; // Beam energy per nucleon
  Double_t pBeam = TMath::Sqrt( eBeam * eBeam - mProt * mProt ); // Beam momentum
  Int_t mult = 1;  // Multiplicity per event
  // -------------------------------------------------------------------------


  // -----   Define target   -------------------------------------------------
  TString targetElement = "";
  if ( targetType.EqualTo("Gold", TString::kIgnoreCase) ||
       targetType.EqualTo("Au", TString::kIgnoreCase) ) {
    targetElement = "Gold";
  }
  else {
    std::cout << "Beam species " << beamType
        << " not supported; please define in macro body." << std::endl;
    return;
  }
  Double_t targetX = 0.;
  Double_t targetY = 0.;
  Double_t targetZ = 0.;
  Double_t targetRotY = beamAngle;
  // -------------------------------------------------------------------------


  // -----   Transport run   -------------------------------------------------
  TStopwatch timer;
  timer.Start();
  CbmTransport run;
  auto beamGen = new FairIonGenerator(beamZ, beamA, beamQ, 1, 0., 0., pBeam);
  run.AddInput(beamGen);
  run.SetOutFileName(outFileName);
  run.SetParFileName(parFileName);
  run.SetGeoFileName(geoFileName);
  run.LoadSetup(setup);
  run.SetField(new CbmFieldConst());
  run.SetTarget(targetElement, targetDz, targetR,
                targetX, targetY, targetZ, targetRotY);
  run.SetVertexSmearZ(kFALSE);

  run.SetBeamPosition(0., 0., beamWidthX, beamWidthY, beamStartZ);
  run.SetBeamAngle(0., beamAngle, 0., 0.);
  run.ForceVertexInTarget(kFALSE);


  // Store trajectories should not be done for large statistics.
  // Un-comment if needed.
  // run.StoreTrajectories();

  run.Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is "    << outFileName << std::endl;
  std::cout << "Parameter file is " << parFileName << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime
      << "s" << std::endl << std::endl;
  // ------------------------------------------------------------------------


  // -----   Resource monitoring   ------------------------------------------
  FairSystemInfo sysInfo;
  Float_t maxMemory=sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;

  Float_t cpuUsage=ctime/rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;

  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------


}

