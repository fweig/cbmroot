
#include <cassert>
#include <cmath>

#include "TClonesArray.h"

#include "FairRootManager.h"

#include "AnalysisTree/Matching.hpp"

#include "CbmMCTrack.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"
#include "CbmVertex.h"
#include "Interface/CbmKFVertex.h"
#include "L1Field.h"
#include "ParticleFinder/CbmL1PFFitter.h"

#include "CbmStsTracksConverter.h"

ClassImp(CbmStsTracksConverter)

  void CbmStsTracksConverter::Exec() {
  assert(cbm_sts_tracks_ != nullptr && cbm_mc_tracks_ != nullptr);

  vtx_tracks_2_sim_->Clear();
  out_indexes_map_.clear();

  ReadVertexTracks();
  MapTracks();
}

CbmStsTracksConverter::~CbmStsTracksConverter() {
  delete vtx_tracks_;
  delete vtx_tracks_2_sim_;
};

// TODO misleading name, move field filling somewhere else?
float CbmStsTracksConverter::ExtrapolateToVertex(CbmStsTrack* sts_track,
                                                 AnalysisTree::Track* track,
                                                 int pdg) {

  vector<CbmStsTrack> tracks = {*sts_track};
  CbmL1PFFitter fitter;
  vector<float> chi2_to_vtx;
  vector<L1FieldRegion> field;
  CbmKFVertex kfVertex = CbmKFVertex(*cbm_prim_vertex_);
  if (is_reproduce_cbmkfpf_) {
    std::vector<int> pdgVector = {pdg};
    fitter.Fit(tracks, pdgVector);
  }
  fitter.GetChiToVertex(tracks, field, chi2_to_vtx, kfVertex, 3.);
  *sts_track = tracks[0];

  if (is_write_kfinfo_) {
    track->SetField(float(field.at(0).cx0[0]), imf_);
    track->SetField(float(field.at(0).cx1[0]), imf_ + 1);
    track->SetField(float(field.at(0).cx2[0]), imf_ + 2);
    track->SetField(float(field.at(0).cy0[0]), imf_ + 3);
    track->SetField(float(field.at(0).cy1[0]), imf_ + 4);
    track->SetField(float(field.at(0).cy2[0]), imf_ + 5);
    track->SetField(float(field.at(0).cz0[0]), imf_ + 6);
    track->SetField(float(field.at(0).cz1[0]), imf_ + 7);
    track->SetField(float(field.at(0).cz2[0]), imf_ + 8);
    track->SetField(float(field.at(0).z0[0]), imf_ + 9);
  }

  return chi2_to_vtx[0];
}

