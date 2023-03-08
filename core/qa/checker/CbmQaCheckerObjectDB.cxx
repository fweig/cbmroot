/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerObjectDB.h
/// @brief  Database for processed objects in the QA checker framework (implementation)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  15.02.2023

#include "CbmQaCheckerObjectDB.h"

#include "Logger.h"

#include <algorithm>
#include <regex>
#include <sstream>

#include <yaml-cpp/yaml.h>

using cbm::qa::checker::ObjectDB;

// ---------------------------------------------------------------------------------------------------------------------
//
void ObjectDB::Clear()
{
  fvDatasets.clear();
  fvFiles.clear();
  fvFileLabels.clear();
  fvObjects.clear();
  fvVersionLabels.clear();
  fvVersionPaths.clear();
  fvGlobalToFileObject.clear();
  fvCmpResults.clear();
}

// ---------------------------------------------------------------------------------------------------------------------
//
// ObjectDB::GetObjects(int iFile) const
// {
//   auto itBegin = std::upper_bound(fvGlobalToFileObject.begin(), fvGlobalToFileObject.end(), iFile,
//     [](int i, const std::pair<int, int>& p) { return i <= p.first; });
//   auto itEnd   = std::lower_bound(itBegin, fvGlobalToFileObject.end(), iFile,
//     [](const std::pair<int, int>& p, int i) { return i >= p.first; });
//
//   int iBegin = itBegin - fvGlobalToFileObject.begin();
//   int iEnd   = itEnd - fvGlobalToFileObject.begin();
//   return boost::make_iterator_range(fvObjects.begin() + iBegin, fvObjects.begin() + iEnd);
// }

