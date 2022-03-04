/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese[committer] */

#include "CbmSourceTs.h"

#include <TimesliceAutoSource.hpp>

#include <FairSource.h>
#include <Logger.h>


using fles::Timeslice;
using std::string;
using std::vector;


// -----   Constructor   ------------------------------------------------------
CbmSourceTs::CbmSourceTs(const char* fileName) { AddInputFile(fileName); }
// ----------------------------------------------------------------------------

// -----   Constructor   ------------------------------------------------------
CbmSourceTs::CbmSourceTs(vector<string> fileNames) : fFileNames(fileNames) {}
// ----------------------------------------------------------------------------


// -----   Add an input file   ------------------------------------------------
size_t CbmSourceTs::AddInputFile(const char* fileName)
{
  string sFile(fileName);
  if (sFile.size()) fFileNames.push_back(sFile);
  return fFileNames.size();
}
// ----------------------------------------------------------------------------


// -----   Close   -----------------------------------------------------------
void CbmSourceTs::Close() {}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
Bool_t CbmSourceTs::Init()
{
  LOG(info) << "SourceTs: Creating TimesliceSource with " << fFileNames.size() << " input files.";
  fFlesSource = new fles::TimesliceAutoSource(fFileNames);
  return kTRUE;
}
// ----------------------------------------------------------------------------


// -----   Read one time slice from archive   ---------------------------------
Int_t CbmSourceTs::ReadEvent(UInt_t)
{
  fFlesTs = fFlesSource->get();
  if (!fFlesTs) {
    LOG(info) << "SourceTs: End of archive reached; stopping run.";
    return 1;
  }
  LOG(info) << "SourceTs: Reading time slice " << fNumTs << " (index " << fFlesTs->index() << ")";
  fNumTs++;
  return 0;
}
// ----------------------------------------------------------------------------


ClassImp(CbmSourceTs)
