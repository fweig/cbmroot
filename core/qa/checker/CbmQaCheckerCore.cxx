/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerCore.cxx
/// @brief  Core class of the QA checking framework (implementation)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  06.02.2023

#include "CbmQaCheckerCore.h"

#include "CbmQaCheckerFileHandler.h"

#include "Logger.h"

#include "TClonesArray.h"
#include "TFile.h"
#include "TFolder.h"
//#include <boost/filesystem.hpp>
#include <regex>

using cbm::qa::checker::Core;

// ---------------------------------------------------------------------------------------------------------------------
//
Core::Core()
{
  // Define object names data base
  fpObjDB = std::make_shared<ObjectDB>();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void Core::AddVersion(const char* version, const char* pathName) { fpObjDB->AddVersion(version, pathName); }

// ---------------------------------------------------------------------------------------------------------------------
//
void Core::AddDataset(const char* dataset) { fpObjDB->AddDataset(dataset); }

// ---------------------------------------------------------------------------------------------------------------------
//
void Core::RegisterOutFile(const char* filename)
{
  LOG(info) << "Core: Registering output file: " << filename;
  fpOutFile = std::make_unique<TFile>(filename, "RECREATE");
}

// ---------------------------------------------------------------------------------------------------------------------
//
void Core::Process(Option_t* opt)
{
  // ----- Init the object database
  fpObjDB->Init();

  // ----- Register output file
  if (fpOutFile == nullptr) { this->RegisterOutFile("QaCheckerOutput.root"); }

  // ----- Process datasets and files
  int nDatasets = fpObjDB->GetNofDatasets();
  std::vector<TFolder*> vDSFolders(nDatasets, nullptr);


  for (int iDS = 0; iDS < nDatasets; ++iDS) {
    // Create a new folder for dataset (/dataset)
    auto* pDSDir = fpOutFile->mkdir(fpObjDB->GetDataset(iDS).data());

    // Loop over files
    for (int iFile = 0; iFile < fpObjDB->GetNofFiles(); ++iFile) {
      // Define output folder for file (/dataset/file)
      auto* pFileDir = pDSDir->mkdir(fpObjDB->GetFileLabel(iFile).data());

      // Create and process a file handler
      auto pFileHandler = std::make_unique<FileHandler>(fpObjDB, pFileDir, iDS, iFile);
      pFileHandler->Process(opt);
    }  // iFile
  }    // iDS
}

// ---------------------------------------------------------------------------------------------------------------------
//
void Core::SetFromYAML(const char* configName) { fpObjDB->ReadFromYAML(configName); }

// ---------------------------------------------------------------------------------------------------------------------
//
bool Core::Scan() const
{
  LOG(info) << "Core: Scanning comparison result ...";
  bool res = true;  /// Equal
  for (int iFile = 0; iFile < fpObjDB->GetNofFiles(); ++iFile) {
    for (int iObj = 0; iObj < fpObjDB->GetNofObjects(iFile); ++iObj) {
      for (int iDS = 0; iDS < fpObjDB->GetNofDatasets(); ++iDS) {
        for (int iVer = 0; iVer < fpObjDB->GetNofVersions(); ++iVer) {
          // For now we check, if anything has been changed with respect to default. Later one can add a flag to test
          // compatibility for different levels (for example, two histograms could be obtained from different systems,
          // or different seeds, but still be consistent).
          if (fpObjDB->GetCmpResult(iDS, iFile, iObj, iVer)) {
            LOG(info) << "\t file: " << fpObjDB->GetInputFileName(iVer, iFile, iDS)
                      << ", object: " << fpObjDB->GetObject(iFile, iObj);
            res = false;
          }
        }  // iVer
      }    // iDS
    }      // iObj
  }        // iFile
  LOG(info) << "Core: Scanning comparison result ... done";

  return res;
}
