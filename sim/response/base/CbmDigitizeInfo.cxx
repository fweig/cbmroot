/** @file CbmDigitizeInfo.cxx
  * @copyright Copyright (C) 2018-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmDigitizeInfo.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 18.05.2018
 **/

#include "CbmDigitizeInfo.h"

#include <sstream>


// --- String output
std::string CbmDigitizeInfo::ToString() const
{
  std::stringstream ss;
  ss << "DigitizeInfo: System " << fSystem << ", present " << (fPresent ? "yes" : "no") << ", active "
     << (fActive ? "yes" : "no") << ", branch " << fBranch;
  return ss.str();
}


ClassImp(CbmDigitizeInfo)
