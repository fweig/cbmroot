/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmQaTask.h
/// \brief  A base class for CBM QA task logic
/// \author S.Zharko <s.zharko@gsi.de>
/// \data   12.01.2022

#ifndef CbmQaTask_h
#define CbmQaTask_h 1

#include "CbmQaTable.h"

#include "FairTask.h"
#include "Logger.h"

#include "TCanvas.h"
#include "TEfficiency.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TParameter.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TProfile3D.h"
#include "TROOT.h"

#include <algorithm>
#include <regex>
#include <string_view>
#include <type_traits>

#include <yaml-cpp/yaml.h>

/// Class CbmQaTask is to be inherited with a particular QA-task. It provides mechanisms for storage and management
/// of QA canvases and histograms management
class CbmQaTask : public FairTask {
public:
  /// Constructor from parameters
  /// \param  name     Name of the task
  /// \param  prefix   Unique prefix for writeable root objects
  /// \param  verbose  Verbose level
  /// \param  isMCUsed Flag: true - MC information is used, false - only reconstructed data QA is processed
  CbmQaTask(const char* name, const char* prefix, int verbose, bool isMCUsed);

  /// Default constructor
  CbmQaTask() = delete;  // TODO: Let's see, what can happen, if one deletes default constructor

  /// Destructor
  virtual ~CbmQaTask() = default;

  // Copy and move semantics
  CbmQaTask(const CbmQaTask&) = delete;
  CbmQaTask(CbmQaTask&&)      = delete;
  CbmQaTask& operator=(const CbmQaTask&) = delete;
  CbmQaTask& operator=(CbmQaTask&&) = delete;

  /// Returns flag, whether MC information is used or not in the task
  bool IsMCUsed() const { return fbUseMC; }

  /// FairTask: Task initialization in the beginning of the run
  InitStatus Init();

  /// FairTask: Task reinitialization
  InitStatus ReInit();

  /// FairTask: Defines action of the task in the event/TS
  void Exec(Option_t* /*option*/);

  /// FairTask: Defines action of the task in the end of run
  void Finish();

  /// @brief Sets task configuration file
  /// @param filename  Name of file
  ///
  /// Sets name of YAML configuration file, which defines parameters needed for the task. The file is assumed
  /// to contain root branch "qa/[task name]". A subbranch "histograms" contains a list of histograms with pre defined
  /// parameters: title, number of bins and axis ranges.
  void SetConfigName(const char* filename) { fsConfigName = filename; }

  ClassDef(CbmQaTask, 0);

protected:
  // *****************************************************
  // ** Functions accessible inside the derived classes **
  // *****************************************************

  /// \brief Method to check, if the QA results are acceptable
  virtual bool Check() = 0;

  /// De-initialize the task
  virtual void DeInit() {}

  /// Initializes data branches
  virtual InitStatus InitDataBranches() { return kSUCCESS; }

  /// Initializes histograms
  virtual InitStatus InitHistograms() { return kSUCCESS; }

  /// Initializes canvases
  virtual InitStatus InitCanvases() { return kSUCCESS; }

  /// Initializes event / time-slice
  virtual InitStatus InitTimeSlice() { return kSUCCESS; }

  /// Method to fill histograms per event or time-slice
  virtual void FillHistograms() {}

  /// Creates, initializes and registers a canvas
  /// \tparam  T     Type of the canvas: TCanvas or CbmQaCanvas (\note T should not be a pointer)
  /// \param   name  Name of canvas
  /// \param   args  The rest of the arguments, which will be passed to the canvas constructor
  template<typename T, typename... Args>
  T* MakeCanvas(const char* name, Args... args);

  /// Creates, initializes and registers an efficiency
  /// \tparam  T     Type of the canvas: either TProfile, TProfile2D or TEfficiency (\note T should not be a pointer)
  /// \param   name  Name of canvas
  /// \param   args  The rest of the arguments, which will be passed to the efficiency constructor
  template<typename T, typename... Args>
  T* MakeEfficiency(const char* name, Args... args);

