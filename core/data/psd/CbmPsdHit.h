/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alla Maevskaya, Volker Friese [committer] */

/** CbmPsdHit.h
 **@author Alla Maevskaya <alla@inr.ru>
 **@since 23.10.2012
 **@version 1.0
 **
 ** Data class for PSD reconstruction
 ** Energy deposition per module
 **
 ** Modified to simplify fEdep[49] -> fEdep (S. Seddiki)
 **/


#ifndef CBMPSDHIT_H
#define CBMPSDHIT_H 1

#include <Rtypes.h>      // for ClassDef
#include <TObject.h>     // for TObject

#include <cstdint>

class CbmPsdHit : public TObject {

public:
  /**   Default constructor   **/
  CbmPsdHit();

  CbmPsdHit(int32_t module, double edep);


  /**   Destructor   **/
  virtual ~CbmPsdHit();


  /**   Setters - Getters   **/

  //float GetEdep(int32_t module) const { return fEdep[module]; }      // SELIM: simplification vector [49] -> simple double
  //void SetEdep(float edep, int32_t module) {fEdep[module]=edep;}

  double GetEdep() const { return fEdep; }  // SELIM: simplification vector [49] -> simple double
  void SetEdep(double edep) { fEdep = edep; }

  int32_t GetModuleID() const { return fModuleID; }
  void SetModuleID(int32_t mod) { fModuleID = mod; }

  void Print(Option_t* = "") const;

private:
  /**   Data members  **/

  int32_t fModuleID;
  double fEdep;  //[49];    // SELIM: simplification vector [49] -> simple double


  ClassDef(CbmPsdHit, 1);
};


#endif
