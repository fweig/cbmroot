/** @file draw_analysis.C
  * @copyright Copyright (C) 2010-2020 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Elena Lebedeva [orginator], Semen Lebedev, Andrey Lebedev **/

//#include <experimental/filesystem>

void draw_analysis(const string& histRootFile = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/analysis.all.root",
                   const string& resultDir    = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/histograms/inmed/",
                   Bool_t useMvd = false, Bool_t drawSignificance = true)

{
  string outputDir = resultDir + "lmvm_results/";
  //std::filesystem::remove(outputDir);
  gSystem->mkdir(outputDir.c_str(), true);

  CbmAnaDielectronTaskDraw* draw = new CbmAnaDielectronTaskDraw();
  draw->DrawHistFromFile(histRootFile, outputDir, useMvd, drawSignificance);
}
