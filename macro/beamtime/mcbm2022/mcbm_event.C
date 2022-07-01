/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Adrian Weber */


// --- Includes needed for IDE
#include <RtypesCore.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <math.h>
#include <stdio.h>
#if !defined(__CLING__)
#include "CbmTrdRawMessageSpadic.h"
#include "CbmTrdSpadic.h"

#include <FairLogger.h>
#include <FairRootFileSink.h>
#include <FairRunOnline.h>
#include <Logger.h>

#include <TStopwatch.h>
#include <TSystem.h>
#endif

/// FIXME: Disable clang formatting to keep easy parameters overview
/* clang-format off */
Bool_t mcbm_event(std::string infile,
                  UInt_t uRunId,
                  uint32_t uTriggerSet = 3,
                  std::int32_t nTimeslices = -1,
                  std::string sOutDir = "data/")
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  std::vector<std::string> vInFile = {infile};

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_event";                   // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   EventBuilder Settings----------------
  /// FIXME: Disable clang formatting to keep easy parameters overview
  /* clang-format off */
  UInt_t eb_TriggerMinNumberBmon  { 0 };
  UInt_t eb_TriggerMinNumberSts   { 0 };
  UInt_t eb_TriggerMinNumberMuch  { 0 };
  UInt_t eb_TriggerMinNumberTrd1d { 0 };
  UInt_t eb_TriggerMinNumberTrd2d { 0 };
  UInt_t eb_TriggerMinNumberTof   { 4 };
  UInt_t eb_TriggerMinNumberRich  { 0 };

  Int_t  eb_TriggerMaxNumberBMon  { -1 };
  Int_t  eb_TriggerMaxNumberSts   { -1 };
  Int_t  eb_TriggerMaxNumberMuch  { -1 };
  Int_t  eb_TriggerMaxNumberTrd1d { -1 };
  Int_t  eb_TriggerMaxNumberTrd2d { -1 };
  Int_t  eb_TriggerMaxNumberTof   { -1 };
  Int_t  eb_TriggerMaxNumberRich  { -1 };

  UInt_t eb_TriggerMinLayersNumberTof  { 0 };
  UInt_t eb_TriggerMinLayersNumberSts  { 0 };

  Double_t eb_TrigWinMinBMon  { -20};
  Double_t eb_TrigWinMaxBMon  {  20};
  Double_t eb_TrigWinMinSts   { -40};
  Double_t eb_TrigWinMaxSts   {  40};
  Double_t eb_TrigWinMinMuch  { -50};
  Double_t eb_TrigWinMaxMuch  { 500};
  Double_t eb_TrigWinMinTrd1d { -50};
  Double_t eb_TrigWinMaxTrd1d { 400};
  Double_t eb_TrigWinMinTrd2d { -60};
  Double_t eb_TrigWinMaxTrd2d { 350};
  Double_t eb_TrigWinMinTof   { -10};
  Double_t eb_TrigWinMaxTof   {  70};
  Double_t eb_TrigWinMinRich  { -10};
  Double_t eb_TrigWinMaxRich  {  40};


  bool bTrigSet = true;
  switch (uTriggerSet) {
    case 0: {
      // default, any Tof hit
      eb_TriggerMaxNumberBMon      = 1000;

      eb_TriggerMinNumberTof       =    1;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -80;
      eb_TrigWinMaxTof             =  120;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 1: {
      // default,  Tof - T0 concidences (pulser)
      eb_TriggerMinNumberBmon      =    1;
      eb_TriggerMaxNumberBMon      = 1000;

      eb_TriggerMinNumberTof       =    2;
      eb_TriggerMinLayersNumberTof =    1;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             = -180;
      eb_TrigWinMaxTof             =  220;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 2: {
      // Tof standalone track trigger (cosmic)
      eb_TriggerMaxNumberBMon      = 1000;

      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -30;
      eb_TrigWinMaxTof             =   70;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 3: {
      // # Tof track trigger with T0
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMaxNumberBMon      = 2;

      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -20;
      eb_TrigWinMaxTof             =   60;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 4: {
      // mCbm track trigger Tof, T0 & STS
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMaxNumberBMon      = 2;

      eb_TriggerMinNumberSts       = 2;
      eb_TriggerMinLayersNumberSts = 1;

      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -20;
      eb_TrigWinMaxTof             =   60;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 5: {
      // mCbm lambda trigger
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMaxNumberBMon      = 2;

      eb_TriggerMinNumberSts       = 8;
      eb_TriggerMinLayersNumberSts = 2;

      eb_TriggerMinNumberTof       = 16;
      eb_TriggerMinLayersNumberTof = 8; // # PAL 07/04/2022: Not sure here if it should be 4 or 8 (2 tracks in same det. stack excluded?)

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -20;
      eb_TrigWinMaxTof             =   60;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 6: {
      // One hit per detector system w/ big acceptance = mCbm full track trigger
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMaxNumberBMon      = 1;

      eb_TriggerMinNumberSts       = 4;
      eb_TriggerMinLayersNumberSts = 0;

      eb_TriggerMinNumberMuch      = 2;

      eb_TriggerMinNumberTrd1d     = 2;

      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -20;
      eb_TrigWinMaxTof             =   60;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 7: {
      /// PAL default: T0 + STS + TOF, only digi cut
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 2;
      eb_TriggerMinNumberTof       = 4;
      break;
    }
    case 8: {
      // default,  Tof - T0 concidences (pulser)
      eb_TriggerMinNumberBmon      = 4;
      eb_TriggerMinNumberTof       = 2;
      eb_TriggerMinLayersNumberTof = 1;
      break;
    }
    case 9: {
      // Tof standalone track trigger (cosmic)
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;
      break;
    }
    case 10: {
      // # Tof track trigger with T0
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;
      break;
    }
    case 11: {
      // mCbm track trigger Tof, T0 & STS
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 2;
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;
      break;
    }
    case 12: {
      // mCbm lambda trigger
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 8;
      eb_TriggerMinNumberTof       = 16;
      eb_TriggerMinLayersNumberTof = 8; // # PAL 07/04/2022: Not sure here if it should be 4 or 8 (2 tracks in same det. stack excluded?)
      break;
    }
    case 13: {
      // One hit per detector system w/ big acceptance = mCbm full track trigger
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 4;
      eb_TriggerMinNumberTrd1d     = 2;
      eb_TriggerMinNumberTrd1d     = 1;
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinNumberRich      = 1;
      break;
    }
    case 14: {
      /// PAL mCbm track trigger Tof, T0 & STS
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 4;
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;
      eb_TriggerMinLayersNumberSts = 2;
      break;
    }
    default: {
      bTrigSet = false;
      break;
    }
  }
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */
  // ------------------------------------------------------------------------


  // -----   Output filename   ----------------------------------------------
  std::string filename    = Form("%d%s.event.root", uRunId, (bTrigSet ? Form("_%u", uTriggerSet) : ""));
  std::string outfilename = sOutDir + "/" + filename;
  std::cout << "-I- " << myName << ": Output file will be " << outfilename << std::endl;
  std::string histosfilename = sOutDir + "/" + filename;
  histosfilename.replace(histosfilename.find(".event.root"), 11, ".hist.root");
  std::cout << "-I- " << myName << ": Histos file will be " << histosfilename << std::endl;
  // ------------------------------------------------------------------------

  // --------------------event builder---------------------------------------
  CbmTaskBuildRawEvents* evBuildRaw = new CbmTaskBuildRawEvents();

  //Choose between NoOverlap, MergeOverlap, AllowOverlap
  evBuildRaw->SetEventOverlapMode(EOverlapModeRaw::AllowOverlap);

  // Set TOF as reference detector
  evBuildRaw->SetReferenceDetector(kRawEventBuilderDetTof);
  evBuildRaw->AddDetector(kRawEventBuilderDetT0);

  // Set T0 as reference detector
  evBuildRaw->SetReferenceDetector(kRawEventBuilderDetT0);
  evBuildRaw->AddDetector(kRawEventBuilderDetTof);

  // Remove detectors not there in 2022
  evBuildRaw->RemoveDetector(kRawEventBuilderDetSts);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd2D);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetPsd);

  // Add all 2022 detectors in the right order
  evBuildRaw->AddDetector(kRawEventBuilderDetSts);
  //  evBuildRaw->AddDetector(kRawEventBuilderDetMuch);
  evBuildRaw->AddDetector(kRawEventBuilderDetTrd);
  evBuildRaw->AddDetector(kRawEventBuilderDetTrd2D);
  evBuildRaw->AddDetector(kRawEventBuilderDetRich);

  // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength):
  // => TsStartTime=0, TsLength=256ms in 2021, TsOverLength=TS overlap, not used in mCBM2021
  //evBuildRaw->SetTsParameters(0.0, 2.56e8, 0.0);, 0.0);

  // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength):
  // => TsStartTime=0, TsLength=102.4ms in 2022, TsOverLength=TS overlap not used in mCBM2022?
  evBuildRaw->SetTsParameters(0.0, 1.28e8, 1.12928e8);

  /// FIXME: Disable clang formatting to keep easy parameters overview
  /* clang-format off */
  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kT0,    eb_TriggerMinNumberBmon);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kT0,    eb_TriggerMaxNumberBMon);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts,   eb_TriggerMinNumberSts);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts,   eb_TriggerMaxNumberSts);
  evBuildRaw->SetTriggerMinLayersNumber(ECbmModuleId::kSts, eb_TriggerMinLayersNumberSts);
/*
  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kMuch,  eb_TriggerMinNumberMuch);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kMuch,  eb_TriggerMaxNumberMuch);
*/
  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTrd,   eb_TriggerMinNumberTrd1d);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTrd,   eb_TriggerMaxNumberTrd1d);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTrd2d, eb_TriggerMinNumberTrd2d);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTrd2d, eb_TriggerMaxNumberTrd2d);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTof, eb_TriggerMinNumberTof);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTof, eb_TriggerMaxNumberTof);
  evBuildRaw->SetTriggerMinLayersNumber(ECbmModuleId::kTof, eb_TriggerMinLayersNumberTof);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kRich,  eb_TriggerMinNumberRich);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kRich,  eb_TriggerMaxNumberRich);


  evBuildRaw->SetTriggerWindow(ECbmModuleId::kT0,    eb_TrigWinMinBMon,  eb_TrigWinMaxBMon);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts,   eb_TrigWinMinSts,   eb_TrigWinMaxSts);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kMuch,  eb_TrigWinMinMuch,  eb_TrigWinMaxMuch);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd,   eb_TrigWinMinTrd1d, eb_TrigWinMaxTrd1d);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd2d, eb_TrigWinMinTrd2d, eb_TrigWinMaxTrd2d);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kTof,   eb_TrigWinMinTof,   eb_TrigWinMaxTof);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kRich,  eb_TrigWinMinRich,  eb_TrigWinMaxRich);

  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kT0,
                                    (0 < eb_TriggerMaxNumberBMon ? eb_TriggerMaxNumberBMon : 50));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kSts,
                                    (0 < eb_TriggerMaxNumberSts ? eb_TriggerMaxNumberSts : 2000));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kMuch,
                                    (0 < eb_TriggerMaxNumberMuch ? eb_TriggerMaxNumberMuch : 1000));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kTrd,
                                    (0 < eb_TriggerMaxNumberTrd1d ? eb_TriggerMaxNumberTrd1d : 1500));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kTrd2d,
                                    (0 < eb_TriggerMaxNumberTrd2d ? eb_TriggerMaxNumberTrd2d : 500));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kTof,
                                    (0 < eb_TriggerMaxNumberTof ? eb_TriggerMaxNumberTof : 500));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kRich,
                                    (0 < eb_TriggerMaxNumberRich ? eb_TriggerMaxNumberRich : 600));

  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  // Use standard MUCH digis
  evBuildRaw->ChangeMuchBeamtimeDigiFlag();

  evBuildRaw->SetOutFilename(histosfilename);
  // evBuildRaw->SetOutputBranchPersistent("CbmEvent", kFALSE);
  evBuildRaw->SetWriteHistosToFairSink(kFALSE);
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  auto run         = new FairRunAna();
  auto inputSource = new FairFileSource(infile);
  run->SetSource(inputSource);
  auto sink = new FairRootFileSink(outfilename.data());
  run->SetSink(sink);
  run->SetRunId(uRunId);

  run->AddTask(evBuildRaw);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  if (nTimeslices < 0) {
    std::cout << "-I- " << myName << ": Starting run over all timeslices in input" << std::endl;
    run->Run(0, -1);
  }
  else {
    std::cout << "-I- " << myName << ": Starting run over " << nTimeslices
              << " timeslices (or less if not enough in input)" << std::endl;
    run->Run(0, nTimeslices);
  }
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "After CpuTime = " << timer.CpuTime() << " s RealTime = " << timer.RealTime() << " s." << std::endl;
  // ------------------------------------------------------------------------

  return kTRUE;
}  // End of main macro function
