void run_at_kfpf(int nEntries               = -1,
                 const std::string& dataset = "test",
                 const std::string& ATTree  = "rTree") {
  ATKFParticleFinder man;
  const std::string ATFile = dataset + ".analysistree.root";
  man.InitInput(ATFile.c_str(), ATTree.c_str());
  man.InitOutput(std::string(dataset + ".kfpftree.root"));
  man.SetPIDMode(1);

  CutsContainer cuts;
  cuts.SetCutChi2Prim(18.4207);
  cuts.SetCutDistance(1.);
  cuts.SetCutChi2Geo(3.);
  cuts.SetCutLdL(5.);
  man.SetCuts(cuts);

  man.Run(nEntries);

  // -----   Finish   -------------------------------------------------------
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
