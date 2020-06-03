// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmROCUnpackSts                         -----
// -----                    Created 10.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMROCUNPACKSTS_H
#define CBMROCUNPACKSTS_H

#include "roc/Message.h"

#include "CbmROCUnpack.h"
#include <boost/any.hpp>
#include <vector>
#include "CbmStsDigi.h"

namespace roc { class Message; }

class CbmTbDaqBuffer;
class CbmDaqMap;
class CbmSourceLmdNew;

class CbmROCUnpackSts : public CbmROCUnpack
{
 public:
  
  CbmROCUnpackSts();
  virtual ~CbmROCUnpackSts();
    
  virtual Bool_t Init();
  virtual Bool_t DoUnpack(roc::Message* Message, ULong_t hitTime);
  virtual void FillOutput(boost::any);
  virtual void Reset();
  virtual void Finish() {;}
 private:

  CbmTbDaqBuffer* fBuffer;        ///< Digi buffer instance
  CbmDaqMap* fDaqMap;             ///< Mapping from electronics to detectors
  CbmSourceLmdNew* fSource;
  std::vector<CbmStsDigi>* fStsDigis;          ///< Output array of CbmStsDigi
  std::vector<CbmStsDigi>* fStsBaselineDigis;  ///< Output array for baseline calib.

  CbmROCUnpackSts(const CbmROCUnpackSts&);
  CbmROCUnpackSts& operator=(const CbmROCUnpackSts&);
  
  ClassDef(CbmROCUnpackSts, 2)
};

#endif