void CbmStsTracksConverter::ReadVertexTracks() {
  assert(cbm_prim_vertex_ && cbm_sts_tracks_);

  vtx_tracks_->ClearChannels();

  const int iq =
    out_config_->GetBranchConfig(vtx_tracks_->GetId()).GetFieldId("q");
  const int indf =
    out_config_->GetBranchConfig(vtx_tracks_->GetId()).GetFieldId("ndf");
  const int ichi2 =
    out_config_->GetBranchConfig(vtx_tracks_->GetId()).GetFieldId("chi2");
  const int inhits =
    out_config_->GetBranchConfig(vtx_tracks_->GetId()).GetFieldId("nhits");
  const int inhits_mvd =
    out_config_->GetBranchConfig(vtx_tracks_->GetId()).GetFieldId("nhits_mvd");
  const int idcax =
    out_config_->GetBranchConfig(vtx_tracks_->GetId()).GetFieldId("dcax");
  const int ivtx_chi2 =
    out_config_->GetBranchConfig(vtx_tracks_->GetId()).GetFieldId("vtx_chi2");

  const int n_sts_tracks = cbm_sts_tracks_->GetEntries();
  vtx_tracks_->Reserve(n_sts_tracks);

  for (short i_track = 0; i_track < n_sts_tracks; ++i_track) {
    auto* sts_track = (CbmStsTrack*) cbm_sts_tracks_->At(i_track);
    if (!sts_track) { throw std::runtime_error("empty track!"); }

    auto* track = vtx_tracks_->AddChannel();
    track->Init(out_config_->GetBranchConfig(vtx_tracks_->GetId()));

    int pdg = GetMcPid(
      sts_track, (CbmTrackMatchNew*) cbm_sts_match_->At(i_track), track);

    float chi2_vertex = ExtrapolateToVertex(sts_track, track, pdg);
    const FairTrackParam* trackParamFirst = sts_track->GetParamFirst();
    TVector3 momRec;
    trackParamFirst->Momentum(momRec);
    const Int_t q = trackParamFirst->GetQp() > 0 ? 1 : -1;

    track->SetMomentum3(momRec);
    track->SetField(int(q), iq);
    track->SetField(int(sts_track->GetNDF()), indf);
    track->SetField(float(sts_track->GetChiSq()), ichi2);
    track->SetField(int(sts_track->GetNofHits()), inhits);
    track->SetField(float(trackParamFirst->GetX() - cbm_prim_vertex_->GetX()),
                    idcax);
    track->SetField(float(trackParamFirst->GetY() - cbm_prim_vertex_->GetY()),
                    idcax + 1);
    track->SetField(float(trackParamFirst->GetZ() - cbm_prim_vertex_->GetZ()),
                    idcax + 2);
    track->SetField(int(sts_track->GetNofMvdHits()), inhits_mvd);
    track->SetField(float(chi2_vertex), ivtx_chi2);

    out_indexes_map_.insert(std::make_pair(i_track, track->GetId()));

    if (is_write_kfinfo_) { WriteKFInfo(track, sts_track); }
  }
}

void CbmStsTracksConverter::WriteKFInfo(AnalysisTree::Track* track,
                                        const CbmStsTrack* sts_track) const {
  assert(track && sts_track);
  const FairTrackParam* trackParamFirst = sts_track->GetParamFirst();

  track->SetField(float(trackParamFirst->GetX()), ipar_);
  track->SetField(float(trackParamFirst->GetY()), ipar_ + 1);
  track->SetField(float(trackParamFirst->GetZ()), ipar_ + 2);
  track->SetField(float(trackParamFirst->GetTx()), ipar_ + 3);
  track->SetField(float(trackParamFirst->GetTy()), ipar_ + 4);
  track->SetField(float(trackParamFirst->GetQp()), ipar_ + 5);

  for (Int_t i = 0, iCov = 0; i < 5; i++) {
    for (Int_t j = 0; j <= i; j++, iCov++) {
      track->SetField(float(trackParamFirst->GetCovariance(i, j)),
                      icov_ + iCov);
    }
  }
  track->SetField(IsGoodCovMatrix(sts_track), ipasscuts_);
}

bool CbmStsTracksConverter::IsGoodCovMatrix(
  const CbmStsTrack* sts_track) const {

  if (!is_reproduce_cbmkfpf_) return true;

  assert(sts_track);
  const FairTrackParam* trackParamFirst = sts_track->GetParamFirst();

  Double_t cov_matrix[15] = {0.f};
  for (Int_t i = 0, iCov = 0; i < 5; i++) {
    for (Int_t j = 0; j <= i; j++, iCov++) {
      cov_matrix[iCov] = trackParamFirst->GetCovariance(i, j);
    }
  }
  // Cuts, coded in MZ's CbmKFParticleFinder.cxx
  bool ok = true;
  ok      = ok && isfinite(sts_track->GetParamFirst()->GetX());
  ok      = ok && isfinite(sts_track->GetParamFirst()->GetY());
  ok      = ok && isfinite(sts_track->GetParamFirst()->GetZ());
  ok      = ok && isfinite(sts_track->GetParamFirst()->GetTx());
  ok      = ok && isfinite(sts_track->GetParamFirst()->GetTy());
  ok      = ok && isfinite(sts_track->GetParamFirst()->GetQp());

  for (auto element : cov_matrix) {
    ok = ok && isfinite(element);
  }
  ok = ok && (cov_matrix[0] < 1. && cov_matrix[0] > 0.)
       && (cov_matrix[2] < 1. && cov_matrix[2] > 0.)
       && (cov_matrix[5] < 1. && cov_matrix[5] > 0.)
       && (cov_matrix[9] < 1. && cov_matrix[9] > 0.)
       && (cov_matrix[14] < 1. && cov_matrix[14] > 0.);

  ok = ok && sts_track->GetChiSq() < 10. * sts_track->GetNDF();

  return ok;
}

