/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerFileHandler.h
/// @brief  A handler class to process versions from similar files (declaration)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  08.02.2023

#ifndef CbmQaCheckerFileHandler_h
#define CbmQaCheckerFileHandler_h 1

#include "CbmQaCheckerObjectDB.h"
#include "CbmQaCheckerTypedefs.h"

#include "TClonesArray.h"

#include <memory>
#include <vector>

class TNamed;
class TDirectory;
class TFile;

namespace cbm::qa::checker
{
  /// @brief Handler for single files, created from different QA versions
  ///
  class FileHandler {
  public:
    /// @brief Constructor
    /// @param pObjDB    Shared pointer to object database
    /// @param pOutDir   Directory to store the output  (/dataset/file)
    /// @param iDataset  Index of dataset
    /// @param iFile     Index of file
    FileHandler(std::shared_ptr<ObjectDB>& pObjDB, TDirectory* pOutDir, int iDataset, int iFile);

    /// @brief Destructor
    ~FileHandler();

    /// @brief Copy constructor
    FileHandler(const FileHandler&) = delete;

    /// @brief Move constructor
    FileHandler(FileHandler&&) = delete;

    /// @brief Copy assignment operator
    FileHandler& operator=(const FileHandler&) = delete;

    /// @brief Move assignment operator
    FileHandler& operator=(FileHandler&&) = delete;

    /// @brief  Gets index of dataset
    /// @return Index of dataset
    int GetDatasetID() const { return fDatasetID; }

    /// @brief  Gets index of file
    /// @return Index of file
    int GetFileID() const { return fFileID; }

    /// @brief Processes comparison
    /// @param opt  Option:
    ///             "B": suppress canvas creation
    ///             "P": enables bin-by-bin comparison
    ///             "S": enables statistical hypothesis test, where is possible
    ///             "U": enables interval comparison
    void Process(Option_t* opt = "");

  private:
    /// @brief Creates nested directory from a given path
    /// @param path  Path, parts of which are separated with slash
    /// @return  Pointer to created TDirectory object
    TDirectory* CreateNestedDirectory(const std::string& path);

    /// @brief Iteratively reads object from file by the provided full path to the object
    /// @param pFile  Pointer to TFile instance
    /// @param path   Full path to the object inside the file
    /// @return A pointer to TNamed object
    TNamed* ReadObjectFromFile(TFile* pFile, const std::string& path) const;


    int fFileID    = -1;  ///< Index of file
    int fDatasetID = -1;  ///< Index of dataset

    std::shared_ptr<ObjectDB> fpObjDB          = nullptr;  ///< Pointer to object database
    TDirectory* fpOutDir                       = nullptr;  ///< Pointer to output directory
    std::unique_ptr<TClonesArray> fpInputFiles = nullptr;  ///< Pointer to input files array

    // TODO: replace fpOutputFolder with shared_ptr
  };
}  // namespace cbm::qa::checker

#endif  // CbmQaCheckerFileHandler_h
