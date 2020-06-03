// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmTSUnpackFiberHodo                          -----
// -----                    Created 07.11.2014 by                          -----
// -----                   E. Ovcharenko, F. Uhlig                         -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTSUNPACKFIBERHODO_H
#define CBMTSUNPACKFIBERHODO_H

#include "Timeslice.hpp"

#include "CbmTSUnpack.h"

#include "CbmNxyterRawMessage.h"
#include "CbmFiberHodoDigi.h"
#include "CbmTbDaqBuffer.h"
#include "TClonesArray.h"

class CbmFiberHodoMapping;

class CbmTSUnpackFiberHodo : public CbmTSUnpack
{
 public:

  CbmTSUnpackFiberHodo();
  virtual ~CbmTSUnpackFiberHodo();

  virtual Bool_t Init();

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

  virtual void Reset();

  virtual void Finish() {;}

  void SetParContainers() {;}
  // protected:
  //  virtual void Register();

  virtual void FillOutput(boost::any){;}

 private:
  Int_t fCurrEpoch; // Current epoch (first epoch in the stream initialises the 

  TClonesArray* fFiberHodoRaw;
  TClonesArray* fFiberHodoDigi;
  CbmNxyterRawMessage* fRawMessage;  
  CbmFiberHodoDigi* fDigi;  
  CbmFiberHodoMapping* fHodoMap;
  CbmTbDaqBuffer* fBuffer;

  void FillHodoDigiToBuffer();

  CbmTSUnpackFiberHodo(const CbmTSUnpackFiberHodo&);
  CbmTSUnpackFiberHodo operator=(const CbmTSUnpackFiberHodo&);

  ClassDef(CbmTSUnpackFiberHodo, 1)
};

#endif