int CbmStsTracksConverter::GetMcPid(const CbmStsTrack* /*sts_track*/,
                                    const CbmTrackMatchNew* match,
                                    AnalysisTree::Track* track) const {

  if (!is_write_kfinfo_) { return -2; }
  //-----------PID as in MZ's
  //CbmKFPF----------------------------------------------------------
  Int_t nMCTracks = cbm_mc_tracks_->GetEntriesFast();

  int pdgCode = -2;
  if (match->GetNofLinks() > 0) {  // there is at least one matched MC-track
    Float_t bestWeight  = 0.f;
    Float_t totalWeight = 0.f;
    Int_t mcTrackId     = -1;

    for (int iLink = 0; iLink < match->GetNofLinks(); iLink++) {
      totalWeight += match->GetLink(iLink).GetWeight();
      if (match->GetLink(iLink).GetWeight() > bestWeight) {
        bestWeight = match->GetLink(iLink).GetWeight();
        mcTrackId  = match->GetLink(iLink).GetIndex();
      }
    }

    if (!((bestWeight / totalWeight < 0.7)
          || (mcTrackId >= nMCTracks || mcTrackId < 0))) {
      auto* mctrack = static_cast<CbmMCTrack*>(cbm_mc_tracks_->At(mcTrackId));

      if (!(TMath::Abs(mctrack->GetPdgCode()) == 11
            || TMath::Abs(mctrack->GetPdgCode()) == 13
            || TMath::Abs(mctrack->GetPdgCode()) == 211
            || TMath::Abs(mctrack->GetPdgCode()) == 321
            || TMath::Abs(mctrack->GetPdgCode()) == 2212
            || TMath::Abs(mctrack->GetPdgCode()) == 1000010020
            || TMath::Abs(mctrack->GetPdgCode()) == 1000010030
            || TMath::Abs(mctrack->GetPdgCode()) == 1000020030
            || TMath::Abs(mctrack->GetPdgCode()) == 1000020040)) {
        pdgCode = -1;
      } else {
        pdgCode = mctrack->GetPdgCode();
      }
      if (mctrack->GetMotherId() > -1) {
        track->SetField(
          int(((CbmMCTrack*) cbm_mc_tracks_->At(mctrack->GetMotherId()))
                ->GetPdgCode()),
          imother_pdg_);
      }
    }
  }
  track->SetField(pdgCode, imc_pdg_);

  return pdgCode;
}

void CbmStsTracksConverter::InitInput() {
  auto* ioman = FairRootManager::Instance();

  cbm_prim_vertex_ =
    (CbmVertex*) ioman->GetObject(in_branches_.at(ePrimiryVertex).c_str());
  cbm_sts_tracks_ =
    (TClonesArray*) ioman->GetObject(in_branches_.at(eStsTracks).c_str());
  cbm_mc_tracks_ =
    (TClonesArray*) ioman->GetObject(in_branches_.at(eSimTracks).c_str());
  cbm_sts_match_ = (TClonesArray*) ioman->GetObject("StsTrackMatch");
}

