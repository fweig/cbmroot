/* Copyright (C) 2010-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Semen Lebedev, Andrey Lebedev */

//#include <experimental/filesystem>

void draw_analysis(const string& histRootFile = "/home/aghoehne/soft/cbm/data/output/phi/analysis.1.root",
                   const string& resultDir = "/home/aghoehne/soft/cbm/data/output/results/", Bool_t useMvd = true)

{
  gSystem->mkdir(resultDir.c_str(), true);

  LmvmDraw* draw = new LmvmDraw();
  draw->DrawHistFromFile(histRootFile, resultDir, useMvd);
}
