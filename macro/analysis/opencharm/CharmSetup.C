/** @file CharmSetup.C
  * @copyright Copyright (C) 2015-2021 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Philipp Sitzmann [committer] **/

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
