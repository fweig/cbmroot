/* Copyright (C) 2015 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "FileListDef.h"

Bool_t loopTreeConv(Int_t iSecOffset = 0, Int_t iMilliSecOffset = 0)
{

  for (Int_t iFileIndex = 0; iFileIndex < kiNbFiles; iFileIndex++)
    gROOT->ProcessLine(Form(".x currentTreeConv.C+( %d, %d, %d)", iFileIndex, iSecOffset, iMilliSecOffset));

  cout << "Finished creating Root files, now merge them" << endl;

  // For now offsets are not used, probably better to shift it to the previous loop
  gROOT->ProcessLine(".x mergeTrees.C()");

  return kTRUE;
}
