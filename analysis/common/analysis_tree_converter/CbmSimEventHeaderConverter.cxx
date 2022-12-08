/* Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universitaet Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov [committer] */

#include "CbmSimEventHeaderConverter.h"

#include "CbmEvent.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCDataObject.h"

#include "FairMCEventHeader.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

#include <AnalysisTree/TaskManager.hpp>

#include "cassert"
#include "iostream"

ClassImp(CbmSimEventHeaderConverter);

void CbmSimEventHeaderConverter::Init()
{
  assert(!out_branch_.empty());
  auto* ioman = FairRootManager::Instance();
  assert(ioman != nullptr);

  cbm_mc_manager_ = dynamic_cast<CbmMCDataManager*>(ioman->GetObject("MCDataManager"));
  cbm_header_obj_ = cbm_mc_manager_->GetObject("MCEventHeader.");

  //  ***** SimEventHeader *******
  AnalysisTree::BranchConfig SimEventHeaderBranch("SimEventHeader", AnalysisTree::DetType::kEventHeader);
  SimEventHeaderBranch.AddField<float>("psi_RP", "reaction plane orientation");
  SimEventHeaderBranch.AddField<float>("b", "fm, impact parameter");
  SimEventHeaderBranch.AddFields<float>({"start_time", "end_time"}, "ns (?), event time");
  SimEventHeaderBranch.AddField<int>("run_id", "run identifier");
  SimEventHeaderBranch.AddField<int>("event_id", "event identifier");

  ipsi_RP_     = SimEventHeaderBranch.GetFieldId("psi_RP");
  ib_          = SimEventHeaderBranch.GetFieldId("b");
  istart_time_ = SimEventHeaderBranch.GetFieldId("start_time");
  iend_time_   = SimEventHeaderBranch.GetFieldId("end_time");
  irun_id_     = SimEventHeaderBranch.GetFieldId("run_id");
  ievent_id_   = SimEventHeaderBranch.GetFieldId("event_id");

  auto* man = AnalysisTree::TaskManager::GetInstance();
  man->AddBranch(sim_event_header_, SimEventHeaderBranch);
  sim_event_header_->Init(SimEventHeaderBranch);
}

void CbmSimEventHeaderConverter::ProcessData(CbmEvent* event)
{
  FairMCEventHeader* cbm_header {nullptr};

  if (event) {
    if (!event->GetMatch()) {
      LOG(error) << "No match to SimEvent!";
      return;
    }
    const auto& link = event->GetMatch()->GetMatchedLink();
    cbm_header       = (FairMCEventHeader*) (cbm_header_obj_->Get(link));
  }
  else {
    cbm_header = (FairMCEventHeader*) (cbm_header_obj_->Get(0, FairRootManager::Instance()->GetEntryNr()));
  }

  LOG(info) << "MCEvent " << cbm_header->GetEventID() << " " << cbm_header->GetT();

  if (!cbm_header) { throw std::runtime_error("CbmSimEventHeaderConverter::Exec - ERROR! No fHeader!"); }
  auto* out_config_  = AnalysisTree::TaskManager::GetInstance()->GetConfig();
  const auto& branch = out_config_->GetBranchConfig(out_branch_);

  TVector3 pos {cbm_header->GetX(), cbm_header->GetY(), cbm_header->GetZ()};
  sim_event_header_->SetVertexPosition3(pos);

  sim_event_header_->SetField(float(cbm_header->GetRotZ()), ipsi_RP_);
  sim_event_header_->SetField(float(cbm_header->GetB()), ib_);
  sim_event_header_->SetField(int(cbm_header->GetEventID()), ievent_id_);
  sim_event_header_->SetField(int(cbm_header->GetRunID()), irun_id_);

  if (event) {
    LOG(info) << "TIME: " << event->GetStartTime() << "  " << event->GetEndTime();
    sim_event_header_->SetField(float(event->GetStartTime()), istart_time_);
    sim_event_header_->SetField(float(event->GetEndTime()), iend_time_);
  }
}
