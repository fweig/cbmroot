/** @file CbmSourceTsArchive.cxx
 ** @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
 ** @license SPDX-License-Identifier: GPL-3.0-only
 ** @author Volker Friese [originator]
 **/


#include "CbmSourceTsArchive.h"

#include <TimesliceMultiInputArchive.hpp>
#include <TimesliceMultiSubscriber.hpp>

#include <FairSource.h>
#include <Logger.h>


using fles::Timeslice;
using std::string;
using std::unique_ptr;


// -----   Constructor   ------------------------------------------------------
CbmSourceTsArchive::CbmSourceTsArchive(const char* fileName) { AddInputFile(fileName); }
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

  switch (fCbmSourceType) {
    // Use again when kFILE does not skipp the first TS by default anymore
    // case Source_Type::kONLINE: {
    case eCbmSourceType::kOnline: {
      // Create a ";" separated string with all host/port combinations
      // Build a semicolon-separated list of file names for TimesliceMultiInputArchive
      string fileList;
      for (const auto& fileName : fFileNames) {
        fileList += fileName;
        fileList += ";";
      }
      fileList.pop_back();  // Remove last semicolon after last file name

      fTsSource = new fles::TimesliceMultiSubscriber(fileList, fHighWaterMark);

      /// Initialize the Multisubscriber
      /// (This restores the original behavior after modifications needed to make the MQ version
      dynamic_cast<fles::TimesliceMultiSubscriber*>(fTsSource)->InitTimesliceSubscriber();

      if (!fTsSource) {
        LOG(fatal) << "Could not connect to the TS publisher.";
        return kFALSE;
      }
      break;
    }
    // Se above
    // case Source_Type::kFILE: {
    case eCbmSourceType::kOffline: {
      // Return error for empty file list and an offline run
      if (fFileNames.empty()) return kFALSE;


      // Build a semicolon-separated list of file names for TimesliceMultiInputArchive
      string fileList;
      for (const auto& fileName : fFileNames) {
        fileList += fileName;
        fileList += ";";
      }
      fileList.pop_back();  // Remove last semicolon after last file name

      // Create the proper TS source
      fTsSource = new fles::TimesliceMultiInputArchive(fileList);
      if (!fTsSource) {
        LOG(error) << "SourceTsArchive: Failed to create TSMultiInputArchive!";
        return kFALSE;
      }
      break;
    }
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
