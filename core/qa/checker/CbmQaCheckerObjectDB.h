/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerObjectDB.h
/// @brief  Database for processed objects in the QA checker framework (header)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  15.02.2023

#ifndef CbmQaCheckerObjectDB_h
#define CbmQaCheckerObjectDB_h 1

#include "CbmQaCheckerTypedefs.h"

#include "Logger.h"

#include <string>
#include <utility>
#include <vector>

#include <yaml-cpp/yaml.h>

// TMP
#include <boost/core/demangle.hpp>

#include <typeinfo>
template<typename T>
std::string type_str()
{
  return boost::core::demangle(typeid(T).name());
}

namespace cbm::qa::checker
{
  /// @brief A data base class for processed objects
  ///
  /// The data base contains information on datasets, file-object pairs and versions, which are compared.
  class ObjectDB {
  public:
    /// @brief Default constructor
    ObjectDB() = default;

    /// @brief Destructor
    ~ObjectDB() = default;

    /// @brief Copy constructor
    ObjectDB(const ObjectDB&) = delete;

    /// @brief Move constructor
    ObjectDB(ObjectDB&&) = delete;

    /// @brief Copy assignment operator
    ObjectDB& operator=(const ObjectDB&) = delete;

    /// @brief Move assignment operator
    ObjectDB& operator=(ObjectDB&&) = delete;

    /// @brief Adds version
    /// @param label      Label of version
    /// @param path       Path to output files made for this version
    void AddVersion(const char* label, const char* path);

    /// @brief Adds dataset
    /// @param dataset  Name of dataset
    void AddDataset(const char* dataset);

    /// @brief Clears content
    void Clear();

    /// @brief Gets name of dataset
    /// @param iDataset  Index of dataset
    /// @return  Name of dataset
    const auto& GetDataset(int iDataset) const { return fvDatasets[iDataset]; }

    /// @brief Gets reference to dataset name array
    const auto& GetDatasets() const { return fvDatasets; }

    /// @brief Gets index of default version
    /// @return  Index of default version
    int GetDefaultID() const { return fDefVersionID; }

    /// @brief Gets label of the default version
    const std::string& GetDefaultLabel() const
    {
      assert(fDefVersionID > -1);
      return GetVersionLabel(fDefVersionID);
    }

    /// @brief Gets label of file
    /// @param iFile  Index of file
    /// @return  Label of file
    const auto& GetFileLabel(int iFile) const { return fvFileLabels[iFile]; }

    /// @brief Gets iterator range for object names stored in a file
    /// @note  The iterator_range object behaves itself just like an ordinary STL container.
    /// @param iFile  Index of file
    /// @return  iterator_range for ROOT objects of this file
    /// TODO: Understand, which type is deduced
    auto GetObjects(int iFile) const
    {
      auto itBegin = std::upper_bound(fvGlobalToFileObject.begin(), fvGlobalToFileObject.end(), iFile,
                                      [](int i, const std::pair<int, int>& p) { return i <= p.first; });
      auto itEnd   = std::lower_bound(itBegin, fvGlobalToFileObject.end(), iFile,
                                    [](const std::pair<int, int>& p, int i) { return i >= p.first; });

      int iBegin = itBegin - fvGlobalToFileObject.begin();
      int iEnd   = itEnd - fvGlobalToFileObject.begin();
      return boost::make_iterator_range(fvObjects.begin() + iBegin, fvObjects.begin() + iEnd);
    }

    /// @brief Gets comparison result
    /// @param iDS    Index of dataset
    /// @param iFile  Index of file
    /// @param iObj   Index of object within the file
    /// @param iVer   Index of version
    /// @return  Value of comparison result
    CmpResult_t GetCmpResult(int iDS, int iFile, int iObj, int iVer) const
    {
      int iObjGlob = GetObjects(iFile).begin() - fvObjects.begin() + iObj;
      int iRes     = iVer + fvVersionLabels.size() * (iObjGlob + iDS * fvObjects.size());
      return fvCmpResults[iRes];
    }

