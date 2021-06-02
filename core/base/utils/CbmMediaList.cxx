/** @file CbmMediaList.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmMediaList.h"

void CbmMediaList::AddEntry(TString path, TString medium) { fMatList.push_back(std::make_pair(path, medium)); }

ClassImp(CbmMediaList)
