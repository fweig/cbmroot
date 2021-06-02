/** @file CbmMuchModuleGemRadial.h
  * @copyright Copyright (C) 2011-2020 Petersburg Nuclear Physics Institute named by B.P.Konstantinov of National Research Centre "Kurchatov Institute", Gatchina
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Evgeny Kryshen [committer], Florian Uhlig **/

/** CbmMuchModuleGemRadial.h
 *@author  E.Kryshen <e.kryshen@gsi.de>
 *@version 1.0
 *@since   03.08.12
 */

#ifndef CBMMUCHMODULEGEMRADIAL_H
#define CBMMUCHMODULEGEMRADIAL_H 1

#include "CbmMuchModuleGem.h"  // for CbmMuchModuleGem

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t
#include <TVector3.h>    // for TVector3

#include <vector>  // for vector

class CbmMuchPadRadial;
class CbmMuchSectorRadial;

class CbmMuchModuleGemRadial : public CbmMuchModuleGem {
public:
  CbmMuchModuleGemRadial();
  // Detector type variable added in the class constructor on 03-07-2019
  CbmMuchModuleGemRadial(Int_t DetType, Int_t iStation, Int_t iLayer, Bool_t iSide, Int_t iModule, TVector3 position,
                         Double_t dx1, Double_t dx2, Double_t dy, Double_t dz, Double_t cutRadius);
  virtual ~CbmMuchModuleGemRadial() {}
  Double_t GetDx1() const { return fDx1; }
  Double_t GetDx2() const { return fDx2; }
  Double_t GetDy() const { return fDy; }
  Double_t GetDz() const { return fDz; }
  CbmMuchSectorRadial* GetSectorByRadius(Double_t r);
  CbmMuchPadRadial* GetPad(Double_t x, Double_t y);
  Bool_t InitModule();

protected:
  std::vector<Double_t> fSectorRadii;  //! vector of sector radii

  Double_t fDx1;
  Double_t fDx2;
  Double_t fDy;
  Double_t fDz;
  Int_t fDetType;
  ClassDef(CbmMuchModuleGemRadial, 2);
};
#endif
