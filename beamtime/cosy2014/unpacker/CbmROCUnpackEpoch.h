// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmROCUnpackEpoch                        -----
// -----                    Created 13.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMROCUNPACKEPOCH_H
#define CBMROCUNPACKEPOCH_H

#include <Rtypes.h>        // for ClassDef
#include <RtypesCore.h>    // for Bool_t, Int_t, ULong_t
#include "CbmROCUnpack.h"
#include <boost/any.hpp>

namespace roc { class Message; }

class CbmSourceLmdNew;

class CbmROCUnpackEpoch : public CbmROCUnpack
{
 public:
  
  CbmROCUnpackEpoch();
  virtual ~CbmROCUnpackEpoch();
    
  virtual Bool_t Init();
  virtual Bool_t DoUnpack(roc::Message* Message, ULong_t hitTime);
  virtual void FillOutput(boost::any) {;}
  virtual void Reset();
  virtual void Finish() {;}

 private:

  CbmSourceLmdNew* fSource;
  Int_t fNofEpochs;

  CbmROCUnpackEpoch(const CbmROCUnpackEpoch&);
  CbmROCUnpackEpoch& operator=(const CbmROCUnpackEpoch&);
  
  ClassDef(CbmROCUnpackEpoch, 1)
};

#endif
