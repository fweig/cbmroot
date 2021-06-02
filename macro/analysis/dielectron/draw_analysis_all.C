/** @file draw_analysis_all.C
  * @copyright Copyright (C) 2011-2020 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Elena Lebedeva [committer], Semen Lebedev **/

void draw_analysis_all(
  const string& fileInmed  = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/analysis.all.root",
  const string& fileQgp    = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/qgp/analysis.all.root",
  const string& fileOmega  = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/omegaepem/analysis.all.root",
  const string& filePhi    = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/phi/analysis.all.root",
  const string& fileOmegaD = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/omegadalitz/analysis.all.root",
  const string& resultDir  = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/histograms/results_all/",
  Bool_t useMvd            = false)

{
  CbmAnaDielectronTaskDrawAll* draw = new CbmAnaDielectronTaskDrawAll();
  draw->DrawHistosFromFile(fileInmed, fileQgp, fileOmega, filePhi, fileOmegaD, resultDir, useMvd);
}