// ---------------------------------------------------------------------------------------------------------------------
//
void ObjectDB::AddVersion(const char* label, const char* path)
{
  fvVersionLabels.push_back(label);
  fvVersionPaths.push_back(path);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void ObjectDB::AddDataset(const char* dataset) { fvDatasets.push_back(dataset); }

// ---------------------------------------------------------------------------------------------------------------------
//
std::string ObjectDB::GetInputFileName(int iVersion, int iFile, int iDataset) const
{
  std::string res = fvFiles[iFile];
  res             = std::regex_replace(res, std::regex("\\%v"), fvVersionPaths[iVersion]);
  res             = std::regex_replace(res, std::regex("\\%d"), fvDatasets[iDataset]);
  return res;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void ObjectDB::Init()
{
  // ----- Check consistency of input values
  LOG_IF(fatal, !GetNofObjects()) << "ObjectDB: No objects were passed to the checker";
  LOG_IF(fatal, GetNofDatasets() < 1) << "ObjectDB: No datasets were found, at least one dataset should be provided";
  LOG_IF(fatal, GetNofVersions() < 2) << "ObjectDB: File handler should have at least two versions to compare ("
                                      << GetNofVersions() << " were provided)";


  // ----- Define default version index
  if (fsDefaultLabel.size()) {
    auto it = std::find(fvVersionLabels.cbegin(), fvVersionLabels.cend(), fsDefaultLabel);
    if (it == fvVersionLabels.cend()) {
      std::stringstream msg;
      msg << "ObjectDB: registered default label \"" << fsDefaultLabel << "\" is not found among the version labels:\n";
      for (const auto& label : fvVersionLabels) {
        msg << "\t- " << label << '\n';
      }
      LOG(fatal) << msg.str();
    }
    fDefVersionID = it - fvVersionLabels.cbegin();
  }
  else {
    fDefVersionID = 0;
    LOG(warn) << "ObjectDB: default version was not registered. Using the first version as the default one (\""
              << fvVersionLabels[fDefVersionID] << "\")";
  }
  LOG(info) << this->ToString();

  // ----- Reserve space for object comparison results
  fvCmpResults.resize(fvVersionLabels.size() * fvObjects.size() * fvDatasets.size());

  // ----- Add root path of input, if it were defined
  auto regexSlashes = std::regex("(/+)");  // regular expression for a sequence of consecutive slashes
  for (auto& path : fvVersionPaths) {
    if (fsInputRootPath.size()) { path = fsInputRootPath + "/" + path; }
    path = std::regex_replace(path, regexSlashes, "/");  // replace all consecutive slashes with a single one
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void ObjectDB::ReadFromYAML(const char* configName)
{
  // ----- Open input file
  YAML::Node config;
  try {
    config = YAML::LoadFile(configName)["checker"];
  }
  catch (const YAML::BadFile& exc) {
    LOG(fatal) << "ObjectDB: configuration file " << configName << " does not exist";
  }
  catch (const YAML::ParserException& exc) {
    LOG(fatal) << "ObjectDB: configuration file " << configName << " is badly formatted";
  }

  // ----- Define file-object map
  if (config["files"]) {
    if (fvGlobalToFileObject.size()) {
      LOG(warn) << "ObjectDB: file-object map was defined before. Redefining it from the config file " << configName;
      fvGlobalToFileObject.clear();
      fvFiles.clear();
      fvFileLabels.clear();
      fvObjects.clear();
    }
    try {
      const auto& rootNode = config["files"];

      // Calculate total number of objects and files
      size_t nFiles   = rootNode.size();
      size_t nObjects = 0;
      for (const auto& fileNode : rootNode) {
        nObjects += fileNode["objects"].size();
      }
      fvFiles.reserve(nFiles);
      fvFileLabels.reserve(nFiles);
      fvObjects.reserve(nObjects);
      fvGlobalToFileObject.reserve(nObjects);

      // Fill vectors
      for (const auto& fileNode : rootNode) {
        size_t iObj = 0;
        for (const auto& objectNode : fileNode["objects"]) {
          fvGlobalToFileObject.emplace_back(fvFiles.size(), iObj++);
          fvObjects.push_back(objectNode.as<std::string>());
        }
        fvFiles.push_back(fileNode["name"].as<std::string>());
        fvFileLabels.push_back(fileNode["label"].as<std::string>());
      }
    }
    catch (const YAML::InvalidNode& exc) {
      LOG(fatal) << "ObjectDB: error while reading checker/files node from the config " << configName;
    }
  }
  else {
    LOG(warn) << "ObjectDB: node checker/inputformat is not defined in the config " << configName;
  }

  // ----- Define dataset names
  if (config["datasets"]) {
    LOG_IF(fatal, fvDatasets.size())
      << "ObjectDB: dataset names were defined before. Please, use only one initialisation method:"
      << " either configuration file, either setters from macro";
    try {
      const auto& rootNode = config["datasets"];
      fvDatasets.reserve(rootNode.size());
      for (const auto& datasetNode : rootNode) {
        fvDatasets.push_back(datasetNode.as<std::string>());
      }
    }
    catch (const YAML::InvalidNode& exc) {
      LOG(fatal) << "ObjectDB:: error while reading checker/datasets node from the config " << configName;
    }
  }
  else {
    LOG(warn) << "ObjectDB: node checker/datasets is not defined in the config " << configName;
  }

  // ----- Define version names
  if (config["versions"]) {
    LOG_IF(fatal, fvVersionLabels.size())
      << "ObjectDB: dataset names were defined before. Attempt to redefine dataset names from config " << configName;
    try {
      const auto& rootNode = config["versions"];
      fvVersionLabels.reserve(rootNode.size());
      fvVersionPaths.reserve(rootNode.size());
      for (const auto& versionNode : rootNode) {
        fvVersionLabels.push_back(versionNode["label"].as<std::string>());
        fvVersionPaths.push_back(versionNode["path"].as<std::string>());
      }
    }
    catch (const YAML::InvalidNode& exc) {
      LOG(fatal) << "ObjectDB:: error while reading checker/versions node from the config " << configName;
    }
  }
  else {
    LOG(warn) << "ObjectDB: node checker/versions is not defined in the config " << configName;
  }

  // ----- Define default version
  if (config["default_label"]) {
    try {
      SetDefaultLabel(config["default_label"].as<std::string>().data());
    }
    catch (const YAML::InvalidNode& exc) {
      LOG(fatal) << "ObjectDB:: error while reading checker/default_label node from the config " << configName;
    }
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::string ObjectDB::ToString() const
{
  std::stringstream msg;
  msg << "*** CBM QA-Checker: defined files ****\n\n";

  msg << "----- Versions:\n";
  for (size_t iV = 0; iV < fvVersionLabels.size(); ++iV) {
    if (iV == (size_t) fDefVersionID) {
      msg << "\033[1;32m- " << fvVersionLabels[iV] << " (path: " << fvVersionPaths[iV] << ")\n ---> DEFAULT\033[0m";
    }
    else {
      msg << "- " << fvVersionLabels[iV] << " (path: " << fvVersionPaths[iV] << ")\n";
    }
  }

  msg << "----- Datasets:\n";
  for (const auto& dataset : fvDatasets) {
    msg << "- \033[1m" << dataset << "\033[0m\n";
  }

  msg << "----- Objects\n";
  for (size_t iF = 0; iF < fvFiles.size(); ++iF) {
    msg << "- \033[1m" << fvFiles[iF] << "\033[0m\n";
    for (const auto& object : this->GetObjects(iF)) {
      msg << "\t- " << object << '\n';
    }
  }
  return msg.str();
}
