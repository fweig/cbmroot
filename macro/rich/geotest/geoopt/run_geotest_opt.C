/** @file run_geotest_opt.C
  * @copyright Copyright (C) 2020 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev [orginator] **/

void run_geotest_opt()
{
  int mirrorRotation    = 10;
  std::string mainDir   = "/Users/slebedev/Development/cbm/data/geoopt/m" + to_string(mirrorRotation) + "/";
  std::string outputDir = "results_geoopt/m" + to_string(mirrorRotation) + "/";
  int minIndex          = 1;
  int maxIndex          = 648;

  vector<string> geoTestBoxPathes;
  vector<string> geoTestOmega3Pathes;
  vector<string> geoTestOmega8Pathes;
  vector<string> urqmdTestPathes;
  vector<string> recoQaBox30Pathes;
  vector<string> recoQaUrqmdPathes;

  for (int i = minIndex; i <= maxIndex; i++) {
    geoTestBoxPathes.push_back(mainDir + "geotest_box/qa." + to_string(i) + ".root");
    geoTestOmega3Pathes.push_back(mainDir + "geotest_plutoOmega3_5gev/qa." + to_string(i) + ".root");
    geoTestOmega8Pathes.push_back(mainDir + "geotest_plutoOmega8gev/qa." + to_string(i) + ".root");
    urqmdTestPathes.push_back(mainDir + "urqmdtest_8gev/qa." + to_string(i) + ".root");
    recoQaBox30Pathes.push_back(mainDir + "recoqa_box30/qa." + to_string(i) + ".root");
    recoQaUrqmdPathes.push_back(mainDir + "recoqa_urqmd8gev/qa." + to_string(i) + ".root");
  }

  CbmRichGeoTestOpt* richGeoTestOpt = new CbmRichGeoTestOpt();
  richGeoTestOpt->SetFilePathes(geoTestBoxPathes, geoTestOmega3Pathes, geoTestOmega8Pathes, urqmdTestPathes,
                                recoQaBox30Pathes, recoQaUrqmdPathes);
  richGeoTestOpt->SetOutputDir(outputDir);
  richGeoTestOpt->SetReferenceInd(457);
  richGeoTestOpt->SetDrawReference(true);
  richGeoTestOpt->Draw();
}
