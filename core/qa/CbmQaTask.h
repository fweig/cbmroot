/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmQaTask.h
/// \brief  A base class for CBM QA task logic
/// \author S.Zharko <s.zharko@gsi.de>
/// \data   12.01.2022

#ifndef CbmQaTask_h
#define CbmQaTask_h 1

#include "FairTask.h"
#include "Logger.h"

#include "TCanvas.h"
#include "TEfficiency.h"
#include "TFolder.h"
#include "TH1.h"
#include "TParameter.h"
#include "TROOT.h"

#include <string_view>

/// Class CbmQaTask is to be inherited with a particular QA-task. It provides mechanisms for storage and management
/// of QA canvases and histograms management
class CbmQaTask : public FairTask {
public:
  /// Constructor from parameters
  /// \param  name     Name of the task
  /// \param  verbose  Verbose level
  /// \param  isMCUsed Flag: true - MC information is used, false - only reconstructed data QA is processed
  CbmQaTask(const char* name, int verbose, bool isMCUsed);

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

  ClassDef(CbmQaTask, 0);

protected:
  // *****************************************************
  // ** Functions accessible inside the derived classes **
  // *****************************************************

  /// Method to check, if the QA results are acceptable
  /// NOTE: Here one can add a geometry check ...
  virtual bool Check() const { return true; }

  /// De-initialize the task
  virtual void DeInit();

  /// Initializes data branches
  virtual InitStatus InitDataBranches();

  /// Initializes histograms
  virtual InitStatus InitHistograms();

  /// Initializes canvases
  virtual InitStatus InitCanvases();

  /// Method to fill histograms per event or time-slice
  virtual void FillHistograms();

  /// Method to process histograms in the end of the run (optional)
  virtual void AnalyzeHistograms();

  /// Creates, initializes and registers the histogram
  /// \tparam  T     Type of the canvas: TCanvas or CbmQaCanvas (\note T should not be a pointer)
  /// \param   name  Name of canvas
  /// \param   args  The rest of the arguments, which will be passed to the canvas constructor
  template<typename T, typename... Args>
  T* MakeCanvas(const char* name, Args... args);

  /// Creates, initializes and registers the efficiency
  /// \tparam  T     Type of the canvas: either TProfile, TProfile2D or TEfficiency (\note T should not be a pointer)
  /// \param   name  Name of canvas
  /// \param   args  The rest of the arguments, which will be passed to the efficiency constructor
  template<typename T, typename... Args>
  T* MakeEfficiency(const char* name, Args... args);

  /// Creates, initializes and registers the histogram
  /// \tparam  T     Type of the histogram (\note T should not be a pointer)
  /// \param   name  Name of histogram
  /// \param   args  The rest of the arguments, which will be passed to the histogram constructor
  template<typename T, typename... Args>
  T* MakeHisto(const char* name, Args... args);

  /// Method to setup histogram properties (text sizes etc.)
  /// \param  pHist  Pointer to a histogram to tune
  virtual void SetHistoProperties(TH1* pHist);

  /// Gets a reference to histograms map
  ///const auto& GetHistoMap() const { return fmpHistList; }

  /// Gets a reference to canvases map
  ///const auto& GetCanvasMap() const { return fmpCanvList; }

  /// Get current event number
  int GetEventNumber() const { return fNofEvents.GetVal(); }


private:
  // **********************************************************
  // ** Functions accessible inside the CbmQaTask class only **
  // **********************************************************

  /// De-initialize this task
  void DeInitBase();

  // ***********************
  // ** Private variables **
  // ***********************

  // Flags
  bool fbUseMC = false;  ///< Flag, if MC is used

  // I/O
  std::unique_ptr<TFolder> fpFolderRoot = nullptr;  ///< Root folder to store histograms and canvases
  TFolder* fpFolderHist                 = nullptr;  ///< Folder for raw histograms
  TFolder* fpFolderCanv                 = nullptr;  ///< Folder for canvases
  TFolder* fpFolderEff                  = nullptr;  ///< Folder for efficiencies

  TParameter<int> fNofEvents {"nEvents", 0};  ///< Number of processed events
};


// *************************************************
// ** Inline and template function implementation **
// *************************************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, typename... Args>
T* CbmQaTask::MakeCanvas(const char* name, Args... args)
{
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
T* CbmQaTask::MakeEfficiency(const char* name, Args... args)
{
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
T* CbmQaTask::MakeHisto(const char* name, Args... args)
{
  // Check, if the histogram with a given name was already created. If so, delete it
  if (gROOT->FindObject(name)) {
    LOG(warn) << fName << ": A histogram with name \"" << name << "\" was previously created and will be deleted now "
              << "to avoid memory leaks";
    T* pHist = (T*) gROOT->FindObject(name);
    delete pHist;
  }

  // Create a new histogram or profile
  T* pHist = new T(name, args...);
  pHist->Sumw2();

  // Register histogram in the folder
  if (!fpFolderHist) { fpFolderHist = fpFolderRoot->AddFolder("histograms", "Histograms"); }
  fpFolderHist->Add(pHist);

  SetHistoProperties(pHist);

  return pHist;
}

#endif  // CbmQaTask_h
