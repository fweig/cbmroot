/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland, Volker Friese[committer], Pierre-Alain Loizeau */

#include "CbmSourceTsArchive.h"

#include <TimesliceAutoSource.hpp>

#include <FairSource.h>
#include <Logger.h>


using fles::Timeslice;
using std::string;
using std::unique_ptr;


// -----   Constructor   ------------------------------------------------------
CbmSourceTsArchive::CbmSourceTsArchive(const char* fileName) { AddInputFile(fileName); }
// ----------------------------------------------------------------------------

// -----   Constructor   ------------------------------------------------------
CbmSourceTsArchive::CbmSourceTsArchive(std::vector<std::string> fileNames) : fFileNames(fileNames) {}
// ----------------------------------------------------------------------------


// -----   Add an input file   ------------------------------------------------
size_t CbmSourceTsArchive::AddInputFile(const char* fileName)
{
  string sFile(fileName);
  if (sFile.size()) fFileNames.push_back(sFile);
  return fFileNames.size();
}
// ----------------------------------------------------------------------------


// -----   Close   -----------------------------------------------------------
void CbmSourceTsArchive::Close()
{
  LOG(info) << "SourceTsArchive::Close() Let's hear some famous last words: ";
  fUnpack.Finish();
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
Bool_t CbmSourceTsArchive::Init()
{
  if (1 == fFileNames.size()) {
    LOG(info) << "SourceTsArchive::Init() calling string constructor with ";
    LOG(info) << fFileNames[0];
    fTsSource = new fles::TimesliceAutoSource(fFileNames[0]);
  }
  else {
    LOG(info) << "SourceTsArchive::Init() calling vector constructor with size ";
    LOG(info) << fFileNames.size();
    fTsSource = new fles::TimesliceAutoSource(fFileNames);
  }

  // Initialise unpacker
  fUnpack.Init();
  LOG(info) << "Source: Init done";
  return kTRUE;
}
// ----------------------------------------------------------------------------


// -----   Read one time slice from archive   ---------------------------------
Int_t CbmSourceTsArchive::ReadEvent(UInt_t)
{

  unique_ptr<Timeslice> ts;
  ts = fTsSource->get();

  if (!ts) {
    LOG(info) << "SourceTsArchive: End of archive reached; stopping run.";
    return 1;
  }
  LOG(info) << "SourceTsArchive: Reading time slice " << fTsCounter << " (index " << ts->index() << ")";

  fUnpack.Unpack(std::move(ts));


  fTsCounter++;
  return 0;
}
// ----------------------------------------------------------------------------


ClassImp(CbmSourceTsArchive)
