/* Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universitaet Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov [committer] */

#ifndef ANALYSIS_TREE_RECEVENTHEADERCONVERTER_H_
#define ANALYSIS_TREE_RECEVENTHEADERCONVERTER_H_

#include "CbmConverterTask.h"
#include "CbmDefs.h"
#include "CbmEvent.h"

#include "AnalysisTree/EventHeader.hpp"

class FairMCEventHeader;
class CbmVertex;
class TClonesArray;

class CbmRecEventHeaderConverter final : public CbmConverterTask {
public:
  explicit CbmRecEventHeaderConverter(std::string out_branch_name) : CbmConverterTask(std::move(out_branch_name)) {};
  ~CbmRecEventHeaderConverter() final = default;

  void Init() final;
  void ProcessData(CbmEvent* event) final;
  void Finish() final { delete rec_event_header_; };

private:
  float GetPsdEnergy(CbmEvent* event);

  AnalysisTree::EventHeader* rec_event_header_ {nullptr};

  TClonesArray* cbm_psd_hits_ {nullptr};
  TClonesArray* cbm_sts_tracks_ {nullptr};   ///< non-owning pointer
  FairMCEventHeader* cbm_header_ {nullptr};  ///< non-owning pointer
  CbmVertex* cbm_prim_vertex_ {nullptr};     ///< non-owning pointer

  int ivtx_chi2_ {AnalysisTree::UndefValueInt};
  int iEpsd_ {AnalysisTree::UndefValueInt};
  int iM_ {AnalysisTree::UndefValueInt};
  int ievt_id_ {AnalysisTree::UndefValueInt};
  int istart_time_ {AnalysisTree::UndefValueInt};
  int iend_time_ {AnalysisTree::UndefValueInt};
  int imatch_weight_ {AnalysisTree::UndefValueInt};

  ClassDef(CbmRecEventHeaderConverter, 1)
};

#endif  // ANALYSIS_TREE_RECEVENTHEADERCONVERTER_H_
