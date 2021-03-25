#include "ATKFParticleFinder.h"

void ATKFParticleFinder::InitInput(const std::string& file_name,
                                   const std::string& tree_name) {
  std::cout << "ATKFParticleFinder::InitInput()\n";

  //   topo_reconstructor_ = new KFParticleTopoReconstructor;
  //   topo_reconstructor_->Clear();

  in_file_ = TFile::Open(file_name.c_str(), "read");
  config_  = (AnalysisTree::Configuration*) in_file_->Get("Configuration");

  in_chain_ = new TChain(tree_name.c_str());
  in_chain_->Add(file_name.c_str());
  in_chain_->SetBranchAddress("VtxTracks", &kf_tracks_);
  in_chain_->SetBranchAddress("RecEventHeader", &rec_event_header_);

  auto branch_conf_kftr = config_->GetBranchConfig("VtxTracks");
  q_field_id_           = branch_conf_kftr.GetFieldId("q");

  par_field_id_ = branch_conf_kftr.GetFieldId("x");     // par0
  mf_field_id_  = branch_conf_kftr.GetFieldId("cx0");   // magnetic field par0
  cov_field_id_ = branch_conf_kftr.GetFieldId("cov1");  // cov matrix 0

  passcuts_field_id_  = branch_conf_kftr.GetFieldId("pass_cuts");
  pdg_field_id_       = branch_conf_kftr.GetFieldId("mc_pdg");
  nhits_field_id_     = branch_conf_kftr.GetFieldId("nhits");
  nhits_mvd_field_id_ = branch_conf_kftr.GetFieldId("nhits_mvd");
  vtx_chi2_field_id_  = branch_conf_kftr.GetFieldId("vtx_chi2");
}

void ATKFParticleFinder::InitOutput(const std::string& file_name) {
  std::cout << "ATKFParticleFinder::InitOutput()\n";

  out_file_ = TFile::Open(file_name.c_str(), "recreate");

  AnalysisTree::BranchConfig ParticlesRecoBranch(
    "ParticlesReconstructed", AnalysisTree::DetType::kParticle);

  ParticlesRecoBranch.AddField<int>("daughter1id");
  ParticlesRecoBranch.AddField<int>("daughter2id");

  out_config_.AddBranchConfig(ParticlesRecoBranch);
  particles_reco_ = new AnalysisTree::Particles(out_config_.GetBranchConfig("ParticlesReconstructed").GetId());

  out_tree_ = new TTree("aTree", "AnalysisTree ParticlesReco");
  out_tree_->Branch(
    "ParticlesReconstructed", "AnalysisTree::Particles", &particles_reco_);
  out_config_.Write("Configuration");

  daughter1_id_field_id_ = out_config_.GetBranchConfig(particles_reco_->GetId())
                             .GetFieldId("daughter1id");
  daughter2_id_field_id_ = out_config_.GetBranchConfig(particles_reco_->GetId())
                             .GetFieldId("daughter2id");
}

void ATKFParticleFinder::Finish() {
  std::cout << "ATKFParticleFinder::Finish()\n";

  out_tree_->Write();
  out_file_->Close();
}

void ATKFParticleFinder::Run(int n_events) {
  if (n_events < 0 || n_events > in_chain_->GetEntries())
    n_events = in_chain_->GetEntries();

  std::cout << "ATKFParticleFinder::Run() " << n_events << " events\n";

  for (int i_event = 0; i_event < n_events; i_event++) {
    std::cout << "eveNo = " << i_event << "\n";
    in_chain_->GetEntry(i_event);
    KFParticleTopoReconstructor* eventTopoReconstructor =
      CreateTopoReconstructor();

    //     const KFPTrackVector* tv = eventTopoReconstructor->GetTracks();
    //     KFPTrackVector tvv = *tv;
    //     tvv.Print();

    eventTopoReconstructor->SortTracks();
    eventTopoReconstructor->ReconstructParticles();

    WriteCandidates(eventTopoReconstructor);
  }
  Finish();
}

