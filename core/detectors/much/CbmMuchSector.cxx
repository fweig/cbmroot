/** @file CbmMuchSector.cxx
  * @copyright Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Evgeny Kryshen, Florian Uhlig, Volker Friese [committer] **/

#include "CbmMuchSector.h"

#include <Logger.h>  // for Logger, LOG

#include <vector>  // for vector

using std::vector;

// -----   Default constructor   -------------------------------------------
CbmMuchSector::CbmMuchSector() : fAddress(0), fNChannels(0), fPads() {}
// -------------------------------------------------------------------------

// -----  Standard constructor  --------------------------------------------
CbmMuchSector::CbmMuchSector(UInt_t modAddress, UInt_t index, Int_t nChannels)
  : fAddress(CbmMuchAddress::SetElementId(modAddress, kMuchSector, index))
  , fNChannels(nChannels)
  , fPads()
{
  fPads.resize(nChannels);
}
// -------------------------------------------------------------------------

CbmMuchPad* CbmMuchSector::GetPadByChannelIndex(Int_t iChannel) const
{
  //  LOG(debug) << "iChannel=" << iChannel << " fPads.size()=" << fPads.size()
  //             << " fNChannels=" << fNChannels;
  if (iChannel >= static_cast<Int_t>(fPads.size()) || iChannel < 0) {
    LOG(error) << "iChannel=" << iChannel << " fPads.size()=" << fPads.size();
    LOG(error) << "  station index=" << CbmMuchAddress::GetStationIndex(fAddress);
    LOG(error) << "    layer index=" << CbmMuchAddress::GetLayerIndex(fAddress);
    LOG(error) << "   module index=" << CbmMuchAddress::GetModuleIndex(fAddress);
    LOG(error) << "   sector index=" << CbmMuchAddress::GetSectorIndex(fAddress);
    return nullptr;
  }
  return fPads[iChannel];
}


//// -------------------------------------------------------------------------
//void CbmMuchSector::DrawPads(){
//  for(Int_t iChannel = 0; iChannel < fNChannels; iChannel++){
//     CbmMuchPad* pad = fPads[iChannel];
////     pad->Draw("f");
////     pad->Draw();
//  }
//}
//// -------------------------------------------------------------------------
ClassImp(CbmMuchSector)
