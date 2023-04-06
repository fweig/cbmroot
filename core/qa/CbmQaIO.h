/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaIO.h
/// @brief  Module for ROOT objects IO interface (header)
/// @author S.Zharko <s.zharko@gsi.de>
/// @since   29.03.2023

#ifndef CbmQaIO_h
#define CbmQaIO_h 1

#include "CbmQaTable.h"

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
#include "TString.h"

/// @brief  ROOT object IO interface for QA
///
/// The class provides interface to write ROOT object into resulted files
class CbmQaIO {
public:
  enum class EStoringMode
  {
    kSAMEDIR,  ///< Objects will be stored to root directory
    kSUBDIR    ///< Objects will be stored to corresponding subdirectory
  };

  /// @brief Constructor
  /// @param folderName    Name of the root folder
  /// @param prefixName    Name of the unique prefix
  /// @param pParentFolder Pointer to parent folder
  CbmQaIO(const char* folderName, const char* prefixName, TFolder* pParentFolder = nullptr);

  /// @brief Destructor
  virtual ~CbmQaIO();

  /// @brief Copy constructor
  CbmQaIO(const CbmQaIO&) = delete;

  /// @brief Move constructor
  CbmQaIO(CbmQaIO&&) = delete;

  /// @brief Copy assignment operator
  CbmQaIO& operator=(const CbmQaIO&) = delete;

  /// @brief Move assignment operator
  CbmQaIO& operator=(CbmQaIO&&) = delete;

  /// @brief Creates, initializes and registers a canvas
  /// @tparam  T     Type of the canvas: TCanvas or CbmQaCanvas (\note T should not be a pointer)
  /// @param   name  Name of canvas
  /// @param   args  The rest of the arguments, which will be passed to the canvas constructor
  template<typename T, typename... Args>
  T* MakeCanvas(const char* name, Args... args);

  /// @brief Creates, initializes and registers an efficiency
  /// @tparam  T     Type of the canvas: either TProfile, TProfile1D or TEfficiency (\note T should not be a pointer)
  /// @param   name  Name of canvas
  /// @param   args  The rest of the arguments, which will be passed to the efficiency constructor
  template<typename T, typename... Args>
  T* MakeEfficiency(const char* name, Args... args);

  /// @brief Creates, initializes and registers a histogram
  /// @tparam  T     Type of the histogram (\note T should not be a pointer)
  /// @param   name  Name of histogram
  /// @param   args  The rest of the arguments, which will be passed to the histogram constructor
  template<typename T, typename... Args>
  T* MakeHisto(const char* name, Args... args);

  /// @brief Creates, initializes and registers a table
  /// @param  name  Name of the table
  /// @param  args  The rest of the arguments, which will be passed to the table constructor
  template<typename... Args>
  CbmQaTable* MakeTable(const char* name, Args... args);

protected:
  TString fsRootFolderName = "";  ///< Name of root folder
  TString fsPrefix         = "";  ///< Unique prefix for all writeable root

  EStoringMode fStoringMode = EStoringMode::kSUBDIR;  ///< Objects storing mode
  TFolder* fpFolderRoot     = nullptr;                ///< Root folder to store histograms and canvases
  TFolder* fpFolderHist     = nullptr;                ///< Folder for raw histograms
  TFolder* fpFolderCanv     = nullptr;                ///< Folder for canvases
  TFolder* fpFolderEff      = nullptr;                ///< Folder for efficiencies
  TFolder* fpFolderTable    = nullptr;                ///< Folder for tables
  TFolder* fpParentFolder   = nullptr;                ///< Pointer to parent folder
};


// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, typename... Args>
T* CbmQaIO::MakeCanvas(const char* nameBase, Args... args)
{
  TString name = fsPrefix + "_" + nameBase;
  if (gROOT->FindObject(name)) {
    LOG(warn) << fsRootFolderName << ": A previously created canvas \"" << name << "\" will be deleted";
    T* pCanv = (T*) gROOT->FindObject(name);
    delete pCanv;
  }

  // Create a new canvas
  T* pCanv = new T(name, args...);
  pCanv->SetLeftMargin(-1.2);
  pCanv->SetBottomMargin(-1.2);


  // Register canvas in the folder
  if (fStoringMode == EStoringMode::kSUBDIR) {
    if (!fpFolderCanv) { fpFolderCanv = fpFolderRoot->AddFolder("canvases", "Canvases"); }
    fpFolderCanv->Add(pCanv);
  }
  else if (fStoringMode == EStoringMode::kSAMEDIR) {
    fpFolderRoot->Add(pCanv);
  }

  return pCanv;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, typename... Args>
T* CbmQaIO::MakeEfficiency(const char* nameBase, Args... args)
{
  TString name = fsPrefix + "_" + nameBase;
  if (gROOT->FindObject(name)) {
    LOG(warn) << fsRootFolderName << ": A previously created histogram \"" << name << "\" will be deleted";
    auto* pEff = (T*) gROOT->FindObject(name);
    delete pEff;
  }

  // Create a new efficiency
  auto* pEff = new T(name, args...);

  // Register efficiency in the folder
  if (fStoringMode == EStoringMode::kSUBDIR) {
    if (!fpFolderEff) { fpFolderEff = fpFolderRoot->AddFolder("efficiencies", "Efficiencies"); }
    fpFolderEff->Add(pEff);
  }
  else if (fStoringMode == EStoringMode::kSAMEDIR) {
    fpFolderRoot->Add(pEff);
  }

  return pEff;
}


// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, typename... Args>
T* CbmQaIO::MakeHisto(const char* nameBase, Args... args)
{
  TString name = fsPrefix + "_" + nameBase;
  // Check, if the histogram with a given name was already created. If so, delete it
  if (gROOT->FindObject(name)) {
    LOG(warn) << fsRootFolderName << ": A previously created histogram \"" << name << "\" will be deleted";
    T* pHist = (T*) gROOT->FindObject(name);
    delete pHist;
  }

  T* pHist = new T(name, args...);
  pHist->SetStats(kTRUE);
  pHist->Sumw2();

  // Register histogram in the folder
  if (fStoringMode == EStoringMode::kSUBDIR) {
    if (!fpFolderHist) { fpFolderHist = fpFolderRoot->AddFolder("histograms", "Histograms"); }
    fpFolderHist->Add(pHist);
  }
  else if (fStoringMode == EStoringMode::kSAMEDIR) {
    fpFolderRoot->Add(pHist);
  }

  return pHist;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename... Args>
CbmQaTable* CbmQaIO::MakeTable(const char* nameBase, Args... args)
{
  TString name = fsPrefix + "_" + nameBase;
  // Check, if the table with a given name was already created. If so, delete it
  if (gROOT->FindObject(name)) {
    LOG(warn) << fsRootFolderName << ": A previously created table \"" << name << "\" will be deleted";
    CbmQaTable* pTable = (CbmQaTable*) gROOT->FindObject(name);
    delete pTable;
  }

  // Create a new table
  CbmQaTable* pTable = new CbmQaTable(name, args...);

  // Register table in folder
  if (fStoringMode == EStoringMode::kSUBDIR) {
    if (!fpFolderTable) { fpFolderTable = fpFolderRoot->AddFolder("tables", "Tables"); }
    fpFolderTable->Add(pTable);
  }
  else if (fStoringMode == EStoringMode::kSAMEDIR) {
    fpFolderRoot->Add(pTable);
  }

  return pTable;
}

#endif  // CbmQaIO_h