    /// @brief Gets name of file from indexes of version, file and dataset
    /// @param iVersion  Index of version
    /// @param iFile     Index of file
    /// @param iDataset  Index of dataset
    /// @return  Name of input file
    std::string GetInputFileName(int iVersion, int iFile, int iDataset) const;

    /// @brief Gets object name by its local index and index of file
    /// @param iFile    Index of file
    /// @param iObject  Index of object for a given file
    /// @return  Name of object
    const std::string& GetObject(int iFile, int iObject) const { return GetObjects(iFile)[iObject]; }

    /// @brief Gets number of datasets
    int GetNofDatasets() const { return fvDatasets.size(); }

    /// @brief Gets total number of objects
    int GetNofObjects() const { return fvObjects.size(); }

    /// @brief Gets number of objects in file
    /// @param iFile  Index of file
    int GetNofObjects(int iFile) const { return GetObjects(iFile).size(); }

    /// @brief Gets number of files
    int GetNofFiles() const { return fvFiles.size(); }

    /// @brief Gets number of versions
    int GetNofVersions() const { return fvVersionLabels.size(); }

    /// @brief Gets version label
    /// @param iVersion  Index of version
    const std::string& GetVersionLabel(int iVersion) const { return fvVersionLabels[iVersion]; }

    /// @brief Gets reference to version label array
    const auto& GetVersionLabels() const { return fvVersionLabels; }

    /// @brief Gets version path
    /// @param iVersion  Index of version
    const std::string& GetVersionPath(int iVersion) const { return fvVersionPaths[iVersion]; }

    /// @brief Gets reference to version path array
    const auto& GetVersionPaths() const { return fvVersionPaths; }

    /// @brief Initializes the database
    void Init();

    /// @brief Reads DB from YAML node
    /// @param config Root node of the YAML file
    void ReadFromYAML(const char* configName);

    /// @brief Saves content to string
    /// @return A string representation of the DB contents
    std::string ToString() const;

    /// @brief Sets comparison result
    /// @param iDS    Index of dataset
    /// @param iFile  Index of file
    /// @param iObj   Index of object within the file
    /// @param iVer   Index of version
    /// @param value  Value of comparison result
    void SetCmpResult(int iDS, int iFile, int iObj, int iVer, CmpResult_t value)
    {
      int iObjGlob       = GetObjects(iFile).begin() - fvObjects.begin() + iObj;
      int iRes           = iVer + fvVersionLabels.size() * (iObjGlob + iDS * fvObjects.size());
      fvCmpResults[iRes] = value;
    }

    /// @brief  Sets default version label
    /// @param  defaultLabel  Name of default label
    ///
    /// If the default version is not provided as well as the provided, the first version will be used as the
    /// default one.
    void SetDefaultLabel(const char* defaultLabel) { fsDefaultLabel = defaultLabel; }

    /// @brief Sets root path to input files
    /// @param pathName  Relative or absolute root path to input the input directories
    void SetInputRootPath(const char* pathName) { fsInputRootPath = pathName; }

  private:
    int fDefVersionID = -1;  ///< Index of default version

    std::string fsInputRootPath = "";  ///< Root path for input files
    std::string fsDefaultLabel  = "";  ///< Name of default version label

    std::vector<std::string> fvDatasets;                    ///< Container of dataset names
    std::vector<std::string> fvFiles;                       ///< Container of file names
    std::vector<std::string> fvFileLabels;                  ///< Container of file labels (used in output)
    std::vector<std::string> fvObjects;                     ///< Container of object names (joint for all the files)
    std::vector<std::string> fvVersionLabels;               ///< Container of version labels
    std::vector<std::string> fvVersionPaths;                ///< Container of version paths
    std::vector<std::pair<int, int>> fvGlobalToFileObject;  ///< Map of global obj. index -> local file-object
    std::vector<CmpResult_t> fvCmpResults;                  ///< Comparison results vs. dataset, object and version
  };
}  // namespace cbm::qa::checker

#endif  // CbmQaCheckerObjectDB_h
