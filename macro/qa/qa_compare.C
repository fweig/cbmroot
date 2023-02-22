/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   qa_compare.C
/// @author Sergei Zharko <s.zharko@gsi.de>
/// @since  06.02.2023
/// @brief  ROOT macro to run QA-Checker framework comparison of the old and new ROOT-file versions.

/// @brief  Function to compare two different versions of ROOT-files
/// @param configName          Name of config containing file-object map
/// @param datasetName         Name of the dataset, represented with a particular setup
/// @param oldVersionInputDir  Input directory for old version of files
/// @param newVersionInputDir  Input directory for new version of files
/// @param outputName          Name of the QA-Checker output (default is "QaCheckerResult.root")
/// @return  Result flag:
///          - 0: all objects are the same within defined comparison procedure (point-by-point for now)
///          - 1: some of the objects differ


/// NOTE: Disable clang formatting to keep easy parameters overview
/* clang-format off */
int qa_compare( 
                const char* configName         = "objects.yaml",
                const char* datasetName        = "s100e",
                const char* oldVersionInputDir = "../run/data",       // NOTE: Files from external repository
                const char* newVersionInputDir = "../run/data",       // NOTE: Files from preceding fixture
                const char* outputName         = "QACheckerOutput.h"  // TODO: Add tag of a merge request or commit
              )
/* clang-format on */
{
  // ----- Logger settings
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetColoredLog(true);

  //// ----- Style settings
  gStyle->SetPalette(kSolar);

  //// ----- Configure QA-Checker
  auto pQaChecker = std::make_unique<cbm::qa::checker::Core>();
  pQaChecker->RegisterOutFile(outputName);  // Set name of the output file
  pQaChecker->SetFromYAML(configName);      // Read file-object map

  // Add dataset
  pQaChecker->AddDataset(datasetName);

  // Add versions
  pQaChecker->AddVersion("old", oldVersionInputDir);
  pQaChecker->AddVersion("new", newVersionInputDir);
  pQaChecker->SetDefaultVersion("old");

  //// ----- Run comparision routine
  pQaChecker->Process("P");

  //// ----- Scan results
  bool res = pQaChecker->Scan();  // true - objects are the same, false - objects differ
  std::cout << "Macro finished successfully." << std::endl;
  return !res;
}
