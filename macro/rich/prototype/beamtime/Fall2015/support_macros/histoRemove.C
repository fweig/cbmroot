/* Copyright (C) 2015 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Egor Ovcharenko [committer] */

void histoRemove(TString filename, TString histoname)
{
  TFile f(filename, "UPDATE");
  f.Delete(histoname);
  f.Close();
}
