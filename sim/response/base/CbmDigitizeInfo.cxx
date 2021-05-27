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
