/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// The macro compares the media for defined TGeoNodes in the mcbm setup with
// the media assigned to the nodes during creation of the feometries of the
// different detector systems.
// Due to the way we build up the complete CBM geometry from the
// several independent geometries of the detector systems it can happen that
// a node gets a wrong media information.

int loop_over_vector(std::vector<std::pair<TString, TString>>& MaterialList)
{
  int wrong_media {0};
  TGeoNode* node {nullptr};
  TString medName {""};
  std::cout << std::endl;
  for (auto material : MaterialList) {
    if (gGeoManager->cd(material.first)) {
      node    = gGeoManager->GetCurrentNode();
      medName = node->GetMedium()->GetName();
      if (medName.CompareTo(material.second)) {
        wrong_media++;
        std::cout << "Medium for " << material.first << " is wrong." << std::endl;
        std::cout << "Expected: " << material.second << std::endl;
        std::cout << "Found   : " << medName << std::endl;
        std::cout << std::endl;
        //      } else {
        //       std::cout << "Medium for " << material.first << " is correct." << std::endl;
        //       std::cout << std::endl;
      }
    }
  }
  return wrong_media;
}

void mcbm_check_materials(const char* dataset = "test")
{
  TString geoFile = TString(dataset) + ".geo.root";
  TFile* f        = new TFile(geoFile);
  if (!f->IsOpen()) {
    std::cout << "mcbm_check_materials: geometry file " << geoFile << " is not accessible!" << std::endl;
    return;
  }

  // setup a vector which contain pairs of geometry paths and expected
  // material
  std::vector<std::pair<TString, TString>> MaterialList = {
    {"/cave_1/platform_v18d_mcbm_0", "dummy"},
    {"/cave_1/platform_v18d_mcbm_0/platform_1", "air"},
    {"/cave_1/trd_v18q_mcbm_0", "dummy"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101", "dummy"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001", "dummy"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001/lat_grid_mod1_1", "air"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001/lat_grid_mod1_1/"
     "lattice1ho_1",
     "TRDG10"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001/kaptonfoil_1", "TRDkapton"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001/gas_1", "TRDgas"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001/frame1_1", "TRDG10"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001/padcopper_1", "TRDcopper"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001/honeycomb_1", "TRDaramide"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001/carbonsheet_1", "TRDcarbon"},
    {"/cave_1/trd_v18q_mcbm_0/layer01_10101/module8_101201001/aluledge1_1", "aluminium"},
    {"/cave_1/much_v19a_mcbm_0", "dummy"},
    {"/cave_1/much_v19a_mcbm_0/station_1/muchstation01_0/muchstation01layer1_0/"
     "muchstation01layer1fsupport001_0",
     "MUCHnoryl"},
    {"/cave_1/much_v19a_mcbm_0/station_1/muchstation01_0/muchstation01layer1_0/"
     "muchstation01layer1factive001gasArgon_0",
     "MUCHargon"},
    {"/cave_1/much_v19a_mcbm_0/station_1/muchstation01_0/muchstation01layer1_0/"
     "muchstation01layer1fcool001Aluminum_0",
     "aluminium"},
  };

  std::vector<std::pair<TString, TString>> MaterialList_2019_11 = {
    {"/cave_1/psd_v18d_mcbm_0/module2060_0", "iron"},
    {"/cave_1/psd_v18d_mcbm_0/module2060_0/lead_0", "lead"},
    {"/cave_1/psd_v18d_mcbm_0/module2060_0/lead_0/channel_0", "PsdFibre"},
    {"/cave_1/psd_v18d_mcbm_0/module2060_0/lead_0/tyvek_0", "PsdTyvek"},
    {"/cave_1/psd_v18d_mcbm_0/module2060_0/lead_0/tyvek_0/scint_0", "PsdScint"},
    {"/cave_1/sts_v19b_mcbm_0/Station01_1/Ladder09_2/Ladder09_FullFrameBox_1", "air"},
    {"/cave_1/sts_v19b_mcbm_0/Station01_1/Ladder09_2/Ladder09_FullFrameBox_1/"
     "FrameBox_vertpillar_1",
     "carbon"},
    {"/cave_1/sts_v19b_mcbm_0/Station01_1/Ladder09_2/HalfLadder09d_2/"
     "HalfLadder09d_Module04_1/Sensor04_1",
     "silicon"},
    {"/cave_1/sts_v19b_mcbm_0/Station01_1/Ladder09_2/HalfLadder09d_2/"
     "HalfLadder09d_Module04_1/HalfLadder09d_Module04_cable_1",
     "STScable"},
    {"/cave_1/targetbox_v19d_0/pipe20_1", "iron"},
    {"/cave_1/targetbox_v19d_0/vacu20_1", "vacuum"},
    {"/cave_1/pipe_v19d_0/pipe30_1", "iron"},
    {"/cave_1/pipe_v19d_0/vacu30_1", "vacuum"},
    {"/cave_1/tof_v19b_mcbm_0", "dummy"},
    {"/cave_1/tof_v19b_mcbm_0/tof_v19b_mcbmStand_1/module_0_0/", "aluminium"},
    {"/cave_1/tof_v19b_mcbm_0/tof_v19b_mcbmStand_1/module_0_0/gas_box_0/", "RPCgas_noact"},
    {"/cave_1/tof_v19b_mcbm_0/tof_v19b_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0",
     "RPCgas_noact"},
    {"/cave_1/tof_v19b_mcbm_0/tof_v19b_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0/tof_glass_0",
     "RPCglass"},
    {"/cave_1/tof_v19b_mcbm_0/tof_v19b_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0/Gap_0",
     "RPCgas"},
    {"/cave_1/tof_v19b_mcbm_0/tof_v19b_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0/Gap_0/Cell_1",
     "RPCgas"},
    {"/cave_1/tof_v19b_mcbm_0/tof_v19b_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0/pcb_0",
     "carbon"},
    {"/cave_1/rich_v19c_mcbm_0", "dummy"},
    {"/cave_1/rich_v19c_mcbm_0/box_1", "aluminium"},
    {"/cave_1/rich_v19c_mcbm_0/box_1/Gas_1", "RICHgas_N2_dis"},
    {"/cave_1/rich_v19c_mcbm_0/box_1/Gas_1/aerogel_1", "aerogel"},
    {"/cave_1/rich_v19c_mcbm_0/box_1/Gas_1/pmt_plane_1/pmt_cont_vol_0/"
     "pmt_vol_1_1/pmt_pixel_1",
     "CsI"},
    {"/cave_1/rich_v19c_mcbm_0/box_1/Gas_1/pmt_plane_1/pmt_cont_vol_0/"
     "pmt_vol_1_1/pmt_Window_1",
     "PMTglass"},
    {"/cave_1/rich_v19c_mcbm_0/box_1/kapton_1", "kapton"}};

  std::vector<std::pair<TString, TString>> MaterialList_2019_03 = {
    {"/cave_1/sts_v19a_mcbm_0/Station01_1/Ladder09_2/Ladder09_FullFrameBox_1", "air"},
    {"/cave_1/sts_v19a_mcbm_0/Station01_1/Ladder09_2/Ladder09_FullFrameBox_1/"
     "FrameBox_vertpillar_1",
     "carbon"},
    {"/cave_1/sts_v19a_mcbm_0/Station01_1/Ladder09_2/HalfLadder09d_2/"
     "HalfLadder09d_Module04_1/Sensor04_1",
     "silicon"},
    {"/cave_1/sts_v19a_mcbm_0/Station01_1/Ladder09_2/HalfLadder09d_2/"
     "HalfLadder09d_Module04_1/HalfLadder09d_Module04_cable_1",
     "STScable"},
    {"/cave_1/pipe_v19b_0/pipe20_1", "iron"},
    {"/cave_1/pipe_v19b_0/vacu20_1", "vacuum"},
    {"/cave_1/pipe_v19b_0/pipe30_1", "iron"},
    {"/cave_1/pipe_v19b_0/vacu30_1", "vacuum"},
    {"/cave_1/tof_v19a_mcbm_0", "dummy"},
    {"/cave_1/tof_v19a_mcbm_0/tof_v19a_mcbmStand_1/module_0_0/", "aluminium"},
    {"/cave_1/tof_v19a_mcbm_0/tof_v19a_mcbmStand_1/module_0_0/gas_box_0/", "RPCgas_noact"},
    {"/cave_1/tof_v19a_mcbm_0/tof_v19a_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0",
     "RPCgas_noact"},
    {"/cave_1/tof_v19a_mcbm_0/tof_v19a_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0/tof_glass_0",
     "RPCglass"},
    {"/cave_1/tof_v19a_mcbm_0/tof_v19a_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0/Gap_0",
     "RPCgas"},
    {"/cave_1/tof_v19a_mcbm_0/tof_v19a_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0/Gap_0/Cell_1",
     "RPCgas"},
    {"/cave_1/tof_v19a_mcbm_0/tof_v19a_mcbmStand_1/module_0_0/gas_box_0/"
     "counter_0/pcb_0",
     "carbon"},
    {"/cave_1/rich_v19a_mcbm_0", "dummy"},
    {"/cave_1/rich_v19a_mcbm_0/box_1", "aluminium"},
    {"/cave_1/rich_v19a_mcbm_0/box_1/Gas_1", "RICHgas_N2_dis"},
    {"/cave_1/rich_v19a_mcbm_0/box_1/Gas_1/aerogel_1", "aerogel"},
    {"/cave_1/rich_v19a_mcbm_0/box_1/Gas_1/pmt_plane_1/pmt_cont_vol_0/"
     "pmt_vol_1_1/pmt_pixel_1",
     "CsI"},
    {"/cave_1/rich_v19a_mcbm_0/box_1/Gas_1/pmt_plane_1/pmt_cont_vol_0/"
     "pmt_vol_1_1/pmt_Window_1",
     "PMTglass"},
    {"/cave_1/rich_v19a_mcbm_0/box_1/kapton_1", "kapton"}};

  gGeoManager = dynamic_cast<TGeoManager*>(f->Get("FAIRGeom"));
  assert(gGeoManager);

  int wrong_media {0};
  wrong_media = loop_over_vector(MaterialList);
  if (TString(dataset).Contains("2019_11")) { wrong_media += loop_over_vector(MaterialList_2019_11); }
  else if (TString(dataset).Contains("2019_03")) {
    wrong_media += loop_over_vector(MaterialList_2019_03);
  }

  if (0 != wrong_media) {
    std::cout << " Test failed" << std::endl;
    std::cout << " We have in total " << wrong_media << " wrongly assigned media." << std::endl;
  }
  else {
    std::cout << " Test passed" << std::endl;
    std::cout << " All ok " << std::endl;
  }


  RemoveGeoManager();
}
