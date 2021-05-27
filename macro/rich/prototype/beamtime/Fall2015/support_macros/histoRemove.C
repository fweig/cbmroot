/** @file histoRemove.C
  * @copyright Copyright (C) 2015 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Egor Ovcharenko [orginator] **/

void histoRemove(TString filename, TString histoname)
{
  TFile f(filename, "UPDATE");
  f.Delete(histoname);
  f.Close();
}
