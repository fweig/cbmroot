/* Copyright (C) 2010-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig [committer] */

void check_overlaps(const char* dataset = "test")
{
  // 2014-07-04 - DE - test CBM setups for collisions in nightly tests
  // 2014-07-04 - DE - currently there are 2 overlaps between the PIPE and STS layer 8
  // 2014-07-04 - DE - set the default to 0 overlaps, anyway
  // 2017-29-11 - FU - define some expected overlaps between magnet and rich or much
  //                   these overlas are accepted for the time being until
  //                   there is a new magnet geometry

  UInt_t unexpectedOverlaps {0};

  TString geoFile = TString(dataset) + ".geo.root";
  TFile* f        = new TFile(geoFile);
  if (!f->IsOpen()) {
    std::cout << "check_overlaps: geometry file " << geoFile << " is not accessible!" << std::endl;
    return;
  }

  gGeoManager = (TGeoManager*) f->Get("FAIRGeom");

  gGeoManager->CheckOverlaps(0.0001);
  TIter next(gGeoManager->GetListOfOverlaps());
  TGeoOverlap* ov;
  while ((ov = (TGeoOverlap*) next())) {
    TString OverlapName = ov->GetTitle();
    if (OverlapName.Contains("cave/magnet_v15b_0 overlapping cave/much")) {
      if (OverlapName.Contains("/absorber_1/absblock1_1/absorber_1")) {
        std::cout << "Expected Overlap between much and magnet" << endl;
        std::cout << ov->GetTitle() << std::endl << std::endl;
      }
      else if (OverlapName.Contains("/absorber_1/absblock0_1/absorber_0")) {
        std::cout << "Expected Overlap between much and magnet" << endl;
        std::cout << ov->GetTitle() << std::endl << std::endl;
      }
    }
    else if (OverlapName.Contains("cave/magnet_v15a_0 overlapping "
                                  "cave/rich_v17a_1e_0/rich_container_290")) {
      std::cout << "Expected Overlap between rich and magnet" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    else if (OverlapName.Contains("cave/magnet_v15a_0 overlapping "
                                  "cave/rich_v17a_3e_0/rich_container_288")) {
      std::cout << "Expected Overlap between rich and magnet" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    else if (OverlapName.Contains("cave/magnet_container_0 overlapping "
                                  "cave/rich_v17a_3e_0/rich_container_288")) {
      std::cout << "Expected Overlap between rich and magnet" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    else if (OverlapName.Contains("cave/magnet_container_0 overlapping "
                                  "cave/rich_v17a_1e_0/rich_container_290")) {
      std::cout << "Expected Overlap between rich and magnet" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    else {
      cout << "Unexpected Overlap:" << endl;
      ov->PrintInfo();
      cout << endl;
      unexpectedOverlaps++;
    }
  }
  std::cout << std::endl;

  gGeoManager->CheckOverlaps(0.0001, "s");
  TIter next1(gGeoManager->GetListOfOverlaps());
  while ((ov = (TGeoOverlap*) next1())) {
    TString OverlapName = ov->GetTitle();
    if (OverlapName.Contains("magnet_v15b_0 overlapping much")) {
      if (OverlapName.Contains("/absorber_1/absblock1_1/absorber_1")) {
        std::cout << "Expected Overlap between much and magnet" << endl;
        std::cout << ov->GetTitle() << std::endl << std::endl;
      }
      else if (OverlapName.Contains("/absorber_1/absblock0_1/absorber_0")) {
        std::cout << "Expected Overlap between much and magnet" << endl;
        std::cout << ov->GetTitle() << std::endl << std::endl;
      }
    }
    else if (OverlapName.Contains("magnet_v15a_0 overlapping "
                                  "rich_v17a_1e_0/rich_container_290")) {
      std::cout << "Expected Overlap between rich and magnet" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    else if (OverlapName.Contains("magnet_v15a_0 overlapping "
                                  "rich_v17a_3e_0/rich_container_288")) {
      std::cout << "Expected Overlap between rich and magnet" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    else if (OverlapName.Contains("magnet_container_0 overlapping "
                                  "rich_v17a_1e_0/rich_container_290")) {
      std::cout << "Expected Overlap between rich and magnet" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    else if (OverlapName.Contains("magnet_container_0 overlapping "
                                  "rich_v17a_3e_0/rich_container_288")) {
      std::cout << "Expected Overlap between rich and magnet" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    else if (OverlapName.Contains("belt_part3_156 overlapping belt_part4_157")) {
      std::cout << "Expected Overlap between rich and magnet" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    else {
      std::cout << "Unexpected Overlap:" << std::endl;
      ov->PrintInfo();
      std::cout << std::endl;
      unexpectedOverlaps++;
    }
  }
  std::cout << std::endl;

  if (unexpectedOverlaps != 0) {
    std::cout << " Test failed" << std::endl;
    std::cout << " We have in total " << unexpectedOverlaps << " unexpected overlaps." << std::endl;
  }
  else {
    std::cout << " There are no unexpected overlaps." << std::endl;
    std::cout << " Test passed" << std::endl;
    std::cout << " All ok " << std::endl;
  }


  RemoveGeoManager();
}
