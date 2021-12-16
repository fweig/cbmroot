/* Copyright (C) 2021 Nuclear Physics Institute of the CAS, Rez
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Petr Chudoba  [committer] */

#include "TGeoArb8.h"
#include "TGeoCompositeShape.h"
#include "TGeoCone.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoSystemOfUnits.h"
#include "TGeoTube.h"
#include "TPad.h"
#include "TROOT.h"
#include "TSystem.h"

#include "iostream"
#include "stdio.h"
using namespace std;
using namespace TGeoUnit;

void create_bpipe_geometry_v21e(const char* geoTag = "v21e")
{
  //--- Definition of a simple geometry
  gSystem->Load("libGeom");
  TGeoManager* geom = new TGeoManager("beampipe_v21e", "Beampipe for maximum deflection");


  //--- define some materials
  TGeoMixture* matCarbonFiber = new TGeoMixture("BP_carbonfiber", 4, 1.5);
  matCarbonFiber->AddElement(12.001, 6.0, 0.8926);
  matCarbonFiber->AddElement(1.0079, 1.0, 0.0192);
  matCarbonFiber->AddElement(35.453, 17.0, 0.0271);
  matCarbonFiber->AddElement(15.999, 8.0, 0.0611);

  TGeoMixture* matStainless = new TGeoMixture("BP_stainless", 10, 8.0);
  matStainless->AddElement(12.0, 6.0, 0.00035);
  matStainless->AddElement(51.94, 24.0, 0.175);
  matStainless->AddElement(54.94, 25.0, 0.01);
  matStainless->AddElement(95.94, 42.0, 0.025);
  matStainless->AddElement(14.0, 7.0, 0.0005);
  matStainless->AddElement(58.6934, 28.0, 0.115);
  matStainless->AddElement(30.97, 15.0, 0.000225);
  matStainless->AddElement(31.97, 16.0, 0.000075);
  matStainless->AddElement(27.98, 14.0, 0.005);
  matStainless->AddElement(55.93, 26.0, 0.67135);

  TGeoMaterial* matVacuum = new TGeoMaterial("BP_vacuum", 1.e-16, 1.e-16, 1.e-16);
  TGeoMaterial* matAl     = new TGeoMaterial("BP_aluminium", 26.9815386, 13, 2.7, 24.01);

  //   //--- define some media
  TGeoMedium* Vacuum    = new TGeoMedium("BP_vacuum", 1, matVacuum);
  TGeoMedium* Al        = new TGeoMedium("BP_aluminium", 2, matAl);
  TGeoMedium* CF        = new TGeoMedium("BP_carbonfiber", 3, matCarbonFiber);
  TGeoMedium* Stainless = new TGeoMedium("BP_stainless", 4, matStainless);

  Double_t conus_lenght = 1585 * mm;
  Double_t rmin1        = 55 * mm;
  Double_t rmax1        = 56 * mm;
  Double_t rmin2        = 125 * mm;
  Double_t rmax2        = 126 * mm;

  Double_t rail_cord[]       = {23.1, 0, 4.5, 21.5, 2.8, 4.5};
  Double_t top_plate_cord[]  = {10,   0,    2.1,  10.15, 0,     4.05, 11.75, 2.15, 4.0,  15.3, 4.28,
                               2.85, 18.1, 5.27, 2.85,  19.75, 5.65, 5.0,   21.1, 5.65, 0};
  Double_t side_plate_cord[] = {11.3, 0,   2,    11.3, 0,   7.55, 16,   6.8, 4.6, 8.7,  6.8, 4.6,
                                19.8, 8.3, 2.35, 13.6, 8.3, 2.35, 20.8, 7.9, 3.0, 14.8, 7.9, 3.0,
                                21.3, 6.8, 1.5,  16.3, 6.8, 1.5,  21.3, 6.3, 0,   17.4, 6.3, 0};


  //-----------------------DEFINITION OF TRANSFORMATIONS--------------------------------------------------------------------------------------

  TGeoRotation* rot_top = new TGeoRotation("rot_top");
  rot_top->SetAngles(0, 0, 0);
  TGeoCombiTrans* combi_top = new TGeoCombiTrans(0, 15.2, 0, rot_top);

  TGeoRotation* rot_bott = new TGeoRotation("rot_bott");
  rot_bott->SetAngles(180, 0, 0);
  TGeoCombiTrans* combi_bott = new TGeoCombiTrans(0, -15.2, 0, rot_bott);

  TGeoRotation* rot_rail_top = new TGeoRotation("rot_rail_top");
  rot_rail_top->SetAngles(0, 0, 0);
  TGeoCombiTrans* combi_rail_top = new TGeoCombiTrans(0, 20.15 - rail_cord[4], rail_cord[2] + 33, rot_rail_top);

  TGeoRotation* rot_rail = new TGeoRotation("rot_rail_bott");
  rot_rail->SetAngles(0, 0, 0);
  TGeoCombiTrans* combi_rail_bott = new TGeoCombiTrans(0, -20.15, rail_cord[2] + 33, rot_rail);


  TGeoTranslation* ident_translation = new TGeoTranslation("ident_translation", 0, 0, 0);
  ident_translation->RegisterYourself();

  TGeoRotation* ident_rotation = new TGeoRotation("ident_rotation");
  ident_rotation->RotateY(0);
  ident_rotation->RotateY(0);
  ident_rotation->RegisterYourself();

  TGeoTranslation orig_translation("orig_translation", -0.15707676 * mm, 0, 3230 * mm);
  //TGeoCombiTrans *origin_translation = new TGeoCombiTrans(orig_translation,ident);


  TGeoRotation orig_rotation("orig_rotation");
  orig_rotation.RotateY(1.2);
  //TGeoCombiTrans *origin_rotation = new TGeoCombiTrans(0,orig_rotation);
  //orig_rotation->RegisterYourself();


  //TGeoHMatrix orig_combi = orig_translation * orig_rotation;
  TGeoHMatrix orig_combi = orig_rotation * orig_translation;

  TGeoHMatrix* origin_combi = new TGeoHMatrix(orig_combi);


  TGeoRotation* rot_left = new TGeoRotation("rot_left");
  rot_left->SetAngles(90, 0, 0);
  TGeoCombiTrans* combi_left = new TGeoCombiTrans(-15.2, 0, 0, rot_left);

  TGeoRotation* rot_right = new TGeoRotation("rot_right");
  rot_right->SetAngles(270, 0, 0);
  TGeoCombiTrans* combi_right = new TGeoCombiTrans(15.2, 0, 0, rot_right);

  TGeoCombiTrans* tr_top_0a = new TGeoCombiTrans(0, 15.2, top_plate_cord[2], rot_top);
  TGeoCombiTrans* tr_top_1a = new TGeoCombiTrans(0, 15.2, 2 * top_plate_cord[2] + top_plate_cord[5], rot_top);
  TGeoCombiTrans* tr_top_2a = new TGeoCombiTrans(
    0, 15.2, 2 * top_plate_cord[2] + top_plate_cord[5] + top_plate_cord[5] + top_plate_cord[8], rot_top);
  TGeoCombiTrans* tr_top_3a = new TGeoCombiTrans(0, 15.2,
                                                 2 * top_plate_cord[2] + top_plate_cord[5] + top_plate_cord[5]
                                                   + top_plate_cord[8] + top_plate_cord[8] + top_plate_cord[11],
                                                 rot_top);
  TGeoCombiTrans* tr_top_4a =
    new TGeoCombiTrans(0, 15.2,
                       2 * top_plate_cord[2] + top_plate_cord[5] + top_plate_cord[5] + top_plate_cord[8]
                         + top_plate_cord[8] + top_plate_cord[11] + top_plate_cord[11] + top_plate_cord[14],
                       rot_top);
  TGeoCombiTrans* tr_top_5a = new TGeoCombiTrans(
    0, 15.2,
    2 * top_plate_cord[2] + top_plate_cord[5] + top_plate_cord[5] + top_plate_cord[8] + top_plate_cord[8]
      + top_plate_cord[11] + top_plate_cord[11] + top_plate_cord[14] + top_plate_cord[14] + top_plate_cord[17],
    rot_top);

  TGeoCombiTrans* tr_bott_0a = new TGeoCombiTrans(0, -15.2, top_plate_cord[2], rot_bott);
  TGeoCombiTrans* tr_bott_1a = new TGeoCombiTrans(0, -15.2, 2 * top_plate_cord[2] + top_plate_cord[5], rot_bott);
  TGeoCombiTrans* tr_bott_2a = new TGeoCombiTrans(
    0, -15.2, 2 * top_plate_cord[2] + top_plate_cord[5] + top_plate_cord[5] + top_plate_cord[8], rot_bott);
  TGeoCombiTrans* tr_bott_3a = new TGeoCombiTrans(0, -15.2,
                                                  2 * top_plate_cord[2] + top_plate_cord[5] + top_plate_cord[5]
                                                    + top_plate_cord[8] + top_plate_cord[8] + top_plate_cord[11],
                                                  rot_bott);
  TGeoCombiTrans* tr_bott_4a =
    new TGeoCombiTrans(0, -15.2,
                       2 * top_plate_cord[2] + top_plate_cord[5] + top_plate_cord[5] + top_plate_cord[8]
                         + top_plate_cord[8] + top_plate_cord[11] + top_plate_cord[11] + top_plate_cord[14],
                       rot_bott);
  TGeoCombiTrans* tr_bott_5a = new TGeoCombiTrans(
    0, -15.2,
    2 * top_plate_cord[2] + top_plate_cord[5] + top_plate_cord[5] + top_plate_cord[8] + top_plate_cord[8]
      + top_plate_cord[11] + top_plate_cord[11] + top_plate_cord[14] + top_plate_cord[14] + top_plate_cord[17],
    rot_bott);

  TGeoCombiTrans* tr_side_left_0a = new TGeoCombiTrans(-15.2, 0, side_plate_cord[2], rot_left);
  TGeoCombiTrans* tr_side_left_1a = new TGeoCombiTrans(-15.2, 0, 2 * side_plate_cord[2] + side_plate_cord[5], rot_left);
  TGeoCombiTrans* tr_side_left_2a = new TGeoCombiTrans(
    -15.2, 0, 2 * side_plate_cord[2] + side_plate_cord[5] + side_plate_cord[5] + side_plate_cord[8], rot_left);
  TGeoCombiTrans* tr_side_left_3a =
    new TGeoCombiTrans(-15.2, 0,
                       2 * side_plate_cord[2] + side_plate_cord[5] + side_plate_cord[5] + side_plate_cord[8]
                         + side_plate_cord[8] + side_plate_cord[14],
                       rot_left);
  TGeoCombiTrans* tr_side_left_4a =
    new TGeoCombiTrans(-15.2, 0,
                       2 * side_plate_cord[2] + side_plate_cord[5] + side_plate_cord[5] + side_plate_cord[8]
                         + side_plate_cord[8] + side_plate_cord[14] + side_plate_cord[14] + side_plate_cord[20],
                       rot_left);
  TGeoCombiTrans* tr_side_left_5a = new TGeoCombiTrans(
    -15.2, 0,
    2 * side_plate_cord[2] + side_plate_cord[5] + side_plate_cord[5] + side_plate_cord[8] + side_plate_cord[8]
      + side_plate_cord[14] + side_plate_cord[14] + side_plate_cord[20] + side_plate_cord[20] + side_plate_cord[26],
    rot_left);

  TGeoCombiTrans* tr_side_right_0a = new TGeoCombiTrans(15.2, 0, side_plate_cord[2], rot_right);
  TGeoCombiTrans* tr_side_right_1a =
    new TGeoCombiTrans(15.2, 0, 2 * side_plate_cord[2] + side_plate_cord[5], rot_right);
  TGeoCombiTrans* tr_side_right_2a = new TGeoCombiTrans(
    15.2, 0, 2 * side_plate_cord[2] + side_plate_cord[5] + side_plate_cord[5] + side_plate_cord[8], rot_right);
  TGeoCombiTrans* tr_side_right_3a =
    new TGeoCombiTrans(15.2, 0,
                       2 * side_plate_cord[2] + side_plate_cord[5] + side_plate_cord[5] + side_plate_cord[8]
                         + side_plate_cord[8] + side_plate_cord[14],
                       rot_right);
  TGeoCombiTrans* tr_side_right_4a =
    new TGeoCombiTrans(15.2, 0,
                       2 * side_plate_cord[2] + side_plate_cord[5] + side_plate_cord[5] + side_plate_cord[8]
                         + side_plate_cord[8] + side_plate_cord[14] + side_plate_cord[14] + side_plate_cord[20],
                       rot_right);
  TGeoCombiTrans* tr_side_right_5a = new TGeoCombiTrans(
    15.2, 0,
    2 * side_plate_cord[2] + side_plate_cord[5] + side_plate_cord[5] + side_plate_cord[8] + side_plate_cord[8]
      + side_plate_cord[14] + side_plate_cord[14] + side_plate_cord[20] + side_plate_cord[20] + side_plate_cord[26],
    rot_right);

  TGeoRotation* rot_sts_conus_connection = new TGeoRotation("rot_sts_conus_connection");
  rot_sts_conus_connection->RotateZ(180);
  TGeoCombiTrans* ctr_sts_conus_connection = new TGeoCombiTrans(0, 0, -3185 * mm, rot_sts_conus_connection);

  TGeoRotation* rotation_of_sts_tube = new TGeoRotation("rotation_of_sts_tube");
  rotation_of_sts_tube->RotateZ(180);
  rotation_of_sts_tube->RotateY(-1.2);
  TGeoCombiTrans* ctr_sts_tube = new TGeoCombiTrans(0.15707676 * mm, 0, -3214.99917754 * mm, rotation_of_sts_tube);

  TGeoRotation* rot_global = new TGeoRotation("rot_global");
  rot_global->RotateY(1.2);
  TGeoCombiTrans* ctr_global = new TGeoCombiTrans(0, 0, 3214.99917754 * mm, rot_global);

  TGeoRotation* rot_s1 = new TGeoRotation("rot_s1");
  rot_s1->RotateZ(180.);
  rot_s1->RotateY(4.80111677);
  TGeoCombiTrans* ctr_s1 = new TGeoCombiTrans("ctr_s1", 2.50041582 * mm, 0, 59.76969908 * mm, rot_s1);

  TGeoRotation* rot_s2 = new TGeoRotation("rot_s2");
  rot_s2->RotateZ(180.);
  rot_s2->RotateY(18.84127012);
  TGeoCombiTrans* ctr_s2 = new TGeoCombiTrans("ctr_s2", (36.22434468) * mm, 0, (181.04176807) * mm, rot_s2);

  TGeoRotation* rot_s3 = new TGeoRotation("rot_s3");
  //rot_s3->RotateZ(180.);
  rot_s3->RotateY(8.5257557);

  TGeoCombiTrans* ctr_s3 = new TGeoCombiTrans("ctr_s3", (70.72502201) * mm, 0, (294.40492979) * mm, rot_s3);

  TGeoRotation* rot_s4 = new TGeoRotation("rot_s4");
  //rot_s4->RotateZ(180.);
  rot_s4->RotateY(5.7511677);

  TGeoCombiTrans* ctr_s4 = new TGeoCombiTrans("ctr_s4", (76.53811002) * mm, 0, 341.44507901 * mm, rot_s4);


  TGeoRotation* rot_plate = new TGeoRotation("rot_plate");
  rot_plate->RotateY(-0.95);
  TGeoCombiTrans* ctr_plate_up =
    new TGeoCombiTrans("ctr_plate_up", 80. * mm, (147 + 12.5) * mm, (292.8 + 90) * mm, rot_plate);
  TGeoCombiTrans* ctr_plate_down =
    new TGeoCombiTrans("ctr_plate_down", 80. * mm, -(147 + 12.5) * mm, (292.8 + 90) * mm, rot_plate);
  TGeoTranslation* tr_plate_up = new TGeoTranslation("tr_plate_up", 0. * mm, (147 + 12.5) * mm, 0. * mm);
  tr_plate_up->RegisterYourself();
  TGeoTranslation* tr_plate_down = new TGeoTranslation("tr_plate_down", 0. * mm, -(147 + 12.5) * mm, 0. * mm);
  tr_plate_down->RegisterYourself();

  TGeoRotation* rot_lsn = new TGeoRotation("rot_lsn");
  //rot_s5->RotateZ(180.);
  rot_lsn->RotateY(-90);
  TGeoCombiTrans* ctr_lsn = new TGeoCombiTrans("ctr_s5", -22.5 * mm, -159.5 * mm, 534. * mm, rot_lsn);

  TGeoTranslation* mot_transl = new TGeoTranslation("mot_transl", -286.5 * mm, -159.5 * mm, 534. * mm);

  TGeoRotation* rot_psd_tube = new TGeoRotation("rot_psd_tube");
  rot_psd_tube->RotateY(2.15);
  TGeoCombiTrans* ctr_psd_tube =
    new TGeoCombiTrans("ctr_psd_tube", 426.44387679 * mm, 0. * mm, 10849.15 * mm, rot_psd_tube);

  TGeoRotation* rot_s5 = new TGeoRotation("rot_s5");
  //rot_s5->RotateZ(180.);
  rot_s5->RotateY(0.95);
  TGeoCombiTrans* ctr_s5 = new TGeoCombiTrans("ctr_s5", (79.32273177) * mm, 0, 381.62237458 * mm, rot_s5);

  TGeoTranslation* nut_transl = new TGeoTranslation("nut_transl", 0. * mm, 0. * mm, -100. * mm);
  nut_transl->RegisterYourself();

  //------------------------------------DEFINITIONS OF VOLUMES----------------------------------------
  //--- make the top container volume
  Double_t worldx           = 320 * mm;
  Double_t worldy           = 220 * mm;
  Double_t worldz           = 18100 * mm;
  TGeoVolume* full_assembly = geom->MakeBox("pipe_v21e", Vacuum, worldx, worldy, worldz);
  geom->SetTopVolume(full_assembly);
  full_assembly->SetVisibility(kTRUE);
  TGeoVolume* conical_beam_pipe = geom->MakeBox("conical_beam_pipe", Vacuum, 220 * mm, 220 * mm, 3650 * mm);
  conical_beam_pipe->SetVisibility(kFALSE);
  TGeoVolume* bellow_ass = geom->MakeBox("Bellow assembly", Vacuum, 320 * mm, 220 * mm, 240 * mm);
  bellow_ass->SetVisibility(kFALSE);

  // TOP PLATE--------------------------------------------------------------------------------------
  TGeoVolume* sub_t_plate1 = geom->MakeArb8("sub_t_plate1", CF, top_plate_cord[2]);
  sub_t_plate1->SetLineColor(kGray);
  TGeoArb8* t_arb1 = (TGeoArb8*) sub_t_plate1->GetShape();
  t_arb1->SetVertex(0, top_plate_cord[0], top_plate_cord[1]);
  t_arb1->SetVertex(1, -top_plate_cord[0], top_plate_cord[1]);
  t_arb1->SetVertex(2, -top_plate_cord[0], top_plate_cord[1] + 1.0);
  t_arb1->SetVertex(3, top_plate_cord[0], top_plate_cord[1] + 1.0);
  t_arb1->SetVertex(4, top_plate_cord[3], top_plate_cord[4]);
  t_arb1->SetVertex(5, -top_plate_cord[3], top_plate_cord[4]);
  t_arb1->SetVertex(6, -top_plate_cord[3], top_plate_cord[4] + 1.0);
  t_arb1->SetVertex(7, top_plate_cord[3], top_plate_cord[4] + 1.0);

  TGeoVolume* sub_t_plate2 = geom->MakeArb8("sub_t_plate2", CF, top_plate_cord[5]);
  sub_t_plate2->SetLineColor(kGray);
  TGeoArb8* t_arb2 = (TGeoArb8*) sub_t_plate2->GetShape();
  t_arb2->SetVertex(0, top_plate_cord[3], top_plate_cord[4]);
  t_arb2->SetVertex(1, -top_plate_cord[3], top_plate_cord[4]);
  t_arb2->SetVertex(2, -top_plate_cord[3], top_plate_cord[4] + 1.0);
  t_arb2->SetVertex(3, top_plate_cord[3], top_plate_cord[4] + 1.0);
  t_arb2->SetVertex(4, top_plate_cord[6], top_plate_cord[7]);
  t_arb2->SetVertex(5, -top_plate_cord[6], top_plate_cord[7]);
  t_arb2->SetVertex(6, -top_plate_cord[6], top_plate_cord[7] + 1.0);
  t_arb2->SetVertex(7, top_plate_cord[6], top_plate_cord[7] + 1.0);

  TGeoVolume* sub_t_plate3 = geom->MakeArb8("sub_t_plate3", CF, top_plate_cord[8]);
  sub_t_plate3->SetLineColor(kGray);
  TGeoArb8* t_arb3 = (TGeoArb8*) sub_t_plate3->GetShape();
  t_arb3->SetVertex(0, top_plate_cord[6], top_plate_cord[7]);
  t_arb3->SetVertex(1, -top_plate_cord[6], top_plate_cord[7]);
  t_arb3->SetVertex(2, -top_plate_cord[6], top_plate_cord[7] + 1.0);
  t_arb3->SetVertex(3, top_plate_cord[6], top_plate_cord[7] + 1.0);
  t_arb3->SetVertex(4, top_plate_cord[9], top_plate_cord[10]);
  t_arb3->SetVertex(5, -top_plate_cord[9], top_plate_cord[10]);
  t_arb3->SetVertex(6, -top_plate_cord[9], top_plate_cord[10] + 1.0);
  t_arb3->SetVertex(7, top_plate_cord[9], top_plate_cord[10] + 1.0);

  TGeoVolume* sub_t_plate4 = geom->MakeArb8("sub_t_plate4", CF, top_plate_cord[11]);
  sub_t_plate4->SetLineColor(kGray);
  TGeoArb8* t_arb4 = (TGeoArb8*) sub_t_plate4->GetShape();
  t_arb4->SetVertex(0, top_plate_cord[9], top_plate_cord[10]);
  t_arb4->SetVertex(1, -top_plate_cord[9], top_plate_cord[10]);
  t_arb4->SetVertex(2, -top_plate_cord[9], top_plate_cord[10] + 1.0);
  t_arb4->SetVertex(3, top_plate_cord[9], top_plate_cord[10] + 1.0);
  t_arb4->SetVertex(4, top_plate_cord[12], top_plate_cord[13]);
  t_arb4->SetVertex(5, -top_plate_cord[12], top_plate_cord[13]);
  t_arb4->SetVertex(6, -top_plate_cord[12], top_plate_cord[13] + 1.0);
  t_arb4->SetVertex(7, top_plate_cord[12], top_plate_cord[13] + 1.0);

  TGeoVolume* sub_t_plate5 = geom->MakeArb8("sub_t_plate5", CF, top_plate_cord[14]);
  sub_t_plate5->SetLineColor(kGray);
  TGeoArb8* t_arb5 = (TGeoArb8*) sub_t_plate5->GetShape();
  t_arb5->SetVertex(0, top_plate_cord[12], top_plate_cord[13]);
  t_arb5->SetVertex(1, -top_plate_cord[12], top_plate_cord[13]);
  t_arb5->SetVertex(2, -top_plate_cord[12], top_plate_cord[13] + 1.0);
  t_arb5->SetVertex(3, top_plate_cord[12], top_plate_cord[13] + 1.0);
  t_arb5->SetVertex(4, top_plate_cord[15], top_plate_cord[17]);
  t_arb5->SetVertex(5, -top_plate_cord[15], top_plate_cord[17]);
  t_arb5->SetVertex(6, -top_plate_cord[15], top_plate_cord[17] + 1.0);
  t_arb5->SetVertex(7, top_plate_cord[15], top_plate_cord[17] + 1.0);

  TGeoVolume* sub_t_plate6 = geom->MakeArb8("sub_t_plate6", CF, top_plate_cord[17]);
  sub_t_plate6->SetLineColor(kGray);
  TGeoArb8* t_arb6 = (TGeoArb8*) sub_t_plate6->GetShape();
  t_arb6->SetVertex(0, top_plate_cord[15], top_plate_cord[17]);
  t_arb6->SetVertex(1, -top_plate_cord[15], top_plate_cord[17]);
  t_arb6->SetVertex(2, -top_plate_cord[15], top_plate_cord[17] + 1.0);
  t_arb6->SetVertex(3, top_plate_cord[15], top_plate_cord[17] + 1.0);
  t_arb6->SetVertex(4, top_plate_cord[18], top_plate_cord[17]);
  t_arb6->SetVertex(5, -top_plate_cord[18], top_plate_cord[17]);
  t_arb6->SetVertex(6, -top_plate_cord[18], top_plate_cord[17] + 1.0);
  t_arb6->SetVertex(7, top_plate_cord[18], top_plate_cord[17] + 1.0);

  // SIDE PLATE--------------------------------------------------------------------------------------

  TGeoVolume* sub_s_plate1 = geom->MakeArb8("sub_s_plate1", CF, side_plate_cord[2]);
  sub_s_plate1->SetLineColor(kGray);
  TGeoArb8* s_arb1 = (TGeoArb8*) sub_s_plate1->GetShape();
  s_arb1->SetVertex(0, side_plate_cord[0], side_plate_cord[1]);
  s_arb1->SetVertex(1, -side_plate_cord[0], side_plate_cord[1]);
  s_arb1->SetVertex(2, -side_plate_cord[0], side_plate_cord[1] + 1.0);
  s_arb1->SetVertex(3, side_plate_cord[0], side_plate_cord[1] + 1.0);
  s_arb1->SetVertex(4, side_plate_cord[3], side_plate_cord[4]);
  s_arb1->SetVertex(5, -side_plate_cord[3], side_plate_cord[4]);
  s_arb1->SetVertex(6, -side_plate_cord[3], side_plate_cord[4] + 1.0);
  s_arb1->SetVertex(7, side_plate_cord[3], side_plate_cord[4] + 1.0);

  TGeoVolume* sub_s_plate2 = geom->MakeArb8("sub_s_plate2", CF, side_plate_cord[5]);
  sub_s_plate2->SetLineColor(kGray);
  TGeoArb8* s_arb2 = (TGeoArb8*) sub_s_plate2->GetShape();
  s_arb2->SetVertex(0, side_plate_cord[3], side_plate_cord[4]);
  s_arb2->SetVertex(1, -side_plate_cord[3], side_plate_cord[4]);
  s_arb2->SetVertex(2, -side_plate_cord[3], side_plate_cord[4] + 1.0);
  s_arb2->SetVertex(3, side_plate_cord[3], side_plate_cord[4] + 1.0);
  s_arb2->SetVertex(4, side_plate_cord[6], side_plate_cord[7]);
  s_arb2->SetVertex(5, -side_plate_cord[6], side_plate_cord[7]);
  s_arb2->SetVertex(6, -side_plate_cord[6], side_plate_cord[7] + 1.0);
  s_arb2->SetVertex(7, side_plate_cord[6], side_plate_cord[7] + 1.0);

  TGeoVolume* sub_s_plate3 = geom->MakeArb8("sub_s_plate3", CF, side_plate_cord[8]);
  sub_s_plate3->SetLineColor(kGray);
  TGeoArb8* s_arb3 = (TGeoArb8*) sub_s_plate3->GetShape();
  s_arb3->SetVertex(0, side_plate_cord[6], side_plate_cord[7]);
  s_arb3->SetVertex(1, side_plate_cord[9], side_plate_cord[10]);
  s_arb3->SetVertex(2, side_plate_cord[9], side_plate_cord[10] + 1.0);
  s_arb3->SetVertex(3, side_plate_cord[6], side_plate_cord[7] + 1.0);
  s_arb3->SetVertex(4, side_plate_cord[12], side_plate_cord[13]);
  s_arb3->SetVertex(5, side_plate_cord[15], side_plate_cord[16]);
  s_arb3->SetVertex(6, side_plate_cord[15], side_plate_cord[16] + 1.0);
  s_arb3->SetVertex(7, side_plate_cord[12], side_plate_cord[13] + 1.0);

  TGeoVolume* sub_s_plate4 = geom->MakeArb8("sub_s_plate4", CF, side_plate_cord[8]);
  sub_s_plate4->SetLineColor(kGray);
  TGeoArb8* s_arb4 = (TGeoArb8*) sub_s_plate4->GetShape();
  s_arb4->SetVertex(0, -side_plate_cord[6], side_plate_cord[7]);
  s_arb4->SetVertex(1, -side_plate_cord[9], side_plate_cord[10]);
  s_arb4->SetVertex(2, -side_plate_cord[9], side_plate_cord[10] + 1.0);
  s_arb4->SetVertex(3, -side_plate_cord[6], side_plate_cord[7] + 1.0);
  s_arb4->SetVertex(4, -side_plate_cord[12], side_plate_cord[13]);
  s_arb4->SetVertex(5, -side_plate_cord[15], side_plate_cord[16]);
  s_arb4->SetVertex(6, -side_plate_cord[15], side_plate_cord[16] + 1.0);
  s_arb4->SetVertex(7, -side_plate_cord[12], side_plate_cord[13] + 1.0);

  TGeoVolume* sub_s_plate5 = geom->MakeArb8("sub_s_plate5", CF, side_plate_cord[14]);
  sub_s_plate5->SetLineColor(kGray);
  TGeoArb8* s_arb5 = (TGeoArb8*) sub_s_plate5->GetShape();
  s_arb5->SetVertex(0, side_plate_cord[12], side_plate_cord[13]);
  s_arb5->SetVertex(1, side_plate_cord[15], side_plate_cord[16]);
  s_arb5->SetVertex(2, side_plate_cord[15], side_plate_cord[16] + 1.0);
  s_arb5->SetVertex(3, side_plate_cord[12], side_plate_cord[13] + 1.0);
  s_arb5->SetVertex(4, side_plate_cord[18], side_plate_cord[19]);
  s_arb5->SetVertex(5, side_plate_cord[21], side_plate_cord[22]);
  s_arb5->SetVertex(6, side_plate_cord[21], side_plate_cord[22] + 1.0);
  s_arb5->SetVertex(7, side_plate_cord[18], side_plate_cord[19] + 1.0);

  TGeoVolume* sub_s_plate6 = geom->MakeArb8("sub_s_plate6", CF, side_plate_cord[14]);
  sub_s_plate6->SetLineColor(kGray);
  TGeoArb8* s_arb6 = (TGeoArb8*) sub_s_plate6->GetShape();
  s_arb6->SetVertex(0, -side_plate_cord[12], side_plate_cord[13]);
  s_arb6->SetVertex(1, -side_plate_cord[15], side_plate_cord[16]);
  s_arb6->SetVertex(2, -side_plate_cord[15], side_plate_cord[16] + 1.0);
  s_arb6->SetVertex(3, -side_plate_cord[12], side_plate_cord[13] + 1.0);
  s_arb6->SetVertex(4, -side_plate_cord[18], side_plate_cord[19]);
  s_arb6->SetVertex(5, -side_plate_cord[21], side_plate_cord[22]);
  s_arb6->SetVertex(6, -side_plate_cord[21], side_plate_cord[22] + 1.0);
  s_arb6->SetVertex(7, -side_plate_cord[18], side_plate_cord[19] + 1.0);

  TGeoVolume* sub_s_plate7 = geom->MakeArb8("sub_s_plate7", CF, side_plate_cord[20]);
  sub_s_plate7->SetLineColor(kGray);
  TGeoArb8* s_arb7 = (TGeoArb8*) sub_s_plate7->GetShape();
  s_arb7->SetVertex(0, side_plate_cord[18], side_plate_cord[19]);
  s_arb7->SetVertex(1, side_plate_cord[21], side_plate_cord[22]);
  s_arb7->SetVertex(2, side_plate_cord[21], side_plate_cord[22] + 1.0);
  s_arb7->SetVertex(3, side_plate_cord[18], side_plate_cord[19] + 1.0);
  s_arb7->SetVertex(4, side_plate_cord[24], side_plate_cord[25]);
  s_arb7->SetVertex(5, side_plate_cord[27], side_plate_cord[28]);
  s_arb7->SetVertex(6, side_plate_cord[27], side_plate_cord[28] + 1.0);
  s_arb7->SetVertex(7, side_plate_cord[24], side_plate_cord[25] + 1.0);

  TGeoVolume* sub_s_plate8 = geom->MakeArb8("sub_s_plate8", CF, side_plate_cord[20]);
  sub_s_plate8->SetLineColor(kGray);
  TGeoArb8* s_arb8 = (TGeoArb8*) sub_s_plate8->GetShape();
  s_arb8->SetVertex(0, -side_plate_cord[18], side_plate_cord[19]);
  s_arb8->SetVertex(1, -side_plate_cord[21], side_plate_cord[22]);
  s_arb8->SetVertex(2, -side_plate_cord[21], side_plate_cord[22] + 1.0);
  s_arb8->SetVertex(3, -side_plate_cord[18], side_plate_cord[19] + 1.0);
  s_arb8->SetVertex(4, -side_plate_cord[24], side_plate_cord[25]);
  s_arb8->SetVertex(5, -side_plate_cord[27], side_plate_cord[28]);
  s_arb8->SetVertex(6, -side_plate_cord[27], side_plate_cord[28] + 1.0);
  s_arb8->SetVertex(7, -side_plate_cord[24], side_plate_cord[25] + 1.0);

  TGeoVolume* sub_s_plate9 = geom->MakeArb8("sub_s_plate9", CF, side_plate_cord[26]);
  sub_s_plate9->SetLineColor(kGray);
  TGeoArb8* s_arb9 = (TGeoArb8*) sub_s_plate9->GetShape();
  s_arb9->SetVertex(0, side_plate_cord[24], side_plate_cord[25]);
  s_arb9->SetVertex(1, side_plate_cord[27], side_plate_cord[28]);
  s_arb9->SetVertex(2, side_plate_cord[27], side_plate_cord[28] + 1.0);
  s_arb9->SetVertex(3, side_plate_cord[24], side_plate_cord[25] + 1.0);
  s_arb9->SetVertex(4, side_plate_cord[30], side_plate_cord[31]);
  s_arb9->SetVertex(5, side_plate_cord[33], side_plate_cord[34]);
  s_arb9->SetVertex(6, side_plate_cord[33], side_plate_cord[34] + 1.0);
  s_arb9->SetVertex(7, side_plate_cord[30], side_plate_cord[31] + 1.0);

  TGeoVolume* sub_s_plate10 = geom->MakeArb8("sub_s_plate10", CF, side_plate_cord[26]);
  sub_s_plate10->SetLineColor(kGray);
  TGeoArb8* s_arb10 = (TGeoArb8*) sub_s_plate10->GetShape();
  s_arb10->SetVertex(0, -side_plate_cord[24], side_plate_cord[25]);
  s_arb10->SetVertex(1, -side_plate_cord[27], side_plate_cord[28]);
  s_arb10->SetVertex(2, -side_plate_cord[27], side_plate_cord[28] + 1.0);
  s_arb10->SetVertex(3, -side_plate_cord[24], side_plate_cord[25] + 1.0);
  s_arb10->SetVertex(4, -side_plate_cord[30], side_plate_cord[31]);
  s_arb10->SetVertex(5, -side_plate_cord[33], side_plate_cord[34]);
  s_arb10->SetVertex(6, -side_plate_cord[33], side_plate_cord[34] + 1.0);
  s_arb10->SetVertex(7, -side_plate_cord[30], side_plate_cord[31] + 1.0);

  // RAIL------------------------------------------------------------------------------------
  TGeoVolume* sub_rail = geom->MakeArb8("sub_rail", Al, rail_cord[2]);
  sub_rail->SetLineColor(kBlue);
  TGeoArb8* r_arb1 = (TGeoArb8*) sub_rail->GetShape();
  r_arb1->SetVertex(0, rail_cord[0], rail_cord[1]);
  r_arb1->SetVertex(1, -rail_cord[0], rail_cord[1]);
  r_arb1->SetVertex(2, -rail_cord[0], rail_cord[1] + rail_cord[4]);
  r_arb1->SetVertex(3, rail_cord[0], rail_cord[1] + rail_cord[4]);
  r_arb1->SetVertex(4, rail_cord[3], rail_cord[1]);
  r_arb1->SetVertex(5, -rail_cord[3], rail_cord[1]);
  r_arb1->SetVertex(6, -rail_cord[3], rail_cord[1] + rail_cord[4]);
  r_arb1->SetVertex(7, rail_cord[3], rail_cord[1] + rail_cord[4]);

  TGeoBBox* flange_box  = new TGeoBBox("flange_box", 15.2, 15.2, 1.5);
  TGeoTube* flange_hole = new TGeoTube("flange_hole", 0., 12.5, 2.5);

  // DOWNSTREAM_CARBON-FIBRE_RICH_MUCH_FLANGE_TO_BELLOW_ASS------------------------------------------------------------------------------------
  TGeoBBox* rich_much_flange_box  = new TGeoBBox("rich_much_flange_box", 15.2, 15.2, 1.5);
  TGeoTube* rich_much_flange_hole = new TGeoTube("rich_much_flange_hole", 0., 12.5, 2.5);
  TGeoCompositeShape* rich_much_flange_composite =
    new TGeoCompositeShape("flange_composite_rich", "rich_much_flange_box-rich_much_flange_hole");
  TGeoVolume* rich_much_downstream_flange =
    new TGeoVolume("rich_much_downstream_flange", rich_much_flange_composite, CF);
  rich_much_downstream_flange->SetLineColor(kGray);

  // CONICAL_BEAM_PIPE WITH CARBONE-FIBRE FLANGE AND TILTED END PIECE OF TUBE TO STS SECTION------------------------------------------------------------------------------------
  TGeoCone* conus                      = new TGeoCone("conical_beam_pipe", conus_lenght, rmin1, rmax1, rmin2, rmax2);
  TGeoVolume* conical_beam_pipe_volume = new TGeoVolume("conical_beam_pipe_volume", conus, CF);
  conical_beam_pipe_volume->SetLineColor(kGray);

  TGeoCtub* rich_much_upstream_tube_connection =
    new TGeoCtub("rich_much_upstream_tube_connection", 55. * mm, 56. * mm, 15. * mm, 360., 360., 0.05235892, 0.,
                 4.99972585, 0., 0., 1.);
  TGeoVolume* rich_much_upstream_tube_connection_volume =
    new TGeoVolume("rich_much_upstream_tube_connection_volume", rich_much_upstream_tube_connection, CF);
  rich_much_upstream_tube_connection_volume->SetLineColor(kGray);

  TGeoCtub* rich_much_upstream_tube_connection_to_sts =
    new TGeoCtub("rich_much_upstream_tube_connection_to_sts", 55. * mm, 56. * mm, 15. * mm, 360., 360., 0., 0., 1.,
                 -0.05235892, 0., 4.99972585);
  TGeoVolume* rich_much_upstream_tube_connection_to_sts_volume =
    new TGeoVolume("rich_much_upstream_tube_connection_to_sts_volume", rich_much_upstream_tube_connection_to_sts, CF);
  rich_much_upstream_tube_connection_to_sts_volume->SetLineColor(kGray);

  //-------------------------------BELLOW DEFINITION----------------------------------------------------------------
  TGeoCompositeShape* flange_composite = new TGeoCompositeShape("flange_composite", "flange_box-flange_hole");
  TGeoVolume* bellow_flange_0          = new TGeoVolume("bellow_flange_0", flange_composite, Stainless);
  bellow_flange_0->SetLineColor(kRed);

  TGeoCtub* bellow_s1  = new TGeoCtub("bellow_s1", 124. * mm, 142. * mm, 29.87452196 * mm, 0., 360., -0.083697266, 0.,
                                     -0.996491228, -0.122217128, 0., 0.992503387);
  TGeoVolume* bellow_1 = new TGeoVolume("bellow_s1", bellow_s1, Stainless);
  bellow_1->SetLineColor(kRed);

  TGeoCtub* bellow_s2  = new TGeoCtub("bellow_s1", 124. * mm, 142. * mm, 96.682942995 * mm, 0., 360., -0.122217128, 0.,
                                     -0.992503387, 0.089898314, 0, 0.995950949);
  TGeoVolume* bellow_2 = new TGeoVolume("bellow_s2", bellow_s2, Stainless);
  bellow_2->SetLineColor(kRed);

  TGeoCtub* bellow_s3  = new TGeoCtub("bellow_s3", 124. * mm, 142. * mm, 22.10506784 * mm, 0., 360., -0.089898314, 0.,
                                     -0.995950949, -0.024210927, 0., 0.999706873);
  TGeoVolume* bellow_3 = new TGeoVolume("bellow_s3", bellow_s3, Stainless);
  bellow_3->SetLineColor(kRed);

  TGeoCtub* bellow_s4  = new TGeoCtub("bellow_s4", 124. * mm, 142. * mm, 25.30673722 * mm, 0., 360., -0.024210927, 0.,
                                     -0.999706873, -0.083697266, 0., 0.996491228);
  TGeoVolume* bellow_4 = new TGeoVolume("bellow_s3", bellow_s4, Stainless);
  bellow_4->SetLineColor(kRed);


  TGeoBBox* roll_plate_s = new TGeoBBox("roll_plate_s", 75. * mm, 12.5 * mm, 90. * mm);
  TGeoVolume* roll_plate = new TGeoVolume("roll_plate", roll_plate_s, Al);
  roll_plate->SetLineColor(kBlue);

  TGeoTube* flange_cyl                     = new TGeoTube("flange_cyl", 0., 15., 1.5);
  TGeoTube* flange_cyl_hole                = new TGeoTube("flange_cyl_hole", 0., 8.9, 2.5);
  TGeoCompositeShape* flange_cyl_composite = new TGeoCompositeShape(
    "flange_cyl_composite", "((flange_cyl-flange_cyl_hole)-roll_plate_s:tr_plate_up)-roll_plate_s:tr_plate_down");
  TGeoVolume* bellow_flange_1 = new TGeoVolume("bellow_flange_1", flange_cyl_composite, Stainless);
  bellow_flange_1->SetLineColor(kRed);


  TGeoTube* lead_screw_s = new TGeoTube("lead_screw_s", 0. * mm, 6. * mm, 205. * mm);
  TGeoTube* nut_small_s  = new TGeoTube("nut_small_s", 6. * mm, 14. * mm, 50. * mm);
  TGeoTube* nut_big_s    = new TGeoTube("nut_big_s", 14. * mm, 24. * mm, 27. * mm);

  TGeoCompositeShape* lead_screw_nut_s =
    new TGeoCompositeShape("lead_screw_nut_s", "lead_screw_s+nut_small_s:nut_transl+nut_big_s:nut_transl");
  TGeoVolume* lead_screw_nut = new TGeoVolume("lead_screw_nut", lead_screw_nut_s, Stainless);
  lead_screw_nut->SetLineColor(kRed);


  TGeoBBox* motor_s = new TGeoBBox("motor_s", 57.5 * mm, 30. * mm, 30. * mm);
  TGeoVolume* motor = new TGeoVolume("motor", motor_s, Stainless);
  motor->SetLineColor(kRed);


  TGeoTube* psd_tube_s = new TGeoTube("psd_tube_s", 90. * mm, 92. * mm, 7230. * mm);
  TGeoVolume* psd_tube = new TGeoVolume("psd_tube", psd_tube_s, CF);
  psd_tube->SetLineColor(kGray);

  //------------------TOP plate------------------------------
  bellow_ass->AddNode(sub_t_plate1, 1, tr_top_0a);
  bellow_ass->AddNode(sub_t_plate2, 1, tr_top_1a);
  bellow_ass->AddNode(sub_t_plate3, 1, tr_top_2a);
  bellow_ass->AddNode(sub_t_plate4, 1, tr_top_3a);
  bellow_ass->AddNode(sub_t_plate5, 1, tr_top_4a);
  bellow_ass->AddNode(sub_t_plate6, 1, tr_top_5a);

  //-----------------BOTTOM plate----------------------------
  bellow_ass->AddNode(sub_t_plate1, 2, tr_bott_0a);
  bellow_ass->AddNode(sub_t_plate2, 2, tr_bott_1a);
  bellow_ass->AddNode(sub_t_plate3, 2, tr_bott_2a);
  bellow_ass->AddNode(sub_t_plate4, 2, tr_bott_3a);
  bellow_ass->AddNode(sub_t_plate5, 2, tr_bott_4a);
  bellow_ass->AddNode(sub_t_plate6, 2, tr_bott_5a);

  //-----------------LEFT plate------------------------------
  bellow_ass->AddNode(sub_s_plate1, 1, tr_side_left_0a);
  bellow_ass->AddNode(sub_s_plate2, 1, tr_side_left_1a);
  bellow_ass->AddNode(sub_s_plate3, 1, tr_side_left_2a);
  bellow_ass->AddNode(sub_s_plate4, 1, tr_side_left_2a);
  bellow_ass->AddNode(sub_s_plate5, 1, tr_side_left_3a);
  bellow_ass->AddNode(sub_s_plate6, 1, tr_side_left_3a);
  bellow_ass->AddNode(sub_s_plate7, 1, tr_side_left_4a);
  bellow_ass->AddNode(sub_s_plate8, 1, tr_side_left_4a);
  bellow_ass->AddNode(sub_s_plate9, 1, tr_side_left_5a);
  bellow_ass->AddNode(sub_s_plate10, 1, tr_side_left_5a);

  //---------------RIGHT plate--------------------------------
  bellow_ass->AddNode(sub_s_plate1, 2, tr_side_right_0a);
  bellow_ass->AddNode(sub_s_plate2, 2, tr_side_right_1a);
  bellow_ass->AddNode(sub_s_plate3, 2, tr_side_right_2a);
  bellow_ass->AddNode(sub_s_plate4, 2, tr_side_right_2a);
  bellow_ass->AddNode(sub_s_plate5, 2, tr_side_right_3a);
  bellow_ass->AddNode(sub_s_plate6, 2, tr_side_right_3a);
  bellow_ass->AddNode(sub_s_plate7, 2, tr_side_right_4a);
  bellow_ass->AddNode(sub_s_plate8, 2, tr_side_right_4a);
  bellow_ass->AddNode(sub_s_plate9, 2, tr_side_right_5a);
  bellow_ass->AddNode(sub_s_plate10, 2, tr_side_right_5a);

  //------------------RAIL-------------------------------------
  bellow_ass->AddNode(sub_rail, 1, combi_rail_top);
  bellow_ass->AddNode(sub_rail, 2, combi_rail_bott);

  //----------------Bellow with flanges-----------------------------
  bellow_ass->AddNode(bellow_flange_0, 1, new TGeoTranslation(0, 0, 1.5));
  bellow_ass->AddNode(bellow_1, 1, ctr_s1);
  bellow_ass->AddNode(bellow_2, 1, ctr_s2);
  bellow_ass->AddNode(bellow_3, 1, ctr_s3);
  bellow_ass->AddNode(bellow_4, 1, ctr_s4);
  bellow_ass->AddNode(bellow_flange_1, 1, ctr_s5);

  //------------------DRIVE assembly--------------------------------------
  bellow_ass->AddNode(roll_plate, 1, ctr_plate_up);
  bellow_ass->AddNode(roll_plate, 2, ctr_plate_down);
  bellow_ass->AddNode(lead_screw_nut, 1, ctr_lsn);
  bellow_ass->AddNode(motor, 1, mot_transl);

  //-------------------CONICAL section--------------------------------------------------
  conical_beam_pipe->AddNode(rich_much_downstream_flange, 1, new TGeoTranslation(0, 0, -15 * mm));
  conical_beam_pipe->AddNode(conical_beam_pipe_volume, 1, new TGeoTranslation(0, 0, -1585 * mm));
  conical_beam_pipe->AddNode(rich_much_upstream_tube_connection_volume, 1, ctr_sts_conus_connection);
  conical_beam_pipe->AddNode(rich_much_upstream_tube_connection_to_sts_volume, 1, ctr_sts_tube);


  //------------------INSERT into top volume
  full_assembly->AddNode(conical_beam_pipe, 1, origin_combi);
  full_assembly->AddNode(bellow_ass, 1, origin_combi);
  full_assembly->AddNode(psd_tube, 1, ctr_psd_tube);

  //beampipe->AddNode(full_assembly,1, new TGeoTranslation(0, 0, 0*mm));
  // --- close the geometry
  geom->CloseGeometry();

  //--- draw the ROOT box
  geom->SetVisLevel(4);
  full_assembly->Draw();
  if (gPad && !gROOT->IsBatch()) gPad->GetViewer3D();

  // ---------------   Finish   -----------------------------------------------
  TString geo_bin_filename = "pipe_";
  TString geo_name         = geo_bin_filename + geoTag;
  geo_bin_filename         = geo_bin_filename + geoTag + ".geo.root";
  TFile* binfile;
  binfile = new TFile(geo_bin_filename, "RECREATE");
  geom->GetTopVolume()->Write(geo_name);  // outputting on the top volume
  TGeoTranslation* trans = new TGeoTranslation(0.0, 0.0, 82.0);
  trans->Write("trans");
  cout << endl;
  cout << "Geometry " << geom->GetTopVolume()->GetName() << " written to " << geo_bin_filename << std::endl;
  binfile->Close();
  //--- close the geometry
  geom->CloseGeometry();
}
