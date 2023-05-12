/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#include "Options.h"
#include "Reco.h"

int main(int argc, char** argv)
{
  cbm::algo::Options opts {argc, argv};

  cbm::algo::Reco reco {};
  reco.Init(opts);
  reco.Run();
  reco.Finalize();

  return 0;
}
