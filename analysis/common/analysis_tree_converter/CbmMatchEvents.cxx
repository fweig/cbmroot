/* Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universität Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov [committer] */

#include "CbmMatchEvents.h"

#include "CbmEvent.h"
#include "CbmMCDataManager.h"
#include "CbmTrackMatchNew.h"

#include "TClonesArray.h"

#include "AnalysisTree/TaskManager.hpp"

//ClassImp(CbmMatchEvents);

void CbmMatchEvents::Init()
{
  auto* ioman    = FairRootManager::Instance();
  cbm_sts_match_ = (TClonesArray*) ioman->GetObject("StsTrackMatch");
}

void CbmMatchEvents::ProcessData(CbmEvent* event)
{
  if (!event) { throw std::runtime_error("No event to match"); }

  LOG(info) << "Event: " << event->GetNumber() << " t_start " << event->GetStartTime()
            << " Msts = " << event->GetNofStsTracks();

  count_map_.clear();

  const int n_sts_tracks = event->GetNofStsTracks();
  for (short i_track = 0; i_track < n_sts_tracks; ++i_track) {
    const auto track_index = event->GetStsTrackIndex(i_track);

    auto* match = (CbmTrackMatchNew*) cbm_sts_match_->At(track_index);
    if (match->GetNofLinks() == 0) { continue; }

    const auto& link = match->GetMatchedLink();
    auto file        = link.GetFile();
    auto entry       = link.GetEntry();

    count_map_[{file, entry}]++;
  }
  //  TODO remove later if not needed
  //  using pair_type = decltype(count_map_)::value_type;
  //  auto matched_event = std::max_element(count_map_.begin(), count_map_.end(),
  //                                        [](const pair_type& a, const pair_type& b) -> bool{ return a.second < b.second; } );

  auto match_event = new CbmMatch();

  int i {0};
  for (const auto& match : count_map_) {
    auto weight = float(match.second) / n_sts_tracks;
    match_event->AddLink(weight, i++, match.first.entry, match.first.file);
    LOG(info) << " matched to " << match.first.entry << " " << match.second << " weight = " << weight;
  }

  if (match_event->GetNofLinks() > 0) event->SetMatch(match_event);
}
