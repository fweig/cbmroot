/* Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universität Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov [committer] */

#ifndef ANALYSIS_TREE_STSTRACKSCONVERTER_H_
#define ANALYSIS_TREE_STSTRACKSCONVERTER_H_

#include "CbmConverterTask.h"
#include <CbmMCDataManager.h>

#include "AnalysisTree/Detector.hpp"

class TClonesArray;
class CbmVertex;
class CbmStsTrack;
class CbmTrackMatchNew;
class CbmMCDataManager;
class CbmMCDataArray;

namespace AnalysisTree
{
  class Matching;
}

class CbmStsTracksConverter final : public CbmConverterTask {

public:
  explicit CbmStsTracksConverter(std::string out_branch_name, std::string match_to = "")
    : CbmConverterTask(std::move(out_branch_name), std::move(match_to))
  {
  }

  ~CbmStsTracksConverter() final;

  void Init() final;
  void ProcessData(CbmEvent* event) final;
  void Finish() final {}

  void SetIsWriteKFInfo(bool is = true) { is_write_kfinfo_ = is; }
  void SetIsReproduceCbmKFPF(bool is = true) { is_reproduce_cbmkfpf_ = is; }

private:
  void ReadVertexTracks(CbmEvent* event);
  void MapTracks(CbmEvent* event);
  void InitInput();
  float ExtrapolateToVertex(CbmStsTrack* sts_track, AnalysisTree::Track& track, int pdg);

  void WriteKFInfo(AnalysisTree::Track& track, const CbmStsTrack* sts_track, bool is_good_track) const;
  bool IsGoodCovMatrix(const CbmStsTrack* sts_track) const;
  //  int GetMcPid(const CbmTrackMatchNew* match, AnalysisTree::Track& track) const;

  AnalysisTree::TrackDetector* vtx_tracks_ {nullptr};   ///< raw pointers are needed for TTree::Branch
  AnalysisTree::Matching* vtx_tracks_2_sim_ {nullptr};  ///< raw pointers are needed for TTree::Branch

  CbmVertex* cbm_prim_vertex_ {nullptr};    ///< non-owning pointer
                                            //  TClonesArray* cbm_mc_tracks_ {nullptr};   ///< non-owning pointer
  TClonesArray* cbm_sts_tracks_ {nullptr};  ///< non-owning pointer
  TClonesArray* cbm_sts_match_ {nullptr};   ///< non-owning pointer

  CbmMCDataManager* cbm_mc_manager_ {nullptr};
  CbmMCDataArray* cbm_mc_tracks_new_ {nullptr};

  bool is_write_kfinfo_ {true};
  bool is_reproduce_cbmkfpf_ {true};

  int iq_ {AnalysisTree::UndefValueInt};
  int indf_ {AnalysisTree::UndefValueInt};
  int ichi2_ {AnalysisTree::UndefValueInt};
  int inhits_ {AnalysisTree::UndefValueInt};
  int inhits_mvd_ {AnalysisTree::UndefValueInt};
  int idcax_ {AnalysisTree::UndefValueInt};
  int ivtx_chi2_ {AnalysisTree::UndefValueInt};
  int ide_dx_ {AnalysisTree::UndefValueInt};
  int imatch_weight_ {AnalysisTree::UndefValueInt};

  int ipar_ {AnalysisTree::UndefValueInt};
  int imf_ {AnalysisTree::UndefValueInt};
  int icov_ {AnalysisTree::UndefValueInt};
  int imc_pdg_ {AnalysisTree::UndefValueInt};
  int imother_pdg_ {AnalysisTree::UndefValueInt};
  int ipasscuts_ {AnalysisTree::UndefValueInt};

  ClassDef(CbmStsTracksConverter, 1)
};

#endif  // ANALYSIS_TREE_STSTRACKSCONVERTER_H_
