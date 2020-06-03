// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                       CbmROCUnpackSystemMessage                   -----
// -----                    Created 10.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMROCUNPACKSYSTEMMESSAGE_H
#define CBMROCUNPACKSYSTEMMESSAGE_H

#include <Rtypes.h>        // ClassDef
#include <RtypesCore.h>    // for Bool_t, ULong_t
#include "CbmROCUnpack.h"  // for CbmROCUnpack
#include <boost/any.hpp>

class CbmSourceLmdNew;
class CbmTbDaqBuffer;

namespace roc { class Message; }

class CbmTbDaqBuffer;
class CbmSourceLmdNew;
class CbmROCUnpackSystemMessage : public CbmROCUnpack
{
 public:
  
  CbmROCUnpackSystemMessage();
  virtual ~CbmROCUnpackSystemMessage();
    
  virtual Bool_t Init();
  virtual Bool_t DoUnpack(roc::Message* Message, ULong_t hitTime);
  virtual void FillOutput(boost::any) {;}
  virtual void Reset();
  virtual void Finish() {;}
  
 private:

  CbmTbDaqBuffer* fBuffer;
  CbmSourceLmdNew* fSource;

  CbmROCUnpackSystemMessage(const CbmROCUnpackSystemMessage&);
  CbmROCUnpackSystemMessage& operator=(const CbmROCUnpackSystemMessage&);
  
  ClassDef(CbmROCUnpackSystemMessage, 1)
};

#endif
