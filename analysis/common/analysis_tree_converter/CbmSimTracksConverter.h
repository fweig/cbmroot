#ifndef ANALYSIS_TREE_SIMTRACKSCONVERTER_H_
#define ANALYSIS_TREE_SIMTRACKSCONVERTER_H_

#include "AnalysisTree/Detector.hpp"

#include "CbmConverterTask.h"

class TClonesArray;

class CbmSimTracksConverter final : public CbmConverterTask {
public:
  explicit CbmSimTracksConverter(std::string out_branch_name,
                                 std::string match_to = "")
    : CbmConverterTask(std::move(out_branch_name), std::move(match_to)) {
    in_branches_.emplace_back("MCTrack");
  };

  ~CbmSimTracksConverter() final;

  void Init(std::map<std::string, void*>&) final;
  void Exec() final;
  void Finish() final {};

private:
  AnalysisTree::Particles* sim_tracks_ {nullptr};
  TClonesArray* cbm_mc_tracks_ {nullptr};

  ClassDef(CbmSimTracksConverter, 1)
};

#endif
