/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerCore.h
/// @brief  Core class of the QA checking framework (declaration)
/// @author S. Zharko <s.zharko@gsi.de>
/// @date   06.02.2023

#ifndef CbmQaCheckerCore_h
#define CbmQaCheckerCore_h 1

#include "CbmQaCheckerObjectDB.h"
#include "CbmQaCheckerTypedefs.h"

#include "TFile.h"

#include <bitset>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace cbm::qa::checker
{
  /// @brief Core class for CBM QA checker framework (declaration)
  ///
  /// Class CbmQaCheckerCore defines a core of the QA checker framework and provides a user interface for
  /// the comparison routine execution
  ///
  class Core {
  public:
    /// @brief Default ctor
    Core();

    /// @brief Destructor
    ~Core() = default;

    /// @brief Copy constructor
    Core(const Core&) = delete;

    /// @brief Move constructor
    Core(Core&&) = delete;

    /// @brief Copy assignment operator
    Core& operator=(const Core&) = delete;

    /// @brief Move assignment operator
    Core& operator=(Core&&) = delete;

    // ----- User interface
    /// @brief Adds a version of QA output for a comparison
    /// @param version    Label of the version
    /// @param path       Path to the QA output directory for this version
    void AddVersion(const char* version, const char* path);

    /// @brief Adds a dataset name
    /// @param datasetName  Name of dataset
    void AddDataset(const char* datasetName);

    /// @brief Runs checking routine
    /// @param opt  Option:
    ///             "B": suppress canvas creation
    ///             "P": enables bin-by-bin comparison
    ///             "S": enables statistical hypothesis test, where is possible
    ///             "U": enables interval comparison, where is possible
    void Process(Option_t* opt = "P");

    /// @brief Registers root-file for storing output
    /// @param filename  Name of file
    void RegisterOutFile(const char* filename);

    /// @brief Scans comparison results
    /// @return  Comparison flag:
    ///          - true:  All histograms are the same under conditions
    ///          - false: Some of the histograms were changed
    bool Scan() const;

    /// @brief Sets control flag
    /// @param flag   Flag label
    /// @param value  Flag value
    void SetControlBitFlag(EFlagBit flag, bool value = true) { fControlBits[static_cast<int>(flag)] = value; }

    /// @brief  Sets default version label
    /// @param  defaultLabel  Name of default label
    ///
    /// If the default version is not provided, the first version will be used as the default one.
    void SetDefaultVersion(const char* defaultLabel) { fpObjDB->SetDefaultLabel(defaultLabel); }

    /// @brief Sets checker configuration from YAML file
    /// @param configName  Name of YAML configuration file
    void SetFromYAML(const char* configName);

    /// @brief Sets root path to input files
    /// @param pathName  Relative or absolute root path to input the input directories
    void SetInputRootPath(const char* pathName) { fpObjDB->SetInputRootPath(pathName); }

  private:
    std::shared_ptr<TFile> fpOutFile  = nullptr;  ///< Output file
    std::shared_ptr<ObjectDB> fpObjDB = nullptr;  ///< Database of names

    FlagBitSet_t fControlBits;  ///< Control bit flags
  };
}  // namespace cbm::qa::checker

#endif  // CbmQaCheckerCore_h
