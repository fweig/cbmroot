/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

Int_t loadlib(TString libname)
{
  Int_t retval = gSystem->Load(libname);
  return retval;
}
