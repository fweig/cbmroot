/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig, Denis Bertini [committer] */

/** CbmStsDigi.cxx
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 28.08.2006
 ** @version 4.0
 **/

#include "CbmStsDigi.h"

#include <sstream>  // for operator<<, basic_ostream, char_traits

using std::string;
using std::stringstream;

// --- String output
string CbmStsDigi::ToString() const
{
  stringstream ss;
  ss << "StsDigi: address " << fAddress << " | channel " << fChannel << " | charge " << fCharge << " | time " << fTime;
  return ss.str();
}

#ifndef NO_ROOT
ClassImp(CbmStsDigi)
#endif