void CbmStsTracksConverter::Init(map<std::string, void*>& branches) {
  InitInput();

  AnalysisTree::BranchConfig vtx_tracks_config(out_branch_,
                                               AnalysisTree::DetType::kTrack);
  vtx_tracks_config.AddField<float>("chi2");
  vtx_tracks_config.AddField<float>("vtx_chi2");
  vtx_tracks_config.AddFields<float>({"dcax", "dcay", "dcaz"});
  vtx_tracks_config.AddField<int>("ndf");
  vtx_tracks_config.AddField<int>("q");
  vtx_tracks_config.AddField<int>("nhits");
  vtx_tracks_config.AddField<int>("nhits_mvd");

  if (is_write_kfinfo_) {
    vtx_tracks_config.AddFields<float>({"x", "y", "z", "tx", "ty", "qp"});
    vtx_tracks_config.AddFields<float>(
      {"cx0", "cx1", "cx2", "cy0", "cy1", "cy2", "cz0", "cz1", "cz2", "z0"});
    vtx_tracks_config.AddFields<float>({"cov1",
                                        "cov2",
                                        "cov3",
                                        "cov4",
                                        "cov5",
                                        "cov6",
                                        "cov7",
                                        "cov8",
                                        "cov9",
                                        "cov10",
                                        "cov11",
                                        "cov12",
                                        "cov13",
                                        "cov14",
                                        "cov15"});

    vtx_tracks_config.AddField<int>("mother_pdg");
    vtx_tracks_config.AddField<int>("mc_pdg");
    vtx_tracks_config.AddField<bool>("pass_cuts");

    ipar_        = vtx_tracks_config.GetFieldId("x");
    imf_         = vtx_tracks_config.GetFieldId("cx0");
    icov_        = vtx_tracks_config.GetFieldId("cov1");
    imc_pdg_     = vtx_tracks_config.GetFieldId("mc_pdg");
    imother_pdg_ = vtx_tracks_config.GetFieldId("mother_pdg");
    ipasscuts_   = vtx_tracks_config.GetFieldId("pass_cuts");
  }

  out_config_->AddBranchConfig(std::move(vtx_tracks_config));
  vtx_tracks_       = new AnalysisTree::TrackDetector(out_config_->GetLastId());
  vtx_tracks_2_sim_ = new AnalysisTree::Matching(
    out_config_->GetLastId(), out_config_->GetBranchConfig(match_to_).GetId());

  out_config_->AddMatch(vtx_tracks_2_sim_);

  out_tree_->Branch(
    out_branch_.c_str(), "AnalysisTree::TrackDetector", &vtx_tracks_);
  out_tree_->Branch((out_branch_ + "2" + match_to_).c_str(),
                    "AnalysisTree::Matching",
                    &vtx_tracks_2_sim_);

  branches.emplace(out_branch_, vtx_tracks_);
}

void CbmStsTracksConverter::MapTracks() {
  const auto it = indexes_map_->find(match_to_);
  if (it == indexes_map_->end()) {
    throw std::runtime_error(match_to_
                             + " is not found to match with vertex tracks");
  }
  auto sim_tracks_map = it->second;

  assert(!out_indexes_map_.empty() || !sim_tracks_map.empty());
  CbmTrackMatchNew* match {nullptr};

  for (const auto& track_id : out_indexes_map_) {
    const int cbm_id = track_id.first;
    const int out_id = track_id.second;
    match            = (CbmTrackMatchNew*) cbm_sts_match_->At(cbm_id);
    if (match->GetNofLinks() > 0) {  // there is at least one matched MC-track
      const int mc_track_id = match->GetMatchedLink().GetIndex();

      if (match->GetTrueOverAllHitsRatio()
          < 0.5)  // not enough hits to be a match
        continue;

      auto p = sim_tracks_map.find(mc_track_id);
      if (p == sim_tracks_map.end())  // match is not found
        continue;

      vtx_tracks_2_sim_->AddMatch(out_id, p->second);
    }
  }
}
