/** @file unpack_tsa_cosy2019.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Pierre-Alain Loizeau [committer] **/

/** @file MCBM DATA unpacking
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by P.-A. Loizeau
 ** @date 30.01.2019
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */
// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void unpack_tsa_cosy2019(TString inFile = "", UInt_t uRunId = 0, UInt_t nrEvents = 0, TString outDir = "data")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%04u", uRunId);
  TString outFile = outDir + "/unp_cosy_" + runId + ".root";
  TString parFile = outDir + "/unp_cosy_params_" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG4");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/cosy2019/";

  TString paramFileSts       = paramDir + "mStsPar.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);

  TString paramFileTof       = paramDir + "mT0Par.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);


  TString paramFileHodo       = paramDir + "mHodoPar.par";
  TObjString* parHodoFileName = new TObjString(paramFileHodo);
  parFileList->Add(parHodoFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: Initialising..." << std::endl;

  CbmMcbm2018UnpackerTaskTof* unpacker_tof   = new CbmMcbm2018UnpackerTaskTof();
  CbmCosy2019UnpackerTaskHodo* unpacker_hodo = new CbmCosy2019UnpackerTaskHodo();

  unpacker_tof->SetMonitorMode();
  unpacker_hodo->SetMonitorMode();

  unpacker_tof->SetIgnoreOverlapMs();
  unpacker_hodo->SetIgnoreOverlapMs();

  unpacker_tof->SetSeparateArrayT0();

  switch (uRunId) {
    case 1:  // No correlation and missing ASICs?
    {
      break;
    }
    case 2: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);  // Run 2
      unpacker_tof->SetTimeOffsetNs(0);      // Run 2 = not there
      unpacker_hodo->SetTimeOffsetNs(0);     // Run 2 = reference

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 2, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 2, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 2, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 2, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 2, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 2, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 2, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 2, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 2, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 2, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 2, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 2, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 2, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 2, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 2, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 2, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 25.0);  // back

      break;
    }        // 2
    case 3:  // First 200 TS = spill break?
    {
      break;
    }
    case 4:  // First 200 TS = spill break?
    {
      break;
    }
    case 5: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);  // Run 5
      unpacker_tof->SetTimeOffsetNs(0);      // Run 5 = not there
      unpacker_hodo->SetTimeOffsetNs(0);     // Run 5 = reference

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 5, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 5, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 5, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 5, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 5, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 5, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 5, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 5, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 5, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 5, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 5, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 5, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 5, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 5, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 5, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 5, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back

      break;
    }  // 5
    case 6: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);  // Run 6
      unpacker_tof->SetTimeOffsetNs(0);      // Run 6 = not there
      unpacker_hodo->SetTimeOffsetNs(0);     // Run 6 = reference

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 6, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 6, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 6, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 6, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 6, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 6, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 6, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 6, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 6, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 6, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 6, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 6, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 6, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 6, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 6, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 6, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back

      break;
    }  // 6
    case 7: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);  // Run 7
      unpacker_tof->SetTimeOffsetNs(0);      // Run 7 = not there
      unpacker_hodo->SetTimeOffsetNs(0);     // Run 7 = reference

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 7, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 7, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 7, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 7, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 7, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 7, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 7, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 7, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 7, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 7, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 7, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 7, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 7, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 7, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 7, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 7, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back

      break;
    }  // 7
    case 8: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);  // Run 8
      unpacker_tof->SetTimeOffsetNs(0);      // Run 8 = not there
      unpacker_hodo->SetTimeOffsetNs(0);     // Run 8 = reference

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 8, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 8, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 8, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 8, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 8, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 8, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 8, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 8, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 8, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 8, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 8, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 8, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 8, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 8, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 8, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 8, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back

      break;
    }  // 8
    case 9: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);  // Run 9
      unpacker_tof->SetTimeOffsetNs(0);      // Run 9 = not there
      unpacker_hodo->SetTimeOffsetNs(0);     // Run 9 = reference

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 9, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 9, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 9, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 9, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 9, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 9, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 9, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 9, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 9, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 9, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 9, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 9, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 9, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 9, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 9, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 9, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back

      break;
    }         // 9
    case 10:  // First 100 TS = spill break?
    {
      break;
    }
    case 11: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);  // Run 11
      unpacker_tof->SetTimeOffsetNs(0);      // Run 11 = not there
      unpacker_hodo->SetTimeOffsetNs(0);     // Run 11 = reference

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 11, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 11, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 11, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 11, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 11, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 11, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 11, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 11, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 11, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 11, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 11, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 11, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 11, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 11, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 11, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 11, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back

      break;
    }  // 11
    case 12: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(-8000);  // Run 12
      unpacker_tof->SetTimeOffsetNs(0);          // Run 12 = not there
      unpacker_hodo->SetTimeOffsetNs(0);         // Run 12 = reference

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 12, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 12, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 12, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 12, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 12, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 12, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 12, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 12, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 12, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 12, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 12, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 12, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 12, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 12, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 12, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 12, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back

      break;
    }  // 12
    case 13: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);  // Run 13 = Unused
      unpacker_tof->SetTimeOffsetNs(0);      // Run 13 = reference
      unpacker_hodo->SetTimeOffsetNs(-975);  // Run 13

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 13, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 13, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 13, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 13, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 13, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 13, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 13, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 13, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 13, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 13, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 13, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 13, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 13, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 13, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 13, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 13, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);     // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, -43.75);  // back
      break;
    }  // 13
    case 14: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);  // Run 14 = Unused
      unpacker_tof->SetTimeOffsetNs(0);      // Run 14 = reference
      unpacker_hodo->SetTimeOffsetNs(-975);  // Run 14

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 14, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 14, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 14, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 14, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 14, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 14, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 14, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 14, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 14, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 14, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 14, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 14, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 14, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 14, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 14, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 14, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);     // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, -43.75);  // back
      break;
    }  // 14
    case 15: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);   // Run 15 = Unused
      unpacker_tof->SetTimeOffsetNs(0);       // Run 15 = reference
      unpacker_hodo->SetTimeOffsetNs(10225);  // Run 15

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 15, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 15, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 15, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 15, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 15, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 15, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 15, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 15, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 15, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 15, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 15, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 15, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 15, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 15, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 15, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 15, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);     // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, -43.75);  // back
      break;
    }  // 15
    case 16: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);      // Run 16
      unpacker_tof->SetTimeOffsetNs(0);          // Run 16 = reference
      unpacker_hodo->SetTimeOffsetNs(-1006.25);  // Run 16

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 16, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 16, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 16, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 16, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 16, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 16, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 16, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 16, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 16, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 16, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 16, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 16, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 16, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 16, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 16, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 16, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back
      break;
    }  // 16
    case 17: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);      // Run 17
      unpacker_tof->SetTimeOffsetNs(0);          // Run 17 = reference
      unpacker_hodo->SetTimeOffsetNs(-1006.25);  // Run 17

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 17, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 17, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 17, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 17, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 17, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 17, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 17, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 17, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 17, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 17, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 17, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 17, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 17, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 17, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 17, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 17, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back
      break;
    }  // 17
    case 18: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);      // Run 18
      unpacker_tof->SetTimeOffsetNs(0);          // Run 18 = reference
      unpacker_hodo->SetTimeOffsetNs(-1006.25);  // Run 18

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 18, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 18, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 18, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 18, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 18, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 18, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 18, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 18, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 18, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 18, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 18, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 18, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 18, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 18, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 18, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 18, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back
      break;
    }  // 18
    case 19: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);      // Run 19
      unpacker_tof->SetTimeOffsetNs(0);          // Run 19 = reference
      unpacker_hodo->SetTimeOffsetNs(-1006.25);  // Run 19

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 19, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 19, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 19, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 19, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 19, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 19, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 19, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 19, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 19, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 19, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 19, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 19, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 19, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 19, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 19, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 19, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back
      break;
    }  // 19
    case 20: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);      // Run 20
      unpacker_tof->SetTimeOffsetNs(0);          // Run 20 = reference
      unpacker_hodo->SetTimeOffsetNs(-1006.25);  // Run 20

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 20, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 20, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 20, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 20, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 20, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 20, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 20, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 20, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 20, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 20, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 20, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 20, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 20, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 20, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 20, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 20, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back
      break;
    }  // 20
    case 21: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);      // Run 21
      unpacker_tof->SetTimeOffsetNs(0);          // Run 21 = reference
      unpacker_hodo->SetTimeOffsetNs(-1006.25);  // Run 21

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 21, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 21, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 21, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 21, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 21, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 21, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 21, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 21, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 21, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 21, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 21, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 21, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 21, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 21, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 21, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 21, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back
      break;
    }  // 21
    case 22: {
      // Empty run?!?
      break;
    }  // 22
    case 23: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);      // Run 23
      unpacker_tof->SetTimeOffsetNs(0);          // Run 23 = reference
      unpacker_hodo->SetTimeOffsetNs(-1006.25);  // Run 23

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 23, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 23, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 23, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 23, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 23, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 23, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 23, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 23, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 23, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 23, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 23, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 23, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 23, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 23, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 23, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 23, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 37.5);  // back
      break;
    }  // 23
    case 24: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);      // Run 24
      unpacker_tof->SetTimeOffsetNs(0);          // Run 24 = reference
      unpacker_hodo->SetTimeOffsetNs(-1006.25);  // Run 24

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);     // Run 24, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, -25.0);   // Run 24, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);    // Run 24, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);    // Run 24, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);    // Run 24, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);    // Run 24, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);    // Run 24, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);    // Run 24, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);    // Run 24, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, -25.0);  // Run 24, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 12.5);   // Run 24, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);    // Run 24, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 12.5);   // Run 24, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, -25.0);  // Run 24, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, -25.0);  // Run 24, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 25.0);   // Run 24, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);    // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);    // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 18.75);  // back
      break;
    }  // 24
    case 25: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(5392);  // Run 25
      unpacker_tof->SetTimeOffsetNs(0);         // Run 25 = reference
      unpacker_hodo->SetTimeOffsetNs(2187.5);   // Run 25

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);     // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);     // Run 25, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, -25.0);   // Run 25, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);    // Run 25, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);    // Run 25, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);    // Run 25, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);    // Run 25, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);    // Run 25, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);    // Run 25, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);    // Run 25, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, -25.0);  // Run 25, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 12.5);   // Run 25, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);    // Run 25, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 12.5);   // Run 25, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, -25.0);  // Run 25, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, -25.0);  // Run 25, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 25.0);   // Run 25, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);   // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 25.0);  // back
      break;
    }  // 25
    case 26: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_hodo->SetTimeOffsetNsSts(0);      // Run 26
      unpacker_tof->SetTimeOffsetNs(0);          // Run 26 = reference
      unpacker_hodo->SetTimeOffsetNs(-1006.25);  // Run 26

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_hodo->SetTimeOffsetNsAsicSts(0, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(1, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(2, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(3, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(4, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(5, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(6, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(7, 0.0);   // Unused
      unpacker_hodo->SetTimeOffsetNsAsicSts(8, 0.0);   // Run 26, Ladder 0, Module 0, P, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(9, 0.0);   // Run 26, Ladder 0, Module 0, P, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(10, 0.0);  // Run 26, Ladder 0, Module 0, P, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(11, 0.0);  // Run 26, Ladder 0, Module 0, P, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(12, 0.0);  // Run 26, Ladder 0, Module 0, P, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(13, 0.0);  // Run 26, Ladder 0, Module 0, P, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(14, 0.0);  // Run 26, Ladder 0, Module 0, P, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(15, 0.0);  // Run 26, Ladder 0, Module 0, P, Asic 7
      unpacker_hodo->SetTimeOffsetNsAsicSts(16, 0.0);  // Run 26, Ladder 0, Module 0, N, Asic 0
      unpacker_hodo->SetTimeOffsetNsAsicSts(17, 0.0);  // Run 26, Ladder 0, Module 0, N, Asic 1
      unpacker_hodo->SetTimeOffsetNsAsicSts(18, 0.0);  // Run 26, Ladder 0, Module 0, N, Asic 2
      unpacker_hodo->SetTimeOffsetNsAsicSts(19, 0.0);  // Run 26, Ladder 0, Module 0, N, Asic 3
      unpacker_hodo->SetTimeOffsetNsAsicSts(20, 0.0);  // Run 26, Ladder 0, Module 0, N, Asic 4
      unpacker_hodo->SetTimeOffsetNsAsicSts(21, 0.0);  // Run 26, Ladder 0, Module 0, N, Asic 5
      unpacker_hodo->SetTimeOffsetNsAsicSts(22, 0.0);  // Run 26, Ladder 0, Module 0, N, Asic 6
      unpacker_hodo->SetTimeOffsetNsAsicSts(23, 0.0);  // Run 26, Ladder 0, Module 0, N, Asic 7

      unpacker_hodo->SetTimeOffsetNsAsic(0, 0.0);    // Unused
      unpacker_hodo->SetTimeOffsetNsAsic(1, 0.0);    // front = reference
      unpacker_hodo->SetTimeOffsetNsAsic(2, 18.75);  // back
      break;
    }  // 26
    default: break;
  }  // switch( uRunId )

  // ADC cut
  unpacker_hodo->SetAdcCutSts(3);

  // Noisy Channel masking
  unpacker_hodo->MaskNoisyChannelSts(1, 65, true);
  unpacker_hodo->MaskNoisyChannelSts(1, 253, true);
  unpacker_hodo->MaskNoisyChannelSts(1, 255, true);
  unpacker_hodo->MaskNoisyChannelSts(1, 260, true);

  unpacker_hodo->MaskNoisyChannelSts(2, 640, true);
  unpacker_hodo->MaskNoisyChannelSts(2, 642, true);
  unpacker_hodo->MaskNoisyChannelSts(2, 648, true);
  unpacker_hodo->MaskNoisyChannelSts(2, 764, true);
  unpacker_hodo->MaskNoisyChannelSts(2, 766, true);

  /// Mask all channels in FEB 1 end even channels in FEB 2 in mSTS
  /*
  for( UInt_t uChan = 0; uChan < 1024; ++uChan )
  {
    unpacker_hodo ->MaskNoisyChannelSts(  1,   uChan, true );
    if( 0 == uChan %2 )
      unpacker_hodo ->MaskNoisyChannelSts(  2,   uChan, true );
  } // for( UInt_t uChan = 0; uChan < 1024; ++uChan )
  */

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  source->SetFileName(inFile);
  if (13 <= uRunId) {
    source->AddUnpacker(unpacker_tof, 0x90, ECbmModuleId::kTof);  //gDPB T0
  }                                                               // if (13 <= uRunId)
  source->AddUnpacker(unpacker_hodo, 0x10, ECbmModuleId::kHodo);  //HODO + STS xyter

  // --- Event header
  FairEventHeader* event = new FairEventHeader();
  event->SetRunId(uRunId);

  // --- RootFileSink
  // --- Open next outputfile after 4GB
  FairRootFileSink* sink = new FairRootFileSink(outFile);
  //  sink->GetOutTree()->SetMaxTreeSize(4294967295LL);

  // --- Run
  run = new FairRunOnline(source);
  run->SetSink(sink);
  run->SetEventHeader(event);
  run->SetAutoFinish(kFALSE);


  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  Bool_t kParameterMerged   = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parOut->open(parFile.Data());
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);
  rtdb->setOutput(parOut);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> unpack_tsa_mcbm: Starting run..." << std::endl;
  if (0 == nrEvents) {
    run->Run(nEvents, 0);  // run until end of input file
  }
  else {
    run->Run(0, nrEvents);  // process  N Events
  }
  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Macro finished successfully." << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Output file is " << outFile << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}

void unpack_tsa_cosy2019(UInt_t uSlurmRunId = 0, UInt_t nrEvents = 0)
{
  static const UInt_t kuNbRuns = 26;

  if (kuNbRuns <= uSlurmRunId) {
    std::cout << "Invalid slurm run ID: runs got from 1 to 26, corresponding "
                 "to Slurm ID 0 to 25 instead of "
              << uSlurmRunId;
    return;
  }  // if( kuNbRuns <= uSlurmRunId )

  TString sDir = "/lustre/cbm/prod/beamtime/2019/11/cosy/";

  TString sRunTag[kuNbRuns] = {
    "r0001_20191106_1140", "r0002_20191106_1346", "r0003_20191106_1543", "r0004_20191106_1735", "r0005_20191106_1825",
    "r0006_20191107_1016", "r0007_20191107_1048", "r0008_20191107_1059", "r0009_20191107_1126", "r0010_20191107_1136",
    "r0011_20191107_1419", "r0012_20191107_1432", "r0013_20191107_1725", "r0014_20191107_1738", "r0015_20191107_1750",
    "r0016_20191108_0930", "r0017_20191108_1051", "r0018_20191108_1121", "r0019_20191108_1250", "r0020_20191108_1400",
    "r0021_20191108_1432", "r0022_20191108_1558", "r0023_20191108_1610", "r0024_20191108_1716", "r0025_20191108_1752",
    "r0026_20191108_1808"};

  TString sInFile = Form("%s%s/%s*.tsa", sDir.Data(), sRunTag[uSlurmRunId].Data(), sRunTag[uSlurmRunId].Data());

  unpack_tsa_cosy2019(sInFile, uSlurmRunId + 1, nrEvents);
}
