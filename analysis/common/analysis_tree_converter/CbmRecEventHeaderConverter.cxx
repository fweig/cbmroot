#include "CbmRecEventHeaderConverter.h"

#include "CbmPsdHit.h"
#include "CbmVertex.h"

#include "FairMCEventHeader.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

#include <AnalysisTree/TaskManager.hpp>

#include "cassert"
#include "iostream"

ClassImp(CbmRecEventHeaderConverter)

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

  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(out_branch_, rec_event_header_, RecEventHeaderBranch);
  rec_event_header_->Init(RecEventHeaderBranch);
}

void CbmRecEventHeaderConverter::Exec()
{

  auto* out_config_  = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = out_config_->GetBranchConfig(out_branch_);

  if (!cbm_prim_vertex_) { throw std::runtime_error("No fPrimVtx"); }

  rec_event_header_->SetVertexPosition3({cbm_prim_vertex_->GetX(), cbm_prim_vertex_->GetY(), cbm_prim_vertex_->GetZ()});
  rec_event_header_->SetField(int(cbm_sts_tracks_->GetEntries()), branch.GetFieldId("M"));
  rec_event_header_->SetField(GetPsdEnergy(), branch.GetFieldId("Epsd"));
  rec_event_header_->SetField(int(cbm_header_->GetEventID()), branch.GetFieldId("evt_id"));
  rec_event_header_->SetField(float(cbm_prim_vertex_->GetChi2() / cbm_prim_vertex_->GetNDF()),
                              branch.GetFieldId("vtx_chi2"));
}

float CbmRecEventHeaderConverter::GetPsdEnergy()
{
  //TODO avoid duplicating the code with PsdModulesConverter
  float psd_energy {0.f};
  const int nPsdHits = cbm_psd_hits_->GetEntriesFast();
  for (int i = 0; i < nPsdHits; ++i) {
    auto* hit = (CbmPsdHit*) cbm_psd_hits_->At(i);
    if (hit == nullptr) continue;
    psd_energy += hit->GetEdep();
  }
  return psd_energy;
}
