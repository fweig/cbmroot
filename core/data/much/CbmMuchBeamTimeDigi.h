/* Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mikhail Ryzhinskiy, Florian Uhlig [committer], Volker Friese, Pierre-Alain Loizeau, David Emschermann */

// TODO comment to be changed
/** CbmMuchBeamTimeDigi.h
 **@author M.Ryzhinskiy <m.ryzhinskiy@gsi.de>
 **@since 19.03.07
 **@version 1.0
 **
 **@author Vikas Singhal <vikas@vecc.gov.in>
 **@since 06.03.19
 **@version 2.0
 **
 ** Data class for digital MUCH information collected during BeamTime
 ** Data level: RAW
 ** To use reconstruction classes for CbmMuchBeamTimeDigi deriving it from CbmMuchDigi. VS
 **
 **
 **/


#ifndef CBMMUCHBEAMTIMEDIGI_H
#define CBMMUCHBEAMTIMEDIGI_H 1

#include "CbmMuchDigi.h"  // for CbmMuchDigi

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, ULong64_t

#include <string>  // for string

class CbmMuchBeamTimeDigi : public CbmMuchDigi {
public:
  CbmMuchBeamTimeDigi();
  CbmMuchBeamTimeDigi(Int_t address, Int_t charge = 0, ULong64_t time = 0);
  CbmMuchBeamTimeDigi(CbmMuchBeamTimeDigi* digi);
  CbmMuchBeamTimeDigi(const CbmMuchBeamTimeDigi&);
  CbmMuchBeamTimeDigi& operator=(const CbmMuchBeamTimeDigi&);


  virtual ~CbmMuchBeamTimeDigi() {}

  void SetPadX(Int_t padX) { fPadX = padX; }
  void SetPadY(Int_t padY) { fPadY = padY; }
  void SetRocId(Int_t rocId) { fRocId = rocId; }
  void SetNxId(Int_t nxId) { fNxId = nxId; }
  void SetNxCh(Int_t nxCh) { fNxCh = nxCh; }
  void SetElink(Int_t elink) { fElink = elink; }

  Int_t GetPadX() const { return fPadX; }
  Int_t GetPadY() const { return fPadY; }
  Int_t GetRocId() const { return fRocId; }
  Int_t GetNxId() const { return fNxId; }
  Int_t GetNxCh() const { return fNxCh; }
  Int_t GetElink() const { return fElink; }

  std::string ToString() const { return std::string {""}; }


  /** @brief Class name (static)
   ** @return CbmMuchBeamTimeDigi
   **/
  static const char* GetClassName() { return "CbmMuchBeamTimeDigi"; }


private:
  Int_t fPadX;
  Int_t fPadY;
  Int_t fRocId;
  Int_t fNxId;
  Int_t fNxCh;
  Int_t fElink;

  ClassDef(CbmMuchBeamTimeDigi, 3);
};
#endif
