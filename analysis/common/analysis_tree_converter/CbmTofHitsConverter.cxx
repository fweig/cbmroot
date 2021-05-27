/** @file CbmTofHitsConverter.cxx
  * @copyright Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universität Tübingen, Tübingen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Viktor Klochkov [orginator], Viktor Klochkov [orginator] **/

#include "CbmTofHitsConverter.h"

#include <CbmGlobalTrack.h>
#include <CbmMCTrack.h>
#include <CbmTofHit.h>
#include <CbmTrackMatchNew.h>

#include <FairMCPoint.h>
#include <FairRootManager.h>

#include "TClonesArray.h"

#include <AnalysisTree/TaskManager.hpp>
#include <cassert>
#include <vector>

#include "AnalysisTree/Matching.hpp"

ClassImp(CbmTofHitsConverter);

void CbmTofHitsConverter::Init()
{
  assert(!out_branch_.empty());
  auto* ioman = FairRootManager::Instance();

  cbm_tof_hits_      = (TClonesArray*) ioman->GetObject("TofHit");
  cbm_global_tracks_ = (TClonesArray*) ioman->GetObject("GlobalTrack");
  cbm_tof_match_     = (TClonesArray*) ioman->GetObject("TofHitMatch");
  cbm_tof_points_    = (TClonesArray*) ioman->GetObject("TofPoint");
  cbm_mc_tracks_     = (TClonesArray*) ioman->GetObject("MCTrack");

  AnalysisTree::BranchConfig tof_branch(out_branch_, AnalysisTree::DetType::kHit);
  tof_branch.AddField<float>("mass2", "Mass squared");
  tof_branch.AddField<float>("l", "Track lenght");
  tof_branch.AddField<float>("t", "ps(?), Measured time ");
  tof_branch.AddField<float>("qp_tof", "charge * momentum extrapoleted to TOF");
  tof_branch.AddFields<float>({"dx", "dy", "dz"}, "Distance between TOF hit and extrapolated global track, cm");
  tof_branch.AddField<int>("mc_pdg", "MC-true PDG code of particle with highest contribution to TOF hit");

  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(out_branch_, tof_hits_, tof_branch);
  man->AddMatching(match_to_, out_branch_, vtx_tracks_2_tof_);
  man->AddMatching(out_branch_, mc_tracks_, tof_hits_2_mc_tracks_);
}

void CbmTofHitsConverter::ExtrapolateStraightLine(FairTrackParam* params, float z)
{
  const Float_t Tx    = params->GetTx();
  const Float_t Ty    = params->GetTy();
  const Float_t old_z = params->GetZ();
  const Float_t dz    = z - old_z;

  const Float_t x = params->GetX() + Tx * dz;
  const Float_t y = params->GetY() + Ty * dz;

  params->SetPosition({x, y, z});
}


void CbmTofHitsConverter::FillTofHits()
{
  assert(cbm_tof_hits_);
  tof_hits_->ClearChannels();
  vtx_tracks_2_tof_->Clear();
  tof_hits_2_mc_tracks_->Clear();

  auto* out_config_  = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = out_config_->GetBranchConfig(out_branch_);

  const int i_mass2 = branch.GetFieldId("mass2");
  const int i_qp    = branch.GetFieldId("qp_tof");
  const int i_dx    = branch.GetFieldId("dx");
  const int i_t     = branch.GetFieldId("t");
  const int i_l     = branch.GetFieldId("l");

  auto rec_tracks_map = GetMatchMap(match_to_);
  auto sim_tracks_map = GetMatchMap(mc_tracks_);

  tof_hits_->Reserve(cbm_global_tracks_->GetEntriesFast());

  for (Int_t igt = 0; igt < cbm_global_tracks_->GetEntriesFast(); igt++) {
    const auto* globalTrack = static_cast<const CbmGlobalTrack*>(cbm_global_tracks_->At(igt));
    const Int_t tofHitIndex = globalTrack->GetTofHitIndex();
    if (tofHitIndex < 0) continue;

    const auto* tofHit = static_cast<const CbmTofHit*>(cbm_tof_hits_->At(tofHitIndex));

    FairTrackParam param_last = *(globalTrack->GetParamLast());
    TVector3 p_tof;
    param_last.Momentum(p_tof);

    const Float_t p    = p_tof.Mag();
    const Int_t q      = param_last.GetQp() > 0 ? 1 : -1;
    const Float_t l    = globalTrack->GetLength();  // l is calculated by global tracking
    const Float_t time = tofHit->GetTime();
    const Float_t beta = l / (time * 29.9792458);
    const Float_t m2   = p * p * (1. / (beta * beta) - 1.);

    const Float_t hitX = tofHit->GetX();
    const Float_t hitY = tofHit->GetY();
    const Float_t hitZ = tofHit->GetZ();

    ExtrapolateStraightLine(&param_last, tofHit->GetZ());

    auto& hit = tof_hits_->AddChannel(branch);
    hit.SetPosition(hitX, hitY, hitZ);
    hit.SetSignal(time);
    hit.SetField(m2, i_mass2);
    hit.SetField(float(q * p_tof.Mag()), i_qp);
    hit.SetField(float(param_last.GetX() - hitX), i_dx);
    hit.SetField(float(param_last.GetY() - hitY), i_dx + 1);
    hit.SetField(float(param_last.GetZ() - hitZ), i_dx + 2);
    hit.SetField(l, i_l);
    hit.SetField(time, i_t);

    if (rec_tracks_map.empty()) { continue; }
    const Int_t stsTrackIndex = globalTrack->GetStsTrackIndex();
    if (rec_tracks_map.find(stsTrackIndex) != rec_tracks_map.end()) {
      vtx_tracks_2_tof_->AddMatch(rec_tracks_map.find(stsTrackIndex)->second, hit.GetId());
    }

    const auto* tofMatch = dynamic_cast<CbmMatch*>(cbm_tof_match_->At(tofHitIndex));
    if (tofMatch && tofMatch->GetNofLinks() > 0) {
      const auto* tofPoint = dynamic_cast<FairMCPoint*>(cbm_tof_points_->At(tofMatch->GetMatchedLink().GetIndex()));
      if (!tofPoint) { throw std::runtime_error("no TOF point"); }

      Int_t mc_track_id = tofPoint->GetTrackID();
      if (mc_track_id >= 0) {
        auto it = sim_tracks_map.find(mc_track_id);
        if (it != sim_tracks_map.end()) {  // match is found
          tof_hits_2_mc_tracks_->AddMatch(hit.GetId(), it->second);
        }
      }
    }
  }
}

void CbmTofHitsConverter::Exec() { FillTofHits(); }

CbmTofHitsConverter::~CbmTofHitsConverter()
{
  delete tof_hits_;
  delete vtx_tracks_2_tof_;
};
