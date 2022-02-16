/* Copyright (C) 2022 UGiessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Adrian Weber [committer]*/

#include <TFile.h>
#include <TGeoMatrix.h>

#include <map>
#include <string>

std::pair<std::string, TGeoHMatrix> AlignNode(std::string path, double shiftX, double shiftY, double shiftZ,
                                              double rotX, double rotY, double rotZ)
{

  TGeoHMatrix result;
  result.SetDx(shiftX);
  result.SetDy(shiftY);
  result.SetDz(shiftZ);
  result.RotateX(rotX);
  result.RotateY(rotY);
  result.RotateZ(rotZ);

  std::cout << "Alignment matrix for node " << path << " is: " << std::endl;
  result.Print();
  std::cout << std::endl;

  return std::pair<std::string, TGeoHMatrix>(path, result);
}


int create_alignment_file()
{
  // Define the basic structure which needs to be filled with information
  // This structure is stored in the output file and later passed to the
  // FairRoot framework to do the (miss)alignment
  std::map<std::string, TGeoHMatrix> matrices;

  // ----------------  STS  ----------------------------//
  // Align full STS
  matrices.insert(AlignNode("/cave_1/sts_v22c_mcbm_0", 0.275, -0.89, -.5, 0., 0., 0.));

  // Align individual STS Units
  // Station 1
  // Unit 0
  matrices.insert(AlignNode("/cave_1/sts_v22c_mcbm_0/Station01_1/Ladder09_1", 0.0, 0.045, 0., 0., 0., 0.));
  // Unit 1
  matrices.insert(AlignNode("/cave_1/sts_v22c_mcbm_0/Station01_1/Ladder09_2", -0.04, 0.06, 0., 0., 0., 0.));

  // Station 2
  // Unit 2
  matrices.insert(AlignNode("/cave_1/sts_v22c_mcbm_0/Station02_2/Ladder10_2", 0.0, -0.11, 0., 0., 0., 0.));

  // ----------------  TOF  ----------------------------//
  // Align full Tof
  matrices.insert(AlignNode("/cave_1/tof_v21d_mcbm_0/tof_v21d_mcbmStand_1", 0.85, -1.05, 0.0, 0., 0.5, 0.));


  // save matrices to disk
  TFile* misalignmentMatrixRootfile = new TFile("AlignmentMatrices.root", "RECREATE");
  if (misalignmentMatrixRootfile->IsOpen()) {
    gDirectory->WriteObject(&matrices, "MisalignMatrices");
    misalignmentMatrixRootfile->Write();
    misalignmentMatrixRootfile->Close();
  }

  return 0;
}
