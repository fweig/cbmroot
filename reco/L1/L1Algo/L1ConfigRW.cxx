/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   L1ConfigRW.h
/// @brief  Configuration parameter file reader/writer for L1 tracking algorithm (implementation)
/// @author S.Zharko <s.zharko@gsi.de>
/// @since  18.07.2022

#include "L1ConfigRW.h"

#include <iostream>
#include <numeric>
#include <sstream>

#include <yaml-cpp/yaml.h>

#include "L1CAIteration.h"
#include "L1InitManager.h"

using namespace std::string_literals;

// ---------------------------------------------------------------------------------------------------------------------
//
L1ConfigRW::L1ConfigRW(L1InitManager* pInitManager, int verbose) : fpInitManager(pInitManager), fVerbose(verbose) {}

// ---------------------------------------------------------------------------------------------------------------------
//
std::vector<std::string> L1ConfigRW::GetNodeKeys(const YAML::Node& node) const
{
  std::vector<std::string> res;
  res.reserve(node.size());
  try {
    for (const auto& item : node) {
      res.push_back(item.first.as<std::string>());
    }
  }
  catch (const YAML::InvalidNode& exc) {  // It is impossible to
    LOG(warn) << "L1 config: attempt to call L1ConfigRW::GetNodeKeys for node, keys of which could not be represented "
              << "with strings. An empty vector will be returned";
    std::vector<std::string>().swap(res);
  }

  return res;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1ConfigRW::ReadCAIterations(const YAML::Node& node)
{
  if (node) {
    if (fVerbose > -1) {
      LOG(info) << "L1 config: Reading CA tracking iterations sequence. Default iterations will be ignored";
    }
    fpInitManager->ClearCAIterations();
    fpInitManager->SetCAIterationsNumberCrosscheck(node.size());
    if (fVerbose > 2) { LOG(info) << "L1 config: " << fVerbose << " CA iterations were recorded"; }
    if (fVerbose > 3) { LOG(info) << "L1 config: Recorded iterations:"; }

    // Loop over input sub-nodes. Each sub-node corresponds to one L1CAIteration object
    for (const auto& input : node) {
      try {
        auto caIter = L1CAIteration(input["name"].as<std::string>());
        caIter.SetTrackChi2Cut(input["track_chi2_cut"].as<float>(caIter.GetTrackChi2Cut()));
        caIter.SetTripletChi2Cut(input["triplet_chi2_cut"].as<float>(caIter.GetTripletChi2Cut()));
        caIter.SetTripletFinalChi2Cut(input["triplet_final_chi2_cut"].as<float>(caIter.GetTripletFinalChi2Cut()));
        caIter.SetDoubletChi2Cut(input["doublet_chi2_cut"].as<float>(caIter.GetDoubletChi2Cut()));
        caIter.SetPickGather(input["pick_gather"].as<float>(caIter.GetPickGather()));
        caIter.SetTripletLinkChi2(input["triplet_link_chi2"].as<float>(caIter.GetTripletLinkChi2()));
        caIter.SetMaxInvMom(1. / input["min_momentum"].as<float>(caIter.GetMaxInvMom()));
        caIter.SetMaxSlopePV(input["max_slope_pv"].as<float>(caIter.GetMaxSlopePV()));
        caIter.SetMaxSlope(input["max_slope"].as<float>(caIter.GetMaxSlope()));
        caIter.SetMaxDZ(input["max_dz"].as<float>(caIter.GetMaxDZ()));
        caIter.SetTargetPosSigmaXY(input["target_pos_sigma_x"].as<float>(caIter.GetTargetPosSigmaX()),
                                   input["target_pos_sigma_y"].as<float>(caIter.GetTargetPosSigmaY()));
        caIter.SetFirstStationIndex(input["first_station_index"].as<int>(caIter.GetFirstStationIndex()));
        caIter.SetPrimaryFlag(input["is_primary"].as<bool>(caIter.GetPrimaryFlag()));
        caIter.SetElectronFlag(input["is_electron"].as<bool>(caIter.GetElectronFlag()));
        caIter.SetTrackFromTripletsFlag(input["is_track_from_triplets"].as<bool>(caIter.GetTrackFromTripletsFlag()));
        caIter.SetExtendTracksFlag(input["if_extend_tracks"].as<bool>(caIter.GetExtendTracksFlag()));
        caIter.SetJumpedFlag(input["is_jumped"].as<bool>(caIter.GetJumpedFlag()));
        caIter.SetMinNhits(input["min_n_hits"].as<bool>(caIter.GetMinNhits()));
        caIter.SetMinNhitsStation0(input["min_n_hits_sta_0"].as<bool>(caIter.GetMinNhitsStation0()));
        if (fVerbose > 3) { LOG(info) << "L1 config:\n" << caIter.ToString(1); }
        fpInitManager->PushBackCAIteration(caIter);
      }
      catch (const YAML::InvalidNode& exc) {
        const auto nodeKeys = this->GetNodeKeys(input);
        const auto nodeKeysStr =
          std::accumulate(nodeKeys.cbegin(), nodeKeys.cend(), std::string(""),
                          [](std::string lhs, std::string rhs) { return std::move(lhs) + "\n\t" + std::move(rhs); });
        LOG(fatal) << "L1 config: attempt to access key which does not exist in the configuration file (message from "
                   << "YAML exception: " << exc.what() << "). Defined keys: " << nodeKeysStr;
      }
    }  // Loop over input sub-nodes: end
  }
  else {
    LOG(warn) << "L1 config: CA tracking iterations sequence was not found in the parameters file, default will be "
              << "used. To define iterations please use the following path in the YAML file: l1/algorithm/iterations";
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1ConfigRW::ReadYaml(const std::string& filename)
{
  // Load YAML node from the file
  YAML::Node config;
  try {
    config = YAML::LoadFile(filename);
  }
  catch (const YAML::BadFile& exc) {
    throw std::runtime_error("file does not exist");
  }
  catch (const YAML::ParserException& exc) {
    throw std::runtime_error("file is formatted improperly");
  }

  // Tracking iterations
  this->ReadCAIterations(config["l1"]["algorithm"]["iterations"]);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1ConfigRW::WriteYaml(const std::string& /*filename*/) {}