KFParticleTopoReconstructor* ATKFParticleFinder::CreateTopoReconstructor() {
  //
  // Creates the pointer on the KFParticleTopoReconstructor object
  // with all necessary input information in order to perform particle selection using
  // non-simplified "standard" KFParticle algorithm.
  //
  auto* TR = new KFParticleTopoReconstructor;

  // cuts setting
  TR->GetKFParticleFinder()->SetChiPrimaryCut2D(cuts_.GetCutChi2Prim());
  TR->GetKFParticleFinder()->SetMaxDistanceBetweenParticlesCut(
    cuts_.GetCutDistance());
  TR->GetKFParticleFinder()->SetChi2Cut2D(cuts_.GetCutChi2Geo());
  TR->GetKFParticleFinder()->SetLCut(cuts_.GetCutLDown());
  TR->GetKFParticleFinder()->SetLdLCut2D(cuts_.GetCutLdL());

  int n_good_tracks = 0;

  for (unsigned int i_track = 0; i_track < kf_tracks_->GetNumberOfChannels();
       i_track++) {
    const AnalysisTree::Track& rec_track = kf_tracks_->GetChannel(i_track);
    if (rec_track.GetField<bool>(passcuts_field_id_) == 0) continue;
    if (rec_track.GetField<int>(pdg_field_id_) == -2 && pid_mode_ == 1)
      continue;
    n_good_tracks++;
  }

  KFPTrackVector track_vector1, track_vector2;
  track_vector1.Resize(n_good_tracks);

  int j_track = 0;

  for (unsigned int i_track = 0; i_track < kf_tracks_->GetNumberOfChannels();
       i_track++) {
    const AnalysisTree::Track& rec_track = kf_tracks_->GetChannel(i_track);

    if (rec_track.GetField<bool>(passcuts_field_id_) == 0) continue;
    if (rec_track.GetField<int>(pdg_field_id_) == -2 && pid_mode_ == 1)
      continue;

    for (Int_t iP = 0; iP < 3; iP++)
      track_vector1.SetParameter(
        rec_track.GetField<float>(par_field_id_ + iP), iP, j_track);
    track_vector1.SetParameter(rec_track.GetPx(), 3, j_track);
    track_vector1.SetParameter(rec_track.GetPy(), 4, j_track);
    track_vector1.SetParameter(rec_track.GetPz(), 5, j_track);
    auto cov_matrix = GetCovMatrixCbm(rec_track);
    for (Int_t iC = 0; iC < 21; iC++)
      track_vector1.SetCovariance(cov_matrix.at(iC), iC, j_track);
    for (Int_t iF = 0; iF < 10; iF++)
      track_vector1.SetFieldCoefficient(
        rec_track.GetField<float>(mf_field_id_ + iF), iF, j_track);
    if (pid_mode_ == 0)
      track_vector1.SetPDG(-1, j_track);
    else if (pid_mode_ == 1)
      track_vector1.SetPDG(rec_track.GetField<int>(pdg_field_id_), j_track);
    track_vector1.SetQ(rec_track.GetField<int>(q_field_id_), j_track);
    if (rec_track.GetField<float>(vtx_chi2_field_id_) < 3.)
      track_vector1.SetPVIndex(0, j_track);
    else
      track_vector1.SetPVIndex(-1, j_track);
    track_vector1.SetNPixelHits(rec_track.GetField<int>(nhits_mvd_field_id_),
                                j_track);
    track_vector1.SetId(rec_track.GetId(), j_track);
    j_track++;
  }
  TR->Init(track_vector1, track_vector2);

  KFPVertex primVtx_tmp;
  primVtx_tmp.SetXYZ(rec_event_header_->GetVertexX(),
                     rec_event_header_->GetVertexY(),
                     rec_event_header_->GetVertexZ());
  primVtx_tmp.SetCovarianceMatrix(0, 0, 0, 0, 0, 0);
  primVtx_tmp.SetNContributors(0);
  primVtx_tmp.SetChi2(-100);
  std::vector<int> pvTrackIds;
  KFVertex pv(primVtx_tmp);
  TR->AddPV(pv, pvTrackIds);

  std::cout << track_vector1.Size() << "\n";

  return TR;
}

void ATKFParticleFinder::WriteCandidates(
  const KFParticleTopoReconstructor* eventTR) {
  particles_reco_->ClearChannels();

  for (const auto & particle : eventTR->GetParticles()) {
    auto* particlerec = particles_reco_->AddChannel();
    particlerec->Init(out_config_.GetBranchConfig(particles_reco_->GetId()));

    float mass, masserr;
    particle.GetMass(mass, masserr);
    particlerec->SetMass(mass);
    particlerec->SetField(particle.DaughterIds()[0], daughter1_id_field_id_);
    particlerec->SetField(particle.DaughterIds()[1], daughter2_id_field_id_);
    particlerec->SetMomentum(
      particle.GetPx(), particle.GetPy(), particle.GetPz());
    particlerec->SetPid(particle.GetPDG());

    //     topo_reconstructor_->AddParticle(particle);
  }
  out_tree_->Fill();
}

std::vector<float>
ATKFParticleFinder::GetCovMatrixCbm(const AnalysisTree::Track& track) const {
  const double tx = track.GetField<float>(par_field_id_ + 3);
  const double ty = track.GetField<float>(par_field_id_ + 4);
  const double qp = track.GetField<float>(par_field_id_ + 5);
  const Int_t q   = track.GetField<int>(q_field_id_);

  //calculate covariance matrix
  const double t      = sqrt(1.f + tx * tx + ty * ty);
  const double t3     = t * t * t;
  const double dpxdtx = q / qp * (1.f + ty * ty) / t3;
  const double dpxdty = -q / qp * tx * ty / t3;
  const double dpxdqp = -q / (qp * qp) * tx / t;
  const double dpydtx = -q / qp * tx * ty / t3;
  const double dpydty = q / qp * (1.f + tx * tx) / t3;
  const double dpydqp = -q / (qp * qp) * ty / t;
  const double dpzdtx = -q / qp * tx / t3;
  const double dpzdty = -q / qp * ty / t3;
  const double dpzdqp = -q / (qp * qp * t3);

  const double F[6][5] = {{1.f, 0.f, 0.f, 0.f, 0.f},
                          {0.f, 1.f, 0.f, 0.f, 0.f},
                          {0.f, 0.f, 0.f, 0.f, 0.f},
                          {0.f, 0.f, dpxdtx, dpxdty, dpxdqp},
                          {0.f, 0.f, dpydtx, dpydty, dpydqp},
                          {0.f, 0.f, dpzdtx, dpzdty, dpzdqp}};

  double VFT[5][6];
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 6; j++) {
      VFT[i][j] = 0;
      for (int k = 0; k < 5; k++) {
        if (k <= i)
          VFT[i][j] +=
            track.GetField<float>(cov_field_id_ + k + i * (i + 1) / 2)
            * F[j][k];  //parameters->GetCovariance(i,k) * F[j][k];
        else
          VFT[i][j] +=
            track.GetField<float>(cov_field_id_ + i + k * (k + 1) / 2)
            * F[j][k];  //parameters->GetCovariance(i,k) * F[j][k];
      }
    }


  std::vector<float> cov(21, 0);
  for (int i = 0, l = 0; i < 6; i++)
    for (int j = 0; j <= i; j++, l++) {
      cov[l] = 0;
      for (int k = 0; k < 5; k++) {
        cov[l] += F[i][k] * VFT[k][j];
      }
    }
  return cov;
}
