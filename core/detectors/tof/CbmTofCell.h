/** @file CbmTofCell.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef CBMTOFCELL_H
#define CBMTOFCELL_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Option_t
#include <TNamed.h>      // for TNamed

class CbmTofCell : public TNamed {

public:
  /** Default constructor **/
  CbmTofCell();


  /** Standard constructor
   *@param detId     Unique detector identifier
   *@param x0        cell centre x coordinate in global c.s [cm]
   *@param y0        cell centre y coordinate in global c.s [cm]
   *@param z0        cell centre z coordinate in global c.s [cm]
   *@param sizex     cell size in x [cm]
   *@param sizey     cell size in y [cm]
   **/

  CbmTofCell(Int_t detId, Double_t x, Double_t y, Double_t z, Double_t sizex, Double_t sizey);

  /** Destructor **/
  virtual ~CbmTofCell();

  /** Accessors **/
  Int_t GetDetectorId() const { return fDetectorId; }
  Double_t GetX() const { return fX; }
  Double_t GetY() const { return fY; }
  Double_t GetZ() const { return fZ; }

  Double_t GetSizex() const { return fSizex; }
  Double_t GetSizey() const { return fSizey; }

  void Print(Option_t* = "") const;

private:
  /** -------------   Data members   --------------------------**/

  Int_t fDetectorId;  // Unique detector ID
  Double_t fX;        // center of module in global c.s. [cm]
  Double_t fY;        // center of module in global c.s. [cm]
  Double_t fZ;        // center of module in global c.s. [cm]
  Double_t fSizex;    // module size in x [cm]
  Double_t fSizey;    // module size in y [cm]

  /** --------------- private functions ----------------------**/


  ClassDef(CbmTofCell, 1);
};


#endif
