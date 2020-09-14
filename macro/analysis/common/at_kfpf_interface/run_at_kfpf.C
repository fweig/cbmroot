void run_at_kfpf(const std::string& ATFile,
                 int nEntries              = -1,
                 const std::string& ATTree = "aTree") {
  ATKFParticleFinder man;
  man.InitInput(ATFile.c_str(), ATTree.c_str());
  man.InitOutput("KFPF.pwgc2f.root");
  man.SetPIDMode(1);

  CutsContainer cuts;
  cuts.SetCutChi2Prim(18.4207);
  cuts.SetCutDistance(1.);
  cuts.SetCutChi2Geo(3.);
  cuts.SetCutLdL(5.);
  man.SetCuts(cuts);

  man.Run(nEntries);
}
