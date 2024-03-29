/* Copyright (C) 2008-2020 St. Petersburg Polytechnic University, St. Petersburg
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Evgeny Kryshen, Mikhail Ryzhinskiy [committer], Florian Uhlig */

/** CbmMuchModuleGem.h
 *
 *@author  M.Ryzhinskiy <m.ryzhinskiy@gsi.de>
 *@version 1.0
 *@since   11.02.08
 *
 * This class holds geometry parameters of much modules
 */

#ifndef CBMMUCHMODULEGEM_H
#define CBMMUCHMODULEGEM_H 1

#include "CbmMuchModule.h"  // for CbmMuchModule

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Bool_t, Color_t, Double_t, kTRUE
#include <TVector3.h>    // for TVector3

#include <vector>  // for vector

class CbmMuchPad;
class CbmMuchSector;

class CbmMuchModuleGem : public CbmMuchModule {
public:
  /** Default constructor **/
  CbmMuchModuleGem();

  /** Standard constructor
   *@param iStation  Station index
   *@param iLayer    Layer index
   *@param iSide     Defines side of the layer (0 - Front, 1 - Back)
   *@param iModule   Module index
   *@param position  Location of the module center in global c.s. (all dimensions in [cm])
   *@param cutRadius Radius of the cut (if any, otherwise = -1.) [cm]
   **/
  CbmMuchModuleGem(Int_t iStation, Int_t iLayer, Bool_t iSide, Int_t iModule, TVector3 position, TVector3 size,
                   Double_t cutRadius);

  /** Destructor */
  virtual ~CbmMuchModuleGem() {}

  /** Gets sector by the given address */
  CbmMuchSector* GetSector(Int_t address);

  /** Gets pad by the given address */
  CbmMuchPad* GetPad(Int_t address);

  CbmMuchSector* GetSectorByIndex(Int_t iSector) { return fSectors[iSector]; }

  /** */
  Int_t GetNSectors() const { return fSectors.size(); }

  /** Gets array of pads for this module. */
  std::vector<CbmMuchPad*> GetPads();

  /** */
  Int_t GetNPads();

  /** Adds a given sector to the array.
   *@param  sector CbmMuchSector which should be added to the array.**/
  void AddSector(CbmMuchSector* sector) { fSectors.push_back(sector); }

  /** */
  virtual Bool_t InitModule() { return kTRUE; }

  /** */
  void DrawModule(Color_t color);

  void DrawPads();
  void SetPadFired(Int_t address, Int_t digiIndex, Int_t adcCharge);

protected:
  std::vector<CbmMuchSector*> fSectors;  // Array of sectors within this module

  ClassDef(CbmMuchModuleGem, 2);
};
#endif
