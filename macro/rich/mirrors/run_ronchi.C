/* Copyright (C) 2018-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Semen Lebedev, Egor Ovcharenko */

void run_ronchi()
{
  //gSystem->Load("libCbmRichMirrors");
  //gSystem->Load("libtiff");

  std::string dir       = "/dir/";
  std::string fileName1 = "horizontal_191111.tif";
  std::string fileName2 = "vertical_191111.tif";

  CbmRichRonchiAna* ronchiAna = new CbmRichRonchiAna();
  ronchiAna->SetTiffFileNameH(fileName1);
  ronchiAna->SetTiffFileNameV(fileName2);
  ronchiAna->Run();
}
