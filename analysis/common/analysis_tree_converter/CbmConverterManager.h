/* Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universitaet Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov [committer], Viktor Klochkov [committer] */

#ifndef ANALYSIS_TREE_CONVERTERMANAGER_H_
#define ANALYSIS_TREE_CONVERTERMANAGER_H_

#include "FairTask.h"

#include <utility>

#include "AnalysisTree/TaskManager.hpp"

namespace AnalysisTree
{
  class Configuration;
  class DataHeader;
}  // namespace AnalysisTree

class CbmConverterTask;

class CbmConverterManager : public FairTask {

public:
  CbmConverterManager() = default;
  ~CbmConverterManager() override;

  InitStatus Init() override;
  void Exec(Option_t* opt) override;
  void Finish() override;

  void AddTask(CbmConverterTask* task)
  {
    tasks_.emplace_back(task);
    task_manager_->AddTask(reinterpret_cast<AnalysisTree::Task*>(task));
  }

  void SetSystem(const std::string& system) { system_ = system; }
  void SetBeamMomentum(float beam_mom) { beam_mom_ = beam_mom; }

  void SetOutputName(std::string file, std::string tree = "rTree") { task_manager_->SetOutputName(file, tree); }

private:
  void FillDataHeader();

  AnalysisTree::TaskManager* task_manager_ {AnalysisTree::TaskManager::GetInstance()};

  std::string system_;
  float beam_mom_ {0.};

  std::vector<CbmConverterTask*> tasks_ {};

  std::map<std::string, std::map<int, int>> index_map_ {};  ///< map CbmRoot to AT of indexes for a given branch

  ClassDefOverride(CbmConverterManager, 1)
};

#endif  // ANALYSIS_TREE_CONVERTERMANAGER_H_
