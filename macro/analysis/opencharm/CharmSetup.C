/* Copyright (C) 2015-2021 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

void CharmSetup()
{
  // Input Parameter
  input    = "nini";
  inputGEV = "15gev";
  system   = "centr";
  signal   = "d0";  // "dminus" "dplus" "d0_4B"
  iVerbose = 0;
  setup    = "sis100_electron";

  littrack = false;
  useMC    = kFALSE;
}
