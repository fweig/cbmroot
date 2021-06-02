/** @file mcbm_check_overlaps.C
  * @copyright Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, David Emschermann [committer] **/

void mcbm_check_overlaps(const char* dataset = "test")
{
  // 2020-07-10 - DE - simplify overlap name checking
  // 2019-12-20 - FU - adapt expected overlaps to new targetbox name
  // 2019-08-13 - FU - Filter "expected overlaps"
  // 2017-11-06 - DE - adapt to mCBM
  // 2014-07-04 - DE - test CBM setups for collisions in nightly tests
  // 2014-07-04 - DE - currently there are 2 overlaps between the PIPE and STS layer 8
  // 2014-07-04 - DE - set the default to 0 overlaps, anyway
  //
  // There are some overlaps which we know
  // Until they can be fixed we will not mark the test as error but
  // we will remove them from the list of unxpected overlaps

  UInt_t unexpectedOverlaps {0};
  UInt_t benchmarkNumber {0};

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
    // within the TOF detector geometry
    if (OverlapName.Contains("gas_box extruded by: gas_box/counter_0")) {
      std::cout << "Expected Overlap between gas_box and gas_box/counter_0" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("gas_box/counter_0 overlapping gas_box/counter_1")) {
      std::cout << "Expected Overlap between gas_box/counter_0 and gas_box/counter_1" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // overlap of T0 detector with pipe vacuum geometry
    else if (OverlapName.Contains("vacu20_1 overlapping"))
      if (OverlapName.Contains("module_5_0")) {
        std::cout << "Expected Overlap between pipevacuum and T0 counter" << endl;
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
    // within the TOF detector geometry
    if (OverlapName.Contains("gas_box: node counter_4 overlapping counter_5")) {
      std::cout << "Expected Overlap between counter_4 and counter_5" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("gas_box: node counter_5 overlapping counter_6")) {
      std::cout << "Expected Overlap between counter_6 and counter_6" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("gas_box: node counter_1 overlapping counter_2")) {
      std::cout << "Expected Overlap between counter_1 and counter_2" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("gas_box: node counter_3 overlapping counter_4")) {
      std::cout << "Expected Overlap between counter_3 and counter_4" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("gas_box: node counter_0 overlapping counter_1")) {
      std::cout << "Expected Overlap between counter_0 and counter_1" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("gas_box: node counter_2 overlapping counter_3")) {
      std::cout << "Expected Overlap between counter_2 and counter_3" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("gas_box: node counter_6 overlapping counter_7")) {
      std::cout << "Expected Overlap between counter_6 and counter_7" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("node module_8_0 overlapping module_8_0")) {
      std::cout << "Expected Overlap between tof module_8_0 and module_8_0" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("node Gap_3 overlapping tof_glass_4")) {
      std::cout << "Expected Overlap between Gap_3 and tof_glass_4" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the TOF detector geometry
    else if (OverlapName.Contains("tof_glass_1 overlapping Gap_1")) {
      std::cout << "Expected Overlap between Gap_1 and tof_glass_1" << endl;
      std::cout << ov->GetTitle() << std::endl << std::endl;
    }
    // within the RICH detector geometry
    else if (OverlapName.Contains("node Gas_1 overlapping kapton_1")) {
      std::cout << "Expected Overlap between  Gas_1 and kapton_1" << endl;
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

  if (unexpectedOverlaps != benchmarkNumber) {
    cout << " Test failed" << endl;
    cout << " We have in total " << unexpectedOverlaps << " unexpected overlaps." << endl;
  }
  else {
    cout << " Test passed" << endl;
    cout << " All ok " << endl;
  }

  RemoveGeoManager();
}
