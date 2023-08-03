#ifndef ANALYSISTREEQA_SRC_TASK_HPP_
#define ANALYSISTREEQA_SRC_TASK_HPP_

#include <utility>

#include "TFile.h"

#include "AnalysisTree/AnalysisTask.hpp"

#include "EntryConfig.hpp"

namespace AnalysisTree {
namespace QA {

class Task : public AnalysisTask {
 public:
  Task() = default;

  void Init() override;
  void Exec() override;
  void Finish() override;

  size_t AddH1(const Axis& x, Cuts* cuts = nullptr) {
    entries_.emplace_back(EntryConfig(x, cuts, false));
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts()));
    entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}});
    return entries_.size() - 1;
  }

  size_t AddH2(const Axis& x, const Axis& y, Cuts* cuts = nullptr) {
    entries_.emplace_back(EntryConfig(x, y, cuts));
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts()));
    entries_.back().SetVariablesId({ {var_id.first, var_id.second.at(0)}, {var_id.first, var_id.second.at(1)} });
    return entries_.size() - 1;
  }

  size_t AddProfile(const Axis& x, const Axis& y, Cuts* cuts = nullptr) {
    entries_.emplace_back(EntryConfig(x, y, cuts, true));
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts()));
    entries_.back().SetVariablesId({ {var_id.first, var_id.second.at(0)}, {var_id.first, var_id.second.at(1)} });
    return entries_.size() - 1;
  }

  size_t AddIntegral(const Axis& x, Cuts* cuts = nullptr) {
    entries_.emplace_back(EntryConfig(x, cuts, true));
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts()));
    entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}});
    return entries_.size() - 1;
  }

  size_t AddIntegral(const Axis& x, const Axis& y, Cuts* cuts_x = nullptr, Cuts* cuts_y = nullptr) {
    entries_.emplace_back(EntryConfig(x, cuts_x, y, cuts_y));
    auto var_id_x = AddEntry(AnalysisEntry({entries_.back().GetVariables()[0]}, cuts_x));
    auto var_id_y = AddEntry(AnalysisEntry({entries_.back().GetVariables()[1]}, cuts_y));
    entries_.back().SetVariablesId({ {var_id_x.first, var_id_x.second.at(0)}, {var_id_y.first, var_id_y.second.at(0)} });
    return entries_.size() - 1;
  }

  std::vector<EntryConfig>& Entries() { return entries_; }
  void SetOutputFileName(std::string name) { out_file_name_ = std::move(name); }

 private:
  void FillIntegral(EntryConfig& plot);
  size_t AddAnalysisEntry(const Axis& a, Cuts* cuts, bool is_integral);

  std::vector<EntryConfig> entries_{};
  std::map<std::string, TDirectory*> dir_map_{};
  std::string out_file_name_{"QA.root"};
  TFile* out_file_{nullptr};

  ClassDefOverride(Task, 1);
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SRC_TASK_HPP_
