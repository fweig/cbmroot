// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmTSUnpackDummy                         -----
// -----                    Created 21.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTSUNPACKDUMMY_H
#define CBMTSUNPACKDUMMY_H

#include "Timeslice.hpp"


#include "CbmTSUnpack.h"

class CbmTSUnpackDummy : public CbmTSUnpack
{
 public:
  
  CbmTSUnpackDummy();
  virtual ~CbmTSUnpackDummy();
    
  virtual Bool_t Init() { return kTRUE; }

  virtual Bool_t DoUnpack(const fles::Timeslice& /*ts*/, size_t /*component*/) 
  { return kTRUE; }

  virtual void Reset() { return; }

  virtual void Finish() { return; }

  void SetParContainers() {;}

  virtual void FillOutput(boost::any){;}

  ClassDef(CbmTSUnpackDummy, 1)
};

#endif
