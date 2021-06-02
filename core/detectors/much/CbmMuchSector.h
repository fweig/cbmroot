/** @file CbmMuchSector.h
  * @copyright Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Evgeny Kryshen, Volker Friese [committer], Florian Uhlig **/

/** CbmMuchSector.h
 *@author  E.Kryshen <e.kryshen@gsi.de>
 *@since   02.08.12
 *@version 2.0
 **
 ** This class describes the digitization scheme for a sector of the MuCh.
 ** The sector is a rectangle of size fLx and fLy.
 **
 **/
#ifndef CBMMUCHSECTOR_H
#define CBMMUCHSECTOR_H 1

#include "CbmMuchAddress.h"  // for CbmMuchAddress

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for UInt_t, Int_t

#include <vector>  // for vector

class CbmMuchPad;

class CbmMuchSector {
public:
  CbmMuchSector();
  CbmMuchSector(UInt_t modAddress, UInt_t index, Int_t nChannels);
  virtual ~CbmMuchSector() {}
  UInt_t GetAddress() const { return fAddress; }
  UInt_t GetSectorIndex() const { return CbmMuchAddress::GetSectorIndex(fAddress); }
  Int_t GetNChannels() const { return fNChannels; }
  CbmMuchPad* GetPadByChannelIndex(Int_t iChannel) const;
  //  virtual void GetPadVertices(Int_t iChannel, Double_t* xPad, Double_t* yPad) {}
  virtual void AddPads() {}
  virtual void DrawPads() {}

protected:
  UInt_t fAddress;                 //  Sector address
  Int_t fNChannels;                //  Number of channels
  std::vector<CbmMuchPad*> fPads;  //! Vector of pads

  ClassDef(CbmMuchSector, 2);
};

#endif
