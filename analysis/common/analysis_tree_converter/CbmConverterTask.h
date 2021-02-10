#ifndef ANALYSIS_TREE_CONVERTERTASK_H_
#define ANALYSIS_TREE_CONVERTERTASK_H_

#include "AnalysisTree/Task.hpp"

class FairRootManager;

class CbmConverterTask : public AnalysisTree::Task {

public:
  CbmConverterTask() = delete;
  explicit CbmConverterTask(std::string out_branch_name,
                            std::string match_to = "") {
    out_branch_ = std::move(out_branch_name);
    match_to_   = std::move(match_to);
  };

  ~CbmConverterTask() override = default;

  const std::map<int, int>& GetOutIndexesMap() const {
    return out_indexes_map_;
  }

  void SetIndexesMap(std::map<std::string, std::map<int, int>>* indexes_map) {
    indexes_map_ = indexes_map;
  }

  const std::string& GetOutputBranchName() const { return out_branch_; }

protected:
  std::map<int, int> out_indexes_map_ {};  ///< CbmRoot to AnalysisTree indexes
                                           ///< map for output branch
  std::string out_branch_ {};
  std::map<std::string, std::map<int, int>>*
    indexes_map_ {};  ///< CbmRoot to AnalysisTree indexes map for branches
  ///< from other tasks
  std::string match_to_ {};  ///< AT branch to match
};

#endif  // ANALYSIS_TREE_CONVERTERTASK_H_
