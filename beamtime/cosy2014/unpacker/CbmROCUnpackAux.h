// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                            CbmROCUnpackAux                        -----
// -----                    Created 14.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMROCUNPACKAUX_H
#define CBMROCUNPACKAUX_H

#include <Rtypes.h>        // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>    // for Bool_t, ULong_t
#include "CbmROCUnpack.h"  // for CbmROCUnpack
#include <boost/any.hpp>
#include <vector>
#include "CbmAuxDigi.h"

class CbmSourceLmdNew;
class CbmTbDaqBuffer;

namespace roc { class Message; }

class CbmROCUnpackAux : public CbmROCUnpack
{
 public:
  
  CbmROCUnpackAux();
  virtual ~CbmROCUnpackAux();
    
  virtual Bool_t Init();
  virtual Bool_t DoUnpack(roc::Message* Message, ULong_t hitTime);
  virtual void FillOutput(boost::any);
  virtual void Reset();
  virtual void Finish() {;}

 private:

  CbmSourceLmdNew* fSource;
  CbmTbDaqBuffer* fBuffer;
  std::vector<CbmAuxDigi>* fAuxDigis;          ///< Output array of CbmAuxDigi
  
  CbmROCUnpackAux(const CbmROCUnpackAux&);
  CbmROCUnpackAux& operator=(const CbmROCUnpackAux&);

  ClassDef(CbmROCUnpackAux, 2)
};

#endif
