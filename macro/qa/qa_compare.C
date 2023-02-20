/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   qa_compare.C
/// @author Sergei Zharko <s.zharko@gsi.de>
/// @since  06.02.2023
/// @brief  ROOT macro to run QA-Checker framework

/// @brief  Function to compare QA results
/// @param configName    Name of configuration file with object list, datasets and versions
/// @param inputRootDir  Root directory with input data
/// @param outputName    Name of the output (default is "QaCheckerResult.root")
/// @return  Result flag:
///          - 0: all objects are the same within defined comparison procedure
///          - 1: some of the objects differ within versions
int qa_compare(const char* configName, const char* inputRootDir = ".", const char* outputName = "QaCheckerResult.root")
{
  // ----- Logger settings
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetColoredLog(true);

  // ----- Style settings
  gStyle->SetPalette(kSolar);

  // ----- Configure QA-Checker
  auto pQaChecker = std::make_unique<cbm::qa::checker::Core>();
  pQaChecker->RegisterOutFile(outputName);  // Set name of the output file
  pQaChecker->SetFromYAML(configName);      // Read file-object map
  pQaChecker->SetInputRootPath(inputRootDir);

  // ----- Run comparision routine
  pQaChecker->Process("P");

  // ----- Scan results
  bool res = pQaChecker->Scan();
  return !res;
}
