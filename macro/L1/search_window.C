/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   search_window.C
/// \brief  A macro to run a hit-search window estimator for the CA tracking
/// \since  8.11.2022
/// \author S.Zharko <s.zharko@gsi.de>

/// Splits a string to a vector of substrings using a char token
/// \param  origString  Original string
/// \param  token       Char token
std::vector<TString> SplitString(const char* aString, char token = ':');

// Instruction
//
// 1. Creation of MC-triplets tree input
// In a reconstruction macro, please, run CbmL1::SetOutputMcTripletsTreeFilename("<prefix>"), where <prefix> is a pre-
// fix for an output file. NOTE: this feature will not work with the "debugWithMC" flag switched off
//
// 2. Initialization of search windows file to CA tracking
// The file contains an array of serialized L1SearchWindow objects. The file can be generated
// for a given set of station IDs, MC-triplet trees and a sequence of track finder iteration using
// this macro. The L1SearchWindow objects are optional. They are initialized as far as a corresponding input file
// is provided. To do so, please, add this function into your reconstruction macro
//l1->SetInputSearchWindowFilename(TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/L1/SearchWindows.dat");


/// Estimates search windows
void search_window(
  const char* yamlConfigFile = "",  ///< Name of yaml configuration file containing track finder iterations
  const char* inputTreeFile  = "",  ///< Name of ROOT input file with MC triplets
  const char* swOutputFile   = "",  ///< Name of L1SearchWindow objects output
  const char* logOutputName  = ""   ///< Name of pdf log
)
{
  // ----- Create a window finder object
  ca::tools::WindowFinder wf;

  // ----- Initialize window finder parameters
  wf.SetOutputName(swOutputFile);
  wf.ReadTrackingIterationsFromYAML(yamlConfigFile);

  // Set global indexes of active tracking stations by which the search windows will be estimated
  std::vector<int> vStationIDs = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  wf.SetStationIndexes(vStationIDs);

  // Set target (NOTE: for now x and y of the target should be 0.)
  wf.SetTarget(0., 0., -44.);

  // Additional cut
  //TCut extraCut = "abs(pdg) == 211";  // Select pions only
  //wf.SetExtraCut(extraCut);

  // ----- Define input trees with MC triplets
  auto vsInputTreeFilenames = SplitString(inputTreeFile);
  for (const auto& inputTreeFilename : vsInputTreeFilenames) {
    wf.AddInputFile(inputTreeFilename);
  }

  // ----- Run the windows estimator
  wf.Process();

  // ----- Save a log to pdf files
  wf.DumpCanvasesToPdf(logOutputName);
}


// *********************
// ** Implementations **
// *********************

std::vector<TString> SplitString(const char* aString, char token)
{
  TString sOrigin    = TString(aString);
  TObjArray* strings = sOrigin.Tokenize(TString(token));

  std::vector<TString> res;
  for (int i = 0; i < strings->GetEntries(); ++i) {
    res.push_back(((TObjString*) strings->At(i))->String());
  }
  return res;
}
