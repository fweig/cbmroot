/* Copyright (C) 2022 Facility for AntiProton and Ion Research in Europe GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Eoin Clerkin [committer] */

/**
 ** Creation of passive piping in ROOT format (TGeo).
 **
 ** @file create_bpipe_v22b.C
 ** @author Eoin Clerkin <e.clerkin@gsi.de>
 ** @date 16.02.2022
 **/

#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoPhysicalNode.h"
#include "TGeoVolume.h"

#include <iomanip>
#include <iostream>

using namespace std;

#define NODE_DEPTH 25
#define NUM_COMP 100
#define H_VERBOSE 1;

TFile* tmp_binary;

TGeoTranslation null_tra = TGeoTranslation(0, 0, 0);
TGeoRotation null_rot    = TGeoRotation("null rotate", 0, 0, 0);

TGeoNode* node[NODE_DEPTH];
TGeoCombiTrans geo_ctrans[NODE_DEPTH];
TGeoHMatrix h, tmp_mat[NUM_COMP];
TGeoTranslation tmp_shift;

TString VMCWORKDIR = gSystem->Getenv("VMCWORKDIR");

struct rtn_ptrs {
  TGeoVolume* vol;
  TGeoHMatrix* pos;
};

// Finds the count object in the tree matching the name.
struct rtn_ptrs vol_from_binary(const char* tmp_binname, const char* tmp_obj, int count = 1)
{
  struct rtn_ptrs rtn_bin = {nullptr, nullptr};
  int i_array[NODE_DEPTH], num_array[NODE_DEPTH];
  for (int i = 0; i < NODE_DEPTH; i++) {
    i_array[i]   = 0;
    num_array[i] = 0;
  };

  std::cout << "Looking for the object" << tmp_obj << "in the root binary" << tmp_binname << "." << std::endl;

  tmp_binary = TFile::Open(tmp_binname, "READ");
  if (!tmp_binary) return {nullptr, nullptr};

  TKey* tmp_key       = (TKey*) tmp_binary->GetListOfKeys()->At(0);
  TGeoVolume* tmp_top = (TGeoVolume*) tmp_binary->Get(tmp_key->GetName());
  num_array[0]        = tmp_top->GetNdaughters();
std:
  cout << "Number of Daughters is: " << num_array[0] << endl;

  int j = 0;
  while (num_array[0] != 0) {
    // j is the depth
    // i_array[j] is the current running index of that depth
    // num_array[j] is the current number in that running index
    // tmp_top is the base top volume which contains the nodes.

    if (j == 0) { node[0] = (TGeoNode*) tmp_top->GetNode(i_array[j]); }
    else {
      node[j] = static_cast<TGeoNode*>(node[j - 1]->GetDaughter(i_array[j]));
    };

    if (!strcmp(tmp_obj, node[j]->GetName())) {
      count--;
      if (count == 0) {
        h = TGeoCombiTrans(null_tra, null_rot);
        std::cout << "Object " << tmp_obj << " found" << std::endl;
        rtn_bin.vol = node[j]->GetVolume();
        for (int k = 0; k <= j; k++) {
          geo_ctrans[k] = *(node[k]->GetMatrix());
          h             = h * geo_ctrans[k];
        };

        rtn_bin.pos = &h;
        return rtn_bin;
      }
    };

    i_array[j]++;
    num_array[j + 1] = node[j]->GetNdaughters();

    if (num_array[j + 1] > 0) {
      j++;
      num_array[j + 2] = 0;
    };
    while (i_array[j] == num_array[j]) {
      num_array[j] = 0;
      i_array[j]   = 0;  // Reset the counter before falling back.
      j--;
    };
    if (j >= NODE_DEPTH) {
      std::cerr << "Maximum depth of " << NODE_DEPTH << " exceeded. Increase NODE_DEPTH in header of macro," << endl;
      exit(NODE_DEPTH);
    };
  };
  std::cout << "Volumetric Object Not Found" << std::endl;
  return {nullptr, nullptr};
}


