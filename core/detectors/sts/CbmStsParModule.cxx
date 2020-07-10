/** @file CbmStsParModule.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 28.06.2014
 **/
#include "CbmStsParModule.h"

#include <cassert>   // for assert
#include <iostream>  // for operator<<, basic_ostream, stringstream
#include <sstream>   // for stringstream
#include <string>    // for char_traits

using std::string;
using std::stringstream;

ClassImp(CbmStsParModule)


  // -----   Constructor   ---------------------------------------------------
  CbmStsParModule::CbmStsParModule(UInt_t nChannels, UInt_t nAsicChannels)
  : fNofChannels(nChannels), fNofAsicChannels(nAsicChannels) {
  UInt_t nAsics = (nChannels % nAsicChannels ? nChannels / nAsicChannels + 1
                                             : nChannels / nAsicChannels);
  fAsicPars.resize(nAsics);
}
// -------------------------------------------------------------------------


// -----   Get ASIC parameters   -------------------------------------------
const CbmStsParAsic& CbmStsParModule::GetParAsic(UInt_t channel) const {
  assert(!fAsicPars.empty());
  assert(channel < fNofChannels);
  UInt_t nAsic = channel / fNofAsicChannels;
  assert(nAsic < GetNofAsics());
  return fAsicPars[nAsic];
}
// -------------------------------------------------------------------------


// -----   Check for a chennel being active   ------------------------------
Bool_t CbmStsParModule::IsChannelActive(UInt_t channel) const {
  const CbmStsParAsic& parAsic = GetParAsic(channel);
  UShort_t asicChannel         = channel % fNofAsicChannels;
  return parAsic.IsChannelActive(asicChannel);
}
// -------------------------------------------------------------------------


// -----   Set parameters for all ASICs   ----------------------------------
void CbmStsParModule::SetAllAsics(const CbmStsParAsic& asicPar) {
  for (auto& par : fAsicPars) {
    par = asicPar;
    par.Init();
  }
}
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
string CbmStsParModule::ToString() const {
  stringstream ss;
  ss << "Channels " << fNofChannels << " | ASICS " << GetNofAsics()
     << " | Channels per ASIC " << fNofAsicChannels;
  return ss.str();
}
// -------------------------------------------------------------------------
