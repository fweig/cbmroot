/* Copyright (C) 2008-2011 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file draw_dedx.C
 * \brief Macro draws energy losses.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 **/

void draw_dedx()
{
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/cbmrootlibs.C");
  cbmrootlibs();

  CbmLitCheckEnergyLossMuons checker;
  checker.SetMaterial("iron");
  checker.Check();

  //	CbmLitCheckBrem checker;
  //	checker.SetMaterial("iron");
  //	checker.Check();
}
