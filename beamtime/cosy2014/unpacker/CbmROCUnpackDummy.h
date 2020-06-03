// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmROCUnpackDummy                        -----
// -----                    Created 14.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMROCUNPACKDUMMY_H
#define CBMROCUNPACKDUMMY_H

#include <Rtypes.h>        // for ClassDef
#include <RtypesCore.h>    // for Bool_t, ULong_t
#include "CbmROCUnpack.h"  // for CbmROCUnpack
#include <boost/any.hpp>

namespace roc { class Message; }

class CbmROCUnpackDummy : public CbmROCUnpack
{
 public:
  
  CbmROCUnpackDummy();
  virtual ~CbmROCUnpackDummy();
    
  virtual Bool_t Init();
  virtual Bool_t DoUnpack(roc::Message* Message, ULong_t hitTime);
  virtual void FillOutput(boost::any) {;}
  virtual void Reset();
  virtual void Finish() {;}

 private:

  ClassDef(CbmROCUnpackDummy, 1)
};

#endif
