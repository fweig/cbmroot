#ifndef ANALYSIS_TREE_RICHRINGSCONVERTER_H_
#define ANALYSIS_TREE_RICHRINGSCONVERTER_H_

#include "CbmConverterTask.h"

#include "AnalysisTree/Detector.hpp"

class TClonesArray;

namespace AnalysisTree
{
  class Matching;
}

class CbmRichRingsConverter final : public CbmConverterTask {
public:
  explicit CbmRichRingsConverter(std::string out_branch_name, std::string match_to = "")
    : CbmConverterTask(std::move(out_branch_name), std::move(match_to)) {};

  ~CbmRichRingsConverter() final;

  void Init() final;
  void Exec() final;
  void Finish() final {};

private:
  void FillRichRings();

  TClonesArray* cbm_global_tracks_ {nullptr};
  TClonesArray* cbm_rich_rings_ {nullptr};

  AnalysisTree::HitDetector* rich_rings_ {nullptr};
  AnalysisTree::Matching* vtx_tracks_2_rich_ {nullptr};

  ClassDef(CbmRichRingsConverter, 1)
};


#endif  //ANALYSIS_TREE_RICHRINGSCONVERTER_H_
