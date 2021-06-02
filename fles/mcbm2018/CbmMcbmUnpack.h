/** @file CbmMcbmUnpack.h
  * @copyright Copyright (C) 2018-2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer], Florian Uhlig **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                            CbmMcbmUnpack                          -----
// -----               Created 09.07.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMMCBMUNPACK_H
#define CBMMCBMUNPACK_H

#include "Timeslice.hpp"

#include "TObject.h"

class CbmMcbmUnpack : public TObject {
public:
  CbmMcbmUnpack();
  virtual ~CbmMcbmUnpack();

  virtual Bool_t Init() = 0;

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component) = 0;

  virtual void Reset() = 0;

  virtual void Finish() = 0;

  virtual void SetParContainers() = 0;

  virtual Bool_t InitContainers() { return kTRUE; }

  virtual Bool_t ReInitContainers() { return kTRUE; }

  virtual void AddMsComponentToList(size_t component, UShort_t usDetectorId) = 0;
  virtual void SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb)            = 0;

  ClassDef(CbmMcbmUnpack, 0)
};

#endif
