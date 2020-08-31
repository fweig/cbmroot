#include <iostream>

#include "AnalysisTree/DataHeader.hpp"
#include "AnalysisTree/TaskManager.hpp"

#include "TGeoBBox.h"
#include "TGeoManager.h"

#include "CbmConverterManager.h"
#include "CbmConverterTask.h"

ClassImp(CbmConverterManager)

  InitStatus CbmConverterManager::Init() {
  assert(!out_file_name_.empty() && !out_tree_name_.empty());

  out_file_ = TFile::Open(out_file_name_.c_str(), "RECREATE");
  out_file_->cd();
  out_tree_ = new TTree(out_tree_name_.c_str(), "Analysis Tree");

  FillDataHeader();

  std::map<std::string, void*> unused_map {};

  out_config_ = new AnalysisTree::Configuration;
  for (auto* task : tasks_) {
    task->SetOutTree(out_tree_);
    task->SetOutConfiguration(out_config_);
    task->SetDataHeader(data_header_);
    task->Init(unused_map);
  }

  out_config_->Write("Configuration");

  return kSUCCESS;
}

void CbmConverterManager::Exec(Option_t* /*opt*/) {
  index_map_.clear();
  for (auto* task : tasks_) {
    task->SetIndexesMap(&index_map_);
    task->Exec();
    //    if(!task->GetOutIndexesMap().empty()) {
    index_map_.insert(
      std::make_pair(task->GetOutputBranchName(), task->GetOutIndexesMap()));
    //    }
  }
  out_tree_->Fill();
}

void CbmConverterManager::Finish() {
  TDirectory* old_dir = gDirectory;

  out_file_->cd();
  for (auto* task : tasks_) {
    task->Finish();
  }
  out_tree_->Write();
  out_file_->Close();

  old_dir->cd();
}

void CbmConverterManager::FillDataHeader() {

  assert(
    !system_.empty()
    && beam_mom_);  // Force user to write data info //TODO is there a way to
                    // read it from a file automatically?

  data_header_ = new AnalysisTree::DataHeader;

  std::cout << "ReadDataHeader" << std::endl;
  data_header_->SetSystem(system_);
  data_header_->SetBeamMomentum(beam_mom_);

  auto& psd_mod_pos              = data_header_->AddDetector();
  const int psd_node_id          = 6;
  const char* module_name_prefix = "module";

  auto* geoMan   = gGeoManager;
  auto* caveNode = geoMan->GetTopNode();
  auto* psdNode  = caveNode->GetDaughter(psd_node_id);
  std::cout << "-I- " << psdNode->GetName() << std::endl;

  auto psdGeoMatrix = psdNode->GetMatrix();
  auto psdBox       = (TGeoBBox*) psdNode->GetVolume()->GetShape();
  TVector3 frontFaceLocal(0, 0, -psdBox->GetDZ());

  TVector3 frontFaceGlobal;
  psdGeoMatrix->LocalToMaster(&frontFaceLocal[0], &frontFaceGlobal[0]);

  for (int i_d = 0; i_d < psdNode->GetNdaughters(); ++i_d) {
    auto* daughter = psdNode->GetDaughter(i_d);
    TString daughterName(daughter->GetName());
    if (daughterName.BeginsWith(module_name_prefix)) {

      auto geoMatrix = daughter->GetMatrix();
      TVector3 translation(geoMatrix->GetTranslation());

      int modID = daughter->GetNumber();
      double x  = translation.X();
      double y  = translation.Y();

      std::cout << "mod" << modID << " : " << Form("(%.3f, %3f)", x, y)
                << std::endl;

      auto* module = psd_mod_pos.AddChannel();
      module->SetPosition(x, y, frontFaceGlobal[2]);
    }
  }

  TDirectory* curr   = gDirectory;  // TODO check why this is needed
  TFile* currentFile = gFile;

  out_file_->cd();
  data_header_->Write("DataHeader");

  gFile      = currentFile;
  gDirectory = curr;
}
CbmConverterManager::~CbmConverterManager() {
  delete out_config_;
  delete data_header_;
  delete out_tree_;
  delete out_file_;
}
