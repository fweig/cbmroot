/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig, Denis Bertini [committer] */

/** CbmStsDigi.cxx
 ** @author V.Friese <v.friese@gsi.de>
 ** @author Felix Weiglhofer <weiglhofer@fias.uni-frankfurt.de>
 ** @since 28.08.2006
 ** @version 4.0
 **/

#include "CbmStsDigi.h"

#include "CbmStsAddress.h"

#include <algorithm>
#include <cstdlib>
#include <sstream>  // for operator<<, basic_ostream, char_traits

using std::string;
using std::stringstream;

// --- String output
string CbmStsDigi::ToString() const
{
  stringstream ss;
  ss << "StsDigi: address " << UnpackAddress() << " | channel " << UnpackChannel() << " | charge " << UnpackCharge()
     << " | time " << UnpackTime();
  return ss.str();
}

#ifndef NO_ROOT
ClassImp(CbmStsDigi)
#endif
