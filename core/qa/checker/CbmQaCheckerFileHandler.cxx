/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerFileHandler.cxx
/// @brief  A handler class to process versions from similar files (implementation)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  08.02.2023

#include "CbmQaCheckerFileHandler.h"

#include "CbmQaCheckerHist1DHandler.h"
#include "CbmQaCheckerHist2DHandler.h"
#include "CbmQaCheckerProfile1DHandler.h"

#include "Logger.h"

#include "TDirectoryFile.h"
#include "TFile.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH2.h"
#include "TNamed.h"
#include "TProfile.h"

#include <boost/algorithm/string.hpp>

#include <iomanip>
#include <sstream>
#include <string>


using cbm::qa::checker::FileHandler;

// ---------------------------------------------------------------------------------------------------------------------
//
FileHandler::FileHandler(std::shared_ptr<ObjectDB>& pObjDB, TDirectory* pOutDir, int iDataset, int iFile)
  : fFileID(iFile)
  , fDatasetID(iDataset)
  , fpObjDB(pObjDB)
  , fpOutDir(pOutDir)
{
  fpInputFiles = std::make_unique<TClonesArray>("TFile");

  for (int iVer = 0; iVer < fpObjDB->GetNofVersions(); ++iVer) {
    std::string filename = fpObjDB->GetInputFileName(iVer, fFileID, fDatasetID);
    auto* pInFile        = new ((*fpInputFiles)[iVer]) TFile(filename.data(), "READONLY");
    LOG(info) << "File: " << pInFile->GetName();
    LOG_IF(fatal, !pInFile->IsOpen()) << "FileHandler: file " << filename << " cannot be opened";
  }

  // Check registered folder
  LOG_IF(fatal, !fpObjDB.get()) << "FileHandler: attempt to register a null pointer for the object database";
  LOG_IF(fatal, !fpOutDir) << "FileHandler: attempt to register a null pointer for the output directory";
  LOG_IF(fatal, fDatasetID < 0) << "FileHandler: attempt to register undefined dataset index";
  LOG_IF(fatal, fFileID < 0) << "FileHandler: attempt to register undefined file index";
}

