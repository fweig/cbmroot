// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                       CbmUnpackTaskBase                           -----
// -----               Created 13.02.2020 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMUNPACKTASKBASE_H
#define CBMUNPACKTASKBASE_H

/// CbmRoot (+externals) headers
#include "Timeslice.hpp"

/// FairRoot headers

/// Fairsoft (Root, Boost, ...) headers
#include "TObject.h"

/// C/C++ headers

class CbmUnpackTaskBase : public TObject
{
 public:

  CbmUnpackTaskBase();
  virtual ~CbmUnpackTaskBase();

  virtual Bool_t Init() = 0;

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component) = 0;

  virtual void Reset() = 0;

  virtual void Finish() = 0;

  virtual void SetParContainers() = 0;

  virtual Bool_t InitContainers() {return kTRUE;}

  virtual Bool_t ReInitContainers() {return kTRUE;}

  virtual void AddMsComponentToList( size_t component, UShort_t usDetectorId ) = 0;
  virtual void SetNbMsInTs( size_t uCoreMsNb, size_t uOverlapMsNb ) = 0;

};

#endif // CBMUNPACKTASKBASE_H
