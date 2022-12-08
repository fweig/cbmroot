/* Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universitaet Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov [committer] */

#include "CbmRecEventHeaderConverter.h"

#include "CbmPsdHit.h"
#include "CbmVertex.h"

#include "FairMCEventHeader.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

#include <AnalysisTree/TaskManager.hpp>

#include "cassert"
#include "iostream"

ClassImp(CbmRecEventHeaderConverter);

void CbmRecEventHeaderConverter::Init()
{
  assert(!out_branch_.empty());
  auto* ioman = FairRootManager::Instance();
  assert(ioman != nullptr);
  cbm_header_      = (FairMCEventHeader*) ioman->GetObject("MCEventHeader.");
  cbm_prim_vertex_ = (CbmVertex*) ioman->GetObject("PrimaryVertex.");
  cbm_sts_tracks_  = (TClonesArray*) ioman->GetObject("StsTrack");
  cbm_psd_hits_    = (TClonesArray*) ioman->GetObject("PsdHit");

  //  ***** RecEventHeader *******
  AnalysisTree::BranchConfig RecEventHeaderBranch("RecEventHeader", AnalysisTree::DetType::kEventHeader);
  RecEventHeaderBranch.AddField<float>("vtx_chi2", "primiry vertex fit chi^2/NDF");
  RecEventHeaderBranch.AddField<float>("Epsd", "GeV, full energy deposit in PSD");
  RecEventHeaderBranch.AddField<int>("M", "total multiplicity in STS(+MVD)");
  RecEventHeaderBranch.AddField<int>("evt_id", "event identifier");
  RecEventHeaderBranch.AddField<float>("start_time", "Start time of the event, ns");
  RecEventHeaderBranch.AddField<float>("end_time", "End time of the event, ns");
  RecEventHeaderBranch.AddField<float>("match_weight", "");

  ivtx_chi2_     = RecEventHeaderBranch.GetFieldId("vtx_chi2");
  iEpsd_         = RecEventHeaderBranch.GetFieldId("Epsd");
  iM_            = RecEventHeaderBranch.GetFieldId("M");
  ievt_id_       = RecEventHeaderBranch.GetFieldId("evt_id");
  istart_time_   = RecEventHeaderBranch.GetFieldId("start_time");
  iend_time_     = RecEventHeaderBranch.GetFieldId("end_time");
  imatch_weight_ = RecEventHeaderBranch.GetFieldId("match_weight");

  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(rec_event_header_, RecEventHeaderBranch);
  rec_event_header_->Init(RecEventHeaderBranch);
}

void CbmRecEventHeaderConverter::ProcessData(CbmEvent* event)
{
  auto* out_config_  = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = out_config_->GetBranchConfig(out_branch_);

  if (event) { cbm_prim_vertex_ = event->GetVertex(); }

  if (!cbm_prim_vertex_) { throw std::runtime_error("No fPrimVtx"); }

  rec_event_header_->SetVertexPosition3({cbm_prim_vertex_->GetX(), cbm_prim_vertex_->GetY(), cbm_prim_vertex_->GetZ()});
  rec_event_header_->SetField(float(cbm_prim_vertex_->GetChi2() / cbm_prim_vertex_->GetNDF()), ivtx_chi2_);

  const int n_sts_tracks = event ? event->GetNofStsTracks() : cbm_sts_tracks_->GetEntries();
  rec_event_header_->SetField(n_sts_tracks, iM_);
  rec_event_header_->SetField(GetPsdEnergy(event), iEpsd_);

  int evt_id;
  float match_weight, start_time, end_time;
  if (event) {
    evt_id     = event->GetUniqueID();
    start_time = event->GetStartTime();
    end_time   = event->GetEndTime();
    if (event->GetMatch()) match_weight = float(event->GetMatch()->GetMatchedLink().GetWeight());
    else
      match_weight = 0.;
  }
  else {
    evt_id       = cbm_header_->GetEventID();
    start_time   = cbm_header_->GetT();
    end_time     = cbm_header_->GetT();
    match_weight = 1.;
  }
  rec_event_header_->SetField(evt_id, ievt_id_);
  rec_event_header_->SetField(start_time, istart_time_);
  rec_event_header_->SetField(end_time, iend_time_);
  rec_event_header_->SetField(match_weight, imatch_weight_);
}

float CbmRecEventHeaderConverter::GetPsdEnergy(CbmEvent* event)
{
  //TODO avoid duplicating the code with PsdModulesConverter
  float psd_energy {0.f};
  const int nPsdHits = event ? event->GetNofData(ECbmDataType::kPsdHit) : cbm_psd_hits_->GetEntriesFast();
  for (int i = 0; i < nPsdHits; ++i) {
    auto* hit = (CbmPsdHit*) cbm_psd_hits_->At(i);
    if (hit == nullptr) continue;
    psd_energy += hit->GetEdep();
  }
  return psd_energy;
}
