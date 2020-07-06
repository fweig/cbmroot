#include "CbmVertex.h"
#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "cassert"

#include "CbmRecEventHeaderConverter.h"

ClassImp(CbmRecEventHeaderConverter)

  void CbmRecEventHeaderConverter::Init(std::map<std::string, void*>&) {
  assert(!in_branches_.empty() && !out_branch_.empty() && out_config_
         && out_tree_);
  auto* ioman = FairRootManager::Instance();
  assert(ioman != nullptr);
  cbm_header_ = (FairMCEventHeader*) ioman->GetObject(in_branches_[0].c_str());
  cbm_prim_vertex_ = (CbmVertex*) ioman->GetObject(in_branches_[1].c_str());

  //  ***** RecEventHeader *******
  AnalysisTree::BranchConfig RecEventHeaderBranch(
    "RecEventHeader", AnalysisTree::DetType::kEventHeader);
  RecEventHeaderBranch.AddField<float>("psi_EP");
  RecEventHeaderBranch.AddField<float>("vtx_chi2");
  RecEventHeaderBranch.AddField<float>("Epsd");
  RecEventHeaderBranch.AddField<int>("M");
  RecEventHeaderBranch.AddField<int>("evt_id");
  out_config_->AddBranchConfig(RecEventHeaderBranch);
  rec_event_header_ = new AnalysisTree::EventHeader(out_config_->GetLastId());
  rec_event_header_->Init(RecEventHeaderBranch);
  out_tree_->Branch(
    out_branch_.c_str(), "AnalysisTree::EventHeader", &rec_event_header_);
}

void CbmRecEventHeaderConverter::Exec() {
  const auto& conf = out_config_->GetBranchConfig(rec_event_header_->GetId());
  if (!cbm_prim_vertex_) {
    std::cout << "WARNING! No fPrimVtx!" << std::endl;

    rec_event_header_->SetVertexPosition3({-999., -999., -999.});
    rec_event_header_->SetField(float(-999.), conf.GetFieldId("vtx_chi2"));
    return;
  }
  rec_event_header_->SetVertexPosition3({cbm_prim_vertex_->GetX(),
                                         cbm_prim_vertex_->GetY(),
                                         cbm_prim_vertex_->GetZ()});
  //  rec_event_header_ -> SetField(int(cbm_sts_tracks_->GetEntries()),
  //  config_.GetBranchConfig(rec_event_header_->GetId() ).GetFieldId("M"));
  rec_event_header_->SetField(int(cbm_header_->GetEventID()),
                              conf.GetFieldId("evt_id"));
  rec_event_header_->SetField(
    float(cbm_prim_vertex_->GetChi2() / cbm_prim_vertex_->GetNDF()),
    conf.GetFieldId("vtx_chi2"));
}
