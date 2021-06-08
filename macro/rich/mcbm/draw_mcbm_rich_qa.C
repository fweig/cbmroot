/* Copyright (C) 2018 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

void draw_mcbm_rich_qa()
{
  std::string outputDir = "results_mcbm_rich_qa/";
  //std::string fileName = "/home/aghoehne/Documents/CbmRoot/Gregor/output/reco_results.root";
  std::string fileName = "/Users/slebedev/Development/cbm/data/sim/rich/mcbm/reco.00000.root";

  CbmRichMCbmQa* richMCbmQa = new CbmRichMCbmQa();
  richMCbmQa->DrawFromFile(fileName, "");
}
