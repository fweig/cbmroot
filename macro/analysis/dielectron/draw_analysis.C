/* Copyright (C) 2010-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Semen Lebedev, Andrey Lebedev */

//#include <experimental/filesystem>

void draw_analysis(
  const string& histRootFile = "/Users/slebedev/Development/cbm/data/lmvm/nov21/analysis.inmed.all.root",
  const string& resultDir = "/Users/slebedev/Development/cbm/data/lmvm/nov21/results/", Bool_t useMvd = true,
  Bool_t drawSignificance = true)

{
  gSystem->mkdir(resultDir.c_str(), true);

  LmvmDraw* draw = new LmvmDraw();
  draw->DrawHistFromFile(histRootFile, resultDir, useMvd, drawSignificance);
}
