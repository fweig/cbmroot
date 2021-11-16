/* Copyright (C) 2011-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Semen Lebedev */

void draw_analysis_all(const string& fileInmed  = "/lustre/cbm/users/criesen/data/lmvm/inmed/analysis.all.root",
                       const string& fileQgp    = "/lustre/cbm/users/criesen/data/lmvm/qgp/analysis.all.root",
                       const string& fileOmega  = "/lustre/cbm/users/criesen/data/lmvm/omegaepem/analysis.all.root",
                       const string& filePhi    = "/lustre/cbm/users/criesen/data/lmvm/phi/analysis.all.root",
                       const string& fileOmegaD = "/lustre/cbm/users/criesen/data/lmvm/omegadalitz/analysis.all.root",
                       const string& resultDir = "/lustre/cbm/users/criesen/data/lmvm/results/", Bool_t useMvd = false)

{
  LmvmDrawAll* draw = new LmvmDrawAll();
  draw->DrawHistFromFile(fileInmed, fileQgp, fileOmega, filePhi, fileOmegaD, resultDir, useMvd);
}