  /// Creates, initializes and registers a histogram
  /// \tparam  T     Type of the histogram (\note T should not be a pointer)
  /// \param   name  Name of histogram
  /// \param   args  The rest of the arguments, which will be passed to the histogram constructor
  template<typename T, typename... Args>
  T* MakeHisto(const char* name, Args... args);

  /// @brief Creates, initializes and registers a histogram, based on the configuration file
  /// @tparam T    Type of the histogram (@note: should not be a pointer)
  /// @param  name  Name of the histogram, stored in config
  /// @param  id0   First index (optional)
  /// @param  id1   Second index (optional)
  /// @param  id2   Third index (optional)
  /// @return  Pointer to the histogram object
  template<typename T>
  T* MakeHistoFromConfig(const char* name, int id0 = -1, int id1 = -1, int id2 = -1);

  /// Creates, initializes and registers a table
  /// \param  name  Name of the table
  /// \param  args  The rest of the arguments, which will be passed to the table constructor
  template<typename... Args>
  CbmQaTable* MakeTable(const char* name, Args... args);

  /// Get current event number
  int GetEventNumber() const { return fNofEvents.GetVal(); }


  // ***********************
  // ** Utility functions **
  // ***********************

  /// Checks range of variable
  /// \param name  Name of the variable
  /// \param var   Variable to check
  /// \param lo    Lower limit of the variable
  /// \param hi    Upper limit of the variable
  /// \return  False, if variable exits the range
  template<typename T>
  bool CheckRange(const std::string_view& name, const T& var, const T& lo, const T& hi) const;


private:
  /// @brief De-initializes this task
  void DeInitBase();

  // I/O
  std::unique_ptr<TFolder> fpFolderRoot = nullptr;  ///< Root folder to store histograms and canvases
  TFolder* fpFolderHist                 = nullptr;  ///< Folder for raw histograms
  TFolder* fpFolderCanv                 = nullptr;  ///< Folder for canvases
  TFolder* fpFolderEff                  = nullptr;  ///< Folder for efficiencies
  TFolder* fpFolderTable                = nullptr;  ///< Folder for tables

  TString fsPrefix = "";  ///< Unique prefix for all writeable root objects to avoid collisions between different tasks
  bool fbUseMC     = false;  ///< Flag, if MC is used

  TParameter<int> fNofEvents {"nEvents", 0};  ///< Number of processed events

  TString fsConfigName      = "";       ///< Name of YAML configuration file
  YAML::Node* fpCurrentNode = nullptr;  ///< Pointer to current node of the configuration file
};


