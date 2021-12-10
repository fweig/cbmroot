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

#include <cstdint>
#include <string>  // for string

class CbmMuchBeamTimeDigi : public CbmMuchDigi {
public:
  CbmMuchBeamTimeDigi();
  CbmMuchBeamTimeDigi(int32_t address, int32_t charge = 0, uint64_t time = 0);
  CbmMuchBeamTimeDigi(CbmMuchBeamTimeDigi* digi);
  CbmMuchBeamTimeDigi(const CbmMuchBeamTimeDigi&);
  CbmMuchBeamTimeDigi& operator=(const CbmMuchBeamTimeDigi&);


  virtual ~CbmMuchBeamTimeDigi() {}

  void SetPadX(int32_t padX) { fPadX = padX; }
  void SetPadY(int32_t padY) { fPadY = padY; }
  void SetRocId(int32_t rocId) { fRocId = rocId; }
  void SetNxId(int32_t nxId) { fNxId = nxId; }
  void SetNxCh(int32_t nxCh) { fNxCh = nxCh; }
  void SetElink(int32_t elink) { fElink = elink; }

  int32_t GetPadX() const { return fPadX; }
  int32_t GetPadY() const { return fPadY; }
  int32_t GetRocId() const { return fRocId; }
  int32_t GetNxId() const { return fNxId; }
  int32_t GetNxCh() const { return fNxCh; }
  int32_t GetElink() const { return fElink; }

  std::string ToString() const { return std::string {""}; }


  /** @brief Class name (static)
   ** @return CbmMuchBeamTimeDigi
   **/
  static const char* GetClassName() { return "CbmMuchBeamTimeDigi"; }


private:
  int32_t fPadX;
  int32_t fPadY;
  int32_t fRocId;
  int32_t fNxId;
  int32_t fNxCh;
  int32_t fElink;

  ClassDef(CbmMuchBeamTimeDigi, 3);
};
#endif