// ---------------------------------------------------------------------------------------------------------------------
//
FileHandler::~FileHandler()
{
  // ----- Clean pointers
  fpInputFiles->Delete();
  fpInputFiles = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
//
TDirectory* FileHandler::CreateNestedDirectory(const std::string& path)
{
  fpOutDir->mkdir(path.data());
  return fpOutDir->GetDirectory(path.data());
}

// ---------------------------------------------------------------------------------------------------------------------
//
void FileHandler::Process(Option_t* opt)
{
  int nObjects  = fpObjDB->GetNofObjects(fFileID);
  int nVersions = fpObjDB->GetNofVersions();

  // ----- Initial checks
  if (!nObjects) {
    LOG(warn) << "FileHandler: No objects were passed to file \"" << nObjects << ". Skipping file";
    return;
  }

  // ----- Option parsing
  std::string sOption = opt;
  for (auto& ch : sOption) {
    ch = std::tolower(ch);
  }
  bool bSuppressCanvases = sOption.find("b") != std::string::npos;
  bool bCmpPointByPoint  = sOption.find("p") != std::string::npos;
  bool bCmpStatAny       = sOption.find("s") != std::string::npos;

  LOG(info) << "FileHandler: processing objects: ";
  std::vector<TNamed*> vpObjects(nVersions, nullptr);  // vector to keep object different versions
  for (int iObj = 0; iObj < nObjects; ++iObj) {
    bool skipObj = false;
    for (int iVer = 0; iVer < nVersions; ++iVer) {
      auto* pInputFile = static_cast<TFile*>(fpInputFiles->At(iVer));
      vpObjects[iVer]  = ReadObjectFromFile(pInputFile, fpObjDB->GetObject(fFileID, iObj));
      if (!vpObjects[iVer]) {
        LOG(warn) << "FileHandler: object " << fpObjDB->GetObject(fFileID, iObj) << " is undefined for version "
                  << fpObjDB->GetVersionLabel(iVer) << ". This object will be skipped";
        skipObj = true;
      }
    }  // iVer
    if (skipObj) { continue; }

    // Create an instance of an object handler
    std::unique_ptr<ObjectHandler> pObjHandler = nullptr;
    LOG(info) << "FileHandler: processing object \"" << vpObjects[0]->GetName() << '\"';
    if (dynamic_cast<TH2*>(vpObjects[0])) {
      pObjHandler = std::make_unique<Hist2DHandler>(iObj, fFileID, fDatasetID);
      if (bCmpPointByPoint) {
        pObjHandler->SetBitFlag(Hist1DHandler::EFlags::kPOINT);  // Compare point-by-point (exact equality)
      }
      if (bCmpStatAny) {
        pObjHandler->SetBitFlag(Hist1DHandler::EFlags::kCHI2);  // Compare with chi2 test
        pObjHandler->SetBitFlag(Hist1DHandler::EFlags::kKOLM);  // Compare with Kolmogorov test
      }
    }
    else if (dynamic_cast<TProfile*>(vpObjects[0])) {
      pObjHandler = std::make_unique<Profile1DHandler>(iObj, fFileID, fDatasetID);
      if (bCmpPointByPoint) {
        pObjHandler->SetBitFlag(Hist1DHandler::EFlags::kPOINT);  // Compare point-by-point (exact equality)
      }
      if (bCmpStatAny) {
        pObjHandler->SetBitFlag(Hist1DHandler::EFlags::kCHI2);  // Compare with chi2 test
        pObjHandler->SetBitFlag(Hist1DHandler::EFlags::kKOLM);  // Compare with Kolmogorov test
      }
    }
    else if (dynamic_cast<TH1*>(vpObjects[0])) {
      pObjHandler = std::make_unique<Hist1DHandler>(iObj, fFileID, fDatasetID);
      if (bCmpPointByPoint) {
        pObjHandler->SetBitFlag(Hist1DHandler::EFlags::kPOINT);  // Compare point-by-point (exact equality)
      }
      if (bCmpStatAny) {
        pObjHandler->SetBitFlag(Hist1DHandler::EFlags::kCHI2);  // Compare with chi2 test
        pObjHandler->SetBitFlag(Hist1DHandler::EFlags::kKOLM);  // Compare with Kolmogorov test
      }
    }
    else {
      LOG(warn) << "FileHandler: Object " << fpObjDB->GetObject(fFileID, iObj) << " has a type \""
                << vpObjects[0]->ClassName()
                << "\", which is unknown to the cbm::qa::checker framework, it will be skipped";
      continue;
    }
    pObjHandler->SetObjectDB(fpObjDB);
    pObjHandler->SetOutputDirectory(CreateNestedDirectory((fpObjDB->GetObject(fFileID, iObj))));
    pObjHandler->AddObjects(vpObjects);
    pObjHandler->CompareWithDefault();

    // Create comparison canvas
    if (!bSuppressCanvases) {
      bool areDifferent = false;
      for (int iVer = 0; iVer < nVersions; ++iVer) {
        if (fpObjDB->GetCmpResult(fDatasetID, fFileID, iObj, iVer)) { areDifferent = true; }
      }
      if (areDifferent) { pObjHandler->CreateCanvases(); }
    }
    pObjHandler->Write();
  }  // iObj
}

// ---------------------------------------------------------------------------------------------------------------------
//
TNamed* FileHandler::ReadObjectFromFile(TFile* pFile, const std::string& path) const
{
  TObject* pObj = pFile;
  size_t iPos   = 0;  // Index of first symbol
  size_t iNext  = 0;  // Index of last symbol
  // Read object iteratively, running throug directories or folders, provided by path
  while (iPos < path.size()) {
    // get name
    iNext = path.find_first_of('/', iPos);
    if (iNext > path.size()) { iNext = path.size(); }
    std::string part = path.substr(iPos, iNext - iPos);  // short name of next object/directory/folder in the path
    iPos             = iNext + 1;
    if (!part.size()) { continue; }

    // test TDirectoryFile
    if (dynamic_cast<TDirectoryFile*>(pObj)) {
      auto* pDir = static_cast<TDirectoryFile*>(pObj);
      pObj       = pDir->FindObjectAny(part.data());
    }
    // test TFolder
    else if (dynamic_cast<TFolder*>(pObj)) {
      auto* pDir = static_cast<TFolder*>(pObj);
      pObj       = pDir->FindObjectAny(part.data());
    }
  }
  return dynamic_cast<TNamed*>(pObj);
}