// *************************************************
// ** Inline and template function implementation **
// *************************************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T>
bool CbmQaTask::CheckRange(const std::string_view& name, const T& var, const T& lo, const T& hi) const
{
  if (std::clamp(var, lo, hi) == lo) {
    LOG(error) << fName << ": " << name << " is found to be under the lower limit (" << var << " < " << lo << ')';
    return false;
  }
  if (std::clamp(var, lo, hi) == hi) {
    LOG(error) << fName << ": " << name << " is found to be above the upper limit (" << var << " > " << hi << ')';
    return false;
  }
  return true;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, typename... Args>
T* CbmQaTask::MakeCanvas(const char* nameBase, Args... args)
{
  TString name = fsPrefix + "_" + nameBase;
  if (gROOT->FindObject(name)) {
    LOG(warn) << fName << ": A canvas with name \"" << name << "\" was previously created and will be deleted now "
              << "to avoid memory leaks";
    T* pCanv = (T*) gROOT->FindObject(name);
    delete pCanv;
  }

  // Create a new canvas
  T* pCanv = new T(name, args...);
  pCanv->SetLeftMargin(0.2);
  pCanv->SetBottomMargin(0.2);


  // Register canvas in the folder
  if (!fpFolderCanv) { fpFolderCanv = fpFolderRoot->AddFolder("canvases", "Canvases"); }
  fpFolderCanv->Add(pCanv);

  return pCanv;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, typename... Args>
T* CbmQaTask::MakeEfficiency(const char* nameBase, Args... args)
{
  TString name = fsPrefix + "_" + nameBase;
  if (gROOT->FindObject(name)) {
    LOG(warn) << fName << ": An efficiency with name \"" << name << "\" was previously created and will be deleted now "
              << "to avoid memory leaks";
    auto* pEff = (T*) gROOT->FindObject(name);
    delete pEff;
  }

  // Create a new efficiency
  auto* pEff = new T(name, args...);

  // Register efficiency in the folder
  if (!fpFolderEff) { fpFolderEff = fpFolderRoot->AddFolder("efficiencies", "Efficiencies"); }
  fpFolderEff->Add(pEff);

  return pEff;
}


// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, typename... Args>
T* CbmQaTask::MakeHisto(const char* nameBase, Args... args)
{
  TString name = fsPrefix + "_" + nameBase;
  // Check, if the histogram with a given name was already created. If so, delete it
  if (gROOT->FindObject(name)) {
    LOG(warn) << fName << ": A histogram with name \"" << name << "\" was previously created and will be deleted now "
              << "to avoid memory leaks";
    T* pHist = (T*) gROOT->FindObject(name);
    delete pHist;
  }

  T* pHist = new T(name, args...);

  // Register histogram in the folder
  if (!fpFolderHist) { fpFolderHist = fpFolderRoot->AddFolder("histograms", "Histograms"); }
  fpFolderHist->Add(pHist);

  return pHist;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T>
T* CbmQaTask::MakeHistoFromConfig(const char* nameBase, int id0, int id1, int id2)
{
  std::string name = std::string(fsPrefix.Data()) + "_" + nameBase;
  if (id0 > -1) { name = std::regex_replace(name, std::regex("\\%0"), std::to_string(id0)); }
  if (id1 > -1) { name = std::regex_replace(name, std::regex("\\%1"), std::to_string(id1)); }
  if (id2 > -1) { name = std::regex_replace(name, std::regex("\\%2"), std::to_string(id2)); }

  // Check, if the histogram with a given name was already created. If so, delete it
  if (gROOT->FindObject(name.data())) {
    LOG(warn) << fName << ": A histogram with name \"" << name << "\" was previously created and will be deleted now "
              << "to avoid memory leaks";
    T* pHist = (T*) gROOT->FindObject(name.data());
    delete pHist;
  }

  T* pHist = nullptr;
  // Create histogram
  LOG_IF(fatal, !fpCurrentNode)
    << fName << ": attempt to make a histogram (\"" << nameBase << "\") from configuration file, which was not "
    << "defined. Please, provide configuration file with defined histogram list to the task via "
    << "SetConfigName(filename) function.";
  try {
    const auto& node = (*fpCurrentNode)["histograms"][nameBase];
    LOG_IF(fatal, !node) << fName << ": node for histogram \"" << nameBase
                         << "\" was not defined in the configuration file";

    std::string title = node["title"].as<std::string>("").data();
    if (id0 > -1) { title = std::regex_replace(title, std::regex("\\%0"), std::to_string(id0)); }
    if (id1 > -1) { title = std::regex_replace(title, std::regex("\\%1"), std::to_string(id1)); }
    if (id2 > -1) { title = std::regex_replace(title, std::regex("\\%2"), std::to_string(id2)); }

    // 1D-profiles
    if constexpr (std::is_base_of_v<TProfile, T>) {
      int nBinsX      = node["nbinsx"].as<int>();
      double minX     = node["minx"].as<double>();
      double maxX     = node["maxx"].as<double>();
      double minY     = node["miny"].as<double>(0.);
      double maxY     = node["maxy"].as<double>(0.);
      std::string opt = node["opt"].as<std::string>("");
      pHist           = new T(name.data(), title.data(), nBinsX, minX, maxX, minY, maxY, opt.data());
    }
    // 2D-profiles
    else if constexpr (std::is_base_of_v<TProfile2D, T>) {
      int nBinsX      = node["nbinsx"].as<int>();
      double minX     = node["minx"].as<double>();
      double maxX     = node["maxx"].as<double>();
      int nBinsY      = node["nbinsy"].as<int>();
      double minY     = node["miny"].as<double>();
      double maxY     = node["maxy"].as<double>();
      double minZ     = node["minz"].as<double>(0.);
      double maxZ     = node["maxz"].as<double>(0.);
      std::string opt = node["opt"].as<std::string>("");
      pHist = new T(name.data(), title.data(), nBinsX, minX, maxX, nBinsY, minY, maxY, minZ, maxZ, opt.data());
    }
    // 3D-profiles
    else if constexpr (std::is_base_of_v<TProfile3D, T>) {
      int nBinsX      = node["nbinsx"].as<int>();
      double minX     = node["minx"].as<double>();
      double maxX     = node["maxx"].as<double>();
      int nBinsY      = node["nbinsy"].as<int>();
      double minY     = node["miny"].as<double>();
      double maxY     = node["maxy"].as<double>();
      int nBinsZ      = node["nbinsz"].as<int>();
      double minZ     = node["minz"].as<double>();
      double maxZ     = node["maxz"].as<double>();
      std::string opt = node["opt"].as<std::string>("");
      pHist = new T(name.data(), title.data(), nBinsX, minX, maxX, nBinsY, minY, maxY, minZ, maxZ, opt.data());
    }
    // 2D-histograms
    else if constexpr (std::is_base_of_v<TH2, T>) {
      int nBinsX  = node["nbinsx"].as<int>();
      double minX = node["minx"].as<double>();
      double maxX = node["maxx"].as<double>();
      int nBinsY  = node["nbinsy"].as<int>();
      double minY = node["miny"].as<double>();
      double maxY = node["maxy"].as<double>();
      pHist       = new T(name.data(), title.data(), nBinsX, minX, maxX, nBinsY, minY, maxY);
    }
    // 3D-histograms + derived
    else if constexpr (std::is_base_of_v<TH3, T>) {
      int nBinsX  = node["nbinsx"].as<int>();
      double minX = node["minx"].as<double>();
      double maxX = node["maxx"].as<double>();
      int nBinsY  = node["nbinsy"].as<int>();
      double minY = node["miny"].as<double>();
      double maxY = node["maxy"].as<double>();
      int nBinsZ  = node["nbinsz"].as<int>();
      double minZ = node["minz"].as<double>();
      double maxZ = node["maxz"].as<double>();
      pHist       = new T(name.data(), title.data(), nBinsX, minX, maxX, nBinsY, minY, maxY, nBinsZ, minZ, maxZ);
    }
    // 1D-histograms + derived
    else if constexpr (std::is_base_of_v<TH1, T>) {
      int nBinsX  = node["nbinsx"].as<int>();
      double minX = node["minx"].as<double>();
      double maxX = node["maxx"].as<double>();
      pHist       = new T(name.data(), title.data(), nBinsX, minX, maxX);
    }
    // Other types are restricted
    //else {
    //  static_assert(false, "CbmQaTask::MakeTable: unsupported type given as a template parameter");
    //}
  }
  catch (const YAML::InvalidNode& exc) {
    LOG(fatal) << fName << ": error while reading the histogram \"" << nameBase << "\" properties from "
               << "configuration file \"" << fsConfigName << "\". Please, check the file formatting. \n Tip: probably, "
               << "there ara mistakes in the obligatory property names, for example, \"nbinsX\" or \"nXBins\" is "
               << "used instead of \"nbinsx\" etc.";
  }

  // Register histogram in the folder
  if (!fpFolderHist) { fpFolderHist = fpFolderRoot->AddFolder("histograms", "Histograms"); }
  fpFolderHist->Add(pHist);

  return pHist;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename... Args>
CbmQaTable* CbmQaTask::MakeTable(const char* nameBase, Args... args)
{
  TString name = fsPrefix + "_" + nameBase;
  // Check, if the table with a given name was already created. If so, delete it
  if (gROOT->FindObject(name)) {
    LOG(warn) << fName << ": A table with name \"" << name << "\" was previously created and will be deleted now "
              << "to avoid memory leaks";
    CbmQaTable* pTable = (CbmQaTable*) gROOT->FindObject(name);
    delete pTable;
  }

  // Create a new table
  CbmQaTable* pTable = new CbmQaTable(name, args...);

  // Register table in folder
  if (!fpFolderTable) { fpFolderTable = fpFolderRoot->AddFolder("tables", "Tables"); }
  fpFolderTable->Add(pTable);

  return pTable;
}


#endif  // CbmQaTask_h
