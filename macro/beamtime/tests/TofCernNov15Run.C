
void TofCernNov15Run( Int_t nEvents = 2 )
{
   TString outDir  = "data/";
   
   // Function needed for CTest runtime dependency
   TString depFile = Remove_CTest_Dependency_File(outDir, "TofCernNov15Run");

   TString sMacroDir  = gSystem->Getenv("VMCWORKDIR");
   sMacroDir  +=  "/macro/beamtime/tests/";

   TString sInputDir  = gSystem->Getenv("VMCWORKDIR");
   TString sDataDir  = sInputDir + "/input/";
   TString sFileId = "CbmTofSps_01Dec0206";

   // -----   Timer   --------------------------------------------------------
   TStopwatch timer;
   timer.Start();
   // ------------------------------------------------------------------------
   
   FairRunOnline *run = new FairRunOnline ();
//   Bool_t hasFairMonitor = Has_Fair_Monitor();
   Bool_t hasFairMonitor = kFALSE;
   if (hasFairMonitor) {
    FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
   }

   gROOT->LoadMacro(sMacroDir + "tof_nov15_setup_unpack.C");

   cout << "Process FileId  "<< sDataDir << " " << sFileId <<endl;

   TString sCom=Form("setup_unpack(1,\"%s\",\"%s\")", sDataDir.Data(), sFileId.Data() );
   cout << "Processline "<<sCom<<endl;
   gInterpreter->ProcessLine(sCom);
   run->Run(nEvents, 0);
   run->Finish();

   
   // --- End-of-run info
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   std::cout << std::endl << std::endl;
   std::cout << ">>> TofCernNov15: Macro finished successfully." << std::endl;
   std::cout << ">>> TofCernNov15: Real time " << rtime << " s, CPU time "
                << ctime << " s" << std::endl;
   std::cout << std::endl;

   /// -----   Resource monitoring in automatic tests  ------------------
   if ( Has_Fair_Monitor() ) {      // FairRoot Version >= 15.11
      // Extract the maximal used memory an add is as Dart measurement
      // This line is filtered by CTest and the value send to CDash
      FairSystemInfo sysInfo;
      Float_t maxMemory=sysInfo.GetMaxMemory();
      std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
      std::cout << maxMemory;
      std::cout << "</DartMeasurement>" << std::endl;

      Float_t cpuUsage=ctime/rtime;
      std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
      std::cout << cpuUsage;
      std::cout << "</DartMeasurement>" << std::endl;
   }

   /// --- Screen output for automatic tests  ---------------------------
   std::cout << " Test passed" << std::endl;
   std::cout << " All ok " << std::endl;
   
   // Function needed for CTest runtime dependency
   Generate_CTest_Dependency_File(depFile);
}
