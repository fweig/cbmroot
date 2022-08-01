/* Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universität Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov [committer] */

#ifndef CBMROOT_ANALYSIS_COMMON_ANALYSIS_TREE_CONVERTER_CBMMATCHEVENTS_H_
#define CBMROOT_ANALYSIS_COMMON_ANALYSIS_TREE_CONVERTER_CBMMATCHEVENTS_H_

#include "CbmConverterTask.h"

class CbmMCDataManager;
class CbmMCDataArray;

class CbmMatchEvents final : public CbmConverterTask {
public:
  explicit CbmMatchEvents() = default;

  ~CbmMatchEvents() final = default;

  void Init() final;

  void ProcessData(CbmEvent* event) final;

  void Finish() final {}

  struct EventId {
    EventId(int f, int e) : file(f), entry(e) {}
    int file {0};
    int entry {0};
    bool operator<(const EventId& other) const { return this->file < other.file || this->entry < other.entry; }
  };

private:
  TClonesArray* cbm_sts_match_ {nullptr};  ///< non-owning pointer
  std::map<EventId, int> count_map_ {};    ///!

  //  ClassDef(CbmMatchEvents, 1);
};

#endif  //CBMROOT_ANALYSIS_COMMON_ANALYSIS_TREE_CONVERTER_CBMMATCHEVENTS_H_
