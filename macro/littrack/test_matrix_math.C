/* Copyright (C) 2011 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

void test_matrix_math()
{
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/cbmrootlibs.C");
  cbmrootlibs();
  gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/determine_setup.C");

  CbmLitTestMatrixMath* test = new CbmLitTestMatrixMath();
  test->RunTest();
}
