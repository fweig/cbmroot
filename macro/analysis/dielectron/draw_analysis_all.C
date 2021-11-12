/* Copyright (C) 2011-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Semen Lebedev */

void draw_analysis_all(
  const string& fileInmed  = "/Users/slebedev/Development/cbm/data/lmvm/nov21/analysis.inmed.all.root",
  const string& fileQgp    = "/Users/slebedev/Development/cbm/data/lmvm/nov21/analysis.qgp.all.root",
  const string& fileOmega  = "/Users/slebedev/Development/cbm/data/lmvm/nov21/analysis.omega.all.root",
  const string& filePhi    = "/Users/slebedev/Development/cbm/data/lmvm/nov21/analysis.phi.all.root",
  const string& fileOmegaD = "/Users/slebedev/Development/cbm/data/lmvm/nov21/analysis.omegaD.all.root",
  const string& resultDir = "/Users/slebedev/Development/cbm/data/lmvm/nov21/results_all/", Bool_t useMvd = false)

{
  LmvmDrawAll* draw = new LmvmDrawAll();
  draw->DrawHistFromFile(fileInmed, fileQgp, fileOmega, filePhi, fileOmegaD, resultDir, useMvd);
}
