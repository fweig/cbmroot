#ifndef ANALYSISTREEQA_SRC_BASICQA_HPP_
#define ANALYSISTREEQA_SRC_BASICQA_HPP_

#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

constexpr int gNbins = 500;

inline void AddEventHeaderQA(Task* task, const std::string& branch, Cuts* cuts = nullptr) {

  task->AddH1({"x_{vertex} (cm)", {branch, "vtx_x"}, {gNbins, -1, 1}}, cuts);
  task->AddH1({"y_{vertex} (cm)", {branch, "vtx_y"}, {gNbins, -1, 1}}, cuts);
  task->AddH1({"z_{vertex} (cm)", {branch, "vtx_z"}, {gNbins, -1, 1}}, cuts);

  task->AddH2({"x_{vertex} (cm)", {branch, "vtx_x"}, {gNbins, -1, 1}}, {"y_{vertex} (cm)", {branch, "vtx_y"}, {gNbins, -1, 1}}, cuts);
}


inline void AddTrackQA(Task* task, const std::string& branch, Cuts* cuts = nullptr) {

  task->AddH1({"p_{x}, GeV/c", {branch, "px"}, {gNbins, -3, 3}}, cuts);
  task->AddH1({"p_{y}, GeV/c", {branch, "py"}, {gNbins, -3, 3}}, cuts);
  task->AddH1({"p_{z}, GeV/c", {branch, "pz"}, {gNbins, -1, 10}}, cuts);
  task->AddH1({"p, GeV/c", {branch, "p"}, {gNbins, 0, 10}}, cuts);

  task->AddH1({"p_{T}, GeV/c", {branch, "pT"}, {gNbins, 0, 2}}, cuts);
  task->AddH1({"#eta", {branch, "eta"}, {gNbins, -1, 7}}, cuts);
  task->AddH1({"#phi", {branch, "phi"}, {gNbins, -3.2, 3.2}}, cuts);

  task->AddH2({"#phi", {branch, "phi"}, {gNbins, -3.2, 3.2}}, {"p_{T}, GeV/c", {branch, "pT"}, {gNbins, 0, 2}}, cuts);
  task->AddH2({"#phi", {branch, "phi"}, {gNbins, -3.2, 3.2}}, {"#eta", {branch, "eta"}, {gNbins, -1, 7}}, cuts);
  task->AddH2({"#eta", {branch, "eta"}, {gNbins, -1, 7}}, {"p_{T}, GeV/c", {branch, "pT"}, {gNbins, 0, 2}}, cuts);
}

inline void AddParticleQA(Task* task, const std::string& branch, Cuts* cuts = nullptr) {

  AddTrackQA(task, branch, cuts);

  task->AddH1({"#it{y}_{Lab}", {branch, "rapidity"}, {gNbins, -1, 5}}, cuts);
  task->AddH1({"PDG code", {branch, "pid"}, {gNbins, -5000, 5000}}, cuts);
  task->AddH1({"Mass, GeV/c^{2}", {branch, "mass"}, {gNbins, 0, 5}}, cuts);

  task->AddH2({"#phi", {branch, "phi"}, {gNbins, -3.2, 3.2}}, {"#it{y}_{Lab}", {branch, "rapidity"}, {gNbins, -1, 5}}, cuts);
  task->AddH2({"#it{y}_{Lab}", {branch, "rapidity"}, {gNbins, -1, 5}}, {"p_{T}, GeV/c", {branch, "pT"}, {gNbins, 0, 2}}, cuts);
}

inline void AddTracksMatchQA(Task* task, const std::string& rec_tracks, const std::string& sim_particles, Cuts* cuts = nullptr) {

  task->AddH2({"p_{x}, GeV/c", {rec_tracks, "px"}, {gNbins, -3, 3}}, {"p_{x}, GeV/c", {sim_particles, "px"}, {gNbins, -3, 3}}, cuts);
  task->AddH2({"p_{y}, GeV/c", {rec_tracks, "py"}, {gNbins, -3, 3}}, {"p_{y}, GeV/c", {sim_particles, "py"}, {gNbins, -3, 3}}, cuts);
  task->AddH2({"p_{z}, GeV/c", {rec_tracks, "pz"}, {gNbins, -1, 10}}, {"p_{z}, GeV/c", {sim_particles, "pz"}, {gNbins, -1, 10}}, cuts);
  task->AddH2({"p, GeV/c", {rec_tracks, "p"}, {gNbins, 0, 10}}, {"p, GeV/c", {sim_particles, "p"}, {gNbins, 0, 10}}, cuts);

  task->AddH2({"p_{T}, GeV/c", {rec_tracks, "pT"}, {gNbins, 0, 2}}, {"p_{T}, GeV/c", {sim_particles, "pT"}, {gNbins, 0, 2}}, cuts);
  task->AddH2({"#eta", {rec_tracks, "eta"}, {gNbins, -1, 7}}, {"#eta", {sim_particles, "eta"}, {gNbins, -1, 7}}, cuts);
  task->AddH2({"#phi", {rec_tracks, "phi"}, {gNbins, -3.2, 3.2}}, {"#phi", {sim_particles, "phi"}, {gNbins, -3.2, 3.2}}, cuts);
}

inline void AddParticlesFlowQA(Task* task, const std::string& particles, const Field& psi_RP, const std::vector<int>& pdg_codes) {

  Variable v1("v1", {{particles, "phi"}, psi_RP}, [](std::vector<double> phi) { return cos(phi[0] - phi[1]); });
  Variable v2("v2", {{particles, "phi"}, psi_RP}, [](std::vector<double> phi) { return cos(2 * (phi[0] - phi[1])); });

  for (auto pdg : pdg_codes) {
    auto* pid_cut = new Cuts("mc_" + std::to_string(pdg), {EqualsCut(particles + ".pid", pdg)});

    task->AddProfile({"#it{y}", {particles, "rapidity"}, {20, 0.5, 2.5}}, {"v_{1}", v1, {}}, pid_cut);
    task->AddProfile({"#it{y}", {particles, "rapidity"}, {20, 0.5, 2.5}}, {"v_{2}", v2, {}}, pid_cut);
  }
}

}// namespace QA
}// namespace AnalysisTree
#endif//ANALYSISTREEQA_SRC_BASICQA_HPP_
