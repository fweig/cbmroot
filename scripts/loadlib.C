/** @file loadlib.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

Int_t loadlib(TString libname)
{
  Int_t retval = gSystem->Load(libname);
  return retval;
}
