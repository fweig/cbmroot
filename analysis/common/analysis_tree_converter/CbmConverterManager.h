#ifndef ANALYSIS_TREE_CONVERTERMANAGER_H_
#define ANALYSIS_TREE_CONVERTERMANAGER_H_

#include "FairTask.h"

namespace AnalysisTree {
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

  void AddTask(CbmConverterTask* task) { tasks_.emplace_back(task); }
  void SetOutFileName(std::string name) { out_file_name_ = std::move(name); }
  void SetOutTreeName(std::string name) { out_tree_name_ = std::move(name); }

  void SetSystem(const std::string& system) { system_ = system; }
  void SetBeamMomentum(float beam_mom) { beam_mom_ = beam_mom; }

private:
  void FillDataHeader();

  TFile* out_file_ {nullptr};
  TTree* out_tree_ {nullptr};
  std::string out_file_name_ {""};
  std::string out_tree_name_ {""};

  std::string system_ {""};
  float beam_mom_ {0.};

  AnalysisTree::Configuration* out_config_ {nullptr};
  AnalysisTree::DataHeader* data_header_ {nullptr};
  std::vector<CbmConverterTask*> tasks_ {};

  std::map<std::string, std::map<int, int>>
    index_map_ {};  ///< map CbmRoot to AT of indexes for a given branch

  ClassDefOverride(CbmConverterManager, 1)
};

#endif  // ANALYSIS_TREE_CONVERTERMANAGER_H_
