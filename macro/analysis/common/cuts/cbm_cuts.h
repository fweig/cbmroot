
AnalysisTree::Cuts* GetCbmEventCuts(const std::string& branch, std::string name = "CbmGoodEvent")
{
  AnalysisTree::SimpleCut vtx_x_cut    = AnalysisTree::RangeCut(branch + ".vtx_x", -0.5, 0.5);
  AnalysisTree::SimpleCut vtx_y_cut    = AnalysisTree::RangeCut(branch + ".vtx_y", -0.5, 0.5);
  AnalysisTree::SimpleCut vtx_z_cut    = AnalysisTree::RangeCut(branch + ".vtx_z", -0.1, 0.1);
  AnalysisTree::SimpleCut vtx_chi2_cut = AnalysisTree::RangeCut(branch + ".vtx_chi2", 0.8, 1.7);

  auto* event_cuts = new AnalysisTree::Cuts(std::move(name), {vtx_x_cut, vtx_y_cut, vtx_z_cut, vtx_chi2_cut});
  return event_cuts;
}

AnalysisTree::Cuts* GetCbmTrackCuts(const std::string& branch, std::string name = "CbmGoodVertexTrack")
{
  AnalysisTree::SimpleCut vtx_chi2_track_cut = AnalysisTree::RangeCut(branch + ".vtx_chi2", 0, 3);
  AnalysisTree::SimpleCut nhits_cut          = AnalysisTree::RangeCut(branch + ".nhits", 4, 100);
  AnalysisTree::SimpleCut chi2_cut({branch + ".chi2", branch + ".ndf"},
                                   [](std::vector<double> par) { return par[0] / par[1] < 3; });
  AnalysisTree::SimpleCut eta_cut = AnalysisTree::RangeCut(branch + ".eta", 0.2, 6);

  auto* vertex_tracks_cuts =
    new AnalysisTree::Cuts(std::move(name), {vtx_chi2_track_cut, nhits_cut, chi2_cut, eta_cut});
  return vertex_tracks_cuts;
}

AnalysisTree::Cuts* GetCbmTofHitsCuts(const std::string& branch, std::string name = "CbmGoodTofHit")
{
  AnalysisTree::SimpleCut tof_cuts_dx = AnalysisTree::RangeCut(branch + ".dx", -2, 2);
  AnalysisTree::SimpleCut tof_cuts_dy = AnalysisTree::RangeCut(branch + ".dy", -1, 1);

  auto* tof_cuts = new AnalysisTree::Cuts(std::move(name), {tof_cuts_dx, tof_cuts_dy});
  return tof_cuts;
}

AnalysisTree::Cuts* GetCbmMcTracksCuts(const std::string& branch, std::string name = "CbmMcPrimaryTrack")
{
  auto* sim_tracks_cut = new AnalysisTree::Cuts(std::move(name), {AnalysisTree::EqualsCut(branch + ".mother_id", -1)});
  return sim_tracks_cut;
}