void create_bpipe_geometry_v22b(const char* geoTag = "v22b")
{

  gSystem->Load("libGeom");
  int id = 0;  // Keep a running index

  TGeoManager* gman        = new TGeoManager("pipe", "Cbm Beam Pipe Candidate");
  TGeoMaterial* d_material = new TGeoMaterial("dummy", 0, 0, 0);
  TGeoMedium* d_medium     = new TGeoMedium("dummy", 1, d_material);
  TGeoMatrix* d_matrix     = gGeoIdentity;
  TGeoMatrix* t_matrix     = gGeoIdentity;
  TGeoHMatrix* tmp_h       = new TGeoHMatrix("temp");
  TGeoRotation* tmp_rot    = new TGeoRotation("temp");
  TGeoTranslation* tmp_tran;

  TGeoVolume* top = gman->MakeBox("top", d_medium, 100000, 100000, 100000);
  top->SetTransparency(100);
  gman->SetTopVolume(top);

  struct rtn_ptrs tmp[NUM_COMP];

  // Import some gdml.
  TGDMLParse parser;
  TGeoManager::LockDefaultUnits(kFALSE);
  TGeoManager::SetDefaultUnits(TGeoManager::EDefaultUnits::kRootUnits);

  // To be used as the basis of the geometry
  TString gdmlFileName   = "window_v22a.gdml";
  TGeoVolume* gdmlVolume = parser.GDMLReadFile(VMCWORKDIR + "/geometry/passive/" + gdmlFileName);
  gdmlVolume->SetName("pipe");
  TGeoTranslation* passiveTrans2 =
    new TGeoTranslation(0., 0., -44);  // shifting to the center of the magnet coordinate system
  top->AddNode(gdmlVolume, id, passiveTrans2);
  id++;


  // Conical Sections - Wide Cone
  tmp[id]     = vol_from_binary(VMCWORKDIR + "/geometry/pipe/pipe_v21e.geo.root", "conical_beam_pipe_1");
  tmp_shift   = TGeoTranslation(0, 0, 123);
  tmp_mat[id] = (tmp_shift) * (*(tmp[id].pos));
  ((top->GetNode(0))->GetVolume())->AddNode((tmp[id].vol), id, &(tmp_mat[id]));
  id++;

  // Sts Flange
  tmp[id]  = vol_from_binary(VMCWORKDIR + "/geometry/sts/sts_v21f.geo.root", "Flange_volume_1");
  t_matrix = new TGeoTranslation(0, 0, 123.5);
  ((top->GetNode(0))->GetVolume())->AddNode((TGeoVolume*) (tmp[id].vol), id, t_matrix);
  id++;

  // Bellows
  TGeoVolumeAssembly* Bellows_Assembly = gman->MakeVolumeAssembly("bellows");
  ((top->GetNode(0))->GetVolume())->AddNode(Bellows_Assembly, id, t_matrix);

  int const num          = 43;
  TString list_vols[num] = {
    "sub_t_plate1_1", "sub_t_plate2_1",  "sub_t_plate3_1", "sub_t_plate4_1",  "sub_t_plate5_1",    "sub_t_plate6_1",
    "sub_s_plate1_1", "sub_s_plate2_1",  "sub_s_plate3_1", "sub_s_plate4_1",  "sub_s_plate5_1",    "sub_s_plate6_1",
    "sub_s_plate7_1", "sub_s_plate8_1",  "sub_s_plate9_1", "sub_s_plate10_1", "sub_t_plate1_2",    "sub_t_plate2_2",
    "sub_t_plate3_2", "sub_t_plate4_2",  "sub_t_plate5_2", "sub_t_plate6_2",  "sub_s_plate1_2",    "sub_s_plate2_2",
    "sub_s_plate3_2", "sub_s_plate4_2",  "sub_s_plate5_2", "sub_s_plate6_2",  "sub_s_plate7_2",    "sub_s_plate8_2",
    "sub_s_plate9_2", "sub_s_plate10_2", "sub_rail_1",     "sub_rail_2",      "bellow_flange_0_1", "bellow_flange_1_1",
    "bellow_s1_1",    "bellow_s2_1",     "bellow_s3_1",    "roll_plate_1",    "roll_plate_2",      "lead_screw_nut_1",
    "motor_1"};


  tmp_shift = TGeoTranslation(0, 0, 0);
  for (int i = 0; i < num; i++) {
    tmp[id]     = vol_from_binary(VMCWORKDIR + "/geometry/pipe/pipe_v21e.geo.root", list_vols[i]);
    tmp_mat[id] = (tmp_shift) * (*(tmp[id].pos));
    ((((top->GetNode(0))->GetVolume())->GetNode(6))->GetVolume())->AddNode((tmp[id].vol), id, &(tmp_mat[id]));
    id++;
  };

  /* Get Missing volume due to double naming error in binary */
  tmp[id]     = vol_from_binary(VMCWORKDIR + "/geometry/pipe/pipe_v21e.geo.root", "bellow_s3_1", 2);
  tmp_mat[id] = (tmp_shift) * (*(tmp[id].pos));
  ((((top->GetNode(0))->GetVolume())->GetNode(6))->GetVolume())->AddNode((tmp[id].vol), id, &(tmp_mat[id]));
  id++;

  /* Downstream tubular section */
  tmp[id]     = vol_from_binary(VMCWORKDIR + "/geometry/pipe/pipe_v21e.geo.root", "psd_tube_1");
  tmp_shift   = TGeoTranslation(0, 0, 123);
  tmp_mat[id] = (tmp_shift) * (*(tmp[id].pos));
  ((top->GetNode(0))->GetVolume())->AddNode((tmp[id].vol), id, &(tmp_mat[id]));
  id++;

  // Works to import in the entire target box extract from STS v21f
  // Coped other assembleis.
  // Comparison between flanges placements one at 57 and other at 54.3 so 2.7cm difference.
  TString targetFileName = "targetbox_v22a.gdml";
  gdmlVolume             = parser.GDMLReadFile(VMCWORKDIR + "/geometry/passive/" + targetFileName);
  gdmlVolume->SetName("targetbox");
  TGeoTranslation* passiveTrans = new TGeoTranslation(0., 0., 68.25 + 1.175 + 1.8);
  gdmlVolume->GetShape()->ComputeBBox();
  ((top->GetNode(0))->GetVolume())->AddNode(gdmlVolume, id++, passiveTrans);


  std::cout << "Drawing" << std::endl;
  // top->Draw();

  std::cout << "Writing to Disk" << std::endl;
  // ---------------   Finish   -----------------------------------------------
  TFile* output;
  output = new TFile("pipe_v22b.geo.root", "RECREATE");
  (top->CloneVolume())->Write();
  //	gman->Export("pipe_v22b.gdml");

  output->Close();

  // visualize it with ray tracing, OGL/X3D viewer
  // top->Raytrace();
  // top->Draw("ogl");
  // top->Draw("x3d");

  gman->CheckOverlaps(0.00001);
  gman->PrintOverlaps();
  gman->GetListOfOverlaps()->Print();
}
