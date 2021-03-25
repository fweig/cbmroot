#ifndef ANALYSIS_TREE_TOFHITSCONVERTER_H
#define ANALYSIS_TREE_TOFHITSCONVERTER_H

#include "CbmConverterTask.h"

#include "AnalysisTree/Detector.hpp"

class TClonesArray;
class FairTrackParam;

namespace AnalysisTree
{
  class Matching;
}

class CbmTofHitsConverter final : public CbmConverterTask {
public:
  explicit CbmTofHitsConverter(std::string out_branch_name, std::string match_to = "")
    : CbmConverterTask(std::move(out_branch_name), std::move(match_to)) {};

  ~CbmTofHitsConverter() final;
  ;

  void Init() final;
  void Exec() final;
  void Finish() final {};

private:
  void FillTofHits();
  static void ExtrapolateStraightLine(FairTrackParam* params, float z);

  TClonesArray* cbm_global_tracks_ {nullptr};
  TClonesArray* cbm_tof_hits_ {nullptr};
  //  TClonesArray *cbm_tof_points_{nullptr};
  //  TClonesArray *cbm_tof_match_{nullptr};
  //  TClonesArray* cbm_mc_tracks_{nullptr};
  //  TClonesArray* cbm_sts_match_{nullptr};

  AnalysisTree::HitDetector* tof_hits_ {nullptr};
  AnalysisTree::Matching* vtx_tracks_2_tof_ {nullptr};

  ClassDef(CbmTofHitsConverter, 1)
};

#endif  // ANALYSIS_TREE_TOFHITSCONVERTER_H
