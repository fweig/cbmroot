#ifndef ANALYSIS_TREE_SIMEVENTHEADERCONVERTER_H_
#define ANALYSIS_TREE_SIMEVENTHEADERCONVERTER_H_

#include "AnalysisTree/EventHeader.hpp"

#include "CbmConverterTask.h"

class FairMCEventHeader;
class CbmVertex;

class CbmSimEventHeaderConverter final : public CbmConverterTask {
public:
  explicit CbmSimEventHeaderConverter(std::string out_branch_name)
    : CbmConverterTask(std::move(out_branch_name)) {
    in_branches_.emplace_back("MCEventHeader.");
  };
  ~CbmSimEventHeaderConverter() final = default;

  void Init(std::map<std::string, void*>&) final;
  void Exec() final;
  void Finish() final { delete sim_event_header_; };

private:
  AnalysisTree::EventHeader* sim_event_header_ {nullptr};
  FairMCEventHeader* cbm_header_ {nullptr};

  ClassDef(CbmSimEventHeaderConverter, 1)
};

#endif  // ANALYSIS_TREE_SIMEVENTHEADERCONVERTER_H_
