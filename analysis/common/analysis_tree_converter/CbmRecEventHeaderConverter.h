#ifndef ANALYSIS_TREE_RECEVENTHEADERCONVERTER_H_
#define ANALYSIS_TREE_RECEVENTHEADERCONVERTER_H_

#include "AnalysisTree/EventHeader.hpp"

#include "CbmConverterTask.h"

class FairMCEventHeader;
class CbmVertex;

class CbmRecEventHeaderConverter final : public CbmConverterTask {
public:
  explicit CbmRecEventHeaderConverter(std::string out_branch_name)
    : CbmConverterTask(std::move(out_branch_name)) {};
  ~CbmRecEventHeaderConverter() final = default;

  void Init() final;
  void Exec() final;
  void Finish() final { delete rec_event_header_; };

private:
  AnalysisTree::EventHeader* rec_event_header_ {nullptr};

  FairMCEventHeader* cbm_header_ {nullptr};
  CbmVertex* cbm_prim_vertex_ {nullptr};

  ClassDef(CbmRecEventHeaderConverter, 1)
};

#endif  // ANALYSIS_TREE_RECEVENTHEADERCONVERTER_H_
