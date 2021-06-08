/* Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file run_digi.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 8 June 2018
 **/


// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmDigitization.h"

#include "FairSystemInfo.h"
#endif


/** @brief Macro for detector response simulation in CBM
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  8 June 2018
 ** @param inputEvents  Main input file (w/o extension .tra.root)
 ** @param nEvents      Number of events to process
 ** @param output       Name of output file (w/o extension .raw.root)
 ** @param eventRate    Event rate [1/s]
 ** @param tsLength     Length of time slice [ns]
 ** @param inputSignal  Signal input file, will be embedded to the events
 ** @param inputBeam    Third input file, will be mixed with the events
 ** @param beamRate     Beam rate [1/s]
 **
 ** Macro for detector response simulation in CBM
 **
 ** The detector response produces a raw data file from the transport
 ** data, which serves as input for reconstruction. Raw data will
 ** be delivered in time-slice format (one tree entry per time-slice).
 **
 ** This macro allows to simultaneously digitise three inputs, which were
 ** separately generated in respective transport runs:
 ** - the main event input,
 ** - a signal input, which is embedded into the event input (optional),
 ** - a beam input, which is mixed to the event input (optional).
 ** Please note that it is in the user's responsibility that the input files
 ** are consistent, i.e., the transport simulation was performed with the
 ** same settings (geometry, engine, cuts).
 **
 ** The input file names are expected to have the extension .tra.root.
 ** The parameter file [inputEvents].par.root will be used also for
 ** parameter output. if inputEvents is not specified by the user, it will
 ** be set to "test.tra.root", to enable execution of the macro chain
 ** run_tra_file.C and run_digi.C from the ROOT prompt without any argument.
 **
 ** The output file will be [output].raw.root. If the argument output is
 ** not specified by the user, the output file will be [inputEvents].raw.root.
 **
 ** If the event rate is set to a negative value, the simulation will be
 ** event-based, i.e. one time slice (with flexible time interval) will be
 ** created for each input event. In that case, embedding and mixing is not
 ** possible, i.e., all following arguments will be ignored.
 **
 ** If the time-slice length is set to a negative value, all data will be
 ** contained in one single time-slice (entry) with flexible time interval.
 ** Be careful with this, since it may exceed the available RAM at a certain
 ** number of events.
 **
 ** The run will terminate after the specified number of input events or when
 ** when the event input is exhausted. Entries from the beam input are taken
 ** randomly and can thus be re-used, so the beam input cannot be exhausted.
 ** Make sure that there is sufficient statistics in the beam input to avoid
 ** a bias due to repeated use of beam events.
 **
 ** For further options to modify the run settings, the macro body must be
 ** modified. Please consult the documentation of the CbmDigitization class
 ** (http://computing.gitpages.cbm.gsi.de/cbmroot/classCbmDigitization.html).
 **/
void run_digi(TString inputEvents = "", Int_t nEvents = -1, TString output = "", Double_t eventRate = 1.e7,
              Double_t tsLength = -1., TString inputSignal = "", TString inputBeam = "", Double_t beamRate = 1.e9)
{

  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TString myName = "run_digi";  // this macro's name for screen output
  // ------------------------------------------------------------------------


  // -----   Allow overwriting of output file   -----------------------------
  Bool_t overwrite = kTRUE;
  // ------------------------------------------------------------------------


  // -----   Full file names   ----------------------------------------------
  if (inputEvents.IsNull()) inputEvents = "test";
  TString evntFile = inputEvents + ".tra.root";
  TString signFile = inputSignal + ".tra.root";
  TString beamFile = inputBeam + ".tra.root";
  TString parFile  = inputEvents + ".par.root";
  if (output.IsNull()) output = inputEvents;
  TString outFile  = output + ".raw.root";
  TString moniFile = output + ".moni_digi.root";
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----  Catch event-by-event case   -------------------------------------
  // Mixing different inputs does not make sense in event-by-event.
  // Embedding event-by-event is not yet possible (will change soon).
  Bool_t eventMode = eventRate < 0.;
  if (eventMode) {
    if (!inputSignal.IsNull()) {
      std::cout << std::endl;
      std::cout << "-E- " << myName << ": Embedding is not (yet) possible in event-by-event mode! "
                << " Terminating macro execution." << std::endl;
      return;
    }  //? Signal input specified
    if (!inputBeam.IsNull()) {
      std::cout << std::endl;
      std::cout << "-E- " << myName << ": Mixing inputs is not possible in event-by-event mode! "
                << " Terminating macro execution." << std::endl;
      return;
    }  //? Beam input specified
  }    //? Event-by-event mode
  // ------------------------------------------------------------------------


  // -----   Digitization run   ---------------------------------------------
  CbmDigitization run;
  run.AddInput(0, evntFile, eventRate);
  if (!eventMode) {
    if (!inputSignal.IsNull()) run.EmbedInput(1, signFile, 0);
    if (!inputBeam.IsNull()) run.AddInput(2, beamFile, beamRate, ECbmTreeAccess::kRandom);
  }
  run.SetOutputFile(outFile, overwrite);
  run.SetMonitorFile(moniFile);
  run.SetParameterRootFile(parFile);
  run.SetTimeSliceLength(tsLength);
  run.SetEventMode(eventMode);
  run.SetProduceNoise(kFALSE);
  if (nEvents < 0) run.Run();
  else
    run.Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">" << maxMemory << "</DartMeasurement>"
            << std::endl;
  std::cout << "<DartMeasurement name=\"WallTime\" type=\"numeric/double\">" << rtime << "</DartMeasurement>"
            << std::endl;
  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">" << cpuUsage << "</DartMeasurement>"
            << std::endl;
  // ------------------------------------------------------------------------

}  // End of macro
