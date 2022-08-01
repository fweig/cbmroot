/* Copyright (C) 2021 Physikalisches Institut, Eberhard Karls Universitaet Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov [committer] */

#ifndef ANALYSIS_TREE_TRDTRACKSCONVERTER_H_
#define ANALYSIS_TREE_TRDTRACKSCONVERTER_H_

#include "CbmConverterTask.h"

#include "AnalysisTree/Detector.hpp"

class TClonesArray;

namespace AnalysisTree
{
  class Matching;
}

class CbmTrdTracksConverter final : public CbmConverterTask {

public:
  explicit CbmTrdTracksConverter(std::string out_branch_name, std::string match_to = "")
    : CbmConverterTask(std::move(out_branch_name), std::move(match_to))
  {
  }

  ~CbmTrdTracksConverter() final;

  void Init() final;
  void ProcessData(CbmEvent* event) final;
  void Finish() final {};

private:
  TClonesArray* cbm_global_tracks_ {nullptr};
  TClonesArray* cbm_trd_tracks_ {nullptr};
  TClonesArray* cbm_trd_hits_ {nullptr};

  AnalysisTree::TrackDetector* trd_tracks_ {nullptr};
  AnalysisTree::Matching* vtx_tracks_2_trd_ {nullptr};

  ClassDef(CbmTrdTracksConverter, 1)
};


#endif  //ANALYSIS_TREE_TRDTRACKSCONVERTER_H_
