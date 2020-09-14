#ifndef ATKFParticleFinder_HH
#define ATKFParticleFinder_HH

#include <string>
#include <utility>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

#include "AnalysisTree/Configuration.hpp"
#include "AnalysisTree/Detector.hpp"
#include "AnalysisTree/EventHeader.hpp"

#include "CutsContainer.h"
#include "KFParticleTopoReconstructor.h"

class ATKFParticleFinder {
public:
  void InitInput(const std::string& file_name, const std::string& tree_name);
  void InitOutput(const std::string& file_name = "KFPF_output.root");
  void Finish();
  void SetCuts(CutsContainer cuts) { cuts_ = cuts; };
  //   const KFParticleTopoReconstructor* GetTopoReconstructor() const {return topo_reconstructor_;};
  void SetPIDMode(int pid) { pid_mode_ = pid; };
  void WriteCandidates(const KFParticleTopoReconstructor* TR);
  void Run(int n_events = -1);

private:
  KFParticleTopoReconstructor* CreateTopoReconstructor();

  std::string in_file_name_;
  std::string in_tree_name_;
  TFile* in_file_ {nullptr};
  TChain* in_chain_ {nullptr};
  AnalysisTree::Configuration* config_ {nullptr};
  AnalysisTree::EventHeader* rec_event_header_ {nullptr};
  AnalysisTree::TrackDetector* kf_tracks_ {nullptr};

  TFile* out_file_ {nullptr};
  TTree* out_tree_ {nullptr};
  AnalysisTree::Configuration out_config_;
  AnalysisTree::Particles* particles_reco_ {nullptr};

  int q_field_id_ {-999};
  int par_field_id_ {-999};
  int mf_field_id_ {-999};
  int cov_field_id_ {-999};
  int passcuts_field_id_ {-999};
  int pdg_field_id_ {-999};
  int vtx_chi2_field_id_ {-999};
  int nhits_field_id_ {-999};
  int nhits_mvd_field_id_ {-999};

  int daughter1_id_field_id_ {-999};
  int daughter2_id_field_id_ {-999};

  CutsContainer cuts_;
  //   KFParticleTopoReconstructor* topo_reconstructor_{nullptr};

  std::vector<float> GetCovMatrixCbm(const AnalysisTree::Track& track) const;

  int pid_mode_ {1};
};
#endif  //ATKFParticleFinder_HH